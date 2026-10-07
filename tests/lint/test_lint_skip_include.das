options gen2

require dastest/testing_boost
require daslib/ast
require daslib/rtti
require daslib/perf_lint
require daslib/lint_config
require strings

[test]
def test_lint_skip_include(t : T?) {
    t |> run("the runner drops a finding inside an included lint-skip-file file; a direct collect keeps both") @@(t : T?) {
        let path = "{get_das_root()}/tests/lint/_fixture_lint_skip_include.das"
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.export_all = true
                cop.lint_check = true
                cop.no_optimizations = true
                compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                    if (!ok) {
                        t |> failure("fixture failed to compile: {issues}")
                        return
                    }
                    let disabled : table<string>
                    let enabled <- { "PERF013" }
                    var found : array<LintIssue>
                    perf_lint_collect_issues(program, found, disabled, enabled)
                    t |> equal(length(found), 2)
                    t |> equal(drop_skipped_file_issues(path, found), 1)
                    t |> equal(length(found), 1)
                    if (length(found) == 1) {
                        t |> success(ends_with(found[0].file, "_fixture_lint_skip_included_plain.das"), "kept finding is in the plain file: {found[0].file}")
                    }
                }
            }
        }
    }
}
