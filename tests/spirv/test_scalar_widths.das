options gen2
options no_unused_block_arguments = false

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_builtins public
require spirv/spirv_dis
require spirv/spirv_grammar
require daslib/shader_lingua_franca
require daslib/math_bits
require math

// 16-bit integer gate: the storage + convert rail (GLSL's int16_t /
// i16vec2), and the ARITHMETIC on top of it.
//
// The storage + convert half needs exactly three things, and they are
// the three glslang emits: the TYPE      OpTypeInt 16 1 (+
// OpTypeVector) behind OpCapability Int16 the CTOR      short2(int,
// int) -> one OpSConvert per lane, then OpCompositeConstruct the READ
// int(v.x) -> OpSConvert back up to 32 bits A narrowing SConvert
// truncates and a widening one sign-extends, which is what makes a
// das body bit-identical to the GLSL it ports: 0xabcd packs to -21555
// and reads back as -21555.
//
// The ARITHMETIC half is closed +/- over the family
// (daslib/shader_lingua_franca declares the operators daslang itself
// lacks), lowering to a NATIVE OpIAdd / OpISub / OpSNegate on
// OpTypeInt 16 -- not to a widen / 32-bit-op / narrow sandwich. That
// distinction is the whole point, and counting opcodes cannot see it,
// so every arithmetic assertion below pins the op's RESULT TYPE to
// the %short / %v2short id: a 32-bit lowering would carry the %int id
// there and fail. spirv-val is the independent witness (it rejects an
// OpIAdd whose operand width disagrees with its result).
//
// What GLSL widens, das must widen too: a compare against an int
// literal (`p.x < 0`) has no 16-bit form on either rail, so it reads
// through int() and lowers to OpSConvert + a 32-bit compare, which is
// exactly what glslang does with the implicit conversion.
//
// Both spellings of a GLSL helper are gated. `i16_unpack` RETURNS the
// pair (the natural das form, and the only one available for a `let`,
// since an uninitialized local is rejected); `i16_unpack_out` takes
// it by reference, which is what GLSL's `out i16vec2` is -- a
// Function-storage pointer to a v2short. spirv-val is what proves the
// pointer form is well-formed.
var @ssbo @binding = 0 i16_packed : array<int>
var @ssbo @binding = 1 i16_pair : array<int2>
var @ssbo @binding = 2 i16_quad : array<int4>

def i16_unpack(packed : int) : short2 => short2(packed >> 16, packed & 0xffff)

def i16_unpack_out(packed : int; var tc : short2&) {
    tc = short2(packed >> 16, packed & 0xffff)
}

[compute_shader(local_size_x=64, name="i16_spv"), marker(no_coverage)]
def i16_shader {
    let i = int(gl_GlobalInvocationID.x)
    let tc = i16_unpack(i16_packed[i])
    i16_pair[i] = int2(int(tc.x), int(tc.y))
}

[compute_shader(local_size_x=64, name="i16_out_spv"), marker(no_coverage)]
def i16_out_shader {
    let i = int(gl_GlobalInvocationID.x)
    var tc = short2(0, 0)
    i16_unpack_out(i16_packed[i], tc)
    i16_pair[i] = int2(int(tc.x), int(tc.y))
}

// The cube-map wrap step, in miniature: the exact arithmetic shapes a
// cube_map_wrap port needs. The scalar `+` / `-` / unary `-` are
// 16-bit; the bounds test against an int literal is not, and reads
// through int() to say so.
def i16_wrap_step(p : short2; s : int16) : short2 {
    // vs an int literal: widened, as in GLSL
    if (int(p.x) < 0) return short2(p.x + s, -p.y)
    // vs another int16: native 16-bit compare
    if (p.x >= s) return short2(p.x - s, -p.y - s)
    return p
}

[compute_shader(local_size_x=64, name="i16_arith_spv"), marker(no_coverage)]
def i16_arith_shader {
    let i = int(gl_GlobalInvocationID.x)
    let p = i16_unpack(i16_packed[i])
    let neigh = p + short2(1, -1)               // the whole-vector add, OpIAdd on %v2short
    let q = i16_wrap_step(neigh, int16(64))
    i16_pair[i] = int2(int(q.x), int(q.y))
}

// The unsigned half of the family, in the shape a material-weight
// sort needs: a u16vec4 whose lanes are shuffled by a uint16
// comparison, and an int16 max. Every 16-bit value here re-enters a
// vector through the ushort4 lane ctor, so no lane ever round-trips
// through int.
def u16_pick(a : ushort4; hi : uint16) : ushort4 {
    if (hi >= a.x) return ushort4(hi, a.x, a.y, a.z)        // uint16 compare + 16-bit-lane compose
    return a
}

[compute_shader(local_size_x=64, name="u16_spv"), marker(no_coverage)]
def u16_shader {
    let i = int(gl_GlobalInvocationID.x)
    let p = i16_quad[i]
    let a = ushort4(uint16(uint(p.x)), uint16(uint(p.y)), uint16(uint(p.z)), uint16(uint(p.w)))
    let b = u16_pick(a, uint16(uint(p.w)))
    let m = max(int16(p.x), int16(p.y))                     // OpExtInst SMax on %short
    i16_quad[i] = int4(int(uint(b.x)), int(uint(b.y)), int(uint(b.z)), int(m))
}

