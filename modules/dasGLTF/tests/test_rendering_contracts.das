options gen2
require dastest/testing_boost public
require gltf/gltf_processed_gl
require daslib/fio
require strings

[test]
def test_packed_geometry_preserves_skinning_for_every_skinned_mesh_instance(t : T?){
    var scene : GltfScene
    scene.materials |> push(GltfMaterial(normalTex = 0))
    scene.nodes <- [GltfNode(mesh = 1, skin = -1), GltfNode(mesh = 0, skin = 0)]
    let skinned = gltf_packed_primitive_attributes(scene, 0, 0)
    let rigid = gltf_packed_primitive_attributes(scene, 1, -1)
    t |> success((skinned & VERTEX_SKIN) != 0)
    t |> success((skinned & VERTEX_TANGENT) != 0)
    t |> success((rigid & VERTEX_SKIN) == 0)
}

[test]
def test_processed_draw_mask_tracks_only_missing_generic_attributes(t : T?){
    let minimal = gltf_processed_missing_attributes(VERTEX_UV)
    t |> success((minimal & GLTF_GL_ATTR_UV0) == 0u)
    t |> success((minimal & GLTF_GL_ATTR_TANGENT) != 0u)
    t |> success((minimal & GLTF_GL_ATTR_JOINTS0) != 0u)
    t |> success((minimal & GLTF_GL_ATTR_WEIGHTS0) != 0u)
    let complete = gltf_processed_missing_attributes(VERTEX_UV | VERTEX_TANGENT | VERTEX_SKIN | VERTEX_UV1 | VERTEX_COLOR)
    t |> equal(complete, 0u)
}

[test]
def test_compact_skin_upload_and_shader_source_contracts(t : T?){
    let root = get_das_root()
    let gl_source = fread("{root}/modules/dasGLTF/gltf/gltf_gl.das")
    let processed_gl_source = fread("{root}/modules/dasGLTF/gltf/gltf_processed_gl.das")
    let shader_source = fread("{root}/modules/dasGLTF/gltf/gltf_pbr.das")
    t |> success(find(gl_source, "let skin = canonicalize_skin_influences(v.joints0, v.weights0)") >= 0)
    t |> success(find(processed_gl_source, "packed_attribute(6u, 4, GL_UNSIGNED_BYTE, false, p.stride, at)") >= 0)
    t |> success(find(processed_gl_source, "packed_attribute(7u, 4, GL_UNSIGNED_BYTE, true, p.stride, at + 4); at += 8") >= 0)
    t |> success(find(shader_source, "let weights = float4(1.0 - a_weights0.y - a_weights0.z - a_weights0.w, a_weights0.yzw)") >= 0)
    t |> success(find(shader_source, "if (weights.y > 0.0)") >= 0)
    t |> success(find(shader_source, "if (weights.z > 0.0)") >= 0)
    t |> success(find(shader_source, "if (weights.w > 0.0)") >= 0)
}
