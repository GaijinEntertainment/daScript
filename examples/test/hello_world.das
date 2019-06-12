// options logInferPasses = true
// options log = true

[export]
def test:bool
    let mls = "this is
multi line string"
    assert(mls=="this is\nmulti line string")
    return true


