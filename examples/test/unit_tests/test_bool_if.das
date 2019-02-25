// options log=true, logInferPasses=true, optimize=false, logNodes=true, logStack=true

[export]
def test
    let a = false
    if a
        assert(false, "can't get here")
    return true

