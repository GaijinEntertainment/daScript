options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module glsl_common shared public

struct sampler2D
    texture2D : uint

struct image2D
    texture2D : uint

def texture(sampler:sampler2D;uv:float2)
    assert(false,"we should not be here\n")
    return float4(0.)

def texture2D(sampler:sampler2D;uv:float2)
    assert(false,"we should not be here\n")
    return float4(0.)

def textureOffset(sample:sampler2D; uv:float2; ofs:int2)
    assert(false,"we should not be here\n")
    return float4(0.)

def textureSize(sampler:sampler2D;lod:int)
    assert(false,"we should not be here\n")
    return int2(0)

[sideeffects]
def imageStore(img:image2D;uv:int2;data:float4)
    assert(false,"we should not be here\n")

def imageLoad(img:image2D;uv:int2)
    assert(false,"we should not be here\n")
    return float4(0.)

def imageSize(img:image2D)
    assert(false,"we should not be here\n")
    return int2(0)

[sideeffects]
def barrier
    assert(false,"we should not be here\n")

[sideeffects]
def memoryBarrier
    assert(false,"we should not be here\n")

[sideeffects]
def memoryBarrierShared
    assert(false,"we should not be here\n")

[sideeffects]
def memoryBarrierImage
    assert(false,"we should not be here\n")

def dFdx ( val:float )
    assert(false,"we should not be here\n")
    return val

def dFdx ( val:float2 )
    assert(false,"we should not be here\n")
    return val

def dFdx ( val:float3 )
    assert(false,"we should not be here\n")
    return val

def dFdx ( val:float4 )
    assert(false,"we should not be here\n")
    return val

def dFdy ( val:float )
    assert(false,"we should not be here\n")
    return val

def dFdy ( val:float2 )
    assert(false,"we should not be here\n")
    return val

def dFdy ( val:float3 )
    assert(false,"we should not be here\n")
    return val

def dFdy ( val:float4 )
    assert(false,"we should not be here\n")
    return val

var gl_NumWorkGroups : uint3        // global work group size we gave to glDispatchCompute()
var gl_WorkGroupSize : uint3        // local work group size we defined with layout
var gl_WorkGroupID : uint3          // position of current invocation in global work group
var gl_LocalInvocationID : uint3    // of current invocation in local work group
var gl_GlobalInvocationID : uint3	// unique index of current invocation in global work group
var gl_LocalInvocationIndex	: uint  // 1d index representation of gl_LocalInvocationID

var gl_Position : float4
var gl_FragColor : float4

def operator := ( var image:image2D; sampler:sampler2D )
    image.texture2D = sampler.texture2D

def operator := ( var sampler:sampler2D; image:image2D )
    sampler.texture2D = image.texture2D

def operator := ( var image:image2D; sampler:uint )
    image.texture2D = sampler

def operator := ( var sampler:sampler2D; image:uint )
    sampler.texture2D = image
