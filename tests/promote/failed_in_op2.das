//! An out-of-range literal in a compound assignment must report the
//! range error (30515) and not a numeric type mismatch.
options gen2

expect 30515

[export]
def test {
    var d : uint8 = 0
    d += 256
    return true
}
