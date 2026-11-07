options gen2

[never_inline]
def asserted(x : int) : int {
    assert(x > 0, "not positive")
    return x
}

[export]
def unused_asserted_call() : bool {
    var ex = false
    try {
        var _unused = asserted(-1)
    } recover {
        ex = true
    }
    return ex
}
