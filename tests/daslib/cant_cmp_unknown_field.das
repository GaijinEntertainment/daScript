//! [ordered(nope)] must not compile: a field name the struct does not
//! have is rejected.
options gen2
options no_aot
expect 20800

require daslib/cmp

[ordered(nope)]
struct A {
    x : int
}

[export]
def main() {
    pass
}
