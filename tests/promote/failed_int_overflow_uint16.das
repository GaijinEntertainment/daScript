//! 65536 assigned to a uint16 must not compile (30515), the range
//! being 0..65535.
options gen2

expect 30515

[export]
def test {
    var d : uint16 = 65536
    return true
}
