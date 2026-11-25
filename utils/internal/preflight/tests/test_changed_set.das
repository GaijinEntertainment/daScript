options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/strings_boost

require ../config.das

[test(description = "a changed path under the prefix arms the gate, CR-terminated or not, while a look-alike elsewhere in the path or an empty diff does not")]
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
[test(description = "an empty reach set runs on any change, and a reach set is hit only by its own prefixes or a core change")]
def test_reach_hit(t : T?) {
    let none : array<string>
    let imgui <- ["modules/dasImgui/", "modules/dasGlfw/"]
    t |> success(reach_hit(["x.md"], none), "an empty reach set runs on any change")
    t |> success(reach_hit(["modules/dasImgui/daslib/imgui_boost.das"], imgui), "a path under the gate's own prefix reaches it")
    t |> success(reach_hit(["src/ast/ast_parse.cpp"], imgui), "a core change reaches every gate")
    t |> success(reach_hit(["CMakeLists.txt"], imgui), "the top-level CMakeLists is core")
    t |> success(!reach_hit(["modules/dasLLAMA/tests/run.das", "x.md"], imgui), "a change elsewhere misses the gate")
    t |> success(!reach_hit(none, imgui), "an empty diff misses every reach set")
}

[test(description = "the review-md suites run on a change to any REVIEW.das gate, the walkers or the gate library, and not on a checklist, a tool edit or a look-alike name")]
def test_review_md_tests_reach(t : T?) {
    t |> success(review_md_tests_reach(["utils/REVIEW.das"]), "a gate under utils/ reaches the suites")
    t |> success(review_md_tests_reach(["modules/dasLLAMA/REVIEW.das", "x.md"]), "a module's gate reaches them")
    t |> success(review_md_tests_reach(["utils/internal/review-md/gate.das"]), "the walkers reach them")
    t |> success(review_md_tests_reach(["dastest/review_gate.das"]), "the gate library reaches them")
    t |> success(!review_md_tests_reach(["utils/watchdog/REVIEW.md", "utils/watchdog/watchdog.das"]), "a checklist or a tool edit misses them")
    t |> success(!review_md_tests_reach(["modules/dasLLAMA/dasllama/PREVIEW.das"]), "a name that merely ends in the letters misses them")
}

// The compile sweep's root filter: program roots under the swept trees, never tests, generated files,
// fixtures, or a module tree nested inside an example through a junction.
[test(description = "the compile sweep takes tool mains, examples and tutorials as roots and skips tests, test_ files, fixtures, generated files, junctioned module trees and non-.das files")]
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
[test(description = "a lane child's output parses into one report per verdict line with its seconds, detail and trimmed breakdown, behind a log-level prefix too, and RUN lines and prose yield none")]
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

[test(description = "a gate --only names runs whatever changed, and a gate it does not name runs only when the diff reaches it")]
def test_gate_reached(t : T?) {
    let imgui <- ["modules/dasImgui/"]
    let elsewhere <- ["x.md"]
    t |> success(gate_reached(true, elsewhere, imgui), "a named gate runs on a diff that misses it")
    t |> success(!gate_reached(false, elsewhere, imgui), "an unnamed gate does not")
    t |> success(gate_reached(false, ["modules/dasImgui/a.das"], imgui), "it runs on a reach hit")
}

[test(description = "the fast tier always runs, a lane runs only under --full, and a module gate never runs from a tier")]
def test_tier_runs(t : T?) {
    t |> success(tier_runs("fast", false), "the fast tier runs without --full")
    t |> success(tier_runs("fast", true), "and with it")
    t |> success(!tier_runs("lane", false), "a lane waits for --full")
    t |> success(tier_runs("lane", true), "and runs under it")
    t |> success(!tier_runs("module", false) && !tier_runs("module", true), "a module gate never runs from a tier")
}

[test(description = "a lane runs in a child unless the run is --serial, every other tier runs in process, and a lane child is started --serial so it starts no child of its own")]
def test_runs_in_child(t : T?) {
    t |> success(runs_in_child("lane", false), "a lane goes to a child")
    t |> success(!runs_in_child("lane", true), "a --serial run keeps it in process")
    t |> success(!runs_in_child("fast", false), "a fast gate runs in process")
    t |> success(!runs_in_child("module", false), "a module gate runs in process")
    let head <- lane_child_head("das.exe", "tests-jit")
    t |> equal(head,
        ["das.exe", "utils/internal/preflight/main.das", "--", "--only", "tests-jit", "--serial"],
        "the child is this tool for one gate, serial")
}

