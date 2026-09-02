options gen2
options indenting = 4

// The four-wide decode twin (SPV_NV_cooperative_matrix_decode_vector): the tenth
// coopmatLoadTensorDecode argument. `true` synthesizes the twin over the scalar body (four
// OpFunctionCalls composed into the vector); `false` is the scalar-only load; a [spirv_decode]
// function returning half4 is a hand-laid twin, as a free function or a class method. The scalar
// DecodeFunc stays on the load beside the DecodeVectorFunc, so the mask word carries both bits and
// two operands follow it in that order. The extension adds no opcode, so the census cannot see it:
// the assertions here are on the mask word, the operand ids, and the function set - and they hold
// without spirv-val, which CI does not carry.

require dastest/testing_boost public
require _spirv_common               // validate_spirv, coopmat2_words, Cm2BlockQ8, cm2_decode_q8
require spirv/spirv_shader
require spirv/spirv_builtins
require spirv/spirv_dis             // count_op, op_has_operand, count_op_operand_at, foreach_inst, strip_decode_vector
require spirv/spirv_builder         // string_words
require spirv/spirv_grammar         // SpvOp, SpvCapability, SpvTensorAddressingOperands, SpvMemoryAccess
require strings                     // find

var @ssbo @binding = 0 dv_a : array<Cm2BlockQ8>
var @ssbo @binding = 1 dv_b : array<float16>
var @ssbo @binding = 2 dv_c : array<float>
var @ssbo @binding = 3 dv_meta : array<uint>

// the free-function twin: four consecutive quants are two u16 lanes of the block
[spirv_decode, unused_argument(bc)]
def dv_decode_q8_v4(blk : Cm2BlockQ8; bc, cib : uint2) : half4 {
    let i0 = int((cib.y & 28u) >> 1u)
    let s0 = int(uint(blk.qs[i0]))
    let s1 = int(uint(blk.qs[i0 + 1]))
    let d = float(blk.d)
    return half4(float4(float((s0 << 24) >> 24) * d, float((s0 << 16) >> 24) * d,
        float((s1 << 24) >> 24) * d, float((s1 << 16) >> 24) * d))
}

// the scalar q8 GEMM of _spirv_common with TWO decode loads sharing one synthesized twin: the
// unrolled k loop and a tail step, the dasLLAMA tile's shape
[compute_shader(local_size_x=128, name="coopmat2_dv_synth_spv"), marker(no_coverage)]
def coopmat2_dv_synth_kernel {
    let m = dv_meta[0u]
    let n = dv_meta[1u]
    let kk = dv_meta[2u]
    let ti = gl_WorkGroupID.x
    let tj = gl_WorkGroupID.y
    var tla : tensorLayout2DPad
    tensorLayoutCreate(tla)
    tensorLayoutSetBlockSize(tla, 1u, 32u)
    tensorLayoutSetDimension(tla, m, kk)
    tensorLayoutSetStride(tla, kk / 32u, 1u)
    var tlb : tensorLayout2DPad
    tensorLayoutCreate(tlb)
    tensorLayoutSetDimension(tlb, n, kk)
    tensorLayoutSetStride(tlb, kk, 1u)
    var tlo : tensorLayout2D
    tensorLayoutCreate(tlo)
    tensorLayoutSetDimension(tlo, m, n)
    tensorLayoutSetStride(tlo, n, 1u)
    var tv : tensorView2Dt
    tensorViewCreate(tv)
    var a : coopmatWgA_f16_64x16
    var b : coopmatWgB_f16_16x32
    var acc : coopmatWgAcc_f16_64x32
    var k = 0u
    while (k + 32u <= kk) {
        coopmatLoadTensorDecode(a, dv_a, 0u, tla, ti * 64u, 64u, k, 16u, @@cm2_decode_q8, true)
        coopmatLoadTensor(b, dv_b, 0u, tlb, tj * 32u, 32u, k, 16u, tv)
        acc = coopmatMulAdd(a, b, acc)
        k += 16u
    }
    if (k < kk) {
        coopmatLoadTensorDecode(a, dv_a, 0u, tla, ti * 64u, 64u, k, 16u, @@cm2_decode_q8, true)
        coopmatLoadTensor(b, dv_b, 0u, tlb, tj * 32u, 32u, k, 16u, tv)
        acc = coopmatMulAdd(a, b, acc)
    }
    coopmatClamp(acc, -65504.0, 65504.0)
    var accw : coopmatWgAcc_f32_64x32
    coopmatConvert(accw, acc)
    coopmatStoreTensor(accw, dv_c, 0u, tlo, ti * 64u, 64u, tj * 32u, 32u)
}

