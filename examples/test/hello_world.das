options log = true

require UnitTest

struct Foo
    a : array<int>
    b : string

[export, unsafe]
def test
    let sa <- [{int 1;2;3;4}]
    let faw <- [[Foo a := sa, b="a is " where $(var boo) { boo.b+=join(boo.a,"-"); } ]]
    assert(length(faw.a)==4)
    assert(faw.b=="a is 1-2-3-4")
    return true
