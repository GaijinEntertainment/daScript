options gen2
options indenting = 4

// 16-bit FLOAT gate: the storage + convert rail (GLSL's float16_t / f16vec4), and the ARITHMETIC on
// top of it.
//
// The storage + convert half needs exactly three things, and they are the three glslang emits:
//   the TYPE      OpTypeFloat 16 (+ OpTypeVector) behind OpCapability Float16
//   the CONVERT   half4(vec4) / vec4(half4) -> one OpFConvert, narrowing (round to nearest even) or
//                 widening -- a whole-vector op, not one per lane
//   the PACK      packHalf2x16 / unpackHalf2x16 -> the GLSL.std.450 ext-insts. Both sides are FLOAT2,
//                 not half2: the pack is where a float becomes 16 bits, so a shader holding a half2
//                 converts on the way in, which is the OpFConvert glslang spends there too.
// Unlike the 16-bit INTS, whose storage carries a 16-bit SSBO member, fp16 here lives only in
// registers: an f16vec4 is moved through a vec4 as two packed uints, so NO 16-bit storage capability
// is declared and the interface block stays 32-bit. That is exactly how a renderer packing SH
// coefficients into a float4 buffer carries them.
//
// The COMPOSE is the one thing daslang itself lacks (daslib/shader_lingua_franca declares it): a
// half4 built from two half2. It matters because a shader unpacking two uints holds two half2 and
// must re-enter a half4 WITHOUT widening back to float and narrowing all four lanes again. The
// constituents already ARE the component type, so the assertion below is that exactly TWO OpFConvert
// serve the pair -- a third would mean the compose round-tripped through 32 bits.
//
// The ARITHMETIC half is closed over the family (daslang's fp16 promotes-and-rounds per op, which is
// bit-identical to native fp16), lowering to a NATIVE OpFAdd / OpFMul on OpTypeFloat 16 rather than a
// widen / 32-bit-op / narrow sandwich. Counting opcodes cannot see that distinction, so every
// arithmetic assertion pins the op's RESULT TYPE to the %half / %v4half id, exactly as the int16 gate
// pins %short. spirv-val is the independent witness.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader          // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis             // count_op, op_has_operand, op_has_operand_pair
require spirv/spirv_grammar         // SpvOp, SpvCapability
require daslib/shader_lingua_franca // packHalf2x16 / unpackHalf2x16, half4(half2, half2)
require daslib/math_bits            // float_bits_to_uint / uint_bits_to_float
require math                        // lerp

var @ssbo @binding = 0 f16_data : array<float4>

// A renderer's SH-coefficient read, in miniature: four SH coefficients ride in a float4 as two packed
// uints per half, so the unpack is bitcast -> UnpackHalf2x16 -> narrow, and the compose is the
// half4(half2, half2) daslang lacks.
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

// The blur step, in miniature: the filter math runs in 32-bit float (as the GLSL does), so the half4
// widens on the way in and narrows on the way out -- one OpFConvert each way.
[compute_shader(local_size_x=64, name="f16_filt_spv"), marker(no_coverage)]
def f16_filt_shader {
    let i = int(gl_GlobalInvocationID.x)
    let sh = f16_unpack(f16_data[i])
    let blurred = lerp(float4(sh), float4(1.0, 2.0, 3.0, 4.0), 0.25)
    f16_data[i] = f16_pack(half4(blurred))
}

// The (vector, scalar) APPEND form, which is what GLSL writes as a pair of component stores into a
// Function f16vec3 (`dir.xy = ...; dir.z = ...`). A multi-component write swizzle has no daslang
// counterpart, so the port composes the vector whole -- and the point of the append ctor is that the
// lanes already ARE the component type, so it costs no convert of its own.
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

