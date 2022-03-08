require daslib/rst_comment

struct Foo
    /* this is a C comment in structure */
    a : int // this is a C++ comment in the field
    b,c : int // this is a C++ comment for the fields b, c

struct Bar
    // this is a C++ comment in structure
    a : int


[export]
def foo ( a, b : int )
    // this is a C++ comment
    pass

[export]
def bar ( a, b : int )
    /* this is a C comment */
    pass

[export]
def main
    print("hello, world!\n")