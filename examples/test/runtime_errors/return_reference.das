// options log=true, log_infer_passes=false, optimize=false
// options log = true, log_optimization_passes=true

[sideeffects, unsafe]
def ret_unsafe_ref : int &
    var t = 2
    return t

[export]
def test
    ret_unsafe_ref()
    return true
