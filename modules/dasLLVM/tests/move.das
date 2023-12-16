require dastest/testing_boost

require jit

struct Foo
    a : int
    b : float

[jit]
def move_jit ( var a, b )
    a <- b

[test]
def test_move ( t:T? )
    t |> run("move") <| @ ( t : T? )
        t |> success(is_jit_function(@@<(var a,b:Foo):void>move_jit))
        var a, b : Foo
        b = [[Foo a=1,b=2.0]]
        move_jit(a, b)
        t |> equal(1, a.a)
        t |> equal(2.0, a.b)
        t |> equal(0, b.a)
        t |> equal(0.0, b.b)