// The material sort's spelling: an INT straight to uint16, changing
// signedness AND width at once. That is ONE OpUConvert, because a
// NARROWING convert keeps the low bits whatever the source's
// signedness -- exactly the truncation daslang's narrowing ctor and
// GLSL's uint16_t(int) both mean. (The u16 fixture above spells the
// same narrowing through uint(x); neither round-trip costs an extra
// instruction.) The opposite direction is NOT symmetric and stays
// refused: C sign-extends an int16 into a uint32 where OpUConvert
// would zero-extend.
[compute_shader(local_size_x=64, name="u16_narrow_spv"), marker(no_coverage)]
def u16_narrow_shader {
    let i = int(gl_GlobalInvocationID.x)
    let p = i16_quad[i]
    let a = ushort4(uint16(p.x), uint16(p.y), uint16(p.z), uint16(p.w))
    i16_quad[i] = int4(int(uint(a.x)), int(uint(a.y)), int(uint(a.z)), int(uint(a.w)))
}

// The id OpTypeInt 16 0 declares -- the UNSIGNED 16-bit type,
// distinct from the signed one below.
def private ushort_type_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeInt) && n >= 3 && w[s + 1] == 16u && w[s + 2] == 0u) {
            id = w[s]
        }
    }
    return id
}

// How many OpExtInst carry `ext` as their ext-op selector? Operands
// are [result-type, result-id, set-id, ext-op, args...], so the
// selector sits at index 3.
def private count_ext_op(words : array<uint>; ext : GLSLstd450) : int =>
    count_op_operand_at(words, SpvOp.ExtInst, 3, uint(ext))

// The id OpTypeInt 16 1 declares (0 if absent). Operand layout is
// [result-id, width, signedness], so the width/signedness pair sits
// at 1..2 and the id we want at 0 -- which is why this cannot be
// spelled with op_has_operand_pair, whose answer is a bool.
def private short_type_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeInt) && n >= 3 && w[s + 1] == 16u && w[s + 2] == 1u) {
            id = w[s]
        }
    }
    return id
}

// The id of OpTypeVector <short_id> <lanes>. Operands are [result-id,
// component-type, count].
def private short_vec_type_id(words : array<uint>; short_id : uint; lanes : uint) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeVector) && n >= 3 && w[s + 1] == short_id
            && w[s + 2] == lanes) {
            id = w[s]
        }
    }
    return id
}

// How many `op` instructions produce a value of type `type_id`? An
// arithmetic op's result type is operand 0, so this is what separates
// a native 16-bit lowering from a widened 32-bit one.
def private count_op_of_type(words : array<uint>; op : SpvOp; type_id : uint) : int =>
    count_op_operand_at(words, op, 0, type_id)

