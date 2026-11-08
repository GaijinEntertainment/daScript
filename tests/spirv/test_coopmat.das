options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_shader
require spirv/spirv_builtins
require spirv/spirv_builder
require strings

// Cooperative-matrix gate (SPV_KHR_cooperative_matrix — Tensor-Core MMA). The coopmat_kernel fixture is
// an f16 x f16 -> f32 GEMM tile: it loads a 16x16 A (row-major) and B (column-major) from f16 SSBOs into
// subgroup tiles, does one OpCooperativeMatrixMulAddKHR into a zero-initialized (OpConstantNull) f32
// accumulator, and stores the 16x16 result column-major. This pins the four KHR coopmat opcodes, the
// CooperativeMatrixKHR capability + SPV_KHR_cooperative_matrix extension, and the SPIR-V 1.6 version
// floor; spirv-val (vulkan1.3) is the independent witness that the whole shape is valid.
[test(description = "Cooperative-matrix tiles (f16 GEMM, int8, f32 accumulator with convert, workgroup-memory load, accumulator arrays under unroll_full) emit the KHR coopmat opcodes, capabilities and SPIR-V 1.6 floor.")]
def test_coopmat(t : T?) { // nolint:STYLE038 - flat list of opcode/capability assertions
    t |> run("cooperative matrix f16xf16->f32 GEMM tile: KHR coopmat opcodes + capability + 1.6 floor") <| @(t : T?) {
        var words <- coopmat_words()
        // capability + extension + the SPIR-V 1.6 version floor coopmat forces
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR (6022)")
        t |> success(count_op(words, SpvOp.Extension) >= 1,
            "an OpExtension is declared (SPV_KHR_cooperative_matrix)")
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6,
            "SPIR-V header version floored to 1.6")
        // three distinct tile types (A f16/use0, B f16/use1, accumulator f32/use2), two loads, one
        // multiply-accumulate, one store
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 3,
            "three OpTypeCooperativeMatrixKHR (A, B, accumulator)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixLoadKHR) == 2,
            "two OpCooperativeMatrixLoadKHR (A, B tiles)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "one OpCooperativeMatrixMulAddKHR")
        t |> success(count_op(words, SpvOp.CooperativeMatrixStoreKHR) == 1,
            "one OpCooperativeMatrixStoreKHR")
        // the accumulator is zero-initialized (a bare `var acc : coopmatAcc...`) -> OpConstantNull
        t |> success(count_op(words, SpvOp.ConstantNull) >= 1,
            "OpConstantNull zero-inits the accumulator tile")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping - run on a box with the Vulkan SDK on PATH\n")
        }
        delete words
    }
    t |> run("cooperative matrix INT8 tile: s8xs8->s32 (16x16x32) + the signed-components MulAdd mask") <| @(t : T?) {
        var words <- coopmat_i8_words()
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR (6022)")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.StorageBuffer8BitAccess)),
            "OpCapability StorageBuffer8BitAccess (the int8 SSBO tiles)")
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 3,
            "three OpTypeCooperativeMatrixKHR (A s8 16x32, B s8 32x16, accumulator s32 16x16)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixLoadKHR) == 2, "two loads")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1, "one MulAdd")
        t |> success(count_op(words, SpvOp.CooperativeMatrixStoreKHR) == 1, "one store")
        // the integer MulAdd carries the CooperativeMatrixOperands mask at operand index 5:
        // A|B|C|Result signed = 0x1|0x2|0x4|0x8 = 0xF (float MulAdd omits it)
        t |> success(count_op_operand_at(words, SpvOp.CooperativeMatrixMulAddKHR, 5, 0xFu) == 1,
            "the MulAdd declares all operands signed (mask 0xF)")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping - run on a box with the Vulkan SDK on PATH\n")
        }
        delete words
    }
    t |> run("cooperative matrix f32 ACCUMULATOR + coopmatConvert narrowing to f16") <| @(t : T?) {
        var words <- coopmat_f16acc_words()
        t |> success(op_has_operand(words, SpvOp.Capability,
                uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR (6022)")
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6,
            "SPIR-V header version floored to 1.6")
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 4,
            "four OpTypeCooperativeMatrixKHR (A, B, f32 accumulator, f16 narrowed)")
        t |> equal(count_uint_indexed_coopmat_loads(words), 2,
            "both f16 A/B loads chain their pointer through a uint-typed element index (the widened `idx : int | uint`)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "one OpCooperativeMatrixMulAddKHR (f32 accumulate, no operands mask)")
        t |> success(count_op(words, SpvOp.FConvert) == 1,
            "one OpFConvert - the componentwise f32->f16 tile narrowing")
        t |> success(count_op(words, SpvOp.CooperativeMatrixStoreKHR) == 1,
            "one OpCooperativeMatrixStoreKHR (the f16 tile)")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping - run on a box with the Vulkan SDK on PATH\n")
        }
        delete words
    }
    t |> run("cooperative matrix from @workgroup shared memory (the tiled-GEMM staging path)") <| @(t : T?) {
        var words <- coopmat_sm_words()
        t |> success(count_op(words, SpvOp.CooperativeMatrixLoadKHR) == 2, "two coopmat loads (from shared)")
        // the shared tiles are Workgroup-storage OpVariables; the loads chain a Workgroup pointer
        t |> success(op_has_operand(words, SpvOp.Variable, uint(SpvStorageClass.Workgroup)),
            "a @workgroup (Workgroup-storage) OpVariable is declared")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping - run on a box with the Vulkan SDK on PATH\n")
        }
        delete words
    }
    t |> run("an ARRAY of accumulator tiles under [unroll_full]: one OpTypeArray, every element chain a constant") <| @(t : T?) {
        var words <- coopmat_arr_words()
        t |> equal(count_op(words, SpvOp.TypeArray), 1, "one OpTypeArray (the tile array; the ssbos are runtime arrays)")
        t |> equal(count_op(words, SpvOp.LoopMerge), 0, "no loop construct survives the unroll")
        t |> equal(count_op(words, SpvOp.CooperativeMatrixMulAddKHR), 4, "four OpCooperativeMatrixMulAddKHR, one per copy")
        t |> equal(count_op(words, SpvOp.FConvert), 4, "four OpFConvert widenings")
        t |> equal(count_op(words, SpvOp.CooperativeMatrixStoreKHR), 4, "four stores")
        let fc = count_function_storage_chains(words)
        t |> equal(fc.chains, 12, "twelve element chains into the tile array (read, write and convert per copy)")
        t |> equal(fc.dynamic_indices, 0, "every chain into the tile array indexes with an OpConstant")
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6, "SPIR-V header version 1.6")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping - run on a box with the Vulkan SDK on PATH\n")
        }
        delete words
    }
}

