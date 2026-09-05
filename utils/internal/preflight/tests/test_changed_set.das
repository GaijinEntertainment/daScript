options gen2
options indenting = 4

require dastest/testing_boost public

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
}