// the same GEMM with `decode_v` = `false`: the scalar-only load, the arm a format that opts out of
// the twin takes
[compute_shader(local_size_x=128, name="coopmat2_dv_scalar_spv"), marker(no_coverage)]
def coopmat2_dv_scalar_kernel {
    let m = dv_meta[0u]
    let n = dv_meta[1u]
    let kk = dv_meta[2u]
    let ti = gl_WorkGroupID.x
    let tj = gl_WorkGroupID.y
    var tla : tensorLayout2DPad
    tensorLayoutCreate(tla)
    tensorLayoutSetBlockSize(tla, 1u, 32u)
    tensorLayoutSetDimension(tla, m, kk)
    tensorLayoutSetStride(tla, kk / 32u, 1u)
    var tlb : tensorLayout2DPad
    tensorLayoutCreate(tlb)
    tensorLayoutSetDimension(tlb, n, kk)
    tensorLayoutSetStride(tlb, kk, 1u)
    var tlo : tensorLayout2D
    tensorLayoutCreate(tlo)
    tensorLayoutSetDimension(tlo, m, n)
    tensorLayoutSetStride(tlo, n, 1u)
    var tv : tensorView2Dt
    tensorViewCreate(tv)
    var a : coopmatWgA_f16_64x16
    var b : coopmatWgB_f16_16x32
    var acc : coopmatWgAcc_f16_64x32
    var k = 0u
    while (k < kk) {
        coopmatLoadTensorDecode(a, dv_a, 0u, tla, ti * 64u, 64u, k, 16u, @@cm2_decode_q8, false)
        coopmatLoadTensor(b, dv_b, 0u, tlb, tj * 32u, 32u, k, 16u, tv)
        acc = coopmatMulAdd(a, b, acc)
        k += 16u
    }
    coopmatClamp(acc, -65504.0, 65504.0)
    var accw : coopmatWgAcc_f32_64x32
    coopmatConvert(accw, acc)
    coopmatStoreTensor(accw, dv_c, 0u, tlo, ti * 64u, 64u, tj * 32u, 32u)
}

// the same GEMM with the free-function half4 twin
[compute_shader(local_size_x=128, name="coopmat2_dv_free_spv"), marker(no_coverage)]
def coopmat2_dv_free_kernel {
    let m = dv_meta[0u]
    let n = dv_meta[1u]
    let kk = dv_meta[2u]
    let ti = gl_WorkGroupID.x
    let tj = gl_WorkGroupID.y
    var tla : tensorLayout2DPad
    tensorLayoutCreate(tla)
    tensorLayoutSetBlockSize(tla, 1u, 32u)
    tensorLayoutSetDimension(tla, m, kk)
    tensorLayoutSetStride(tla, kk / 32u, 1u)
    var tlb : tensorLayout2DPad
    tensorLayoutCreate(tlb)
    tensorLayoutSetDimension(tlb, n, kk)
    tensorLayoutSetStride(tlb, kk, 1u)
    var tlo : tensorLayout2D
    tensorLayoutCreate(tlo)
    tensorLayoutSetDimension(tlo, m, n)
    tensorLayoutSetStride(tlo, n, 1u)
    var tv : tensorView2Dt
    tensorViewCreate(tv)
    var a : coopmatWgA_f16_64x16
    var b : coopmatWgB_f16_16x32
    var acc : coopmatWgAcc_f16_64x32
    var k = 0u
    while (k < kk) {
        coopmatLoadTensorDecode(a, dv_a, 0u, tla, ti * 64u, 64u, k, 16u, @@cm2_decode_q8, @@dv_decode_q8_v4)
        coopmatLoadTensor(b, dv_b, 0u, tlb, tj * 32u, 32u, k, 16u, tv)
        acc = coopmatMulAdd(a, b, acc)
        k += 16u
    }
    coopmatClamp(acc, -65504.0, 65504.0)
    var accw : coopmatWgAcc_f32_64x32
    coopmatConvert(accw, acc)
    coopmatStoreTensor(accw, dv_c, 0u, tlo, ti * 64u, 64u, tj * 32u, 32u)
}

