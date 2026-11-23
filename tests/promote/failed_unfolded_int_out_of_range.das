//! With infer-time folding off, an int constant from a call that
//! does not fit the other operand must report the range error (30515)
//! and nothing else: inference still ends.
options gen2
options infer_time_folding = false

expect 30515

[export]
def test {
    var d : uint8 = 0u8
    d += int(256)
    return true
}
