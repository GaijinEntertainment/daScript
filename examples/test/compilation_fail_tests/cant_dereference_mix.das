expect 30501:6    // cant_dereference

struct Bar
    c:int

struct Foo
    a:int
    b:Bar?

def test
    let a:int          // can't dereference not a pointer
    debug(deref(a))
    let b:void?        // can't dereference void pointer
    debug(deref(b))
    let c:int
    debug(c ?? 0)   // can't dereference non-pointer
    let d:void?
    debug(d ?? 0)   // can't dereference void pointer
    let e:int?
    debug(e ?? 1.0) // default value type mismatch
    let f:Foo?
    let g:Foo const
    debug(f ?? g)   // const matters

