require dastest/testing_boost

require jit

var count = 1

[jit]
def inc_global
    return count++

[test]
def test_global ( t:T? )
    t |> run("global") <| @ ( t : T? )
        t |> success(is_jit_function(@@inc_global))
        t |> equal(1, inc_global())
        t |> equal(2, inc_global())
        t |> equal(3, inc_global())
        t |> equal(4, inc_global())
        t |> equal(5, count)

