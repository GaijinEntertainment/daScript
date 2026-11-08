options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false
require dastest/testing_boost public
require math
require daslib/random
require daslib/fuzzer
require daslib/faker

struct Foo {
    t : bool
    v : int3 = int3(1, 2, 3)
}

def funV(v : int3) : int3 => v

def funU(v : uint4) : uint4 => v

def test_vec2_swizzle(tt : T?; a : auto(vec2) -const -&) {
    var f2_x = vec2(1, 2)
    let _x : auto(numT) = f2_x[0]
    tt |> equal("{f2_x}", "1{VEC_SEP}2")
    var f2_y = f2_x.yx
    tt |> equal("{f2_y}", "2{VEC_SEP}1")
    f2_x.x = numT(3)
    f2_x.y = numT(4)
    tt |> equal("{f2_x}", "3{VEC_SEP}4")
    f2_x.xy = vec2(5, 6)
    tt |> equal("{f2_x}", "5{VEC_SEP}6")
    f2_y = f2_x.xy
    tt |> equal("{f2_y}", "5{VEC_SEP}6")
}

def test_vec3_swizzle(tt : T?; a : auto(vec3) -const -&) {
    var f3_x = vec3(1, 2, 3)
    let _x : auto(numT) = f3_x[0]
    let _y : auto(vec2) = f3_x.xy
    tt |> equal("{f3_x}", "1{VEC_SEP}2{VEC_SEP}3")
    var f3_y = f3_x.zyx
    tt |> equal("{f3_y}", "3{VEC_SEP}2{VEC_SEP}1")
    f3_x.x = numT(3)
    f3_x.y = numT(4)
    f3_x.z = numT(5)
    tt |> equal("{f3_x}", "3{VEC_SEP}4{VEC_SEP}5")
    f3_x.xyz = vec3(5, 6, 7)
    tt |> equal("{f3_x}", "5{VEC_SEP}6{VEC_SEP}7")
    f3_y = f3_x.xyz
    tt |> equal("{f3_y}", "5{VEC_SEP}6{VEC_SEP}7")
    f3_x.xy = vec2(8, 9)
    tt |> equal("{f3_x}", "8{VEC_SEP}9{VEC_SEP}7")
    f3_x.yz = vec2(1, 2)
    tt |> equal("{f3_x}", "8{VEC_SEP}1{VEC_SEP}2")
    tt |> equal(f3_x.zx, vec2(2, 8))
}

def test_vec4_swizzle(tt : T?; a : auto(vec4) -const -&) {
    var f4_x = vec4(1, 2, 3, 4)
    let _x : auto(numT) = f4_x[0]
    let _y : auto(vec2) = f4_x.xy
    let _z : auto(vec3) = f4_x.xyz
    tt |> equal("{f4_x}", "1{VEC_SEP}2{VEC_SEP}3{VEC_SEP}4")
    var f4_y = f4_x.wzyx
    tt |> equal("{f4_y}", "4{VEC_SEP}3{VEC_SEP}2{VEC_SEP}1")
    f4_x.x = numT(3)
    f4_x.y = numT(4)
    f4_x.z = numT(5)
    f4_x.w = numT(6)
    tt |> equal("{f4_x}", "3{VEC_SEP}4{VEC_SEP}5{VEC_SEP}6")
    f4_x.xyzw = vec4(5, 6, 7, 8)
    tt |> equal("{f4_x}", "5{VEC_SEP}6{VEC_SEP}7{VEC_SEP}8")
    f4_y = f4_x.xyzw
    tt |> equal("{f4_y}", "5{VEC_SEP}6{VEC_SEP}7{VEC_SEP}8")
    f4_x.xyz = vec3(8, 9, 0)
    tt |> equal("{f4_x}", "8{VEC_SEP}9{VEC_SEP}0{VEC_SEP}8")
    f4_x.yz = vec2(1, 2)
    tt |> equal("{f4_x}", "8{VEC_SEP}1{VEC_SEP}2{VEC_SEP}8")
    tt |> equal(f4_x.zx, vec2(2, 8))
}

