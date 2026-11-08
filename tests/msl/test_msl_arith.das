options gen2

require dastest/testing_boost public
require _msl_common
require daslib/strings_boost

// Phase-2 arithmetic gate (text layer): per-scalar-class structural facts about the emitted MSL —
// every binary/unary opcode spelling, fmod for float %, all()/any() for das's scalar-bool vector
// ==/!= — plus a golden snapshot per kernel as the forward regression guard.
let GOLDEN_UARITH = "#include <metal_stdlib>
using namespace metal;
kernel void uarith(device const uint* a [[buffer(0)]],
        device const uint* b [[buffer(1)]],
        device uint* c [[buffer(2)]],
        device uint* d [[buffer(3)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) \{
    uint i = gl_GlobalInvocationID.x;
    uint x = a[i];
    uint y = (b[i] | 1u);
    c[i] = (((x + y) - (x * y)) + ((x / y) + (x % y)));
    d[i] = (((x & y) | (x ^ y)) + ((~x) + ((x << 2u) >> 1u)));
\}
"

let GOLDEN_IARITH = "#include <metal_stdlib>
using namespace metal;
kernel void iarith(device const int* a [[buffer(0)]],
        device const int* b [[buffer(1)]],
        device int* c [[buffer(2)]],
        device int* d [[buffer(3)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) \{
    uint i = gl_GlobalInvocationID.x;
    int x = a[i];
    int y = (b[i] | 1);
    c[i] = ((((x + y) - (x * y)) + ((x / y) + (x % y))) + (-x));
    d[i] = (((x & y) | (x ^ y)) + ((~x) + ((x << 2) >> 1)));
\}
"

let GOLDEN_FARITH = "#include <metal_stdlib>
using namespace metal;
kernel void farith(device const float* a [[buffer(0)]],
        device const float* b [[buffer(1)]],
        device float* c [[buffer(2)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) \{
    uint i = gl_GlobalInvocationID.x;
    float x = a[i];
    float y = (b[i] + 1.5f);
    c[i] = (((((x + y) - (x * y)) + (x / y)) + fmod(x, y)) + (-x));
\}
"

let GOLDEN_VECARITH = "#include <metal_stdlib>
using namespace metal;
kernel void vecarith(device const float4* fa [[buffer(0)]],
        device const float4* fb [[buffer(1)]],
        device float4* fc [[buffer(2)]],
        device const int2* ia [[buffer(3)]],
        device const int2* ib [[buffer(4)]],
        device int* flags [[buffer(5)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) \{
    uint i = gl_GlobalInvocationID.x;
    float4 u = fa[i];
    float4 v = (fb[i] * 0.5f);
    fc[i] = ((((u + v) - (u * v)) + (u / v)) + ((u * 2.0f) + (-u)));
    int2 p = ia[i];
    int2 q = ib[i];
    flags[i] = (all(p == q) ? 1 : (any(((p + q) - (p * q)) != q) ? 2 : 3));
\}
"

[test(description = "uint arithmetic and bitwise operators emit the expected MSL spellings and match the golden snapshot.")]
def test_uarith_shape(t : T?) {
    t |> run("uarith: uint arithmetic + bitwise spellings") <| @(t : T?) {
        t |> success(contains(uarith_msl, "device const uint* a [[buffer(0)]]"), "read-only uint buffer")
        t |> success(contains(uarith_msl, "device uint* c [[buffer(2)]]"), "written uint buffer")
        t |> success(contains(uarith_msl, "(x + y)") && contains(uarith_msl, "(x * y)"), "add/mul")
        t |> success(contains(uarith_msl, "(x / y)") && contains(uarith_msl, "(x % y)"), "div/mod stay infix for uint")
        t |> success(contains(uarith_msl, "(x & y)") && contains(uarith_msl, "(x ^ y)"), "band/bxor")
        t |> success(contains(uarith_msl, "(b[i] | 1u)"), "bor + decimal uint literal (das prints uint as hex)")
        t |> success(contains(uarith_msl, "(~x)"), "bitwise not")
        t |> success(contains(uarith_msl, "((x << 2u) >> 1u)"), "shifts")
    }
    t |> run("uarith: golden snapshot") <| @(t : T?) {
        t |> equal(uarith_msl, GOLDEN_UARITH)
    }
}

[test(description = "int arithmetic and unary negate emit the expected MSL and match the golden snapshot.")]
def test_iarith_shape(t : T?) {
    t |> run("iarith: int arithmetic + unary negate") <| @(t : T?) {
        t |> success(contains(iarith_msl, "device const int* a [[buffer(0)]]"), "read-only int buffer")
        t |> success(contains(iarith_msl, "(x % y)"), "int % stays infix (C-truncated on both sides)")
        t |> success(contains(iarith_msl, "(-x)"), "unary negate")
        t |> success(contains(iarith_msl, "((x << 2) >> 1)"), "int shifts (arithmetic >> matches das)")
    }
    t |> run("iarith: golden snapshot") <| @(t : T?) {
        t |> equal(iarith_msl, GOLDEN_IARITH)
    }
}

[test(description = "float arithmetic emits the expected MSL with % lowering to fmod and matches the golden snapshot.")]
def test_farith_shape(t : T?) {
    t |> run("farith: float arithmetic, % lowers to fmod") <| @(t : T?) {
        t |> success(contains(farith_msl, "fmod(x, y)"), "float % -> fmod (MSL % is integral-only)")
        t |> success(contains(farith_msl, "(b[i] + 1.5f)"), "float literal keeps its fraction + f suffix")
        t |> success(contains(farith_msl, "(x / y)"), "float div")
        t |> success(contains(farith_msl, "(-x)"), "float negate")
    }
    t |> run("farith: golden snapshot") <| @(t : T?) {
        t |> equal(farith_msl, GOLDEN_FARITH)
    }
}

[test(description = "Componentwise vector arithmetic, scalar broadcast and all/any compares emit the expected MSL and match the golden snapshot.")]
def test_vecarith_shape(t : T?) {
    t |> run("vecarith: componentwise vectors, broadcast, all/any compare") <| @(t : T?) {
        t |> success(contains(vecarith_msl, "device const float4* fa [[buffer(0)]]"), "float4 buffer param")
        t |> success(contains(vecarith_msl, "device const int2* ia [[buffer(3)]]"), "int2 buffer param")
        t |> success(contains(vecarith_msl, "(u + v)") && contains(vecarith_msl, "(u / v)"), "componentwise infix")
        t |> success(contains(vecarith_msl, "(u * 2.0f)") && contains(vecarith_msl, "(fb[i] * 0.5f)"), "vector * scalar broadcast")
        t |> success(contains(vecarith_msl, "(-u)"), "vector negate")
        t |> success(contains(vecarith_msl, "all(p == q)"), "das vector == is all-equal -> all()")
        t |> success(contains(vecarith_msl, "any("), "das vector != is any-differ -> any()")
        t |> success(contains(vecarith_msl, "? 1 : "), "ternary select")
    }
    t |> run("vecarith: golden snapshot") <| @(t : T?) {
        t |> equal(vecarith_msl, GOLDEN_VECARITH)
    }
}

// Phase-6 math builtins (text layer): the value-call whitelist lowers 1:1 to MSL spellings —
// float scalar + vector classes (abs also signed int; min/max any numeric class) — plus the
// golden snapshot as the forward regression guard.
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

[test(description = "Unary float math, vector and integer math classes, round and the f32/i8 converts lower by name and match the golden snapshot.")]
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
