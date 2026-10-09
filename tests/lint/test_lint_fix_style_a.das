options gen2

require dastest/testing_boost
require daslib/ast
require daslib/fio
require daslib/lint
require daslib/perf_lint
require daslib/style_lint
require daslib/lint_config

def fixture(name : string) : string => "{get_das_root()}/tests/lint/{name}"

def collect_fixes(t : T?; path : string; enabled : table<string>) : array<LintFix> {
    var fixes : array<LintFix>
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    t |> failure("{path} failed to compile: {issues}")
                    return
                }
                let disabled : table<string>
                var found : array<LintIssue>
                paranoid_collect_issues(program, found, disabled, enabled)
                perf_lint_collect_issues(program, found, disabled, enabled)
                style_lint_collect_issues(program, found, disabled, enabled, false)
                for (issue in found) {
                    if (issue.fix.line > 0) {
                        fixes |> push(issue.fix)
                    }
                }
            }
        }
    }
    return <- fixes
}

def check_pair(t : T?; before, after : string; enabled : table<string>; applied : int) {
    let fixes <- collect_fixes(t, fixture(before), enabled)
    let fixed = apply_lint_fixes(fread(fixture(before)), fixes)
    t |> equal(fixed.applied, applied)
    t |> equal(fixed.text, fread(fixture(after)))
    let left <- collect_fixes(t, fixture(after), enabled)
    t |> equal(length(left), 0)
}

[test]
def test_lint_fix_style_pipes_braces_declarations(t : T?) {
    check_pair(t, "_fixture_lint_fix_style_a_before.das", "_fixture_lint_fix_style_a_after.das",
        { "STYLE001", "STYLE002", "STYLE003", "STYLE005", "STYLE006", "STYLE010", "STYLE011", "STYLE012", "STYLE013" }, 20)
}

[test]
def test_lint_fix_style_guards_and_operators(t : T?) {
    check_pair(t, "_fixture_lint_fix_style_b_before.das", "_fixture_lint_fix_style_b_after.das",
        { "STYLE006", "STYLE016", "STYLE017", "STYLE018", "STYLE019", "STYLE020", "STYLE022", "STYLE023" }, 21)
}
