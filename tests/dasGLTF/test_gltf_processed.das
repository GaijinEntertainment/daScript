options gen2
options persistent_heap
require dastest/testing_boost public
require gltf/gltf_processed
require daslib/fio
require math
[test]
def test_processed_files_preserve_geometry_and_animation(t : T?){
    let directory = create_temp_directory_result("gltf_processed")
    if (!(directory is value)){t |> failure("create processed asset directory"); return }
    let work_dir = unsafe(directory.value)
    let root = "{get_das_root()}/tests/dasGLTF/models"
    for (relative in ["Box.glb", "BoxInterleaved.glb", "SimpleSkin.gltf"]){
        var raw <- load_gltf("{root}/{relative}")
        var profile = ProcessingProfile(texture_cache = "{work_dir}/test-cache")
        var cooked <- process_gltf(raw, relative, profile)
        t |> success(processed_valid(cooked), "processed asset validates")
        t |> success(cooked.processed_vertices <= cooked.source_vertices, "processing does not add vertices")
        t |> equal(hash(cooked.scene.nodes), hash(raw.nodes), "named nodes and transforms survive")
        t |> equal(hash(cooked.scene.skins), hash(raw.skins), "skin bindings survive")
        t |> equal(hash(cooked.scene.animations), hash(raw.animations), "animation data survives")
        var unpacked <- unpack_processed_geometry(cooked)
        let bounds = gltf_scene_bounds(unpacked)
        t |> success(length(bounds._0 - cooked.lo) < 0.0001 && length(bounds._1 - cooked.hi) < 0.0001, "positions/bounds survive packing")
        if (relative == "SimpleSkin.gltf") {
            for (mesh in cooked.meshes) { for (p in mesh.primitives) {
                t |> success((p.attributes & VERTEX_SKIN) != 0, "skinned fixtures retain skin attributes")
                t |> equal(p.stride, processed_stride(p.attributes), "skin stride matches the compact layout")
            }}
            for (mesh in unpacked.meshes) { for (p in mesh.primitives) { for (v in p.vertices) {
                t |> success(abs(dot(v.weights0, float4(1.0)) - 1.0) < .00001, "decoded skin weights remain normalized")
                for (i in range(1, 4)) { t |> success(v.weights0[i - 1] >= v.weights0[i], "decoded influences remain ordered") }
            }}}
        }
        for (ext in [".das_gltf", ".das_glb"]){
            let path = "{work_dir}/roundtrip/{base_name(relative)}{ext}"
            t |> success(save_processed(cooked, path, "{work_dir}/roundtrip"), "save custom format")
            var loaded <- load_processed(path)
            t |> equal(hash(loaded), hash(cooked), "both containers restore the same asset")
            delete loaded
        }
        var strides : array<int>
        for (mesh in cooked.meshes){for (p in mesh.primitives){
            t |> success(p.stride < 104, "GPU vertices remain compact")
            var vertices <- processed_vertex_bytes(p)
            t |> equal(length(vertices), p.vertex_count * p.stride)
            delete vertices
        }}
        delete raw; delete cooked; delete unpacked; delete profile; delete strides
    }
    let removed = rmdir_rec_result(work_dir)
    t |> success(removed is value && unsafe(removed.value), "remove processed asset fixtures")
}

[test]
def test_translucent_triangle_order(t : T?){
    var raw : GltfScene
    raw.materials |> emplace(GltfMaterial(alphaMode = GltfAlphaMode.blend))
    var mesh : GltfMesh; var prim = GltfPrimitive(material = 0)
    prim.vertices |> reserve(10)
    for (i in range(10)){prim.vertices |> push(GltfVertex(position = float3(float(i), float(i * i % 3), 0.0), normal = float3(0.0, 0.0, 1.0)))}
    prim.indices <- [0u, 1u, 2u, 6u, 7u, 8u, 0u, 2u, 3u, 6u, 8u, 9u]
    mesh.primitives |> emplace(prim); raw.meshes |> emplace(mesh)
    var cooked <- process_gltf(raw, "transparent-fixture", ProcessingProfile(texture_cache = ""))
    var decoded <- unpack_processed_geometry(cooked)
    let before & = unsafe(raw.meshes[0].primitives[0]); let after & = unsafe(decoded.meshes[0].primitives[0])
    for (tri in range(4)){
        let first = before.vertices[before.indices[tri * 3]].position
        var corner = -1
        for (c in range(3)){if (after.vertices[after.indices[tri * 3 + c]].position == first){corner = c}}
        t |> success(corner >= 0, "transparent triangle remains in its original position")
        if (corner >= 0){for (c in range(3)){t |> equal(after.vertices[after.indices[tri * 3 + (corner + c) % 3]].position, before.vertices[before.indices[tri * 3 + c]].position)}}
    }
    delete raw; delete cooked; delete decoded
}

