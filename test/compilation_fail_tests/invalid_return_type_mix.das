expect 30102:8    // invalid_return_type

struct Foo
    a:int

def test_boxed:Foo              // 30102: not a valid function return type
    let foo : Foo
    return foo                  // 30102: incompatible return type, expecting (Foo) vs (Foo&)

def test_ref:int&               // 30102: not a valid function return type
    let i = 0
    return i                    // 30102: incompatible return type, expecting int& vs int

def test_container:array<int>   // 30102: not a valid function return type
    let Ai:array<int>
    return Ai                   // 30102: incompatible return type, expecting (array<int>) vs (array<int>&)

def test_array:int[10]          // 30102: not a valid function return type
    let ai:int[10]
    return ai                   // 30102: incompatible return type, expecting (int[10]) vs (int[10]&)

