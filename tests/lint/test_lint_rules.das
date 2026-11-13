options gen2

require dastest/testing_boost public
require daslib/lint
require daslib/ast
require strings
require daslib/fio
require daslib/strings_boost
require daslib/rtti
require daslib/perf_lint
require daslib/lint_config
require daslib/style_lint

// LINT002 false positive: invoke devirtualization splices the block
// literal in place and eats the holder's only read. Lint runs after
// the patch slot but before optimize reaps the dead `let`, so it saw
// a variable with no access flag and reported error[50503]. The
// devirtualizer now marks such holders marked_used. A
// function-pointer local never devirtualizes, so it was always clean
// - that asymmetry is what made this look block-typed-specific.

def apply_via_invoke(x : int) : int {
    let add_one = $(v : int) : int { return v + 1 }
    return invoke(add_one, invoke(add_one, x))
}

def apply_block(blk : block<(v : int) : int>; x : int) : int => invoke(blk, x)

def apply_via_call(x : int) : int {
    let add_two = $(v : int) : int { return v + 2 }
    return apply_block(add_two, x)
}

[test(description = "a block variable read only by invoke is not reported as unused after devirtualization")]
def test_block_var_read_by_invoke(t : T?) {
    t |> equal(apply_via_invoke(1), 3)
}

[test(description = "a block variable passed to a function is not reported as unused")]
def test_block_var_passed_to_function(t : T?) {
    t |> equal(apply_via_call(1), 3)
}

def assume_fixture_path() : string =>
    "{get_das_root()}/tests/lint/_assume_block_lint013_fixture.das"


[test(description = "LINT013 does not fire on an assume-aliased block whose arguments are all used")]
def test_assume_block_no_false_lint013(t : T?) {
    t |> run("LINT013 does not false-fire on assume-aliased block whose args are used") @@(t : T?) {
        compile_and_collect(t, assume_fixture_path()) $(issues; count) {
            var lint013 = 0
            for (issue in issues) {
                if (find(issue, "LINT013") >= 0) {
                    lint013 ++
                }
            }
            // The alias-definition blocks read every argument; none must be
            // flagged.
            t |> equal(lint013, 0)
        }
    }
}

def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

