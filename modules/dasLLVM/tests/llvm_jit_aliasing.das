options gen2
require dastest/testing_boost public
require daslib/fio
require strings
require _spawn_common
require _exe_test_common

//! under the typed model every load and store carries the class of
//! the type it accesses, so the `int?` reads leave a loop that stores
//! through `float?`; `[hint(no_strict_aliasing)]` keeps them in it
[test(description = "the typed model tags a function's accesses and hoists its int reads out of a float-store loop, while a no_strict_aliasing function stays untagged and reloads them", timeout = 1200)]
def test_the_typed_model_separates_float_from_int(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_strict_aliasing_root.das", [])
    t |> success(find(out, "TYPED 5.25 5.25 UNTYPED 5.25 5.25 PUNNING true VIEW true FILL 3") >= 0,
        "the fixture printed its results: {tail(out)}")
    let typed = scale_loop_of(t, out, "scale_typed")
    t |> success(find(typed.ir, "!tbaa") >= 0,
        "the typed function's accesses carry tbaa tags:\n{typed.ir}")
    t |> equal(-1, find(typed.body, "load i32"),
        "the bound and the addend are read outside the typed loop:\n{typed.body}")
    let untyped = scale_loop_of(t, out, "scale_untyped")
    t |> equal(-1, find(untyped.ir, "!tbaa"),
        "an opted-out function carries no tbaa tag:\n{untyped.ir}")
    t |> success(find(untyped.body, "load i32") >= 0,
        "opted out, the bound and the addend are reloaded after every store:\n{untyped.body}")
}

//! `options strict_aliasing = false` takes a whole program out of the
//! typed model
[test(description = "options strict_aliasing = false leaves every access untagged, and [hint(strict_aliasing)] opts one function back in with its reads hoisted", timeout = 1200)]
def test_the_program_option_turns_the_typed_model_off(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_strict_aliasing_off_root.das", [])
    t |> success(find(out, "OFF 5.25 5.25 FORCED 5.25 5.25") >= 0,
        "the fixture printed its result: {tail(out)}")
    let off = scale_loop_of(t, out, "scale_program_off")
    t |> equal(-1, find(off.ir, "!tbaa"), "no access carries a tbaa tag:\n{off.ir}")
    t |> success(find(off.body, "load i32") >= 0,
        "the bound and the addend are reloaded after every store:\n{off.body}")
    let forced = scale_loop_of(t, out, "scale_forced")
    t |> success(find(forced.ir, "!tbaa") >= 0,
        "[hint(strict_aliasing)] opts a function back in under the program option:\n{forced.ir}")
    t |> equal(-1, find(forced.body, "load i32"), "and its loop hoists the reads:\n{forced.body}")
}