struct DvClsArgs {
    d : uint
    n : uint
}

// the hand-laid twin as a class method
class DvDecodeCls {
    @ssbo @binding = 0 wq : array<Cm2BlockQ8>
    @ssbo @binding = 1 xb : array<float16>
    @ssbo @binding = 2 yc : array<float>
    @push_constant pa : DvClsArgs

    [spirv_decode, unused_argument(bc)]
    def decode_q8(blk : Cm2BlockQ8; bc, cib : uint2) : float16 {
        let s = uint(blk.qs[int((cib.y & 30u) >> 1u)])
        let sh = 24 - int(cib.y & 1u) * 8
        let q = (int(s) << sh) >> 24
        return blk.d * float16(float(q))
    }

    [spirv_decode, unused_argument(bc)]
    def decode_q8_v4(blk : Cm2BlockQ8; bc, cib : uint2) : half4 {
        let i0 = int((cib.y & 28u) >> 1u)
        let s0 = int(uint(blk.qs[i0]))
        let s1 = int(uint(blk.qs[i0 + 1]))
        let d = float(blk.d)
        return half4(float4(float((s0 << 24) >> 24) * d, float((s0 << 16) >> 24) * d,
            float((s1 << 24) >> 24) * d, float((s1 << 16) >> 24) * d))
    }

    [spirv_kernel(local_size_x = 256, name = "coopmat2_dv_hand_spv")]
    def run {
        var tla : tensorLayout2DPad
        tensorLayoutCreate(tla)
        tensorLayoutSetBlockSize(tla, 1u, 32u)
        tensorLayoutSetDimension(tla, pa.d, pa.n)
        tensorLayoutSetStride(tla, pa.n >> 5u, 1u)
        var tlb : tensorLayout2DPad
        tensorLayoutCreate(tlb)
        tensorLayoutSetDimension(tlb, 256u, pa.n)
        tensorLayoutSetStride(tlb, pa.n, 1u)
        var tlo : tensorLayout2D
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, 256u, pa.d)
        tensorLayoutSetStride(tlo, pa.d, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var a : coopmatWgA_f16_128x64
        var b : coopmatWgB_f16_64x256
        var acc : coopmatWgAcc_f16_128x256
        let ir = gl_WorkGroupID.x
        var k = 0u
        while (k < pa.n) {
            coopmatLoadTensorDecode(a, wq, 0u, tla, ir * 128u, 128u, k, 64u, self.decode_q8, self.decode_q8_v4)
            coopmatLoadTensor(b, xb, 0u, tlb, 0u, 256u, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : coopmatWgAcc_f32_128x256
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, yc, 0u, tlo, 0u, 256u, ir * 128u, 128u, tv)
    }
}

def private decode_and_vector_bits : uint => uint(SpvTensorAddressingOperands.DecodeFunc) | uint(SpvTensorAddressingOperands.DecodeVectorFunc)

// the OpFunction result ids in emission order (main, then the decode bodies)
def private function_ids(words : array<uint>) : array<uint> {
    var ids : array<uint>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.Function) && n >= 2) {
            ids |> push(w[s + 1])
        }
    }
    return <- ids
}

// the operand words of the first OpCooperativeMatrixLoadTensorNV whose mask carries `bit`
def private load_operands_with(words : array<uint>; bit : uint) : array<uint> {
    var ops : array<uint>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.CooperativeMatrixLoadTensorNV) && n > 6 && (w[s + 6] & bit) != 0u && empty(ops)) {
            for (k in range(n)) {
                ops |> push(w[s + k])
            }
        }
    }
    return <- ops
}

