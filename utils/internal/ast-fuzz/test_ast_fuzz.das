options gen2

require dastest/testing_boost public
require daslib/defer
require daslib/fio
require daslib/strings_boost

//! the binary running this test, whatever the build layout put it (`bin/daslang` on a single-config build, `bin/Release/daslang.exe` under MSVC)
def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "bin/daslang" : get_full_file_name(args[0])
}

let BIN = das_exe()

def private run(argv : array<string>; var out : string&; timeout : float) : int =>
    run_and_capture(argv, out, timeout)

def private died(ec : int; out : string) : bool =>
    (is_death_exit_code(ec) || ec == popen_timed_out || find(out, "CRASH:") >= 0)

[test(description = "a compile whose tree holds a null ExprOp2 operand reports `ExprOp2.right is null` instead of crashing", timeout = 1200)]
def test_verifier_reports_null_child(t : T?) {
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/selftest/run.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "AST verify: ExprOp2.right is null") >= 0,
        "did not report the null child\n{out}")
}

[test(description = "a node that points at its own ancestor is reported as a cycle and the walk survives it", timeout = 1200)]
def test_verifier_cuts_a_cycle(t : T?) {
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/selftest/cycle.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "points at its own ancestor") >= 0,
        "did not report the cycle\n{out}")
}

[test(description = "a null entry in ExprLet.variables is reported by count and dropped instead of crashing the compile", timeout = 1200)]
def test_verifier_drops_a_null_vector_entry(t : T?) {
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/selftest/null_entry.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "ExprLet.variables holds 1 null entry") >= 0,
        "did not report the null entry\n{out}")
}

[test(description = "one TypeDecl shared by two owners is reported as aliased", timeout = 1200)]
def test_verifier_reports_an_aliased_type(t : T?) {
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/selftest/alias.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "TypeDecl int const is aliased") >= 0,
        "did not report the aliased type\n{out}")
}

// the flag a macro that rewrites a literal's values must clear; nothing but this verifier
// notices one it did not
[test(description = "an ExprMakeArray left flagged constValues over a value that is no typed constant is reported", timeout = 1200)]
def test_verifier_reports_a_stale_const_values_flag(t : T?) {
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/selftest/const_values.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "ExprMakeArray.constValues is set over a value that is not a typed constant") >= 0,
        "did not report the stale flag\n{out}")
}

// expr.type is not walked by the visitor, so this alias is only visible to the type
// tree recorded from preVisitExpression
[test(description = "an aliased expression type, which the visitor never walks, is reported and its report trails no empty location", timeout = 1200)]
def test_verifier_reports_an_aliased_expression_type(t : T?) {
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/selftest/alias_type.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "TypeDecl int (ExprConstInt.type) is aliased") >= 0,
        "did not report the aliased expression type\n{out}")
    // nothing in a detached synthetic tree is located, so the report carries no location - and
    // must not trail an empty " at " either
    t |> success(find(out, "(missing clone) at") < 0, "trailing empty location\n{out}")
}

// A node with no `at` is anchored by the verify itself, tier by tier: its own subtree first, then
// the node the walk passed last (in pre-order the enclosing one, for a first child), then the last
// located non-expression site. One function per tier in at_unset.das.
[test(description = "--ast-verify anchors a node with an unset `at` at each fallback tier: its subtree, the enclosing node, the argument, the function", timeout = 1200)]
def test_ast_verify_guesses_a_location_for_an_unset_at(t : T?) {
    var out : string
    let f = "utils/internal/ast-fuzz/selftest/at_unset.das"
    let ec = run([BIN, "--ast-verify", f], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "ExprOp2 has no source location (at is unset) in 'lost_one'") >= 0,
        "did not report the unset at\n{out}")
    t |> success(find(out, "in 'lost_one'; first located node inside it is ExprVar at {f}:9:11") >= 0,
        "did not anchor inside the subtree, on the operand\n{out}")
    t |> success(find(out, "in 'deep_add'; preceded by ExprReturn at {f}:14:4") >= 0,
        "an unlocated subtree did not anchor to the node enclosing it\n{out}")
    t |> success(find(out, "in 'all_gone'; closest location the walk passed is function 'all_gone' argument 'a' at {f}:18:") >= 0,
        "a fully unlocated body did not fall back to the argument\n{out}")
    t |> success(find(out, "in 'no_args'; closest location the walk passed is function 'no_args' at {f}:23:") >= 0,
        "a fully unlocated body with no argument did not fall back to the function\n{out}")
    // the alias expression on line 30 is located, but it is never inferred and never runs, so the
    // search has to skip it exactly as the verify walk does
    t |> success(find(out, "in 'with_assume'; first located node inside it is ExprReturn at {f}:31:4") >= 0,
        "the search walked into an assume alias it should not see\n{out}")
}

