require dastest/testing_boost

require llvm/llvm_jit
require llvm

[jit]
def jit_test_abi_mad ( a,b,c:float3 )
    return test_abi_mad(a,b,c)

[test]
def test_vec_abi ( t:T? )
    let a = float3(1,2,3)
    let b = float3(4,5,6)
    let c = float3(7,8,9)
    let tt = test_abi_mad(a,b,c)
    let jt = jit_test_abi_mad(a,b,c)
    t |> equal(tt,jt)
