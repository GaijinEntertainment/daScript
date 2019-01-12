def test_vec2(a:auto(vec2)!const !&)
    let f2_x  = vec2(1,2)
    let x : auto(numT) = f2_x[0]
    assert("{f2_x}"=="1,2")
    let f2_y = f2_x.yx
    assert("{f2_y}"=="2,1")

[export]
def test
    test_vec2(float2())
    // test_vec3(float3())
    // test_vec4(float4())
    // test_vec2(int2())
    // test_vec3(int3())
    // test_vec4(int4())
    // test_vec2(uint2())
    // test_vec3(uint3())
    // test_vec4(uint4())
    // test_vec2(range())
    // test_vec2(urange())
    return true
