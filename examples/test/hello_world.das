options log = true // , printVarAccess=true, printRef=true, optimize=false

struct Ball
    center : float3
    radius : float = 1.234f

def Ball(center:float3; radius:float)
    let b : Ball
    b.center = center
    b.radius = radius
    return b

[export]
def test
    let b1 = new Ball()
    print("b1={b1}\n")
    let b2 = new Ball(float3(1,2,3),4.0f)
    print("b2={b2}\n")
    let b3 = new Ball[3] (float3(3,2,1),0.5f)
    print("b3={b3}\n")
    return true
