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
    let s2:string = "   hello, world!   "
    assert(strip(s2) == s)
    assert(strip_right(strip_left(s2)) == s)
    assert(strip_left(s2) == "hello, world!   ")
    assert(strip_right(s2) == "   hello, world!")
    assert(slice(s, 7) == "world!")
    assert(slice(s, -6) == "world!")
    assert(slice(s, 6, -6) == " ")
    assert(slice(s, 6, 7) == " ")
    return true
