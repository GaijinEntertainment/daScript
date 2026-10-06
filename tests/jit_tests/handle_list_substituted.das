options gen2

require UnitTest
require math
require dastest/testing_boost public

[jit]
def list_length(p : Float3x4List?) : int => length(*p)

[test]
def test_jit_length_of_a_substituted_math_list(t : T?) {
    testFloat3x4List() $(list) {
        let p = unsafe(addr(list))
        t |> equal(list_length(p), 10)
        t |> success(!jit_enabled() || is_jit_function(@@list_length))
    }
}
