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

// vertex (gl_Position + gl_PointSize are the Output builtins; the indices are Inputs)
var gl_Position             : float4    // BuiltIn Position (Output)
var gl_PointSize            : float     // BuiltIn PointSize (Output) — point size in pixels when rasterising POINT_LIST; needs the largePoints device feature for values > 1
var gl_VertexIndex          : int       // BuiltIn VertexIndex (Input)
var gl_InstanceIndex        : int       // BuiltIn InstanceIndex (Input)
var gl_DrawID               : int       // BuiltIn DrawIndex (Input) — multi-draw draw call ordinal; needs the DrawParameters capability (in core in SPIR-V 1.3)

// fragment
var gl_FragCoord            : float4    // BuiltIn FragCoord (Input)
var gl_FragDepth            : float     // BuiltIn FragDepth (Output) — writing it forces DepthReplacing
var gl_PointCoord           : float2    // BuiltIn PointCoord (Input) — UV within the rasterised point primitive, [0,1]^2; valid only when POINT_LIST topology is rendered

// ===== combined image samplers =====
// sampler2D is an opaque marker type: a global of this type lowers to an OpTypeSampledImage in
// UniformConstant storage (the emitter recognizes it by name in classify_global). The struct has no
// fields — shader code never reads it directly, only passes it to texture(). texture() is likewise a
// stub: its body is never executed (only the AST is read), and the call lowers to OpImageSampleImplicitLod.
struct sampler2D {}

