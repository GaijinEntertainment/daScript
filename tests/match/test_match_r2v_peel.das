options gen2
require dastest/testing_boost public
require daslib/match
require daslib/ast
require daslib/ast_boost

// ExprRef2Value transparency: the typer wraps reads of references in
// ExprRef2Value (no surface syntax), so AST class patterns peel it on the
// source side at every nesting level — mirroring qmatch. An explicit
// ExprRef2Value(...) pattern still matches the wrapper itself, and $v
// captures of Expression-typed values bind the peeled node.

[sideeffects]
def match_field_of_var(E : ExpressionPtr) : string {
    return match (E) {
        ExprField(name = fname, value = ExprVar(name = vname)) => "{vname}.{fname}"
        _ => "other"
    }
}

[sideeffects]
def match_wrapper_explicitly(E : ExpressionPtr) : string {
    return match (E) {
        ExprRef2Value(subexpr = ExprVar(name = vname)) => "wrapped {vname}"
        ExprVar(name = vname) => "bare {vname}"
        _ => "other"
    }
}

[sideeffects]
def capture_field_value(E : ExpressionPtr) : string {
    return match (E) {
        ExprField(value = base) => "{base.__rtti}"
        _ => "other"
    }
}

[test]
def test_r2v_peel(t : T?) {
    t |> run("top-level wrapper peels for class patterns") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "a")
            var fld <- new ExprField(name := "foo", value = v)
            var wrapped <- new ExprRef2Value(subexpr = fld)
            t |> equal("a.foo", match_field_of_var(wrapped))
        }
    }
    t |> run("nested wrapper peels at the field boundary") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "b")
            var wrappedVar <- new ExprRef2Value(subexpr = v)
            var fld <- new ExprField(name := "bar", value = wrappedVar)
            t |> equal("b.bar", match_field_of_var(fld))
        }
    }
    t |> run("unwrapped source still matches (no-op peel)") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "c")
            var fld <- new ExprField(name := "baz", value = v)
            t |> equal("c.baz", match_field_of_var(fld))
        }
    }
    t |> run("explicit ExprRef2Value pattern matches the wrapper itself") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "d")
            var wrapped <- new ExprRef2Value(subexpr = v)
            t |> equal("wrapped d", match_wrapper_explicitly(wrapped))
            var bare <- new ExprVar(name := "e")
            t |> equal("bare e", match_wrapper_explicitly(bare))
        }
    }
    t |> run("$v capture of an Expression field binds the peeled node") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "f")
            var wrappedVar <- new ExprRef2Value(subexpr = v)
            var fld <- new ExprField(name := "qux", value = wrappedVar)
            t |> equal("ExprVar", capture_field_value(fld))
        }
    }
    t |> run("double wrapper peels fully") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "g")
            var w1 <- new ExprRef2Value(subexpr = v)
            var w2 <- new ExprRef2Value(subexpr = w1)
            var fld <- new ExprField(name := "deep", value = w2)
            t |> equal("g.deep", match_field_of_var(fld))
        }
    }
}
