options gen2
require dastest/testing_boost public
require gltf/gltf_processed_gl

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
