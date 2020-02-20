[sideeffects]
def test_mat(m:float3x4)
    var t : float3x4 = m
    for x in range(0,3)
        for y in range(0,4)
            assert(t[y][x] == float(y*3 + x))
            assert(t[y][x] == m[y][x])

[export]
def test
    var m : float3x4
    for x in range(0,3)
        for y in range(0,4)
            m[y][x] = float(y*3 + x)
    test_mat(m)
    return true

