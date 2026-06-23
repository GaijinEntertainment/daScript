options gen2
options indenting = 4

// Shader lingua franca: the backend-invariant builtin surface shared by daslang's two shader
// emitter cores -- dasGlsl (-> GLSL text) and dasSpirv (-> SPIR-V binary). Both
// `require daslib/shader_lingua_franca public`, so every existing `require glsl/glsl_common` /
// `require spirv/spirv_builtins` consumer keeps seeing these symbols unchanged.
//
// All declarations here are AST-only: the bodies never execute in daslang. Each emitter reads the
// call / reference AST and lowers it (a GLSL builtin call, or a SPIR-V opcode / BuiltIn-decorated
// variable). Recognition is by SYMBOL name in spirv_emit, and by MODULE name (this module) in
// glsl_internal -- both emitters treat a symbol from here exactly as they treat their own builtins.
//
// Only the MODERN WebGL2 / GL-4.1-core surface that BOTH rails speak lives here. GL legacy
// (GLSL 1.x: gl_FragColor, texture2D()) stays in glsl_common; VK-specific intrinsics
// (samplerCube, subpassInput, atomics, mesh, ...) stay in spirv_builtins.

module shader_lingua_franca shared public

// ===== opaque resource types =====
// GL-superset: the `texture2D`/`texture3D` uint carries the GL texture-unit handle the host binds
// (glUniform1i). The SPIR-V emitter recognizes each type by NAME (classify_global / sampler_info)
// and ignores the field entirely -- it is a pure descriptor marker there. Shader code never reads
// the field; it only passes the value to texture()/imageLoad()/imageStore().
struct sampler2D       { texture2D : uint }
struct sampler3D       { texture3D : uint }
struct image2D         { texture2D : uint }
// samplerCube (cubemap), sampler2DArray (layered 2D), sampler2DShadow (depth-compare 2D) are all
// core GLSL + GLES 3.0 / WebGL2 portable, so they live in the modern shared surface. Each carries the
// same GL-superset texture-unit handle field for the dasOpenGL rail (set host-side, ignored by SPIR-V).
struct samplerCube     { textureCube : uint }
struct sampler2DArray  { texture2DArray : uint }
struct sampler2DShadow { texture2D : uint }

// ===== vertex / fragment / point builtins =====
var gl_Position   : float4    // clip-space output position (vertex stage)
var gl_PointSize  : float     // rasterised point size, POINT topology
var gl_PointCoord : float2    // UV within a rasterised point, [0,1]^2 (fragment stage)
var gl_FragCoord  : float4    // window-space fragment coordinate, .xy in pixels (fragment stage); GLSL name-identical, SPIR-V BuiltIn FragCoord
var gl_FragDepth  : float     // fragment depth Output (fragment stage); GLSL name-identical (GL core + ES 3.0), SPIR-V BuiltIn FragDepth -- writing it forces the DepthReplacing execution mode
// gl_VertexIndex / gl_InstanceIndex are the unified (Vulkan) spellings -- the SPIR-V emitter lowers
// them to BuiltIn VertexIndex / InstanceIndex; the GLSL emitter renames them to gl_VertexID /
// gl_InstanceID (the GLSL desktop-core / ES names for the same per-vertex / per-instance index) on
// the way out. Author shaders with the one unified name on both rails.
var gl_VertexIndex   : int    // per-vertex index (vertex stage)
var gl_InstanceIndex : int    // per-instance index (vertex stage, instanced draws)

// ===== compute builtins =====
var gl_NumWorkGroups        : uint3   // global dispatch size (glDispatchCompute args)
var gl_WorkGroupID          : uint3   // this invocation's work group, in the global grid
var gl_LocalInvocationID    : uint3   // this invocation's position within its work group
var gl_GlobalInvocationID   : uint3   // unique invocation index in the global grid
var gl_LocalInvocationIndex : uint    // 1D flattening of gl_LocalInvocationID