// Cooperative-matrix v2 gate (SPV_NV_cooperative_matrix2 + SPV_NV_tensor_addressing). The
// coopmat2_kernel fixture is the q8_0 decode-in-load GEMM shape: workgroup-scope tiles, tensor
// layout/view objects, per-K-tile OpTensorLayoutSliceNV, an A load through a [spirv_decode]
// function (a real OpFunction taking a PhysicalStorageBuffer block pointer — Aliased-decorated,
// Aligned loads), a B load through the (1,0) transpose view, the ±65504 f16 clamp loop, and a
// tensor store. Pins the NV capability set, the PhysicalStorageBuffer64 addressing model, and the
// exact operand shapes; spirv-val (vulkan1.3) is the independent witness the whole shape is valid.
[test(description = "A cm2 q8 GEMM with decode-in-load emits tensor addressing, the decode function and workgroup-scope tiles.")]
def test_coopmat2(t : T?) {
    t |> run("cm2 q8 decode-in-load GEMM: tensor addressing + decode function + wg-scope tiles") <| @(t : T?) {
        var words <- coopmat2_words()
        // the glslang cm2 capability set + both extensions
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.PhysicalStorageBufferAddresses)),
            "OpCapability PhysicalStorageBufferAddresses")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixTensorAddressingNV)),
            "OpCapability CooperativeMatrixTensorAddressingNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixBlockLoadsNV)),
            "OpCapability CooperativeMatrixBlockLoadsNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.TensorAddressingNV)),
            "OpCapability TensorAddressingNV")
        // PhysicalStorageBuffer64 Vulkan memory model + the 1.6 floor
        t |> success(op_has_operand_at(words, SpvOp.MemoryModel, 0, uint(SpvAddressingModel.PhysicalStorageBuffer64)),
            "OpMemoryModel PhysicalStorageBuffer64")
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6,
            "SPIR-V header version floored to 1.6")
        // tensor objects: 2 layout types (clamp Constant + clamp Undefined), 1 transpose view type,
        // 3 created layouts + 1 created view, the 3 setters, and one slice per load/store
        t |> success(count_op(words, SpvOp.TypeTensorLayoutNV) == 2,
            "two OpTypeTensorLayoutNV (zero-pad inputs, undefined-clamp store)")
        t |> success(count_op(words, SpvOp.TypeTensorViewNV) == 1,
            "one OpTypeTensorViewNV (the transpose view)")
        t |> success(count_op(words, SpvOp.CreateTensorLayoutNV) == 3,
            "three OpCreateTensorLayoutNV (A, B, out)")
        t |> success(count_op(words, SpvOp.CreateTensorViewNV) == 1,
            "one OpCreateTensorViewNV")
        t |> success(count_op(words, SpvOp.TensorLayoutSetDimensionNV) == 3,
            "three OpTensorLayoutSetDimensionNV")
        t |> success(count_op(words, SpvOp.TensorLayoutSetStrideNV) == 3,
            "three OpTensorLayoutSetStrideNV")
        t |> success(count_op(words, SpvOp.TensorLayoutSetBlockSizeNV) == 1,
            "one OpTensorLayoutSetBlockSizeNV (the 1x32 q8_0 block)")
        t |> success(count_op(words, SpvOp.TensorLayoutSliceNV) == 3,
            "three OpTensorLayoutSliceNV (A load, B load, store)")
        // tensor-addressed loads/stores + the wg-scope MMA
        t |> success(count_op(words, SpvOp.CooperativeMatrixLoadTensorNV) == 2,
            "two OpCooperativeMatrixLoadTensorNV (DecodeFunc A, TensorView B)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixStoreTensorNV) == 1,
            "one OpCooperativeMatrixStoreTensorNV")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "one OpCooperativeMatrixMulAddKHR")
        t |> success(count_op(words, SpvOp.FConvert) >= 1,
            "OpFConvert widens the f16 accumulator for the f32 store")
        // the clamp loop: Length + the emitter's only OpPhi + FClamp through GLSL.std.450
        t |> success(count_op(words, SpvOp.CooperativeMatrixLengthKHR) == 1,
            "one OpCooperativeMatrixLengthKHR")
        t |> success(count_op(words, SpvOp.Phi) == 1,
            "one OpPhi (the coopmatClamp element loop)")
        t |> success(count_op_operand_at(words, SpvOp.LoopMerge, 2, uint(SpvLoopControl.Unroll)) == 1,
            "the coopmatClamp element loop's OpLoopMerge carries Unroll (left rolled, the accumulator leaves tensor registers)")
        // the decode function: a second OpFunction with exactly 3 OpFunctionParameters, its
        // PhysicalStorageBuffer pointer decorated Aliased, and Aligned loads inside
        t |> success(count_op(words, SpvOp.Function) == 2,
            "two OpFunction (main + the decode function)")
        t |> success(count_op(words, SpvOp.FunctionParameter) == 3,
            "three OpFunctionParameter (block pointer + two uint[2] coords)")
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Aliased)) == 1,
            "the decode function's PSB pointer parameter is decorated Aliased")
        t |> success(count_op_operand_at(words, SpvOp.Load, 3, uint(SpvMemoryAccess.Aligned)) >= 2,
            "the decode function's PSB loads carry the Aligned memory operand")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}

