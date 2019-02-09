options log = true // , printVarAccess=true, printRef=true, optimize=false

typedef point2 = float2

def point2(x,y)
    return float2(x,y)

[export]
def test
    let t : point2
    t = point2(1,2)
    print("t={t}\n")
    return true