[test(description = "the sweep excludes listed trees by whole path segment, each with a reason, and excludes a module-gated tree exactly when the box lacks its module")]
def test_sweep_excluded(t : T?) {
    t |> success(sweep_excluded("examples/crash/crash.das"), "an excluded tree's root")
    t |> success(sweep_excluded("utils/internal/ast-fuzz/selftest/broken.das"), "the fuzz fixtures")
    t |> success(sweep_excluded("examples/dasLLAMA/library/main.das"), "the opt-in library example, whose bindings a build writes")
    t |> success(!sweep_excluded("examples/dasLLAMA/parrot/main.das"), "its sibling examples are still swept")
    t |> success(!sweep_excluded("examples/crashes/x.das"), "a prefix match is on the whole path segment")
    t |> success(!sweep_excluded("utils/lint/main.das"), "a swept tool root")
    for (why in values(SWEEP_EXCLUDED)) {
        t |> success(!empty(why), "every exclusion carries its reason")
    }
    // a module-gated tree follows the box: swept with the artifact built, skipped without it
    t |> success(key_exists(SWEEP_NEEDS_MODULE, "examples/vulkan/"), "the vulkan examples are a module-gated tree")
    var vulkan_absent = false
    SWEEP_NEEDS_MODULE |> get("examples/vulkan/") $(gate) {
        t |> equal(gate.name, "vulkan", "the gate names the daslang module")
        vulkan_absent = module_absent(gate)
    }
    let absent_text = vulkan_absent ? "absent" : "present"
    t |> equal(sweep_excluded("examples/vulkan/one_source_path_tracer/main.das"), vulkan_absent,
        "a vulkan example is excluded exactly when the box has no dasVulkan ({absent_text})")
    t |> equal(sweep_excluded("examples/vulkan_imgui_cube/main.das"), vulkan_absent, "and so is the imgui cube")
    t |> success(!sweep_excluded("examples/vulkan_less/main.das"), "a prefix match is on the whole path segment here too")
    for (gate in values(SWEEP_NEEDS_MODULE)) {
        t |> success(!empty(gate.name) && (gate.artifact |> ends_with(".shared_module")), "every module gate names a module and its shared artifact: {gate.name} / {gate.artifact}")
    }
}

[test(description = "only text with an exported main is a sweep root, and one requiring the dasllama engine is an engine root, which the sweep leaves out")]
def test_sweep_root_text(t : T?) {
    t |> success(is_sweep_root_text("options gen2\n[export]\ndef main() \{\n\}\n"), "an exported main is a root")
    t |> success(!is_sweep_root_text("options gen2\ndef helper() \{\n\}\n"), "a module without main is not")
    t |> success(!is_sweep_root_text("def main() \{\n\}\n"), "an unexported main is not a program root")
    let engine = "require dasllama/dasllama\n[export]\ndef main() \{\}\n"
    let plain = "require daslib/fio\n[export]\ndef main() \{\}\n"
    t |> success(is_engine_sweep_root(engine), "a root that requires the engine is an engine root")
    t |> success(!is_engine_sweep_root(plain), "a plain root is not")
}

[test(description = "the FAILURES block names the failed files, five at most with the rest counted, and a stated default when the block is absent")]
def test_failed_files_block(t : T?) {
    let one = "13205 tests, 13201 passed, 1 failed\n\nFAILURES:\n  tests/json/test_sscan_json.das — 1 failed, 0 errors\n\nFAILED! (109.3s)\n"
    t |> equal(failed_files_block(one), "tests/json/test_sscan_json.das", "the file before the dash")
    t |> equal(failed_files_block("all green\n"), "the suite (no FAILURES block in the output)", "no block, a stated default")
    let rows <- [for (i in range(7)); "  tests/t{i}.das — 1 failed"]
    let many = "FAILURES:\n" + join(rows, "\n") + "\n"
    t |> equal(failed_files_block(many), "tests/t0.das, tests/t1.das, tests/t2.das, tests/t3.das, tests/t4.das, and 2 more", "five named, the rest counted")
}

[test(description = "the line-length gate passes the base as given, passes on rc 0, and fails on rc 1 naming the base")]
def test_line_length_run(t : T?) {
    t |> equal(line_length_argv("das.exe", "upstream/main"),
        ["das.exe", "utils/internal/line-length/main.das", "--", "--base", "upstream/main"],
        "the base rides as given, never under origin/")
    t |> equal(line_length_fail_detail(0, "origin/master"), "", "rc 0 passes")
    t |> equal(line_length_fail_detail(1, "origin/master"),
        ("line-length check vs origin/master failed - wrap the lines it lists, " +
         "or fix the base it names"),
        "rc 1 fails the gate and names the base - the tool fails on an unresolvable base too")
}