// The cm2 decode-METHOD gate: [spirv_decode] on a kernel-class method, referenced as
// `self.<method>` from coopmatLoadTensorDecode. The das-level self is erased — the emitted decode
// function keeps the rigid 3-param SPV_NV_cooperative_matrix2 shape — while the body reads CLASS
// MEMBERS: a separate scale plane (the two-plane fmt-0 q8 layout: qs words + f16 scales), the push
// constants, and an @workgroup scalar staged by the kernel body. Also pins the l-tile GEMM shapes
// (A f16 128x64, B f16 64x256, acc 128x256 — llama.cpp's cm2 quant "l" geometry) through the
// name-parsed wg marker rail. spirv-val (vulkan1.3) is the independent witness.
struct Cm2ClsArgs {
    d : uint
    n : uint
    blk0 : uint
}

// the two-plane fmt-0 block: 8 words = 32 packed int8 quants, 32 bytes — the scale is NOT here
struct Cm2BlockQ8P {
    qs : uint[8]
}

class Cm2DecodeCls {
    @ssbo @binding = 0 wq : array<Cm2BlockQ8P>
    @ssbo @binding = 1 wsh : array<float16>
    @ssbo @binding = 2 xb : array<float16>
    @ssbo @binding = 3 yc : array<float>
    @push_constant pa : Cm2ClsArgs
    @workgroup blk0_sh : uint

