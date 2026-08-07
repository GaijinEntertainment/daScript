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
def test_ast_verify_clean_on_valid_code(t : T?) {
    var out : string
    let ec = run([BIN, "--ast-verify", "utils/ast-fuzz/selftest/cli_victim.das"], out, 60.0)
    t |> equal(ec, 0)
    t |> success(find(out, "AST verify:") < 0, "reported on valid code\n{out}")
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
