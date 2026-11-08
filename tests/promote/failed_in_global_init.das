//! An out-of-range initializer of a global variable must not compile
//! (30515).
options gen2

expect 30515

var g : uint8 = 256

[export]
def test {
    return g == 0u8
}
