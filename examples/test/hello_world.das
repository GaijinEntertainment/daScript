options log = true// , optimize = false // , logInferPasses = true, logOptimizationPasses = true

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

[export]
def test
    test_delete_array()
    return true
