// options log=true,optimize=true,logOptimizationPasses=false

[export]
def test:bool
    let s:string = "hello, world!"
    assert(endswith(s, "world!"))
    assert(startswith(s, "hello"))
    assert(startswith(s, ""))
    assert(endswith(s, ""))
    assert(startswith(s, "a") == false)
    assert(endswith(s, "b") == false)
    return true
