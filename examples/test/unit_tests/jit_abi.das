options gen2
options no_aot // `test_abi_func` etc is not declared in headers.

require UnitTest

def jit_test_abi_mad(a, b, c) {
    return test_abi_mad(a, b, c)
}

def test_vec_abi() {
    if (true) {
        verify(!jit_enabled() || is_jit_function(@@ < (a, b, c : float2) : float2 > jit_test_abi_mad))
        let a = float2(1, 2)
        let b = float2(3, 4)
        let c = float2(5, 6)
        let tt = test_abi_mad(a, b, c)
        let jt = jit_test_abi_mad(a, b, c)
        assert(tt == jt)
    }

    if (true) {
        verify(!jit_enabled() || is_jit_function(@@ < (a, b, c : float3) : float3 > jit_test_abi_mad))
        let a = float3(1, 2, 3)
        let b = float3(4, 5, 6)
        let c = float3(7, 8, 9)
        let tt = test_abi_mad(a, b, c)
        let jt = jit_test_abi_mad(a, b, c)
        assert(tt == jt)
    }

    if (true) {
        verify(!jit_enabled() || is_jit_function(@@ < (a, b, c : float4) : float4 > jit_test_abi_mad))
        let a = float4(1, 2, 3, 4)
        let b = float4(5, 6, 7, 8)
        let c = float4(9, 10, 11, 12)
        let tt = test_abi_mad(a, b, c)
        let jt = jit_test_abi_mad(a, b, c)
        assert(tt == jt)
    }
}

[sideeffects]
def dummy {
    assert(false)
}

def test_func_abi() {
    let fnA = @@dummy
    let fnB = test_abi_func(fnA)
    assert(fnA == fnB)
}

[export]
def test {
    test_vec_abi()
    test_func_abi()
    return true
}
