options gen2
options indenting = 4

// Integer vector * scalar gate.
//
// SPIR-V has no OpVectorTimesScalar for integers (that op is float-only), so `ivecN * int` must splat
// the scalar to the vector width (OpCompositeConstruct) and then do a component-wise OpIMul. Before the
// fix the emitter rejected this with "integer vector/scalar mixes need a splat" (fail-closed, but a
// missing feature). Both operand orders (vec*scalar, scalar*vec) and both int + uint are exercised.
//
// The vectors are loaded from SSBO array<ivec> elements (NOT constructed), so the only
// OpCompositeConstruct instructions are the three scalar->vector splats — the counts are exact and
// attributable even when spirv-val soft-skips locally.

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require spirv/spirv_shader         // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis            // count_op
require spirv/spirv_grammar        // SpvOp

var @ssbo @binding = 0 ivs_iv : array<int3>
var @ssbo @binding = 1 ivs_uv : array<uint3>
var @ssbo @binding = 2 ivs_is : array<int>
var @ssbo @binding = 3 ivs_us : array<uint>
[compute_shader(local_size_x=64, name="ivs_spv"), marker(no_coverage)]
def ivs {
    let i = gl_GlobalInvocationID.x
    ivs_iv[i] = ivs_iv[i] * ivs_is[i]       // int3 * int   -> splat + OpIMul
    ivs_iv[i] = ivs_is[i] * ivs_iv[i]       // int * int3   -> splat + OpIMul (reverse order)
    ivs_uv[i] = ivs_uv[i] * ivs_us[i]       // uint3 * uint -> splat + OpIMul
}

[test]
def test_int_vec_scalar(t : T?) {
    t |> run("ivecN * int -> splat (OpCompositeConstruct) + OpIMul; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(ivs_spv)
        // Three integer vector*scalar multiplies -> three OpIMul.
        t |> success(count_op(words, SpvOp.IMul) == 3, "three OpIMul (component-wise integer multiply)")
        // The only OpCompositeConstruct are the three scalar->vec splats (vectors are loaded, not built).
        t |> success(count_op(words, SpvOp.CompositeConstruct) == 3, "three OpCompositeConstruct == the splats")
        // Integers must NOT route through OpVectorTimesScalar (that op is float-only).
        t |> success(!has_op(words, SpvOp.VectorTimesScalar), "no OpVectorTimesScalar (ints use IMul)")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
