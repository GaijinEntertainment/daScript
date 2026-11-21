options gen2

require dastest/testing_boost
require daslib/ast
require daslib/lint
require strings
require daslib/strings_boost
require daslib/rtti
require daslib/style_lint
require daslib/lint_config

def fixture_path() : string => "{get_das_root()}/tests/lint/_fixture_nolint_lint003.das"

def hardening_fixture_path() : string => "{get_das_root()}/tests/lint/_fixture_nolint_hardening.das"

def compile_and_collect(t : T?; path : string;
                        blk : block<(issues : array<string>; count : int) : void>) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    t |> failure("fixture failed to compile: {issues}")
                    return
                }
                var collected : array<string>
                let count = paranoid_collect(program, collected)
                invoke(blk, collected, count)
            }
        }
    }
}

[test(description = "a nolint:LINT003 directive suppresses exactly the variables it marks")]
def test_nolint_lint003_suppression(t : T?) {
    t |> run("nolint:LINT003 suppresses the matched variable") @@(t : T?) {
        compile_and_collect(t, fixture_path()) $(issues; count) {
            // Three `var` declarations are LINT003-eligible; two carry
            // `// nolint:LINT003`, so exactly one should remain.
            t |> equal(count, 1)
            t |> equal(length(issues), 1)
            t |> success(find(issues[0], "LINT003") >= 0)
            t |> success(find(issues[0], "should_be_const") >= 0)
        }
    }
}

[test(description = "string literals, URLs and mid-comment spellings do not suppress a finding while first-token directives still do")]
def test_nolint_hardening(t : T?) {
    t |> run("string-literal, URL, and mid-comment spellings do not suppress") @@(t : T?) {
        compile_and_collect(t, hardening_fixture_path()) $(issues; count) {
            // Eight LINT003-eligible `var` declarations; five carry genuine
            // directives (incl. char-literal and escaped-quote lines), three
            // carry non-directive spellings that must NOT suppress.
            t |> equal(count, 3)
            let text = join(issues, "\n")
            t |> success(find(text, "in_string") >= 0, "string-literal case reported: {text}")
            t |> success(find(text, "in_url") >= 0, "URL case reported: {text}")
            t |> success(find(text, "mid_comment") >= 0, "mid-comment case reported: {text}")
            t |> success(find(text, "honored") < 0, "first-token directives still suppress: {text}")
            t |> success(find(text, "quote_") < 0, "quote-stress lines still suppress: {text}")
        }
    }
}

// Pins for three behaviors the comment-sweep branch changed, none of
// which the fixture corpus can distinguish (the dirty sources live as
// embedded strings — on-disk copies would fail the repo lint gate).

def defer_prefix_source() : string {
    return [
        /*  1 */ "options gen2",
        /*  2 */ "",
        /*  3 */ "def each_item(items : array<int>; blk : block<(x : int) : void>) \{",
        /*  4 */ "    for (i in items) \{",
        /*  5 */ "        invoke(blk, i)",
        /*  6 */ "    \}",
        /*  7 */ "\}",
        /*  8 */ "",
        /*  9 */ "[export]",
        /* 10 */ "def deferred_consumer() \{",
        /* 11 */ "    var deferred_items <- [1, 2, 3]",
        /* 12 */ "    var total = 0",
        /* 13 */ "    deferred_items |> each_item() <| $(x : int) \{",
        /* 14 */ "        total += x",
        /* 15 */ "    \}",
        /* 16 */ "\}"
    ] |> join("\n")
}

def lint022_latch_source() : string {
    return [
        /*  1 */ "options gen2",
        /*  2 */ "options _unused_private = true",
        /*  3 */ "def private dead_helper(x : int) : int \{",
        /*  4 */ "    return x * 2",
        /*  5 */ "\}",
        /*  6 */ "",
        /*  7 */ "[no_lint, export]",
        /*  8 */ "def trailing_shape() : int \{",
        /*  9 */ "    return 7",
        /* 10 */ "\}"
    ] |> join("\n")
}

