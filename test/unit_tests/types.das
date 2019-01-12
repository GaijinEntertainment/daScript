// options log=true

let
    g_var_i:int=1
    g_var_f:float=3.14
    g_var_s:string="blah"

def test_global
    assert(g_var_i==1)
    g_var_i = g_var_i + 1
    assert(g_var_i==2)
    assert(g_var_f==3.14)
    g_var_f = g_var_f * 2.0
    assert(g_var_f==6.28)
    assert(g_var_s=="blah")
    g_var_s += "_blah"
    assert(g_var_s == "blah_blah")

def test_local
    let l_var_i:int=1
    let l_var_f:float=3.14
    let l_var_s:string="blah"
    assert(l_var_i==1)
    l_var_i = l_var_i + 1
    assert(l_var_i==2)
    assert(l_var_f==3.14)
    l_var_f = l_var_f * 2.0
    assert(l_var_f==6.28)
    assert(l_var_s=="blah")
    l_var_s += "_blah"
    assert(l_var_s == "blah_blah")

def test_arg(a_var_i:int=1;a_var_f:float=3.14;a_var_s:string="blah")
    assert(a_var_i==1)
    a_var_i = a_var_i + 1
    assert(a_var_i==2)
    assert(a_var_f==3.14)
    a_var_f = a_var_f * 2.0
    assert(a_var_f==6.28)
    assert(a_var_s=="blah")
    a_var_s += "_blah"
    assert(a_var_s == "blah_blah")

def test_block_invoke(b:block<(b_var_i:int;b_var_f:float;b_var_s:string):void>)
    invoke(b,1,3.14,"blah")

def test_block
    test_block_invoke() <| $ (b_var_i:int;b_var_f:float;b_var_s:string)
        assert(b_var_i==1)
        b_var_i = b_var_i + 1
        assert(b_var_i==2)
        assert(b_var_f==3.14)
        b_var_f = b_var_f * 2.0
        assert(b_var_f==6.28)
        assert(b_var_s=="blah")
        b_var_s += "_blah"
        assert(b_var_s == "blah_blah")

[export]
def test:bool
    test_local()
    test_global()
    test_arg()
    return true
