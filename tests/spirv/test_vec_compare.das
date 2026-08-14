options gen2
options indenting = 4

// Whole-vector `==` / `!=`. In daslang (and GLSL) comparing two vectors with `==` / `!=` yields a
// SINGLE bool (all lanes equal / any lane differs), but the SPIR-V compare op is component-wise and
// produces a bvecN — so the result must be reduced with OpAll (`==`) / OpAny (`!=`) to the scalar
// bool the branch consumes. The emitter used to type the compare's result `bool` directly on vector
// operands, which is invalid SPIR-V (spirv-val + drivers reject `OpFOrdEqual %bool %vecN %vecN`);
// the first shader to hit it (a vertex shader culling a black star with `if (rgb == float3(0))`)
// compiled but crashed the driver's shader compiler. This gates the OpAll/OpAny reduction and
// spirv-val's the blob (the invalid form never validated because no prior fixture compared vectors).

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require spirv/spirv_shader         // [vertex_shader]
require spirv/spirv_builtins public
require spirv/spirv_dis            // has_op
require spirv/spirv_grammar        // SpvOp

var @in @location = 0 vc_a : float3
var @in @location = 1 vc_b : float3
var @out @location = 0 vc_col : float3
[vertex_shader(name="vec_cmp_spv"), marker(no_coverage)]
def vec_cmp {
    // `==` on vec3 -> OpFOrdEqual (bvec3) + OpAll -> scalar bool for the branch
    if (vc_a == float3(0.0f, 0.0f, 0.0f)) {
        gl_Position = float4(0.0f, 0.0f, 2.0f, 1.0f)
        vc_col = float3(0.0f, 0.0f, 0.0f)
        return
    }
    // `!=` on vec3 -> OpFUnordNotEqual/OpFOrdNotEqual (bvec3) + OpAny -> scalar bool
    var tint = vc_a
    if (vc_a != vc_b) {
        tint = vc_b
    }
    gl_Position = float4(tint, 1.0f)
    vc_col = tint
}

def public vec_cmp_words : array<uint> {
    return clone_to_move(vec_cmp_spv)
}

[test]
def test_vec_compare(t : T?) {
    t |> run("whole-vector ==/!= reduce via OpAll/OpAny to a scalar bool; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(vec_cmp_spv)
        t |> success(!empty(words), "vector-compare shader lowers")
        t |> success(has_op(words, SpvOp.All), "== reduces with OpAll")
        t |> success(has_op(words, SpvOp.Any), "!= reduces with OpAny")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
