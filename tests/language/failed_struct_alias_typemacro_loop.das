//! a struct-local typedef that refers to itself through an unknown
//! type macro does not compile: the alias loop is detected
options gen2
expect 31106

struct S {
    typedef T = Unknown<T>
}

[export]
def main {
    pass
}
