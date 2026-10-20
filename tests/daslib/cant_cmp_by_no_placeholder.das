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
