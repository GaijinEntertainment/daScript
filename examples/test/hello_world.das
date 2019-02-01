options log=true

def strcat(a,b)
    return a + b

def make_name(x)
    return strcat("hello_",x)

[export]
def test
    let t = make_name("world")
    print("\n{t}\n")
    return true
