options gen2
options no_unused_function_arguments = false

def test_vec2(a : auto(vec2) -const -&) {
    let x : auto(numT) = a[0]

    var mut_a_mul = a;
    var mut_a_div = a;
    let c = numT(2)
    mut_a_mul *= c
    mut_a_div /= c

    assert(mut_a_mul == vec2(a[0] * c, a[1] * c))
    assert(mut_a_div == vec2(a[0] / c, a[1] / c))
}


def test_vec3(a : auto(vec3) -const -&) {
    let x : auto(numT) = a[0]

    var mut_a_mul = a;
    var mut_a_div = a;
    let c = numT(0)
    mut_a_mul *= c
    mut_a_div /= c

    assert(mut_a_mul == vec3(a[0] * c, a[1] * c, a[2] * c))
    assert(mut_a_div == vec3(a[0] / c, a[1] / c, a[2] / c))
}


def test_vec4(a : auto(vec4) -const -&) {
    let x : auto(numT) = a[0]

    var mut_a_mul = a;
    var mut_a_div = a;
    let c = numT(2)
    mut_a_mul *= c
    mut_a_div /= c

    assert(mut_a_mul == vec4(a[0] * c, a[1] * c, a[2] * c, a[3] * c))
    assert(mut_a_div == vec4(a[0] / c, a[1] / c, a[2] / c, a[3] / c))
}


[export]
def test {
    test_vec2(float2(3.15, 2.14))
    test_vec3(float3(3.15, 2.14, 9.32))
    test_vec4(float4(3.15, 2.14, 9.32, 1.23))
    test_vec2(int2(6, -5))
    test_vec3(int3(6, -5, 8))
    test_vec4(int4(6, -9, 13, -12))
    test_vec2(uint2(15, 20))
    test_vec3(uint3(15, 20, 25))
    test_vec4(uint4(15, 20, 25, 30))
    return true
}

