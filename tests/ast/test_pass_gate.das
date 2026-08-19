options gen2

// Reaching the test body IS the assertion: _pass_gate_helper's lint verdict fails the compile
// unless the driver honors AstPassMacro.canVisitPass (its error text names the broken clause).

module pass_gate_fixture

require dastest/testing_boost public
require _pass_gate_helper

def sum_to(n : int) : int {
    var s = 0
    for (i in range(n)) {
        s += i
    }
    return s
}

[test]
def test_pre_infer_pass_gate_is_honored(t : T?) {
    t |> equal(6, sum_to(4))
}
