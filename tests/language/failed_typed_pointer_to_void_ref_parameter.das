//! a typed pointer variable does not bind to a var void?& parameter
//! outside unsafe: the callee could store any pointer into it
expect 31013

options gen2

struct Foo {
    x : int
}

def reset(var p : void?&) {
    p = null
}

[export]
def main {
    var q : Foo? = new Foo(x = 1)
    reset(q)
    print("{q}\n")
}
