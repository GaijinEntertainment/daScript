options gen2
options indenting = 4

// 64-bit FLOAT gate: the scalar double rail (GLSL's double), and the ARITHMETIC on top of it.
//
// It needs exactly three things, and they are the three glslang emits:
//   the TYPE      OpTypeFloat 64 behind OpCapability Float64
//   the CONVERT   double(x) / float(d) -> one OpFConvert, widening or narrowing; double(i) from an
//                 INT -> one OpConvertSToF, which is width-agnostic on both sides (no widen-first pair)
//   the CONSTANT  a `2.0lf` literal -> an OpConstant on %double whose literal spans TWO words, low
//                 order FIRST. That pair is the one thing spirv-val cannot witness -- a swapped pair
//                 is still a structurally valid 64-bit constant, just a different number -- so the
//                 bits are asserted here or nowhere.
// There is NO double VECTOR, and daslang can never grow one: its value ABI is a single 16-byte vec4f
// (a static_assert in cast.h), so a 24- or 32-byte vector does not fit the register at all. A GLSL
// dvec3 therefore ports as three doubles, which costs nothing on the GPU -- doubles are scalar in
// hardware anyway -- and shows up only as a body-histogram delta against the glslang reference's vector ops.
//
// Nothing 64-bit reaches an interface block: scalar_class admits 32-bit leaves only, so a double
// member fails closed rather than mis-laying-out against std140 sizes that are hardcoded 4-byte. The
// double lives in registers, which is exactly what the terrain-noise closure needs of it.
//
// A 64-bit INT stays unsupported on every rail (arith_width_ok admits width 64 only for the float
// class); tests/spirv/_fail_closed pins that.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader          // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis             // count_op, op_has_operand, count_op_operand_at
require spirv/spirv_grammar         // SpvOp, SpvCapability

var @ssbo @binding = 0 f64_data : array<float>

// The terrain-noise read, in miniature: the SSBO stays 32-bit, so a double closure widens on the way
// in and narrows on the way out -- one OpFConvert each way, which is what the GLSL spends too.
def f64_widen(x : float) : double => double(x)

[compute_shader(local_size_x=64, name="f64_spv"), marker(no_coverage)]
def f64_shader {
    let i = int(gl_GlobalInvocationID.x)
    let d = f64_widen(f64_data[i])
    f64_data[i] = float(d)
}

// Closed 64-bit arithmetic: daslang has it natively, so this must lower to OpFMul / OpFAdd on the
// 64-bit type -- no converts around either op. Both scales are double LITERALS, which daslang folds to
// an ExprConstDouble leaf and the emitter lands on a %double OpConstant.
[compute_shader(local_size_x=64, name="f64_arith_spv"), marker(no_coverage)]
def f64_arith_shader {
    let i = int(gl_GlobalInvocationID.x)
    let d = double(f64_data[i]) * 2.0lf     // OpFMul on %double, against a %double constant
    let s = d + 1.5lf                       // OpFAdd on %double
    f64_data[i] = float(s)
}

// The sphere arm's other widen: an INT straight to double (its plane offsets are integer). OpConvertSToF
// is width-agnostic on its operand AND its result, so this is ONE instruction -- not a widening pair
// around a 32-bit convert.
var @ssbo @binding = 1 f64_idata : array<int>

[compute_shader(local_size_x=64, name="f64_int_spv"), marker(no_coverage)]
def f64_int_shader {
    let i = int(gl_GlobalInvocationID.x)
    let d = double(f64_idata[i]) * 2.0lf    // int -> double, then closed 64-bit arithmetic
    f64_data[i] = float(d)
}

// The dvec3 spelling the terrain port must use, since daslang can never have a double3: three
// doubles, each lane its own op. The GPU does exactly this with a dvec3 regardless.
[compute_shader(local_size_x=64, name="f64_vec3_spv"), marker(no_coverage)]
def f64_vec3_shader {
    let i = int(gl_GlobalInvocationID.x) * 3
    let px = double(f64_data[i]) * 2.0lf
    let py = double(f64_data[i + 1]) * 2.0lf
    let pz = double(f64_data[i + 2]) * 2.0lf
    f64_data[i] = float(px + py + pz)
}

// The id OpTypeFloat 64 declares (0 if absent). Operands are [result-id, width], so the id we want
// sits at 0 -- which is why this cannot be spelled with op_has_operand, whose answer is a bool.
def private double_type_id(words : array<uint>) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeFloat) && n >= 2 && w[s + 1] == 64u) {
            id = w[s]
        }
    }
    return id
}

// Is there an OpConstant on `type_id` whose two literal words are exactly lo then hi? A 64-bit
// OpConstant's operands are [result-type, result-id, low word, high word] (SPIR-V 2.2.1), so a
// 32-bit constant has n == 3 and never matches.
def private has_const64(words : array<uint>; type_id, lo, hi : uint) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.Constant) && n == 4 && w[s] == type_id &&
                w[s + 2] == lo && w[s + 3] == hi) {
            found = true
        }
    }
    return found
}

// How many `op` instructions produce a value of type `type_id`? An arithmetic op's result type is
// operand 0, so this is what separates a native 64-bit lowering from a 32-bit one.
def private count_op_of_type(words : array<uint>; op : SpvOp; type_id : uint) : int =>
    count_op_operand_at(words, op, 0, type_id)

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

[test]
def test_float64(t : T?) {
    t |> run("double storage: Float64 capability, and one OpFConvert each way") <| @(t : T?) {
        var words <- clone_to_move(f64_spv)
        check_float64_surface(t, words, "storage form")
        // the widen in, the narrow out -- and nothing else
        t |> success(count_op(words, SpvOp.FConvert) == 2, "two OpFConvert -- widen in, narrow out")
        let did = double_type_id(words)
        t |> success(did != 0u, "the %double type is declared")
        // no double vector exists to declare: daslang's 16-byte value ABI cannot hold one
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
        // THE WORD-ORDER ASSERTION, which spirv-val cannot make: 2.0lf is 0x4000000000000000 and
        // 1.5lf is 0x3ff8000000000000, so each lands as low word 0 followed by its high word. Emitting
        // the pair high-first would validate cleanly and silently mean a different number.
        t |> success(has_const64(words, did, 0u, 0x40000000u), "2.0lf is OpConstant %double 0, 0x40000000")
        t |> success(has_const64(words, did, 0u, 0x3ff80000u), "1.5lf is OpConstant %double 0, 0x3ff80000")
        // the widen in and the narrow out; the 64-bit ops convert neither operand
        t |> success(count_op(words, SpvOp.FConvert) == 2,
            "two OpFConvert -- the 64-bit add and multiply convert nothing")
        delete words
    }
    t |> run("an int widens STRAIGHT to double: one OpConvertSToF, no widening pair") <| @(t : T?) {
        var words <- clone_to_move(f64_int_spv)
        check_float64_surface(t, words, "int-to-double form")
        let did = double_type_id(words)
        t |> success(did != 0u, "the %double type is declared")
        // THE assertion: the int reaches %double in ONE instruction. A widen-first lowering would
        // spend an OpConvertSToF to 32-bit float plus an OpFConvert up, which is two ops for one
        // conversion and a rounding step the direct form does not take.
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
        t |> success(count_op_of_type(words, SpvOp.FMul, did) == 3, "one OpFMul on %double per lane")
        t |> success(count_op_of_type(words, SpvOp.FAdd, did) == 2, "the two adds folding the lanes")
        // three widens in, one narrow out
        t |> success(count_op(words, SpvOp.FConvert) == 4, "four OpFConvert -- three lanes in, one out")
        delete words
    }
}