// `signedness` is which 16-bit type the fixture is expected to
// declare: a fixture that only ever spells uint16 has no signed one
// to find.
def private check_int16_surface(t : T?; words : array<uint>; what : string;
                                signedness : uint = 1u) {
    t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.Int16)),
        "{what}: OpCapability Int16")
    t |> success(op_has_operand_pair(words, SpvOp.TypeInt, 16u, signedness),
        "{what}: OpTypeInt 16 {signedness} ({signedness == 1u ? "signed" : "unsigned"} 16-bit)")
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{what}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "short2 and ushort values lower with the Int16 capability, a single narrowing UConvert, native 16-bit arithmetic and unsigned compares, by value and by reference.")]
def test_int16(t : T?) { // nolint:STYLE038 - flat list of opcode/capability assertions
    t |> run("short2 by value: Int16 capability, narrowing ctor, widening read") <| @(t : T?) {
        var words <- clone_to_move(i16_spv)
        check_int16_surface(t, words, "value form")
        // two narrowing (the ctor's lanes) + two widening (the int(tc.x) /
        // int(tc.y) reads)
        t |> success(count_op(words, SpvOp.SConvert) == 4, "four OpSConvert (2 narrow, 2 widen)")
        t |> success(count_op(words, SpvOp.CompositeConstruct) >= 1, "the pair is composed")
        delete words
    }
    t |> run("short2 by reference: GLSL's `out i16vec2` is a Function pointer to a v2short") <| @(t : T?) {
        var words <- clone_to_move(i16_out_spv)
        check_int16_surface(t, words, "out form")
        // the zero-init's two lanes on top of the value form's four
        t |> success(count_op(words, SpvOp.SConvert) == 6,
                     "six OpSConvert (zero-init, ctor, reads)")
        delete words
    }
    t |> run("an int narrows STRAIGHT to uint16: one OpUConvert, signedness and width at once") <| @(t : T?) {
        var words <- clone_to_move(u16_narrow_spv)
        check_int16_surface(t, words, "int-to-uint16 form", 0u)
        let uid = ushort_type_id(words)
        t |> success(uid != 0u, "the %ushort type is declared")
        // THE assertion: four lanes, four converts. A bitcast-then-narrow
        // lowering would spend eight ops, and picking the opcode by the
        // SOURCE's signedness would emit OpSConvert -- whose result must be a
        // SIGNED int type, so it would not even be the %ushort the ctor asked
        // for.
        t |> success(count_op_of_type(words, SpvOp.UConvert, uid) == 4,
            "one OpUConvert per lane, straight from int to %ushort")
        t |> success(count_op_of_type(words, SpvOp.SConvert, uid) == 0,
            "no OpSConvert to %ushort -- the opcode follows the DESTINATION's signedness")
        delete words
    }
    t |> run("closed +/- arithmetic lowers NATIVELY to 16 bits, not through a widen/narrow sandwich") <| @(t : T?) {
        var words <- clone_to_move(i16_arith_spv)
        check_int16_surface(t, words, "arith form")
        let sid = short_type_id(words)
        let vid = short_vec_type_id(words, sid, 2u)
        t |> success(sid != 0u && vid != 0u, "the %short and %v2short types are declared")
        // `p + short2(1, -1)` -- one whole-vector add, on the v2short type
        // and not on v2int
        t |> success(count_op_of_type(words, SpvOp.IAdd, vid) == 1, "one OpIAdd on %v2short")
        // i16_wrap_step: `p.x + s` and `-p.y - s` are the scalar 16-bit add /
        // subtract...
        t |> success(count_op_of_type(words, SpvOp.IAdd, sid) == 1, "one OpIAdd on %short")
        t |> success(count_op_of_type(words, SpvOp.ISub, sid) == 2, "two OpISub on %short")
        // ...and `-p.y` (twice) is the 16-bit negate
        t |> success(count_op_of_type(words, SpvOp.SNegate, sid) == 2, "two OpSNegate on %short")
        // nothing arithmetic leaked to 32 bits
        t |> success(count_op(words, SpvOp.IAdd) == 2 && count_op(words, SpvOp.ISub) == 2
            && count_op(words, SpvOp.SNegate) == 2, "no arithmetic op widened to 32 bits")
        // `p.x >= s` compares two int16 NATIVELY, and the SConvert census is
        // what proves it: a widened compare would have to convert BOTH
        // operands, pushing the count to 9. The seven are the two narrowing
        // ctor pairs (i16_unpack's, and short2(1, -1)'s int lanes), the one
        // widening read the `int(p.x) < 0` literal test forces -- a compare
        // against an int literal has no 16-bit form on either rail, and
        // glslang's implicit widening emits the same convert -- and the two
        // int(q.*) reads. int16(64) costs none: daslang folds it to a %short
        // constant, as glslang folds its int-typed i16vec3 lane.
        t |> success(count_op(words, SpvOp.SGreaterThanEqual) == 1, "one OpSGreaterThanEqual")
        t |> success(count_op(words, SpvOp.SConvert) == 7,
            "seven OpSConvert -- the 16-bit compare converts neither operand")
        delete words
    }
    t |> run("the UNSIGNED twin: uint16 compares unsigned, ushort4 composes from 16-bit lanes, max is native") <| @(t : T?) {
        var words <- clone_to_move(u16_spv)
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.Int16)),
            "u16: OpCapability Int16")
        let uid = ushort_type_id(words)
        let sid = short_type_id(words)
        let v4id = short_vec_type_id(words, uid, 4u)
        t |> success(uid != 0u && sid != 0u && v4id != 0u,
            "u16: the %ushort, %short and %v4ushort types are declared")
        // THE SIGNEDNESS IS THE POINT: a uint16 pair must compare UNSIGNED.
        // Reading the pair through int() (the only spelling available before
        // the operators shipped) would give OpSGreaterThanEqual and order
        // 0xffff BELOW 0 -- valid SPIR-V, and wrong.
        t |> success(count_op(words, SpvOp.UGreaterThanEqual) == 1, "u16: one OpUGreaterThanEqual")
        t |> success(count_op(words, SpvOp.SGreaterThanEqual) == 0,
            "u16: the unsigned compare did NOT lower to the signed opcode")
        // the ushort4 shuffle composes from lanes that already ARE %ushort,
        // so it is one bare OpCompositeConstruct with no convert around it;
        // going through int would spend 8
        t |> success(count_op_operand_at(words, SpvOp.CompositeConstruct, 0, v4id) == 2,
            "u16: both ushort4 composes are on %v4ushort")
        // max(int16, int16) is OpExtInst SMax whose RESULT TYPE is %short --
        // counting the opcode alone cannot tell a native 16-bit max from one
        // widened to %int and narrowed back
        t |> success(count_op_of_type(words, SpvOp.ExtInst, sid) == 1,
                     "u16: one OpExtInst on %short")
        t |> success(count_ext_op(words, GLSLstd450.SMax) == 1, "u16: that ext-op is SMax")
        check_int16_surface(t, words, "u16")
        delete words
    }
}

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

