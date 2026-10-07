options gen2
options persistent_heap
require dastest/testing_boost public
require gltf/gltf_parse
require gltf/gltf_processed
require daslib/fio
require math

[test]
def required_unlit_material_loads_and_survives_processing(t : T?) {
    var scene <- load_gltf("{get_das_root()}/tests/dasGLTF/models/Unlit.gltf")
    t |> equal(length(scene.materials), 2, "required unlit extension loads")
    if (length(scene.materials) == 2) {
        t |> equal(scene.materials[0].baseColorFactor, float4(0.25, 0.5, 0.75, 0.5))
        t |> equal(scene.materials[0].alphaMode, GltfAlphaMode.blend)
        t |> success(scene.materials[0].doubleSided)
        t |> success(scene.materials[0].unlit, "extension marks only its material unlit")
        t |> success(!scene.materials[1].unlit, "ordinary material stays lit")
        t |> equal(length(scene.meshes), 1)
        var cooked <- process_gltf(scene, "unlit", ProcessingProfile(texture_cache = ""))
        t |> success(processed_valid(cooked), "unlit geometry processes")
        t |> success(cooked.scene.materials[0].unlit, "unlit flag survives processing")
        t |> success((cooked.meshes[0].primitives[0].attributes & VERTEX_COLOR) != 0, "unlit vertex colors survive processing")
        scene.materials[0].unlit = false
        var ordinary <- process_gltf(scene, "ordinary", ProcessingProfile(texture_cache = ""))
        t |> success((ordinary.meshes[0].primitives[0].attributes & VERTEX_COLOR) == 0, "ordinary material does not force vertex color retention")
        delete ordinary
        scene.materials[0].unlit = true
        var unpacked <- unpack_processed_geometry(cooked)
        t |> success(length(unpacked.meshes[0].primitives[0].vertices[0].color0 - float4(1., 0., 0., 1.)) < 1e-6, "decoded vertex color survives byte normalization")
        let directory = create_temp_directory_result("gltf_unlit")
        if (directory is value) {
            let work_dir = unsafe(directory.value)
            let path = "{work_dir}/unlit.das_gltf"
            t |> success(save_processed(cooked, path, work_dir), "save processed unlit asset")
            var restored <- load_processed(path)
            t |> success(restored.scene.materials[0].unlit, "unlit flag survives disk round trip")
            delete restored
            var cleanup_error : string
            let removed = rmdir_rec(work_dir, cleanup_error)
            t |> success(removed, cleanup_error)
        } else { t |> failure("create unlit asset directory") }
        delete unpacked; delete cooked
    }
    delete scene
}
