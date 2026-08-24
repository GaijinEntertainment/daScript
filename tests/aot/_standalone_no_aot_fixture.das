options gen2

[no_aot]
def slow_helper(x : int) : int {
    return x * 3
}

[export]
def use_helper(x : int) : int {
    return slow_helper(x) + 1
}