[test(description = "swizzles (.xy, .yx, .xyz, .wzyx and the rest) read and write the right components of float, int and uint vectors, also through struct fields")]
def test_vec_swizzle(t : T?) {
    t |> run("float2/3/4 swizzle") @(t : T?) {
        test_vec2_swizzle(t, float2())
        test_vec3_swizzle(t, float3())
        test_vec4_swizzle(t, float4())
    }
    t |> run("int2/3/4 swizzle") @(t : T?) {
        test_vec2_swizzle(t, int2())
        test_vec3_swizzle(t, int3())
        test_vec4_swizzle(t, int4())
    }
    t |> run("uint2/3/4 swizzle") @(t : T?) {
        test_vec2_swizzle(t, uint2())
        test_vec3_swizzle(t, uint3())
        test_vec4_swizzle(t, uint4())
    }
    t |> run("int3 struct member swizzle") @(t : T?) {
        // regular variable
        var v = int3(1, 2, 3)
        t |> equal(v, int3(1, 2, 3))
        t |> equal(v.x, 1)
        t |> equal(v.y, 2)
        t |> equal(v.z, 3)
        t |> equal(v.xy, int2(1, 2))
        t |> equal(v.yz, int2(2, 3))
        t |> equal(v.zx, int2(3, 1))
        t |> equal(v.yx, int2(2, 1))
        t |> equal(v.zy, int2(3, 2))
        t |> equal(v.xz, int2(1, 3))
        t |> equal(funV(v), int3(1, 2, 3))
        t |> equal(funV(v).x, 1)
        v.x = 11
        t |> equal(v, int3(11, 2, 3))
        v.y = 22
        t |> equal(v, int3(11, 22, 3))
        v.z = 33
        t |> equal(v, int3(11, 22, 33))
        v.xy = int2(1, 2)
        t |> equal(v.x, 1)
        t |> equal(v.y, 2)
        t |> equal(v.z, 33)
        v.yz = int2(2, 3)
        t |> equal(v.z, 3)
        v.xyz = int3(11, 22, 33)
        t |> equal(v.xyz, int3(11, 22, 33))
    }
    t |> run("struct field swizzle") @(t : T?) {
        var fv = Foo()
        t |> equal(fv.v, int3(1, 2, 3))
        t |> equal(fv.v.x, 1)
        t |> equal(fv.v.y, 2)
        t |> equal(fv.v.z, 3)
        t |> equal(fv.v.xy, int2(1, 2))
        t |> equal(fv.v.yz, int2(2, 3))
        t |> equal(fv.v.zx, int2(3, 1))
        t |> equal(fv.v.yx, int2(2, 1))
        t |> equal(funV(fv.v), int3(1, 2, 3))
        t |> equal(funV(fv.v).x, 1)
        fv.v.x = 11
        t |> equal(fv.v, int3(11, 2, 3))
        fv.v.y = 22
        t |> equal(fv.v, int3(11, 22, 3))
        fv.v.z = 33
        t |> equal(fv.v, int3(11, 22, 33))
        fv.v.xy = int2(1, 2)
        t |> equal(fv.v.x, 1)
        t |> equal(fv.v.y, 2)
        t |> equal(fv.v.z, 33)
        fv.v.yz = int2(2, 3)
        t |> equal(fv.v.z, 3)
        fv.v.xyz = int3(11, 22, 33)
        t |> equal(fv.v.xyz, int3(11, 22, 33))
    }
}

[test(description = "a uint component of a returned uint4 stays unsigned through widening, shift, division and printing")]
def test_uint_component_of_value(t : T?) {
    let u = uint4(0x80000000u, 1u, 2u, 0xfffffffeu)
    t |> equal(uint64(funU(u).x), 0x80000000ul)
    t |> equal(funU(u).x >> 1u, 0x40000000u)
    t |> equal(funU(u).w / 2u, 0x7fffffffu)
    t |> equal("{funU(u).x}", "0x80000000")
}

// VEC_SEP is a built-in constant (default ",") used in vector
// printing. globals, so the constant folder leaves the conversion
// chains below to codegen
var g_f2 = float2(1, 2)
var g_f3 = float3(1, 2, 3)
var g_f4 = float4(1, 2, 3, 4)
var g_i3 = int3(1, 2, 3)

