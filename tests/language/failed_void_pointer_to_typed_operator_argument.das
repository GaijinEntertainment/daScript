//! a void? value that is not the null constant does not pass to a
//! typed pointer argument of a user-defined operator
expect 30107

options gen2

struct Foo {
    x : int
}

def operator +(a, b : Foo?) : int => a.x + b.x

[export]
def main {
    var q = new Foo(x = 1)
    var v : void? = q
    print("{q + v}\n")
}
