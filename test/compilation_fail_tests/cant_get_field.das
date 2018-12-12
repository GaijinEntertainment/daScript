expect 30503:2    // cant_get_field

struct Foo
    a:int

def test
    let(a:Foo;pa:Foo?;b:Foo[10];c:int)
        a.a = 0     // structure ok
        b.a = 0     // can't get field of array
        pa.a = 0    // pointer to structure ok
        c.a = 0     // can't get field of some random type