def test_vec2(tt : T?; a : auto(vec2) -const -&; isRange = false) {
    let f2_0  = vec2()
    let f2_x  = vec2(1.0, 2.0)
    let f2_y  = vec2(1, 2)
    let f2_z  = vec2(0x1, 0x2)
    let f2_w  = vec2(1u, 2u)
    let f2_xx = vec2(1.0)
    let f2_yy = vec2(1)
    let f2_zz = vec2(0x1)
    let f2_ww = vec2(1u)
    tt |> equal("{f2_0}", "0{VEC_SEP}0")
    tt |> equal("{f2_x}", "1{VEC_SEP}2")
    tt |> equal("{f2_y}", "1{VEC_SEP}2")
    tt |> equal("{f2_z}", "1{VEC_SEP}2")
    tt |> equal("{f2_w}", "1{VEC_SEP}2")
    if (isRange) {
        tt |> equal("{f2_xx}", "0{VEC_SEP}1")
        tt |> equal("{f2_yy}", "0{VEC_SEP}1")
        tt |> equal("{f2_zz}", "0{VEC_SEP}1")
        tt |> equal("{f2_ww}", "0{VEC_SEP}1")
    } else {
        tt |> equal("{f2_xx}", "1{VEC_SEP}1")
        tt |> equal("{f2_yy}", "1{VEC_SEP}1")
        tt |> equal("{f2_zz}", "1{VEC_SEP}1")
        tt |> equal("{f2_ww}", "1{VEC_SEP}1")
    }
}

def test_vec3(tt : T?; a : auto(vec3) -const -&) {
    let f3_0  = vec3()
    let f3_x  = vec3(1.0, 2.0, 3.0)
    let f3_y  = vec3(1, 2, 3)
    let f3_z  = vec3(0x1, 0x2, 0x3)
    let f3_w  = vec3(1u, 2u, 3u)
    let f3_xx = vec3(1.0)
    let f3_yy = vec3(1)
    let f3_zz = vec3(0x1)
    let f3_ww = vec3(1u)
    tt |> equal("{f3_0}", "0{VEC_SEP}0{VEC_SEP}0")
    tt |> equal("{f3_x}", "1{VEC_SEP}2{VEC_SEP}3")
    tt |> equal("{f3_y}", "1{VEC_SEP}2{VEC_SEP}3")
    tt |> equal("{f3_z}", "1{VEC_SEP}2{VEC_SEP}3")
    tt |> equal("{f3_w}", "1{VEC_SEP}2{VEC_SEP}3")
    tt |> equal("{f3_xx}", "1{VEC_SEP}1{VEC_SEP}1")
    tt |> equal("{f3_yy}", "1{VEC_SEP}1{VEC_SEP}1")
    tt |> equal("{f3_zz}", "1{VEC_SEP}1{VEC_SEP}1")
    tt |> equal("{f3_ww}", "1{VEC_SEP}1{VEC_SEP}1")
}

def test_vec4(tt : T?; a : auto(vec4) -const -&) {
    let f4_0  = vec4()
    let f4_x  = vec4(1.0, 2.0, 3.0, 4.0)
    let f4_y  = vec4(1, 2, 3, 4)
    let f4_z  = vec4(0x1, 0x2, 0x3, 0x4)
    let f4_w  = vec4(1u, 2u, 3u, 4u)
    let f4_xx = vec4(1.0)
    let f4_yy = vec4(1)
    let f4_zz = vec4(0x1)
    let f4_ww = vec4(1u)
    tt |> equal("{f4_0}", "0{VEC_SEP}0{VEC_SEP}0{VEC_SEP}0")
    tt |> equal("{f4_x}", "1{VEC_SEP}2{VEC_SEP}3{VEC_SEP}4")
    tt |> equal("{f4_y}", "1{VEC_SEP}2{VEC_SEP}3{VEC_SEP}4")
    tt |> equal("{f4_z}", "1{VEC_SEP}2{VEC_SEP}3{VEC_SEP}4")
    tt |> equal("{f4_w}", "1{VEC_SEP}2{VEC_SEP}3{VEC_SEP}4")
    tt |> equal("{f4_xx}", "1{VEC_SEP}1{VEC_SEP}1{VEC_SEP}1")
    tt |> equal("{f4_yy}", "1{VEC_SEP}1{VEC_SEP}1{VEC_SEP}1")
    tt |> equal("{f4_zz}", "1{VEC_SEP}1{VEC_SEP}1{VEC_SEP}1")
    tt |> equal("{f4_ww}", "1{VEC_SEP}1{VEC_SEP}1{VEC_SEP}1")
}