def run_argv(args : array<string>; var output : string&) : int {
    return unsafe(popen_argv(args, 0.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

[test(description = "LINT010 treats stores read through a goto as live and still reports a jump-free dead store")]
def test_lint010_goto(t : T?) {
    let root = get_das_root()
    let exe = das_exe()
    if (empty(exe)) {
        t |> failure("could not resolve the daslang binary from argv")
        return
    }
    t |> run("stores read through a goto are not dead; a jump-free dead store still is") @(t : T?) {
        let fixture = "{root}/tests/lint/_fixture_lint010_goto.das"
        var output = ""
        run_argv(
                 [exe, "-dasroot", root, "{root}/utils/lint/main.das", "--", fixture,
                  "--lint-fixtures", "--lint-excluded-paths"], output)
        t |> success(find(output, "LINT010") >= 0, "the jump-free dead store is reported: {output}")
        t |> success(find(output, "dead store of 't'") >= 0,
                     "the finding names the control's variable: {output}")
        t |> success(find(output, "dead store of 'res'") < 0,
                     "stores before a forward or computed goto are not dead: {output}")
        t |> success(find(output, "dead store of 'acc'") < 0
                     && find(output, "dead store of 'i'") < 0,
            "an overwrite under a backward-jump label is not dead: {output}")
        t |> success(find(output, "dead store of 'late'") < 0,
                     "a variable declared under a label starts skipped: {output}")
    }
}

def lint031_fixture_path(name : string) : string => "{get_das_root()}/tests/lint/{name}"

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

[test(description = "a LINT031 finding anchors at the parameter's line and names the line of the write")]
def test_lint031_anchor(t : T?) {
    t |> run("the finding anchors at the parameter's line and names the write's line") @@(t : T?) {
        compile_and_collect(t,
                            lint031_fixture_path("_fixture_lint031_writer.das")) $(issues; count) {
            t |> equal(count, 1)
            t |> equal(length(issues), 1)
            t |> success(find(issues[0], "LINT031") >= 0, issues[0])
            t |> success(find(issues[0], "_fixture_lint031_writer.das:12:") >= 0,
                         "anchored at `def route(dst : int?)`, line 12: {issues[0]}")
            t |> success(find(issues[0], "at line 14") >= 0,
                         "the text names the write's line 14: {issues[0]}")
        }
    }
}

[test(description = "a callee's LINT031 verdict does not outlive its program into the next compile")]
def test_lint031_callee_memo_is_per_program(t : T?) {
    t |> run("a callee's verdict does not outlive its program") @@(t : T?) {
        compile_and_collect(t,
                            lint031_fixture_path("_fixture_lint031_writer.das")) $(issues; count) {
            t |> equal(count, 1, "the writing fill_one flags its caller")
        }
        compile_and_collect(t,
                            lint031_fixture_path("_fixture_lint031_reader.das")) $(issues; count) {
            t |> equal(count, 0, "the same name with a reading body, compiled next in this process, flags nothing: {join(issues, "; ")}")
        }
    }
}

[test(description = "every LINT034 shape is reported on its own marked line, and no unmarked line is")]
def test_lint034_sites(t : T?) {
    t |> run("each line marked LINT034 gets one finding, the rest get none") @@(t : T?) {
        let path = lint031_fixture_path("_fixture_lint034_scratch_borrowed_pointer.das")
        compile_and_collect(t, path) $(issues; count) {
            t |> success(count > 0, "the fixture reports")
            var found <- [for (issue in issues); issue; where find(issue, "LINT034") >= 0]
            var marked = 0
            var lines <- split(fread(path), "\n")
            for (text, i in lines, count(1)) {
                let here_mark = ".das:{i}:"
                var inscope at_line <- [for (issue in found); issue;
                    where find(issue, here_mark) >= 0]
                let here = length(at_line)
                let wants = find(text, "// LINT034") >= 0 ? 1 : 0
                marked += wants
                t |> equal(here, wants, "line {i}: {text}")
            }
            t |> success(marked > 30, "the fixture carries its marks ({marked})")
            t |> equal(length(found), marked, "one finding per marked line: {join(found, "; ")}")
            delete lines
            delete found
        }
    }
}

// PERF030 false positive: a `delete v` releases the old contents, so
// a following `v <- ...` leaks nothing. One release covers one move -
// the second move in the fixture is reported, because the first one
// consumed it.
def fixture_source() : string {
    return [
        /*  1 */ "options gen2",
        /*  2 */ "",
        /*  3 */ "[export]",
        /*  4 */ "def fixture_function() : int \{",
        /*  5 */ "    var fblob <- [1, 2, 3]",
        /*  6 */ "    var nf <- [4, 5]",
        /*  7 */ "    var kept <- [6]",
        /*  8 */ "    delete fblob",
        /*  9 */ "    fblob <- nf",
        /* 10 */ "    fblob <- kept",
        /* 11 */ "    return length(fblob)",
        /* 12 */ "\}"
    ] |> join("\n")
}

def perf030_fixture_path() : string =>
    "{get_das_root()}/tests/lint/_perf030_delete_before_move_fixture.das"  // virtual - not on disk

[test(description = "PERF030 skips a move whose target was just released by delete and reports the next move")]
def test_perf030_skips_move_after_delete(t : T?) {
    t |> run("PERF030 fires only for a move whose target was not released") @@(tt : T?) {
        var inscope access <- make_file_access("")
        set_file_source(access, perf030_fixture_path(), fixture_source())
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.export_all = true
                cop.lint_check = true
                cop.no_optimizations = true
                cop.no_infer_time_folding = true
                compile_file(perf030_fixture_path(), access, unsafe(addr(mg)),
                             cop) $(ok; program; issues) {
                    if (!ok) {
                        tt |> failure("fixture failed to compile: {issues}")
                        return
                    }
                    let empty_set : table<string>
                    var perf_issues : array<LintIssue>
                    perf_lint_collect_issues(program, perf_issues, empty_set, empty_set)
                    var reported = 0
                    var reported_line = -1
                    for (i in perf_issues) {
                        if (i.code == "PERF030") {
                            reported++
                            reported_line = i.line
                        }
                    }
                    // line 9 is covered by the delete above it; line 10 is not - the move
                    // at 9 consumed the release
                    tt |> equal(reported, 1)
                    tt |> equal(reported_line, 10)
                }
            }
        }
    }
}

def private unsafe_fixture_path() : string =>
    "{get_das_root()}/tests/lint/_unsafe_block_scope_fixture.das"

def private style_codes(t : T?; fold : bool) : array<string> {
    var codes : array<string>
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            cop.no_fold_unsafe = !fold
            compile_file(unsafe_fixture_path(), access, unsafe(addr(mg)),
                         cop) $(ok; program; issues) {
                if (!ok) {
                    t |> failure("fixture failed to compile: {issues}")
                    return
                }
                let empty_set : table<string>
                var style_issues : array<LintIssue>
                style_lint_collect_issues(program, style_issues, empty_set, empty_set)
                for (issue in style_issues) {
                    codes |> push(issue.code)
                }
            }
        }
    }
    return <- codes
}

[test(description = "with no_fold_unsafe the block-form unsafe rules see the wrapper and without it they go silent")]
def test_unsafe_block_scope(t : T?) {
    t |> run("with no_fold_unsafe the block-form scope rules see the wrapper") @@(t : T?) {
        let codes <- style_codes(t, false)
        t |> equal(join(codes, ","), "STYLE025",
                   "one unsafe op in the block narrows to unsafe(<sub-expr>)")
    }
    t |> run("without it foldUnsafe erases the wrapper before the walk, and the rules go silent") @@(t : T?) {
        let codes <- style_codes(t, true)
        t |> equal(join(codes, ","), "", "no scope left to report on")
    }
}

