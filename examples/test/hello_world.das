options logInferPasses = true
options logNodes = true
options log = true

// options optimize=false

//  (Assert (Equ
//      (GetLocalRefR2VOff_TT<float3> #80 #0)
//      (AtR2V_TT<float> (GetThisBlockArgument 0) (ConstValue {0,0,0,0}) 12 0 4)) null))

//  (Assert (Equ
//      (Ref2Value_TT<float3> (GetLocalRefOff #80 #0))
//      (Ref2Value_TT<float3> (At (GetThisBlockArgument 0) (ConstValue {0,0,0,0}) 12 0 4))) null))

require fio

[sideeffects]
def inv(var a,b)
    invoke(b,a)

[sideeffects]
def test_func_arg(var data: float3x4)
    data[0] = float3(0.9, 0.2, 0.1)
    let a:float3& = data[0]
    assert(a == data[0])

[sideeffects]
def test_block_arg(var data: float3x4)
    inv(data) <| $(var data0 : float3x4)
        data0[0] = float3(0.8, 0.3, 0.1)
        let a:float3& = data0[0]
        assert(a == data0[0])

[sideeffects]
def test_block_in_block_arg(var data: float3x4)
    inv(data) <| $(var data1 : float3x4)
        inv(data1) <| $(var data0 : float3x4)
            data0[0] = float3(0.8, 0.3, 0.1)
            let a:float3& = data0[0]
            assert(a == data0[0])

[export]
def test:bool
    var data : float3x4
    test_func_arg(data)
    test_block_arg(data)
    test_block_in_block_arg(data)
    return true