[test(description = "float, int, uint and range vector constructors build from components or one splatted scalar, and vector conversions, chained too, keep the component values")]
def test_vec_constructors(t : T?) {
    t |> run("float2/3/4 constructors") @(t : T?) {
        test_vec2(t, float2())
        test_vec3(t, float3())
        test_vec4(t, float4())
    }
    t |> run("int2/3/4 constructors") @(t : T?) {
        test_vec2(t, int2())
        test_vec3(t, int3())
        test_vec4(t, int4())
    }
    t |> run("uint2/3/4 constructors") @(t : T?) {
        test_vec2(t, uint2())
        test_vec3(t, uint3())
        test_vec4(t, uint4())
    }
    t |> run("range constructors") @(t : T?) {
        test_vec2(t, range(), true)
        test_vec2(t, urange(), true)
        test_vec2(t, range64(), true)
        test_vec2(t, urange64(), true)
    }
    t |> run("type conversions") @(t : T?) {
        // float <- int
        t |> equal(float2(int2(1, 2)), float2(1, 2))
        t |> equal(float3(int3(1, 2, 3)), float3(1, 2, 3))
        t |> equal(float4(int4(1, 2, 3, 4)), float4(1, 2, 3, 4))
        // float <- uint
        t |> equal(float2(uint2(1u, 2u)), float2(1, 2))
        t |> equal(float3(uint3(1u, 2u, 3u)), float3(1, 2, 3))
        t |> equal(float4(uint4(1u, 2u, 3u, 4u)), float4(1, 2, 3, 4))
        // int <- float
        t |> equal(int2(float2(1, 2)), int2(1, 2))
        t |> equal(int3(float3(1, 2, 3)), int3(1, 2, 3))
        t |> equal(int4(float4(1, 2, 3, 4)), int4(1, 2, 3, 4))
        // uint <- float
        t |> equal(uint2(float2(1, 2)), uint2(1u, 2u))
        t |> equal(uint3(float3(1, 2, 3)), uint3(1u, 2u, 3u))
        t |> equal(uint4(float4(1, 2, 3, 4)), uint4(1u, 2u, 3u, 4u))
    }
    t |> run("chained type conversions") @(t : T?) {
        // AOT writes these as cvt_pass(cvt_uint3(..)), which resolves only
        // while each cvt_ answers in its vector type - a vec4f argument
        // matches every cvt_pass overload
        t |> equal(g_f2 |> uint2 |> int2, int2(1, 2))
        t |> equal(g_f3 |> uint3 |> int3, int3(1, 2, 3))
        t |> equal(g_f4 |> uint4 |> int4, int4(1, 2, 3, 4))
        t |> equal(g_f2 |> int2 |> uint2, uint2(1u, 2u))
        t |> equal(g_f3 |> int3 |> uint3, uint3(1u, 2u, 3u))
        t |> equal(g_f4 |> int4 |> uint4, uint4(1u, 2u, 3u, 4u))
        t |> equal(g_i3 |> float3 |> int3, int3(1, 2, 3))
        t |> equal(g_i3 |> uint3 |> float3, float3(1, 2, 3))
        // three links: cvt_pass(cvt_pass(cvt_uint3(..))), so a cvt_pass feeds
        // a cvt_pass
        t |> equal(g_f3 |> uint3 |> int3 |> uint3, uint3(1u, 2u, 3u))
        t |> equal(g_f2 |> int2 |> uint2 |> int2, int2(1, 2))
    }
}

def ix_vec2(tt : T?; a : auto(vec2) -const -&) {
    var f2_x = vec2()
    let _x : auto(numT) = f2_x[0]
    tt |> equal(f2_x[0], numT(0))
    tt |> equal(f2_x[1], numT(0))
    tt |> equal(f2_x?[0] ?? numT(-1), numT(0))
    tt |> equal(f2_x?[1] ?? numT(-1), numT(0))
    f2_x = vec2(1, 2)
    tt |> equal("{f2_x}", "1{VEC_SEP}2")
    tt |> equal(f2_x[0], numT(1))
    tt |> equal(f2_x[1], numT(2))
    tt |> equal(f2_x[0], vec2(1, 2)[0])
    tt |> equal(f2_x[1], vec2(1, 2)[1])
    f2_x[0] = numT(2)
    f2_x[1] = numT(3)
    tt |> equal(f2_x[0], numT(2))
    tt |> equal(f2_x[1], numT(3))
    tt |> equal(f2_x?[0] ?? numT(-1), numT(2))
    tt |> equal(f2_x?[1] ?? numT(-1), numT(3))
    // out-of-range checks
    var oor = false
    try {
        f2_x[-1] = numT(5)
    } recover {
        oor = true
    }
    tt |> success(oor)
    oor = false
    try {
        f2_x[2] = numT(5)
    } recover {
        oor = true
    }
    tt |> success(oor)
    // array of vectors
    let v2_vec : array<vec2> = [vec2(numT(1), numT(2)), vec2(numT(4), numT(5))]
    tt |> equal(v2_vec[0][0], numT(1))
    tt |> equal(v2_vec[1][1], numT(5))
}

