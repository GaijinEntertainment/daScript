//! a struct-local typedef that refers to itself through array does
//! not compile: the alias loop is detected
options gen2
expect 31106

struct S {
    typedef T = array<T>
}

[export]
def main {
    pass
}
