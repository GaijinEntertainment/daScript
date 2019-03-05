// options log=true, logStack=true, optimize=true
options logNodes=true

def verifyIdentity(mat:auto(mat))
    let v : auto(vec) = mat[0]
    let col = sizeof(type vec) / 4
    let row = sizeof(type mat) / col / 4
    for y in range(0,row)
        for x in range(0,col)
            let t = x==y ? 1.0 : 0.0
            assert(mat[y][x]==t)

def verifySequence(mat:auto(mat))
    let v : auto(vec) = mat[0]
    let col = sizeof(type vec) / 4
    let row = sizeof(type mat) / col / 4
    let t = 1.0
    for y in range(0,row)
        for x in range(0,col)
            assert(mat[y][x]==t++)

def verifyRows(mat:auto(mat))
    let v : auto(vec) = mat[0]
    let col = sizeof(type vec) / 4
    let row = sizeof(type mat) / col / 4
    for y in range(0,row)
        for x in range(0,col)
            assert(mat[y][x]==float(y))

def test_4x4
    let t : float4x4
    t[0] = float4(1,2,3,4)
    t[1] = float4(5,6,7,8)
    t[2] = float4(9,10,11,12)
    t[3] = float4(13,14,15,16)
    assert("{t}"=="[[ 1,2,3,4; 5,6,7,8; 9,10,11,12; 13,14,15,16]]")
    verifySequence(t)
    for y in range(0,4)
        for x in range(0,4)
            t[y][x] = x==y ? 1.0 : 0.0
    verifyIdentity(t)
    t.x = float4(1,2,3,4)
    t.y = float4(5,6,7,8)
    t.z = float4(9,10,11,12)
    t.w = float4(13,14,15,16)
    verifySequence(t)
    assert("{t}"=="[[ 1,2,3,4; 5,6,7,8; 9,10,11,12; 13,14,15,16]]")
    identity(t)
    verifyIdentity(t)
    let ops = false
    try
        t[pass(4)] = float4(0)
    recover
        ops = true
    assert(ops)

[sideeffects]
def pass(a)
    return a

def test_any(t:auto(mat))
    let v : auto(vec) = t[0]
    let col = sizeof(type vec) / 4
    let row = sizeof(type mat) / col / 4
    identity(t)
    verifyIdentity(t)
    for r in range(0,row)
        t[r] = vec(r)
    verifyRows(t)
    for y in range(0,row)
        for x in range(0,col)
            t[y][x] = x==y ? 1.0 : 0.0
    verifyIdentity(t)
    let ops = false
    try
        t[pass(row)] = vec(0)   // the reason to use pass here is so that we don't get compile time error
    recover
        ops = true
    assert(ops)

[sideeffects]
def test_r2v
    let m : float4x4
    m[0] = float4(1,2,3,4)
    m[1].y = 6.
    m[2] = float4(5,6,7,8)
    m.w = float4(9,10,11,12)
    m.w.z = 111.
    assert(m[0]==float4(1,2,3,4))
    assert(m[1].y==6.)
    assert(m[1]==float4(0,6,0,0))
    assert(m[2]==float4(5,6,7,8))
    assert(m.w.z==111.)
    assert(m.w==float4(9,10,111,12))
    return true

[export]
def test
    test_r2v()
    test_4x4()
    test_any(float4x4())
    test_any(float3x4())
    return true