def ix_vec3(tt : T?; a : auto(vec3) -const -&) {
    var f3_x = vec3()
    let _x : auto(numT) = f3_x[0]
    tt |> equal(f3_x[0], numT(0))
    tt |> equal(f3_x[1], numT(0))
    tt |> equal(f3_x[2], numT(0))
    tt |> equal(f3_x?[0] ?? numT(-1), numT(0))
    tt |> equal(f3_x?[1] ?? numT(-1), numT(0))
    tt |> equal(f3_x?[2] ?? numT(-1), numT(0))
    f3_x = vec3(1, 2, 3)
    tt |> equal("{f3_x}", "1{VEC_SEP}2{VEC_SEP}3")
    tt |> equal(f3_x[0], numT(1))
    tt |> equal(f3_x[1], numT(2))
    tt |> equal(f3_x[2], numT(3))
    tt |> equal(f3_x[0], vec3(1, 2, 3)[0])
    tt |> equal(f3_x[1], vec3(1, 2, 3)[1])
    tt |> equal(f3_x[2], vec3(1, 2, 3)[2])
    f3_x[0] = numT(2)
    f3_x[1] = numT(3)
    f3_x[2] = numT(4)
    tt |> equal(f3_x[0], numT(2))
    tt |> equal(f3_x[1], numT(3))
    tt |> equal(f3_x[2], numT(4))
    tt |> equal(f3_x?[0] ?? numT(-1), numT(2))
    tt |> equal(f3_x?[1] ?? numT(-1), numT(3))
    tt |> equal(f3_x?[2] ?? numT(-1), numT(4))
    var oor = false
    try {
        f3_x[-1] = numT(5)
    } recover {
        oor = true
    }
    tt |> success(oor)
    oor = false
    try {
        f3_x[3] = numT(5)
    } recover {
        oor = true
    }
    tt |> success(oor)
}

def ix_vec4(tt : T?; a : auto(vec4) -const -&) {
    var f4_x = vec4()
    let _x : auto(numT) = f4_x[0]
    tt |> equal(f4_x[0], numT(0))
    tt |> equal(f4_x[1], numT(0))
    tt |> equal(f4_x[2], numT(0))
    tt |> equal(f4_x[3], numT(0))
    tt |> equal(f4_x?[0] ?? numT(-1), numT(0))
    tt |> equal(f4_x?[1] ?? numT(-1), numT(0))
    tt |> equal(f4_x?[2] ?? numT(-1), numT(0))
    tt |> equal(f4_x?[3] ?? numT(-1), numT(0))
    f4_x = vec4(1, 2, 3, 4)
    tt |> equal("{f4_x}", "1{VEC_SEP}2{VEC_SEP}3{VEC_SEP}4")
    tt |> equal(f4_x[0], numT(1))
    tt |> equal(f4_x[1], numT(2))
    tt |> equal(f4_x[2], numT(3))
    tt |> equal(f4_x[3], numT(4))
    tt |> equal(f4_x[0], vec4(1, 2, 3, 4)[0])
    tt |> equal(f4_x[1], vec4(1, 2, 3, 4)[1])
    tt |> equal(f4_x[2], vec4(1, 2, 3, 4)[2])
    tt |> equal(f4_x[3], vec4(1, 2, 3, 4)[3])
    f4_x[0] = numT(2)
    f4_x[1] = numT(3)
    f4_x[2] = numT(4)
    f4_x[3] = numT(5)
    tt |> equal(f4_x[0], numT(2))
    tt |> equal(f4_x[1], numT(3))
    tt |> equal(f4_x[2], numT(4))
    tt |> equal(f4_x[3], numT(5))
    tt |> equal(f4_x?[0] ?? numT(-1), numT(2))
    tt |> equal(f4_x?[1] ?? numT(-1), numT(3))
    tt |> equal(f4_x?[2] ?? numT(-1), numT(4))
    tt |> equal(f4_x?[3] ?? numT(-1), numT(5))
    var oor = false
    try {
        f4_x[-1] = numT(5)
    } recover {
        oor = true
    }
    tt |> success(oor)
    oor = false
    try {
        f4_x[4] = numT(5)
    } recover {
        oor = true
    }
    tt |> success(oor)
}