    // the member reads are the form's whole point: wsh (a second SSBO the rigid decode signature
    // cannot carry), pa (push constants), blk0_sh (@workgroup staging by the kernel body)
    [spirv_decode, unused_argument(bc)]
    def decode_q8(blk : Cm2BlockQ8P; bc, cib : uint2) : float16 {
        let w = blk.qs[int(cib.y >> 2u)]
        let q = (int(w) << ((3 - int(cib.y & 3u)) * 8)) >> 24
        return wsh[blk0_sh + bc.x * (pa.n >> 5u) + bc.y] * float16(float(q))
    }

    [spirv_kernel(local_size_x = 256, name = "cm2_cls_spv")]
    def run {
        if (gl_LocalInvocationID.x == 0u) {
            blk0_sh = pa.blk0
        }
        barrier()
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
            coopmatLoadTensorDecode(a, wq, 0u, tla, ir * 128u, 128u, k, 64u, self.decode_q8)
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

[test(description = "A cm2 decode method on a kernel class emits a self-erased signature with member access and l-tile shapes.")]
def test_coopmat2_cls(t : T?) {
    t |> run("cm2 decode method: self-erased signature + member access + l-tile shapes") <| @(t : T?) {
        // the decode method emits as the SAME rigid shape the module-level form does: a second
        // OpFunction with exactly 3 parameters — the das-level self is erased
        t |> success(count_op(cm2_cls_spv, SpvOp.Function) == 2,
            "two OpFunction (main + the decode method)")
        t |> success(count_op(cm2_cls_spv, SpvOp.FunctionParameter) == 3,
            "three OpFunctionParameter (self erased: block pointer + two uint[2] coords)")
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Decorate, 1, uint(SpvDecoration.Aliased)) == 1,
            "the decode method's PSB pointer parameter is decorated Aliased")
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Load, 3, uint(SpvMemoryAccess.Aligned)) >= 1,
            "the decode method's PSB loads carry the Aligned memory operand")
        // member access from the decode body: the members classify as module-scope OpVariables in
        // their storage classes (storage class = OpVariable operand 2)
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Variable, 2, uint(SpvStorageClass.StorageBuffer)) == 4,
            "four StorageBuffer OpVariable (wq, wsh, xb, yc)")
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Variable, 2, uint(SpvStorageClass.PushConstant)) == 1,
            "one PushConstant OpVariable (pa)")
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Variable, 2, uint(SpvStorageClass.Workgroup)) == 1,
            "one Workgroup OpVariable (blk0_sh)")
        // the l-tile flexible-dimension types: A 128x64, B 64x256, acc f16 + f32 128x256
        t |> success(count_op(cm2_cls_spv, SpvOp.TypeCooperativeMatrixKHR) == 4,
            "four OpTypeCooperativeMatrixKHR (A, B, f16 acc, f32 acc)")
        // the GEMM shape: decode load + view load, one MMA, one clamped tensor store
        t |> success(count_op(cm2_cls_spv, SpvOp.CooperativeMatrixLoadTensorNV) == 2,
            "two OpCooperativeMatrixLoadTensorNV (DecodeFunc A, TensorView B)")
        t |> success(count_op(cm2_cls_spv, SpvOp.CooperativeMatrixStoreTensorNV) == 1,
            "one OpCooperativeMatrixStoreTensorNV")
        t |> success(count_op(cm2_cls_spv, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "one OpCooperativeMatrixMulAddKHR")
        let r = validate_spirv(cm2_cls_spv, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
    }
}

