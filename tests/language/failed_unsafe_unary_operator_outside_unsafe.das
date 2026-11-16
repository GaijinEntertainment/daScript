//! a unary operator marked [unsafe_operation] does not compile
//! outside an unsafe block
expect 31022

options gen2

struct Foo {
    x : int
}

[unsafe_operation]
def operator -(a : Foo) : Foo => Foo(x = -a.x)

[export]
def main {
    var f = Foo(x = 3)
    let g = -f
    print("{g.x}\n")
}
