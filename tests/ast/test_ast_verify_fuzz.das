options gen2
require dastest/testing_boost public
require daslib/ast_boost
require daslib/ast_verify
require daslib/rtti

// Trees the verifier must accept. A quoted tree is never
// inferred, so an increment of a literal inside one is data,
// not a malformed l-value. A template structure is inferred
// only once instantiated, so the same holds inside one.
// typedecl() of a C++ enum constant takes its type from a
// binding with no source of its own.

variant TypedeclOfCppEnum {
    f : typedecl(Type.tInt)
}

struct template DecrementOfAConstant {
    typedef T = int[0 --]
}

[test]
def test_quoted_increment_of_a_constant(t : T?) {
    ast_gc_guard() {
        let q = quote(0 ++)
        t |> success(q != null)
    }
}

[test]
def test_typedecl_of_a_cpp_enum(t : T?) {
    let v : TypedeclOfCppEnum
    t |> success(v is f)
}

[test]
def test_template_structure_decrement_of_a_constant(t : T?) {
    t |> success(true)
}