// The four-wide decode twin (SPV_NV_cooperative_matrix_decode_vector): the tenth
// coopmatLoadTensorDecode argument. `true` synthesizes the twin over the scalar body (four
// OpFunctionCalls composed into the vector); `false` is the scalar-only load; a [spirv_decode]
// function returning half4 is a hand-laid twin, as a free function or a class method. The scalar
// DecodeFunc stays on the load beside the DecodeVectorFunc, so the mask word carries both bits and
// two operands follow it in that order. The extension adds no opcode, so the census cannot see it:
// the assertions here are on the mask word, the operand ids, and the function set - and they hold
// without spirv-val, which CI does not carry.
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

[test(description = "A synthesized vector decode twin declares its capability and extension, sets both mask bits, serves two loads with one twin and makes four calls of the scalar body.")]
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

[test(description = "A scalar-only decode load declares no vector capability or extension and emits DecodeFunc alone in two functions.")]
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

[test(description = "A hand-laid vector twin written as a free function returns half4, sets both mask bits and synthesizes no calls.")]
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

[test(description = "A hand-laid vector twin written as a class method returns half4, sets both mask bits and synthesizes no calls.")]
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

[test(description = "strip_decode_vector turns a vector-decode module into a scalar-only one and leaves foreign word streams intact.")]
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

// The cm2 e-stamp shapes - the m column at the 32-deep k step - emit as OpTypeCooperativeMatrixKHR
// with the rows and columns the stamp declares, and validate.
[test(description = "The cm2 e-stamp tile shapes (A 128x32, B 32x128, f16 and f32 128x128 accumulators) build through the name-parsed marker rail.")]
def test_coopmat2_e_tiles(t : T?) {
    t |> run("cm2 e-stamp shapes: A 128x32, B 32x128, acc f16 128x128, acc f32 128x128 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2e_words()
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6, "SPIR-V header version 1.6")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR")
        var got <- spv_coopmat_shapes(words)
        var want <- ["128x128", "128x128", "128x32", "32x128"]
        t |> equal(want, got, "the sorted OpTypeCooperativeMatrixKHR shape set - the e stamp's 128x32 and 32x128 operand tiles and its two 128x128 accumulators, not the s stamp's")
        delete want
        delete got
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1, "one OpCooperativeMatrixMulAddKHR")
        t |> success(count_op(words, SpvOp.FConvert) >= 1, "the f16 -> f32 accumulator convert emits OpFConvert")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}

