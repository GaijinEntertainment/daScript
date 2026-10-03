options gen2
require dastest/testing_boost public

def private read_with_cleanup(value : int const?; enabled : bool; var cleanup_value : int&) : int {
    if (!enabled) { return 0 }
    let selected = value
    if (selected == null) { return -1 }
    var result : int
    { result = *selected } finally { cleanup_value += *selected }
    return result
}

[test]
def const_pointer_hoisted_through_early_return_retains_lvalue(t : T?) {
    var value = 37
    var cleanup_value = 0
    t |> equal(read_with_cleanup(unsafe(addr(value)), true, cleanup_value), 37)
    t |> equal(cleanup_value, 37)
    t |> equal(read_with_cleanup(null, true, cleanup_value), -1)
    t |> equal(read_with_cleanup(unsafe(addr(value)), false, cleanup_value), 0)
    t |> equal(cleanup_value, 37, "early returns do not execute cleanup from an unentered scope")
}