//! a function that reinterprets memory under another type is left
//! untagged on its own, and so is a function that calls an untagged
//! function returning a pointer: inlining would otherwise mix a
//! tagged access with the punned view
[test(description = "a punning reinterpret leaves its function untagged, and so is every function the punned view reaches through calls, references, structs, globals, out arguments and lambdas", timeout = 1200)]
def test_a_punning_reinterpret_excludes_its_function_and_its_callers(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_strict_aliasing_root.das", [])
    let punning = scale_loop_of(t, out, "scale_punning")
    t |> equal(-1, find(punning.ir, "!tbaa"),
        "a function with a punning reinterpret is untagged:\n{punning.ir}")
    t |> success(find(punning.body, "load i32") >= 0,
        "and its loop keeps the reloads:\n{punning.body}")
    let view = scale_loop_of(t, out, "scale_through_view")
    t |> equal(-1, find(view.ir, "!tbaa"),
        "a caller of a punned-pointer source is untagged too:\n{view.ir}")
    let lam = scale_loop_of(t, out, "scale_through_lambda")
    t |> equal(-1, find(lam.ir, "!tbaa"),
        "a caller that invokes a pointer-yielding function value is untagged:\n{lam.ir}")
    let far = scale_loop_of(t, out, "scale_through_far_view")
    t |> equal(-1, find(far.ir, "!tbaa"),
        "a caller of a punned-pointer source in another module is untagged:\n{far.ir}")
    let by_ref = scale_loop_of(t, out, "scale_through_ref")
    t |> equal(-1, find(by_ref.ir, "!tbaa"),
        "a caller that takes the punned view as a reference is untagged:\n{by_ref.ir}")
    let interp = scale_loop_of(t, out, "scale_through_interp")
    t |> equal(-1, find(interp.ir, "!tbaa"),
        "a caller of a pointer source the JIT never compiled is untagged:\n{interp.ir}")
    let middle = scale_loop_of(t, out, "scale_through_middle")
    t |> equal(-1, find(middle.ir, "!tbaa"),
        "a caller two calls away from the punned source is untagged:\n{middle.ir}")
    let in_block = scale_loop_of(t, out, "scale_in_block")
    t |> equal(-1, find(in_block.ir, "!tbaa"),
        "a loop in a block literal of an excluded function is untagged:\n{in_block.ir}")
    let in_struct = scale_loop_of(t, out, "scale_through_struct")
    t |> equal(-1, find(in_struct.ir, "!tbaa"),
        "a caller that takes the punned view inside a struct is untagged:\n{in_struct.ir}")
    let two_views = function_ir(out,
        "`scale_two_views 1`Lt`f`Gt`? C1`Lt`i`Gt`? C1`Lt`i`Gt`? C1`Lt`i`Gt`? implementation")
    t |> success(!empty(two_views), "the dump carries scale_two_views: {tail(out)}")
    t |> equal(-1, find(two_views, "!tbaa"),
        "a callee handed a punned view by an excluded caller is untagged:\n{two_views}")
    let before = scale_loop_of(t, out, "scale_before_middle")
    t |> equal(-1, find(before.ir, "!tbaa"),
        "a caller placed before its source in the jit set is untagged:\n{before.ir}")
    let int_ref = function_ir(out,
        "`scale_through_int_ref 1`Lt`f`Gt`? C1`Lt`i`Gt`? C1`Lt`i`Gt`? implementation")
    t |> success(!empty(int_ref), "the dump carries scale_through_int_ref: {tail(out)}")
    t |> equal(-1, find(int_ref, "!tbaa"),
        "a function binding a reference under another type is untagged:\n{int_ref}")
    let forced = scale_loop_of(t, out, "scale_through_forced")
    t |> success(find(forced.ir, "!tbaa") >= 0,
        "the caller of an opted-in punning source stays tagged:\n{forced.ir}")
    let via_out = scale_loop_of(t, out, "scale_through_out")
    t |> equal(-1, find(via_out.ir, "!tbaa"),
        "a caller handed the punned view through an out argument is untagged:\n{via_out.ir}")
    let via_global = scale_loop_of(t, out, "scale_via_global")
    t |> equal(-1, find(via_global.ir, "!tbaa"),
        "a function sharing a view-carrying global with a punner is untagged:\n{via_global.ir}")
    let sinks <- lambda_loops(out)
    t |> equal(2, length(sinks), "the dump carries the sink lambda's loop and the callback's")
    for (sink in sinks) {
        t |> equal(-1, find(sink, "!tbaa"),
            "a lambda an excluded function hands a view to is untagged:\n{sink}")
    }
}

//! the dump's lambda functions that hold a loop (the body inlines
//! into the lambda's entry function, so the label carries a suffix)
def private lambda_loops(out : string) : array<string> {
    var found : array<string>
    peek_data(out) $(d) {
        var pos = 0
        while (true) {
            let at = find(d, "\ndefine ", pos)
            if (at < 0) break
            let eol = find(d, "\n", at + 1)
            let header = slice(d, at + 1, eol)
            if (find(header, "_lambda_") >= 0 && find(header, "`function XS") >= 0) {
                let close = find(d, "\n}", eol)
                let body = slice(d, at + 1, close)
                if (find(body, "\nwhile_body") >= 0) {
                    found |> push(body)
                }
            }
            pos = eol
        }
    }
    return <- found
}

//! the exclusion set is computed over the whole program, so a punned
//! view from another partition excludes its caller under split
[test(description = "under --jit-split-modules a caller of a punned view from another partition is untagged while the typed function stays tagged", timeout = 1200)]
def test_the_exclusion_crosses_split_partitions(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_strict_aliasing_root.das",
        ["--jit-split-modules=1"])
    t |> success(find(out, "VIEW true") >= 0, "the fixture printed its results: {tail(out)}")
    let far = scale_loop_of(t, out, "scale_through_far_view")
    t |> equal(-1, find(far.ir, "!tbaa"),
        "a caller of a punned-pointer source in another partition is untagged:\n{far.ir}")
    let typed = scale_loop_of(t, out, "scale_typed")
    t |> success(find(typed.ir, "!tbaa") >= 0,
        "the typed function stays tagged under split:\n{typed.ir}")
}