// Cooperative-matrix v2 flash-attention rail (SPV_NV_cooperative_matrix2 reductions / conversions /
// per-element ops). The coopmat2fa_kernel fixture is llama.cpp's flash_attn_cm2 inner loop: online
// softmax over KV tiles with OpCooperativeMatrixReduceNV (row reduce + the column-RESIZING smear),
// OpCooperativeMatrixPerElementOpNV (no-extra exp, a matrix extra for the running max, scalar extras
// for the on-the-fly causal mask), use-changing OpFConvert (f32 accumulator -> f16 A — the
// ConversionsNV shape glslang emits for coopmat<f16,...,A>(acc)), componentwise tile arithmetic,
// OpMatrixTimesScalar, and single-scalar OpCompositeConstruct fills. Callback functions are BY-VALUE
// scalar OpFunctions — the operand shapes are pinned against llama.cpp's compiled
// flash_attn_f32_f16_cm2.spv; spirv-val (vulkan1.3) is the independent witness.
[test(description = "A cm2 flash-attention kernel emits the reduce and per-element callbacks, the use conversion and tile algebra.")]
def test_coopmat2_fa(t : T?) {
    t |> run("cm2 flash-attention: reduce + per-element callbacks, use conversion, tile algebra") <| @(t : T?) {
        var words <- coopmat2fa_words()
        // the three opt-in cm2 capabilities ride the base cm2 set
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixReductionsNV)),
            "OpCapability CooperativeMatrixReductionsNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixPerElementOperationsNV)),
            "OpCapability CooperativeMatrixPerElementOperationsNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixConversionsNV)),
            "OpCapability CooperativeMatrixConversionsNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixTensorAddressingNV)),
            "OpCapability CooperativeMatrixTensorAddressingNV")
        // reduces: rowmax + the two smears, all Row (mask literal 1), each with a combine-fn id
        t |> success(count_op(words, SpvOp.CooperativeMatrixReduceNV) == 3,
            "three OpCooperativeMatrixReduceNV (rowmax + eM smear + L smear)")
        t |> success(count_op_operand_at(words, SpvOp.CooperativeMatrixReduceNV, 3, 1u) == 3,
            "every reduce carries the Row mask literal")
        // per-element ops: mask (2 scalar extras), pick_max (1 matrix extra), exp x2, recip
        t |> success(count_op(words, SpvOp.CooperativeMatrixPerElementOpNV) == 5,
            "five OpCooperativeMatrixPerElementOpNV (mask, running max, exp x2, recip)")
        // callbacks: main + 6 by-value scalar OpFunctions (max, smear, exp, pick_max, mask, recip)
        t |> success(count_op(words, SpvOp.Function) == 7,
            "seven OpFunction (main + six callbacks)")
        t |> success(count_op(words, SpvOp.FunctionParameter) == 2 + 2 + 3 + 4 + 5 + 3,
            "19 by-value OpFunctionParameter across the six callbacks")
        // tile algebra: 3 MulAdds (S, rowsum, O), componentwise mul x3 / add / sub x2, Q pre-scale
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 3,
            "three OpCooperativeMatrixMulAddKHR (Q@K^T, P@One, P@V)")
        t |> success(count_op(words, SpvOp.MatrixTimesScalar) == 1,
            "one OpMatrixTimesScalar (the Q pre-scale)")
        // the use-changing conversions: Q f32 ACC -> f16 A and P f32 ACC -> f16 A
        t |> success(count_op(words, SpvOp.FConvert) >= 2,
            "OpFConvert covers the two ACC -> A use conversions")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}

