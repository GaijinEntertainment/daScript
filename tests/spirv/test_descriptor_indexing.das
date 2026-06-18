options gen2
options indenting = 4

// Bindless / VK_EXT_descriptor_indexing surface. PR-B ships the nonuniformEXT marker (OpCopyObject +
// OpDecorate NonUniform on the fresh id, OpCapability ShaderNonUniform + OpExtension
// "SPV_EXT_descriptor_indexing" once per module). PR-C adds sampler descriptor arrays
// (sampler2D[N] -> UniformConstant OpTypeArray of OpTypeSampledImage) with NonUniform cascade onto
// the access-chain pointer + loaded sampler when the index carries the marker. Every emitted
// module passes spirv-val.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require daslib/strings_boost

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

// OpExtension's operand words encode the extension-name string (LSB-first bytes, NUL-terminated,
// zero-padded — same form as OpExtInstImport / OpEntryPoint's name slot). Reassemble the bytes across
// each OpExtension and substring-search the requested name.
def private has_extension(words : array<uint>; want : string) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (found || opcode != uint(SpvOp.Extension)) return
        let name = build_string() $(var wr) {
            for (k in range(n)) {
                let wd = w[s + k]
                for (i in range(4)) {
                    let b = uint8((wd >> uint(i * 8)) & 0xFFu)
                    if (b == 0u8) return
                    wr |> write_char(int(b))
                }
            }
        }
        if (find(name, want) >= 0) {
            found = true
        }
    }
    return found
}

[test]
def test_nonuniform_ext_marker(t : T?) {
    t |> run("nonuniformEXT(x) -> OpCopyObject + OpDecorate NonUniform + cap/extension") <| @(t : T?) {
        var words <- nuext_words()
        // OpCapability ShaderNonUniform (the base bindless cap; specific NonUniformIndexing caps land
        // when the access actually hits a typed descriptor array, which is PR-C scope).
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ShaderNonUniform)),
            "nuext: OpCapability ShaderNonUniform")
        let ncap_nu = count_op_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ShaderNonUniform))
        t |> success(ncap_nu == 1, "nuext: exactly one ShaderNonUniform cap (got {ncap_nu})")
        // OpExtension "SPV_EXT_descriptor_indexing" emitted once.
        t |> success(has_extension(words, "SPV_EXT_descriptor_indexing"),
            "nuext: OpExtension SPV_EXT_descriptor_indexing")
        t |> success(count_op(words, SpvOp.Extension) == 1,
            "nuext: exactly one OpExtension (got {count_op(words, SpvOp.Extension)})")
        // OpCopyObject: the fresh id we decorate (one per nonuniformEXT call in this shader).
        t |> success(has_op(words, SpvOp.CopyObject), "nuext: emits OpCopyObject")
        t |> success(count_op(words, SpvOp.CopyObject) == 1,
            "nuext: exactly one OpCopyObject (got {count_op(words, SpvOp.CopyObject)})")
        // OpDecorate <id> NonUniform on the fresh id. Operand at index 1 is the decoration enum.
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.NonUniform)),
            "nuext: OpDecorate <id> NonUniform")
        let nnu = count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.NonUniform))
        t |> success(nnu == 1, "nuext: exactly one NonUniform decoration (got {nnu})")
        validate(t, words, "nuext")
        delete words
    }
}

// Sampler descriptor array: sampler2D[8] -> OpTypeArray of OpTypeSampledImage in UniformConstant
// storage with DescriptorSet/Binding decorations; texture(arr[i], uv) lowers to OpAccessChain ->
// OpLoad -> OpImageSampleImplicitLod. nonuniformEXT on the index cascades NonUniform onto the
// access-chain pointer + the loaded sampler and pulls in the per-resource-type
// SampledImageArrayNonUniformIndexing capability.
[test]
def test_sampler_descriptor_array(t : T?) {
    t |> run("sampler2D[N] + texture(arr[nonuniformEXT(idx)], uv) -> array + access-chain + cascade") <| @(t : T?) {
        var words <- darr_words()
        // OpTypeArray of OpTypeSampledImage built once.
        t |> success(has_op(words, SpvOp.TypeArray), "darr: emits OpTypeArray")
        t |> success(count_op(words, SpvOp.TypeArray) == 1,
            "darr: exactly one OpTypeArray (got {count_op(words, SpvOp.TypeArray)})")
        // DescriptorSet/Binding on the array variable (count=8 lives on the host side; spirv-val
        // doesn't care about it).
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.DescriptorSet)),
            "darr: OpDecorate DescriptorSet")
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Binding)),
            "darr: OpDecorate Binding")
        // OpAccessChain: the array-element pointer derived from the array variable + idx.
        t |> success(has_op(words, SpvOp.AccessChain), "darr: emits OpAccessChain")
        // SampledImageArrayNonUniformIndexing capability: required because the index carries
        // NonUniform (via nonuniformEXT). The base ShaderNonUniform cap rides along from PR-B.
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.SampledImageArrayNonUniformIndexing)),
            "darr: OpCapability SampledImageArrayNonUniformIndexing")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ShaderNonUniform)),
            "darr: OpCapability ShaderNonUniform")
        // Cascade rule: three NonUniform decorations -- on the nonuniformEXT OpCopyObject result, on
        // the OpAccessChain pointer it indexes through, and on the OpLoad'd sampled image.
        let nnu = count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.NonUniform))
        t |> success(nnu == 3, "darr: 3 NonUniform decorations (marker + access-chain + load) (got {nnu})")
        // OpImageSampleImplicitLod still lands as the actual sample (fragment-stage texture()).
        t |> success(has_op(words, SpvOp.ImageSampleImplicitLod),
            "darr: emits OpImageSampleImplicitLod on the loaded sampler")
        validate(t, words, "darr")
        delete words
    }
}
