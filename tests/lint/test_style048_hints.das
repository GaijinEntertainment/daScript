options gen2

require dastest/testing_boost public
require daslib/ast
require daslib/rtti
require daslib/style_lint
require daslib/lint_config

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

[test]
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

[test]
def test_style048_needs_match_required(t : T?) {
    let found <- style048_findings(t, "_fixture_style048_no_match.das")
    let what = "a module that does not require daslib/match gets no match suggestion"
    t |> equal(length(found), 0, what)
}
