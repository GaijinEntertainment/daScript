options gen2
options indenting = 4

module glsl_common shared public

require math
require daslib/shader_lingua_franca public

// The modern, backend-invariant builtin surface (sampler2D/sampler3D/image2D/image3D, gl_Position, the
// compute IDs, texture()/textureSize()/imageLoad()/imageStore()/imageSize()/barrier()/
// memoryBarrierShared()/discard()/dFdx()/dFdy()) now lives in shader_lingua_franca, re-exported
// above so every existing `require glsl/glsl_common` consumer still sees it unchanged. This file
// keeps only the GL-only surface: the modern GL-only intrinsics with no Vulkan counterpart yet, the
// legacy GLSL 1.x builtins, and the host-side sampler/image binding operators.

// ===== GL-only intrinsics (no Vulkan counterpart yet) =====
[unused_argument(sample, uv, ofs)]
def textureOffset(sample : sampler2D; uv : float2; ofs : int2) {
    assert(false, "we should not be here\n")
    return float4(0.)
}

[sideeffects]
def memoryBarrier {
    assert(false, "we should not be here\n")
}

[sideeffects]
def memoryBarrierImage {
    assert(false, "we should not be here\n")
}

def fract(x : float) => x - floor(x)

def fract(x : float2) => x - floor(x)

def fract(x : float3) => x - floor(x)

def fract(x : float4) => x - floor(x)

// ===== Legacy GLSL 1.x builtins =====
// GL-only; NOT in the lingua franca. Invalid in GLSL ES 3.00 and desktop core -- superseded by
// explicit `out vec4` fragment outputs and texture(). Kept so existing demos keep compiling on the
// older path; new shaders should use the modern surface from shader_lingua_franca.
var gl_FragColor : float4

[unused_argument(sampler, uv)]
def texture2D(sampler : sampler2D; uv : float2) {
    assert(false, "we should not be here\n")
    return float4(0.)
}

// ===== host-side sampler/image binding =====
// Assign a GL texture-unit handle into a sampler/image marker (read host-side by the binder). They
// operate on the shared resource types via their GL-superset `texture2D`/`texture3D` field.
def operator :=(var image : image2D; sampler : sampler2D) {
    image.texture2D = sampler.texture2D
}

def operator :=(var sampler : sampler2D; image : image2D) {
    sampler.texture2D = image.texture2D
}

def operator :=(var image : image2D; sampler : uint) {
    image.texture2D = sampler
}

def operator :=(var sampler : sampler2D; image : uint) {
    sampler.texture2D = image
}

def operator :=(var sampler : sampler3D; image : uint) {
    sampler.texture3D = image
}

// The wider sampler dimensionalities (moved to shader_lingua_franca) bind a GL texture-unit handle
// into their own GL-superset field. These complete the host-side assignment surface the portable
// sampler-move work left half-done (it added the bind codegen + bind_sampler_cube paths but not the
// := operators) -- the bind_uniform codegen reads <var>.textureCube / .texture2DArray / .texture2D.
def operator :=(var sampler : samplerCube; image : uint) {
    sampler.textureCube = image
}

def operator :=(var sampler : sampler2DArray; image : uint) {
    sampler.texture2DArray = image
}

def operator :=(var sampler : sampler2DShadow; image : uint) {
    sampler.texture2D = image
}
