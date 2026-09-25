options gen2
options indenting = 4

// Phase-6 math builtins (text layer): the value-call whitelist lowers 1:1 to MSL spellings —
// float scalar + vector classes (abs also signed int; min/max any numeric class) — plus the
// golden snapshot as the forward regression guard.

require dastest/testing_boost public
require _msl_common
require daslib/strings_boost

let GOLDEN_MATHCALLS = "#include <metal_stdlib>
using namespace metal;
kernel void mathcalls(device const float* a [[buffer(0)]],
        device const float* b [[buffer(1)]],
        device float* c [[buffer(2)]],
        device const float4* va [[buffer(3)]],
        device const float4* vb [[buffer(4)]],
        device float4* vc [[buffer(5)]],
        device const int* ia [[buffer(6)]],
        device const int* ib [[buffer(7)]],
        device int* ic [[buffer(8)]],
        device const uint* ua [[buffer(9)]],
        device const uint* ub [[buffer(10)]],
        device uint* uc [[buffer(11)]],
        device const char* qa [[buffer(12)]],
        device char* qc [[buffer(13)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) \{
    uint i = gl_GlobalInvocationID.x;
    float x = a[i];
    float y = b[i];
    c[i] = (((exp(min(x, 4.0f)) + sqrt(abs(x))) + (rsqrt((abs(y) + 1.0f)) + tanh(x))) + ((sin(x) + cos(y)) + (saturate(x) + max(x, y))));
    c[i] += float(qa[i]);
    c[i] += log((abs(x) + 1.0f));
    float4 u = va[i];
    float4 v = vb[i];
    vc[i] = (((exp(min(u, float4(4.0f, 4.0f, 4.0f, 4.0f))) + sqrt(abs(u))) + (rsqrt((abs(v) + float4(1.0f, 1.0f, 1.0f, 1.0f))) + tanh(u))) + ((sin(u) + cos(v)) + (saturate(u) + max(u, v))));
    vc[i] = (vc[i] + round(v));
    ic[i] = (min(abs(ia[i]), ib[i]) + max(ia[i], ib[i]));
    uc[i] = (min(ua[i], ub[i]) + max(ua[i], ub[i]));
    qc[i] = char(round((b[i] * 12.5f)));
    c[i] += (((floor(x) + ceil(y)) + (atan(x) + atan2(y, x))) + (pow((abs(x) + 1.0f), y) + fmod(x, 3.0f)));
    c[i] += (clamp(x, -1.0f, 1.0f) + fma(x, y, 0.5f));
    vc[i] = (((vc[i] + (floor(u) + ceil(v))) + clamp(u, float4(-1.0f, -1.0f, -1.0f, -1.0f), float4(1.0f, 1.0f, 1.0f, 1.0f))) + fma(u, v, float4(0.5f, 0.5f, 0.5f, 0.5f)));
    ic[i] += clamp(ia[i], -8, 8);
\}
"

[test]
def test_mathcalls_shape(t : T?) {
    t |> run("mathcalls: unary float math lowers by name") <| @(t : T?) {
        t |> success(contains(mathcalls_msl, "exp(min(x, 4.0f))"), "exp + scalar min")
        t |> success(contains(mathcalls_msl, "sqrt(abs(x))"), "sqrt + float abs")
        t |> success(contains(mathcalls_msl, "rsqrt((abs(y) + 1.0f))"), "rsqrt")
        t |> success(contains(mathcalls_msl, "log((abs(x) + 1.0f))"), "log")
        t |> success(contains(mathcalls_msl, "tanh(x)"), "tanh")
        t |> success(contains(mathcalls_msl, "sin(x)") && contains(mathcalls_msl, "cos(y)"), "sin/cos")
        t |> success(contains(mathcalls_msl, "saturate(x)"), "saturate")
        t |> success(contains(mathcalls_msl, "max(x, y)"), "binary max")
        t |> success(contains(mathcalls_msl, "floor(x)") && contains(mathcalls_msl, "ceil(y)"), "floor/ceil")
        t |> success(contains(mathcalls_msl, "atan(x)") && contains(mathcalls_msl, "atan2(y, x)"), "atan/atan2")
        t |> success(contains(mathcalls_msl, "pow((abs(x) + 1.0f), y)") && contains(mathcalls_msl, "fmod(x, 3.0f)"), "pow/fmod")
        t |> success(contains(mathcalls_msl, "clamp(x, -1.0f, 1.0f)"), "ternary clamp")
        t |> success(contains(mathcalls_msl, "fma(x, y, 0.5f)"), "mad lowers to fma")
    }
    t |> run("mathcalls: vector + integer classes") <| @(t : T?) {
        t |> success(contains(mathcalls_msl, "exp(min(u, float4(4.0f, 4.0f, 4.0f, 4.0f)))"), "float4 exp/min (folded vector const)")
        t |> success(contains(mathcalls_msl, "saturate(u)") && contains(mathcalls_msl, "max(u, v)"), "float4 saturate/max")
        t |> success(contains(mathcalls_msl, "min(abs(ia[i]), ib[i])"), "signed-int abs + min")
        t |> success(contains(mathcalls_msl, "clamp(ia[i], -8, 8)"), "signed-int clamp")
        t |> success(contains(mathcalls_msl, "fma(u, v, float4(0.5f, 0.5f, 0.5f, 0.5f))") && contains(mathcalls_msl, "floor(u)"), "float4 mad/floor")
        t |> success(contains(mathcalls_msl, "min(ua[i], ub[i])") && contains(mathcalls_msl, "max(ua[i], ub[i])"), "uint min/max")
    }
    t |> run("mathcalls: round + f32/i8 converts (the quantize-kernel trio)") <| @(t : T?) {
        t |> success(contains(mathcalls_msl, "round(v)"), "float4 round (das half-away == MSL round)")
        t |> success(contains(mathcalls_msl, "qc[i] = char(round((b[i] * 12.5f)));"), "int8(round(f)) -> char(round(f))")
        t |> success(contains(mathcalls_msl, "c[i] += float(qa[i]);"), "int8 -> float dequant convert")
    }
    t |> run("mathcalls: golden snapshot") <| @(t : T?) {
        t |> equal(mathcalls_msl, GOLDEN_MATHCALLS)
    }
}
