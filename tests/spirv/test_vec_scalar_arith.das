options gen2
options indenting = 4

// Vector / scalar arithmetic broadcast (found by spirv-val on the one-source path tracer:
// `acc / float(frame + 1)` emitted a shape-mismatched OpFDiv).
//
// SPIR-V arithmetic requires both operands to match the result type. Of daslang's binops only
// * and / broadcast vec <op> scalar (+/- are typer-rejected); `*` was already routed through
// OpVectorTimesScalar (test_vecarith), so `/` — plain and compound — is the exposed path. The
// emitter now splats the scalar operand to the vector width via OpCompositeConstruct. spirv-val'd.

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require spirv/spirv_shader         // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis            // has_op
require spirv/spirv_grammar        // SpvOp

var @ssbo @binding = 0 vsa_out : array<float4>
var @ssbo @binding = 1 vsa_iout : array<int4>

[compute_shader(local_size_x=64, name="vsa_spv"), marker(no_coverage)]
def vsa {
    let i = int(gl_GlobalInvocationID.x)
    let n = float(i + 1)
    var v = vsa_out[i].xyz
    v = v / n                       // FDiv: scalar rhs splat
    var acc = vsa_out[i]
    acc /= n                        // compound /= with scalar rhs (float4)
    vsa_out[i] = float4(v, 0.0) + acc
    var iv = vsa_iout[i].xyz
    iv = iv / (i + 2)               // SDiv: int vector / int scalar splat
    vsa_iout[i] = int4(iv, 0)
}

[test]
def test_vec_scalar_arith(t : T?) {
    t |> run("vec / scalar splats the scalar operand (FDiv/SDiv, plain + compound); spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(vsa_spv)
        t |> success(has_op(words, SpvOp.FDiv), "v / s -> OpFDiv (splat rhs)")
        t |> success(has_op(words, SpvOp.SDiv), "ivec / int -> OpSDiv (splat rhs)")
        t |> success(has_op(words, SpvOp.CompositeConstruct), "scalar operands splatted via OpCompositeConstruct")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
