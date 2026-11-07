options gen2
require dastest/testing_boost

def bw(c : int) : int {
    var r = 0
    var n = c
    label 1:
    r++
    while (true) {
        if (n > 0) {
            n--
            goto label 1
        }
        label 2:
        r += 100
        break
    }
    return r
}

[test]
def test_a_goto_from_a_loop_body_to_an_outer_label_numbered_below_the_bodys_labels(t : T?) {
    t |> equal(bw(2), 103)
}
