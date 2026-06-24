options gen2
options no_unused_block_arguments
options no_unused_function_arguments
module shader_dsl_primitives shared public

require daslib/stub


// Global engine inputs — not tied to any specific geometry contract.
// Access these directly by name in any shader.
var @alias = gTime g_Time : float
var @alias = lightDir g_LightDirection : float3
var @alias = cameraPos   g_CameraPosition : float3
var @alias = cameraFwd   g_CameraForward  : float3
var @alias = cameraRight g_CameraRight    : float3
var @alias = cameraUp    g_CameraUp       : float3



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
    @alias = screenPos   screenPos   : float2
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
    @alias = screenPos   screenPos   : float2
}

struct UnlitOutput {
    @alias = out_albedo      color       : float3 = float3(0)
    @alias = out_alpha       alpha       : float  = 1.0
    @alias = out_alphaCutoff alphaCutoff : float  = 0.0
}

struct PostEffectInput {
    @alias = screenPos screenPos : float2
}

struct PostEffectOutput {
    @alias = out_albedo out_color : float3 = float3(0)
    @alias = out_alpha  alpha     : float  = 1.0
}

// struct VertexInput {}
// struct VertexOutput {}


// ---- Texture sampling -------------------------------------------------------

[stub, hint(sampleTexture)]
def tex2d(sampler : Sampler2D, uv : float2) : float4 {}

[stub, hint(sampleSceneColor)]
def sample_scene_color(uv : float2) : float3 {}

// Returns eye depth (distance from camera in world units, reversed-Z corrected).
[stub, hint(sampleSceneDepth)]
def sample_scene_depth(uv : float2) : float {}


// ---- Noise ------------------------------------------------------------------

[stub, hint(perlinNoise2D)]
def noise(pos : float2) : float {}

[stub, hint(perlinNoise3D)]
def noise(pos : float3) : float {}


// ---- Generic (AnyFloat) component-wise operations ---------------------------
// Overloaded per float width; the AST transformer maps them all to the same
// shader graph node via the [hint] annotation.

// add = + operator
// sub = - operator
// mul = * operator
// div = / operator
// neg = unary - operator

// -- abs (unary) --
[stub, hint(abs)]
def abs(x : float) : float {}
[stub, hint(abs)]
def abs(x : float2) : float2 {}
[stub, hint(abs)]
def abs(x : float3) : float3 {}
[stub, hint(abs)]
def abs(x : float4) : float4 {}

// -- min (binary) --
[stub, hint(min)]
def min(x : float | int, y : float | int) : float {}
[stub, hint(min)]
def min(x : float2, y : float2) : float2 {}
[stub, hint(min)]
def min(x : float3, y : float3) : float3 {}
[stub, hint(min)]
def min(x : float4, y : float4) : float4 {}

// -- max (binary) --
[stub, hint(max)]
def max(x : float | int, y : float | int) : float {}
[stub, hint(max)]
def max(x : float2, y : float2) : float2 {}
[stub, hint(max)]
def max(x : float3, y : float3) : float3 {}
[stub, hint(max)]
def max(x : float4, y : float4) : float4 {}

// -- horizontal reduce (vector -> scalar) --
[stub, hint(hmin)]
def hmin(x : float2) : float {}
[stub, hint(hmin)]
def hmin(x : float3) : float {}
[stub, hint(hmin)]
def hmin(x : float4) : float {}
[stub, hint(hmax)]
def hmax(x : float2) : float {}
[stub, hint(hmax)]
def hmax(x : float3) : float {}
[stub, hint(hmax)]
def hmax(x : float4) : float {}
[stub, hint(hadd)]
def hadd(x : float2) : float {}
[stub, hint(hadd)]
def hadd(x : float3) : float {}
[stub, hint(hadd)]
def hadd(x : float4) : float {}

// -- clamp (ternary) --
[stub, hint(clamp)]
def clamp(x : float | int, min : float | int, max : float | int) : float {}
[stub, hint(clamp)]
def clamp(x : float2, min : float2, max : float2) : float2 {}
[stub, hint(clamp)]
def clamp(x : float3, min : float3, max : float3) : float3 {}
[stub, hint(clamp)]
def clamp(x : float4, min : float4, max : float4) : float4 {}

// -- saturate (unary) --
[stub, hint(sat)]
def saturate(x : float | int) : float {}
[stub, hint(sat)]
def saturate(x : float2) : float2 {}
[stub, hint(sat)]
def saturate(x : float3) : float3 {}
[stub, hint(sat)]
def saturate(x : float4) : float4 {}

