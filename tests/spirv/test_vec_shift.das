options gen2
options indenting = 4

// Vector shift gate: the splat takes the SHIFT operand's component type, not the result's.
//
// `uintN >> int` is the only vector-shift spelling daslang has (there is no uintN >> uintN overload),
// so the shift count reaches the emitter as an int while the base is a uint vector. SPIR-V requires
// Base and Shift to agree on component COUNT, not on component type -- so the scalar count splats to
// an int vector. Splatting it to the RESULT type instead built a v4uint out of int constituents:
// valid enough to emit, rejected by spirv-val ("Expected Constituents to be scalars or vectors of the
// same type as Result Type components"). Both operand widths and the compound form are exercised.

require dastest/testing_boost public
require _spirv_common              // validate_spirv
require spirv/spirv_shader         // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis            // count_op, foreach_inst
require spirv/spirv_grammar        // SpvOp

var @ssbo @binding = 0 vsh_uv : array<uint4>
var @ssbo @binding = 1 vsh_iv : array<int2>
[compute_shader(local_size_x=64, name="vsh_spv"), marker(no_coverage)]
def vsh {
    let i = gl_GlobalInvocationID.x
    vsh_uv[i] = vsh_uv[i] >> 8                  // uint4 >> int -> splat to int4, OpShiftRightLogical
    vsh_uv[i] = vsh_uv[i] << 8                  // uint4 << int -> splat to int4, OpShiftLeftLogical
    vsh_iv[i] = vsh_iv[i] >> 3                  // int2 >> int  -> splat to int2, arithmetic shift
}

// The component type of every OpCompositeConstruct's result: signed iff OpTypeInt's signedness is 1.
// A splat that took the base's type would report unsigned here, which is exactly the bug.
def private all_splats_are_signed_int(words : array<uint>) : bool {
    var int_signedness, vec_comp : table<uint; uint>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeInt) && n >= 3) {
            int_signedness[w[s]] = w[s + 2]
        } elif (opcode == uint(SpvOp.TypeVector) && n >= 3) {
            vec_comp[w[s]] = w[s + 1]
        }
    }
    var all_signed = true
    var seen = 0
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.CompositeConstruct) && n >= 1) {
            seen++
            let comp = vec_comp?[w[s]] ?? 0u
            if ((int_signedness?[comp] ?? 0u) != 1u) {
                all_signed = false
            }
        }
    }
    return all_signed && seen == 3
}

[test]
def test_vec_shift(t : T?) {
    t |> run("vecN >> int splats the count to an int vector; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(vsh_spv)
        t |> success(count_op(words, SpvOp.ShiftRightLogical) == 1, "one OpShiftRightLogical (uint4)")
        t |> success(count_op(words, SpvOp.ShiftLeftLogical) == 1, "one OpShiftLeftLogical (uint4)")
        t |> success(count_op(words, SpvOp.ShiftRightArithmetic) == 1, "one OpShiftRightArithmetic (int2)")
        t |> success(all_splats_are_signed_int(words), "the three splats are int vectors, not the base's type")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
