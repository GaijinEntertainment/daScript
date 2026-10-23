options gen2
options force_inscope_pod
options rtti
options no_coverage
require dastest/testing_boost public
require daslib/ast
require daslib/ast_boost
require daslib/rtti

struct HundredInts {
    a : int[100]
}

// typeinfo leaves Holder's layout unresolved in the first pass
let HOLDER_DIM = typeinfo sizeof(type<HundredInts>) / 4

struct Holder {
    b : int[HOLDER_DIM]
    a : array<int>
}

def is_collect_call(expr : ExprCall?) : bool => (expr.func != null
    && expr.func.name == "builtin_collect_local_and_zero")

class CollectSizeFinder : AstVisitor {
    sizes : array<uint>
    inside_collect : int
    def override preVisitExprCall(var expr : ExprCall?) : void {
        if (is_collect_call(expr)) {
            inside_collect++
        }
    }
    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        if (is_collect_call(expr)) {
            inside_collect--
        }
        return expr
    }
    def override visitExprConstUInt(var expr : ExprConstUInt?) : ExpressionPtr {
        if (inside_collect > 0) {
            sizes |> push(expr.value)
        }
        return expr
    }
}

def collect_sizes(nm : string) : array<uint> {
    var sizes : array<uint>
    compiling_module() |> for_each_function(nm) $(func) {
        var v = new CollectSizeFinder()
        make_visitor(*v) $(adapter) {
            visit(func.body, adapter)
        }
        sizes |> push_from(v.sizes)
        unsafe {
            delete v
        }
    }
    return <- sizes
}

[export]
def target_move_holder() : int {
    var x, y : Holder
    y.a |> push(1)
    x <- y
    return length(x.a)
}

[test]
def test_inscope_move_zeroes_the_whole_value(t : T?) {
    t |> equal(target_move_holder(), 1)
    let sizes <- collect_sizes("target_move_holder")
    t |> success(!empty(sizes))
    for (sz in sizes) {
        t |> equal(sz, uint(typeinfo sizeof(type<Holder>)))
    }
}

variant BigVariant {
    p : int
    w : int4[8]
}

struct HolderWithInferredField {
    v = BigVariant(p = 1)
    a : array<int>
}

[export]
def target_move_holder_with_inferred_field() : int {
    var x : HolderWithInferredField <- HolderWithInferredField()
    var y : HolderWithInferredField <- HolderWithInferredField()
    y.a |> push(1)
    x <- y
    return length(x.a)
}

[test]
def test_inscope_move_zeroes_the_whole_value_with_inferred_field(t : T?) {
    t |> equal(target_move_holder_with_inferred_field(), 1)
    let sizes <- collect_sizes("target_move_holder_with_inferred_field")
    t |> success(!empty(sizes))
    for (sz in sizes) {
        t |> equal(sz, uint(typeinfo sizeof(type<HolderWithInferredField>)))
    }
}
