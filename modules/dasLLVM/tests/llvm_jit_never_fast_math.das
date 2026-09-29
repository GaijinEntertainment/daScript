options gen2
require dastest/testing_boost public
require daslib/fio
require _spawn_common
require strings
require daslib/rtti
require daslib/ast
require _exe_test_common

// Under options fast_math every FP instruction is stamped reassoc|nsz|contract before the optimizer;
// a [never_fast_math] function, and a block literal written in its body, keep no flags
// (modules/dasLLVM/ARCHITECTURE_FAST_MATH.md#never-fast-math). The check reads the dumped IR, so it
// holds on every host - the numeric failure it prevents shows only on an AVX-512 target.

//! the dumped IR of the function whose quoted name ends in `name_end`; signature to closing brace
def private function_ir(dump : string; name_end : string) : string {
    let start = find(dump, "{name_end}\"(")
    if (start < 0) {
        return ""
    }
    let body = slice(dump, start)
    let close = find(body, "\n}")
    return close < 0 ? body : slice(body, 0, close)
}

def private expect_unstamped(t : T?; ir, what : string) {
    t |> success(!empty(ir), "the dump carries {what}")
    t |> success(find(ir, "fmul double") >= 0, "{what} keeps its multiply:\n{ir}")
    t |> equal(-1, find(ir, "reassoc"), "{what} carries no fast-math flag:\n{ir}")
}

[test]
def test_never_fast_math_keeps_flags_off(t : T?) {
    let root = "modules/dasLLVM/tests/_never_fast_math_root.das"
    var out = ""
    var rc = 0
    with_child_log_info() {
        // -jit-no-cache: a cache hit runs no codegen, so it prints no dump
        rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", "-jit-no-cache", root, "--", "--jit-dump"], out, 300.0)
    }
    t |> equal(0, rc, "the fixture runs under -jit: {tail(out)}")
    t |> success(find(out, "MACS 4 4 4") >= 0, "the fixture printed its three results: {tail(out)}")
    let fast = function_ir(out, "`fast_mac Cd Cd Cd implementation")
    t |> success(!empty(fast), "the dump carries fast_mac: {tail(out)}")
    t |> success(find(fast, "fmul reassoc nsz contract double") >= 0, "an unmarked function is stamped under options fast_math:\n{fast}")
    expect_unstamped(t, function_ir(out, "`precise_mac Cd Cd Cd implementation"), "the [never_fast_math] function")
    expect_unstamped(t, function_ir(out, "_block_ Cd implementation"), "a block literal in a [never_fast_math] function")
}

[test]
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
