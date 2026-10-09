options gen2
options rtti
options no_coverage
options disable_auto_inline

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require dastest/testing_boost public
require daslib/strings_boost
require _optimized_body_helper

// a const global with a constant initializer folds on every value read, the right-hand
// side of a plain copy included - a store through a pointer as much as a local copy

let K_COPY : int = 7

[export]
def target_copy_through_pointer(var p : int?) {
    *p = K_COPY
}

[export]
def target_copy_to_local() : int {
    var x : int
    x = K_COPY  // nolint:STYLE011 - the plain copy is the shape under test
    return x
}

[test]
def test_const_global_folds_on_the_right_of_a_copy(t : T?) {
    ast_gc_guard() {
        for (nm in ["target_copy_through_pointer", "target_copy_to_local"]) {
            t |> run(nm) @(t : T?) {
                let b = body_of(t, nm)
                t |> success(!(b |> contains("K_COPY")), "the global read is folded, got:\n{b}")
                t |> success(b |> contains("= 7"), "the copy stores the constant, got:\n{b}")
            }
        }
    }
    var a = 0
    target_copy_through_pointer(unsafe(addr(a)))
    t |> equal(a, 7)
    t |> equal(target_copy_to_local(), 7)
}
