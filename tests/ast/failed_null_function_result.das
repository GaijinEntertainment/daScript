//! A macro that leaves a function without its result type yields a
//! malformed tree that the compiler reports as an internal-function
//! error.
options gen2
// internal_function — malformed AST, function is missing its result
// type
expect 50609

require _malformed_tree_macros

[null_result_fn]
struct Foo {
    a : int
}

[export]
def main {
    var f = Foo(a = 1)
    print("{f.a}\n")
}
