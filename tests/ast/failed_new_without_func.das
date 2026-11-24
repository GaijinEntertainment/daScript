//! An initializer with no resolved function reports error 50613
//! instead of crashing.
options gen2
expect 50613

require _clone_inferred_new_macros

class Foo {
    x : int = 13
}

def make_foo() : Foo? {
    return new Foo()
}

def target_null_func() : Foo? {
    return null
}

[export]
def main {
    print("{target_null_func()}\n")
}
