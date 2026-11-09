options gen2
require dastest/testing_boost public
require strings
require daslib/rtti
require daslib/ast
require llvm/daslib/llvm_jit_plan
require _exe_test_common

// The JIT cache keys fold the annotations only the backend reads - the AOT hash never sees them
// (modules/dasLLVM/ARCHITECTURE_JIT_ENTRY.md#hint-folds). Each annotation folds after its function's
// name, so the same annotation on a different function is a different key.

def private fold_source_function_hints(t : T?; src : string) : uint64 {
    var h = 0ul
    compile("fold_source_function_hints", src, CodeOfPolicies()) $(ok; prog; errors) {
        t |> success(ok, "the probe source compiles: {errors}")
        if (ok) {
            var funcs : array<FunctionPtr>
            get_this_module(prog) |> for_each_function("") $(fn) {
                funcs |> push(fn)
            }
            h = fold_function_hints(funcs, 0ul)
        }
    }
    return h
}

//! the typed model's verdicts over `src`, folded as a cache key
//! takes them
def private fold_source_exclusions(t : T?; src : string) : uint64 {
    var h = 0ul
    compile("fold_source_exclusions", src, CodeOfPolicies()) $(ok; prog; errors) {
        t |> success(ok, "the probe source compiles: {errors}")
        if (ok) {
            var funcs : array<FunctionPtr>
            get_this_module(prog) |> for_each_function("") $(fn) {
                funcs |> push(fn)
            }
            compute_strict_aliasing_exclusions(funcs)
            h = fold_strict_aliasing_exclusions(funcs, 0ul)
        }
    }
    return h
}

//! two functions over a float buffer; `pun_first` puts the int view
//! in the first, else the second
def private two_views(pun_first : bool) : string {
    let pun = "        let bits & = unsafe(reinterpret<int&>(fa[0]))\n        return float(bits)\n"
    let plain = "        return fa[0]\n"
    let first = pun_first ? pun : plain
    let second = pun_first ? plain : pun
    let head = "options gen2\ndef first_view(var fa : float?) : float \{\n    unsafe \{\n"
    let mid = "{first}    \}\n\}\ndef second_view(var fa : float?) : float \{\n    unsafe \{\n"
    return "{head}{mid}{second}    \}\n\}\n"
}

//! two functions with identical bodies; `on_first` puts `ann` on the first, else on the second
def private two_functions(ann : string; on_first : bool) : string {
    let first = on_first ? "{ann}\n" : ""
    let second = on_first ? "" : "{ann}\n"
    return "options gen2\n{first}def first_mac(a, b, c : double) : double \{\n    return a * b + c\n\}\n{second}def second_mac(a, b, c : double) : double \{\n    return a * b + c\n\}\n"
}

[test(description = "the [hint] fold is a function of the source, and the same hint moved to another function changes the key")]
def test_hint_folds_bind_to_their_function(t : T?) {
    let hint = "[hint(noinline)]"
    t |> equal(fold_source_function_hints(t, two_functions(hint, true)), fold_source_function_hints(t, two_functions(hint, true)), "the fold is a function of the source")
    t |> success(fold_source_function_hints(t, two_functions(hint, true)) != fold_source_function_hints(t, two_functions(hint, false)), "a [hint] moved to another function changes the key")
}

//! a function's typed-model verdict is not in its AOT hash, and it
//! can change through another partition's body
[test(description = "the strict-aliasing exclusion fold is a function of the source, and the punning view moved to another function changes the key")]
def test_an_aliasing_exclusion_rekeys_the_jit_cache(t : T?) {
    let pun_first = fold_source_exclusions(t, two_views(true))
    t |> equal(pun_first, fold_source_exclusions(t, two_views(true)),
        "the fold is a function of the source")
    t |> success(pun_first != fold_source_exclusions(t, two_views(false)),
        "the punning view moved to the other function changes the key")
}

[test(description = "[never_fast_math] changes the fold both JIT cache keys take, and moving it to another function changes it again")]
def test_never_fast_math_rekeys_the_jit_cache(t : T?) {
    let marker = "[never_fast_math]"
    t |> success(fold_source_function_hints(t, two_functions("", true)) != fold_source_function_hints(t, two_functions(marker, true)), "[never_fast_math] changes the fold both cache keys take, so a cached DLL built with the stamp is not served back")
    t |> success(fold_source_function_hints(t, two_functions(marker, true)) != fold_source_function_hints(t, two_functions(marker, false)), "[never_fast_math] moved to another function changes the key")
}

// Under options fast_math every FP instruction is stamped reassoc|nsz|contract before the optimizer;
// a [never_fast_math] function, and a block literal written in its body, keep no flags
// (modules/dasLLVM/ARCHITECTURE_FAST_MATH.md#never-fast-math). The check reads the dumped IR, so it
// holds on every host - the numeric failure it prevents shows only on an AVX-512 target.

def private expect_unstamped(t : T?; ir, what : string) {
    t |> success(!empty(ir), "the dump carries {what}")
    t |> success(find(ir, "fmul double") >= 0, "{what} keeps its multiply:\n{ir}")
    t |> equal(-1, find(ir, "reassoc"), "{what} carries no fast-math flag:\n{ir}")
}

[test(description = "under options fast_math an unmarked function's multiply is stamped reassoc nsz contract, while a [never_fast_math] function and a block literal in it carry no flag", timeout = 1200)]
def test_never_fast_math_keeps_flags_off(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_never_fast_math_root.das", [])
    t |> success(find(out, "MACS 4 4 4") >= 0, "the fixture printed its three results: {tail(out)}")
    let fast = function_ir(out, "`fast_mac Cd Cd Cd implementation")
    t |> success(!empty(fast), "the dump carries fast_mac: {tail(out)}")
    t |> success(find(fast, "fmul reassoc nsz contract double") >= 0, "an unmarked function is stamped under options fast_math:\n{fast}")
    expect_unstamped(t, function_ir(out, "`precise_mac Cd Cd Cd implementation"), "the [never_fast_math] function")
    expect_unstamped(t, function_ir(out, "_block_ Cd implementation"), "a block literal in a [never_fast_math] function")
}

[test(description = "[never_fast_math] sets the function's neverInline flag, so no AST splice moves its arithmetic into an unmarked caller")]
def test_never_fast_math_implies_never_inline(t : T?) {
    let marked = "options gen2\n[never_fast_math]\ndef mac(a, b, c : double) : double \{\n    return a * b + c\n\}\n"
    compile("never_inline_probe", marked, CodeOfPolicies()) $(ok; prog; errors) {
        t |> success(ok, "the probe source compiles: {errors}")
        if (ok) {
            var found = 0
            get_this_module(prog) |> for_each_function("mac") $(fn) {
                found++
                t |> success(fn.moreFlags.neverInline, "an AST splice would move the arithmetic into an unmarked caller, so the marker keeps the function out of it")
            }
            t |> equal(1, found, "the probe defines mac")
        }
    }
}
