options gen2
options indenting = 4

// Bool constants gate.
//
// `false` / `true` literals (ExprConstBool) lower to OpConstantFalse / OpConstantTrue — these carry NO
// literal word (the opcode IS the value), so they are type + result-id only. Before the fix the emitter
// had no rvalue path for ExprConstBool and failed the annotation patch with "no rvalue available for
// ExprConstBool". (Bool locals, comparisons, &&/||/!, and bool `if` conditions already worked — only
// the bool literals were missing.) Asserts OpTypeBool + both bool constants are emitted, spirv-val clean.

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require spirv/spirv_shader         // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis            // has_op
require spirv/spirv_grammar        // SpvOp

var @ssbo @binding = 0 bc_data : array<uint>
[compute_shader(local_size_x=64, name="bc_spv"), marker(no_coverage)]
def bc {
    let i = gl_GlobalInvocationID.x
    var hit = false                 // ExprConstBool false -> OpConstantFalse
    if (bc_data[i] > 5u) {
        hit = true                  // ExprConstBool true -> OpConstantTrue
    }
    if (!hit) {                     // bool local read + LogicalNot
        bc_data[i] = 0u
    }
}

[test]
def test_bool_const(t : T?) {
    t |> run("false/true -> OpConstantFalse/OpConstantTrue (+ OpTypeBool); spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(bc_spv)
        t |> success(has_op(words, SpvOp.TypeBool), "emits OpTypeBool")
        t |> success(has_op(words, SpvOp.ConstantTrue), "emits OpConstantTrue (the `true` literal)")
        t |> success(has_op(words, SpvOp.ConstantFalse), "emits OpConstantFalse (the `false` literal)")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