// the operand words of the first OpCooperativeMatrixLoadTensorNV, wherever its mask sits
def private first_load_operands(words : array<uint>) : array<uint> {
    var ops : array<uint>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.CooperativeMatrixLoadTensorNV) && empty(ops)) {
            for (k in range(n)) {
                ops |> push(w[s + k])
            }
        }
    }
    return <- ops
}

def private has_extension(words : array<uint>; name : string) : bool {
    var inscope want <- string_words(name)
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.Extension) && n == length(want)) {
            var same = true
            for (k in range(n)) {
                if (w[s + k] != want[k]) {
                    same = false
                }
            }
            if (same) {
                found = true
            }
        }
    }
    return found
}

// spirv-val is a witness, never the gate: absent it skips, and a SPIRV-Tools older than the
// extension (SDK 1.4.350 and below rejects capability 5447) skips the same way
def private validate_or_skip(t : T?; words : array<uint>; what : string) {
    let r = validate_spirv(words, "vulkan1.3")
    if (!r.ran) return
    if (find(r.msg, "Invalid capability operand: 5447") >= 0) {
        to_log(LOG_WARNING, "{what}: spirv-val predates SPV_NV_cooperative_matrix_decode_vector - skipped\n")
        return
    }
    t |> success(r.ok, "spirv-val (vulkan1.3) on {what}: {r.msg}")
}