[test]
def test_load_rejects_truncated_encoded_geometry(t : T?){
    var raw : GltfScene; var mesh : GltfMesh; var prim = GltfPrimitive()
    prim.vertices <- [
        GltfVertex(position = float3(0.0, 0.0, 0.0), normal = float3(0.0, 0.0, 1.0)),
        GltfVertex(position = float3(1.0, 0.0, 0.0), normal = float3(0.0, 0.0, 1.0)),
        GltfVertex(position = float3(0.0, 1.0, 0.0), normal = float3(0.0, 0.0, 1.0))
    ]
    prim.indices <- [0u, 1u, 2u]; mesh.primitives |> emplace(prim); raw.meshes |> emplace(mesh)
    var cooked <- process_gltf(raw, "truncated-fixture", ProcessingProfile(texture_cache = ""))
    cooked.meshes[0].primitives[0].vertices |> resize(length(cooked.meshes[0].primitives[0].vertices) - 1)
    let file_r = create_temp_file_result("gltf_truncated", ".das_glb")
    if (!(file_r is value)){t |> failure("create malformed fixture"); delete raw; delete cooked; return }
    let path = unsafe(file_r.value)
    t |> success(save_processed(cooked, path), "structural save accepts encoded payload without decoding")
    var rejected = false
    try {
        var loaded <- load_processed(path)
        delete loaded
    } recover {
        rejected = true
    }
    t |> success(rejected, "load validates encoded payloads before returning")
    remove(path); delete raw; delete cooked
}

[test]
def test_load_rejects_skin_weights_without_exact_byte_sum(t : T?){
    var prim = GltfPrimitive()
    prim.vertices <- [GltfVertex(weights0 = float4(1.0)), GltfVertex(weights0 = float4(1.0)), GltfVertex(weights0 = float4(1.0))]
    prim.indices <- [0u, 1u, 2u]
    var packed <- process_primitive(prim, VERTEX_SKIN, false, false)
    for (vertex in range(packed.vertex_count)) { for (component in range(4)) { packed.vertices[vertex * packed.stride + 20 + component] = 0u8 } }
    var mesh : ProcessedMesh; mesh.primitives |> emplace(packed)
    var asset = ProcessedAsset(version = DAS_GLTF_VERSION); asset.meshes |> emplace(mesh)
    let file_r = create_temp_file_result("gltf_bad_skin_sum", ".das_glb")
    if (!(file_r is value)){t |> failure("create malformed skin fixture"); delete asset; return }
    let path = unsafe(file_r.value)
    t |> success(save_processed(asset, path), "structural save accepts payload before load validation")
    var rejected = false
    try { var loaded <- load_processed(path); delete loaded } recover { rejected = true }
    t |> success(rejected, "load rejects compact skin weights that do not sum to 255")
    let stride = asset.meshes[0].primitives[0].stride
    for (vertex in range(asset.meshes[0].primitives[0].vertex_count)) { asset.meshes[0].primitives[0].vertices[vertex * stride + 20] = 255u8 }
    t |> success(save_processed(asset, path), "write canonical compact skin weights")
    var canonical_loaded = false
    try { var loaded <- load_processed(path); canonical_loaded = true; delete loaded } recover { canonical_loaded = false }
    t |> success(canonical_loaded, "canonical compact skin weights load")
    asset.meshes[0].primitives[0].vertices[20] = 155u8; asset.meshes[0].primitives[0].vertices[21] = 0u8
    asset.meshes[0].primitives[0].vertices[22] = 100u8; asset.meshes[0].primitives[0].vertices[23] = 0u8
    t |> success(save_processed(asset, path), "structural save accepts noncanonical weight order")
    rejected = false
    try { var loaded <- load_processed(path); delete loaded } recover { rejected = true }
    t |> success(rejected, "load rejects a nonzero influence after a zero weight")
    remove(path); delete asset
}
