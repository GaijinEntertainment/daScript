options gen2

require dastest/testing_boost public
require daslib/clargs
require daslib/fio
require ../../../common/verify_pool.das

[test]
def test_verify_verdict(t : T?) {
    t |> equal(verify_verdict("compiled\n", 0, 300.0).verdict, "clean", "a clean run")
    t |> equal(verify_verdict("error[30341]: no matching functions\n", 1, 300.0).verdict, "clean",
        "a compile error is the file owner's gate, not this one's")
    let report = "AST verify (post-infer): ExprOp2 has no source location"
    let hit = verify_verdict("x\n  {report}\ny\n", 1, 300.0)
    t |> equal(hit.verdict, "AST verify", "an AST verify line")
    t |> equal(hit.findings, report, "the findings are the stripped line")
    let closure = verify_verdict("error[20510]: AST verify cannot\n", 1, 300.0)
    t |> success(closure.unverifiable && closure.verdict == "unverifiable",
        "20510 wins over the AST verify line")
    t |> equal(verify_verdict("", popen_timed_out, 300.0).findings, "timed out after 300s",
        "a timeout")
    let crash = verify_verdict("CRASH: SIGSEGV\n", 255, 300.0)
    t |> equal(crash.verdict, "crashed", "a crash banner")
    t |> equal(crash.findings, "crashed (exit 255)\nCRASH: SIGSEGV",
        "the banner rides the findings")
    for (rc in [9, 139, -1073741819]) {
        t |> equal(verify_verdict("", rc, 300.0).verdict, "killed",
            "exit {rc} with no banner is a kill")
    }
}

[test]
def test_verify_argv(t : T?) {
    t |> equal(verify_argv("d", "f.das", ""),
        ["d", "-no-module-cache", "-dry-run", "--ast-verify-batch", "f.das"],
        "no run cache: the default cache is off")
    t |> equal(verify_argv("d", "f.das", "c"),
        ["d", "-module-cache", "c", "-dry-run", "--ast-verify-batch", "f.das"],
        "a run cache is named")
}

//! the broken fixture reported on twice - a module cache the first
//! child wrote must not pass the second
[test]
def test_pool_reports_in_input_order(t : T?) {
    let root = get_das_root()
    let clean = path_join(root, "utils/internal/ast-fuzz/selftest/cli_victim.das")
    let broken = path_join(root, "utils/internal/ast-fuzz/selftest/at_unset.das")
    let results <- run_verify_pool(get_host_binary(), [broken, clean, broken], 2, 120.0, "")
    t |> equal(length(results), 3)
    t |> equal(results[0].verdict, "AST verify",
        "the broken fixture reports\n{results[0].findings}")
    t |> equal(results[1].verdict, "clean", "the clean fixture passes\n{results[1].findings}")
    t |> equal(results[2].verdict, "AST verify", "and reports again on the second child")
    for (i in range(3)) {
        t |> equal(results[i].index, i, "results come back in input order")
    }
}
