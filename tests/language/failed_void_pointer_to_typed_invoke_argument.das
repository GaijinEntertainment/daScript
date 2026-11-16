//! a void? value that is not the null constant does not pass to a
//! typed pointer argument of an invoked block
expect 30107

options gen2

struct Foo {
    x : int
}

[export]
def main {
    let read_x = $(p : Foo?) : int {
        return p != null ? p.x : -1
    }
    var v : void? = null
    print("{invoke(read_x, v)}\n")
}
