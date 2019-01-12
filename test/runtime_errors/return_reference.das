// options log=true, logInferPasses=false, optimize=false

def ret_unsafe_ref : int &
    let t = 2
    return t

[export]
def test
    ret_unsafe_ref()
    return true
