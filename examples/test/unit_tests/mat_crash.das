// options log=true, logStack=true, optimize=true
[export]
def test
    let a,b : float3x4
    identity(a)
    identity(b)
    a = a*b
    return true