[test(description = "the lint pass detail names the rails that ran and any files --fix rewrote, and the darwin mirror runs only where the box carries dasVulkan")]
def test_lint_pass_detail(t : T?) {
    t |> equal(lint_pass_detail(6, 0, "host, linux-mirror, darwin-mirror"),
        ("6 file(s) clean on every rail (host, linux-mirror, darwin-mirror), " +
         "added lines within CI's length limits"),
        "nothing rewritten: the rails that ran and the length check")
    t |> equal(lint_pass_detail(6, 2, "host = darwin world, linux-mirror"),
        ("6 file(s) clean on every rail (host = darwin world, linux-mirror) after --fix " +
         "rewrote 2 of them - review and commit those edits; " +
         "added lines within CI's length limits"),
        "--fix edits are named so they get committed")
    let rails = lint_rails()
    let expected = (module_absent(VULKAN_GATE) ? "host = darwin world, linux-mirror" :
        "host, linux-mirror, darwin-mirror")
    t |> equal(rails, expected, "the darwin mirror runs only where the box carries dasVulkan")
}

[test(description = "a .das or .md file is a lint input and an .rst, .cpp or .txt is not")]
def test_lint_inputs(t : T?) {
    t |> success(is_lint_input("daslib/lint.das"), "a .das file is a lint input")
    t |> success(is_lint_input("modules/dasLLVM/ARCHITECTURE.md"), "a .md file is a lint input - LINT027's line gate reads it")
    t |> success(!is_lint_input("doc/source/index.rst"), "an .rst is not")
    t |> success(!is_lint_input("src/ast/ast.cpp"), "a .cpp is not")
    t |> success(!is_lint_input("utils/internal/preflight/ci_only_das.txt"), "a .txt is not")
}

[test(description = "the lint gate takes every changed lint input except the tree-sitter grammar copy")]
def test_lint_gate_files(t : T?) {
    var inscope files <- lint_gate_files(["daslib/lint.das", "tree-sitter-daslang/test/tags/tags.das"],
                                         ["tree-sitter-daslang/CLAUDE.md", "utils/REVIEW.md"],
                                         ["tree-sitter-daslang/bindings/node/binding.cc"],
                                         ["tree-sitter-daslang/src/scanner.c", "src/ast/ast.h"])
    t |> equal(join(files, ","), "daslib/lint.das,utils/REVIEW.md,src/ast/ast.h",
               "every changed lint input but the grammar copy, which mirrors its own repository byte for byte")
}

[test(description = "the lint output counts one rewritten file per line starting with FIXED, and none without --fix")]
def test_count_fixed_files(t : T?) {
    t |> equal(count_fixed_files(""), 0, "a run without --fix edits rewrote nothing")
    let out = "FIXED /a/b.das (2)\nWARN /a/c.das (1)\n  LINT003: variable x ...\nFIXED /a/d.das (1)\n"
    t |> equal(count_fixed_files(out), 2, "one FIXED line per rewritten file")
    t |> equal(count_fixed_files("  FIXED is prose here\n"), 0, "only a line that starts with FIXED counts")
}

[test(description = "the utils suite list reads one suite a line, test or compile, and skips what is neither")]
def test_utils_suites(t : T?) {
    let list = "test utils/lint\r\ncompile utils/a/b.das\n\nnoise line\ntest  utils/dascov \n"
    let suites <- utils_suites(list)
    t |> equal(length(suites), 3, "two test lines and one compile line")
    t |> equal(suites[0].path, "utils/lint", "the path follows the verb, the line end trimmed")
    t |> success(!suites[0].compile_only, "a test line runs")
    t |> equal(suites[1].path, "utils/a/b.das", "a compile line names its path")
    t |> success(suites[1].compile_only, "a compile line compiles only")
    t |> equal(suites[2].path, "utils/dascov", "spaces around the path are trimmed")
    t |> equal(length(utils_suites("")), 0, "an empty list names no suite")
}

[test(description = "the build lock has one holder at a time: a second take waits out its limit and fails, a drop frees it, and an empty lock path takes no turn")]
def test_build_lock(t : T?) {
    let lock = "{test_temp_dir()}/build.lock"
    t |> success(take_build_lock(lock, 1.0lf), "a free lock is taken at once")
    t |> success(!take_build_lock(lock, 0.3lf), "a held lock is not taken within the wait")
    drop_build_lock(lock)
    t |> success(take_build_lock(lock, 1.0lf), "a dropped lock is free again")
    drop_build_lock(lock)
    t |> success(take_build_lock("", 0.0lf), "no lock path, no turn to wait for")
}

[test(description = "a build output that says cmake re-ran itself part way reads as regenerated, and an ordinary failure does not")]
def test_regenerated_mid_build(t : T?) {
    let regen = ("  Building Custom Rule CMakeLists.txt\n"
        + "  CMake is re-running because build/CMakeFiles/generate.stamp is out-of-date.\n")
    t |> success(regenerated_mid_build(regen), "the regenerate line marks the build")
    let compile_error = "error C2065: 'x': undeclared identifier\n"
    t |> success(!regenerated_mid_build(compile_error), "a compile error does not")
    t |> success(!regenerated_mid_build(""), "no output, no regenerate")
}
