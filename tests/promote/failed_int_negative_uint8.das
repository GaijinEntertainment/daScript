//! -1 assigned to a uint8 must not compile (30515), because uint8 is
//! unsigned.
options gen2

expect 30515

[export]
def test {
    var d : uint8 = -1
    return true
}
