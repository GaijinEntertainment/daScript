//! An out-of-range default in a struct field declaration must not
//! compile (30515).
options gen2

expect 30515

struct Foo {
    x : uint8 = 256
}

[export]
def test {
    var f = Foo()
    return f.x == 0u8
}
