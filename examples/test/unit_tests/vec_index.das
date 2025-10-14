options gen2
options no_unused_function_arguments = false

def test_vec2(a : auto(vec2) -const -&) {
    var f2_x  = vec2()
    let x : auto(numT) = f2_x[0]
    assert(f2_x[0] == numT(0) && f2_x[1] == numT(0))
    assert(f2_x?[0] ?? numT(-1) == numT(0) && f2_x?[1] ?? numT(-1) == numT(0))
    f2_x  = vec2(1, 2)
    assert("{f2_x}" == "1{VEC_SEP}2")
    assert(f2_x[0] == numT(1) && f2_x[1] == numT(2))
    assert(f2_x[0] == vec2(1, 2)[0] && f2_x[1] == vec2(1, 2)[1])
    f2_x[0] = numT(2)
    f2_x[1] = numT(3)
    assert(f2_x[0] == numT(2) && f2_x[1] == numT(3))
    assert(f2_x?[0] ?? numT(-1) == numT(2) && f2_x?[1] ?? numT(-1) == numT(3))
    var oor = false
    try {
        f2_x[-1] = numT(5)
    } recover {
        oor = true
    }
    assert(oor)
    oor = false
    try {
        f2_x[2] = numT(5)
    } recover {
        oor = true
    }
    assert(oor)

    var v2_vec : array<vec2> = [vec2(numT(1), numT(2)), vec2(numT(4), numT(5))]
    assert(v2_vec[0][0] == numT(1))
    assert(v2_vec[1][1] == numT(5))
}


def test_vec3(a : auto(vec3) -const -&) {
    var f3_x  = vec3()
    let x : auto(numT) = f3_x[0]
    assert(f3_x[0] == numT(0) && f3_x[1] == numT(0) && f3_x[2] == numT(0))
    assert(f3_x?[0] ?? numT(-1) == numT(0) && f3_x?[1] ?? numT(-1) == numT(0) && f3_x?[2] ?? numT(-1) == numT(0))
    f3_x  = vec3(1, 2, 3)
    assert("{f3_x}" == "1{VEC_SEP}2{VEC_SEP}3")
    assert(f3_x[0] == numT(1) && f3_x[1] == numT(2) && f3_x[2] == numT(3))
    assert(f3_x[0] == vec3(1, 2, 3)[0] && f3_x[1] == vec3(1, 2, 3)[1] && f3_x[2] == vec3(1, 2, 3)[2])
    f3_x[0] = numT(2)
    f3_x[1] = numT(3)
    f3_x[2] = numT(4)
    assert(f3_x[0] == numT(2) && f3_x[1] == numT(3) && f3_x[2] == numT(4))
    assert(f3_x?[0] ?? numT(-1) == numT(2) && f3_x?[1] ?? numT(-1) == numT(3) && f3_x?[2] ?? numT(-1) == numT(4))
    var oor = false
    try {
        f3_x[-1] = numT(5)
    } recover {
        oor = true
    }
    assert(oor)
    oor = false
    try {
        f3_x[3] = numT(5)
    } recover {
        oor = true
    }
    assert(oor)
}

def test_vec4(a : auto(vec4) -const -&) {
    var f4_x  = vec4()
    let x : auto(numT) = f4_x[0]
    assert(f4_x[0] == numT(0) && f4_x[1] == numT(0) && f4_x[2] == numT(0) && f4_x[3] == numT(0))
    assert(f4_x?[0] ?? numT(-1) == numT(0) && f4_x?[1] ?? numT(-1) == numT(0) && f4_x?[2] ?? numT(-1) == numT(0) && f4_x?[3] ?? numT(-1) == numT(0))
    f4_x  = vec4(1, 2, 3, 4)
    assert("{f4_x}" == "1{VEC_SEP}2{VEC_SEP}3{VEC_SEP}4")
    assert(f4_x[0] == numT(1) && f4_x[1] == numT(2) && f4_x[2] == numT(3) && f4_x[3] == numT(4))
    assert(f4_x[0] == vec4(1, 2, 3, 4)[0] && f4_x[1] == vec4(1, 2, 3, 4)[1] && f4_x[2] == vec4(1, 2, 3, 4)[2] && f4_x[3] == vec4(1, 2, 3, 4)[3])
    f4_x[0] = numT(2)
    f4_x[1] = numT(3)
    f4_x[2] = numT(4)
    f4_x[3] = numT(5)
    assert(f4_x[0] == numT(2) && f4_x[1] == numT(3) && f4_x[2] == numT(4) && f4_x[3] == numT(5))
    assert(f4_x?[0] ?? numT(-1) == numT(2) && f4_x?[1] ?? numT(-1) == numT(3) && f4_x?[2] ?? numT(-1) == numT(4) && f4_x?[3] ?? numT(-1) == numT(5))
    var oor = false
    try {
        f4_x[-1] = numT(5)
    } recover {
        oor = true
    }
    assert(oor)
    oor = false
    try {
        f4_x[4] = numT(5)
    } recover {
        oor = true
    }
    assert(oor)
}

[export]
def test {
    test_vec2(float2())
    test_vec3(float3())
    test_vec4(float4())
    test_vec2(int2())
    test_vec3(int3())
    test_vec4(int4())
    test_vec2(uint2())
    test_vec3(uint3())
    test_vec4(uint4())
    test_vec2(range())
    test_vec2(urange())
    return true
}

