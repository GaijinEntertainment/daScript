struct Foo
    a : array<int>

[export, unsafe]
def test
    let sa <- [{int 1;2;3;4}]
    let sb <- [{int 1;2;3}]
    let fa <- [[Foo a := sa ]]
    let fa2 <- [[Foo a := sa; a := sb ]]
    assert(length(sa)==4)
    assert(length(fa.a)==4)
    assert(length(fa2[0].a)==4)
    assert(length(fa2[1].a)==3)
    return true