// The h256 flash-attention tiles: coopmatWgA_f16_64x256, coopmatWgB_f16_256x32, coopmatWgB_f16_32x256
// and the 64x256 accumulators parse out of their names into OpTypeCooperativeMatrixKHR with the
// declared rows and columns, and the online-softmax kernel that multiplies through them validates.
[test(description = "The cm2 h256 flash tile shapes build through the name-parsed marker rail.")]
def test_coopmat2_fa256_tiles(t : T?) {
    t |> run("cm2 h256 flash tiles: A 64x256, B 256x32, B 32x256, acc 64x256 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2fa256_words()
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR")
        // acc f32 64x256, A f16 64x256, acc f32 64x32, B f16 256x32, A f16 64x32, B f16 32x32, B f16 32x256 - one type each
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 7,
            "seven OpTypeCooperativeMatrixKHR (the h256 loop's shapes)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 3, "three OpCooperativeMatrixMulAddKHR (S, rowsum, O)")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}

// The h512 flash-attention tiles: coopmatWgA_f16_64x512, coopmatWgB_f16_512x32, coopmatWgB_f16_32x512 and
// the 64x512 accumulators - f32 and the f16 O beside it - parse out of their names into
// OpTypeCooperativeMatrixKHR, the f16-accumulator MulAdd and the f16-to-f32 convert lower, and the
// online-softmax kernel that multiplies through them validates.
[test(description = "The cm2 h512 flash tile shapes in f32 and f16 build through the name-parsed marker rail.")]
def test_coopmat2_fa512_tiles(t : T?) {
    t |> run("cm2 h512 flash tiles: A 64x512, B 512x32, B 32x512, acc 64x512 in f32 and f16 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2fa512_words()
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR")
        // acc f32 64x512, A f16 64x512, acc f32 64x32, B f16 512x32, A f16 64x32, B f16 32x32, B f16 32x512, acc f16 64x512 - one type each
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 8,
            "eight OpTypeCooperativeMatrixKHR (the h512 loop's shapes, the f16 O beside the f32)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 4, "four OpCooperativeMatrixMulAddKHR (S, rowsum, O in f32, O in f16)")
        t |> success(count_op(words, SpvOp.FConvert) >= 2, "the f16 O widens to f32 through OpFConvert beside the Q narrow")
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6, "SPIR-V header version 1.6")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}

//! the head-96 flash-tile shapes: A 64x96 x B 96x32 into a 64x32
//! accumulator (the Q K^T step), A 64x32 x B 32x96 into a 64x96 one
//! (the P V step), with the f32 -> f16 convert of the 64x96 tile;
//! spirv-val (vulkan1.3) is the independent witness
struct Cm2H96Args {
    n : uint
}

class Cm2H96Cls {
    @ssbo @binding = 0 qa : array<float16>
    @ssbo @binding = 1 kb : array<float16>
    @ssbo @binding = 2 vb : array<float16>
    @ssbo @binding = 3 yc : array<float16>
    @push_constant pa : Cm2H96Args

    [spirv_kernel(local_size_x = 256, name = "cm2_h96_spv")]
    def run {
        var tlq : tensorLayout2D
        tensorLayoutCreate(tlq)
        tensorLayoutSetDimension(tlq, 64u, 96u)
        tensorLayoutSetStride(tlq, 96u, 1u)
        var tlk : tensorLayout2D
        tensorLayoutCreate(tlk)
        tensorLayoutSetDimension(tlk, 96u, pa.n)
        tensorLayoutSetStride(tlk, pa.n, 1u)
        var tlv : tensorLayout2D
        tensorLayoutCreate(tlv)
        tensorLayoutSetDimension(tlv, pa.n, 96u)
        tensorLayoutSetStride(tlv, 96u, 1u)
        var tlo : tensorLayout2D
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, 64u, 96u)
        tensorLayoutSetStride(tlo, 96u, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var q : coopmatWgA_f16_64x96
        coopmatLoadTensor(q, qa, 0u, tlq, 0u, 64u, 0u, 96u, tv)
        var o : coopmatWgAcc_f32_64x96
        var kc = 0u
        while (kc < pa.n) {
            var s : coopmatWgAcc_f32_64x32
            var k : coopmatWgB_f16_96x32
            coopmatLoadTensor(k, kb, 0u, tlk, 0u, 96u, kc, 32u, tv)
            s = coopmatMulAdd(q, k, s)
            var p : coopmatWgA_f16_64x32
            coopmatConvert(p, s)
            var v : coopmatWgB_f16_32x96
            coopmatLoadTensor(v, vb, 0u, tlv, kc, 32u, 0u, 96u, tv)
            o = coopmatMulAdd(p, v, o)
            kc += 32u
        }
        coopmatClamp(o, -65504.0, 65504.0)
        var oh : coopmatWgAcc_f16_64x96
        coopmatConvert(oh, o)
        coopmatStoreTensor(oh, yc, 0u, tlo, 0u, 64u, 0u, 96u, tv)
    }
}

[test(description = "The cm2 head-96 flash shapes (64x96 by 96x32 and 64x32 by 32x96) emit and validate.")]
def test_coopmat2_h96(t : T?) {
    t |> run("cm2 head-96 flash shapes: 64x96 x 96x32 and 64x32 x 32x96") <| @(t : T?) {
        //! A 64x96, B 96x32, acc f32 64x32, A 64x32, B 32x96, acc f32 64x96,
        //! acc f16 64x96
        t |> success(count_op(cm2_h96_spv, SpvOp.TypeCooperativeMatrixKHR) == 7,
            "seven OpTypeCooperativeMatrixKHR (two A, two B, two f32 accumulators, the f16 one)")
        t |> success(count_op(cm2_h96_spv, SpvOp.CooperativeMatrixMulAddKHR) == 2,
            "two OpCooperativeMatrixMulAddKHR (Q K^T, then P V)")
        t |> success(count_op(cm2_h96_spv, SpvOp.CooperativeMatrixLoadTensorNV) == 3,
            "three OpCooperativeMatrixLoadTensorNV (Q, K, V)")
        t |> success(count_op(cm2_h96_spv, SpvOp.CooperativeMatrixStoreTensorNV) == 1,
            "one OpCooperativeMatrixStoreTensorNV")
        let r = validate_spirv(cm2_h96_spv, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
    }
}

// The cm2 s-tile shapes - the MoE expert-bucket geometry at both k
// steps the s stamps run - and the int8 workgroup tiles with their
// scaled s32 -> f32 fold emit as OpTypeCooperativeMatrixKHR with the
// rows and columns the kernels declare, and validate.
//! the shape set is the claim: a fixture's four tile types at exactly these rows x cols, one MulAdd, the f16 -> f32 convert
def private check_cm2_tiles(t : T?; words : array<uint>; lbl : string; want : array<string>) {
    t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6, "{lbl}: SPIR-V header version 1.6")
    t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)), "{lbl}: OpCapability CooperativeMatrixKHR")
    var got <- spv_coopmat_shapes(words)
    t |> equal(want, got, "{lbl}: the sorted OpTypeCooperativeMatrixKHR shape set - the two operand tiles and the f16 and f32 accumulators")
    delete got
    t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1, "{lbl}: one OpCooperativeMatrixMulAddKHR")
    t |> success(count_op(words, SpvOp.FConvert) >= 1, "{lbl}: the f16 -> f32 accumulator convert emits OpFConvert")
    let r = validate_spirv(words, "vulkan1.3")
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val (vulkan1.3): {r.msg}")
    }
}

