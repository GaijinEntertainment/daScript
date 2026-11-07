options gen2
require dastest/testing_boost

var r = 0

def b(c : int) : int {
    var q = 0
    if (c > 0) {
        if (c == 1) {
            goto label 3
        }
        q += 100
        label 3:
    }
    q += 7
    return q
}

def f(c : int) {
    if (c == 1) {
        goto label 3
    }
    r += 100
    label 3:
}

def o(c : int) : int {
    var k = 0
    var n = c
    label 1:
    k++
    if (n > 0) {
        label 2:
        n--
        goto label 1
    }
    return k
}

[test]
def test_a_goto_to_a_label_that_ends_a_block(t : T?) {
    t |> equal(b(1), 7, "the block finishes and the statement after it runs")
    t |> equal(b(2), 107)
    f(1)
    f(2)
    t |> equal(r, 100)
}

[test]
def test_a_goto_from_a_labelled_block_to_an_outer_label(t : T?) {
    t |> equal(o(2), 3)
}
