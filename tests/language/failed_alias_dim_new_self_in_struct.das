//! a struct-scoped type alias whose dimension allocates the alias
//! itself does not compile, and inference terminates
options gen2
expect 30109, 30823

struct S {
    typedef T = int[new T] | #
}

[export]
def main {
    pass
}
