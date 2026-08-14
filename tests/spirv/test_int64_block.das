options gen2
options indenting = 4

// 64-bit integer interface-block members (int64 / uint64 — GLSL's GL_ARB_gpu_shader_int64). A UBO
// laying out a 64-bit scalar between 32-bit fields (e.g. a scene-info UBO carrying an int64
// global_time between 32-bit offsets) used to fail-closed: dasSpirv rejected the whole block, which
// cascaded into "used before assigned SSA id" for every value read through it. Now int64/uint64
// lower to OpTypeInt 64 (with the Int64 capability declared once) and pack std140-identically to the
// host (base align 8, size 8, like a double). The shader reads only the 32-bit neighbour; the 64-bit
// members are layout-only here. spirv-val'd (the structured-CFG / define-before-use / layout oracle).

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require spirv/spirv_shader         // [vertex_shader]
require spirv/spirv_builtins public
require spirv/spirv_dis            // has_op / op_has_operand_at
require spirv/spirv_grammar        // SpvOp / SpvCapability

// std140: a64 @0 (align 8, size 8), au64 @8, tail (vec3) @16 — the vec3 must clear the two 8-byte
// scalars, so its offset proves the 64-bit size is accounted for in the layout.
struct I64Block {
    a64  : int64
    au64 : uint64
    tail : float3
}
var @uniform @set = 0 @binding = 0 i64ubo : I64Block
var @out @location = 0 i64_out : float3
[vertex_shader(name="int64_block_spv"), marker(no_coverage)]
def int64_block_vs {
    gl_Position = float4(i64ubo.tail, 1.0f)
    i64_out = i64ubo.tail
}

def public int64_block_words : array<uint> {
    return clone_to_move(int64_block_spv)
}

[test]
def test_int64_block(t : T?) {
    t |> run("int64/uint64 UBO members lower (Int64 cap + OpTypeInt 64), spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(int64_block_spv)
        t |> success(!empty(words), "shader with int64/uint64 UBO members lowers")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.Int64)),
            "Int64 capability declared")
        t |> success(op_has_operand_at(words, SpvOp.TypeInt, 1, 64u),
            "a 64-bit OpTypeInt is emitted")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