def compile_source_and_collect(t : T?; name, source : string; style : bool;
                               blk : block<(issues : array<LintIssue>) : void>) {
    var inscope access <- make_file_access("")
    let path = "{get_das_root()}/tests/lint/{name}"
    set_file_source(access, path, source)
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    t |> failure("fixture failed to compile: {issues}")
                    return
                }
                let empty_set : table<string>
                var found : array<LintIssue>
                if (style) {
                    style_lint_collect_issues(program, found, empty_set, empty_set)
                } else {
                    paranoid_collect_issues(program, found, empty_set, empty_set)
                }
                invoke(blk, found)
            }
        }
    }
}

[test(description = "an identifier that merely starts with 'defer' adds no second finding on its line")]
def test_defer_scan_word_boundary(t : T?) {
    t |> run("an identifier starting with 'defer' adds no second finding on its line") @@(t : T?) {
        compile_source_and_collect(t, "_defer_prefix_fixture.das", defer_prefix_source(),
                                   true) $(issues) {
            var on_pipe_line = 0
            for (issue in issues) {
                if (issue.line == 13) {
                    on_pipe_line++
                    t |> equal(issue.code, "STYLE001")
                }
            }
            t |> equal(on_pipe_line, 1)
        }
    }
}

[test(description = "a trailing [no_lint] function does not silence the module's LINT022")]
def test_lint022_survives_trailing_no_lint(t : T?) {
    t |> run("a trailing [no_lint] function does not silence the module's LINT022") @@(t : T?) {
        compile_source_and_collect(t, "_lint022_latch_fixture.das", lint022_latch_source(),
                                   false) $(issues) {
            var lint022 = 0
            for (issue in issues) {
                if (issue.code == "LINT022" && find(issue.message, "dead_helper") >= 0) {
                    lint022++
                }
            }
            t |> equal(lint022, 1)
        }
    }
}

//! the error text of compiling `source`, empty when it compiles
def compile_source_errors(source : string; no_lint : bool; no_unsafe : bool = false) : string {
    var errors = ""
    var inscope access <- make_file_access("")
    let path = "{get_das_root()}/tests/lint/_lint_switch_fixture.das"
    set_file_source(access, path, source)
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.no_lint = no_lint
            cop.no_unsafe = no_unsafe
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; _program; issues) {
                errors = ok ? "" : "does not compile: {issues}"
            }
        }
    }
    return errors
}

def lint_switch_source(lint_off : bool; body : string) : string {
    let head = lint_off ? "options gen2\noptions lint = false\n" : "options gen2\n"
    return head + body
}

def lint_macro_finding_body() : string {
    return [
        "require daslib/lint",
        "def keep(used_arg : int; dead_arg : int) : int \{",
        "    return used_arg",
        "\}",
        "[export]",
        "def main : int \{",
        "    return keep(1, 2)",
        "\}"
    ] |> join("\n")
}

def unused_function_argument_body() : string {
    return [
        "options no_unused_function_arguments = true",
        "[sideeffects]",
        "def twice(a, b : int) \{",
        "    return a * 2",
        "\}",
        "[export]",
        "def main : int \{",
        "    return twice(1, 2)",
        "\}"
    ] |> join("\n")
}

def unused_block_argument_body() : string {
    return [
        "options no_unused_block_arguments = true",
        "def call(b : block<(x : int) : int>) : int \{",
        "    return invoke(b, 1)",
        "\}",
        "[export]",
        "def main : int \{",
        "    return call() $(x) \{",
        "        return 7",
        "    \}",
        "\}"
    ] |> join("\n")
}

def deprecated_call_body() : string {
    return [
        "options no_deprecated = true",
        "[deprecated(message = \"use fresh\")]",
        "def stale() : int \{",
        "    return 1",
        "\}",
        "[export]",
        "def main : int \{",
        "    return stale()",
        "\}"
    ] |> join("\n")
}

