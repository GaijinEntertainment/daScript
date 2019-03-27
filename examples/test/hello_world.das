options log = true, optimize=false

struct Foo
    a:int
    b:float
    c:double
    e:Foo?
    // f:string

[export]
def test
    let f : Foo
    let x = typeinfo(typename f)
    let y = typeinfo(sizeof type Foo)
    let z = typeinfo(typename type Foo)
    let q = typeinfo(is_pod type Foo)
    let qq = typeinfo(is_raw type Foo)
    return true
