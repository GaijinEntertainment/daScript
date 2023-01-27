require dastest/testing_boost public

require daslib/contracts

def take_not ( a )
    return 1

[!expect_dim(a)]
def take_not ( a )
    return 2

[expect_any_tuple(a) || expect_any_variant(a)]
def take_or ( a )
    return 1

def take_or ( a )
    return 2

[expect_any_tuple(a) && expect_any_variant(b)]
def take_and ( a, b )
    return 1

def take_and ( a, b )
    return 2

[expect_any_tuple(a) ^^ expect_any_variant(b)]
def take_xor ( a, b )
    return 1

def take_xor ( a, b )
    return 2


variant Foo
    i : int
    f : float

[test]
def test_logical_ops ( t : T? )
    t |> run("!") <| @ ( t : T? )
        var a : int[3]
        t |> equal(1, take_not(a))
        t |> equal(2, take_not(13))
    t |> run("||") <| @ ( t : T? )
        t |> equal(2, take_or(13))
        t |> equal(1, take_or([[auto 1,"2",3.]]))
        t |> equal(1, take_or([[Foo i=1]]))
    t |> run("&&") <| @ ( t : T? )
        t |> equal(2, take_and(13,13))
        t |> equal(2, take_and(13,[[Foo i=1]]))
        t |> equal(2, take_and([[auto 1,"2",3.]],13))
        t |> equal(1, take_and([[auto 1,"2",3.]],[[Foo i=1]]))
    t |> run("^^") <| @ ( t : T? )
        t |> equal(2, take_xor(13,13))
        t |> equal(1, take_xor(13,[[Foo i=1]]))
        t |> equal(1, take_xor([[auto 1,"2",3.]],13))
        t |> equal(2, take_xor([[auto 1,"2",3.]],[[Foo i=1]]))



