require dastest/testing_boost

require llvm/llvm_jit
require llvm

[jit]
def jit_test_abi_mad ( a,b,c )
    return test_abi_mad(a,b,c)

[test]
def test_vec_abi ( t:T? )
    t |> run("test_abi_mad float2") <| @ ( t : T? )
        t |> success(is_jit_function(@@<(a,b,c:float2):float2>jit_test_abi_mad))
        let a = float2(1,2)
        let b = float2(3,4)
        let c = float2(5,6)
        let tt = test_abi_mad(a,b,c)
        let jt = jit_test_abi_mad(a,b,c)
        t |> equal(tt,jt)
    t |> run("test_abi_mad float3") <| @ ( t : T? )
        t |> success(is_jit_function(@@<(a,b,c:float3):float3>jit_test_abi_mad))
        let a = float3(1,2,3)
        let b = float3(4,5,6)
        let c = float3(7,8,9)
        let tt = test_abi_mad(a,b,c)
        let jt = jit_test_abi_mad(a,b,c)
        t |> equal(tt,jt)
    t |> run("test_abi_mad float4") <| @ ( t : T? )
        t |> success(is_jit_function(@@<(a,b,c:float4):float4>jit_test_abi_mad))
        let a = float4(1,2,3,4)
        let b = float4(5,6,7,8)
        let c = float4(9,10,11,12)
        let tt = test_abi_mad(a,b,c)
        let jt = jit_test_abi_mad(a,b,c)
        t |> equal(tt,jt)

[sideeffects]
def dummy
    assert(false)

[test]
def test_func_abi ( t:T? )
    let fnA = @@dummy
    let fnB = test_abi_func(fnA)
    t |> equal ( fnA, fnB )