[test(description = "A 64-bit visibility-buffer pack with atomicMax, and its shift-and-mask unpack back to 32 bits, emit native 64-bit opcodes.")]
def test_int64(t : T?) {
    t |> run("visibility-buffer pack + 64-bit atomicMax") <| @(t : T?) {
        var words <- clone_to_move(i64_vis_spv)
        check_int64_surface(t, words, "pack form")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.Int64Atomics)),
            "OpCapability Int64Atomics")
        let ul = ulong_type_id(words)
        t |> success(ul != 0u, "the %ulong type is declared")
        t |> success(count_op_of_type(words, SpvOp.AtomicUMax, ul) == 1,
                     "one OpAtomicUMax on %ulong")
        t |> success(count_op_of_type(words, SpvOp.UConvert, ul) == 2, "two OpUConvert to %ulong")
        t |> success(count_op_of_type(words, SpvOp.ShiftLeftLogical, ul) == 1,
            "one 64-bit OpShiftLeftLogical")
        t |> success(count_op_of_type(words, SpvOp.BitwiseOr, ul) == 1, "one 64-bit OpBitwiseOr")
        // word order: 32ul is low word 32, high word 0
        t |> success(has_const64(words, ul, 32u, 0u), "32ul is OpConstant %ulong 32, 0")
        delete words
    }
    t |> run("unpack: 64-bit shift / and, narrow back to 32 bits") <| @(t : T?) {
        var words <- clone_to_move(i64_unpack_spv)
        check_int64_surface(t, words, "unpack form")
        let ul = ulong_type_id(words)
        t |> success(count_op_of_type(words, SpvOp.ShiftRightLogical, ul) == 1,
            "one 64-bit OpShiftRightLogical")
        t |> success(count_op_of_type(words, SpvOp.BitwiseAnd, ul) == 1, "one 64-bit OpBitwiseAnd")
        t |> success(count_op(words, SpvOp.UConvert) == 2, "two OpUConvert narrow back to uint")
        t |> success(has_const64(words, ul, 0xffffffffu, 0u),
            "0xfffffffful is OpConstant %ulong 0xffffffff, 0")
        delete words
    }
}

// 64-bit integer interface-block members (int64 / uint64 — GLSL's
// GL_ARB_gpu_shader_int64). A UBO laying out a 64-bit scalar between
// 32-bit fields (e.g. a scene-info UBO carrying an int64 global_time
// between 32-bit offsets) used to fail-closed: dasSpirv rejected the
// whole block, which cascaded into "used before assigned SSA id" for
// every value read through it. Now int64/uint64 lower to OpTypeInt 64
// (with the Int64 capability declared once) and pack
// std140-identically to the host (base align 8, size 8, like a
// double). The shader reads only the 32-bit neighbour; the 64-bit
// members are layout-only here. spirv-val'd (the structured-CFG /
// define-before-use / layout oracle). std140: a64 @0 (align 8, size
// 8), au64 @8, tail (vec3) @16 — the vec3 must clear the two 8-byte
// scalars, so its offset proves the 64-bit size is accounted for in
// the layout.
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

def public int64_block_words : array<uint> => clone_to_move(int64_block_spv)

[test(description = "int64 and uint64 UBO members lower with the Int64 capability and an OpTypeInt 64 and pass spirv-val.")]
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

// 16-bit FLOAT gate: the storage + convert rail (GLSL's float16_t /
// f16vec4), and the ARITHMETIC on top of it.
//
// The storage + convert half needs exactly three things, and they are
// the three glslang emits: the TYPE      OpTypeFloat 16 (+
// OpTypeVector) behind OpCapability Float16 the CONVERT   half4(vec4)
// / vec4(half4) -> one OpFConvert, narrowing (round to nearest even)
// or widening -- a whole-vector op, not one per lane the PACK
// packHalf2x16 / unpackHalf2x16 -> the GLSL.std.450 ext-insts. Both
// sides are FLOAT2, not half2: the pack is where a float becomes 16
// bits, so a shader holding a half2 converts on the way in, which is
// the OpFConvert glslang spends there too. Unlike the 16-bit INTS,
// whose storage carries a 16-bit SSBO member, fp16 here lives only in
// registers: an f16vec4 is moved through a vec4 as two packed uints,
// so NO 16-bit storage capability is declared and the interface block
// stays 32-bit. That is exactly how a renderer packing SH
// coefficients into a float4 buffer carries them.
//
// The COMPOSE is the one thing daslang itself lacks
// (daslib/shader_lingua_franca declares it): a half4 built from two
// half2. It matters because a shader unpacking two uints holds two
// half2 and must re-enter a half4 WITHOUT widening back to float and
// narrowing all four lanes again. The constituents already ARE the
// component type, so the assertion below is that exactly TWO
// OpFConvert serve the pair -- a third would mean the compose
// round-tripped through 32 bits.
//
// The ARITHMETIC half is closed over the family (daslang's fp16
// promotes-and-rounds per op, which is bit-identical to native fp16),
// lowering to a NATIVE OpFAdd / OpFMul on OpTypeFloat 16 rather than
// a widen / 32-bit-op / narrow sandwich. Counting opcodes cannot see
// that distinction, so every arithmetic assertion pins the op's
// RESULT TYPE to the %half / %v4half id, exactly as the int16 gate
// pins %short. spirv-val is the independent witness.
var @ssbo @binding = 0 f16_data : array<float4>

// A renderer's SH-coefficient read, in miniature: four SH
// coefficients ride in a float4 as two packed uints per half, so the
// unpack is bitcast -> UnpackHalf2x16 -> narrow, and the compose is
// the half4(half2, half2) daslang lacks.
def f16_unpack(data : float4) : half4 {
    return half4(half2(unpackHalf2x16(float_bits_to_uint(data.x))),
        half2(unpackHalf2x16(float_bits_to_uint(data.y))))
}

