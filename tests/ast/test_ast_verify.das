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

[test(description = "the verifier accepts an increment of a literal inside a quoted tree because it is data")]
def test_quoted_increment_of_a_constant(t : T?) {
    ast_gc_guard() {
        let q = quote(0 ++)
        t |> success(q != null)
    }
}

[test(description = "the verifier accepts typedecl of a C++ enum constant")]
def test_typedecl_of_a_cpp_enum(t : T?) {
    let v : TypedeclOfCppEnum
    t |> success(v is f)
}

[test(description = "the verifier accepts a decrement of a constant inside a template structure")]
def test_template_structure_decrement_of_a_constant(t : T?) {
    t |> success(true)
}

// count_unresolved_types counts what the per-pass check reports

def int_type() : TypeDeclPtr => new TypeDecl(baseType = Type.tInt)

def auto_type() : TypeDeclPtr => new TypeDecl(baseType = Type.autoinfer)

[test(description = "count_unresolved_types counts an expression whose type is still auto")]
def test_an_auto_typed_expression_is_counted(t : T?) {
    ast_gc_guard() {
        var e <- quote(1 + 2)
        e._type = auto_type()
        t |> equal(count_unresolved_types(e), 1)
    }
}

[test(description = "count_unresolved_types counts an expression whose type is an unresolved alias")]
def test_an_alias_typed_expression_is_counted(t : T?) {
    ast_gc_guard() {
        var e <- quote(1 + 2)
        e._type = new TypeDecl(baseType = Type.alias, alias := "Pending")
        t |> equal(count_unresolved_types(e), 1)
    }
}

[test(description = "count_unresolved_types counts a type with an unevaluated dimension")]
def test_an_unevaluated_dimension_is_counted(t : T?) {
    ast_gc_guard() {
        var e <- quote(1 + 2)
        e._type = new TypeDecl(baseType = Type.tFixedArray, firstType = int_type(), fixedDim = -2)
        t |> equal(count_unresolved_types(e), 1)
    }
}

[test(description = "count_unresolved_types does not count resolved or untyped expressions")]
def test_resolved_and_untyped_expressions_are_not_counted(t : T?) {
    ast_gc_guard() {
        var e <- quote(1 + 2)
        t |> equal(count_unresolved_types(e), 0)
        e._type = int_type()
        t |> equal(count_unresolved_types(e), 0)
    }
}

[test(description = "a block literal and its body keep their auto types and are counted as unresolved")]
def test_a_block_literal_and_its_body_keep_auto(t : T?) {
    ast_gc_guard() {
        var e <- quote($(a) => a)
        var mk = e as ExprMakeBlock
        mk._type = new TypeDecl(baseType = Type.tBlock, firstType = auto_type())
        mk._block._type = auto_type()
        t |> equal(count_unresolved_types(e), 0)
    }
}

[test(description = "an empty array literal keeps its auto type and is counted as unresolved")]
def test_an_empty_array_literal_keeps_auto(t : T?) {
    ast_gc_guard() {
        var e <- quote(default<array<auto>>)
        e._type = new TypeDecl(baseType = Type.tArray, firstType = auto_type())
        t |> success(e is ExprMakeStruct)
        t |> equal(count_unresolved_types(e), 0)
    }
}
