options gen2

require dastest/testing_boost
require daslib/ast
require daslib/ast_boost
require daslib/rtti
require daslib/fio
require daslib/lint
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
                stale_nolint_collect(program, path, fread(path), disabled, enabled, found)
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

//! The before fixture fixed with the ``enabled`` rules matches the after fixture, which compiles and carries no fix
def check_pair(t : T?; name : string; enabled : table<string>; applied : int) {
    let before = fixture("_fixture_lint_fix_paranoid_{name}_before.das")
    let after = fixture("_fixture_lint_fix_paranoid_{name}_after.das")
    let fixes <- collect_fixes(t, before, enabled)
    let fixed = apply_lint_fixes(fread(before), fixes)
    t |> equal(fixed.applied, applied)
    t |> equal(fixed.text, fread(after))
    let again <- collect_fixes(t, after, enabled)
    t |> equal(length(again), 0)
}

[test]
def test_lint_fix_paranoid(t : T?) {
    t |> run("LINT002 / LINT004 / LINT012 / LINT013 rename locals and arguments") @@(t : T?) {
        check_pair(t, "names", { "LINT002", "LINT004", "LINT012", "LINT013" }, 5)
    }
    t |> run("LINT016 / LINT017 / LINT018 / LINT020 / LINT024 rewrite the narrowing and cloning spellings") @@(t : T?) {
        check_pair(t, "casts", { "LINT016", "LINT017", "LINT018", "LINT020", "LINT024" }, 15)
    }
    t |> run("LINT001 / LINT008 / LINT009 drop dead or redundant code") @@(t : T?) {
        check_pair(t, "dead", { "LINT001", "LINT008", "LINT009" }, 4)
    }
    t |> run("LINT019 / LINT022 / LINT031 edit declarations and directives") @@(t : T?) {
        check_pair(t, "decls", { "LINT002", "LINT006", "LINT007", "LINT008", "LINT019", "LINT022", "LINT031" }, 10)
    }
}
