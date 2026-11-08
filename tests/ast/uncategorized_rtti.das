options gen2
options rtti

require dastest/testing_boost public
require daslib/ast
require _test_collect_deps_empty_helper
require daslib/cpp_bind
require daslib/ast_boost
require strings
require daslib/strings_boost
require daslib/fio
require daslib/clargs
require daslib/templates_boost

//! A bare `return` inside a block exits THE BLOCK only. The interpreter's invokeEx code0 fast
//! path handed enumerators (for_each_function et al.) the closure body's single statement WITHOUT
//! the SimNode_ClosureBlock epilogue that clears stopForReturn — so the first firing return
//! killed the remaining iterations AND leaked the stop flag into the calling function, silently
//! skipping its tail (rst.das documents() lost every Uncategorized function this way). The
//! single-statement and multi-statement closures below are semantically identical; they must
//! count the same. JIT and AOT were always correct — this is an interpreter-only regression rail.

// single-statement closure body (one IfThenElse) — rides the code0 fast path
def private count_single(mod : rtti::Module?) : int {
    var n = 0
    for_each_function(mod, "") $(func) {
        if (func.flags.generated) {
            return
        } else {
            n++
        }
    }
    return n
}

// multi-statement closure body — never rode the fast path, always correct
def private count_multi(mod : rtti::Module?; var fired : int&) : int {
    var n = 0
    for_each_function(mod, "") $(func) {
        if (func.flags.generated) {
            fired++
            return
        }
        n++
    }
    return n
}

// tail sentinel: a leaked stop flag would skip everything after the enumeration
def private tail_runs(mod : rtti::Module?) : bool {
    for_each_function(mod, "") $(func) {
        if (func.flags.generated) {
            return
        }
    }
    return true
}

[test(description = "a return inside a closure block is handled correctly for a single closure, several closures and a tail statement")]
def test_closure_block_return(t : T?) {
    ast_gc_guard() {
        var mod = get_module("$")
        var fired = 0
        let multi = count_multi(mod, fired)
        t |> success(fired > 0, "the early return must actually fire (module $ has generated fns)")
        t |> success(multi > 0, "enumeration continues past a block return")
        t |> equal(count_single(mod), multi)
        t |> success(tail_runs(mod), "block return must not leak past the enumeration")
    }
}

//! Regression: collect_dependencies on a function with NO function-deps AND NO
//! variable-deps used to leave the dep arrays uninitialised (Array.magic == garbage), so
//! the das-side `for (... in ...)` lock-magic check tripped "array magic mismatch on lock"
//! and the macro_function callers (dasGlsl, dasSpirv) crashed at the first iteration.
//! Fixed in src/simulate/simulate_fn_hash.cpp by calling array_mark_locked unconditionally
//! with a (nullptr, 0) pair when the dep list is empty. The helper module's
//! [probe_empty_deps] annotation calls collect_dependencies inside its patch() — before the
//! fix that crashed the compile pass; after the fix the patch returns cleanly and the test
//! file itself compiles + runs.

// A function whose body references neither a function nor a global. patch() of
// [probe_empty_deps] runs at compile time and exercises the (now-fixed) zero-dep
// collect_dependencies path. The pre-fix daslang panics here before any test runs; the
// post-fix daslang compiles + arrives at the assertion below.
[probe_empty_deps]
def private trivial_no_deps {
}

[test(description = "collecting the dependencies of a function with none does not crash")]
def test_collect_deps_empty_no_crash(t : T?) {
    // If we got this far, [probe_empty_deps]'s collect_dependencies block iterated empty
    // function + variable arrays without tripping the lock-magic check. The pre-fix
    // daslang would have aborted the compile at the patch() `for (_ in vfun)` line above.
    t |> success(true, "collect_dependencies on a no-dep function did not crash at compile time")
}

// Compiler AST inspection/code generation is tested in the interpreter.

class AdapterProbe {
    def abstract required(value : int) : int
    def defaulted(value : int) : int => value + 1
    def hidden(value : int) : int => value - 1
}

[test(description = "the C++ adapter lists the default callbacks it explicitly includes")]
def adapter_explicitly_includes_default_callbacks(t : T?) {
    ast_gc_guard() {
        var declaration = typeinfo ast_typedecl(type<AdapterProbe>)
        let output = build_string() $(var writer) {
            log_cpp_class_adapter(writer, "Adapter", declaration, ["defaulted"])
        }
        t |> success(contains(output, "invoke_required"))
        t |> success(contains(output, "invoke_defaulted"))
        t |> success(!contains(output, "invoke_hidden"))
        let legacy = build_string() $(var writer) {
            log_cpp_class_adapter(writer, "Adapter", declaration)
        }
        t |> success(contains(legacy, "invoke_required"))
        t |> success(!contains(legacy, "invoke_defaulted"))
    }
}

[test(description = "naming an unknown default callback for the C++ adapter is an error")]
def adapter_unknown_default_callback_is_an_error(t : T?) {
    var output : string
    let code = run_and_capture([get_host_binary(), "-dasroot", get_das_root(), "{get_das_root()}/tests/ast/_fixture_cpp_adapter_missing.das"], output, 15.0)
    t |> success(code != 0)
    t |> success(contains(output, "adapter method missing does not exist"))
}

//! describe() must be a pure function of node structure. The printer marks sticky per-node
//! print flags (argLevel / topLevel / bottomLevel); without the pre-print wipe a node once
//! printed as a call argument keeps argLevel forever and describes WITHOUT outer parens,
//! while an identical fresh node describes with them. CSE value-numbering keys on describe,
//! so the unstable key silently loses shares (glitch_2d spelled `_cse_2 + _cse_1` twice and
//! shared it zero times).

[test(description = "describe reports the purity of a type")]
def test_describe_purity(t : T?) {
    ast_gc_guard() {
        var fresh = qmacro(1f - x)
        let d0 = "{describe(fresh)}"
        // print an identical sum in a CALL-ARG position — this used to stamp argLevel on it
        // for good, so its next standalone describe dropped the outer parens
        var wrapped = qmacro(sin(1f - x))
        let _whole = "{describe(wrapped)}"
        if (wrapped is ExprCall) {
            var inner = (wrapped as ExprCall).arguments[0]
            t |> equal("{describe(inner)}", d0, "print history must not change a subtree's describe")
        } else {
            t |> failure("expected qmacro(sin(…)) to parse as ExprCall, got {wrapped.__rtti}")
        }
        // the fresh node's own describe must not drift across repeated prints either
        t |> equal("{describe(fresh)}", d0, "repeated describe of the same node must be stable")
    }
}

[test(description = "describe reports shared element types once")]
def test_describe_shared_element_types(t : T?) {
    ast_gc_guard() {
        var element = new TypeDecl(baseType = Type.tFloat)
        for (kind, expected in [Type.tTuple, Type.tVariant, Type.option],
                ["tuple<a:float;b:float;c:float>",
                 "variant<a:float;b:float;c:float>", "float|float|float"]) {
            var compound = new TypeDecl(baseType = kind)
            compound.argTypes |> resize(3)
            if (kind != Type.option) {
                compound.argNames |> resize(3)
            }
            for (name, i in ["a", "b", "c"], range(3)) {
                compound.argTypes[i] = element
                if (kind != Type.option) {
                    compound.argNames[i] := name
                }
            }
            t |> equal(describe(compound), expected,
                "elements sharing one TypeDecl are each separated")
        }
    }
}