[test(description = "vector [] and ?[] read and write each component, an out-of-range index on either side panics, and arrays of vectors index through")]
def test_vec_index(t : T?) {
    t |> run("float2/3/4 indexing") @(t : T?) {
        ix_vec2(t, float2())
        ix_vec3(t, float3())
        ix_vec4(t, float4())
    }
    t |> run("int2/3/4 indexing") @(t : T?) {
        ix_vec2(t, int2())
        ix_vec3(t, int3())
        ix_vec4(t, int4())
    }
    t |> run("uint2/3/4 indexing") @(t : T?) {
        ix_vec2(t, uint2())
        ix_vec3(t, uint3())
        ix_vec4(t, uint4())
    }
    t |> run("range indexing") @(t : T?) {
        ix_vec2(t, range())
        ix_vec2(t, urange())
    }
}

def test_vec2_ops(tt : T?; a : auto(vec2) -const -&) {
    let _x : auto(numT) = a[0]
    var mut_a_mul = a
    var mut_a_div = a
    let c = numT(2)
    mut_a_mul *= c
    mut_a_div /= c
    tt |> equal(mut_a_mul, vec2(a[0] * c, a[1] * c))
    tt |> equal(mut_a_div, vec2(a[0] / c, a[1] / c))
}

def test_vec3_ops(tt : T?; a : auto(vec3) -const -&) {
    let _x : auto(numT) = a[0]
    var mut_a_mul = a
    var mut_a_div = a
    let c = numT(2)
    mut_a_mul *= c
    mut_a_div /= c
    tt |> equal(mut_a_mul, vec3(a[0] * c, a[1] * c, a[2] * c))
    tt |> equal(mut_a_div, vec3(a[0] / c, a[1] / c, a[2] / c))
}

def test_vec4_ops(tt : T?; a : auto(vec4) -const -&) {
    let _x : auto(numT) = a[0]
    var mut_a_mul = a
    var mut_a_div = a
    let c = numT(2)
    mut_a_mul *= c
    mut_a_div /= c
    tt |> equal(mut_a_mul, vec4(a[0] * c, a[1] * c, a[2] * c, a[3] * c))
    tt |> equal(mut_a_div, vec4(a[0] / c, a[1] / c, a[2] / c, a[3] / c))
}

[test(description = "vector *= and /= by a scalar match the component-wise scalar result for float, int and uint vectors (skipped on a fast-math host)")]
def test_vec_ops(t : T?) {
    if (HOST_FAST_MATH) {
        t |> skip("host runtime is built -ffast-math: vector mul-div becomes a reciprocal multiply, not bit-equal to the scalar reference")
        return
    }
    t |> run("float2/3/4 mul-div") @(t : T?) {
        test_vec2_ops(t, float2(3.15, 2.14))
        test_vec3_ops(t, float3(3.15, 2.14, 9.32))
        test_vec4_ops(t, float4(3.15, 2.14, 9.32, 1.23))
    }
    t |> run("int2/3/4 mul-div") @(t : T?) {
        test_vec2_ops(t, int2(6, -5))
        test_vec3_ops(t, int3(6, -5, 8))
        test_vec4_ops(t, int4(6, -9, 13, -12))
    }
    t |> run("uint2/3/4 mul-div") @(t : T?) {
        test_vec2_ops(t, uint2(15, 20))
        test_vec3_ops(t, uint3(15, 20, 25))
        test_vec4_ops(t, uint4(15, 20, 25, 30))
    }
}

var g3 : float3

[test(description = "vector pow keeps a negative base's sign for odd exponents, while pow_est (GLSL pow) drops it")]
def test_vector_pow_keeps_sign(t : T?) {
    g3 = float3(-2.0, 2.0, -3.0)
    let odd = pow(g3, float3(3.0))
    t |> equal(odd.x, -8.0, "pow(-2, 3) is -8")
    t |> equal(odd.y, 8.0, "a positive base is untouched")
    t |> equal(pow(g3, float3(4.0)).x, 16.0, "an even exponent leaves the sign alone")
    t |> success(pow(g3, float3(-3.0)).x < 0.0, "a negative odd exponent flips too")
    t |> equal(pow(g3, float3(0.0)).x, 1.0, "pow(x, 0) is 1")
    t |> equal(pow_est(g3, float3(3.0)).x, 8.0, "pow_est drops the sign - it is GLSL pow")
}

