options log = true // , logInferPasses=true

[export]
def clone(a:das_string;b:string const)
    set(a,b)

[export]
def set_it(a:das_string)
    a := "hello"

[export]
def test
    return true