// A promoted verify's post-compile pass reaches every later program
// of the process; one that never required it is left alone. The lint
// runner shares modules, so the verify fixture ahead of the breaking
// one in one run is the shape.
[test(description = "a verify one program promoted in a shared-module lint run does not fail a later program that never required it", timeout = 1200)]
def test_ast_verify_leaves_a_program_that_did_not_ask(t : T?) {
    var out : string
    let d = "utils/internal/ast-fuzz/selftest"
    let argv <- [BIN, "utils/lint/main.das", "--",
        "{d}/verify_breaker.das", "{d}/at_unset.das", "--no-cache"]
    let ec = run(argv, out, 300.0)
    t |> success(!died(ec, out), "the lint crashed (exit {ec})\n{out}")
    t |> success(find(out, "{d}/at_unset.das") >= 0 && find(out, "FAIL") < 0,
        "the promoted verify reached a program that did not require it\n{out}")
}

[test(description = "--ast-verify on valid code exits 0 and prints no AST verify report", timeout = 1200)]
def test_ast_verify_clean_on_valid_code(t : T?) {
    var out : string
    let ec = run([BIN, "--ast-verify", "utils/internal/ast-fuzz/selftest/cli_victim.das"], out, 60.0)
    t |> equal(ec, 0)
    t |> success(find(out, "AST verify:") < 0, "reported on valid code\n{out}")
}

// Batch mode keeps the post-infer walk, so a tree an [infer_macro] broke after inference is
// still reported before lint/fold/codegen consume it.
[test(description = "--ast-verify-batch still reports an unset `at` an [infer_macro] left after inference", timeout = 1200)]
def test_ast_verify_batch_still_reports_a_macro_broken_tree(t : T?) {
    var out : string
    let f = "utils/internal/ast-fuzz/selftest/at_unset.das"
    let ec = run([BIN, "--ast-verify-batch", f], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "ExprOp2 has no source location (at is unset) in 'lost_one'") >= 0,
        "batch mode did not report the unset at\n{out}")
}

def check_user_macro_break_is_reported(t : T?; f : string) {
    for (mode in ["--ast-verify", "--ast-verify-batch"]) {
        var out : string
        let ec = run([BIN, mode, f], out, 60.0)
        t |> success(!died(ec, out), "{mode}: compile crashed (exit {ec})\n{out}")
        t |> success(find(out, "AST verify: ExprOp2.right is null") >= 0,
            "{mode} did not report the null a user macro left\n{out}")
        t |> success(ec != 0, "{mode} compiled a repaired tree as a success (exit {ec})\n{out}")
    }
}

[test(description = "a null a user macro leaves mid-pass is reported under both verify modes and fails the compile", timeout = 1200)]
def test_user_macro_break_is_reported_mid_pass(t : T?) {
    check_user_macro_break_is_reported(t, "utils/internal/ast-fuzz/selftest/_mid_pass.das")
}

[test(description = "a null a user macro leaves at run start is reported under both verify modes and fails the compile", timeout = 1200)]
def test_user_macro_break_is_reported_run_start(t : T?) {
    check_user_macro_break_is_reported(t, "utils/internal/ast-fuzz/selftest/_run_start.das")
}

