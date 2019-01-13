options log=true // , logStack=true, optimize=true

def verifyIdentity(mat,col,row)
    for y in range(0,row)
        for x in range(0,col)
            let t = x==y ? 1.0 : 0.0
            assert(mat[y][x]==t)

def verifySequence(mat,col,row)
    let t = 1.0
    for y in range(0,row)
        for x in range(0,col)
            assert(mat[y][x]==t++)

def test_4x4
    let t : float4x4
    t[0] = float4(1,2,3,4)
    t[1] = float4(5,6,7,8)
    t[2] = float4(9,10,11,12)
    t[3] = float4(13,14,15,16)
    assert("{t}"=="1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16")
    verifySequence(t,4,4)
    for y in range(0,4)
        for x in range(0,4)
            t[y][x] = x==y ? 1.0 : 0.0
    verifyIdentity(t,4,4)
    t.x = float4(1,2,3,4)
    t.y = float4(5,6,7,8)
    t.z = float4(9,10,11,12)
    t.w = float4(13,14,15,16)
    verifySequence(t,4,4)
    assert("{t}"=="1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16")
    identity(t)
    verifyIdentity(t,4,4)
    let ops = false
    try
        t[4] = float4(0)
    catch
        ops = true
    assert(ops)
    return true

def test_any(a:auto(mat))
    let t : mat
    let v : auto(vec) = t[0]
    let col = sizeof(type vec) / 4
    let row = sizeof(type mat) / col / 4
    print("testing float{row}x{col}\n")
    return true

[export]
def test
    test_4x4()
    let f4x4 : float3x4
    test_any(f4x4)
    return true