let DEAD_ARM = ("STYLE048: this if/elif arm can never run; " +
                "the arms before it already take every value it tests")

def hint(text : string) : string {
    let what = "if/elif chain compares the same values against constants"
    return "STYLE048: {what}; rewrite it as {text}"
}

def expected_findings() : array<tuple<line : int; text : string>> {
    //! Every finding the fixture produces, by line - a function with
    //! none is a decision the rule makes to stay silent.
    return <- [
        (line = 33, text = hint("match (m) \{ Mode.idle / Mode.run / Mode.stop \}")),
        (line = 45, text = hint("match (m, n) \{ (Mode.idle, 0..10) / " +
                                "(Mode.run, _) | (Mode.stop, 5) / (_, 100) / _ \}")),
        (line = 59, text = hint("match (s) \{ circle(...) / square(...) \}")),
        (line = 69, text = hint("match (a, b) \{ (1, 2) / (1, 3) / (1, _) => \{\} / (2, _) / " +
                                "_ => \{\} \} [nested if flattened]")),
        (line = 83, text = hint("match (a, b) \{ (1, true) / (1, false) / (2, _) / " +
                                "_ => \{\} \} [nested if flattened]")),
        (line = 99, text = DEAD_ARM),
        (line = 109, text = hint("match (m) \{ Mode.idle / Mode.run / " +
                                 "Mode.stop \} [the else branch never runs]")),
        (line = 123, text = hint("match (k, op) \{ (0, \"==\" | \"<=\") / (1, \"<\") / " +
                                 "(1, \"==\") / _ => \{\} \}")),
        (line = 137, text = hint("match (k, op) \{ (0, \"x\") / (_, \"a\") / (1, _) / " +
                                 "_ => \{\} \}")),
        (line = 149, text = hint("match (k, op) \{ (0, \"a\" | \"b\") / (1, \"a\") / " +
                                 "(2, \"c\") / _ => \{\} \}")),
        (line = 161, text = hint("match (n) \{ 0..10 / 10..100 / 100 / _ => \{\} \}")),
        (line = 173, text = hint("match (n) \{ 0..0x100 / 0x100..0x200 / 0x200 / _ => \{\} \}")),
        (line = 185, text = hint("match (n) \{ 10u..20u / 20u..30u / 30u / _ => \{\} \}")),
        (line = 197, text = hint("match (verb) \{ \"GET\" / \"PUT\" / \"DELETE\" / _ => \{\} \}")),
        (line = 209, text = hint("match (on, m) \{ (true, Mode.idle) / (false, Mode.idle) / " +
                                 "(true, Mode.run) / (false, Mode.run) / " +
                                 "_ => \{\} \} [unmatched: " +
                                 "`(true, Mode.stop)`, `(false, Mode.stop)`]")),
        (line = 223, text = hint("match (m, n) \{ (Mode.idle, 0) / (Mode.idle, 1) / " +
                                 "(Mode.run, _) / _ => \{\} \}")),
        (line = 235, text = hint("match (w) \{ Wide.a | Wide.b / Wide.c | Wide.d / " +
                                 "Wide.e | Wide.f / _ => \{\} \} [unmatched: `Wide.g`]")),
        (line = 247, text = hint("match (w) \{ Wide.a / Wide.b / Wide.c / _ => \{\} \}")),
        (line = 259, text = hint("match (a) \{ 1 && <guard> / 2 / 3 / _ => \{\} \}")),
        (line = 271, text = hint("match (u) \{ _ && <guard> / 1ul / 2ul / 3ul / _ => \{\} \}")),
    ]
}

def style048_findings(t : T?; fixture : string) : array<LintIssue> {
    //! The STYLE048 findings of one fixture under tests/lint.
    var found : array<LintIssue>
    let path = "{get_das_root()}/tests/lint/{fixture}"
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    t |> failure("the fixture failed to compile: {issues}")
                    return
                }
                let nothing : table<string>
                let only <- { "STYLE048" }
                style_lint_collect_issues(program, found, nothing, only)
            }
        }
    }
    return <- found
}

[test(description = "STYLE048 reports every if/elif chain over constants with its exact match rewrite and dead-arm text, and none from the quiet functions")]
def test_style048_hints(t : T?) {
    let found <- style048_findings(t, "_fixture_style048_hints.das")
    let expected <- expected_findings()
    let what = "one finding per reporting function, none from the quiet ones"
    t |> equal(length(found), length(expected), what)
    for (want in expected) {
        let at = find_index_if(found) $(f) => f.line == want.line
        if (at < 0) {
            t |> failure("no finding at line {want.line}, expected: {want.text}")
            continue
        }
        t |> equal(found[at].message, want.text, "the finding at line {want.line}")
    }
}

[test(description = "STYLE048 suggests no match rewrite in a module that does not require daslib/match")]
def test_style048_needs_match_required(t : T?) {
    let found <- style048_findings(t, "_fixture_style048_no_match.das")
    let what = "a module that does not require daslib/match gets no match suggestion"
    t |> equal(length(found), 0, what)
}
