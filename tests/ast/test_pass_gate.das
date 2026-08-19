options gen2

// Compiles only if the driver honors AstPassMacro.canVisitPass: _pass_gate_helper's lint
// verdict raises a macro_error unless (a) the ungated pre-infer probe was applied on a pass
// past 0, (b) the pass-0-only probe fired, and (c) it was never applied outside pass 0.
// Reaching the test body IS the assertion.

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
