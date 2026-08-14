options gen2
options indenting = 4

// 16-bit integer gate: the storage + convert rail (GLSL's int16_t / i16vec2), and the ARITHMETIC on
// top of it.
//
// The storage + convert half needs exactly three things, and they are the three glslang emits:
//   the TYPE      OpTypeInt 16 1 (+ OpTypeVector) behind OpCapability Int16
//   the CTOR      short2(int, int) -> one OpSConvert per lane, then OpCompositeConstruct
//   the READ      int(v.x) -> OpSConvert back up to 32 bits
// A narrowing SConvert truncates and a widening one sign-extends, which is what makes a das body
// bit-identical to the GLSL it ports: 0xabcd packs to -21555 and reads back as -21555.
//
// The ARITHMETIC half is closed +/- over the family (daslib/shader_lingua_franca declares the
// operators daslang itself lacks), lowering to a NATIVE OpIAdd / OpISub / OpSNegate on OpTypeInt 16
// -- not to a widen / 32-bit-op / narrow sandwich. That distinction is the whole point, and counting
// opcodes cannot see it, so every arithmetic assertion below pins the op's RESULT TYPE to the
// %short / %v2short id: a 32-bit lowering would carry the %int id there and fail. spirv-val is the
// independent witness (it rejects an OpIAdd whose operand width disagrees with its result).
//
// What GLSL widens, das must widen too: a compare against an int literal (`p.x < 0`) has no 16-bit
// form on either rail, so it reads through int() and lowers to OpSConvert + a 32-bit compare, which
// is exactly what glslang does with the implicit conversion.
//
// Both spellings of a GLSL helper are gated. `i16_unpack` RETURNS the pair (the natural das form,
// and the only one available for a `let`, since an uninitialized local is rejected); `i16_unpack_out`
// takes it by reference, which is what GLSL's `out i16vec2` is -- a Function-storage pointer to a
// v2short. spirv-val is what proves the pointer form is well-formed.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader          // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis             // count_op, op_has_operand, op_has_operand_pair
require spirv/spirv_grammar         // SpvOp, SpvCapability

var @ssbo @binding = 0 i16_packed : array<int>
var @ssbo @binding = 1 i16_pair : array<int2>
var @ssbo @binding = 2 i16_quad : array<int4>

def i16_unpack(packed : int) : short2 {
    return short2(packed >> 16, packed & 0xffff)
}

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

// The cube-map wrap step, in miniature: the exact arithmetic shapes a cube_map_wrap port needs. The
// scalar `+` / `-` / unary `-` are 16-bit; the bounds test against an int literal is not, and reads
// through int() to say so.
def i16_wrap_step(p : short2; s : int16) : short2 {
    if (int(p.x) < 0) return short2(p.x + s, -p.y)          // vs an int literal: widened, as in GLSL
    if (p.x >= s) return short2(p.x - s, -p.y - s)          // vs another int16: native 16-bit compare
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

// The unsigned half of the family, in the shape a material-weight sort needs: a u16vec4 whose lanes
// are shuffled by a uint16 comparison, and an int16 max. Every 16-bit value here re-enters a vector
// through the ushort4 lane ctor, so no lane ever round-trips through int.
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

// The material sort's spelling: an INT straight to uint16, changing signedness AND width at once. That
// is ONE OpUConvert, because a NARROWING convert keeps the low bits whatever the source's signedness --
// exactly the truncation daslang's narrowing ctor and GLSL's uint16_t(int) both mean. (The u16 fixture
// above spells the same narrowing through uint(x); neither round-trip costs an extra instruction.) The
// opposite direction is NOT symmetric and stays refused: C sign-extends an int16 into a uint32 where
// OpUConvert would zero-extend.
[compute_shader(local_size_x=64, name="u16_narrow_spv"), marker(no_coverage)]
def u16_narrow_shader {
    let i = int(gl_GlobalInvocationID.x)
    let p = i16_quad[i]
    let a = ushort4(uint16(p.x), uint16(p.y), uint16(p.z), uint16(p.w))
    i16_quad[i] = int4(int(uint(a.x)), int(uint(a.y)), int(uint(a.z)), int(uint(a.w)))
}

// The id OpTypeInt 16 0 declares -- the UNSIGNED 16-bit type, distinct from the signed one below.
def private ushort_type_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeInt) && n >= 3 && w[s + 1] == 16u && w[s + 2] == 0u) {
            id = w[s]
        }
    }
    return id
}

// How many OpExtInst carry `ext` as their ext-op selector? Operands are [result-type, result-id,
// set-id, ext-op, args...], so the selector sits at index 3.
def private count_ext_op(words : array<uint>; ext : GLSLstd450) : int {
    return count_op_operand_at(words, SpvOp.ExtInst, 3, uint(ext))
}

// The id OpTypeInt 16 1 declares (0 if absent). Operand layout is [result-id, width, signedness],
// so the width/signedness pair sits at 1..2 and the id we want at 0 -- which is why this cannot be
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

// The id of OpTypeVector <short_id> <lanes>. Operands are [result-id, component-type, count].
def private short_vec_type_id(words : array<uint>; short_id : uint; lanes : uint) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeVector) && n >= 3 && w[s + 1] == short_id && w[s + 2] == lanes) {
            id = w[s]
        }
    }
    return id
}

