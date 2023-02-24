expect 30305:2

require dastest/testing_boost public

let a = x                   //  this is fine, constant will fold
let x = 1234

var g_b := g_a              //  30305: global variable g_a is initialized after g_b
var g_a : array<int> <- g_a //  30305: global variable g_a cant't be initialized with itself

[test]
def test_global_variable_init_order( t: T? )
    t |> equal(1234, a)
    t |> equal(1234, x)


