options gen2
expect 50503

require daslib/with_boost

struct A {
    f1 : int
}

[export]
def main {
    var a <- [A(f1=1)]
    var b <- [A(f1=2)]
    with_(a[0], b[0]) $(var x : A; var y : A) {  // refuse: multi-arg + return
        x.f1 = 1
        return x.f1 + y.f1
    }
    delete a
    delete b
}
