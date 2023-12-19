require dastest/testing_boost

require jit

require UnitTest

[jit]
def test_using( t : T? )
    using() <| $( var f : FancyClass )
        t |> equal(13, f.value)
    using(1,3) <| $( var f : FancyClass )
        t |> equal(4, f.value)

[jit]
def test_placement_new( t : T? )
    var fc0 = FancyClass()
    t |> equal(13, fc0.value)
    var fc = FancyClass(1,2)
    t |> equal(3, fc.value)

[test]
def test_ref ( t:T? )
    t |> run("using") <| @ ( t : T? )
        test_using(t)
    t |> run("placement new") <| @ ( t : T? )
        test_placement_new(t)

