//! a void? value that is not the null constant does not pass to a
//! typed pointer parameter: the callee would dereference it as that
//! type
expect 30107

options gen2

struct Foo {
    x : int
}

def read_x(p : Foo?) : int => p != null ? p.x : -1

[export]
def main {
    var v : void? = null
    print("{read_x(v)}\n")
}
