options gen2

require dastest/testing_boost
require daslib/ast
require daslib/rtti
require daslib/fio
require daslib/perf_lint
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
                perf_lint_collect_issues(program, found, disabled, enabled)
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
    let again <- collect_fixes(t, fixture(after), enabled)
    t |> equal(length(again), 0)
}

[test]
def test_lint_fix_perf_expressions(t : T?) {
    t |> run("expression-level PERF fixes rewrite the before fixture into the after " +
        "fixture") @@(t : T?) {
        check_pair(t, "_fixture_lint_fix_perf_before.das", "_fixture_lint_fix_perf_after.das",
            { "PERF003", "PERF007", "PERF010", "PERF011", "PERF013", "PERF014", "PERF015",
                "PERF016", "PERF017", "PERF019", "PERF020", "PERF021", "PERF025" }, 30)
    }
}

[test]
def test_lint_fix_perf_statements(t : T?) {
    t |> run("statement-level PERF fixes rewrite the before fixture into the after " +
        "fixture") @@(t : T?) {
        check_pair(t, "_fixture_lint_fix_perf_stmt_before.das",
                   "_fixture_lint_fix_perf_stmt_after.das",
            { "PERF009", "PERF018", "PERF022", "PERF023", "PERF024", "PERF030", "PERF032" }, 13)
    }
}
