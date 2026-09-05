options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/strings_boost

require ../config.das

[test]
def test_has_path_under(t : T?) {
    let prefix = "modules/dasLLAMA/"
    t |> success(has_path_under(["src/ast/ast_parse.cpp", "modules/dasLLAMA/tests/run.das"], prefix), "a path under the prefix arms the gate")
    t |> success(!has_path_under(["modules/dasLLVM/daslib/llvm_jit.das", "doc/modules/dasLLAMA/x.rst"], prefix), "a look-alike elsewhere in the path does not")
    let none : array<string>
    t |> success(!has_path_under(none, prefix), "an empty diff does not")
    t |> success(has_path_under(["modules/dasLLAMA/REVIEW.md\r"], prefix), "a CR-terminated line still counts")
}

// A gate's reach: no reach set means the gate runs whatever changed; a reach set is hit by its own
// prefixes or by the core, and missed by everything else.
[test]
def test_reach_hit(t : T?) {
    let none : array<string>
    let imgui <- ["modules/dasImgui/", "modules/dasGlfw/"]
    t |> success(reach_hit(["plans/x.md"], none), "an empty reach set runs on any change")
    t |> success(reach_hit(["modules/dasImgui/daslib/imgui_boost.das"], imgui), "a path under the gate's own prefix reaches it")
    t |> success(reach_hit(["src/ast/ast_parse.cpp"], imgui), "a core change reaches every gate")
    t |> success(reach_hit(["CMakeLists.txt"], imgui), "the top-level CMakeLists is core")
    t |> success(!reach_hit(["modules/dasLLAMA/tests/run.das", "plans/x.md"], imgui), "a change elsewhere misses the gate")
    t |> success(!reach_hit(none, imgui), "an empty diff misses every reach set")
}

// The compile sweep's root filter: program roots under the swept trees, never tests, generated files,
// fixtures, or a module tree nested inside an example through a junction.
[test]
def test_is_sweep_root_path(t : T?) {
    t |> success(is_sweep_root_path("utils/lint/main.das"), "a tool's main is a root candidate")
    t |> success(is_sweep_root_path("modules/dasImgui/examples/features/active_widget.das"), "a module's own example is a candidate")
    t |> success(is_sweep_root_path("tutorials/language/01_hello.das"), "a tutorial is a candidate")
    t |> success(!is_sweep_root_path("utils/lint/tests/lint006_division_by_zero.das"), "a tests folder is not swept")
    t |> success(!is_sweep_root_path("examples/x/test_thing.das"), "a test_ file is not swept")
    t |> success(!is_sweep_root_path("examples/x/_fixture.das"), "an underscore fixture is not swept")
    t |> success(!is_sweep_root_path("tutorials/_aot_generated/x.das"), "generated files are not swept")
    t |> success(!is_sweep_root_path("examples/daStrudel/sfx_lab/modules/dasImgui/bind/bind_imgui.das"), "a module tree reached through an example's junction is not swept")
    t |> success(!is_sweep_root_path("utils/lint/README.md"), "only .das files are candidates")
}