// ===== texture sampling =====
// texture(s, uv): filtered sample with implicit LOD (fragment-only on both rails). textureSize:
// the mip-0 dimensions in texels.
[unused_argument(s, uv), sideeffects]
def public texture(s : sampler2D; uv : float2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(s, uv), sideeffects]
def public texture(s : sampler3D; uv : float3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(s, lod), sideeffects]
def public textureSize(s : sampler2D; lod : int) : int2 {
    return int2(0, 0)
}

// textureLod samples with an EXPLICIT level-of-detail: no screen-space derivatives, so unlike
// texture() it is valid in any stage (vertex/compute), not fragment-only. GLSL name-identical
// (core + ES 3.0); SPIR-V lowers it to OpImageSampleExplicitLod with a Lod operand.
[unused_argument(s, uv, lod), sideeffects]
def public textureLod(s : sampler2D; uv : float2; lod : float) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// texelFetch reads a single texel by integer coordinate + mip level, unfiltered. Stage-agnostic
// (no derivatives). GLSL name-identical (core + ES 3.0); SPIR-V lowers it to OpImage + OpImageFetch.
[unused_argument(s, coord, lod), sideeffects]
def public texelFetch(s : sampler2D; coord : int2; lod : int) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

// texture() on the wider sampler dimensionalities: a samplerCube samples by a float3 direction, a
// sampler2DArray by (u, v, layer). Both GLSL name-identical (the emitter's texture() is dimension-
// agnostic); SPIR-V keys the marker by name (Dim Cube / Arrayed). textureSize returns the mip-level
// size in texels (int2 for cube, int3 for 2D-array / 3D = width,height,layers-or-depth).
[unused_argument(s, uv), sideeffects]
def public texture(s : samplerCube; uv : float3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(s, uv), sideeffects]
def public texture(s : sampler2DArray; uv : float3) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(s, lod), sideeffects]
def public textureSize(s : samplerCube; lod : int) : int2 {
    return int2(0, 0)
}

[unused_argument(s, lod), sideeffects]
def public textureSize(s : sampler3D; lod : int) : int3 {
    return int3(0, 0, 0)
}

[unused_argument(s, lod), sideeffects]
def public textureSize(s : sampler2DArray; lod : int) : int3 {
    return int3(0, 0, 0)
}

// textureCompare samples a depth-comparison (shadow) sampler: returns the scalar compare result in
// [0,1] (1 = lit, 0 = shadowed; a filtered sampler returns the PCF average). Implicit LOD ->
// fragment-only. SPIR-V lowers it to OpImageSampleDrefImplicitLod; the GLSL emitter rewrites it to
// the GL spelling texture(sampler2DShadow, vec3(uv, compare)) (GL has no distinct shadow function).
[unused_argument(s, uv, compare), sideeffects]
def public textureCompare(s : sampler2DShadow; uv : float2; compare : float) : float {
    return 0.0
}

// ===== storage images =====
// image2D is a read-write storage image; imageStore writes a texel by integer coordinate,
// imageLoad reads one, imageSize queries the dimensions. Compute-capable rails only.
[unused_argument(img, uv, data), sideeffects]
def public imageStore(img : image2D; uv : int2; data : float4) : void {}

[unused_argument(img, uv), sideeffects]
def public imageLoad(img : image2D; uv : int2) : float4 {
    return float4(0.0, 0.0, 0.0, 0.0)
}

[unused_argument(img), sideeffects]
def public imageSize(img : image2D) : int2 {
    return int2(0, 0)
}

// ===== compute synchronization =====
// barrier(): control + shared-memory barrier (every invocation in the work group rendezvous here).
// memoryBarrierShared(): order shared-memory writes without the execution rendezvous.
[sideeffects]
def public barrier() : void {}

[sideeffects]
def public memoryBarrierShared() : void {}

// ===== fragment realism =====
// discard(): abandon the current fragment (block terminator -- last statement on its path).
// dFdx/dFdy: screen-space partial derivatives over the 2x2 fragment quad. fwidth = |dFdx| + |dFdy|,
// the cheap antialiasing width. All fragment-only; GLSL spells them identically (the GL emitter
// passes the names through), SPIR-V lowers to OpDPdx/OpDPdy/OpFwidth.
[sideeffects]
def public discard() : void {}

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
