options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_shader
require spirv/spirv_builtins public
require spirv/spirv_reflect
require _texture_grad_shaders

// Phase-4d combined image sampler gate: a sampler2D global lowers to OpTypeImage + OpTypeSampledImage
// in UniformConstant storage with DescriptorSet/Binding; texture(tex, uv) lowers to OpLoad (the
// sampled image) + OpImageSampleImplicitLod. The blob is spirv-val'd.
def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "A fragment shader sampling a sampler2D emits an OpTypeImage and OpTypeSampledImage in UniformConstant storage and an implicit-lod sample.")]
def test_sampler_fragment(t : T?) {
    t |> run("sampler: sampler2D -> OpTypeImage/SampledImage (UniformConstant) + texture() sample") <| @(t : T?) {
        var words <- tex_frag_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "tex_frag: EntryPoint model is Fragment")
        // the image type pair
        t |> success(has_op(words, SpvOp.TypeImage), "tex_frag: emits OpTypeImage")
        t |> success(has_op(words, SpvOp.TypeSampledImage), "tex_frag: emits OpTypeSampledImage")
        // a UniformConstant-storage OpVariable (the sampler descriptor) at operand index 2
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.UniformConstant)),
            "tex_frag: has a UniformConstant-storage OpVariable")
        // descriptor decorations
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.DescriptorSet), 0u),
            "tex_frag: DescriptorSet 0")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u),
            "tex_frag: Binding 0")
        // the sample: OpLoad the sampled image + OpImageSampleImplicitLod
        t |> success(has_op(words, SpvOp.Load), "tex_frag: OpLoad the sampled image")
        t |> success(has_op(words, SpvOp.ImageSampleImplicitLod), "tex_frag: texture() -> OpImageSampleImplicitLod")
        validate(t, words, "tex_frag")
        delete words
    }
}

// samplerCubeArray gate -- the sampled counterpart of imageCubeArray.
//
// A LAYERED cubemap sampled through a RUNTIME-SIZED descriptor array (GLSL `uniform samplerCubeArray
// pool[]`), which is how a pool of cube textures is bound: the host sizes the pool, so the array is an
// OpTypeRuntimeArray of OpTypeSampledImage in UniformConstant storage (no ArrayStride -- a descriptor
// slot has no byte layout) and the whole read is OpAccessChain + OpLoad + OpImageSampleExplicitLod.
//
// Two capabilities ride along and are BOTH pinned: SampledCubeArray for the Arrayed=1 cube image, and
// RuntimeDescriptorArray for the unsized array. The coordinate is a float4 -- .xyz the direction that
// picks the face, .w the cube layer -- and the Lod operand is what makes the sample stage-agnostic
// (an implicit-lod texture() would be fragment-only, and is deliberately not declared).
var @binding = 6 sca_cubes : array<samplerCubeArray>
var @ssbo @binding = 7 sca_slot : array<int>
var @ssbo @binding = 8 sca_out : array<float4>

[compute_shader(local_size_x=64, name="sca_spv")]
def sca_comp {
    let i = int(gl_GlobalInvocationID.x)
    let dir = float3(0.0, 1.0, 0.0)
    sca_out[i] = textureLod(sca_cubes[sca_slot[i]], float4(dir, float(i)), 0.0)
}

