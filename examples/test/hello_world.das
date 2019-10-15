// options log_infer_passes=true

options log=true

require aaa
require ccc
require bbb

[export]
def test()
    var a : A
    var b : A
    a := b

    var aa : array<A>
    var bb : array<A>
    aa := bb

    // boo()

    return true
