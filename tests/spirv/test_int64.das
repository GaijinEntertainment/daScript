options gen2
options indenting = 4

// 64-bit INT gate: int64 / uint64 scalars, the way a visibility
// buffer uses them.
//
//   TYPE      OpTypeInt 64 behind OpCapability Int64
//   CONVERT   uint64(uint) -> OpUConvert (widen),
//             uint(uint64) -> OpUConvert (narrow, keeps low bits)
//   CONSTANT  `32ul` -> OpConstant on %ulong, TWO words, low first
//   ARITH     << >> | & on 64 bits, no 32-bit detour
//   STORAGE   @ssbo array<uint64>: runtime array, ArrayStride 8
//   ATOMIC    atomicMax(uint64&, uint64) -> OpAtomicUMax on the
//             64-bit pointer + OpCapability Int64Atomics
//
// daslang has no 64-bit int VECTORS, so (like double) the rail is
// scalar-only.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader          // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID, atomicMax
require spirv/spirv_dis             // count_op, op_has_operand, ...
require spirv/spirv_grammar         // SpvOp, SpvCapability

var @ssbo @binding = 0 vis : array<uint64>
var @ssbo @binding = 1 ids : array<uint>

// The visibility-buffer write: depth bits in the high word, an id in
// the low word, keep the max.
[compute_shader(local_size_x=64, name="i64_vis_spv"), marker(no_coverage)]
def i64_vis_shader {
    let i = int(gl_GlobalInvocationID.x)
    let depth = ids[i]
    let packed = (uint64(depth) << 32ul) | uint64(uint(i))
    atomicMax(vis[i], packed)
}

// The resolve: read the packed value back, split the two halves.
[compute_shader(local_size_x=64, name="i64_unpack_spv"), marker(no_coverage)]
def i64_unpack_shader {
    let i = int(gl_GlobalInvocationID.x)
    let v = vis[i]      // nolint:LINT021 - the 64-bit shift / mask before narrowing is tested
    ids[i] = uint(v >> 32ul) ^ uint(v & 0xfffffffful)
}

def private ulong_type_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeInt) && n >= 3 && w[s + 1] == 64u && w[s + 2] == 0u) {
            id = w[s]
        }
    }
    return id
}

// An OpConstant on `type_id` whose two literal words are lo then hi.
def private has_const64(words : array<uint>; type_id, lo, hi : uint) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.Constant) && n == 4 && w[s] == type_id
                && w[s + 2] == lo && w[s + 3] == hi) {
            found = true
        }
    }
    return found
}

// How many `op` instructions produce a value of type `type_id`.
def private count_of_type(words : array<uint>; op : SpvOp; type_id : uint) : int =>
    count_op_operand_at(words, op, 0, type_id)

def private check_int64_surface(t : T?; words : array<uint>; what : string) {
    t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.Int64)),
        "{what}: OpCapability Int64")
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{what}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_int64(t : T?) {
    t |> run("visibility-buffer pack + 64-bit atomicMax") <| @(t : T?) {
        var words <- clone_to_move(i64_vis_spv)
        check_int64_surface(t, words, "pack form")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.Int64Atomics)),
            "OpCapability Int64Atomics")
        let ul = ulong_type_id(words)
        t |> success(ul != 0u, "the %ulong type is declared")
        t |> success(count_of_type(words, SpvOp.AtomicUMax, ul) == 1, "one OpAtomicUMax on %ulong")
        t |> success(count_of_type(words, SpvOp.UConvert, ul) == 2, "two OpUConvert to %ulong")
        t |> success(count_of_type(words, SpvOp.ShiftLeftLogical, ul) == 1,
            "one 64-bit OpShiftLeftLogical")
        t |> success(count_of_type(words, SpvOp.BitwiseOr, ul) == 1, "one 64-bit OpBitwiseOr")
        // word order: 32ul is low word 32, high word 0
        t |> success(has_const64(words, ul, 32u, 0u), "32ul is OpConstant %ulong 32, 0")
        delete words
    }
    t |> run("unpack: 64-bit shift / and, narrow back to 32 bits") <| @(t : T?) {
        var words <- clone_to_move(i64_unpack_spv)
        check_int64_surface(t, words, "unpack form")
        let ul = ulong_type_id(words)
        t |> success(count_of_type(words, SpvOp.ShiftRightLogical, ul) == 1,
            "one 64-bit OpShiftRightLogical")
        t |> success(count_of_type(words, SpvOp.BitwiseAnd, ul) == 1, "one 64-bit OpBitwiseAnd")
        t |> success(count_op(words, SpvOp.UConvert) == 2, "two OpUConvert narrow back to uint")
        t |> success(has_const64(words, ul, 0xffffffffu, 0u),
            "0xfffffffful is OpConstant %ulong 0xffffffff, 0")
        delete words
    }
}
