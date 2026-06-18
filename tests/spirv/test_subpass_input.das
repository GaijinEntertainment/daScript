options gen2
options indenting = 4

// Subpass input attachment: a Vulkan deferred-rendering rail (tutorial 10 lighting subpass). A
// shader declares `var @set @binding @input_attachment_index = N x : subpassInput` and reads it
// via `subpassLoad(x)`. The emitter lowers this to:
//   * OpTypeImage Dim=SubpassData (Sampled=2, no format), UniformConstant storage class
//   * Decorate DescriptorSet, Binding, InputAttachmentIndex
//   * OpCapability InputAttachment
//   * Reflection: SpirvDescriptorKind.input_attachment
//   * subpassLoad -> OpLoad + OpImageRead at int2(0, 0)

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_reflect

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_subpass_input(t : T?) {
    t |> run("subpass input: subpassInput global + subpassLoad -> OpImageRead") <| @(t : T?) {
        var words <- subpass_words()
        // Fragment shader (subpassLoad is fragment-only in practice -- the OpImageRead at
        // current-fragment offset only makes sense per fragment).
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "subpass: EntryPoint model is Fragment")
        // InputAttachment capability declared (required for Dim=SubpassData).
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.InputAttachment)),
            "subpass: OpCapability InputAttachment emitted")
        // InputAttachmentIndex decoration on the global (idx 1 in OpDecorate after the target id).
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.InputAttachmentIndex)),
            "subpass: InputAttachmentIndex decoration emitted")
        // DescriptorSet + Binding decorations (set=0, binding=0 per the fixture).
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.DescriptorSet)),
            "subpass: DescriptorSet decoration emitted")
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Binding)),
            "subpass: Binding decoration emitted")
        // OpImageRead is the read instruction (vs OpImageSample* for textures).
        t |> success(has_op(words, SpvOp.ImageRead), "subpass: subpassLoad -> OpImageRead")
        // Reflection: the binding is registered as input_attachment (the host needs to bind it as
        // VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, distinct from combined_image_sampler / storage_image).
        let refl = decode_reflection(subpass_spv_reflect)
        t |> success(length(refl.bindings) >= 1, "subpass: at least one descriptor binding reflected")
        if (length(refl.bindings) >= 1) {
            let b = refl.bindings[0]
            t |> success(b.kind == SpirvDescriptorKind.input_attachment,
                "subpass: binding kind is input_attachment (got {b.kind})")
            t |> success(b.set == 0 && b.binding == 0,
                "subpass: binding at (set=0, binding=0), got (set={b.set}, binding={b.binding})")
        }
        // spirv-val (CI gate) checks the structural CFG + Sampled=2 + Dim=SubpassData semantics.
        validate(t, words, "subpass")
        delete words
    }
}
