options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require scene_helpers

// Guards the independent-count model: the boost descriptor path (create_descriptor_set_layout
// / create_descriptor_pool / update_descriptor_sets) drives a storage-buffer binding whose
// descriptorCount is 1 with no immutable samplers. If the count erased to 0 the shader would
// not see the buffer and the squares would be wrong.
[test]
def test_boost_descriptors(t : T?) {
    let n = 256
    var values <- run_compute_boost(n)
    var wrong = 0
    for (i in range(n)) {
        if (values[i] != uint(i * i)) {
            wrong++
        }
    }
    t |> success(wrong == 0, "boost descriptor path squared all {n} elements (descriptorCount honored)")
    t |> equal(values[255], 65025u)        // 255*255
    delete values
}
