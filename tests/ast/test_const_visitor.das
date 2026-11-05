options gen2
options no_aot

require dastest/testing_boost public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost

class CountInts : AstConstVisitor {
    ints : int = 0
    sum : int = 0

    def override preVisitExprConstInt(expr : ExprConstInt const?) : void {
        ints ++
        sum += expr.value
    }
}

def count_ints(node) : int2 {
    var visitor = new CountInts()
    make_const_visitor(*visitor) $(adapter) {
        visit(node, adapter)
    }
    let res = int2(visitor.ints, visitor.sum)
    unsafe {
        delete visitor
    }
    return res
}

[test]
def test_const_visitor(t : T?) {
    t |> run("a const visitor walks every node of an expression") @(t : T?) {
        ast_gc_guard() {
            var expr <- quote(1 + 2 * 3)
            t |> equal(int2(3, 6), count_ints(expr))
        }
    }
    t |> run("a const visitor walks the body of a function") @(t : T?) {
        ast_gc_guard() {
            var func <- qmacro_function("const_visitor_probe") $() : int {
                return 4 + 5
            }
            t |> equal(int2(2, 9), count_ints(func))
        }
    }
}
