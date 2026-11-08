//! [ordered] on a struct with no fields must not compile: there is
//! nothing to order by.
options gen2
options no_aot
expect 20800

require daslib/cmp

[ordered]
struct A {
}

[export]
def main() {
    pass
}
