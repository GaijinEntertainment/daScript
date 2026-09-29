options gen2
require dastest/testing_boost public
require daslib/rtti
require daslib/ast
require llvm/daslib/llvm_jit_plan

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

//! two functions with identical bodies; `on_first` puts `ann` on the first, else on the second
def private two_functions(ann : string; on_first : bool) : string {
    let first = on_first ? "{ann}\n" : ""
    let second = on_first ? "" : "{ann}\n"
    return "options gen2\n{first}def first_mac(a, b, c : double) : double \{\n    return a * b + c\n\}\n{second}def second_mac(a, b, c : double) : double \{\n    return a * b + c\n\}\n"
}

[test]
def test_hint_folds_bind_to_their_function(t : T?) {
    let hint = "[hint(noinline)]"
    t |> equal(fold_source_function_hints(t, two_functions(hint, true)), fold_source_function_hints(t, two_functions(hint, true)), "the fold is a function of the source")
    t |> success(fold_source_function_hints(t, two_functions(hint, true)) != fold_source_function_hints(t, two_functions(hint, false)), "a [hint] moved to another function changes the key")
}

