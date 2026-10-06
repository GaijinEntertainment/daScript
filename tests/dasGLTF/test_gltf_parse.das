options gen2
options indenting = 4

require dastest/testing_boost public
require gltf/gltf_boost
require math
require daslib/fio
require strings

def private model_path(name : string) : string => "{get_das_root()}/tests/dasGLTF/models/{name}"

// read a JSON fixture, inject `inject` right after the opening '{', parse from memory
def private load_patched(name : string; inject : string) : GltfScene {
    var bytes : array<uint8>
    fopen(model_path(name), "rb") $(f) {
        if (f != null) {
            fmap(f) $(var data : array<uint8>#) {
                bytes := data
            }
        }
    }
    let text = string(bytes)
    delete bytes
    let patched = "\{{inject}{slice(text, 1)}"
    var pbytes : array<uint8>
    peek_data(patched) $(arr) {
        pbytes := arr
    }
    var scene <- load_gltf_from_memory(pbytes, "{get_das_root()}/tests/dasGLTF/models")
    delete pbytes
    return <- scene
}

[test]
def test_missing_file(t : T?) {
    t |> run("missing file yields empty scene") @(t : T?) {
        var scene <- load_gltf("nonexistent.gltf")
        t |> equal(length(scene.meshes), 0)
        t |> equal(length(scene.nodes), 0)
        delete scene
    }
}

[test]
def test_box_glb(t : T?) {
    // GLB container (12-byte header + JSON/BIN chunks); tightly-packed VEC3 f32
    // POSITION/NORMAL; SCALAR u16 indices; node `matrix` (column-major).
    t |> run("Box.glb") @(t : T?) {
        var scene <- load_gltf(model_path("Box.glb"))
        t |> equal(length(scene.meshes), 1)
        t |> equal(length(scene.nodes), 2)
        t |> equal(length(scene.roots), 1)
        t |> equal(scene.roots[0], 0)
        t |> equal(length(scene.meshes[0].primitives), 1)
        t |> equal(length(scene.meshes[0].primitives[0].vertices), 24)
        t |> equal(length(scene.meshes[0].primitives[0].indices), 36)
        t |> equal(scene.meshes[0].primitives[0].material, 0)
        t |> equal(scene.meshes[0].primitives[0].vertices[0].position, float3(-0.5, -0.5, 0.5))
        t |> equal(scene.meshes[0].primitives[0].vertices[0].normal, float3(0., 0., 1.))
        t |> equal(scene.meshes[0].primitives[0].indices[0], 0u)
        t |> equal(scene.meshes[0].primitives[0].indices[1], 1u)
        t |> equal(scene.meshes[0].primitives[0].indices[2], 2u)
        t |> equal(length(scene.materials), 1)
        t |> equal(scene.materials[0].baseColorFactor, float4(0.8, 0., 0., 1.))
        t |> equal(scene.materials[0].metallicFactor, 0.)
        // node 0 carries a column-major rotation matrix; col1 = (0,0,-1,0)
        t |> equal(scene.nodes[0].local[1], float4(0., 0., -1., 0.))
        // node 1 is node 0's child with identity local; world translation stays at origin
        t |> equal(scene.nodes[1].world[3], float4(0., 0., 0., 1.))
        delete scene
    }
}

[test]
def test_box_interleaved(t : T?) {
    // Same geometry as Box but interleaved (bufferView byteStride 24, POSITION at
    // byteOffset 12) — decoded values must match the non-interleaved Box exactly.
    t |> run("BoxInterleaved.glb") @(t : T?) {
        var scene <- load_gltf(model_path("BoxInterleaved.glb"))
        t |> equal(length(scene.meshes[0].primitives[0].vertices), 24)
        t |> equal(length(scene.meshes[0].primitives[0].indices), 36)
        t |> equal(scene.meshes[0].primitives[0].vertices[0].position, float3(-0.5, -0.5, 0.5))
        t |> equal(scene.meshes[0].primitives[0].vertices[0].normal, float3(0., 0., 1.))
        t |> equal(scene.meshes[0].primitives[0].vertices[1].position, float3(0.5, -0.5, 0.5))
        delete scene
    }
}

[test]
def test_extensions_gate(t : T?) {
    // extensionsRequired naming an unsupported extension (e.g. Draco) must fail
    // closed - empty scene + error log - instead of silently-degenerate geometry;
    // the same extension under extensionsUsed only (optional) must still load.
    t |> run("unsupported extensionsRequired yields empty scene") @(t : T?) {
        var scene <- load_patched("SimpleSkin.gltf", "\"extensionsRequired\":[\"KHR_draco_mesh_compression\"],")
        t |> equal(length(scene.meshes), 0)
        t |> equal(length(scene.nodes), 0)
        delete scene
    }
    t |> run("unsupported extensionsUsed (optional) still loads") @(t : T?) {
        var scene <- load_patched("SimpleSkin.gltf", "\"extensionsUsed\":[\"KHR_texture_transform\"],")
        t |> equal(length(scene.meshes), 1)
        t |> equal(length(scene.nodes), 3)
        delete scene
    }
}

[test]
def test_simple_skin(t : T?) {
    // base64-embedded .gltf container; integer-lane VEC4 u16 JOINTS_0 -> float4;
    // strided WEIGHTS_0; MAT4 inverse-bind; skin + animation; TRS node hierarchy.
    t |> run("SimpleSkin.gltf") @(t : T?) {
        var scene <- load_gltf(model_path("SimpleSkin.gltf"))
        t |> equal(length(scene.meshes), 1)
        t |> equal(length(scene.nodes), 3)
        t |> equal(length(scene.skins), 1)
        t |> equal(length(scene.animations), 1)
        t |> equal(length(scene.meshes[0].primitives[0].vertices), 10)
        t |> equal(length(scene.meshes[0].primitives[0].indices), 24)
        t |> equal(scene.meshes[0].primitives[0].vertices[0].position, float3(-0.5, 0., 0.))
        // integer-lane JOINTS_0 (u16 VEC4) decoded to float, WEIGHTS_0 alongside
        t |> equal(scene.meshes[0].primitives[0].vertices[0].joints0, float4(0., 0., 0., 0.))
        t |> equal(scene.meshes[0].primitives[0].vertices[0].weights0, float4(1., 0., 0., 0.))
        t |> equal(scene.meshes[0].primitives[0].vertices[5].joints0, float4(0., 1., 0., 0.))
        t |> equal(scene.meshes[0].primitives[0].vertices[5].weights0, float4(0.5, 0.5, 0., 0.))
        // skin: two joints, one inverse-bind MAT4 each
        t |> equal(length(scene.skins[0].joints), 2)
        t |> equal(scene.skins[0].joints[0], 1)
        t |> equal(scene.skins[0].joints[1], 2)
        t |> equal(length(scene.skins[0].inverseBind), 2)
        // animation: one rotation channel, 12 keyframes, duration 5.5s
        t |> equal(length(scene.animations[0].samplers), 1)
        t |> equal(length(scene.animations[0].channels), 1)
        t |> equal(length(scene.animations[0].samplers[0].input), 12)
        t |> equal(length(scene.animations[0].samplers[0].output), 12)
        t |> equal(scene.animations[0].duration, 5.5)
        // node hierarchy: node 2 (translate 0,1,0) is node 1's child
        t |> equal(scene.nodes[2].translation, float3(0., 1., 0.))
        t |> equal(scene.nodes[2].world[3], float4(0., 1., 0., 1.))
        delete scene
    }
}
