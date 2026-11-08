options gen2
require dastest/testing_boost public
require daslib/fio
require strings
require _spawn_common
require _exe_test_common

//! the `while_body` block of the fixture function `name`, checked
//! to store the float
def private loop_body(t : T?; out : string; name : string) : string {
    let ir = function_ir(out, "`{name} 1`Lt`f`Gt`? C1`Lt`i`Gt`? C1`Lt`i`Gt`? implementation")
    t |> success(!empty(ir), "the dump carries {name}: {tail(out)}")
    let body = named_block_ir(ir, "while_body")
    t |> success(!empty(body), "{name} keeps a while_body block:\n{ir}")
    t |> success(find(body, "store float") >= 0,
        "the loop body of {name} stores the float:\n{body}")
    return body
}

//! a float store may change what an int? points at, so with no
//! promise the bound and the addend are reloaded every iteration;
//! `[hint(noalias = fa)]` is the promise, under which both leave
//! the loop
[test]
def test_a_pointer_is_an_address_and_the_noalias_hint_is_the_promise(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_pointer_aliasing_root.das", [])
    t |> success(find(out, "SCALE 5.25 5.25 HINTED 5.25 5.25") >= 0,
        "the fixture printed its result: {tail(out)}")
    let plain = loop_body(t, out, "scale")
    t |> success(find(plain, "load i32") >= 0,
        "with no promise the bound and the addend are reloaded after every float store:\n{plain}")
    let hinted = loop_body(t, out, "scale_hinted")
    t |> equal(-1, find(hinted, "load i32"),
        "under [hint(noalias = fa)] the bound and the addend are read outside the loop:\n{hinted}")
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
