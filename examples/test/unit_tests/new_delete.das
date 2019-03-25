require UnitTest
// options logNodes=false

struct SomethingSmall
    a, b, c : int

[unsafe]
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

[unsafe]
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

[unsafe]
def test_delete_struct
    let a : SomethingSmall?
    let w0 = heap_bytes_allocated()
    a = new SomethingSmall
    let w1 = heap_bytes_allocated()
    delete a
    let w2 = heap_bytes_allocated()
    assert(w1>w0 & w0==w2)
    assert(a==null)

[unsafe]
def test_delete_struct_array
    let w0 = heap_bytes_allocated()
    let a = new SomethingSmall[10]
    let w1 = heap_bytes_allocated()
    delete a
    let w2 = heap_bytes_allocated()
    assert(w1>w0 & w0==w2)
    for pA in a
        assert(pA==null)

[unsafe]
def test_delete_handle
    let a : TestObjectFoo?
    a = new TestObjectFoo
    delete a
    assert(a==null)

[unsafe]
def test_delete_handle_array
    let a  = new TestObjectFoo[10]
    delete a
    for pA in a
        assert(pA==null)

[sideeffects,unsafe]
def da_inscope(x)
    let in scope a = new SomethingSmall[10]
    let in scope b = new SomethingSmall
    b.a = x
    for pA in a
        pA.a = x

def test_delete_struct_array_in_scope
    let w0 = heap_bytes_allocated()
    da_inscope(2)
    let w1 = heap_bytes_allocated()
    assert(w0==w1)

[sideeffects,unsafe]
def test_delete_string()
    let w0 = heap_bytes_allocated()
    let a = "{deref(getPtr())}"
    // assert(a=="0")
    delete a
    let w1 = heap_bytes_allocated()
    assert(w0==w1)
    assert(a=="")

[export]
def test
    test_delete_struct_array_in_scope()
    test_delete_handle()
    test_delete_handle_array()
    test_delete_struct()
    test_delete_struct_array()
    test_delete_array()
    test_delete_table()
    test_delete_string()
    return true
