options gen2
options auto_inline_functions = true

require dastest/testing_boost public

def pure_sum(a : int = 13; b : int) => a + b

def discard_sum(x : int) : int {
    pure_sum(x, 2)
    pure_sum([b = 2])
    return x
}

[test]
def test_discarded_pure_inline_result(t : T?) {
    t |> equal(discard_sum(5), 5)
}
