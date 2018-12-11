expect 30102:3    // invalid_return_type

struct Foo
    a:int

def test_boxed:Foo              // can't return boxed types
    assert(false)

def test_ref:int&               // can't return reference
    assert(false)

def test_container:array<int>   // can't return containers
    assert(false)
