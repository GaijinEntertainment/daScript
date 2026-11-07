options gen2
require dastest/testing_boost public
require daslib/fio
require strings
require _exe_test_common

//! the remarks are stderr lines of a child: a second child in the
//! same directory proves they print past the DLL cache the first one
//! minted
[test]
def test_jit_remarks_print_the_vectorizer_decisions(t : T?) {
    var terr : string
    let dir = create_temp_directory("llvm_jit_remarks", terr)
    if (empty(dir)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_full_file_name("modules/dasLLVM/tests/_remarks_root.das")
    let noenv : array<string>
    var out = ""
    var rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", root], dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit: {tail(out)}")
    t |> success(find(out, "HALVED 1 ") >= 0, "the fixture printed its result: {tail(out)}")
    t |> equal(-1, find(out, "remark:"), "no remark prints without a remark flag: {tail(out)}")
    rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", root, "--",
            "--jit-remarks=loop-vectorize"], dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit --jit-remarks alone: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: vectorized loop") >= 0,
        "--jit-remarks alone prints past the cache: {tail(out)}")
    rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", root, "--",
            "--jit-remarks-missed=loop-vectorize"], dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit --jit-remarks-missed alone: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: loop not vectorized") >= 0,
        "--jit-remarks-missed alone prints past the cache: {tail(out)}")
    t |> equal(-1, find(out, "loop not vectorized:"),
        "--jit-remarks-missed prints no analysis remark: {tail(out)}")
    rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", root, "--",
            "--jit-remarks-analysis=loop-vectorize"], dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit --jit-remarks-analysis alone: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: loop not vectorized: ") >= 0,
        "--jit-remarks-analysis alone prints the analysis remarks past the cache: {tail(out)}")
    rc = run_and_capture(
        [das_exe(), "-no-module-cache", "-jit", root, "--", "--jit-remarks=loop-vectorize",
            "--jit-remarks-missed=loop-vectorize"],
        dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit with remarks: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: vectorized loop") >= 0,
        "the counted loop's remark prints past the cache: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: loop not vectorized") >= 0,
        "the data-exit loop's missed remark prints: {tail(out)}")
    rc = run_and_capture(
        [das_exe(), "-no-module-cache", "-jit", root, "--", "--jit-debug",
            "--jit-remarks-missed=loop-vectorize"],
        dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit --jit-debug with remarks: {tail(out)}")
    t |> success(find(out, "_remarks_root.das:18:") >= 0,
        "under --jit-debug a remark names the loop's .das line: {tail(out)}")
    t |> equal(-1, find(out, "vectorized loop"), "only the missed remarks print: {tail(out)}")
    rmdir_rec(dir, terr)
}
