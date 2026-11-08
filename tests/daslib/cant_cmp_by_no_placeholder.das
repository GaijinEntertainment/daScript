//! cmp_by(k) with a key that has no `_` placeholder must not compile:
//! every key has to read from the compared element.
options gen2
options no_aot
expect 50503

require daslib/cmp

struct A {
    x : int
}

[export]
def main() {
    var arr : array<A>
    let k = 1
    sort(arr) <| cmp_by(k)
}
