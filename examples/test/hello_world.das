options logNodes=true

[export]
def test
    let m : float4x4
    m[0] = float4(1,2,3,4)
    m[1].y = 6.
    print("m={m}\n")
    assert(m[1].y==6.)
    return true

