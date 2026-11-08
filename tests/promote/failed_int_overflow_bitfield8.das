//! 256 assigned to a bitfield8 must not compile (30515).
options gen2

expect 30515

[export]
def test {
    var d : bitfield8 = 256
    return true
}
