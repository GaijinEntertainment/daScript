options gen2
require dastest/testing_boost public
require daslib/fio
require strings
require _spawn_common
require _exe_test_common

//! under the typed model the bound and the addend leave the loop on
//! their own; `[hint(noalias = fa)]` promises distinctness instead,
//! and both forms hoist
[test]
def test_the_typed_default_and_the_noalias_hint_both_hoist(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_pointer_aliasing_root.das", [])
    t |> success(find(out, "SCALE 5.25 5.25 HINTED 5.25 5.25") >= 0,
        "the fixture printed its result: {tail(out)}")
    let plain = scale_loop_of(t, out, "scale")
    t |> success(find(plain.body, "store float") >= 0,
        "the loop body stores the float:\n{plain.body}")
    t |> equal(-1, find(plain.body, "load i32"),
        "under the typed model the bound and the addend leave the loop unhinted:\n{plain.body}")
    let hinted = scale_loop_of(t, out, "scale_hinted")
    t |> success(find(hinted.body, "store float") >= 0,
        "the hinted loop body stores the float:\n{hinted.body}")
    t |> equal(-1, find(hinted.body, "load i32"),
        "under [hint(noalias = fa)] the bound and the addend leave the loop:\n{hinted.body}")
}

//! a hint naming no parameter is a compile error that names the
//! function and its parameters, not a hint that silently does nothing
[test]
def test_a_hint_naming_no_parameter_is_a_compile_error(t : T?) {
    var out = ""
    var rc = 0
    with_child_log_info() {
        rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", "-jit-no-cache",
            "modules/dasLLVM/tests/_pointer_aliasing_bad_hint_root.das"], out, 300.0)
    }
    t |> success(rc != 0, "the compile fails: {tail(out)}")
    let scale_error = "[hint(noalias = fb)] on scale names no parameter; the parameters are: fa, n"
    t |> success(find(out, scale_error) >= 0,
        "the error names the hint, the function and its parameters: {tail(out)}")
    let keep_error = "[hint(nocapture = zz)] on keep names no parameter; the parameters are: fa"
    t |> success(find(out, keep_error) >= 0, "a nocapture hint is checked too: {tail(out)}")
    let aligned_error = "[hint(align16 = qq)] on aligned names no parameter; the parameters are: fa"
    t |> success(find(out, aligned_error) >= 0, "an align16 hint is checked too: {tail(out)}")
    t |> equal(-1, find(out, "SCALED"), "the program does not run: {tail(out)}")
}