// -- frac (unary) --
[stub, hint(frac)]
def frac(x : float | int) : float {}
[stub, hint(frac)]
def frac(x : float2) : float2 {}
[stub, hint(frac)]
def frac(x : float3) : float3 {}
[stub, hint(frac)]
def frac(x : float4) : float4 {}

// -- floor (unary) --
[stub, hint(floor)]
def floor(x : float | int) : float {}
[stub, hint(floor)]
def floor(x : float2) : float2 {}
[stub, hint(floor)]
def floor(x : float3) : float3 {}
[stub, hint(floor)]
def floor(x : float4) : float4 {}

// -- ceil (unary) --
[stub, hint(ceil)]
def ceil(x : float | int) : float {}
[stub, hint(ceil)]
def ceil(x : float2) : float2 {}
[stub, hint(ceil)]
def ceil(x : float3) : float3 {}
[stub, hint(ceil)]
def ceil(x : float4) : float4 {}

// -- mad (ternary: a * b + c) --
[stub, hint(mad)]
def mad(a : float | int, b : float | int, c : float | int) : float {}
[stub, hint(mad)]
def mad(a : float2, b : float2, c : float2) : float2 {}
[stub, hint(mad)]
def mad(a : float3, b : float3, c : float3) : float3 {}
[stub, hint(mad)]
def mad(a : float4, b : float4, c : float4) : float4 {}
[stub, hint(mad)]
def mad(a : float2, b : float, c : float2) : float2 {}
[stub, hint(mad)]
def mad(a : float3, b : float, c : float3) : float3 {}
[stub, hint(mad)]
def mad(a : float4, b : float, c : float4) : float4 {}

// -- lerp (a, b: AnyFloat; t: float scalar) --
[stub, hint(lerp)]
def lerp(a : float | int, b : float | int, t : float) : float {}
[stub, hint(lerp)]
def lerp(a : float2, b : float2, t : float) : float2 {}
[stub, hint(lerp)]
def lerp(a : float3, b : float3, t : float) : float3 {}
[stub, hint(lerp)]
def lerp(a : float4, b : float4, t : float) : float4 {}

// -- lerp with component-wise t (float2/3/4 weight per channel) --
[stub, hint(lerp4)]
def lerp(a : float2, b : float2, t : float2) : float2 {}
[stub, hint(lerp4)]
def lerp(a : float3, b : float3, t : float3) : float3 {}
[stub, hint(lerp4)]
def lerp(a : float4, b : float4, t : float4) : float4 {}


// ---- Transcendentals (AnyFloat, component-wise) -----------------------------

// -- sqrt (unary) --
[stub, hint(sqrt)]
def sqrt(x : float | int) : float {}
[stub, hint(sqrt)]
def sqrt(x : float2) : float2 {}
[stub, hint(sqrt)]
def sqrt(x : float3) : float3 {}
[stub, hint(sqrt)]
def sqrt(x : float4) : float4 {}

// -- pow (binary) --
[stub, hint(pow)]
def pow(x : float | int, y : float | int) : float {}
[stub, hint(pow)]
def pow(x : float2, y : float2) : float2 {}
[stub, hint(pow)]
def pow(x : float3, y : float3) : float3 {}
[stub, hint(pow)]
def pow(x : float4, y : float4) : float4 {}

// -- sin (unary) --
[stub, hint(sin)]
def sin(x : float | int) : float {}
[stub, hint(sin)]
def sin(x : float2) : float2 {}
[stub, hint(sin)]
def sin(x : float3) : float3 {}
[stub, hint(sin)]
def sin(x : float4) : float4 {}

// -- cos (unary) --
[stub, hint(cos)]
def cos(x : float | int) : float {}
[stub, hint(cos)]
def cos(x : float2) : float2 {}
[stub, hint(cos)]
def cos(x : float3) : float3 {}
[stub, hint(cos)]
def cos(x : float4) : float4 {}

// -- atan2 (binary) --
[stub, hint(atan2)]
def atan2(x : float | int, y : float | int) : float {}
[stub, hint(atan2)]
def atan2(x : float2, y : float2) : float2 {}
[stub, hint(atan2)]
def atan2(x : float3, y : float3) : float3 {}
[stub, hint(atan2)]
def atan2(x : float4, y : float4) : float4 {}

// -- log (unary) --
[stub, hint(log)]
def log(x : float | int) : float {}
[stub, hint(log)]
def log(x : float2) : float2 {}
[stub, hint(log)]
def log(x : float3) : float3 {}
[stub, hint(log)]
def log(x : float4) : float4 {}

