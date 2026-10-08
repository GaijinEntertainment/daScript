options gen2

require dastest/testing_boost public
require daslib/rtti
require daslib/ast

def fixture(name : string) : string => "{get_das_root()}/tests/module_cache/_fixtures/{name}.das"

//! compiles the user fixture and answers how its compile saw mc_noopt_shared: -1 absent, 0 private to the compile, 1 promoted
def compile_user(t : T?; no_opt : bool) : int {
    var seen = -1
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.no_optimizations = no_opt
            compile_file(fixture("mc_noopt_user"), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                t |> success(ok, "the user fixture compiles: {issues}")
                if (ok) {
                    program_for_each_module(program) $(mod) {
                        if (mod.name == "mc_noopt_shared") {
                            seen = mod.moduleFlags.promoted ? 1 : 0
                        }
                    }
                }
            }
        }
    }
    return seen
}

[test]
def test_no_optimizations_keeps_shared_dependencies_private(t : T?) {
    t |> run("a compile with optimizations off does not promote the shared module it compiled") @(t : T?) {
        t |> equal(compile_user(t, true), 0)
        t |> success(get_module("mc_noopt_shared") == null, "the module died with the compile's ModuleGroup")
    }
    t |> run("an optimizing compile promotes it, and a later compile with optimizations off reuses that copy") @(t : T?) {
        t |> equal(compile_user(t, false), 1)
        t |> success(get_module("mc_noopt_shared") != null, "the promoted module outlives the compile")
        t |> equal(compile_user(t, true), 1)
    }
}
