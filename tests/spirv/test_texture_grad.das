options gen2
require dastest/testing_boost public
require _spirv_common
require _texture_grad_shaders
require spirv/spirv_dis
require spirv/spirv_grammar

// Read the first component of a vector constant through its result ID.
def private first_lane_bits(var words : array<uint>; vector_id : uint) : uint {
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

[test]
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
                t |> success(words[i + 5] == uint(SpvImageOperands.Grad), "Grad operand mask, not Lod")
                t |> success(first_lane_bits(words, words[i + 6]) == 0x3e800000u, "dx first component is 0.25")
                t |> success(first_lane_bits(words, words[i + 7]) == 0u, "dy first component is 0.0")
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
