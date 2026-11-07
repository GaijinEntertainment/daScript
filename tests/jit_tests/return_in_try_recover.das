options gen2
require dastest/testing_boost public

def returns_from_a_block_inside_try : int {
    var r = 0
    try {
        let f = $() : int {
            return 3
        }
        r = invoke(f)
    } recover {
        pass
    }
    return r
}

[test]
def test_a_return_from_a_closure_inside_a_try_arm_is_the_closures_own(t : T?) {
    t |> equal(returns_from_a_block_inside_try(), 3)
}
