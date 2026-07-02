options gen2
options indenting = 4

// Same-width vector component-kind conversions (found writing the tutorial-15 raygen; the natural
// idiom `int2(gl_GlobalInvocationID.xy)` used to fail "component-kind conversion ... not supported").
//
// A single-argument vecN constructor whose argument is a vecN of a DIFFERENT component kind is a
// plain conversion: OpConvertSToF/UToF/FToS/FToU for value conversions, OpBitcast for int<->uint —
// all vector-valid in SPIR-V (result type = the vector type). Cross-width cross-kind stays rejected.
// spirv-val'd.

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require spirv/spirv_shader         // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis            // has_op
require spirv/spirv_grammar        // SpvOp

var @ssbo @binding = 0 vc_out : array<float4>

[compute_shader(local_size_x=64, name="vc_spv"), marker(no_coverage)]
def vc {
    let gid = gl_GlobalInvocationID.xy      // uint2
    let f = float2(gid)                     // uint2 -> float2 : OpConvertUToF
    let si = int2(gid)                      // uint2 -> int2   : OpBitcast
    let f3 = float3(int3(si.x, si.y, 3))    // int3 -> float3  : OpConvertSToF
    let u2 = uint2(f * 4.0)                 // float2 -> uint2 : OpConvertFToU
    let i2 = int2(f)                        // float2 -> int2  : OpConvertFToS
    let i = int(gl_GlobalInvocationID.x)
    vc_out[i] = float4(f.x + f3.x, float(si.y), float(u2.x), float(i2.y))
}

[test]
def test_vec_convert(t : T?) {
    t |> run("same-width vector kind conversions -> OpConvert*/OpBitcast on vectors; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(vc_spv)
        t |> success(has_op(words, SpvOp.ConvertUToF), "float2(uint2) -> OpConvertUToF")
        t |> success(has_op(words, SpvOp.ConvertSToF), "float3(int3) -> OpConvertSToF")
        t |> success(has_op(words, SpvOp.ConvertFToU), "uint2(float2) -> OpConvertFToU")
        t |> success(has_op(words, SpvOp.ConvertFToS), "int2(float2) -> OpConvertFToS")
        t |> success(has_op(words, SpvOp.Bitcast), "int2(uint2) -> OpBitcast")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
