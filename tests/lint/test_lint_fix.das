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

[test(description = "applying the collected fix edits turns each before fixture into its after fixture and the after fixture carries no further fix")]
def test_lint_fix(t : T?) {
    t |> run("--fix edits turn the before fixture into the after fixture") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_before.das"),
                                   { "STYLE042", "STYLE043", "PERF033" })
        let fixed = apply_lint_fixes(fread(fixture("_fixture_lint_fix_before.das")), fixes)
        t |> equal(fixed.applied, 11)
        t |> equal(fixed.text, fread(fixture("_fixture_lint_fix_after.das")))
    }
    t |> run("the after fixture carries no fix") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_after.das"),
                                   { "STYLE042", "STYLE043", "PERF033" })
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
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_var_before.das"),
                                   { "LINT003", "LINT014" })
        let fixed = apply_lint_fixes(fread(fixture("_fixture_lint_fix_var_before.das")), fixes)
        t |> equal(fixed.applied, 5)
        t |> equal(fixed.text, fread(fixture("_fixture_lint_fix_var_after.das")))
    }
    t |> run("the var after fixture carries no fix") @@(t : T?) {
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_var_after.das"),
                                   { "LINT003", "LINT014" })
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
        let fixes <- collect_fixes(t, fixture("_fixture_lint_fix_stale_after.das"),
                                   { "LINT019", "STYLE042", "STYLE038", "PERF001" })
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
        let fixes <- [
                      LintFix(line = 1, column = 3, last_line = 1, last_column = 6,
                              replacement = "\n  x")]
        let fixed = apply_lint_fixes("abcdef\r\nnext\r\n", fixes)
        t |> equal(fixed.text, "abc\r\n  x\r\nnext\r\n")
    }
}

[test(description = "lint_has_comment detects comment openers and lint_indent_of counts leading spaces")]
def test_lint_text_helpers(t : T?) {
    t |> run("comment openers and indentation") @@(t : T?) {
        t |> equal(lint_has_comment("a // b"), true)
        t |> equal(lint_has_comment("a /* b */"), true)
        t |> equal(lint_has_comment("a + b"), false)
        t |> equal(lint_indent_of("    x"), 4)
    }
}

[test(description = "identical fixes from sibling instantiations apply once and merged edits become one fix over their span")]
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

