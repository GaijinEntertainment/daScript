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
    assert(int("11") == 11)
    assert(int("-12") == -12)
    assert(uint("11") == 11U)
    assert(float("-12") == -12.)
    assert(float("1e1") == 10.)
    //try
    //    debug(int("11abc"), "=")
    //    assert(false, "can't convert non int string without exception")
    //catch
    //    print("can't convert non int string without exception")
    assert(to_int("11abc") == 11)
    assert(to_float("11.1abc") == 11.1)
    return true