// -- exp (unary) --
[stub, hint(exp)]
def exp(x : float | int) : float {}
[stub, hint(exp)]
def exp(x : float2) : float2 {}
[stub, hint(exp)]
def exp(x : float3) : float3 {}
[stub, hint(exp)]
def exp(x : float4) : float4 {}

// -- sign (unary) --
[stub, hint(sign)]
def sign(x : float | int) : float {}
[stub, hint(sign)]
def sign(x : float2) : float2 {}
[stub, hint(sign)]
def sign(x : float3) : float3 {}
[stub, hint(sign)]
def sign(x : float4) : float4 {}

// -- radians / degrees (angle conversion; unary) --
// Re-added locally (the client drop dropped them); flagged back to the client. flatten_opt
// lowers each to a single `mul` (radians(x) -> x * K) when they resolve as math builtins.
[stub, hint(radians)]
def radians(x : float | int) : float {}
[stub, hint(radians)]
def radians(x : float2) : float2 {}
[stub, hint(radians)]
def radians(x : float3) : float3 {}
[stub, hint(radians)]
def radians(x : float4) : float4 {}

[stub, hint(degrees)]
def degrees(x : float | int) : float {}
[stub, hint(degrees)]
def degrees(x : float2) : float2 {}
[stub, hint(degrees)]
def degrees(x : float3) : float3 {}
[stub, hint(degrees)]
def degrees(x : float4) : float4 {}


// ---- Dot product (dimension-specific, returns scalar) -----------------------

[stub, hint(dot_f2)]
def dot(x : float2, y : float2) : float {}

[stub, hint(dot_f3)]
def dot(x : float3, y : float3) : float {}

[stub, hint(dot_f4)]
def dot(x : float4, y : float4) : float {}


// ---- Cross product ----------------------------------------------------------

[stub, hint(cross_f3)]
def cross(x : float3, y : float3) : float3 {}


// ---- Length (dimension-specific, returns scalar) ----------------------------

[stub, hint(len_f2)]
def length(x : float2) : float {}

[stub, hint(len_f3)]
def length(x : float3) : float {}

[stub, hint(len_f4)]
def length(x : float4) : float {}


// ---- Normalize (dimension-specific) -----------------------------------------

[stub, hint(norm_f2)]
def normalize(x : float2) : float2 {}

[stub, hint(norm_f3)]
def normalize(x : float3) : float3 {}

[stub, hint(norm_f4)]
def normalize(x : float4) : float4 {}


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

[stub, hint(step)]
def step(x : float | int, y : float | int) : float {}
[stub, hint(step)]
def step(x : float2, y : float2) : float2 {}
[stub, hint(step)]
def step(x : float3, y : float3) : float3 {}
[stub, hint(step)]
def step(x : float4, y : float4) : float4 {}

[stub, hint(smoothStep)]
def smooth_step(min : float | int, max : float | int, x : float | int) : float {}
[stub, hint(smoothStep)]
def smooth_step(min : float2, max : float2, x : float2) : float2 {}
[stub, hint(smoothStep)]
def smooth_step(min : float3, max : float3, x : float3) : float3 {}
[stub, hint(smoothStep)]
def smooth_step(min : float4, max : float4, x : float4) : float4 {}

[stub, hint(unpackNormal)]
def unpack_normal(sample : float4) : float3 {}


// ---- Swizzle / channel ops -------------------------------------------------

// splatX == .x swizzle
// splatY == .y swizzle
// splatZ == .z swizzle
// splatW == .w swizzle

// [stub, hint(splatX)]
// def splatX(v : float) : float {}
// [stub, hint(splatX)]
// def splatX(v : float2) : float {}
// [stub, hint(splatX)]
// def splatX(v : float3) : float {}
// [stub, hint(splatX)]
// def splatX(v : float4) : float {}

// [stub, hint(splatY)]
// def splatY(v : float2) : float {}
// [stub, hint(splatY)]
// def splatY(v : float3) : float {}
// [stub, hint(splatY)]
// def splatY(v : float4) : float {}

// [stub, hint(splatZ)]
// def splatZ(v : float3) : float {}
// [stub, hint(splatZ)]
// def splatZ(v : float4) : float {}

// [stub, hint(splatW)]
// def splatW(v : float4) : float {}

// combineFloat2(x,y) = float2(x,y)
// combineFloat3(x,y,z) = float3(x,y,z)
// [stub, hint(combineFloat2)]
// def combine_float2(x : float, y : float) : float2 {}

// [stub, hint(combineFloat3)]
// def combine_float3(x : float, y : float, z : float) : float3 {}
