options gen2

require dastest/testing_boost public
require daslib/cross_context
require _mc_global_offset_solid

var late : Module?

[init]
def load_late() {
    late = require_module_now("%/tests/module_cache/_mc_global_offset_late.das")
}

def read_c() : int => g_c

[test]
def test_late_require_keeps_global_offsets(t : T?) {
    t |> success(late != null)
    t |> equal(g_a, 1)
    t |> equal(read_c(), 3)
}
