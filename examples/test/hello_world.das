options log = true, logInferPasses=true

struct Foo
    a : int
    b : float

[sideeffects]
def test_clone_array
    let x, y : array<int>
    resize(x,3)
    for t in range(0,3)
        x[t] = t
    y := x
    for t in range(0,3)
        assert(y[t]==t)

[sideeffects]
def test_clone_table
    let x, y : table<int,string>
    for t in range(0,3)
        x[t] = string(t)
    y := x
    assert(length(y)==3)
    for t in range(0,3)
        assert(y[t]==string(t))

[export]
def test
    test_clone_array()
    test_clone_table()
    return true
