options gen2
options debugger
require dastest/testing_boost

def f(c : int) : int {
    var r = 0
    var n = c
    label 1:
    r++
    if (n > 0) {
        label 2:
        r += 10
        n--
        if (n == 5) {
            goto label 2
        }
        goto label 1
    }
    return r
}

def h(c : int) : int {
    var r = 0
    var n = c
    label 5:
    r++
    if (n > 0) {
        label 1:
        if (n < 0) {
            return -1
        }
        n--
        goto label 5
    }
    return r
}

//! the file runs on the debugger's block nodes; the plain nodes
//! take the same shapes in test_goto_to_a_label_at_block_end.das
[test]
def test_a_goto_from_a_labelled_block_to_a_label_outside_it(t : T?) {
    t |> equal(f(2), 23)
    t |> equal(h(1), 2, "a label numbered above the block's own table")
}
