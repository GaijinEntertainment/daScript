//! -129 assigned to an int8 must not compile (30515), the range being
//! -128..127.
options gen2

expect 30515

[export]
def test {
    var d : int8 = -129
    return true
}
