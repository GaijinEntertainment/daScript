let
    g_t = 2

def ret_ref : auto &
    let qq : auto & = g_t
    return qq

def ret_safe_ref(a:auto&) : auto&
    return a

def test
    let x = ret_ref()
    let xx : int& = ret_safe_ref(x)
    assert(xx==2)
    let z : int & = ret_ref()
    assert(x==2 & z==2)
    ret_ref() = 3
    assert(x==2 & z==3)
    xx = 4
    assert(x==4 & z==3 & xx==4)
    return true
