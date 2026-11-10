options gen2

require dastest/testing_boost public
require daslib/clargs
require daslib/fio
require ../../../common/verify_pool.das

[test(description = "a verify child's output and exit code classify as clean, AST verify, unverifiable, timed out, crashed, killed or failed to start, a compile error counting as clean")]
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
    for (rc in [137, 139, -1073741819]) {
        t |> equal(verify_verdict("", rc, 300.0).verdict, "killed",
            "exit {rc} with no banner is a kill")
    }
    t |> equal(verify_verdict("", 9, 300.0).verdict, "clean",
        "a plain exit code below 128 is the file owner's gate")
    for (rc in [-1, 127]) {
        t |> equal(verify_verdict("", rc, 300.0).verdict, "failed to start",
            "a child that did not start (exit {rc}) is not a pass")
    }
}

[test(description = "a verify child runs -dry-run --ast-verify-batch with the module cache off, or on the named run cache")]
def test_verify_argv(t : T?) {
    t |> equal(verify_argv("d", "f.das", ""),
        ["d", "-no-module-cache", "-dry-run", "--ast-verify-batch", "f.das"],
        "no run cache: the default cache is off")
    t |> equal(verify_argv("d", "f.das", "c"),
        ["d", "-module-cache", "c", "-dry-run", "--ast-verify-batch", "f.das"],
        "a run cache is named")
}

[test(description = "the verifiers' skip set is one predicate: compile-fail fixtures by name always; the ast-fuzz self-test breakers and the whole-stack compiles (every path naming dasllama, das2rst) in changed mode only, so --file verifies what it names")]
def test_ast_verify_skips(t : T?) {
    t |> success(ast_verify_skips("tests/x/failed_thing.das", true), "a compile-fail fixture")
    t |> success(ast_verify_skips("tests/x/cant_thing.das", false), "cant_ in tree mode too")
    t |> success(ast_verify_skips("utils/internal/ast-fuzz/selftest/alias_breaker.das", true),
                 "a self-test breaker in changed mode")
    t |> success(!ast_verify_skips("utils/internal/ast-fuzz/selftest/alias_breaker.das", false),
                 "the tree mode verifies the breakers")
    t |> success(ast_verify_skips("utils/dasllama-server/test_openai_server.das", true),
                 "dasllama-server in changed mode")
    t |> success(ast_verify_skips("utils\\dasllama-server\\cli.das", true),
                 "Windows separators")
    t |> success(!ast_verify_skips("utils/dasllama-server/cli.das", false),
                 "a file named to --file is verified")
    t |> success(ast_verify_skips("utils/dasllama-convert/test_convert.das", true),
                 "dasllama-convert pulls the llama stack too")
    t |> success(ast_verify_skips("modules/dasLLAMA/tests/test_facade.das", true),
                 "a dasLLAMA file compiles the engine: its lint lane owns it")
    t |> success(!ast_verify_skips("modules/dasLLAMA/tests/test_facade.das", false),
                 "a dasLLAMA file named to --file is verified")
    t |> success(ast_verify_skips("examples/dasLLAMA/parrot/main.das", true),
                 "its examples, tutorials and tools too, in either case")
    t |> success(ast_verify_skips("utils/internal/dasllama-ladder/ladder_server.das", true),
                 "the ladder service is a dasllama tool")
    t |> success(!ast_verify_skips("modules/dasLLVM/tests/llvm_exe.das", true),
                 "the other modules are verified")
    t |> success(ast_verify_skips("doc/reflections/das2rst.das", true),
                 "das2rst requires every module")
    t |> success(!ast_verify_skips("doc/reflections/imgui2rst.das", true),
                 "the other reflections are verified")
    t |> success(!ast_verify_skips("utils/lint/main.das", true), "an ordinary file is verified")
}

//! the broken fixture reported on twice - a module cache the first
//! child wrote must not pass the second
[test(description = "the verify pool reports results in input order and the broken fixture fails on its second child too, past the cache the first one wrote", timeout = 1200)]
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
