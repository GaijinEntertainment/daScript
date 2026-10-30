options gen2
require dastest/testing_boost

var g_moved = 5

def move_global => (<- g_moved)

def move_through_ref(var a : int&) : int& {
    unsafe {
        return <- a
    }
}

[test]
def test_move_reference_return(t : T?) {
    t |> run("a moved global through a one-element tuple") @(t : T?) {
        t |> equal(move_global(), 5)
    }
    t |> run("a reference return under move semantics is the argument's address") @(t : T?) {
        var x = 7
        move_through_ref(x) = 9
        t |> equal(x, 9)
        t |> success(!jit_enabled() || is_jit_function(@@move_through_ref))
    }
}
