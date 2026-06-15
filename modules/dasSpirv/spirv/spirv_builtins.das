options gen2

// Shader builtin globals, recognized by name in the emitter (mirrors dasGlsl's glsl_common
// gl_* declarations). They exist only so authored shader code typechecks; the shader body is
// never executed in daslang — only its AST is read and lowered to SPIR-V. Each maps to a
// BuiltIn-decorated variable in spirv_emit.das — Input for read-only builtins (the compute IDs,
// gl_VertexIndex/gl_InstanceIndex, gl_FragCoord), Output for gl_Position (the one written builtin).

module spirv_builtins shared public

// compute
var gl_NumWorkGroups        : uint3     // OpEntryPoint NumWorkgroups
var gl_WorkGroupID          : uint3     // WorkgroupId
var gl_LocalInvocationID    : uint3     // LocalInvocationId
var gl_GlobalInvocationID   : uint3     // GlobalInvocationId
var gl_LocalInvocationIndex : uint      // LocalInvocationIndex

// vertex (gl_Position is the one Output builtin; the indices are Inputs)
var gl_Position             : float4    // BuiltIn Position (Output)
var gl_VertexIndex          : int       // BuiltIn VertexIndex (Input)
var gl_InstanceIndex        : int       // BuiltIn InstanceIndex (Input)

// fragment
var gl_FragCoord            : float4    // BuiltIn FragCoord (Input)

// ===== combined image samplers =====
// sampler2D is an opaque marker type: a global of this type lowers to an OpTypeSampledImage in
// UniformConstant storage (the emitter recognizes it by name in classify_global). The struct has no
// fields — shader code never reads it directly, only passes it to texture(). texture() is likewise a
// stub: its body is never executed (only the AST is read), and the call lowers to OpImageSampleImplicitLod.
struct sampler2D {}

[unused_argument(s, uv)]
def public texture(s : sampler2D; uv : float2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// textureLod samples with an EXPLICIT level-of-detail (OpImageSampleExplicitLod): no screen-space
// derivatives, so unlike texture() it is valid in any stage (vertex/compute), not fragment-only.
[unused_argument(s, uv, lod)]
def public textureLod(s : sampler2D; uv : float2; lod : float) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// texelFetch reads a single texel by integer coordinate + mip level, unfiltered (OpImage to extract the
// image from the sampled image, then OpImageFetch). Stage-agnostic (no derivatives).
[unused_argument(s, coord, lod)]
def public texelFetch(s : sampler2D; coord : int2; lod : int) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// ===== additional sampler dimensionalities =====
// Each is an opaque marker recognized by name in classify_global (sampler_info maps it to the SPIR-V
// image Dim + Arrayed flag). All sample via texture() with a float3 coordinate: a cubemap direction, a
// 3D texture coordinate, or (u,v,layer) for a 2D array. The emitter's texture() handler is dimension-
// agnostic, so these need only their marker type + a texture() overload.
struct sampler3D {}
struct samplerCube {}
struct sampler2DArray {}

[unused_argument(s, uv)]
def public texture(s : sampler3D; uv : float3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(s, uv)]
def public texture(s : samplerCube; uv : float3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(s, uv)]
def public texture(s : sampler2DArray; uv : float3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// ===== separate image + sampler =====
// texture2D is a standalone sampled image (OpTypeImage, no sampler) and sampler is a standalone
// OpTypeSampler -- two independent descriptors with their own @binding. sampleTexture combines them at
// the call site (OpSampledImage) before sampling (OpImageSampleImplicitLod, fragment-only). This is the
// Vulkan separate-image/sampler model, distinct from the combined sampler2D.
struct texture2D {}
struct sampler {}

[unused_argument(tex, smp, uv)]
def public sampleTexture(tex : texture2D; smp : sampler; uv : float2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// ===== storage images =====
// image2D is an opaque marker for a read-write storage image (UniformConstant OpTypeImage with a
// known Rgba8 format, recognized by name in classify_global). imageStore/imageLoad are stubs: their
// bodies never run (only the AST is read), and they lower to OpImageWrite / OpImageRead. A KNOWN
// format means no StorageImageRead/WriteWithoutFormat capability is required (lavapipe-safe).
struct image2D {}

[unused_argument(img, coord, value)]
def public imageStore(img : image2D; coord : int2; value : float4) : void {}

[unused_argument(img, coord)]
def public imageLoad(img : image2D; coord : int2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}
