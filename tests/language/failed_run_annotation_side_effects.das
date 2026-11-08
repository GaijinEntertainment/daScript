//! a [run] function with side effects is not evaluated at compile
//! time and reports an error
options gen2
expect 50500

var {
    g = 5
}

[run]
def add13(n) {
    g = n
    return n + 13
}

[export]
def test {
    // 40101: function did not run at compilation time because it has
    // side-effects
    feint("{add13(3)}\n")
    return true
}


