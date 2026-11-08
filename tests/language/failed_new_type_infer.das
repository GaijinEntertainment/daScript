//! new of a non-struct type through a generic, or of an undefined
//! type name, does not compile
// new type inference failures
// verifies compiler rejects `new` on non-struct types (via generic)
// and `new` on undefined type names
options gen2
// options log_infer_passes=true

expect 30165, 30214, 30823

struct FooBar {
    a, b : int
}

def oops(a : auto(SomeType)) {
    // 30109: only tuples, variants, structures or handled types can be
    // allocated on the heap, not 'int aka SomeType'
    return new SomeType
}

def test {
    new Foo                 // 30301: undefined new expression type 'Foo'
    // 30101: cannot infer type for function call 'oops' with 'auto'
    // return type
    oops(1)
    let t : FooBar
    oops(t)
}
