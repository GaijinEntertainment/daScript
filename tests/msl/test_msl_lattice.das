options gen2
options indenting = 4

// Phase-4 16/8-bit lattice gate (text layer): fp16 closed arithmetic (h literals, broadcast,
// all/any half-vector compares), int-lattice storage + converts (widen / truncating narrow /
// clamp-based saturating narrow — MSL conversion-ctor semantics match das), and the packed 3-lane
// rail (das packs float3 = 12B / half3 = 6B / byte3 = 3B; buffer and uniform elements lower to
// packed_T3, loads wrap back into the plain type, stores assign whole elements). Golden snapshot
// per kernel as the forward regression guard.

require dastest/testing_boost public
require _msl_common
require daslib/strings_boost

let GOLDEN_HALFARITH = "#include <metal_stdlib>
using namespace metal;
kernel void halfarith(device const half4* ha [[buffer(0)]],
        device const half4* hb [[buffer(1)]],
        device half4* hc [[buffer(2)]],
        device const half* hs [[buffer(3)]],
        device half2* hd [[buffer(4)]],
        device int* hflags [[buffer(5)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) \{
    uint i = gl_GlobalInvocationID.x;
    half4 a = ha[i];
    half4 b = (hb[i] * 0.5h);
    half4 acc = \{\};
    half s = hs[i];
    acc = (((a + b) - (a * b)) + ((a / b) + (-a)));
    hc[i] = acc;
    half t = ((s * 2.0h) + 0.25h);
    hd[i] = half2(t, s);
    hflags[i] = (all(a == b) ? 1 : (any(a.wzyx != b) ? 2 : ((s < t) ? 3 : 4)));
\}
"

let GOLDEN_LATCONV = "#include <metal_stdlib>
using namespace metal;
kernel void latconv(device const char4* bi [[buffer(0)]],
        device int4* io [[buffer(1)]],
        device char4* bo [[buffer(2)]],
        device char4* bs [[buffer(3)]],
        device short4* st [[buffer(4)]],
        device const uchar2* ui [[buffer(5)]],
        device ushort2* uso [[buffer(6)]],
        device uint2* uo [[buffer(7)]],
        device ushort2* usat [[buffer(8)]],
        device const half2* hi [[buffer(9)]],
        device float2* fo [[buffer(10)]],
        device half2* ho [[buffer(11)]],
        device int* so [[buffer(12)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) \{
    uint i = gl_GlobalInvocationID.x;
    int4 w = int4(bi[i]);
    io[i] = ((w * w) + w);
    bo[i] = char4(((w * 3) - int4(100, 100, 100, 100)));
    bs[i] = char4(clamp(((w * 40) + w), -128, 127));
    st[i] = short4(bi[i]);
    uint2 uw = uint2(ui[i]);
    uso[i] = ushort2(ui[i]);
    uo[i] = (((uw * 3u) + uw) + uint2(7u, 9u));
    usat[i] = ushort2(clamp((uw * 1000u), 0u, 65535u));
    float2 f = float2(hi[i]);
    fo[i] = (f * 2.0f);
    ho[i] = half2((f + float2(0.25f, 0.5f)));
    short sc = short(w.x);
    so[i] = (int(sc) + int(bi[i].x));
\}
"

let GOLDEN_PACKED3 = "#include <metal_stdlib>
using namespace metal;
kernel void packed3(device const packed_float3* pf [[buffer(0)]],
        device packed_float3* pfo [[buffer(1)]],
        device const packed_half3* ph [[buffer(2)]],
        device packed_half3* pho [[buffer(3)]],
        device const packed_char3* pb [[buffer(4)]],
        device packed_int3* pio [[buffer(5)]],
        constant half& scale [[buffer(6)]],
        constant packed_float3& off [[buffer(7)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) \{
    uint i = gl_GlobalInvocationID.x;
    float3 v = float3(pf[i]);
    pfo[i] = ((v * 2.0f) + float3(off));
    half3 h = half3(ph[i]);
    pho[i] = ((h + h) * half3(scale));
    pio[i] = (int3(char3(pb[i])) * 3);
\}
"

[test(description = "fp16 closed arithmetic, h literals and all/any compares emit native half MSL and match the golden snapshot.")]
def test_halfarith_shape(t : T?) {
    t |> run("halfarith: fp16 closed arithmetic, h literals, all/any compares") <| @(t : T?) {
        t |> success(contains(halfarith_msl, "device const half4* ha [[buffer(0)]]"), "half4 buffer param")
        t |> success(contains(halfarith_msl, "device const half* hs [[buffer(3)]]"), "scalar float16 buffer -> half*")
        t |> success(contains(halfarith_msl, "(hb[i] * 0.5h)"), "half literal keeps fraction + h suffix, f16 broadcast")
        t |> success(contains(halfarith_msl, "half4 acc = \{\};"), "half4 zero-init local")
        t |> success(contains(halfarith_msl, "(a / b)") && contains(halfarith_msl, "(-a)"), "half vector div + negate")
        t |> success(contains(halfarith_msl, "((s * 2.0h) + 0.25h)"), "scalar half arithmetic")
        t |> success(contains(halfarith_msl, "all(a == b)"), "das half-vector == is all-equal -> all()")
        t |> success(contains(halfarith_msl, "any(a.wzyx != b)"), "half-vector swizzle + != -> any()")
        t |> success(contains(halfarith_msl, "half2(t, s)"), "half2 lane ctor")
    }
    t |> run("halfarith: golden snapshot") <| @(t : T?) {
        t |> equal(halfarith_msl, GOLDEN_HALFARITH)
    }
}

[test(description = "Int-lattice storage and converts emit clamp-based saturating narrows and match the golden snapshot.")]
def test_latconv_shape(t : T?) {
    t |> run("latconv: int-lattice storage + converts, clamp-based sat narrows") <| @(t : T?) {
        t |> success(contains(latconv_msl, "device const char4* bi [[buffer(0)]]"), "byte4 buffer -> char4 (das byte = SIGNED int8)")
        t |> success(contains(latconv_msl, "device const uchar2* ui [[buffer(5)]]"), "ubyte2 buffer -> uchar2")
        t |> success(contains(latconv_msl, "int4 w = int4(bi[i]);"), "widen byte4 -> int4")
        t |> success(contains(latconv_msl, "char4(((w * 3) - int4(100, 100, 100, 100)))"), "truncating narrow + folded int4 splat constant")
        t |> success(contains(latconv_msl, "char4(clamp(((w * 40) + w), -128, 127))"), "byte4_sat -> clamp + convert")
        t |> success(contains(latconv_msl, "ushort2(clamp((uw * 1000u), 0u, 65535u))"), "ushort2_sat -> unsigned clamp + convert")
        t |> success(contains(latconv_msl, "st[i] = short4(bi[i]);"), "widen within the lattice (byte4 -> short4)")
        t |> success(contains(latconv_msl, "float2 f = float2(hi[i]);"), "half2 -> float2 convert")
        t |> success(contains(latconv_msl, "half2((f + float2(0.25f, 0.5f)))"), "float2 -> half2 convert + folded float2 constant")
        t |> success(contains(latconv_msl, "short sc = short(w.x);"), "scalar int16 -> short")
        t |> success(contains(latconv_msl, "int(bi[i].x)"), "das .s0 lattice swizzle -> .x, scalar int8 widen")
    }
    t |> run("latconv: golden snapshot") <| @(t : T?) {
        t |> equal(latconv_msl, GOLDEN_LATCONV)
    }
}

[test(description = "Three-lane elements take packed_T3 with wrapped loads and whole-element stores and match the golden snapshot.")]
def test_packed3_shape(t : T?) {
    t |> run("packed3: 3-lane elements take packed_T3, loads wrap, stores are whole-element") <| @(t : T?) {
        t |> success(contains(packed3_msl, "device const packed_float3* pf [[buffer(0)]]"), "float3 buffer -> packed_float3 (das packs 12B)")
        t |> success(contains(packed3_msl, "device packed_half3* pho [[buffer(3)]]"), "half3 buffer -> packed_half3 (das packs 6B)")
        t |> success(contains(packed3_msl, "device const packed_char3* pb [[buffer(4)]]"), "byte3 buffer -> packed_char3 (das packs 3B)")
        t |> success(contains(packed3_msl, "device packed_int3* pio [[buffer(5)]]"), "int3 buffer -> packed_int3 (32-bit 3-lane packs too)")
        t |> success(contains(packed3_msl, "constant half& scale [[buffer(6)]]"), "scalar float16 uniform")
        t |> success(contains(packed3_msl, "constant packed_float3& off [[buffer(7)]]"), "3-lane uniform -> packed reference")
        t |> success(contains(packed3_msl, "float3 v = float3(pf[i]);"), "packed load wraps into the plain type")
        t |> success(contains(packed3_msl, "pfo[i] = ((v * 2.0f) + float3(off));"), "whole-element packed store stays raw; packed uniform read wraps")
        t |> success(contains(packed3_msl, "half3(scale)"), "half3 splat ctor from the scalar uniform")
        t |> success(contains(packed3_msl, "int3(char3(pb[i]))"), "packed byte3 load wraps, then widens")
    }
    t |> run("packed3: golden snapshot") <| @(t : T?) {
        t |> equal(packed3_msl, GOLDEN_PACKED3)
    }
}
