struct Foo
    a,b : int

struct Bar
    c, d : int

[sideeffects]
def setA(obj,val)
    if typeinfo(has_field<a> obj)
        obj.a = val

let
    canQQ : bool const = false

[export]
def test
    let f : Foo
    let b : Bar
    setA(f,1)
    setA(b,2)
    if canQQ
        broken = badly  // this will be compiled out, so as long as there are no syntax errors...
    if 2 + 2 == 4
        static_assert(true,"nothing to look at")
    else
        badly = broken  // this will be compiled out, so as long as there are no syntax errors...
    return true

