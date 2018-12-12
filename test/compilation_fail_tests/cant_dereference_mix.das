expect 30501:6    // cant_dereference

struct Foo
    a:int

def test
    let(a:int)          // can't dereference not a pointer
        debug(a->)
    let(a:void?)        // can't dereference void pointer
        debug(a->)
    let(a:int)
        debug(a ?? 0)   // can't dereference non-pointer
    let(a:void?)
        debug(a ?? 0)   // can't dereference void pointer
    let(a:int?)
        debug(a ?? 1.0) // default value type mismatch
    let(a:Foo?;b:const Foo)
        debug(a ?? b)   // const matters

