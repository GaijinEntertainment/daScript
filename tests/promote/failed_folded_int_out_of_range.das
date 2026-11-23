//! An int constant folded from a call that does not fit the other
//! operand must report the range error (30515) and nothing else.
options gen2

expect 30515

[export]
def test {
    var d : uint8 = 0u8
    d += int(256)
    return true
}
