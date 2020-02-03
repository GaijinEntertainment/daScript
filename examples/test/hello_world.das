options log = true
options log_mem = true

var
    a = "hello"
    b = "hello"
    c = "world"

[export]
def test
    debug(a,"a=")
    debug(b,"b=")
    debug(c,"c=")
    return true
