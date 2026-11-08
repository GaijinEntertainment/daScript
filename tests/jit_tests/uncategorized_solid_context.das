options gen2
options solid_context

require dastest/testing_boost

include _global_variables.das

[test(description = "under solid_context a global var and a shared let global keep their values across calls, and the functions reading them JIT")]
def test_global_variables_solid(t : T?) {
    check_global_variables(t)
}