[test(description = "textureLod on a samplerCubeArray with a float4 direction-and-layer lowers to OpImageSampleExplicitLod.")]
def test_sampler_cube_array(t : T?) {
    t |> run("textureLod(samplerCubeArray[idx], float4(dir, layer), lod) -> OpImageSampleExplicitLod") <| @(t : T?) {
        var words <- clone_to_move(sca_spv)
        t |> success(count_op(words, SpvOp.TypeImage) == 1, "one OpTypeImage (the cube array)")
        // OpTypeImage operands: [result_id, sampled_type, dim, depth, arrayed, ms, sampled, format].
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim.Cube)), "Dim Cube")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 4, 1u), "Arrayed 1 (a LAYERED cubemap)")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 6, 1u), "Sampled 1 (sampled, not a storage image)")
        t |> success(count_op(words, SpvOp.TypeSampledImage) == 1, "one OpTypeSampledImage (combined)")
        // The descriptor array is UNSIZED: a runtime array, never an OpTypeArray with a count.
        t |> success(count_op(words, SpvOp.TypeRuntimeArray) >= 1, "the descriptor array is an OpTypeRuntimeArray")
        // The read: chain one slot off the array, load the sampled image, sample it with an explicit Lod.
        t |> success(count_op(words, SpvOp.ImageSampleExplicitLod) == 1, "one OpImageSampleExplicitLod")
        // OpImageSampleExplicitLod operands: [result_type, result_id, sampled_image, coord, mask, lod].
        t |> success(op_has_operand_at(words, SpvOp.ImageSampleExplicitLod, 4, uint(SpvImageOperands.Lod)),
            "the sample carries an explicit Lod operand")
        // An implicit-lod sample would be fragment-only and is not declared, so none may appear.
        t |> success(count_op(words, SpvOp.ImageSampleImplicitLod) == 0, "no implicit-lod sample")
        // The capability is operand 0 of OpCapability, so an id equal to the enum cannot match.
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.SampledCubeArray)),
            "OpCapability SampledCubeArray")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.RuntimeDescriptorArray)),
            "OpCapability RuntimeDescriptorArray")
        // The host sizes the pool, so the binding reports a variable descriptor count (0).
        let refl = decode_reflection(sca_spv_reflect)
        var found = false
        for (b in refl.bindings) {
            if (b.binding == 6) {
                found = true
                t |> success(b.kind == SpirvDescriptorKind.combined_image_sampler,
                    "reflection: binding kind is combined_image_sampler (got {b.kind})")
                t |> success(b.count == 0, "reflection: variable descriptor count (got {b.count})")
            }
        }
        t |> success(found, "reflection: the cube-array pool is reported at its binding")
        // RuntimeDescriptorArray raises the module to SPIR-V 1.5, so validate against the env that
        // speaks it (Vulkan 1.2) rather than the 1.3 default.
        let r = validate_spirv(words, "vulkan1.2")
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

[test(description = "textureLod and texelFetch lower to ExplicitLod sampling and OpImage plus OpImageFetch in a compute shader.")]
def test_explicit_lod_and_fetch(t : T?) {
    t |> run("textureLod/texelFetch -> ExplicitLod + OpImage/ImageFetch (compute, stage-agnostic)") <| @(t : T?) {
        var words <- texlod_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.GLCompute),
            "texlod: EntryPoint model is GLCompute (sampling outside a fragment shader)")
        t |> success(has_op(words, SpvOp.ImageSampleExplicitLod), "texlod: textureLod -> OpImageSampleExplicitLod")
        t |> success(has_op(words, SpvOp.Image), "texlod: texelFetch extracts the image via OpImage")
        t |> success(has_op(words, SpvOp.ImageFetch), "texlod: texelFetch -> OpImageFetch")
        validate(t, words, "texlod")
        delete words
    }
}

// Read a vector constant's first component through its result ID.
def private first_lane_bits(words : array<uint>; vector_id : uint) : uint {
    var scalar_id = 0u
    var i = 5
    while (i < length(words)) {
        if ((words[i] & 0xffffu) == uint(SpvOp.ConstantComposite) && words[i + 2] == vector_id) {
            scalar_id = words[i + 3]
        }
        i += int(words[i] >> 16u)
    }
    i = 5
    while (i < length(words)) {
        if ((words[i] & 0xffffu) == uint(SpvOp.Constant) && words[i + 2] == scalar_id) {
            return words[i + 3]
        }
        i += int(words[i] >> 16u)
    }
    return 0xffffffffu
}

[test(description = "Explicit-gradient sampling of 2D, 3D, cube and 2D-array images in compute emits the Grad image operand.")]
def test_texture_grad(t : T?) {
    t |> run("explicit gradients: 2D, 3D, cube and 2D array in compute") <| @(t : T?) {
        var words <- clone_to_move(texture_grad_spv)
        t |> success(words[1] == 0x00010300u, "SPIR-V 1.3")
        t |> success(count_op(words, SpvOp.ImageSampleExplicitLod) == 4, "four explicit samples")
        t |> success(count_op(words, SpvOp.ImageSampleImplicitLod) == 0, "no implicit derivatives")
        var i = 5
        while (i < length(words)) {
            let count = int(words[i] >> 16u)
            if ((words[i] & 0xffffu) == uint(SpvOp.ImageSampleExplicitLod)) {
                t |> success(count == 8, "sample has both gradient operands")
                t |> success(words[i + 5] == uint(SpvImageOperands.Grad),
                    "Grad operand mask, not Lod")
                t |> success(first_lane_bits(words, words[i + 6]) == 0x3e800000u,
                    "dx first component is 0.25")
                t |> success(first_lane_bits(words, words[i + 7]) == 0u,
                    "dy first component is 0.0")
            }
            i += count
        }
        let validation = validate_spirv(words, "vulkan1.1")
        if (validation.ran) {
            t |> success(validation.ok, "spirv-val: {validation.msg}")
        } else {
            feint("spirv-val not found locally; skipping external validation\n")
        }
        delete words
    }
}