def f16_pack(sh : half4) : float4 {
    return float4(
        uint_bits_to_float(packHalf2x16(float2(sh.xy))),
        uint_bits_to_float(packHalf2x16(float2(sh.zw))),
        0.0, 0.0)
}

[compute_shader(local_size_x=64, name="f16_spv"), marker(no_coverage)]
def f16_shader {
    let i = int(gl_GlobalInvocationID.x)
    let sh = f16_unpack(f16_data[i])
    f16_data[i] = f16_pack(sh)
}

// The blur step, in miniature: the filter math runs in 32-bit float
// (as the GLSL does), so the half4 widens on the way in and narrows
// on the way out -- one OpFConvert each way.
[compute_shader(local_size_x=64, name="f16_filt_spv"), marker(no_coverage)]
def f16_filt_shader {
    let i = int(gl_GlobalInvocationID.x)
    let sh = f16_unpack(f16_data[i])
    let blurred = lerp(float4(sh), float4(1.0, 2.0, 3.0, 4.0), 0.25)
    f16_data[i] = f16_pack(half4(blurred))
}

// The (vector, scalar) APPEND form, which is what GLSL writes as a
// pair of component stores into a Function f16vec3 (`dir.xy = ...;
// dir.z = ...`). A multi-component write swizzle has no daslang
// counterpart, so the port composes the vector whole -- and the point
// of the append ctor is that the lanes already ARE the component
// type, so it costs no convert of its own.
[compute_shader(local_size_x=64, name="f16_append_spv"), marker(no_coverage)]
def f16_append_shader {
    let i = int(gl_GlobalInvocationID.x)
    let data = f16_data[i]
    let xy = half2(unpackHalf2x16(float_bits_to_uint(data.x)))
    let z_w = half2(unpackHalf2x16(float_bits_to_uint(data.y)))
    let dir = half3(xy, z_w.x)                  // one OpCompositeConstruct, no convert
    let n = half3(normalize(float3(dir)))       // widen, normalize, narrow
    f16_data[i] = float4(float3(n), float(z_w.y))
}

// Closed 16-bit arithmetic: daslang's fp16 has it natively, so this
// must lower to OpFAdd / OpFMul on the 16-bit type -- no converts
// around either op. The scale is an fp16 LITERAL, which daslang folds
// to an ExprConstFloat16 leaf and the emitter lands on a %half
// OpConstant: spelling it `2.0` instead would make it a float lane
// the ctor has to narrow, costing four OpFConvert the glslang
// reference never spends.
[compute_shader(local_size_x=64, name="f16_arith_spv"), marker(no_coverage)]
def f16_arith_shader {
    let i = int(gl_GlobalInvocationID.x)
    let sh = f16_unpack(f16_data[i])
    // OpFMul on %v4half, against a splat %half constant
    let scaled = sh * half4(2.0h)
    let x = scaled.x + scaled.y                     // OpFAdd on %half
    f16_data[i] = float4(float(x), 0.0, 0.0, 0.0)
}

// The id OpTypeFloat 16 declares (0 if absent). Operands are
// [result-id, width], so the id we want sits at 0 -- which is why
// this cannot be spelled with op_has_operand, whose answer is a bool.
def private half_type_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeFloat) && n >= 2 && w[s + 1] == 16u) {
            id = w[s]
        }
    }
    return id
}

// The id of OpTypeVector <half_id> <lanes>. Operands are [result-id,
// component-type, count].
def private half_vec_type_id(words : array<uint>; half_id : uint; lanes : uint) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeVector) && n >= 3 && w[s + 1] == half_id
            && w[s + 2] == lanes) {
            id = w[s]
        }
    }
    return id
}

