options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost

let BIN = "bin/daslang"

def private run(argv : array<string>; var out : string&; timeout : float) : int {
    return run_and_capture(argv, out, timeout)
}

def private died(ec : int; out : string) : bool {
    return (ec >= 128 || ec == popen_timed_out || ec < 0 || find(out, "CRASH:") >= 0)
}

[test]
def test_verifier_reports_null_child(t : T?) {
    var out : string
    let ec = run([BIN, "utils/ast-fuzz/selftest/run.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "AST verify: ExprOp2.right is null") >= 0,
        "did not report the null child\n{out}")
}

[test]
def test_verifier_cuts_a_cycle(t : T?) {
    var out : string
    let ec = run([BIN, "utils/ast-fuzz/selftest/cycle.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "points at its own ancestor") >= 0,
        "did not report the cycle\n{out}")
}

[test]
def test_verifier_drops_a_null_vector_entry(t : T?) {
    var out : string
    let ec = run([BIN, "utils/ast-fuzz/selftest/null_entry.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "ExprLet.variables holds 1 null entry") >= 0,
        "did not report the null entry\n{out}")
}

[test]
def test_verifier_reports_an_aliased_type(t : T?) {
    var out : string
    let ec = run([BIN, "utils/ast-fuzz/selftest/alias.das"], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "TypeDecl int const is aliased") >= 0,
        "did not report the aliased type\n{out}")
}

// expr.type is not walked by the visitor, so this alias is only visible to the type
// tree recorded from preVisitExpression
[test]
def test_verifier_reports_an_aliased_expression_type(t : T?) {
    var out : string
    let ec = run([BIN, "utils/ast-fuzz/selftest/alias_type.das"], out, 60.0)
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
[test]
def test_ast_verify_guesses_a_location_for_an_unset_at(t : T?) {
    var out : string
    let f = "utils/ast-fuzz/selftest/at_unset.das"
    let ec = run([BIN, "--ast-verify", f], out, 60.0)
    t |> success(!died(ec, out), "compile crashed (exit {ec})\n{out}")
    t |> success(find(out, "ExprOp2 has no source location (at is unset) in 'lost_one'") >= 0,
        "did not report the unset at\n{out}")
    t |> success(find(out, "in 'lost_one'; first located node inside it is ExprRef2Value at {f}:9:11") >= 0,
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

[test]
def test_ast_verify_clean_on_valid_code(t : T?) {
    var out : string
    let ec = run([BIN, "--ast-verify", "utils/ast-fuzz/selftest/cli_victim.das"], out, 60.0)
    t |> equal(ec, 0)
    t |> success(find(out, "AST verify:") < 0, "reported on valid code\n{out}")
}

[test]
def test_ast_verify_skips_late_extern_stubs(t : T?) {
    // a dasbind stub body is never inferred (InferTypes::canVisitFunction skips
    // stub=true), so its unresolved int() must not trip the post-infer verify
    var out : string
    let ec = run([BIN, "--ast-verify", "utils/ast-fuzz/selftest/late_extern_stub.das"], out, 60.0)
    t |> equal(ec, 0)
    t |> success(find(out, "AST verify") < 0, "reported on a stub body\n{out}")
}

[test]
def test_every_visitor_deref_has_a_repair(t : T?) {
    var out : string
    let ec = run([BIN, "utils/ast-fuzz/gen_invariants.das"], out, 180.0)
    t |> success(ec == 0, "an unguarded child deref has no repair\n{out}")
}

[test]
def test_generator_runs_and_classifies(t : T?) {
    var out : string
    let ec = run([BIN, "utils/ast-fuzz/main.das", "--", "--bin", BIN,
        "--seeds", "3", "--synth-size", "4", "--synth-depth", "2", "--timeout", "30"], out, 300.0)
    t |> success(!died(ec, out), "driver itself crashed or hung (exit {ec})\n{out}")
    t |> success(find(out, "run(s) —") >= 0, "driver produced no summary\n{out}")
}