// Closed 16-bit arithmetic: daslang's fp16 has it natively, so this must lower to OpFAdd / OpFMul on
// the 16-bit type -- no converts around either op. The scale is an fp16 LITERAL, which daslang folds
// to an ExprConstFloat16 leaf and the emitter lands on a %half OpConstant: spelling it `2.0` instead
// would make it a float lane the ctor has to narrow, costing four OpFConvert the glslang reference never spends.
[compute_shader(local_size_x=64, name="f16_arith_spv"), marker(no_coverage)]
def f16_arith_shader {
    let i = int(gl_GlobalInvocationID.x)
    let sh = f16_unpack(f16_data[i])
    let scaled = sh * half4(2.0h)                   // OpFMul on %v4half, against a splat %half constant
    let x = scaled.x + scaled.y                     // OpFAdd on %half
    f16_data[i] = float4(float(x), 0.0, 0.0, 0.0)
}

// The id OpTypeFloat 16 declares (0 if absent). Operands are [result-id, width], so the id we want
// sits at 0 -- which is why this cannot be spelled with op_has_operand, whose answer is a bool.
def private half_type_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeFloat) && n >= 2 && w[s + 1] == 16u) {
            id = w[s]
        }
    }
    return id
}

// The id of OpTypeVector <half_id> <lanes>. Operands are [result-id, component-type, count].
def private half_vec_type_id(words : array<uint>; half_id : uint; lanes : uint) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeVector) && n >= 3 && w[s + 1] == half_id && w[s + 2] == lanes) {
            id = w[s]
        }
    }
    return id
}

// How many `op` instructions produce a value of type `type_id`? An arithmetic op's result type is
// operand 0, so this is what separates a native 16-bit lowering from a widened 32-bit one.
def private count_op_of_type(words : array<uint>; op : SpvOp; type_id : uint) : int =>
    count_op_operand_at(words, op, 0, type_id)

// How many OpExtInst carry `ext` as their ext-op selector? Operands are [result-type, result-id,
// set-id, ext-op, args...], so the selector sits at index 3.
def private count_ext_op(words : array<uint>; ext : GLSLstd450) : int =>
    count_op_operand_at(words, SpvOp.ExtInst, 3, uint(ext))

def private check_float16_surface(t : T?; words : array<uint>; what : string) {
    t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.Float16)),
        "{what}: OpCapability Float16")
    t |> success(op_has_operand(words, SpvOp.TypeFloat, 16u),
        "{what}: OpTypeFloat 16")
    // fp16 rides through a float4 as packed uints, so nothing 16-bit reaches the interface block
    t |> success(!op_has_operand(words, SpvOp.Capability, uint(SpvCapability.StorageBuffer16BitAccess)),
        "{what}: no 16-bit STORAGE capability -- the SSBO stays 32-bit")
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{what}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_float16(t : T?) {
    t |> run("half4 storage: Float16 capability, the pack/unpack pair, and the half2 compose") <| @(t : T?) {
        var words <- clone_to_move(f16_spv)
        check_float16_surface(t, words, "storage form")
        t |> success(count_ext_op(words, GLSLstd450.UnpackHalf2x16) == 2, "two UnpackHalf2x16")
        t |> success(count_ext_op(words, GLSLstd450.PackHalf2x16) == 2, "two PackHalf2x16")
        // THE COMPOSE ASSERTION. Two narrowing converts (one per unpacked float2 -> half2) and two
        // widening ones (one per half2 -> float2 on the way into the pack). A compose that
        // round-tripped through float would spend a fifth on the half4 itself.
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
        // the storage form's four, plus float4(sh) widening and half4(blurred) narrowing
        t |> success(count_op(words, SpvOp.FConvert) == 6, "six OpFConvert (storage four + widen + narrow)")
        t |> success(count_ext_op(words, GLSLstd450.FMix) == 1, "the blend is one FMix, in 32-bit float")
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
        // two narrowing unpack converts, the widen for normalize, the narrow back, and the two
        // reads out (float3(n), float(z_w.y)). The append itself adds none.
        t |> success(count_op(words, SpvOp.FConvert) == 6, "six OpFConvert -- the append costs none")
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
        // the two narrowing unpack converts, plus the one float(x) read -- the arithmetic converts nothing
        t |> success(count_op(words, SpvOp.FConvert) == 3,
            "three OpFConvert -- the 16-bit add and multiply convert neither operand")
        delete words
    }
}
