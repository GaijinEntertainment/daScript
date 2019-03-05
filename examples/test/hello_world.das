options logNodes=true

[export]
def test
    let m : float4x4
    m[0] = float4(1,2,3,4)
    m[1].y = 6.
    m[2] = float4(5,6,7,8)
    assert(m[0]==float4(1,2,3,4))
    assert(m[1].y==6.)
    assert(m[2]==float4(5,6,7,8))
    return true

