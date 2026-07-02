options gen2
options indenting = 4

// Bitwise/shift binary ops gate (found by the one-source path tracer's PCG RNG).
//
// binop_code previously covered only +,-,*,/,% — every bitwise binary op (&, |, ^, <<, >>) failed
// "unsupported binary op". They lower to BitwiseAnd/Or/Xor + ShiftLeftLogical, and `>>` splits by
// signedness: uint -> ShiftRightLogical, int -> ShiftRightArithmetic (daslang C semantics). The
// fixture is the PCG hash step — the canonical stateless shader RNG — plus an int arithmetic-shift
// arm. spirv-val'd.

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require spirv/spirv_shader         // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis            // has_op
require spirv/spirv_grammar        // SpvOp

var @ssbo @binding = 0 bw_data : array<uint>
var @ssbo @binding = 1 bw_idata : array<int>

def private bw_pcg_hash(x : uint) : uint {
    let state = x * 747796405u + 2891336453u
    let word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u
    return (word >> 22u) ^ word
}

[compute_shader(local_size_x=64, name="bw_spv"), marker(no_coverage)]
def bw {
    let i = gl_GlobalInvocationID.x
    var h = bw_pcg_hash(bw_data[i])
    h = (h & 0xFFu) | (h << 8u)
    bw_data[i] = h
    let s = bw_idata[i]
    bw_idata[i] = (s >> 2) & 15
}

[test]
def test_bitwise(t : T?) {
    t |> run("bitwise &,|,^,<<,>> -> BitwiseAnd/Or/Xor + shifts (>> splits by signedness); spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(bw_spv)
        t |> success(has_op(words, SpvOp.BitwiseAnd), "& -> OpBitwiseAnd")
        t |> success(has_op(words, SpvOp.BitwiseOr), "| -> OpBitwiseOr")
        t |> success(has_op(words, SpvOp.BitwiseXor), "^ -> OpBitwiseXor")
        t |> success(has_op(words, SpvOp.ShiftLeftLogical), "<< -> OpShiftLeftLogical")
        t |> success(has_op(words, SpvOp.ShiftRightLogical), "uint >> -> OpShiftRightLogical")
        t |> success(has_op(words, SpvOp.ShiftRightArithmetic), "int >> -> OpShiftRightArithmetic")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
