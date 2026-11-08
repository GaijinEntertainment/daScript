//! A hex literal 0xFF (255) assigned to an int8 must not compile
//! (30515): hex literals are uint and promotion into int8
//! range-checks.
options gen2

expect 30515

[export]
def test {
    var d : int8 = 0xFF
    return true
}
