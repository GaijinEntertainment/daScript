expect 30103    // invalid_argument_type

struct Foo
    a:int

def test(x:Foo&)
    debug(x)