def collect_paranoid_fixes(t : T?; path : string; enabled : table<string>) : array<LintFix> {
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

//! The before fixture fixed with the ``enabled`` rules matches the
//! after fixture, which compiles and carries no fix
def check_paranoid_pair(t : T?; name : string; enabled : table<string>; applied : int) {
    let before = fixture("_fixture_lint_fix_paranoid_{name}_before.das")
    let after = fixture("_fixture_lint_fix_paranoid_{name}_after.das")
    let fixes <- collect_paranoid_fixes(t, before, enabled)
    let fixed = apply_lint_fixes(fread(before), fixes)
    t |> equal(fixed.applied, applied)
    t |> equal(fixed.text, fread(after))
    let again <- collect_paranoid_fixes(t, after, enabled)
    t |> equal(length(again), 0)
}

[test(description = "the paranoid LINT fixes rename locals, rewrite casts, drop dead code and edit declarations as the after fixtures show")]
def test_lint_fix_paranoid(t : T?) {
    t |> run("LINT002 / LINT004 / LINT012 / LINT013 rename locals and arguments") @@(t : T?) {
        check_paranoid_pair(t, "names", { "LINT002", "LINT004", "LINT012", "LINT013" }, 8)
    }
    t |> run("LINT016 / LINT017 / LINT018 / LINT020 / LINT024 rewrite the narrowing and cloning " +
        "spellings") @@(t : T?) {
        check_paranoid_pair(t, "casts", { "LINT016", "LINT017", "LINT018", "LINT020", "LINT024" },
                            12)
    }
    t |> run("LINT001 / LINT008 / LINT009 drop dead or redundant code") @@(t : T?) {
        check_paranoid_pair(t, "dead", { "LINT001", "LINT008", "LINT009" }, 4)
    }
    t |> run("LINT019 / LINT022 / LINT031 edit declarations and directives") @@(t : T?) {
        check_paranoid_pair(t, "decls",
                   { "LINT002", "LINT006", "LINT007", "LINT008", "LINT019", "LINT022", "LINT031" },
            8)
    }
}

def collect_perf_fixes(t : T?; path : string; enabled : table<string>) : array<LintFix> {
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

def check_perf_pair(t : T?; before, after : string; enabled : table<string>; applied : int) {
    let fixes <- collect_perf_fixes(t, fixture(before), enabled)
    let fixed = apply_lint_fixes(fread(fixture(before)), fixes)
    t |> equal(fixed.applied, applied)
    t |> equal(fixed.text, fread(fixture(after)))
    let again <- collect_perf_fixes(t, fixture(after), enabled)
    t |> equal(length(again), 0)
}

[test(description = "expression-level PERF fixes rewrite the before fixture into the after fixture")]
def test_lint_fix_perf_expressions(t : T?) {
    t |> run("expression-level PERF fixes rewrite the before fixture into the after " +
        "fixture") @@(t : T?) {
        check_perf_pair(t, "_fixture_lint_fix_perf_before.das", "_fixture_lint_fix_perf_after.das",
            { "PERF003", "PERF007", "PERF010", "PERF011", "PERF013", "PERF014", "PERF015",
                "PERF016", "PERF017", "PERF019", "PERF020", "PERF021", "PERF025" }, 30)
    }
}

[test(description = "statement-level PERF fixes rewrite the before fixture into the after fixture")]
def test_lint_fix_perf_statements(t : T?) {
    t |> run("statement-level PERF fixes rewrite the before fixture into the after " +
        "fixture") @@(t : T?) {
        check_perf_pair(t, "_fixture_lint_fix_perf_stmt_before.das",
                   "_fixture_lint_fix_perf_stmt_after.das",
            { "PERF009", "PERF018", "PERF022", "PERF023", "PERF024", "PERF030", "PERF032" }, 13)
    }
}

def collect_style_a_fixes(t : T?; path : string; enabled : table<string>) : array<LintFix> {
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

def check_style_a_pair(t : T?; before, after : string; enabled : table<string>; applied : int) {
    let fixes <- collect_style_a_fixes(t, fixture(before), enabled)
    let fixed = apply_lint_fixes(fread(fixture(before)), fixes)
    t |> equal(fixed.applied, applied)
    t |> equal(fixed.text, fread(fixture(after)))
    let left <- collect_style_a_fixes(t, fixture(after), enabled)
    t |> equal(length(left), 0)
}

[test(description = "the pipe, brace and declaration STYLE fixes rewrite the before fixture into the after fixture")]
def test_lint_fix_style_pipes_braces_declarations(t : T?) {
    check_style_a_pair(t, "_fixture_lint_fix_style_a_before.das",
                       "_fixture_lint_fix_style_a_after.das",
        { "STYLE001", "STYLE002", "STYLE003", "STYLE005", "STYLE006", "STYLE010", "STYLE011",
            "STYLE012", "STYLE013" }, 20)
}

[test(description = "the guard and operator STYLE fixes rewrite the before fixture into the after fixture")]
def test_lint_fix_style_guards_and_operators(t : T?) {
    check_style_a_pair(t, "_fixture_lint_fix_style_a2_before.das",
                       "_fixture_lint_fix_style_a2_after.das",
        { "STYLE006", "STYLE016", "STYLE017", "STYLE018", "STYLE019", "STYLE020", "STYLE022",
            "STYLE023" }, 22)
}

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

def collect_style_b_fixes(t : T?; path : string; enabled : table<string>) : array<LintFix> {
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

def check_style_b_pair(t : T?; name : string; enabled : table<string>; applied : int) {
    let before = fixture("_fixture_lint_fix_{name}_before.das")
    let after = fixture("_fixture_lint_fix_{name}_after.das")
    let fixes <- collect_style_b_fixes(t, before, enabled)
    let fixed = apply_lint_fixes(fread(before), fixes)
    t |> equal(fixed.applied, applied)
    t |> equal(fixed.text, fread(after))
    let again <- collect_style_b_fixes(t, after, enabled)
    t |> equal(length(again), 0)
}

[test(description = "the STYLE unsafe, container and misc fixes rewrite their before fixtures into the after fixtures")]
def test_lint_fix_style_b(t : T?) {
    t |> run("STYLE024 / 025 / 026 / 034 / 036 drop or narrow unsafe wraps and cast " +
        "spellings") @@(t : T?) {
        check_style_b_pair(t, "unsafe",
                           { "STYLE024", "STYLE025", "STYLE026", "STYLE034", "STYLE036" }, 11)
    }
    t |> run("STYLE027 / 031 / 032 / 033 build containers in one expression") @@(t : T?) {
        check_style_b_pair(t, "containers", { "STYLE027", "STYLE031", "STYLE032", "STYLE033" }, 9)
    }
    t |> run("STYLE028 / 029 / 030 / 039 rewrite receivers, requires and string bytes") @@(t : T?) {
        check_style_b_pair(t, "misc", { "STYLE028", "STYLE029", "STYLE030", "STYLE039" }, 7)
    }
}

[test(description = "lint_expr_text keeps a trailing field, swizzle or variant name as part of an expression's text")]
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
