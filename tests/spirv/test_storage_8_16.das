options gen2
options indenting = 4
options no_unused_block_arguments = false

// 8/16-bit SSBO storage (std430): the block_q8_0-mirror struct lays out tight (runtime
// ArrayStride 34, member Offsets 0/2, qs element stride 1), half4 elements stride 8, the
// StorageBuffer8/16BitAccess capabilities + the SPV_KHR_8bit_storage extension are pulled, and
// unpack8/pack32 lower to OpBitcast with OpSConvert sign-extended widening. @uniform /
// @push_constant sub-32 members stay fail-closed (std140) — pinned by _fc_lattice_ubo /
// _fc_lattice_push in test_conf_rejects.das.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

[test]
def test_storage_8_16(t : T?) {
    t |> run("block_q8_0 std430 layout + storage caps + unpack8/pack32; spirv-val clean") <| @(t : T?) {
        var words <- q8blk_words()
        // std430 block_q8_0: 34-byte block stride, d at offset 0, qs at offset 2, 1-byte elements
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride), 34u), "runtime ArrayStride 34")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride), 1u), "qs ArrayStride 1")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride), 8u), "half4 ArrayStride 8")
        t |> success(op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), 2u), "qs at Offset 2 (d is 2 bytes)")
        // storage caps + the 8-bit extension (16-bit storage is cap-only at SPIR-V 1.3)
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.StorageBuffer8BitAccess)), "StorageBuffer8BitAccess")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.StorageBuffer16BitAccess)), "StorageBuffer16BitAccess")
        t |> equal(count_op(words, SpvOp.Extension), 1)
        // unpack8 + pack32 -> OpBitcast; int4(b4) -> sign-extending OpSConvert; f16 loads -> OpFConvert
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
        // int8(v)/int16(v) narrow via OpSConvert; uint16(uint) via OpUConvert
        t |> success(count_op(words, SpvOp.SConvert) >= 3, "signed narrowing converts present")
        t |> success(has_op(words, SpvOp.UConvert), "unsigned narrowing convert present")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.StorageBuffer8BitAccess)), "StorageBuffer8BitAccess")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.StorageBuffer16BitAccess)), "StorageBuffer16BitAccess")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