// values come from heap arrays so the element reads are not
// const-folded and hmin/hmax/hadd run through real interpreter, AOT
// and JIT codegen
[test(description = "hmin, hmax and hadd reduce float2, float3 and float4 vectors read from the heap to the right scalar")]
def test_horizontal_reduce(t : T?) {
    let v4 <- [float4(3.0, 1.0, 4.0, 1.5), float4(-2.0, 5.0, -7.0, 0.0)]
    let v3 <- [float3(3.0, 1.0, 4.0)]
    let v2 <- [float2(3.0, 1.0)]

    t |> equal(hmin(v4[0]), 1.0)
    t |> equal(hmax(v4[0]), 4.0)
    t |> equal(hadd(v4[0]), 9.5)
    t |> equal(hmin(v4[1]), -7.0)
    t |> equal(hmax(v4[1]), 5.0)
    t |> equal(hadd(v4[1]), -4.0)

    t |> equal(hmin(v3[0]), 1.0)
    t |> equal(hmax(v3[0]), 4.0)
    t |> equal(hadd(v3[0]), 8.0)

    t |> equal(hmin(v2[0]), 1.0)
    t |> equal(hmax(v2[0]), 3.0)
    t |> equal(hadd(v2[0]), 4.0)
}

[test(description = "clz, ctz and popcnt agree with each other on every single-bit 32 and 64-bit value, and popcnt matches a bit-clearing count on fuzzed values")]
def test_bit_counting_everything(t : T?) {
    t |> run("clz,ctz 32") @(t : T?) {
        for (i in urange(32)) {
            let bits = 1u << i
            let lz = clz(bits)
            let tz = ctz(bits)
            t |> equal(31u, lz + tz)
            t |> equal(bits, 1u << tz)
            t |> equal(bits, 0x80000000u >> lz)
            t |> equal(1u, popcnt(bits))
        }
    }
    t |> run("clz,ctz 64") @(t : T?) {
        for (i in urange(64)) {
            let bits = 1ul << uint64(i)
            let lz = clz(bits)
            let tz = ctz(bits)
            t |> equal(63ul, lz + tz)
            t |> equal(bits, 1ul << tz)
            t |> equal(bits, 0x8000000000000000ul >> lz)
            t |> equal(1ul, popcnt(bits))
        }
    }
    t |> run("popcnt") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            let x = fake |> random_uint()
            var bits = x
            var count = 0u
            while (bits != 0u) {
                bits &= bits - 1u
                count ++
            }
            t |> equal(count, popcnt(x))
        }
    }
    t |> run("popcnt64") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            let x = fake |> random_uint64()
            var bits = x
            var count = 0ul
            while (bits != 0ul) {
                bits &= bits - 1ul
                count ++
            }
            t |> equal(count, popcnt(x))
        }
    }
}

[test(description = "random_seed makes random_int reproducible for one seed and different for two")]
def test_random_seeding(t : T?) {
    t |> run("same seed produces same sequence") @(t : T?) {
        var s1 = random_seed(12345)
        var s2 = random_seed(12345)
        let a1 = random_int(s1)
        let a2 = random_int(s2)
        t |> equal(a1, a2)
        let b1 = random_int(s1)
        let b2 = random_int(s2)
        t |> equal(b1, b2)
    }
    t |> run("different seeds produce different sequences") @(t : T?) {
        var s1 = random_seed(1)
        var s2 = random_seed(999)
        let a1 = random_int(s1)
        let a2 = random_int(s2)
        t |> equal(a1 != a2, true)
    }
}

[test(description = "random_float stays in [0, 1)")]
def test_random_float_range(t : T?) {
    t |> run("random_float is in [0,1)") @(t : T?) {
        var seed = random_seed(42)
        for (_i in range(100)) {
            let f = random_float(seed)
            t |> equal(f >= 0.0, true)
            t |> equal(f < 1.0, true)
        }
    }
}

[test(description = "random_unit_vector returns vectors of length 1")]
def test_random_unit_vector(t : T?) {
    t |> run("unit vector has length ~1") @(t : T?) {
        var seed = random_seed(10)
        for (_i in range(50)) {
            let v = random_unit_vector(seed)
            let len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z)
            t |> equal(abs(len - 1.0) < 0.001, true)
        }
    }
}

[test(description = "random_in_unit_sphere returns points no farther than 1 from the origin")]
def test_random_in_unit_sphere(t : T?) {
    t |> run("point in unit sphere has length <= 1") @(t : T?) {
        var seed = random_seed(30)
        for (_i in range(50)) {
            let v = random_in_unit_sphere(seed)
            let len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z)
            t |> equal(len <= 1.0, true)
        }
    }
}