[unused_argument(s, uv), sideeffects]
def public texture(s : sampler2D; uv : float2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// textureLod samples with an EXPLICIT level-of-detail (OpImageSampleExplicitLod): no screen-space
// derivatives, so unlike texture() it is valid in any stage (vertex/compute), not fragment-only.
[unused_argument(s, uv, lod), sideeffects]
def public textureLod(s : sampler2D; uv : float2; lod : float) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// texelFetch reads a single texel by integer coordinate + mip level, unfiltered (OpImage to extract the
// image from the sampled image, then OpImageFetch). Stage-agnostic (no derivatives).
[unused_argument(s, coord, lod), sideeffects]
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

[unused_argument(s, uv), sideeffects]
def public texture(s : sampler3D; uv : float3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(s, uv), sideeffects]
def public texture(s : samplerCube; uv : float3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(s, uv), sideeffects]
def public texture(s : sampler2DArray; uv : float3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// ===== depth-compare (shadow) sampling =====
// sampler2DShadow is an opaque marker for a DEPTH-comparison combined sampler: its OpTypeImage carries
// the Depth=1 flag and the descriptor is a comparison sampler. textureCompare samples it with a
// reference value (OpImageSampleDrefImplicitLod) and returns the scalar comparison result in [0,1]
// (1 = fully lit, 0 = fully shadowed; a filtered sampler returns the PCF average). Implicit LOD ->
// fragment-only. The stub body never runs — only the call AST is lowered.
struct sampler2DShadow {}

[unused_argument(s, uv, compare), sideeffects]
def public textureCompare(s : sampler2DShadow; uv : float2; compare : float) : float {
    return 0.0
}

// ===== separate image + sampler =====
// texture2D is a standalone sampled image (OpTypeImage, no sampler) and sampler is a standalone
// OpTypeSampler -- two independent descriptors with their own @binding. sampleTexture combines them at
// the call site (OpSampledImage) before sampling (OpImageSampleImplicitLod, fragment-only). This is the
// Vulkan separate-image/sampler model, distinct from the combined sampler2D.
struct texture2D {}
struct sampler {}

[unused_argument(tex, smp, uv), sideeffects]
def public sampleTexture(tex : texture2D; smp : sampler; uv : float2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// ===== storage images =====
// image2D is an opaque marker for a read-write storage image (UniformConstant OpTypeImage with a
// known Rgba8 format, recognized by name in classify_global). imageStore/imageLoad are stubs: their
// bodies never run (only the AST is read), and they lower to OpImageWrite / OpImageRead. A KNOWN
// format means no StorageImageRead/WriteWithoutFormat capability is required (lavapipe-safe).
struct image2D {}

[unused_argument(img, coord, value), sideeffects]
def public imageStore(img : image2D; coord : int2; value : float4) : void {}

[unused_argument(img, coord), sideeffects]
def public imageLoad(img : image2D; coord : int2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// imageSize is a stub too: it lowers to OpImageQuerySize (the storage image's dimensions in texels,
// no LOD). Lets a compute shader size its pixel<->coordinate mapping off the bound image instead of a
// hardcoded constant.
[unused_argument(img), sideeffects]
def public imageSize(img : image2D) : int2 {
    return int2(0, 0)
}

// ===== subpass input attachments (Vulkan deferred-rendering rail) =====
// subpassInput is an opaque marker for a Vulkan subpass input attachment: a per-fragment read of a
// previous subpass's color or depth attachment, sampled at the SAME fragment coordinate (no UV).
// Lowers to OpTypeImage with Dim=SubpassData (Sampled=2, format=Unknown) + an InputAttachmentIndex
// decoration. The host binds it as VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT. Requires the
// InputAttachment capability. The shader declares it with `@input_attachment_index = N` alongside
// the usual `@set` / `@binding` slots:
//     var @set = 0 @binding = 0 @input_attachment_index = 0 subpass_color : subpassInput
// subpassLoad samples it at the current fragment (no coord operand on the daslang side; the int2
// (0,0) is emitted internally as the OpImageRead coordinate). Returns float4 even for depth inputs
// -- the depth value lands in .x.
struct subpassInput {}

[unused_argument(i), sideeffects]
def public subpassLoad(i : subpassInput) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// ===== fragment realism =====
// discard() abandons the current fragment (OpKill). It is a block terminator, so it must be the last
// statement on its control-flow path (typically `if (cond) { discard() }`); fragment-only.
[sideeffects]
def public discard() : void {}

// Screen-space partial derivatives over the 2x2 fragment quad: dFdx/dFdy (OpDPdx/OpDPdy), and
// fwidth = |dFdx| + |dFdy| (OpFwidth). Fragment-only. Concrete per-width overloads (NOT a generic):
// the emitter dispatches by the call name, and a generic instance carries a mangled name the
// name-match would miss. The stub bodies are never executed — only the call AST is lowered.
[unused_argument(p), sideeffects] def public dFdx(p : float) : float { return p }
[unused_argument(p), sideeffects] def public dFdx(p : float2) : float2 { return p }
[unused_argument(p), sideeffects] def public dFdx(p : float3) : float3 { return p }
[unused_argument(p), sideeffects] def public dFdx(p : float4) : float4 { return p }
[unused_argument(p), sideeffects] def public dFdy(p : float) : float { return p }
[unused_argument(p), sideeffects] def public dFdy(p : float2) : float2 { return p }
[unused_argument(p), sideeffects] def public dFdy(p : float3) : float3 { return p }
[unused_argument(p), sideeffects] def public dFdy(p : float4) : float4 { return p }
[unused_argument(p), sideeffects] def public fwidth(p : float) : float { return p }
[unused_argument(p), sideeffects] def public fwidth(p : float2) : float2 { return p }
[unused_argument(p), sideeffects] def public fwidth(p : float3) : float3 { return p }
[unused_argument(p), sideeffects] def public fwidth(p : float4) : float4 { return p }

// textureSize(sampler, lod): the mip-level dimensions in texels (OpImageQuerySizeLod; needs the
// ImageQuery capability). int2 for 2D/Cube, int3 for 3D / 2D-array (width,height,depth-or-layers).
[unused_argument(s, lod), sideeffects]
def public textureSize(s : sampler2D; lod : int) : int2 { return int2(0, 0) }
[unused_argument(s, lod), sideeffects]
def public textureSize(s : samplerCube; lod : int) : int2 { return int2(0, 0) }
[unused_argument(s, lod), sideeffects]
def public textureSize(s : sampler3D; lod : int) : int3 { return int3(0, 0, 0) }
[unused_argument(s, lod), sideeffects]
def public textureSize(s : sampler2DArray; lod : int) : int3 { return int3(0, 0, 0) }

// ===== compute synchronization =====
// barrier() is a control + shared-memory barrier (OpControlBarrier at Workgroup scope): every
// invocation in the workgroup waits here, and writes to @workgroup memory before the barrier are
// visible to all after it. The must-have sync point for shared-memory compute (the tiled-reduction
// pattern). memoryBarrierShared() is the memory-only variant (OpMemoryBarrier) -- it orders shared
// writes without the execution rendezvous. Both compute-only; the stub bodies never run.
[sideeffects]
def public barrier() : void {}
[sideeffects]
def public memoryBarrierShared() : void {}

// ===== atomics =====
// Atomic read-modify-write on a @workgroup or @ssbo lvalue, returning the OLD value (GLSL/SPIR-V
// semantics). The target is the first argument, passed by reference so the emitter recovers its
// pointer; int and uint overloads cover signed/unsigned ops (atomicMin/Max pick SMin/UMin /
// SMax/UMax by the operand's signedness). Lowered to OpAtomicIAdd/SMin/.../Exchange/CompareExchange
// at Workgroup scope (shared) or Device scope (buffer). The stub bodies never run -- only the call
// AST is read. Use as `let old = atomicAdd(counter, 1)`.
[unused_argument(value), sideeffects] def public atomicAdd(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicAdd(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicMin(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicMin(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicMax(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicMax(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicAnd(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicAnd(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicOr(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicOr(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicXor(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicXor(mem : uint &; value : uint) : uint { return mem }
[unused_argument(value), sideeffects] def public atomicExchange(mem : int &; value : int) : int { return mem }
[unused_argument(value), sideeffects] def public atomicExchange(mem : uint &; value : uint) : uint { return mem }
// atomicCompSwap(mem, compare, value): if mem == compare, store value; returns the OLD value either
// way (OpAtomicCompareExchange). The lock-free CAS primitive.
[unused_argument(compare, value), sideeffects] def public atomicCompSwap(mem : int &; compare, value : int) : int { return mem }
[unused_argument(compare, value), sideeffects] def public atomicCompSwap(mem : uint &; compare, value : uint) : uint { return mem }