[test(description = "textureCompare on a sampler2DShadow lowers to OpImageSampleDrefImplicitLod.")]
def test_depth_compare(t : T?) {
    t |> run("depth-compare sampling: sampler2DShadow + textureCompare -> OpImageSampleDrefImplicitLod") <| @(t : T?) {
        var words <- fragshadow_words()
        // a fragment entry point (implicit-LOD Dref sampling is fragment-only)
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "fragshadow: EntryPoint model is Fragment")
        // textureCompare -> the depth-compare sample op
        t |> success(has_op(words, SpvOp.ImageSampleDrefImplicitLod),
            "fragshadow: textureCompare -> OpImageSampleDrefImplicitLod")
        // the sampler's OpTypeImage marks Depth=1 (comparison) and Dim=2D. OpTypeImage operands:
        // result-id(0), Sampled Type(1), Dim(2), Depth(3), Arrayed(4), MS(5), Sampled(6), Format(7).
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 3, 1u),
            "fragshadow: sampler2DShadow OpTypeImage has Depth=1")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim._2D)),
            "fragshadow: sampler2DShadow OpTypeImage Dim=2D")
        validate(t, words, "fragshadow")
        delete words
    }
}

[test(description = "A texture2D and a separate sampler emit standalone image and sampler types combined by OpSampledImage.")]
def test_separate_image_sampler(t : T?) {
    t |> run("texture2D + sampler -> standalone OpTypeImage/OpTypeSampler + OpSampledImage combine") <| @(t : T?) {
        var words <- septex_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "septex: EntryPoint model is Fragment")
        t |> success(has_op(words, SpvOp.TypeImage), "septex: a standalone OpTypeImage (texture2D)")
        t |> success(has_op(words, SpvOp.TypeSampler), "septex: a standalone OpTypeSampler (sampler)")
        t |> success(has_op(words, SpvOp.SampledImage), "septex: sampleTexture combines via OpSampledImage")
        t |> success(has_op(words, SpvOp.ImageSampleImplicitLod), "septex: sample -> OpImageSampleImplicitLod")
        // two UniformConstant descriptors at bindings 0 (image) and 1 (sampler)
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.UniformConstant)),
            "septex: UniformConstant OpVariables")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u),
            "septex: image at Binding 0")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 1u),
            "septex: sampler at Binding 1")
        validate(t, words, "septex")
        delete words
    }
}

// Uniform texel buffer gate.
//
// A textureBuffer is a buffer-backed sampled image: OpTypeImage with Dim=Buffer (Sampled=1, format
// Unknown) in UniformConstant storage, plus the SampledBuffer capability. It carries no sampler, so
// the fetch loads the image variable directly instead of extracting one from a sampled image (no
// OpImage), and it carries no mips, so the OpImageFetch has NO Lod operand -- SPIR-V forbids one on
// Dim=Buffer, which is what makes the two-argument texelFetch a distinct lowering rather than a
// spelling. The index is read off an SSBO, so it is a runtime value and not a folded constant.
var @binding = 3 tb_src : textureBuffer
var @ssbo @binding = 4 tb_idx : array<int>
var @ssbo @binding = 5 tb_out : array<float4>

[compute_shader(local_size_x=64, name="tb_spv")]
def tb_comp {
    let i = int(gl_GlobalInvocationID.x)
    tb_out[i] = texelFetch(tb_src, tb_idx[i])
}

// Operand count of the first instruction with `op` (the opcode word excluded). OpImageFetch's
// mandatory operands are result-type / result-id / image / coordinate; anything beyond them is an
// image-operands mask, so 4 is the proof that no Lod rode along.
def private operand_count(words : array<uint>; op : SpvOp) : int {
    var n = -1
    foreach_inst(words) $(opcode : uint; _w : array<uint>; _s, c : int) {
        if (opcode == uint(op) && n < 0) {
            n = c
        }
    }
    return n
}

