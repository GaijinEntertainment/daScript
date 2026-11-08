options gen2
options strict_smart_pointers

require dastest/testing_boost public
require daslib/ast

struct SspFoo {
    a : ExpressionPtr
}

[test(description = "under strict_smart_pointers an ExpressionPtr moves into an array by emplace, array construction and move_to_local, and into a struct field by <-")]
def test_resize_array(t : T?) {
    t |> run("emplace") @(t : T?) {
        ast_gc_guard() {
            var a : array<ExpressionPtr>
            var b <- new ExprOp2(op := "==")
            a |> emplace(b)
            t |> equal(1, length(a))
            t |> equal(null, b)
        }
    }
    t |> run("array init") @(t : T?) {
        ast_gc_guard() {
            var b : ExpressionPtr <- new ExprOp2(op := "==")
            let a <- array<ExpressionPtr>(b)
            t |> equal(1, length(a))
            // Note: GC-managed ExpressionPtr is not zeroed by array construction (pass by value)
        }
        ast_gc_guard() {
            var b : ExpressionPtr <- new ExprOp2(op := "==")
            let a <- array<ExpressionPtr>(move_to_local(b))
            t |> equal(1, length(a))
            // Note: GC-managed ExpressionPtr is not zeroed by move_to_local
        }
    }
    t |> run("struct init") @(t : T?) {
        ast_gc_guard() {
            var b : ExpressionPtr <- new ExprOp2(op := "==")
            var _a <- SspFoo(a <- b)
            // Note: GC-managed ExpressionPtr is not zeroed by struct construction move
        }
    }
}

var GOO = 1

class Bar {
    def abstract foobar(a : int = 0) : void
}

class Foo : Bar {
    def override foobar(a : int = 0) : void {
        GOO = a
    }
}

[test(description = "an overridden method's default argument applies when the call omits it, through a pointer and through a dereferenced pointer alike")]
def test_default_args(t : T?) {
    t |> equal(GOO, 1)
    var a = new Foo()
    a->foobar()
    t |> equal(GOO, 0)
    a->foobar(2)
    t |> equal(GOO, 2)
    (*a)->foobar()
    t |> equal(GOO, 0)
    (*a)->foobar(2)
    t |> equal(GOO, 2)
}

class StaticCounter {
    static count : int = 1
    static mount : array<int> <- array<int>(1, 2, 3, 4)
    def next => count++
    def get => count
    def static next_func => count++
}

[test(description = "a class static variable is shared by every instance and by the class name, and a static function updates it")]
def test_static_members(t : T?) {
    t |> run("variables") @(t : T?) {
        var c1 = new StaticCounter()
        t |> equal(1, c1->get())
        StaticCounter`count = 13
        t |> equal(13, c1->get())
        t |> equal(13, c1->next())
        t |> equal(14, c1->next())
        var c2 = new StaticCounter()
        t |> equal(15, c2->next())
        t |> equal(16, StaticCounter`count)
        t |> equal(16, c1->get())
        t |> equal(4, length(StaticCounter`mount))
    }
    t |> run("functions") @(t : T?) {
        StaticCounter`count = 13
        t |> equal(13, StaticCounter`next_func())
        t |> equal(14, StaticCounter`next_func())
    }
}