def private check_float16_surface(t : T?; words : array<uint>; what : string) {
    t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.Float16)),
        "{what}: OpCapability Float16")
    t |> success(op_has_operand(words, SpvOp.TypeFloat, 16u),
        "{what}: OpTypeFloat 16")
    // fp16 rides through a float4 as packed uints, so nothing 16-bit
    // reaches the interface block
    t |> success(
                 !op_has_operand(words, SpvOp.Capability,
                                 uint(SpvCapability.StorageBuffer16BitAccess)),
        "{what}: no 16-bit STORAGE capability -- the SSBO stays 32-bit")
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{what}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "half storage declares Float16 with the pack/unpack pair, filter math widens with one OpFConvert each way, and closed arithmetic stays native 16-bit.")]
def test_float16(t : T?) {
    t |> run("half4 storage: Float16 capability, the pack/unpack pair, and the half2 compose") <| @(t : T?) {
        var words <- clone_to_move(f16_spv)
        check_float16_surface(t, words, "storage form")
        t |> success(count_ext_op(words, GLSLstd450.UnpackHalf2x16) == 2, "two UnpackHalf2x16")
        t |> success(count_ext_op(words, GLSLstd450.PackHalf2x16) == 2, "two PackHalf2x16")
        // THE COMPOSE ASSERTION. Two narrowing converts (one per unpacked
        // float2 -> half2) and two widening ones (one per half2 -> float2 on
        // the way into the pack). A compose that round-tripped through float
        // would spend a fifth on the half4 itself.
        t |> success(count_op(words, SpvOp.FConvert) == 4,
            "four OpFConvert -- the half4(half2, half2) compose costs none")
        let hid = half_type_id(words)
        let vid = half_vec_type_id(words, hid, 4u)
        t |> success(hid != 0u && vid != 0u, "the %half and %v4half types are declared")
        t |> success(count_op_operand_at(words, SpvOp.CompositeConstruct, 0, vid) == 1,
            "one OpCompositeConstruct on %v4half -- the two half2 go in as vectors")
        delete words
    }
    t |> run("filter math widens to 32-bit float and narrows back, one OpFConvert each way") <| @(t : T?) {
        var words <- clone_to_move(f16_filt_spv)
        check_float16_surface(t, words, "filter form")
        // the storage form's four, plus float4(sh) widening and
        // half4(blurred) narrowing
        t |> success(count_op(words, SpvOp.FConvert) == 6,
                     "six OpFConvert (storage four + widen + narrow)")
        t |> success(count_ext_op(words, GLSLstd450.FMix) == 1,
                     "the blend is one FMix, in 32-bit float")
        delete words
    }
    t |> run("half3(half2, float16) appends without a convert of its own") <| @(t : T?) {
        var words <- clone_to_move(f16_append_spv)
        check_float16_surface(t, words, "append form")
        let hid = half_type_id(words)
        let vid = half_vec_type_id(words, hid, 3u)
        t |> success(hid != 0u && vid != 0u, "the %half and %v3half types are declared")
        t |> success(count_op_operand_at(words, SpvOp.CompositeConstruct, 0, vid) == 1,
            "one OpCompositeConstruct on %v3half -- the half2 and the half go in as they are")
        // two narrowing unpack converts, the widen for normalize, the narrow
        // back, and the two reads out (float3(n), float(z_w.y)). The append
        // itself adds none.
        t |> success(count_op(words, SpvOp.FConvert) == 6,
                     "six OpFConvert -- the append costs none")
        delete words
    }
    t |> run("closed arithmetic lowers NATIVELY to 16 bits, not through a widen/narrow sandwich") <| @(t : T?) {
        var words <- clone_to_move(f16_arith_spv)
        check_float16_surface(t, words, "arith form")
        let hid = half_type_id(words)
        let vid = half_vec_type_id(words, hid, 4u)
        t |> success(hid != 0u && vid != 0u, "the %half and %v4half types are declared")
        t |> success(count_op_of_type(words, SpvOp.FMul, vid) == 1, "one OpFMul on %v4half")
        t |> success(count_op_of_type(words, SpvOp.FAdd, hid) == 1, "one OpFAdd on %half")
        // nothing arithmetic leaked to 32 bits
        t |> success(count_op(words, SpvOp.FMul) == 1 && count_op(words, SpvOp.FAdd) == 1,
            "no arithmetic op widened to 32 bits")
        // the two narrowing unpack converts, plus the one float(x) read --
        // the arithmetic converts nothing
        t |> success(count_op(words, SpvOp.FConvert) == 3,
            "three OpFConvert -- the 16-bit add and multiply convert neither operand")
        delete words
    }
}

// 64-bit FLOAT gate: the scalar double rail (GLSL's double), and the
// ARITHMETIC on top of it.
//
// It needs exactly three things, and they are the three glslang
// emits: the TYPE      OpTypeFloat 64 behind OpCapability Float64 the
// CONVERT   double(x) / float(d) -> one OpFConvert, widening or
// narrowing; double(i) from an INT -> one OpConvertSToF, which is
// width-agnostic on both sides (no widen-first pair) the CONSTANT  a
// `2.0lf` literal -> an OpConstant on %double whose literal spans TWO
// words, low order FIRST. That pair is the one thing spirv-val cannot
// witness -- a swapped pair is still a structurally valid 64-bit
// constant, just a different number -- so the bits are asserted here
// or nowhere. There is NO double VECTOR, and daslang can never grow
// one: its value ABI is a single 16-byte vec4f (a static_assert in
// cast.h), so a 24- or 32-byte vector does not fit the register at
// all. A GLSL dvec3 therefore ports as three doubles, which costs
// nothing on the GPU -- doubles are scalar in hardware anyway -- and
// shows up only as a body-histogram delta against the glslang
// reference's vector ops.
//
// Nothing 64-bit reaches an interface block: scalar_class admits
// 32-bit leaves only, so a double member fails closed rather than
// mis-laying-out against std140 sizes that are hardcoded 4-byte. The
// double lives in registers, which is exactly what the terrain-noise
// closure needs of it.
//
// 64-bit INTs have their own rail (scalars, constants, SSBO elements,
// atomics): test_int64 above.
var @ssbo @binding = 0 f64_data : array<float>

// The terrain-noise read, in miniature: the SSBO stays 32-bit, so a
// double closure widens on the way in and narrows on the way out --
// one OpFConvert each way, which is what the GLSL spends too.
def f64_widen(x : float) : double => double(x)

[compute_shader(local_size_x=64, name="f64_spv"), marker(no_coverage)]
def f64_shader {
    let i = int(gl_GlobalInvocationID.x)
    let d = f64_widen(f64_data[i])
    f64_data[i] = float(d)
}

