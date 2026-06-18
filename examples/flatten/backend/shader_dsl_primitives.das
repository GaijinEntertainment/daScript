options gen2
options no_unused_block_arguments
options no_unused_function_arguments
module shader_dsl_primitives shared public

require daslib/stub
require math public


// Global engine inputs - not tied to any specific geometry contract.
// Access these directly by name in any shader.
var @alias = gTime g_Time : float
var @alias = lightDir g_LightDirection : float3



struct Sampler2D {}

[stub]
def Sampler2D(path : string) : Sampler2D {}


// ---- Shader I/O contracts ---------------------------------------------------

struct PbrInput {
    @alias = uv          uv          : float2
    @alias = worldPos    worldPos    : float3
    @alias = worldNormal worldNormal : float3
    @alias = viewDir     viewDir     : float3
    @alias = localPos    localPos    : float3
    @alias = localNormal localNormal : float3
}

struct PbrOutput {
    @alias = out_albedo      albedo          : float3 = float3(0)
    @alias = out_alpha       alpha           : float  = 1.0
    @alias = out_alphaCutoff alphaCutoff     : float  = 0.0
    @alias = out_metalness   metalness       : float  = 0.0
    @alias = out_roughness   roughness       : float  = 1.0
    @alias = out_emission    emission        : float3 = float3(0)
    @alias = out_normalMap   normalMap       : float3 = float3(0, 0, 1)
    @alias = out_emissionStr emissionStrength : float = 1.0
    @alias = out_ao          ao              : float  = 1.0
}

struct UnlitInput {
    @alias = uv          uv          : float2
    @alias = worldPos    worldPos    : float3
    @alias = worldNormal worldNormal : float3
    @alias = viewDir     viewDir     : float3
    @alias = localPos    localPos    : float3
    @alias = localNormal localNormal : float3
}

struct UnlitOutput {
    @alias = out_albedo      color       : float3 = float3(0)
    @alias = out_alpha       alpha       : float  = 1.0
    @alias = out_alphaCutoff alphaCutoff : float  = 0.0
}


// ---- Native core math --------------------------------------------------------
// The component-wise math ops (abs/min/max/clamp/saturate/floor/ceil/sign/sqrt/pow/
// sin/cos/atan2/log/exp/lerp/mad/step/smoothstep/radians/degrees/dot/cross/length/
// normalize) are the daslang `math` builtins (re-exported above). The shader graph
// backend recognises them by name (NATIVE_OPCODE in shader_dsl_boost.das) and maps
// each to its opcode; radians/degrees carry no opcode — flatten_opt lowers them to a
// multiply. Only the engine-specific leaves below remain DSL stubs.


// ---- Texture sampling -------------------------------------------------------

[stub, hint(sampleTexture)]
def tex2d(sampler : Sampler2D, uv : float2) : float4 {}


// ---- Noise ------------------------------------------------------------------

[stub, hint(perlinNoise2D)]
def noise(pos : float2) : float {}

[stub, hint(perlinNoise3D)]
def noise(pos : float3) : float {}


// ---- Fractional part (no core builtin) --------------------------------------

[stub, hint(frac)]
def frac(x : float | int) : float {}
[stub, hint(frac)]
def frac(x : float2) : float2 {}
[stub, hint(frac)]
def frac(x : float3) : float3 {}
[stub, hint(frac)]
def frac(x : float4) : float4 {}


// ---- smoothstep: keep the engine `smooth_step` spelling as a native alias ----
// The EdenSpark shaders spell it `smooth_step`; core spells it `smoothstep`. A thin
// native alias (NOT a stub) keeps the shaders unedited — flatten inlines it to the
// core call, which the backend maps to the SmoothStep opcode. The scalar form keeps the
// old stub's `float | int` surface (auto-promote int edges to float) for the same reason
// as the lerp compat overload below.
def smooth_step(mn : float | int; mx : float | int; x : float | int) => smoothstep(float(mn), float(mx), float(x))
def smooth_step(mn, mx, x : float2) => smoothstep(mn, mx, x)
def smooth_step(mn, mx, x : float3) => smoothstep(mn, mx, x)
def smooth_step(mn, mx, x : float4) => smoothstep(mn, mx, x)


// ---- Native-backed int-arg compat -------------------------------------------
// The old DSL stubs accepted `float | int` and auto-promoted; core lerp is float-only.
// A native-backed overload keeps `lerp(0.2, 10, daylight)` compiling unedited (the inner
// all-float call binds to core lerp — concrete beats the OR-type, so no recursion).
def lerp(a, b : float | int; t : float) => lerp(float(a), float(b), t)


// ---- Lighting / surface ops -------------------------------------------------

[stub, hint(fresnel)]
def fresnel(power : float | int; worldNorm : float3; viewDir : float3) : float {}


// ---- Utility ops ------------------------------------------------------------

[stub, hint(remap)]
def remap(x : float | int, inRange : float2, outRange : float2) : float {}
[stub, hint(remap)]
def remap(x : float2, inRange : float2, outRange : float2) : float2 {}
[stub, hint(remap)]
def remap(x : float3, inRange : float2, outRange : float2) : float3 {}
[stub, hint(remap)]
def remap(x : float4, inRange : float2, outRange : float2) : float4 {}

[stub, hint(oneMinus)]
def one_minus(x : float | int) : float {}
[stub, hint(oneMinus)]
def one_minus(x : float2) : float2 {}
[stub, hint(oneMinus)]
def one_minus(x : float3) : float3 {}
[stub, hint(oneMinus)]
def one_minus(x : float4) : float4 {}

[stub, hint(unpackNormal)]
def unpack_normal(sample : float4) : float3 {}
