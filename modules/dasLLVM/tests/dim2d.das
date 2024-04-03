require dastest/testing_boost

require jit

var x : int[4][5]

[no_jit]
def init_no_jit
    var val = 0
    for i in range(4)
        for j in range(5)
            x[i][j] = val++

[jit]
def check_order_jit: bool
    var val = 0
    for i in range(4)
        for j in range(5)
            return false if x[i][j] != val++
    return true

[test, no_jit]
def test_dim ( t:T? )
    t |> run("multidim access") <| @ ( t : T? )
        t |> success ( is_jit_function(@@<():bool>check_order_jit) )
        init_no_jit()
        t |> success ( check_order_jit() )