// Closed 64-bit arithmetic: daslang has it natively, so this must
// lower to OpFMul / OpFAdd on the 64-bit type -- no converts around
// either op. Both scales are double LITERALS, which daslang folds to
// an ExprConstDouble leaf and the emitter lands on a %double
// OpConstant.
[compute_shader(local_size_x=64, name="f64_arith_spv"), marker(no_coverage)]
def f64_arith_shader {
    let i = int(gl_GlobalInvocationID.x)
    let d = double(f64_data[i]) * 2.0lf     // OpFMul on %double, against a %double constant
    let s = d + 1.5lf                       // OpFAdd on %double
    f64_data[i] = float(s)
}

// The sphere arm's other widen: an INT straight to double (its plane
// offsets are integer). OpConvertSToF is width-agnostic on its
// operand AND its result, so this is ONE instruction -- not a
// widening pair around a 32-bit convert.
var @ssbo @binding = 1 f64_idata : array<int>

[compute_shader(local_size_x=64, name="f64_int_spv"), marker(no_coverage)]
def f64_int_shader {
    let i = int(gl_GlobalInvocationID.x)
    let d = double(f64_idata[i]) * 2.0lf    // int -> double, then closed 64-bit arithmetic
    f64_data[i] = float(d)
}

// The dvec3 spelling the terrain port must use, since daslang can
// never have a double3: three doubles, each lane its own op. The GPU
// does exactly this with a dvec3 regardless.
[compute_shader(local_size_x=64, name="f64_vec3_spv"), marker(no_coverage)]
def f64_vec3_shader {
    let i = int(gl_GlobalInvocationID.x) * 3
    let px = double(f64_data[i]) * 2.0lf
    let py = double(f64_data[i + 1]) * 2.0lf
    let pz = double(f64_data[i + 2]) * 2.0lf
    f64_data[i] = float(px + py + pz)
}

// The id OpTypeFloat 64 declares (0 if absent). Operands are
// [result-id, width], so the id we want sits at 0 -- which is why
// this cannot be spelled with op_has_operand, whose answer is a bool.
def private double_type_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeFloat) && n >= 2 && w[s + 1] == 64u) {
            id = w[s]
        }
    }
    return id
}

def private check_float64_surface(t : T?; words : array<uint>; what : string) {
    t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.Float64)),
        "{what}: OpCapability Float64")
    t |> success(op_has_operand(words, SpvOp.TypeFloat, 64u),
        "{what}: OpTypeFloat 64")
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{what}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "double storage declares Float64 with one OpFConvert each way, closed arithmetic stays native 64-bit with two-word literals, and an int widens straight to double.")]
def test_float64(t : T?) {
    t |> run("double storage: Float64 capability, and one OpFConvert each way") <| @(t : T?) {
        var words <- clone_to_move(f64_spv)
        check_float64_surface(t, words, "storage form")
        // the widen in, the narrow out -- and nothing else
        t |> success(count_op(words, SpvOp.FConvert) == 2, "two OpFConvert -- widen in, narrow out")
        let did = double_type_id(words)
        t |> success(did != 0u, "the %double type is declared")
        // no double vector exists to declare: daslang's 16-byte value ABI
        // cannot hold one
        t |> success(!op_has_operand(words, SpvOp.TypeVector, did),
            "no OpTypeVector over %double -- a dvec3 ports as three doubles")
        delete words
    }
    t |> run("closed arithmetic lowers NATIVELY to 64 bits, and its literals are two-word constants") <| @(t : T?) {
        var words <- clone_to_move(f64_arith_spv)
        check_float64_surface(t, words, "arith form")
        let did = double_type_id(words)
        t |> success(did != 0u, "the %double type is declared")
        t |> success(count_op_of_type(words, SpvOp.FMul, did) == 1, "one OpFMul on %double")
        t |> success(count_op_of_type(words, SpvOp.FAdd, did) == 1, "one OpFAdd on %double")
        // nothing arithmetic leaked to 32 bits
        t |> success(count_op(words, SpvOp.FMul) == 1 && count_op(words, SpvOp.FAdd) == 1,
            "no arithmetic op narrowed to 32 bits")
        // THE WORD-ORDER ASSERTION, which spirv-val cannot make: 2.0lf is
        // 0x4000000000000000 and 1.5lf is 0x3ff8000000000000, so each lands
        // as low word 0 followed by its high word. Emitting the pair
        // high-first would validate cleanly and silently mean a different
        // number.
        t |> success(has_const64(words, did, 0u, 0x40000000u),
                     "2.0lf is OpConstant %double 0, 0x40000000")
        t |> success(has_const64(words, did, 0u, 0x3ff80000u),
                     "1.5lf is OpConstant %double 0, 0x3ff80000")
        // the widen in and the narrow out; the 64-bit ops convert neither
        // operand
        t |> success(count_op(words, SpvOp.FConvert) == 2,
            "two OpFConvert -- the 64-bit add and multiply convert nothing")
        delete words
    }
    t |> run("an int widens STRAIGHT to double: one OpConvertSToF, no widening pair") <| @(t : T?) {
        var words <- clone_to_move(f64_int_spv)
        check_float64_surface(t, words, "int-to-double form")
        let did = double_type_id(words)
        t |> success(did != 0u, "the %double type is declared")
        // THE assertion: the int reaches %double in ONE instruction. A
        // widen-first lowering would spend an OpConvertSToF to 32-bit float
        // plus an OpFConvert up, which is two ops for one conversion and a
        // rounding step the direct form does not take.
        t |> success(count_op_of_type(words, SpvOp.ConvertSToF, did) == 1,
            "one OpConvertSToF straight to %double")
        t |> success(count_op(words, SpvOp.FConvert) == 1,
            "one OpFConvert -- the narrow OUT only, nothing widening on the way in")
        t |> success(count_op_of_type(words, SpvOp.FMul, did) == 1, "one OpFMul on %double")
        delete words
    }
    t |> run("a dvec3 spelled as three doubles is three scalar ops, no vector type") <| @(t : T?) {
        var words <- clone_to_move(f64_vec3_spv)
        check_float64_surface(t, words, "vec3 form")
        let did = double_type_id(words)
        t |> success(did != 0u, "the %double type is declared")
        t |> success(!op_has_operand(words, SpvOp.TypeVector, did), "no OpTypeVector over %double")
        t |> success(count_op_of_type(words, SpvOp.FMul, did) == 3,
                     "one OpFMul on %double per lane")
        t |> success(count_op_of_type(words, SpvOp.FAdd, did) == 2,
                     "the two adds folding the lanes")
        // three widens in, one narrow out
        t |> success(count_op(words, SpvOp.FConvert) == 4,
                     "four OpFConvert -- three lanes in, one out")
        delete words
    }
}

