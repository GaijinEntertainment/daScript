options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module math_bits shared public

// _rh, _lh - right handed vs left handed
//  OpenGL, Vulkan - RH

def int_bits_to_float ( x:int )
    unsafe
        return reinterpret<float> x

def int_bits_to_float ( x:int2 )
    unsafe
        return reinterpret<float2> x

def int_bits_to_float ( x:int3 )
    unsafe
        return reinterpret<float3> x

def int_bits_to_float ( x:int4 )
    unsafe
        return reinterpret<float4> x

def uint_bits_to_float ( x:uint )
    unsafe
        return reinterpret<float> x

def uint_bits_to_float ( x:uint2 )
    unsafe
        return reinterpret<float2> x

def uint_bits_to_float ( x:uint3 )
    unsafe
        return reinterpret<float3> x

def uint_bits_to_float ( x:uint4 )
    unsafe
        return reinterpret<float4> x

def float_bits_to_int ( x:float )
    unsafe
        return reinterpret<int> x

def float_bits_to_int ( x:float2 )
    unsafe
        return reinterpret<int2> x

def float_bits_to_int ( x:float3 )
    unsafe
        return reinterpret<int3> x

def float_bits_to_int ( x:float4 )
    unsafe
        return reinterpret<int4> x

def float_bits_to_uint ( x:float )
    unsafe
        return reinterpret<uint> x

def float_bits_to_uint ( x:float2 )
    unsafe
        return reinterpret<uint2> x

def float_bits_to_uint ( x:float3 )
    unsafe
        return reinterpret<uint3> x

def float_bits_to_uint ( x:float4 )
    unsafe
        return reinterpret<uint4> x
