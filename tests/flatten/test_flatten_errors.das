options gen2
options indenting = 4
options no_aot     // invokes the compiler at runtime (compile_file) — not AOT-linkable

require daslib/rtti
require strings
require dastest/testing_boost public

//! Boundary tests for daslib/flatten: each `_err_*.das` fixture trips a
//! supported-subset boundary; we compile it and assert the specific loud
//! `flatten:` error fires (rather than silently emitting wrong output).

def compile_issues(file_path : string) : string {
    var issuesText = ""
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            compile_file(file_path, access, unsafe(addr(mg)), cop) $(_ok, _program, issues) {
                issuesText := issues
            }
        }
    }
    return issuesText
}

[test]
def test_flatten_boundaries(t : T?) {
    t |> run("recursion is rejected") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/flatten/_err_recursion.das")
        t |> success(find(issues, "recursion through") >= 0, "expected recursion error, got: {issues}")
    }
    t |> run("loops are rejected") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/flatten/_err_loop.das")
        t |> success(find(issues, "loops are not supported") >= 0, "expected loop error, got: {issues}")
    }
    t |> run("non-whitelisted builtin is rejected") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/flatten/_err_builtin.das")
        t |> success(find(issues, "not on the whitelist") >= 0, "expected whitelist error, got: {issues}")
    }
    t |> run("move (<-) is rejected") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/flatten/_err_move.das")
        t |> success(find(issues, "cannot be predicated") >= 0, "expected move/clone error, got: {issues}")
    }
}
