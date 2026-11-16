//! a ternary of a void? variable and a typed pointer variable is a
//! value, not a reference: an assignment through it could store a
//! void? into the typed variable
expect 30915

options gen2

struct Foo {
    x : int
}

[export]
def main {
    var q = new Foo(x = 1)
    var v : void? = q
    let yes = q != null
    (yes ? v : q) = v
    print("{q.x}\n")
}
