//! writing a field through a const pointer, directly or through safe
//! navigation, does not compile
// const value write violations
// verifies compiler rejects writes through const pointers,
// including direct field writes and safe-navigation writes
options gen2
expect 30952:3

struct Foo {
    i : int
}

struct Test {
    v : Foo const? const
}

[export]
def test() {
    let foo : Foo
    unsafe {
        var t = Test(v = addr(foo))
        // 30504: can't write to a constant value, int const& = int const
        t.v.i = 10
        var r = t.v
        // 30504: can't write to a constant value, int const& = int const
        r.i = 10
        var j = 10
        // 30504: can't write to a constant value, int const& = int const
        t.v?.i ?? j = 11
    }
    return true
}
