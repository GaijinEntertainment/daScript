options strict_smart_pointers

require dastest/testing_boost public

require ast

struct Foo
    a : ExpressionPtr

[test]
def test_resize_array ( t : T? )
    t |> run("emplace") <| @ ( t : T? )
        if true
            var inscope a : array<ExpressionPtr>
            var inscope b <- new [[ExprOp2() op:="=="]]
            a |> emplace(b)
            t |> equal(1, length(a))
            t |> equal(null, b)
    t |> run("array init") <| @ ( t : T? )
        if true
            var inscope b <- new [[ExprOp2() op:="=="]]
            var inscope a <- [{ExpressionPtr[1] b}]
            t |> equal(1, length(a))
            t |> equal(null, b)
        if true
            var inscope b : ExpressionPtr <- new [[ExprOp2() op:="=="]]
            var inscope a <- [{ExpressionPtr[1] move_to_local(b)}]
            t |> equal(1, length(a))
            t |> equal(null, b)
        /*
        if true
            var inscope b : ExpressionPtr <- new [[ExprOp2() op:="=="]]
            var inscope a <- [{ExpressionPtr b}]    // 40103: can't create array from smart pointer by value. did u mean [{Type[1] value}] instead of [{Type value}]
            t |> equal(1, length(a))
            t |> equal(null, b)
        */
    t |> run("struct init") <| @ ( t : T? )
        if true
            var inscope b <- new [[ExprOp2() op:="=="]]
            var inscope a <- [[Foo a <- b]]
            t |> equal(null, b)


