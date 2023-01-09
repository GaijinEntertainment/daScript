require dastest/testing_boost

require llvm/llvm_jit
require jit

[jit]
def swizzle_ref_xyz ( var v:float4&; xyz:float3 )
    v.xyz = xyz

[jit]
def swizzle_ref_xy ( var v:float4&; xy:float2 )
    v.xy = xy

[jit]
def swizzle_ref_x ( var v:float4&; x:float )
    v.x = x

[jit]
def swizzle_ref_yzw ( var v:float4&; xyz:float3 )
    v.yzw = xyz

[jit]
def swizzle_ref_yz ( var v:float4&; xy:float2 )
    v.yz = xy

[jit]
def swizzle_ref_y ( var v:float4&; x:float )
    v.y = x


[jit]
def swizzle_xyz ( v:float4 )
    return v.xyz

[jit]
def swizzle_xy ( v:float4 )
    return v.xy
[jit]
def swizzle_x ( v:float4 )
    return v.x

[jit]
def swizzle_yzw ( v:float4 )
    return v.yzw

[jit]
def swizzle_yz ( v:float4 )
    return v.yz

[jit]
def swizzle_y ( v:float4 )
    return v.y

[test]
def test_vector_swizzle ( t:T? )
    t |> run("swizzle ref") <| @ ( t : T? )
        var v = float4(1,2,3,4)
        t |> success ( is_jit_function(@@swizzle_ref_xyz) )
        swizzle_ref_xyz(v, float3(5,6,7))
        t |> equal(v, float4(5,6,7,4))
        t |> success ( is_jit_function(@@swizzle_ref_xy) )
        swizzle_ref_xy(v, float2(8,9))
        t |> equal(v, float4(8,9,7,4))
        t |> success ( is_jit_function(@@swizzle_ref_x) )
        swizzle_ref_x(v, 10.)
        t |> equal(v, float4(10,9,7,4))
        v = float4(1,2,3,4)
        t |> success ( is_jit_function(@@swizzle_ref_yzw) )
        swizzle_ref_yzw(v, float3(5,6,7))
        t |> equal(v, float4(1,5,6,7))
        t |> success ( is_jit_function(@@swizzle_ref_yz) )
        swizzle_ref_yz(v, float2(8,9))
        t |> equal(v, float4(1,8,9,7))
        t |> success ( is_jit_function(@@swizzle_ref_y) )
        swizzle_ref_y(v, 10.)
        t |> equal(v, float4(1,10,9,7))
    t |> run("swizzle imm") <| @ ( t : T? )
        var v = float4(1,2,3,4)
        t |> success ( is_jit_function(@@swizzle_xyz) )
        t |> equal( float3(1,2,3), swizzle_xyz(v) )
        t |> success ( is_jit_function(@@swizzle_xy) )
        t |> equal( float2(1,2), swizzle_xy(v) )
        t |> success ( is_jit_function(@@swizzle_x) )
        t |> equal( 1., swizzle_x(v) )
        t |> success ( is_jit_function(@@swizzle_yzw) )
        t |> equal( float3(2,3,4), swizzle_yzw(v) )
        t |> success ( is_jit_function(@@swizzle_yz) )
        t |> equal( float2(2,3), swizzle_yz(v) )
        t |> success ( is_jit_function(@@swizzle_y) )
        t |> equal( 2., swizzle_y(v) )