[test]
def test_coopmat2_decode_vector_synth(t : T?) {
    t |> run("synthesized twin: capability + extension, both mask bits, one twin for two loads, four calls of the scalar body") <| @(t : T?) {
        var inscope base <- coopmat2_words()
        var inscope spv <- clone_to_move(coopmat2_dv_synth_spv)
        t |> success(op_has_operand(spv, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "OpCapability CooperativeMatrixDecodeVectorNV")
        t |> success(!op_has_operand(base, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "the scalar-only load declares no such capability")
        t |> success(has_extension(spv, "SPV_NV_cooperative_matrix_decode_vector"), "OpExtension SPV_NV_cooperative_matrix_decode_vector")
        t |> success(count_op(spv, SpvOp.Extension) == count_op(base, SpvOp.Extension) + 1,
            "one OpExtension more than the scalar-only kernel, declared once for two loads")
        t |> success(count_op_operand_at(spv, SpvOp.CooperativeMatrixLoadTensorNV, 6, decode_and_vector_bits()) == 2,
            "both decode loads carry DecodeFunc | DecodeVectorFunc")
        t |> success(count_op_operand_at(base, SpvOp.CooperativeMatrixLoadTensorNV, 6, uint(SpvTensorAddressingOperands.DecodeFunc)) == 1,
            "the scalar-only load's mask carries DecodeFunc alone")
        t |> success(count_op(spv, SpvOp.Function) == 3,
            "three OpFunction (main, the scalar decode, ONE synthesized twin for two loads)")
        t |> success(count_op(spv, SpvOp.FunctionCall) == 4,
            "the twin calls the scalar body four times")
        t |> success(count_op(spv, SpvOp.FunctionParameter) == 6,
            "six OpFunctionParameter (the rigid three, twice)")
        t |> success(count_op(spv, SpvOp.IAdd) >= 3,
            "the twin steps coordInBlock.y by 1, 2, 3")
        // the operand order behind the mask, checked in the words: DecodeFunc's id then
        // DecodeVectorFunc's, and those are the second and third OpFunction in emission order
        var inscope fids <- function_ids(spv)
        var inscope ops <- load_operands_with(spv, uint(SpvTensorAddressingOperands.DecodeVectorFunc))
        t |> success(length(fids) == 3 && length(ops) == 9, "the load carries nine operands and the module three functions")
        if (length(fids) == 3 && length(ops) == 9) {
            t |> equal(ops[7], fids[1], "operand 7 is the scalar decode's OpFunction id")
            t |> equal(ops[8], fids[2], "operand 8 is the twin's OpFunction id")
        }
        validate_or_skip(t, spv, "the synthesized twin module")
    }
}

[test]
def test_coopmat2_decode_vector_scalar_only(t : T?) {
    t |> run("`false`: the scalar-only load - no capability, no extension, DecodeFunc alone, two functions") <| @(t : T?) {
        var inscope base <- coopmat2_words()
        var inscope spv <- clone_to_move(coopmat2_dv_scalar_spv)
        t |> success(!op_has_operand(spv, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "no CooperativeMatrixDecodeVectorNV capability")
        t |> success(!has_extension(spv, "SPV_NV_cooperative_matrix_decode_vector"), "no decode-vector OpExtension")
        t |> success(count_op(spv, SpvOp.Extension) == count_op(base, SpvOp.Extension),
            "the same extension set as the nine-argument form")
        t |> success(count_op_operand_at(spv, SpvOp.CooperativeMatrixLoadTensorNV, 6, uint(SpvTensorAddressingOperands.DecodeFunc)) == 1,
            "the load's mask carries DecodeFunc alone")
        t |> success(count_op(spv, SpvOp.Function) == 2, "two OpFunction (main + the scalar decode), no twin")
        validate_or_skip(t, spv, "the scalar-only module")
    }
}

[test]
def test_coopmat2_decode_vector_free(t : T?) {
    t |> run("hand-laid twin as a free function: half4 return, both mask bits, no synthesized calls") <| @(t : T?) {
        var inscope spv <- clone_to_move(coopmat2_dv_free_spv)
        t |> success(op_has_operand(spv, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "OpCapability CooperativeMatrixDecodeVectorNV")
        t |> success(count_op_operand_at(spv, SpvOp.CooperativeMatrixLoadTensorNV, 6, decode_and_vector_bits()) == 1,
            "the decode load's addressing mask carries DecodeFunc | DecodeVectorFunc")
        t |> success(count_op(spv, SpvOp.Function) == 3, "three OpFunction (main, the scalar decode, the hand-laid twin)")
        t |> success(count_op(spv, SpvOp.FunctionCall) == 0, "no OpFunctionCall: the hand-laid twin has its own body")
        var inscope fids <- function_ids(spv)
        var inscope ops <- load_operands_with(spv, uint(SpvTensorAddressingOperands.DecodeVectorFunc))
        if (length(fids) == 3 && length(ops) == 9) {
            t |> equal(ops[7], fids[1], "operand 7 is the scalar decode's OpFunction id")
            t |> equal(ops[8], fids[2], "operand 8 is the hand-laid twin's OpFunction id")
        } else {
            t |> failure("expected three functions and a nine-operand load")
        }
        validate_or_skip(t, spv, "the free-function twin module")
    }
}

[test]
def test_coopmat2_decode_vector_hand(t : T?) {
    t |> run("hand-laid twin as a class method: half4 return, both mask bits, no synthesized calls") <| @(t : T?) {
        t |> success(op_has_operand(coopmat2_dv_hand_spv, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "OpCapability CooperativeMatrixDecodeVectorNV")
        t |> success(count_op_operand_at(coopmat2_dv_hand_spv, SpvOp.CooperativeMatrixLoadTensorNV, 6, decode_and_vector_bits()) == 1,
            "the decode load's addressing mask carries DecodeFunc | DecodeVectorFunc")
        t |> success(count_op(coopmat2_dv_hand_spv, SpvOp.Function) == 3,
            "three OpFunction (main, the scalar method, the hand-laid twin)")
        t |> success(count_op(coopmat2_dv_hand_spv, SpvOp.FunctionCall) == 0,
            "no OpFunctionCall: the hand-laid twin has its own body")
        t |> success(count_op(coopmat2_dv_hand_spv, SpvOp.FunctionParameter) == 6,
            "six OpFunctionParameter (self erased from both methods)")
        validate_or_skip(t, coopmat2_dv_hand_spv, "the class-method twin module")
    }
}

[test]
def test_coopmat2_decode_vector_strip(t : T?) {
    t |> run("strip_decode_vector: the device-without-the-feature fallback leaves a scalar-only module") <| @(t : T?) {
        var stripped := coopmat2_dv_synth_spv
        t |> success(strip_decode_vector(stripped), "a module naming the twin reports the strip")
        t |> success(!op_has_operand(stripped, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "the capability is gone")
        t |> success(!has_extension(stripped, "SPV_NV_cooperative_matrix_decode_vector"), "the decode-vector extension declaration is gone")
        t |> success(has_extension(stripped, "SPV_NV_cooperative_matrix2"), "the cm2 extension declaration stays")
        t |> success(count_op_operand_at(stripped, SpvOp.CooperativeMatrixLoadTensorNV, 6, uint(SpvTensorAddressingOperands.DecodeFunc)) == 2,
            "both loads' masks carry DecodeFunc alone")
        t |> success(length(stripped) == length(coopmat2_dv_synth_spv) - 2 - (1 + length(string_words("SPV_NV_cooperative_matrix_decode_vector"))) - 2,
            "exactly the capability, the extension and one operand word per load left the module")
        t |> success(count_op(stripped, SpvOp.Function) == 3,
            "the twin's OpFunction stays, unreferenced")
        var inscope fids <- function_ids(stripped)
        var inscope ops <- load_operands_with(stripped, uint(SpvTensorAddressingOperands.DecodeFunc))
        if (length(fids) == 3 && length(ops) == 8) {
            t |> equal(ops[7], fids[1], "the surviving operand is the scalar decode's id")
        } else {
            t |> failure("expected an eight-operand load after the strip")
        }
        var again := stripped
        t |> success(!strip_decode_vector(again), "a scalar-only module reports nothing to strip")
        t |> success(length(again) == length(stripped), "and is left as it was")
        validate_or_skip(t, stripped, "the stripped module")
        delete again
        delete stripped
    }
    t |> run("strip_decode_vector on foreign words: memory-access extras, a view operand, a load with no addressing operands, a truncated stream") <| @(t : T?) {
        // a hand-built module: header, the capability, the extension, then two loads. The first
        // carries Aligned (one literal) and NoAliasINTEL (one id) before the mask, and all three
        // addressing operands - view 40, decode 41, twin 42. The second carries no mask at all
        // (six operands), the shape a load without decode has; its word count puts the would-be
        // mask index one past its last operand.
        let ld = uint(SpvOp.CooperativeMatrixLoadTensorNV)
        let memop = uint(SpvMemoryAccess.Aligned) | uint(SpvMemoryAccess.NoAliasINTELMask)
        let ta = uint(SpvTensorAddressingOperands.TensorView) | decode_and_vector_bits()
        var words <- [0x07230203u, 0x00010600u, 0u, 100u, 0u,
            (2u << 16u) | uint(SpvOp.Capability), uint(SpvCapability.CooperativeMatrixDecodeVectorNV)]
        var inscope ext <- string_words("SPV_NV_cooperative_matrix_decode_vector")
        words |> push((uint(1 + length(ext)) << 16u) | uint(SpvOp.Extension))
        words |> push_from(ext, [(13u << 16u) | ld, 10u, 11u, 12u, 13u, 14u, memop, 16u, 30u, ta, 40u, 41u, 42u],
            [(7u << 16u) | ld, 10u, 11u, 12u, 13u, 14u, 0u])
        let before = length(words)
        t |> success(strip_decode_vector(words), "the foreign module reports the strip")
        t |> success(length(words) == before - 2 - (1 + length(ext)) - 1, "the capability, the extension and one operand word left")
        var inscope ops <- first_load_operands(words)
        t |> success(length(ops) == 11, "the first load keeps eleven operands")
        if (length(ops) == 11) {
            t |> equal(ops[5], memop, "the memory-access mask is untouched")
            t |> equal(ops[6], 16u, "the Aligned literal is untouched")
            t |> equal(ops[7], 30u, "the NoAliasINTEL id is untouched")
            t |> equal(ops[8], uint(SpvTensorAddressingOperands.TensorView) | uint(SpvTensorAddressingOperands.DecodeFunc), "the DecodeVectorFunc bit is cleared")
            t |> equal(ops[9], 40u, "the view id survives")
            t |> equal(ops[10], 41u, "the decode id survives as the last operand")
        }
        t |> success(count_op(words, SpvOp.CooperativeMatrixLoadTensorNV) == 2, "the second load survived untouched")
        var truncated <- [0x07230203u, 0x00010600u, 0u, 100u, 0u, (13u << 16u) | ld, 10u, 11u]
        t |> success(!strip_decode_vector(truncated), "a truncated stream strips nothing and does not panic")
        delete truncated
        delete words
    }
}
