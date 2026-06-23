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
struct sampler2D { texture2D : uint }
struct sampler3D { texture3D : uint }
struct image2D   { texture2D : uint }

// ===== vertex / fragment / point builtins =====
var gl_Position   : float4    // clip-space output position (vertex stage)
var gl_PointSize  : float     // rasterised point size, POINT topology
var gl_PointCoord : float2    // UV within a rasterised point, [0,1]^2 (fragment stage)

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
// dFdx/dFdy: screen-space partial derivatives over the 2x2 fragment quad. All fragment-only.
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
