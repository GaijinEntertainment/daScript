options log = true// , optimize = false // , logInferPasses = true, logOptimizationPasses = true

struct SomethingSmall
    a, b, c : int

def test_delete_array
    let a : array<int> [5]
    let w0 = heap_bytes_allocated()
    for t in range(0,5)
        resize(a[t],10)
    let w1 = heap_bytes_allocated()
    delete a
    let w2 = heap_bytes_allocated()
    assert(w1>w0 & w0==w2)
    for t in range(0,5)
        assert(length(a[t])==0 & capacity(a[t])==0)

def test_delete_table
    let a : table<int,int>
    let w0 = heap_bytes_allocated()
    a[0] = 1
    a[1] = 2
    let w1 = heap_bytes_allocated()
    delete a
    let w2 = heap_bytes_allocated()
    assert(w1>w0 & w0==w2)
    assert(length(a)==0 & capacity(a)==0)

def test_delete_struct
    let a : SomethingSmall?
    let w0 = heap_bytes_allocated()
    a = new SomethingSmall
    let w1 = heap_bytes_allocated()
    delete a
    let w2 = heap_bytes_allocated()
    assert(w1>w0 & w0==w2)
    assert(a==null)

def test_delete_struct_array
    let a : SomethingSmall? [10]
    let w0 = heap_bytes_allocated()
    for pA in a
        pA = new SomethingSmall
    let w1 = heap_bytes_allocated()
    delete a
    let w2 = heap_bytes_allocated()
    assert(w1>w0 & w0==w2)
    for pA in a
        assert(pA==null)

[export]
def test
    test_delete_struct()
    test_delete_struct_array()
    test_delete_array()
    test_delete_table()
    return true
