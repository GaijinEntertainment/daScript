options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_shader
require spirv/spirv_builtins public
require daslib/math_bits
require spirv/spirv_builder
require spirv/spirv_types
require daslib/ast

// Phase-2 arithmetic gate: every binary/unary scalar op the emitter supports is present in the
// emitted blob (opcode-shape), and each whole module passes spirv-val. Operands are loaded SSBO
// values, so these isolate the arithmetic opcode selection (signed/unsigned/float) from constants
// and conversions (which arrive with control flow).
def private check_ops(t : T?; words : array<uint>; lbl : string; ops : array<SpvOp>) {
    for (op in ops) {
        t |> success(has_op(words, op), "{lbl}: emits {op}")
    }
}

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "uint arithmetic lowers to IAdd, ISub, IMul, UDiv and UMod.")]
def test_uint_arith(t : T?) {
    t |> run("uint arithmetic: IAdd/ISub/IMul/UDiv/UMod") <| @(t : T?) {
        var words <- uarith_words()
        check_ops(t, words, "uarith", [SpvOp.IAdd, SpvOp.ISub, SpvOp.IMul, SpvOp.UDiv, SpvOp.UMod])
        validate(t, words, "uarith")
        delete words
    }
}

[test(description = "int arithmetic lowers to SDiv, SRem, SNegate and Not.")]
def test_int_arith(t : T?) {
    t |> run("int arithmetic: SDiv/SRem/SNegate/Not") <| @(t : T?) {
        var words <- iarith_words()
        check_ops(t, words, "iarith", [SpvOp.SDiv, SpvOp.SRem, SpvOp.SNegate, SpvOp.Not])
        validate(t, words, "iarith")
        delete words
    }
}

[test(description = "float arithmetic lowers to FAdd, FSub, FMul, FDiv, FRem and FNegate.")]
def test_float_arith(t : T?) {
    t |> run("float arithmetic: FAdd/FSub/FMul/FDiv/FRem/FNegate") <| @(t : T?) {
        var words <- farith_words()
        check_ops(t, words, "farith", [SpvOp.FAdd, SpvOp.FSub, SpvOp.FMul, SpvOp.FDiv, SpvOp.FRem, SpvOp.FNegate])
        validate(t, words, "farith")
        delete words
    }
}

// Bitwise/shift binary ops gate (found by the one-source path tracer's PCG RNG).
//
// binop_code previously covered only +,-,*,/,% — every bitwise binary op (&, |, ^, <<, >>) failed
// "unsupported binary op". They lower to BitwiseAnd/Or/Xor + ShiftLeftLogical, and `>>` splits by
// signedness: uint -> ShiftRightLogical, int -> ShiftRightArithmetic (daslang C semantics). The
// fixture is the PCG hash step — the canonical stateless shader RNG — plus an int arithmetic-shift
// arm. spirv-val'd.
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

[test(description = "Bitwise &, |, ^, << and >> lower to BitwiseAnd, Or, Xor and shifts, with >> split by signedness.")]
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

// intBitsToFloat / uintBitsToFloat / floatBitsToInt / floatBitsToUint (daslib/math_bits'
// int_bits_to_float / uint_bits_to_float / float_bits_to_int / float_bits_to_uint) each lower to a
// single core OpBitcast, result type from the overload's return.  math_bits is a builtin module, so
// its CPU-reinterpret body is never emitted as an OpFunction.  The blob is spirv-val clean.
var @out @location = 0 out_color : float4

[fragment_shader(name="bitcast_spv"), marker(no_coverage)]
def bitcast_frag {
    let i = int(gl_FragCoord.x)
    let u = uint(gl_FragCoord.y)
    let j = float_bits_to_int(gl_FragCoord.z)
    let k = float_bits_to_uint(gl_FragCoord.w)
    out_color = float4(
        int_bits_to_float(i),
        uint_bits_to_float(u),
        int_bits_to_float(j),
        uint_bits_to_float(k))
}

def public bitcast_words : array<uint> => clone_to_move(bitcast_spv)

[test(description = "int, uint and float bit-casts lower to OpBitcast and the cast body is never emitted.")]
def test_bit_cast(t : T?) {
    t |> run("int/uint/float bit-casts -> OpBitcast; body never emitted; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(bitcast_spv)
        // six bit-cast calls in the body -> six OpBitcast
        t |> success(count_op(words, SpvOp.Bitcast) == 6,
            "six bit-casts -> six OpBitcast (got {count_op(words, SpvOp.Bitcast)})")
        // the math_bits reinterpret body must never become an OpFunction / OpFunctionCall
        t |> success(count_op(words, SpvOp.FunctionCall) == 0,
            "no OpFunctionCall: math_bits is a builtin, not a user function")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

[test(description = "float, int and uint casts lower to the matching OpConvert and OpBitcast opcodes.")]
def test_scalar_conversions(t : T?) {
    t |> run("conversions: float/int/uint casts -> OpConvert* / OpBitcast") <| @(t : T?) {
        var words <- conv_words()
        t |> success(has_op(words, SpvOp.ConvertSToF), "conv: float(int) -> OpConvertSToF")
        t |> success(has_op(words, SpvOp.ConvertUToF), "conv: float(uint) -> OpConvertUToF")
        t |> success(has_op(words, SpvOp.ConvertFToS), "conv: int(float) -> OpConvertFToS")
        t |> success(has_op(words, SpvOp.ConvertFToU), "conv: uint(float) -> OpConvertFToU")
        t |> success(has_op(words, SpvOp.Bitcast), "conv: int<->uint -> OpBitcast")
        validate(t, words, "conv")
        delete words
    }
}

// Bool constants gate.
//
// `false` / `true` literals (ExprConstBool) lower to OpConstantFalse / OpConstantTrue — these carry NO
// literal word (the opcode IS the value), so they are type + result-id only. Before the fix the emitter
// had no rvalue path for ExprConstBool and failed the annotation patch with "no rvalue available for
// ExprConstBool". (Bool locals, comparisons, &&/||/!, and bool `if` conditions already worked — only
// the bool literals were missing.) Asserts OpTypeBool + both bool constants are emitted, spirv-val clean.
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

[test(description = "false and true lower to OpConstantFalse and OpConstantTrue with an OpTypeBool.")]
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

// Unit gate for spirv_types.emit_type: correct type instructions + dedup pool correctness
// (risk #1 — type/constant dedup). Scalars dedup to one id; distinct base types stay distinct.
[test(description = "emit_type produces the expected type instructions and deduplicates repeated types.")]
def test_emit_type(t : T?) {
    t |> run("emit_type: type instructions + dedup") <| @(t : T?) {
        ast_gc_guard() {
            var m = make_module()
            let u  = emit_type(m, new TypeDecl(baseType = Type.tUInt))
            let u3 = emit_type(m, new TypeDecl(baseType = Type.tUInt3))
            let f  = emit_type(m, new TypeDecl(baseType = Type.tFloat))
            let f4 = emit_type(m, new TypeDecl(baseType = Type.tFloat4))
            let u2 = emit_type(m, new TypeDecl(baseType = Type.tUInt))
            t |> equal(u, u2)                          // scalar dedup
            t |> success(u != f, "distinct scalars")
            t |> success(u3 != f4, "distinct vectors")
            var words <- module_words(m)
            t |> equal(count_op(words, SpvOp.TypeInt), 1)      // uint once (u, u3 component, u2)
            t |> equal(count_op(words, SpvOp.TypeFloat), 1)    // float once (f, f4 component)
            t |> equal(count_op(words, SpvOp.TypeVector), 2)   // uint3 + float4
            delete words
            delete m
        }
    }
}
