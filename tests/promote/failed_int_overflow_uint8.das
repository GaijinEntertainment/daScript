//! 256 assigned to a uint8 must not compile with exactly one
//! exceeds-constant-range error (30515).
options gen2

expect 30515

[export]
def test {
    var d : uint8 = 256
    return true
}