//! the classes at their edges: a byte view is no punning and its read
//! stays in the loop; `double` is a class of its own; a vector store
//! aliases everything; a memset pattern store is untyped
[test(description = "a self-typed or byte view keeps its function tagged, double is a class of its own, and a vector store aliases every class", timeout = 1200)]
def test_the_class_edges(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_strict_aliasing_root.das", [])
    t |> success(find(out, "PATTERN 96 BYTES 0 DOUBLE 5.25 VECTOR 5.25 BOUND 5.25 SELF 5.25") >= 0,
        "the fixture printed its results: {tail(out)}")
    let self_view = scale_loop_of(t, out, "scale_self_view")
    t |> success(find(self_view.ir, "!tbaa") >= 0,
        "a reinterpret to the pointer's own type keeps the function tagged:\n{self_view.ir}")
    t |> equal(-1, find(self_view.body, "load i32"),
        "and its int reads leave the loop:\n{self_view.body}")
    let bytes = scale_loop_of(t, out, "scale_byte_view")
    t |> success(find(bytes.ir, "!tbaa") >= 0, "a byte view keeps the function tagged:\n{bytes.ir}")
    t |> equal(-1, find(bytes.body, "load i32"), "and its int reads leave the loop:\n{bytes.body}")
    t |> success(find(bytes.body, "load i8") >= 0, "while the byte read stays in it:\n{bytes.body}")
    let dbl = function_ir(out, "`scale_double 1`Lt`d`Gt`? C1`Lt`i`Gt`? C1`Lt`i`Gt`? implementation")
    t |> success(!empty(dbl), "the dump carries scale_double: {tail(out)}")
    t |> success(find(dbl, "!tbaa") >= 0, "double accesses are tagged:\n{dbl}")
    t |> equal(-1, find(named_block_ir(dbl, "while_body"), "load i32"),
        "the int reads leave the double loop:\n{dbl}")
    let vec = function_ir(out,
        "`scale_vector 1`Lt`f4`Gt`? C1`Lt`i`Gt`? C1`Lt`i`Gt`? implementation")
    t |> success(!empty(vec), "the dump carries scale_vector: {tail(out)}")
    t |> success(find(named_block_ir(vec, "while_body"), "load i32") >= 0,
        "a vector store aliases everything, so the int reads stay in the loop:\n{vec}")
    let bound = function_ir(out,
        "`scale_double_bound 1`Lt`f`Gt`? C1`Lt`d`Gt`? C1`Lt`i`Gt`? implementation")
    t |> success(!empty(bound), "the dump carries scale_double_bound: {tail(out)}")
    t |> equal(-1, find(named_block_ir(bound, "while_body"), "load double"),
        "a double bound leaves a float loop:\n{bound}")
}

//! under the typed model the bound and the addend leave the loop on
//! their own; `[hint(noalias = fa)]` promises distinctness instead,
//! and both forms hoist
[test(description = "the bound and the addend leave a float-store loop both under the typed model alone and under [hint(noalias = fa)]", timeout = 1200)]
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
[test(description = "a noalias, nocapture or align16 hint naming no parameter fails the compile with an error naming the hint, the function and its parameters", timeout = 1200)]
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

//! `for` over a const container parameter writes its lock through
//! the parameter, so the parameter is not readonly; a const pointer
//! to a scalar still is. A readonly parameter under noalias reads as
//! constant memory, and the unlock then sees the magic the lock had
//! not yet written
[test(description = "a const array parameter and a const pointer to an array carry no readonly attribute, while a const pointer to a scalar keeps it", timeout = 1200)]
def test_a_const_container_parameter_is_not_readonly(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_const_container_param_root.das", [])
    t |> success(find(out, "SUM 36") >= 0,
        "the fixture iterates its const parameters three times each: {tail(out)}")
    let sig = signature_ir(function_ir(out, "`sum_for C1`Lt`i`Gt`A implementation"))
    t |> success(!empty(sig), "the dump carries sum_for: {tail(out)}")
    t |> equal(-1, find(sig, "readonly"), "a const array parameter carries no readonly:\n{sig}")
    let ptr_sig = signature_ir(function_ir(out, "`sum_ptr C1`Lt`C1`Lt`i`Gt`A`Gt`? implementation"))
    t |> success(!empty(ptr_sig), "the dump carries sum_ptr: {tail(out)}")
    t |> equal(-1, find(ptr_sig, "readonly"),
        "a const pointer to an array carries no readonly:\n{ptr_sig}")
    let scalar = jit_dump(t, "modules/dasLLVM/tests/_pointer_aliasing_root.das", [])
    let scale_sig = signature_ir(function_ir(scalar,
        "`scale 1`Lt`f`Gt`? C1`Lt`i`Gt`? C1`Lt`i`Gt`? implementation"))
    t |> success(find(scale_sig, "readonly") >= 0,
        "a const pointer to a scalar keeps readonly:\n{scale_sig}")
}
