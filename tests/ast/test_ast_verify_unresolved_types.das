options gen2
require dastest/testing_boost public
require daslib/ast_boost
require daslib/ast_verify

// count_unresolved_types counts what the per-pass check reports

def int_type() : TypeDeclPtr => new TypeDecl(baseType = Type.tInt)

def auto_type() : TypeDeclPtr => new TypeDecl(baseType = Type.autoinfer)

[test]
def test_an_auto_typed_expression_is_counted(t : T?) {
    ast_gc_guard() {
        var e <- quote(1 + 2)
        e._type = auto_type()
        t |> equal(count_unresolved_types(e), 1)
    }
}

[test]
def test_an_alias_typed_expression_is_counted(t : T?) {
    ast_gc_guard() {
        var e <- quote(1 + 2)
        e._type = new TypeDecl(baseType = Type.alias, alias := "Pending")
        t |> equal(count_unresolved_types(e), 1)
    }
}

[test]
def test_an_unevaluated_dimension_is_counted(t : T?) {
    ast_gc_guard() {
        var e <- quote(1 + 2)
        e._type = new TypeDecl(baseType = Type.tFixedArray, firstType = int_type(), fixedDim = -2)
        t |> equal(count_unresolved_types(e), 1)
    }
}

[test]
def test_resolved_and_untyped_expressions_are_not_counted(t : T?) {
    ast_gc_guard() {
        var e <- quote(1 + 2)
        t |> equal(count_unresolved_types(e), 0)
        e._type = int_type()
        t |> equal(count_unresolved_types(e), 0)
    }
}

[test]
def test_a_block_literal_and_its_body_keep_auto(t : T?) {
    ast_gc_guard() {
        var e <- quote($(a) => a)
        var mk = e as ExprMakeBlock
        mk._type = new TypeDecl(baseType = Type.tBlock, firstType = auto_type())
        mk._block._type = auto_type()
        t |> equal(count_unresolved_types(e), 0)
    }
}

[test]
def test_an_empty_array_literal_keeps_auto(t : T?) {
    ast_gc_guard() {
        var e <- quote(default<array<auto>>)
        e._type = new TypeDecl(baseType = Type.tArray, firstType = auto_type())
        t |> success(e is ExprMakeStruct)
        t |> equal(count_unresolved_types(e), 0)
    }
}
