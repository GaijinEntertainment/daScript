options gen2

require dastest/testing_boost
require daslib/ast
require daslib/ast_boost
require daslib/rtti
require daslib/fio
require strings
require daslib/lint
require daslib/perf_lint
require daslib/style_lint
require daslib/lint_config
require daslib/lint_fixups

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
                if (key_exists(enabled, "LINT019")) {
                    stale_nolint_collect(program, path, fread(path), disabled, enabled, found)
                }
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

[test]
def test_lint_fix(t : T?) {
    t |> run("--fix edits turn the before fixture into the after fixture") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_before.das"), { "STYLE042", "STYLE043", "PERF033" })
        let fixed = apply_lint_fixes(fread(fixture("_fixture_lint_fix_before.das")), fixes)
        t |> equal(fixed.applied, 11)
        t |> equal(fixed.text, fread(fixture("_fixture_lint_fix_after.das")))
    }
    t |> run("the after fixture carries no fix") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_after.das"), { "STYLE042", "STYLE043", "PERF033" })
        t |> equal(length(fixes), 0)
    }
    t |> run("STYLE047 annotates structures, rewrites comparators, adds the require") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_cmp_before.das"), { "STYLE047" })
        let fixed = apply_lint_fixes(fread(fixture("_fixture_lint_fix_cmp_before.das")), fixes)
        t |> equal(fixed.applied, 3)
        t |> equal(fixed.text, fread(fixture("_fixture_lint_fix_cmp_after.das")))
    }
    t |> run("the cmp after fixture carries no fix") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_cmp_after.das"), { "STYLE047" })
        t |> equal(length(fixes), 0)
    }
    t |> run("LINT003 / LINT014 edits turn var into let or drop it") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_var_before.das"), { "LINT003", "LINT014" })
        let fixed = apply_lint_fixes(fread(fixture("_fixture_lint_fix_var_before.das")), fixes)
        t |> equal(fixed.applied, 5)
        t |> equal(fixed.text, fread(fixture("_fixture_lint_fix_var_after.das")))
    }
    t |> run("the var after fixture carries no fix") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_var_after.das"), { "LINT003", "LINT014" })
        t |> equal(length(fixes), 0)
    }
    t |> run("LINT019 edits drop the stale codes, then the comment or the line they leave empty") @@(t : T?) {
        let enabled <- { "LINT019", "STYLE042", "STYLE038", "PERF001" }
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_stale_before.das"), enabled)
        let fixed = apply_lint_fixes(fread(fixture("_fixture_lint_fix_stale_before.das")), fixes)
        t |> equal(fixed.applied, 3)
        t |> equal(fixed.text, fread(fixture("_fixture_lint_fix_stale_after.das")))
    }
    t |> run("the stale after fixture carries no fix") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_stale_after.das"), { "LINT019", "STYLE042", "STYLE038", "PERF001" })
        t |> equal(length(fixes), 0)
    }
    t |> run("overlapping and out-of-text spans are dropped") @@(t : T?) {
        let fixes <- [
            LintFix(line = 1, column = 0, last_line = 1, last_column = 3, replacement = "X"),
            LintFix(line = 1, column = 2, last_line = 1, last_column = 4, replacement = "Y"),
            LintFix(line = 9, column = 0, last_line = 9, last_column = 1, replacement = "Z")]
        let fixed = apply_lint_fixes("abcdef\n", fixes)
        t |> equal(fixed.applied, 1)
        t |> equal(fixed.text, "Xdef\n")
        let used <- applied_lint_fix_spans("abcdef\n", fixes)
        t |> equal(length(used), 1)
        t |> equal(used[0], int3(0, 3, 0))
    }
    t |> run("a fix starting where an applied one ends waits for the next run") @@(t : T?) {
        let fixes <- [
            LintFix(line = 1, column = 2, last_line = 1, last_column = 2, replacement = "X"),
            LintFix(line = 1, column = 2, last_line = 1, last_column = 2, replacement = "X"),
            LintFix(line = 1, column = 2, last_line = 1, last_column = 4, replacement = "Y")]
        let fixed = apply_lint_fixes("abcdef\n", fixes)
        t |> equal(fixed.applied, 1)
        t |> equal(fixed.text, "abXcdef\n")
        let used <- applied_lint_fix_spans("abcdef\n", fixes)
        t |> equal(length(used), 1)
        t |> equal(used[0].z, 0)
    }
    t |> run("a line break in a replacement follows CRLF text") @@(t : T?) {
        let fixes <- [LintFix(line = 1, column = 3, last_line = 1, last_column = 6, replacement = "\n  x")]
        let fixed = apply_lint_fixes("abcdef\r\nnext\r\n", fixes)
        t |> equal(fixed.text, "abc\r\n  x\r\nnext\r\n")
    }
}

[test]
def test_lint_text_helpers(t : T?) {
    t |> run("comment openers and indentation") @@(t : T?) {
        t |> equal(lint_has_comment("a // b"), true)
        t |> equal(lint_has_comment("a /* b */"), true)
        t |> equal(lint_has_comment("a + b"), false)
        t |> equal(lint_indent_of("    x"), 4)
    }
}

[test]
def test_lint_fix_helpers(t : T?) {
    t |> run("identical fixes from sibling instantiations apply once") @@(t : T?) {
        let fixes <- [
            LintFix(line = 1, column = 2, last_line = 1, last_column = 2, replacement = "Y"),
            LintFix(line = 1, column = 2, last_line = 1, last_column = 2, replacement = "Y")]
        let fixed = apply_lint_fixes("abcdef\n", fixes)
        t |> equal(fixed.applied, 1)
        t |> equal(fixed.text, "abYcdef\n")
    }
    t |> run("merged edits become one fix over their span") @@(t : T?) {
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.export_all = true
                compile_file(fixture("_fixture_lint_span.das"), access, unsafe(addr(mg)),
                             cop) $(ok; program; _issues) {
                    t |> success(ok)
                    return if (!ok)
                    let fn = program.getThisModule |> find_unique_function("spans")
                    let edits <- [
                        LintFix(line = 6, column = 11, last_line = 6, last_column = 16,
                                replacement = "count"),
                        LintFix(line = 5, column = 4, last_line = 5, last_column = 9,
                                replacement = "count")]
                    let merged = lint_merge_fixes(fn.at, edits)
                    t |> equal(merged.line, 5)
                    t |> equal(merged.last_line, 6)
                    let src = fread(fixture("_fixture_lint_span.das"))
                    let fixed = apply_lint_fixes(src, [merged])
                    t |> equal(fixed.applied, 1)
                    let text = replace(fixed.text, "\r\n", "\n")
                    t |> success(find(text, "    count += n\n    return count\n") >= 0)
                    t |> equal(lint_merge_fixes(fn.at, [edits[0], edits[0]]).line, 0)
                }
            }
        }
    }
}
