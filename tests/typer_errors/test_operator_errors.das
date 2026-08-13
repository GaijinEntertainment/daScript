// operator declarations dispatch through fixed-operand nodes - shapes no call
// site can honor (like defaulted parameters) must fail compilation
options gen2
options no_aot     // invokes the compiler at runtime (compile_file) — not AOT-linkable

require daslib/ast
require strings
require dastest/testing_boost public


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

def expect_issue(t : T?; fixture, message : string) {
    let issues = compile_issues("{get_das_root()}/tests/typer_errors/{fixture}")
    t |> success(find(issues, message) >= 0,
        "expected '{message}' compiling {fixture}, got: {issues}")
}


[test]
def test_operator_declaration_errors(t : T?) {
    t |> run("operator with a defaulted parameter") @(t : T?) {
        t |> expect_issue("_operator_default_arg.das",
            "operator '+' can't have default arguments")
    }
}
