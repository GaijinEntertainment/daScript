options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require scene_helpers

[test]
def test_compute(t : T?) {
    let n = 256
    var values <- run_compute(n)
    var wrong = 0
    for (i in range(n)) {
        if (values[i] != uint(i * i)) {
            wrong++
        }
    }
    t |> success(wrong == 0, "compute shader squared all {n} elements (out[i] == i*i)")
    t |> equal(values[255], 65025u)        // 255*255
    delete values
}
