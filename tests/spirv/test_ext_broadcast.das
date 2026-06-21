options gen2
options indenting = 4

// Ext-op scalar-broadcast gate (regression for the FMix-scalar-t crash).
//
// `lerp(vecN, vecN, scalar)` lowers to OpExtInst FMix, but SPIR-V FMix requires ALL three operands to
// match the result type — there is no implicit scalar->vector broadcast. So the emitter MUST splat the
// scalar t to the vector width (OpCompositeConstruct) before the OpExtInst. Before that fix, the scalar
// went straight into FMix: the blob still EMITTED (so a bytes-only check passed) but was INVALID SPIR-V
// and blew up as a driver access-violation at vkCreateGraphicsPipelines. The real oracle is spirv-val.
//
// Each lerp result is written straight to its own @out (NO float4(...) constructors), so the only
// OpCompositeConstruct instructions in the module are the three scalar->vector splats — the counts are
// then exact + attributable even when spirv-val soft-skips locally (a missing splat would drop the
// CompositeConstruct count to 0). `lerp` is the only GLSL.std.450 op daslang exposes with a vec+scalar
// overload, so it is the only reachable broadcast case; the emitter's ext_op_broadcasts_scalar also
// covers FClamp/FMin/FMax/Step/SmoothStep defensively (no daslang overload reaches them yet).

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require spirv/spirv_shader         // [fragment_shader] annotation
require spirv/spirv_dis            // count_op / count_op_operand_at
require spirv/spirv_grammar        // SpvOp, GLSLstd450
require math                       // lerp(vec,vec,scalar)

var @in @location = 0 eb_a : float4
var @in @location = 1 eb_b : float4
var @in @location = 2 eb_s : float
var @out @location = 0 eb_out2 : float2
var @out @location = 1 eb_out3 : float3
var @out @location = 2 eb_out4 : float4
[fragment_shader(name="extbcast_spv"), marker(no_coverage)]
def extbcast {
    eb_out2 = lerp(eb_a.xy, eb_b.xy, eb_s)      // FMix float2,float2,scalar -> splat t to float2
    eb_out3 = lerp(eb_a.xyz, eb_b.xyz, eb_s)    // FMix float3,float3,scalar -> splat t to float3
    eb_out4 = lerp(eb_a, eb_b, eb_s)            // FMix float4,float4,scalar -> splat t to float4
}

[test]
def test_ext_broadcast(t : T?) {
    t |> run("lerp(vec,vec,scalar) -> FMix splats the scalar t to vec width; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(extbcast_spv)
        // Exactly the three lerps lower to OpExtInst FMix. GLSLstd450.FMix is operand #3 of OpExtInst
        // (result-type, result-id, set, instruction, args...), so this rejects a non-FMix ext-inst.
        t |> success(count_op_operand_at(words, SpvOp.ExtInst, 3, uint(GLSLstd450.FMix)) == 3,
            "three OpExtInst FMix (lerp lowered to FMix at widths 2/3/4)")
        // The only OpCompositeConstruct in the module are the three scalar->vector splats; if the
        // broadcast were missing, the scalar would go straight into FMix and this count would be 0.
        t |> success(count_op(words, SpvOp.CompositeConstruct) == 3,
            "three OpCompositeConstruct == the scalar->vec splats, and nothing else")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
