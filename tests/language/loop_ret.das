options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public
require daslib/random

var lr_seed : int4

typedef LrFoo = tuple<int; float3>

def lr_foo(var attached_fires : array<LrFoo>) {
    let tf = random_int(lr_seed) % 100
    for (x in attached_fires) {
        if (x._0 == tf) {
            return
        }
    }
    push(attached_fires, (tf, float3(0)))
}

var lr_polls = 0

[never_inline]
def lr_more() : bool {
    lr_polls++
    return lr_polls < 100
}

def lr_return_from_while() : int {
    while (lr_more()) {
        if (lr_polls >= 3) {
            return lr_polls
        }
        var inscope seen : array<int>
        seen |> push(lr_polls)
    }
    return -1
}

[test]
def test_loop_ret(t : T?) {
    t |> run("loop early return fills unique entries") @(t : T?) {
        var a : array<LrFoo>
        for (_x in range(1000)) {
            lr_foo(a)
        }
        t |> equal(length(a), 100)
    }
    t |> run("return leaves a while loop whose body has a finalizer") @(t : T?) {
        t |> equal(lr_return_from_while(), 3)
        t |> equal(lr_polls, 3)
    }
}
