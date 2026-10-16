options gen2
require dastest/testing_boost public
require daslib/match
require daslib/ast
require daslib/ast_boost

[sideeffects]
def op2_left_name(E : ExpressionPtr) : string {
    return match (E) {
        ExprOp2(op = "+" | "-") & o2 =>
            o2.left is ExprVar ? "{o2.op} {(o2.left as ExprVar).name}" : "{o2.op}"
        _ => "other"
    }
}

[sideeffects]
def var_name_len(E : ExpressionPtr) : int {
    match (E) {
        ExprVar() & v => $ {
            return length(v.name)
        }
    }
    return -1
}

[sideeffects]
def const_value(E : ExpressionPtr) : int {
    return match (E) {
        k & ExprConstInt() => k.value
        _ => -1
    }
}

[sideeffects]
def class_only(E : ExpressionPtr) : string {
    return match (E) {
        ExprVar() => "var"
        ExprConstInt() => "int"
        _ => "other"
    }
}

[test]
def test_match_ast_node_bind(t : T?) {
    t |> run("& binds the node at the class the pattern tests") @@(t : T?) {
        ast_gc_guard() {
            var a <- new ExprVar(name := "a")
            var b <- new ExprVar(name := "b")
            var sum <- new ExprOp2(op := "+", left = a, right = b)
            t |> equal("+ a", op2_left_name(sum))
            var c <- new ExprVar(name := "c")
            t |> equal("other", op2_left_name(c))
        }
    }
    t |> run("& binds the peeled node in a statement arm") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "abc")
            var wrapped <- new ExprRef2Value(subexpr = v)
            t |> equal(3, var_name_len(wrapped))
            var k <- new ExprConstInt(value = 1)
            t |> equal(-1, var_name_len(k))
        }
    }
    t |> run("a name on the left of & binds the node at the class too") @@(t : T?) {
        ast_gc_guard() {
            var k <- new ExprConstInt(value = 42)
            t |> equal(42, const_value(k))
            var v <- new ExprVar(name := "x")
            t |> equal(-1, const_value(v))
        }
    }
    t |> run("a class pattern with no fields tests the class only") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "x")
            var k <- new ExprConstInt(value = 7)
            var f <- new ExprConstFloat(value = 1.0)
            t |> equal("var", class_only(v))
            t |> equal("int", class_only(k))
            t |> equal("other", class_only(f))
            t |> equal("other", class_only(null))
        }
    }
}
