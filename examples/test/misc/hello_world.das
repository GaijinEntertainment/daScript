
//        d:\Work\daScript\modules\dasLLVM\tests\type_constructors.das:658: uint2(3.67129e+09,1.39652e-37) // float2 const
//        d:\Work\daScript\modules\dasLLVM\tests\type_constructors.das:658:       expected: 2147483648,0
//        d:\Work\daScript\modules\dasLLVM\tests\type_constructors.das:658:       got: 3671291904,0

require llvm/llvm_jit

[jit,sideeffects]
def jit_uint2_2(a)
    return uint2(a)

[jit,sideeffects]
def jit_uint(a)
    return uint(a)

[export]
def main
    let j = jit_uint2_2(float2(6.47731e-15,-9.40848e+11))
    let jj = uint2(float2(6.47731e-15,-9.40848e+11))
    let q1 = uint(6.47731e-15)
    let q2 = uint(-9.40848e+11)
    let jq1 = jit_uint(6.47731e-15)
    let jq2 = jit_uint(-9.40848e+11)
    print("jit value = {j} // {j.x} {j.y}\nuint2 value = {jj} // {jj.x} {jj.y}\nuint,uint value = {q1},{q2}\n jit conversion = {jq1},{jq2}\n")


/*
[export]
def main
    let jj = uint2(float2(6.47731e-15,-9.40848e+11))
    let jx = uint(6.47731e-15)
    let jy = uint(-9.40848e+11)
    print("uint2 value = {jj} // {jj.x} {jj.y}\nexpecting {jx},{jy}\n")
*/