[test(description = "texelFetch on a textureBuffer emits OpImageFetch on Dim Buffer with no OpImage and no Lod operand.")]
def test_texel_buffer(t : T?) {
    t |> run("texelFetch(textureBuffer, index) -> OpImageFetch on Dim Buffer, no OpImage, no Lod") <| @(t : T?) {
        var words <- clone_to_move(tb_spv)
        t |> success(count_op(words, SpvOp.TypeImage) == 1, "one OpTypeImage (the texel buffer)")
        // OpTypeImage operands: [result_id, sampled_type, dim, depth, arrayed, ms, sampled, format].
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim.Buffer)), "Dim Buffer")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 6, 1u), "Sampled 1 (a sampled image, not a storage one)")
        t |> success(count_op(words, SpvOp.Image) == 0, "no OpImage (the global IS the image)")
        t |> success(count_op(words, SpvOp.ImageFetch) == 1, "one OpImageFetch")
        t |> success(operand_count(words, SpvOp.ImageFetch) == 4, "OpImageFetch carries no image operands (no Lod)")
        // The capability is operand 0 of OpCapability, so an id equal to the enum cannot match.
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.SampledBuffer)),
            "OpCapability SampledBuffer")
        // The host binds a buffer view, not an image view, so the descriptor kind is its own.
        let refl = decode_reflection(tb_spv_reflect)
        var found = false
        for (b in refl.bindings) {
            if (b.binding == 3) {
                found = true
                t |> success(b.kind == SpirvDescriptorKind.uniform_texel_buffer,
                    "reflection: binding kind is uniform_texel_buffer (got {b.kind})")
            }
        }
        t |> success(found, "reflection: the texel buffer is reported at its binding")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// texelFetch on a layered sampler gate.
//
// A sampler2DArray indexes (x, y, layer), and the layer is a plain array index rather than a
// normalized coordinate, so the fetch coordinate is int3 where the sampler2D form takes int2. The
// lowering is the 2D one unchanged -- OpImage to extract the image from the sampled image, then
// OpImageFetch with a Lod operand -- against an OpTypeImage carrying Dim 2D and Arrayed 1. The
// coordinate is built from gl_FragCoord, so it is a runtime value and not a folded constant.
var @binding = 0 tfa_tex : sampler2DArray
var @out @location = 0 tfa_out : float4

[fragment_shader(name="tfa_spv"), marker(no_coverage)]
def tfa_frag {
    let p = int2(gl_FragCoord.xy)
    tfa_out = texelFetch(tfa_tex, int3(p.x, p.y, 2), 0)
}

[test(description = "texelFetch on a sampler2DArray with an int3 coordinate and lod emits OpImage and OpImageFetch on an arrayed 2D image.")]
def test_texel_fetch_array(t : T?) {
    t |> run("texelFetch(sampler2DArray, int3, lod) -> OpImage + OpImageFetch on Dim 2D Arrayed 1") <| @(t : T?) {
        var words <- clone_to_move(tfa_spv)
        t |> success(count_op(words, SpvOp.TypeImage) == 1, "one OpTypeImage (the layered sampler)")
        // OpTypeImage operands: [result_id, sampled_type, dim, depth, arrayed, ...]. Dim at index 2,
        // Arrayed at index 4 -- the pair that distinguishes sampler2DArray from a plain sampler2D.
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim._2D)), "Dim 2D")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 4, 1u), "Arrayed 1")
        t |> success(count_op(words, SpvOp.Image) == 1, "one OpImage (image extracted from the sampled image)")
        t |> success(count_op(words, SpvOp.ImageFetch) == 1, "one OpImageFetch")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

[test(description = "samplerCube, sampler3D and sampler2DArray emit OpTypeImage with the Cube, 3D and Arrayed dimensions.")]
def test_sampler_dims(t : T?) {
    t |> run("samplerCube/3D/2DArray -> OpTypeImage Dim Cube/3D + Arrayed") <| @(t : T?) {
        var words <- dims_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "dims: EntryPoint model is Fragment")
        // OpTypeImage operands: [result_id, sampled_type, dim, depth, arrayed, ...]. Dim at index 2,
        // Arrayed at index 4. Three image types are emitted; assert each distinguishing operand is present.
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim.Cube)),
            "dims: a samplerCube image (Dim Cube)")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim._3D)),
            "dims: a sampler3D image (Dim 3D)")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 4, 1u),
            "dims: a sampler2DArray image (Arrayed = 1)")
        t |> success(has_op(words, SpvOp.ImageSampleImplicitLod), "dims: texture() -> OpImageSampleImplicitLod")
        validate(t, words, "dims")
        delete words
    }
}

[test(description = "A subpassInput global read through subpassLoad lowers to OpImageRead.")]
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
        t |> success(!empty(refl.bindings), "subpass: at least one descriptor binding reflected")
        if (!empty(refl.bindings)) {
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
