options gen2
options indenting = 4

// Phase-7.1a scalar conversions: float(int/uint), int(float/uint), uint(float/int) lower to
// OpConvertSToF/UToF/FToS/FToU and OpBitcast (int<->uint same-width reinterpret). The blob is spirv-val'd.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
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
