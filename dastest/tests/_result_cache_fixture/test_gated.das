options gen2

require dastest/testing_boost public
require _gate_lib

[test]
def test_gate_value(t : T?) {
    t |> equal(gate_value(1), 42)
}
