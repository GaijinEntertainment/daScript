options gen2

require dastest/testing_boost public

[never_inline]
def asserted_positive(x : int) : int {
    assert(x > 0, "not positive")
    return x
}

[test(description = "an assert inside try is recovered")]
def test_caught_exception(t : T?) {
    var caught = false
    try {
        var _unused = asserted_positive(-1)
    } recover {
        caught = true
    }
    t |> success(caught)
}
