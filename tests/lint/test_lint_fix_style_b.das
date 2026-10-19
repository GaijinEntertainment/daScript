options gen2

require dastest/testing_boost
require daslib/ast
require daslib/ast_boost
require daslib/rtti
require daslib/fio
require daslib/lint
require daslib/perf_lint
require daslib/style_lint
require daslib/lint_config
require daslib/lint_fixups

def fixture(name : string) : string => "{get_das_root()}/tests/lint/{name}"

class SpanMarks : AstVisitor {
    found : array<tuple<expected : string; got : string>>
    def override preVisitExprCall(var expr : ExprCall?) : void {
        return if (expr.func == null || expr.func.fromGeneric == null
                   || expr.func.fromGeneric.name != "mark")
        let tag = expr.arguments[1]
        found |> push((expected = tag is ExprConstString
                ? string((tag as ExprConstString).value) : "",
            got = lint_expr_text(expr.arguments[0])))
    }
}

def collect_fixes(t : T?; path : string; enabled : table<string>) : array<LintFix> {
    var fixes : array<LintFix>
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            cop.no_fold_unsafe = true
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

def check_pair(t : T?; name : string; enabled : table<string>; applied : int) {
    let before = fixture("_fixture_lint_fix_{name}_before.das")
    let after = fixture("_fixture_lint_fix_{name}_after.das")
    let fixes <- collect_fixes(t, before, enabled)
    let fixed = apply_lint_fixes(fread(before), fixes)
    t |> equal(fixed.applied, applied)
    t |> equal(fixed.text, fread(after))
    let again <- collect_fixes(t, after, enabled)
    t |> equal(length(again), 0)
}

[test]
def test_lint_fix_style_b(t : T?) {
    t |> run("STYLE024 / 025 / 026 / 034 / 036 drop or narrow unsafe wraps and cast " +
        "spellings") @@(t : T?) {
        check_pair(t, "unsafe", { "STYLE024", "STYLE025", "STYLE026", "STYLE034", "STYLE036" }, 11)
    }
    t |> run("STYLE027 / 031 / 032 / 033 build containers in one expression") @@(t : T?) {
        check_pair(t, "containers", { "STYLE027", "STYLE031", "STYLE032", "STYLE033" }, 9)
    }
    t |> run("STYLE028 / 029 / 030 / 039 rewrite receivers, requires and string bytes") @@(t : T?) {
        check_pair(t, "misc", { "STYLE028", "STYLE029", "STYLE030", "STYLE039" }, 7)
    }
}

[test]
def test_lint_expr_text_trailing_names(t : T?) {
    t |> run("a field, swizzle or variant name at the end of an expression is part of its " +
        "text") @@(t : T?) {
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.export_all = true
                cop.lint_check = true
                cop.no_optimizations = true
                cop.no_infer_time_folding = true
                compile_file(fixture("_fixture_lint_fix_style_b_span.das"), access,
                             unsafe(addr(mg)), cop) $(ok; program; issues) {
                    if (!ok) {
                        t |> failure("span fixture failed to compile: {issues}")
                        return
                    }
                    var marks = new SpanMarks()
                    make_visitor(*marks) $(adapter) {
                        visit(program, adapter)
                    }
                    t |> equal(length(marks.found), 9)
                    for (m in marks.found) {
                        t |> equal(m.got, m.expected)
                    }
                    unsafe {
                        delete marks
                    }
                }
            }
        }
    }
}
