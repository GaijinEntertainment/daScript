//! An out-of-range field value in a struct constructor call must not
//! compile (30515).
options gen2

expect 30515

struct Foo {
    x : uint8
}

[export]
def test {
    var f = Foo(x = 256)
    return f.x == 0u8
}
