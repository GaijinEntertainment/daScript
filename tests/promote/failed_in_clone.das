//! An out-of-range literal assigned with := must report the range
//! error (30515) and not a clone-type mismatch.
options gen2

expect 30515

[export]
def test {
    var d : uint8
    d := 256
    return true
}
