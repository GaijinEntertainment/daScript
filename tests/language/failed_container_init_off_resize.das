//! with default_init_containers off, resize of an array whose element
//! type has field initializers is unsafe again and does not compile
//! outside unsafe
options gen2
options default_init_containers = false
// unsafe_function_call - with the policy off, resize of an
// init-carrying element type is unsafe again
expect 31013

struct Foo {
    x : int = 7
}

[export]
def main {
    var arr : array<Foo>
    arr |> resize(2)
}