def nameless_write_body() : string {
    return [
        "struct Foo \{",
        "    a : int",
        "\}",
        "[export]",
        "def main : int \{",
        "    Foo(a = 1) = Foo(a = 2)",
        "    return 0",
        "\}"
    ] |> join("\n")
}

def falls_off_the_end_body() : string {
    return [
        "def pick(n : int) : int \{",
        "    if (n > 5) \{",
        "        return 2",
        "    \}",
        "\}",
        "[export]",
        "def main : int \{",
        "    return pick(3)",
        "\}"
    ] |> join("\n")
}

def expect_lint_switch_silences(t : T?; code, body : string) {
    let plain = compile_source_errors(lint_switch_source(false, body), false)
    t |> success(find(plain, code) >= 0, "with lint on the source reports {code}: {plain}")
    let by_option = compile_source_errors(lint_switch_source(true, body), false)
    t |> equal(by_option, "", "under options lint = false")
    let by_policy = compile_source_errors(lint_switch_source(false, body), true)
    t |> equal(by_policy, "", "under the no_lint policy")
}

[test(description = "options lint = false and the no_lint policy silence a lint macro's finding", timeout = 1200)]
def test_lint_switch_silences_lint_macros(t : T?) {
    t |> run("a LINT012 finding of daslib/lint") @@(t : T?) {
        expect_lint_switch_silences(t, "LINT012", lint_macro_finding_body())
    }
}

[test(description = "options lint = false and the no_lint policy turn off each of the compiler's four style rules", timeout = 1200)]
def test_lint_switch_turns_off_style_rules(t : T?) {
    t |> run("an unused function argument") @@(t : T?) {
        expect_lint_switch_silences(t, "30159", unused_function_argument_body())
    }
    t |> run("an unused block argument") @@(t : T?) {
        expect_lint_switch_silences(t, "30121", unused_block_argument_body())
    }
    t |> run("a call to a deprecated function") @@(t : T?) {
        expect_lint_switch_silences(t, "30927", deprecated_call_body())
    }
    t |> run("a write to a nameless temporary") @@(t : T?) {
        expect_lint_switch_silences(t, "30922", nameless_write_body())
    }
}

[test(description = "the no_lint policy keeps the error for a function that can end without a return", timeout = 1200)]
def test_no_lint_policy_keeps_semantic_checks(t : T?) {
    t |> run("a function that returns a value on one branch only") @@(t : T?) {
        let source = lint_switch_source(false, falls_off_the_end_body())
        let errors = compile_source_errors(source, true)
        t |> success(find(errors, "30310") >= 0, "the compile reports 30310: {errors}")
    }
}

def unsafe_block_body() : string {
    return [
        "[export]",
        "def main : int \{",
        "    var x = 1",
        "    unsafe \{",
        "        var p = addr(x)",
        "        *p = 2",
        "    \}",
        "    return x",
        "\}"
    ] |> join("\n")
}

def mutable_global_body() : string {
    return [
        "options no_global_variables = true",
        "var g_count = 5",
        "[export]",
        "def main : int \{",
        "    return g_count",
        "\}"
    ] |> join("\n")
}

[test(description = "with lint off the no_unsafe policy and the no_global_variables option still reject a program", timeout = 1200)]
def test_lint_switch_keeps_policy_checks(t : T?) {
    t |> run("an unsafe block under the no_unsafe policy") @@(t : T?) {
        let source = lint_switch_source(true, unsafe_block_body())
        let by_option = compile_source_errors(source, false, true)
        t |> success(find(by_option, "31012") >= 0, "options lint = false: {by_option}")
        let by_policy = compile_source_errors(source, true, true)
        t |> success(find(by_policy, "31012") >= 0, "the no_lint policy: {by_policy}")
    }
    t |> run("a mutable global under options no_global_variables") @@(t : T?) {
        let source = lint_switch_source(true, mutable_global_body())
        let errors = compile_source_errors(source, true)
        t |> success(find(errors, "30930") >= 0, "the compile reports 30930: {errors}")
    }
}
