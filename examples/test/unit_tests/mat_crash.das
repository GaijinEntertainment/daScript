// options log=true, logStack=true, logNodes=true, optimize=true
def get_dimensions():int2
    return int2(1280, 720)

[export]
def test
    let a,b : float3x4
    identity(a)
    identity(b)
    a = a*b
    return true

