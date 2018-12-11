expect 30102:4    // invalid_return_type

struct Foo
    a:int

def test_boxed:Foo              // can't return boxed types
    assert(false)

def test_ref:int&               // can't return reference
    assert(false)

def test_container:array<int>   // can't return containers
    assert(false)

def test_array:int[10]          // can't return array
    assert(false)
