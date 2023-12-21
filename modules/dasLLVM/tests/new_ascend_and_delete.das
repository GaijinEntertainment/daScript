require dastest/testing_boost

require jit

require UnitTest

[jit]
def test_new ( t : T? )
    var f = new FancyClass()
    t |> equal( 13, f.value )

[jit]
def test_ascend ( t : T? )
    var f = new [[FancyClass() value=123]]
    t |> equal( 123, f.value )

[test]
def test_move ( t:T? )
    t |> run("new") <| @ ( t : T? )
        test_new(t)
    t |> run("ascend") <| @ ( t : T? )
        test_ascend(t)
