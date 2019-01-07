expect 30102:5    // invalid_return_type

struct Foo
    a:int

def test_boxed:Foo              // 30102: not a valid function return type
    let foo : Foo
    return foo

def test_ref:int&               // 30102: not a valid function return type
    let i = 0
    return i                    // 30102: incompatible return type, expecting int& vs int

def test_container:array<int>   // 30102: not a valid function return type
    let Ai:array<int>
    return Ai

def test_array:int[10]          // 30102: not a valid function return type
    let ai:int[10]
    return ai

