require dastest/testing_boost

require jit

var count = 1

let shared shar <- [{auto "shared"}]

[jit]
def inc_global
    return count++

[jit]
def get_first_shared
    return shar[0]

[test]
def test_global ( t:T? )
    t |> run("global") <| @ ( t : T? )
        t |> success(is_jit_function(@@inc_global))
        t |> equal(1, inc_global())
        t |> equal(2, inc_global())
        t |> equal(3, inc_global())
        t |> equal(4, inc_global())
        t |> equal(5, count)
    t |> run("shared") <| @ ( t : T? )
        t |> success(is_jit_function(@@get_first_shared))
        t |> equal("shared", get_first_shared())


