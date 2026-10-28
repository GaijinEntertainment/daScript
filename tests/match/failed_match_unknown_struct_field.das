// a struct pattern naming a missing field is an error, not a crash
options gen2

require daslib/match

expect 50503

struct Foo {
    a : int
}

[export]
def test {
    let f = Foo(a = 1)
    return match (f) {
        Foo(b = 1) => true
        _ => false
    }
}