[test(description = "random_in_unit_disk returns points in the unit disk with z = 0")]
def test_random_in_unit_disk(t : T?) {
    t |> run("point in unit disk has length <= 1") @(t : T?) {
        var seed = random_seed(50)
        for (_i in range(50)) {
            let v = random_in_unit_disk(seed)
            let len = sqrt(v.x * v.x + v.y * v.y)
            t |> equal(len <= 1.0, true)
            t |> equal(v.z, 0.0)
        }
    }
}

[test(description = "each_random_uint is an endless iterator a for loop can draw from and break out of")]
def test_each_random_uint(t : T?) {
    t |> run("each_random_uint produces values") @(t : T?) {
        var count = 0
        var rng <- each_random_uint()
        for (_v in rng) {
            count ++
            if (count >= 10) {
                break
            }
        }
        t |> equal(count, 10)
    }
}

// cpu_supports fails closed: an unknown name, or another
// architecture's name, reports false. Only ISA-guaranteed baselines
// and implications are asserted, never a feature a given box may lack
[test(description = "cpu_supports reports false for unknown or foreign-architecture names, true for ISA baselines, and keeps the feature implication ladder consistent")]
def test_cpu_supports(t : T?) {
    t |> run("unknown names fail closed") @(t : T?) {
        t |> success(!cpu_supports("bogus-feature-name"), "unknown feature reports false")
        t |> success(!cpu_supports(""), "empty feature reports false")
    }
    t |> run("architecture consistency") @(t : T?) {
        let arch = get_architecture_name()
        if (arch == "x86_64" || arch == "x86") {
            // implication ladder on real silicon (shared OS-state requirement
            // makes these safe)
            if (cpu_supports("avx2")) {
                t |> success(cpu_supports("avx"), "avx2 implies avx")
            }
            if (cpu_supports("avx512vnni")) {
                t |> success(cpu_supports("avx512f"), "avx512vnni implies avx512f")
            }
            if (cpu_supports("avx512vbmi")) {
                t |> success(cpu_supports("avx512f"), "avx512vbmi implies avx512f")
            }
            if (cpu_supports("avx512bw")) {
                t |> success(cpu_supports("avx512f"), "avx512bw implies avx512f")
            }
            if (cpu_supports("avx512vl")) {
                t |> success(cpu_supports("avx512f"), "avx512vl implies avx512f")
            }
            if (cpu_supports("avx512bf16")) {
                t |> success(cpu_supports("avx512f"), "avx512bf16 implies avx512f")
            }
            for (ext in ["amx-int8", "amx-bf16", "amx-fp16"]) {
                if (cpu_supports(ext)) {
                    t |> success(cpu_supports("amx-tile"), "{ext} implies amx-tile")
                }
            }
        } elif (arch == "arm64" || arch == "arm64ec") {
            // neon and fp-armv8 are mandatory in every arm64 profile, so these
            // are ISA facts
            t |> success(cpu_supports("neon"), "neon is baseline on arm64")
            t |> success(cpu_supports("fp-armv8"), "fp-armv8 is baseline on arm64")
            if (cpu_supports("i8mm")) {
                t |> success(cpu_supports("neon"), "i8mm implies neon")
            }
            if (cpu_supports("bf16")) {
                t |> success(cpu_supports("neon"), "bf16 implies neon")
            }
            t |> success(!cpu_supports("avx2"), "x86 feature names report false off-x86")
            t |> success(!cpu_supports("sse4.2"), "x86 feature names report false off-x86")
            t |> success(!cpu_supports("amx-bf16"), "x86 feature names report false off-x86")
            t |> success(!cpu_supports("amx-fp16"), "x86 feature names report false off-x86")
        } else {
            t |> success(!cpu_supports("avx2"), "x86 feature names report false off-x86")
            t |> success(!cpu_supports("sse4.2"), "x86 feature names report false off-x86")
            t |> success(!cpu_supports("dotprod"), "arm feature names report false off-arm")
        }
    }
    t |> run("arm names off-arm and x86 names off-x86 both fail closed") @(t : T?) {
        let arch = get_architecture_name()
        if (arch == "x86_64" || arch == "x86") {
            t |> success(!cpu_supports("dotprod"), "arm feature names report false on x86")
            t |> success(!cpu_supports("i8mm"), "arm feature names report false on x86")
        }
    }
}