// 8/16-bit SSBO storage (std430): the block_q8_0-mirror struct lays
// out tight (runtime ArrayStride 34, member Offsets 0/2, qs element
// stride 1), half4 elements stride 8, the StorageBuffer8/16BitAccess
// capabilities + the SPV_KHR_8bit_storage extension are pulled, and
// unpack8/pack32 lower to OpBitcast with OpSConvert sign-extended
// widening. @uniform / @push_constant sub-32 members stay fail-closed
// (std140) — pinned by _fc_lattice_ubo / _fc_lattice_push in
// test_fail_closed.das.
[test(description = "An 8/16-bit std430 block lays out tight, pulls the storage capabilities and extension, and its small-int loads and stores narrow and widen correctly.")]
def test_storage_8_16(t : T?) {  // nolint:STYLE038 - one arm per storage width
    t |> run("block_q8_0 std430 layout + storage caps + unpack8/pack32; spirv-val clean") <| @(t : T?) {
        var words <- q8blk_words()
        // std430 block_q8_0: 34-byte block stride, d at offset 0, qs at
        // offset 2, 1-byte elements
        t |> success(
                     op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride),
                                         34u), "runtime ArrayStride 34")
        t |> success(
                     op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride),
                                         1u), "qs ArrayStride 1")
        t |> success(
                     op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride),
                                         8u), "half4 ArrayStride 8")
        t |> success(
                     op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset),
                                         2u), "qs at Offset 2 (d is 2 bytes)")
        // storage caps + the 8-bit extension (16-bit storage is cap-only at
        // SPIR-V 1.3)
        t |> success(
                     op_has_operand_at(words, SpvOp.Capability, 0,
                                       uint(SpvCapability.StorageBuffer8BitAccess)),
            "StorageBuffer8BitAccess")
        t |> success(
                     op_has_operand_at(words, SpvOp.Capability, 0,
                                       uint(SpvCapability.StorageBuffer16BitAccess)),
            "StorageBuffer16BitAccess")
        t |> equal(count_op(words, SpvOp.Extension), 1)
        // unpack8 + pack32 -> OpBitcast; int4(b4) -> sign-extending
        // OpSConvert; f16 loads -> OpFConvert
        t |> success(count_op(words, SpvOp.Bitcast) >= 2, "unpack8 + pack32 Bitcasts present")
        t |> success(has_op(words, SpvOp.SConvert), "sign-extended widen present")
        t |> success(has_op(words, SpvOp.FConvert), "f16 -> f32 conversion present")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
    t |> run("small-int STORE half: narrowing casts + 8/16-bit stores; spirv-val clean") <| @(t : T?) {
        var words <- q8store_words()
        t |> success(count_op(words, SpvOp.SConvert) >= 3,
                     "int8()/int16() narrow via OpSConvert - 3 in q8store_kernel")
        t |> success(has_op(words, SpvOp.UConvert), "uint16(uint) narrows via OpUConvert")
        t |> success(
                     op_has_operand_at(words, SpvOp.Capability, 0,
                                       uint(SpvCapability.StorageBuffer8BitAccess)),
            "StorageBuffer8BitAccess")
        t |> success(
                     op_has_operand_at(words, SpvOp.Capability, 0,
                                       uint(SpvCapability.StorageBuffer16BitAccess)),
            "StorageBuffer16BitAccess")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
    t |> run("unpack8 on 16-bit lanes: int16 -> byte2 and uint16 -> ubyte2 bitcasts with a runtime lane select; spirv-val clean") <| @(t : T?) {
        var words <- unpack16_words()
        t |> success(count_op(words, SpvOp.Bitcast) >= 2,
                     "the two 16-bit -> 8-bit x2 bitcasts are present")
        t |> success(has_op(words, SpvOp.VectorExtractDynamic),
                     "the runtime lane select is a dynamic extract")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.Int16)),
                     "Int16")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.Int8)),
                     "Int8")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