[test(description = "The cm2 s-stamp tile shapes at the 32-deep and 64-deep k steps build through the name-parsed marker rail.")]
def test_coopmat2_s_tiles(t : T?) {
    t |> run("cm2 s-stamp shapes at the 32-deep k step: A 128x32, B 32x32, acc f16 128x32, acc f32 128x32 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2s_words()
        var want <- ["128x32", "128x32", "128x32", "32x32"]
        check_cm2_tiles(t, words, "s 32-deep", want)
        delete want
        delete words
    }
    t |> run("cm2 s-stamp shapes at the 64-deep k step: A 128x64, B 64x32, the same accumulators") <| @(t : T?) {
        var words <- coopmat2s64_words()
        var want <- ["128x32", "128x32", "128x64", "64x32"]
        check_cm2_tiles(t, words, "s 64-deep", want)
        delete want
        delete words
    }
}

[test(description = "The int8 workgroup tiles with s32 and f32 64x64 accumulators build through the marker rail.")]
def test_coopmat2_i8_tiles(t : T?) {
    t |> run("the int8 workgroup tiles: A s8 64x32, B s8 32x64, the s32 and f32 64x64 " +
            "accumulators, the s32 tile converted and folded under the per-element " +
            "function the emitter synthesizes over the shared scale arrays") <| @(t : T?) {
        var words <- coopmat2i8_words()
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6,
            "i8: SPIR-V header version 1.6")
        for (cap in [SpvCapability.CooperativeMatrixKHR, SpvCapability.StorageBuffer8BitAccess,
                SpvCapability.CooperativeMatrixTensorAddressingNV,
                SpvCapability.CooperativeMatrixPerElementOperationsNV]) {
            t |> success(op_has_operand(words, SpvOp.Capability, uint(cap)),
                "i8: OpCapability {cap}")
        }
        var got <- spv_coopmat_shapes(words)
        var want <- ["32x64", "64x32", "64x64", "64x64"]
        t |> equal(want, got, "i8: the sorted OpTypeCooperativeMatrixKHR shape set - " +
            "the two int8 operand tiles and the two accumulators")
        delete got
        delete want
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "i8: one OpCooperativeMatrixMulAddKHR")
        t |> success(count_op(words, SpvOp.ConvertSToF) == 1,
            "i8: the s32 tile converts once (OpConvertSToF)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixPerElementOpNV) == 1,
            "i8: one OpCooperativeMatrixPerElementOpNV")
        t |> success(count_op(words, SpvOp.FConvert) == 0, "i8: no float width convert")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "i8: spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (no CI lane runs spirv-val)\n")
        }
        delete words
    }
}
