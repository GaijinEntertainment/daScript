options log = true, printVarAccess=true, printRef=true, optimize=false

def ch(a,b:int&)
    a = b

[export]
def test
    let x,y = 5
    ch(x,y)
    return true
