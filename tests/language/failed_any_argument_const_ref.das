// a builtin argument bound as `vec4f &` (any type, mutable reference)
// takes the same const and ref checks as a typed `var T&` argument,
// and a builtin's mutable reference argument rejects a fresh
// temporary
options gen2

expect 30341:3, 30107:3

struct Foo {
    a : int
}

let CFG = Foo(a = 1)

def make_foo() : Foo {
    return Foo(a = 3)
}

def load_temporaries(pick : bool; var held : Foo; bytes : array<uint8>) {
    _builtin_binary_load(Foo(a = 4), bytes)             // struct literal
    _builtin_binary_load(make_foo(), bytes)             // function result
    _builtin_binary_load(pick ? held : make_foo(), bytes) // `?:` with a temporary arm
}

def test {
    var bytes : array<uint8>
    let local = Foo(a = 2)
    _builtin_binary_load(CFG, bytes)        // const global
    _builtin_binary_load(local, bytes)      // const local
    _builtin_binary_load(1, bytes)          // non-ref value
    var held : Foo
    load_temporaries(true, held, bytes)
}