// A lane child's report: the verdict lines with their seconds, details and indented breakdown; RUN
// lines and prose are not verdicts; a docs child reports several gates.
[test]
def test_parse_gate_reports(t : T?) {
    let out = "preflight: fast tier; daslang=bin/daslang\n[RUN ] tests-jit\n[PASS] tests-jit (396.2s) — JIT suite ok\n       Top 3 slowest files:\n       12.0s  tests/a.das\n\npreflight: 1 passed\n"
    let one <- parse_gate_reports(out)
    t |> equal(length(one), 1, "one verdict line")
    t |> equal(one[0].tag, "PASS", "the tag")
    t |> equal(one[0].name, "tests-jit", "the gate name, without the seconds")
    t |> success(one[0].seconds > 396.0lf && one[0].seconds < 396.4lf, "the seconds")
    t |> equal(one[0].detail, "JIT suite ok", "the detail after the dash")
    t |> equal(one[0].breakdown, "Top 3 slowest files:\n12.0s  tests/a.das", "the indented breakdown, trimmed")
    let docs = "[RUN ] docs\n[PASS] das2rst (12.0s)\n[SKIP] sphinx — sphinx-build not found\n[FAIL] stubs (0.3s) — 2 stub(s) unfilled\n"
    let many <- parse_gate_reports(docs)
    t |> equal(length(many), 3, "a docs child reports each sub-gate")
    t |> equal(many[1].tag, "SKIP", "a skip without seconds parses")
    t |> equal(many[1].name, "sphinx", "its name")
    t |> equal(many[1].detail, "sphinx-build not found", "its reason")
    t |> equal(many[2].tag, "FAIL", "a fail parses")
    t |> equal(length(parse_gate_reports("died before reporting\n")), 0, "no verdict line yields no report")
    let logged = "[I] [RUN ] tests-cpp\n[I] [PASS] tests-cpp (20.1s)\n[I]        Top 1 slowest files:\n[I]        2.0s  tests/b.das\n[I] \n[W] [SKIP] imgui — no display\n"
    let tagged <- parse_gate_reports(logged)
    t |> equal(length(tagged), 2, "a to_log level prefix on every captured line is not the verdict tag")
    t |> equal(tagged[0].name, "tests-cpp", "the gate name behind the [I] prefix")
    t |> equal(tagged[0].breakdown, "Top 1 slowest files:\n2.0s  tests/b.das", "the breakdown behind the prefix, trimmed")
    t |> equal(tagged[1].tag, "SKIP", "a warning-level skip line parses")
    // the edges: a whitespace-only indented line ends the breakdown (a blank line does), a `[` line with no
    // `]` is prose, a name with ` (` that is not a seconds group keeps it, and a child cut off mid-breakdown
    // still closes its report
    let edges = "[PASS] lint (2.0s)\n       Top 1 slowest files:\n            \n       1.0s  x.das\n[bracket without close\n[PASS] docs (sphinx) — ok\n[FAIL] tests-jit (9.5s)\n       12.0s  tests/z.das"
    let e <- parse_gate_reports(edges)
    t |> equal(length(e), 3, "three verdicts")
    t |> equal(e[0].breakdown, "Top 1 slowest files:", "a whitespace-only indented line ends the breakdown, not a blank row in it")
    t |> equal(e[1].name, "docs (sphinx)", "a parenthesis that is not a seconds group stays in the name")
    t |> success(e[1].seconds == 0.0lf, "no seconds parsed from it")
    t |> equal(e[2].breakdown, "12.0s  tests/z.das", "the last report closes without a trailing newline")
}

[test]
def test_tier_runs(t : T?) {
    t |> success(tier_runs("fast", false), "the fast tier runs without --full")
    t |> success(tier_runs("fast", true), "and with it")
    t |> success(!tier_runs("lane", false), "a lane waits for --full")
    t |> success(tier_runs("lane", true), "and runs under it")
    t |> success(!tier_runs("module", false) && !tier_runs("module", true), "a module gate never runs from a tier")
}

[test]
def test_sweep_excluded(t : T?) {
    t |> success(sweep_excluded("examples/crash/crash.das"), "an excluded tree's root")
    t |> success(sweep_excluded("utils/internal/ast-fuzz/selftest/broken.das"), "the fuzz fixtures")
    t |> success(!sweep_excluded("examples/crashes/x.das"), "a prefix match is on the whole path segment")
    t |> success(!sweep_excluded("utils/lint/main.das"), "a swept tool root")
    for (why in values(SWEEP_EXCLUDED)) {
        t |> success(!empty(why), "every exclusion carries its reason")
    }
}

[test]
def test_sweep_root_text(t : T?) {
    t |> success(is_sweep_root_text("options gen2\n[export]\ndef main() \{\n\}\n"), "an exported main is a root")
    t |> success(!is_sweep_root_text("options gen2\ndef helper() \{\n\}\n"), "a module without main is not")
    t |> success(!is_sweep_root_text("def main() \{\n\}\n"), "an unexported main is not a program root")
    t |> success(is_heavy_sweep_root("require dasllama/dasllama\n[export]\ndef main() \{\}\n"), "an engine require is heavy")
    t |> success(!is_heavy_sweep_root("require daslib/fio\n[export]\ndef main() \{\}\n"), "a plain root is light")
}

[test]
def test_failed_files_block(t : T?) {
    let one = "13205 tests, 13201 passed, 1 failed\n\nFAILURES:\n  tests/json/test_sscan_json.das — 1 failed, 0 errors\n\nFAILED! (109.3s)\n"
    t |> equal(failed_files_block(one), "tests/json/test_sscan_json.das", "the file before the dash")
    t |> equal(failed_files_block("all green\n"), "the suite (no FAILURES block in the output)", "no block, a stated default")
    let rows <- [for (i in range(7)); "  tests/t{i}.das — 1 failed"]
    let many = "FAILURES:\n" + join(rows, "\n") + "\n"
    t |> equal(failed_files_block(many), "tests/t0.das, tests/t1.das, tests/t2.das, tests/t3.das, tests/t4.das, and 2 more", "five named, the rest counted")
}
