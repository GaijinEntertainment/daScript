options gen2
options rtti

require dastest/testing_boost public
require daslib/json
require daslib/json_boost
require strings
require daslib/fio
require _tools_common

require tools/lint_tool public

//! The fix ``replacement`` of the first ``code`` finding in the lint
//! JSON document ``doc``; "" when there is none.
def private json_fix_for(doc : JsonValue?; code : string) : string {
    let results = doc?["results"]
    if (results == null || !(results.value is _array)) return ""
    for (r in results.value as _array) {
        let issues = r?["issues"]
        continue if (issues == null || !(issues.value is _array))
        for (i in issues.value as _array) {
            if ((i?["code"] ?? "") == code) return i?["fix"]?["replacement"] ?? ""
        }
    }
    return ""
}

[test(description = "lint reports a PERF001 finding in a fixture that has one.", timeout = 1200)]
def test_perf_lint_warnings(t : T?) {
    t |> run("detects PERF001 warning") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_perf_lint.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "PERF001") >= 0, "should contain PERF001")
        t |> success(find(text, "lint issue") >= 0, "should report lint issues")
    }
}

[test(description = "lint with fix rewrites the file as the runner does and refuses a fix run that names a project.", timeout = 1200)]
def test_lint_fix(t : T?) {
    t |> run("fix rewrites the file the way the runner's --fix does") <| @(t : T?) {
        var terr : string
        let dir = make_policy_dir("mcp_lint_fix", "", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        let dst = path_join(dir, "victim.das")
        fwrite(dst, "options gen2\n\n[export]\ndef main() \{\n" +
            "    var a <- [1, 2]\n    print(\"\{a\}\\n\")\n\}\n")
        var text : string
        var is_error = false
        let fixed = do_lint(dst, "", "", array<string>(), true)
        t |> success(parse_result(fixed, text, is_error), "parse result JSON")
        t |> success(!is_error, "a fix run is not an error -- text={text}")
        t |> success(find(fread(dst), "let a <- [1, 2]") >= 0, "the var became let")
        var perr = false
        let refused = do_lint(dst, "x.das_project", "", array<string>(), true)
        parse_result(refused, text, perr)
        t |> success(perr, "fix with a project is an error")
    }
}

[test(description = "lint json mode returns every finding with its fix, leaves the file alone, and rewrites it only when fix is also set.", timeout = 1200)]
def test_lint_json(t : T?) {
    t |> run("json returns every finding with its fix and leaves the file alone") <| @(t : T?) {
        var terr : string
        let dir = make_policy_dir("mcp_lint_json", "", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        let dst = path_join(dir, "victim.das")
        let src = ("options gen2\n\n[export]\ndef main() \{\n" +
            "    var a <- [1, 2]\n    print(\"\{a\}\\n\")\n\}\n")
        fwrite(dst, src)
        var text : string
        var is_error = false
        let report = do_lint(dst, "", "", array<string>(), false, true)
        t |> success(parse_result(report, text, is_error), "parse result JSON")
        t |> success(!is_error, "a lint with findings is not an error -- text={text}")
        var err : string
        var doc = read_json(text, err)
        t |> success(doc != null, "the report parses: {err}")
        if (doc != null) {
            t |> equal(json_fix_for(doc, "LINT003"), "let")
            t |> equal(doc?["totals"]?["fixable"] ?? 0, 1)
            unsafe { delete doc; }
        }
        t |> equal(fread(dst), src)
        let fixed = do_lint(dst, "", "", array<string>(), true, true)
        t |> success(parse_result(fixed, text, is_error), "parse fix result JSON")
        var after = read_json(text, err)
        t |> success(after != null, "the fix report parses: {err}")
        if (after != null) {
            t |> equal(after?["totals"]?["issues"] ?? -1, 0)
            unsafe { delete after; }
        }
        t |> success(find(fread(dst), "let a <- [1, 2]") >= 0, "json with fix rewrote the file")
    }
}

[test(description = "lint says there are no issues for a clean file.", timeout = 1200)]
def test_perf_lint_clean(t : T?) {
    t |> run("clean file has no warnings") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "No lint issues") >= 0, "should say no issues")
    }
}

[test(description = "lint reports a file that does not compile as a failed compilation.", timeout = 1200)]
def test_perf_lint_compile_error(t : T?) {
    t |> run("compile error reports failure") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_error.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "Compilation failed") >= 0, "should contain 'Compilation failed'")
    }
}

[test(description = "lint reports a glob that matches nothing as an error.", timeout = 1200)]
def test_perf_lint_no_match(t : T?) {
    t |> run("no files matched reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint("nonexistent_dir_xyz/*.das"), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "no files matched") >= 0, "should mention no files matched")
    }
}

[test(description = "lint detects an unused variable and unreachable code.", timeout = 1200)]
def test_lint_issues(t : T?) {
    t |> run("detects unused variable and unreachable code") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_lint.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "unused variable") >= 0, "should detect unused variable")
        t |> success(find(text, "unreachable code") >= 0, "should detect unreachable code")
    }
}

[test(description = "lint reports a stale nolint directive as LINT019 in single-file and batch runs.", timeout = 1200)]
def test_lint_stale_nolint(t : T?) {
    t |> run("stale nolint reported via LINT019") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_stale_nolint.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "LINT019") >= 0, "should report the stale directive")
        t |> success(find(text, "PERF006") >= 0, "should name the stale code")
    }
    t |> run("stale nolint reported in the multi-file path too") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint("{fixture_path("_fixture_stale_nolint.das")},{fixture_path("_fixture_valid.das")}"), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(find(text, "LINT019") >= 0, "batch path runs the stale scan too")
    }
}

[test(description = "lint finds nothing to report in a clean fixture.", timeout = 1200)]
def test_lint_clean(t : T?) {
    t |> run("clean file has no lint issues") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "No lint issues") >= 0, "should say no issues")
    }
}

[test(description = "lint reports a compile error as a failed compilation.", timeout = 1200)]
def test_lint_compile_error(t : T?) {
    t |> run("compile error reports failure") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_error.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "Compilation failed") >= 0, "should contain 'Compilation failed'")
    }
}

[test(description = "lint skips files that declare compile-error expectations or a skip header, with a valid result envelope.", timeout = 1200)]
def test_lint_skip_expect(t : T?) {
    t |> run("file with `expect` directive is skipped via a VALID result envelope") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_expect.das")), text, is_error)
        t |> success(ok, "SKIP result must be a valid envelope (was a raw string => invalid JSON-RPC)")
        t |> success(!is_error, "skip is not an error")
        t |> success(find(text, "SKIP") >= 0, "should report SKIP")
        t |> success(find(text, "intentional compile errors") >= 0, "should explain the skip reason")
    }
    t |> run("a `lint-skip-file` header skips instead of hard-failing on policy-conflicted files") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint("{get_das_root()}/tests/language/dead_branch_folding.das"),
            text, is_error)
        t |> success(ok, "skip result is a valid envelope")
        t |> success(!is_error, "skip is not an error")
        t |> success(find(text, "SKIP") >= 0, "should report SKIP: {text}")
        t |> success(find(text, "lint-skip-file") >= 0, "should carry the directive reason: {text}")
    }
}

[test(description = "lint says no files matched for a glob with no matches.", timeout = 1200)]
def test_lint_no_match(t : T?) {
    t |> run("no files matched reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint("nonexistent_dir_xyz/*.das"), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "no files matched") >= 0, "should mention no files matched")
    }
}
