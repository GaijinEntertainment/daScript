options gen2
options no_aot

require dastest/testing_boost public
require daslib/ast

[test]
def test_simulate_macros_run_after_a_compile_in_init(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file("{get_das_root()}/tests/daslib/_nested_compile_outer.das", access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                t |> success(ok, "the outer program compiles: {issues}")
                if (!ok) {
                    return
                }
                simulate(program) $(sok; _context; serrors) {
                    t |> success(sok, "a simulate macro still has its program after an [init] compiled another script: {serrors}")
                }
            }
        }
    }
}