[test(description = "a null an [optimization_macro] leaves is reported by --ast-verify-batch and fails the compile", timeout = 1200)]
def test_user_macro_break_at_optimize_is_reported(t : T?) {
    var out : string
    let f = "utils/internal/ast-fuzz/selftest/_optimize.das"
    let ec = run([BIN, "--ast-verify-batch", f], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "AST verify: ExprOp2.right is null") >= 0,
        "the break an [optimization_macro] made went unreported\n{out}")
    t |> success(ec != 0, "compiled a repaired tree as a success (exit {ec})\n{out}")
}

[test(description = "--ast-verify-batch on valid code exits 0 and prints no AST verify report", timeout = 1200)]
def test_ast_verify_batch_clean_on_valid_code(t : T?) {
    var out : string
    let ec = run([BIN, "--ast-verify-batch", "utils/internal/ast-fuzz/selftest/cli_victim.das"], out, 60.0)
    t |> equal(ec, 0)
    t |> success(find(out, "AST verify:") < 0, "reported on valid code\n{out}")
}

[test(description = "the post-infer verify skips a never-inferred dasbind stub body and exits 0", timeout = 1200)]
def test_ast_verify_skips_late_extern_stubs(t : T?) {
    // a dasbind stub body is never inferred (InferTypes::canVisitFunction skips
    // stub=true), so its unresolved int() must not trip the post-infer verify
    var out : string
    let ec = run([BIN, "--ast-verify", "utils/internal/ast-fuzz/selftest/late_extern_stub.das"], out, 60.0)
    t |> equal(ec, 0)
    t |> success(find(out, "AST verify") < 0, "reported on a stub body\n{out}")
}

[test(description = "gen_invariants finds a repair for every child deref the visitor makes", timeout = 1200)]
def test_every_visitor_deref_has_a_repair(t : T?) {
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/gen_invariants.das"], out, 180.0)
    t |> success(ec == 0, "an unguarded child deref has no repair\n{out}")
}

[test(description = "the fuzz driver runs three seeds to its classified summary without crashing or hanging", timeout = 1200)]
def test_generator_runs_and_classifies(t : T?) {
    set_env_variable("DAS_LOG_LEVEL", "info")
    defer() {
        set_env_variable("DAS_LOG_LEVEL", "")
    }
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/main.das", "--", "--bin", BIN,
        "--seeds", "3", "--synth-size", "4", "--synth-depth", "2", "--timeout", "30"], out, 300.0)
    t |> success(!died(ec, out), "driver itself crashed or hung (exit {ec})\n{out}")
    t |> success(find(out, "run(s) —") >= 0, "driver produced no summary\n{out}")
}

[test(description = "a --bin that cannot start makes the driver exit 2 naming it, with no run classified", timeout = 1200)]
def test_a_binary_that_does_not_start_stops_the_driver(t : T?) {
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/main.das", "--", "--bin", "/nonexistent/daslang",
        "--seeds", "2", "--timeout", "30"], out, 300.0)
    t |> equal(2, ec, "the driver exits 2 on a binary it cannot start\n{out}")
    t |> success(find(out, "could not start /nonexistent/daslang") >= 0,
        "the driver names the binary\n{out}")
    t |> success(find(out, "run(s)") < 0, "no run was classified\n{out}")
}

[test(description = "a cross-module ctor in the field default of a struct nothing constructs passes the post-infer verify and runs", timeout = 1200)]
def test_verifier_tolerates_uninstantiated_field_default(t : T?) {
    // a cross-module ctor in a field default of a struct nothing constructs: infer types the
    // decl init but never binds the func - the post-infer walk must not report it
    var out : string
    let ec = run([BIN, "utils/internal/ast-fuzz/selftest/uninstantiated_default.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(ec == 0, "the fixture must compile and run (exit {ec})\n{out}")
    t |> success(find(out, "AST verify") < 0, "no verify report on the dead decl init\n{out}")
    t |> success(find(out, "uninstantiated-default-ok") >= 0, "the fixture ran to its marker\n{out}")
}