// How many `op` instructions produce a value of type `type_id`? An arithmetic op's result type is
// operand 0, so this is what separates a native 16-bit lowering from a widened 32-bit one.
def private count_op_of_type(words : array<uint>; op : SpvOp; type_id : uint) : int {
    return count_op_operand_at(words, op, 0, type_id)
}

// `signedness` is which 16-bit type the fixture is expected to declare: a fixture that only ever spells
// uint16 has no signed one to find.
def private check_int16_surface(t : T?; words : array<uint>; what : string; signedness : uint = 1u) {
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

[test]
def test_int16(t : T?) { // nolint:STYLE038 - flat list of opcode/capability assertions
    t |> run("short2 by value: Int16 capability, narrowing ctor, widening read") <| @(t : T?) {
        var words <- clone_to_move(i16_spv)
        check_int16_surface(t, words, "value form")
        // two narrowing (the ctor's lanes) + two widening (the int(tc.x) / int(tc.y) reads)
        t |> success(count_op(words, SpvOp.SConvert) == 4, "four OpSConvert (2 narrow, 2 widen)")
        t |> success(count_op(words, SpvOp.CompositeConstruct) >= 1, "the pair is composed")
        delete words
    }
    t |> run("short2 by reference: GLSL's `out i16vec2` is a Function pointer to a v2short") <| @(t : T?) {
        var words <- clone_to_move(i16_out_spv)
        check_int16_surface(t, words, "out form")
        // the zero-init's two lanes on top of the value form's four
        t |> success(count_op(words, SpvOp.SConvert) == 6, "six OpSConvert (zero-init, ctor, reads)")
        delete words
    }
    t |> run("an int narrows STRAIGHT to uint16: one OpUConvert, signedness and width at once") <| @(t : T?) {
        var words <- clone_to_move(u16_narrow_spv)
        check_int16_surface(t, words, "int-to-uint16 form", 0u)
        let uid = ushort_type_id(words)
        t |> success(uid != 0u, "the %ushort type is declared")
        // THE assertion: four lanes, four converts. A bitcast-then-narrow lowering would spend eight
        // ops, and picking the opcode by the SOURCE's signedness would emit OpSConvert -- whose result
        // must be a SIGNED int type, so it would not even be the %ushort the ctor asked for.
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
        // `p + short2(1, -1)` -- one whole-vector add, on the v2short type and not on v2int
        t |> success(count_op_of_type(words, SpvOp.IAdd, vid) == 1, "one OpIAdd on %v2short")
        // i16_wrap_step: `p.x + s` and `-p.y - s` are the scalar 16-bit add / subtract...
        t |> success(count_op_of_type(words, SpvOp.IAdd, sid) == 1, "one OpIAdd on %short")
        t |> success(count_op_of_type(words, SpvOp.ISub, sid) == 2, "two OpISub on %short")
        // ...and `-p.y` (twice) is the 16-bit negate
        t |> success(count_op_of_type(words, SpvOp.SNegate, sid) == 2, "two OpSNegate on %short")
        // nothing arithmetic leaked to 32 bits
        t |> success(count_op(words, SpvOp.IAdd) == 2 && count_op(words, SpvOp.ISub) == 2
            && count_op(words, SpvOp.SNegate) == 2, "no arithmetic op widened to 32 bits")
        // `p.x >= s` compares two int16 NATIVELY, and the SConvert census is what proves it: a widened
        // compare would have to convert BOTH operands, pushing the count to 9. The seven are the two
        // narrowing ctor pairs (i16_unpack's, and short2(1, -1)'s int lanes), the one widening read the
        // `int(p.x) < 0` literal test forces -- a compare against an int literal has no 16-bit form on
        // either rail, and glslang's implicit widening emits the same convert -- and the two int(q.*)
        // reads. int16(64) costs none: daslang folds it to a %short constant, as glslang folds its
        // int-typed i16vec3 lane.
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
        // THE SIGNEDNESS IS THE POINT: a uint16 pair must compare UNSIGNED. Reading the pair through
        // int() (the only spelling available before the operators shipped) would give
        // OpSGreaterThanEqual and order 0xffff BELOW 0 -- valid SPIR-V, and wrong.
        t |> success(count_op(words, SpvOp.UGreaterThanEqual) == 1, "u16: one OpUGreaterThanEqual")
        t |> success(count_op(words, SpvOp.SGreaterThanEqual) == 0,
            "u16: the unsigned compare did NOT lower to the signed opcode")
        // the ushort4 shuffle composes from lanes that already ARE %ushort, so it is one bare
        // OpCompositeConstruct with no convert around it; going through int would spend 8
        t |> success(count_op_operand_at(words, SpvOp.CompositeConstruct, 0, v4id) == 2,
            "u16: both ushort4 composes are on %v4ushort")
        // max(int16, int16) is OpExtInst SMax whose RESULT TYPE is %short -- counting the opcode alone
        // cannot tell a native 16-bit max from one widened to %int and narrowed back
        t |> success(count_op_of_type(words, SpvOp.ExtInst, sid) == 1, "u16: one OpExtInst on %short")
        t |> success(count_ext_op(words, GLSLstd450.SMax) == 1, "u16: that ext-op is SMax")
        check_int16_surface(t, words, "u16")
        delete words
    }
}
