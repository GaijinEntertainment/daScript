options gen2
options indenting = 4

// Phase-8.3 GLSL.std.450 math breadth. The opcode census tracks OpExtInst as a single opcode, so it
// cannot enforce per-function coverage; this test pins each ext-inst by its GLSLstd450 sub-opcode
// (OpExtInst operand index 3 = the instruction literal). The blob is also spirv-val'd, which is the
// real per-function oracle (it type-checks each ext-inst's operands).

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

def private has_ext(words : array<uint>; op : GLSLstd450) : bool {
    return op_has_operand_at(words, SpvOp.ExtInst, 3, uint(op))   // operand 3 = GLSLstd450 instruction literal
}

[test]
def test_glsl_std450_breadth(t : T?) {
    t |> run("GLSL.std.450: every reachable ext-inst maps to its sub-opcode") <| @(t : T?) {
        var words <- mathx_words()
        // trig
        t |> success(has_ext(words, GLSLstd450.Sin), "sin -> Sin")
        t |> success(has_ext(words, GLSLstd450.Cos), "cos -> Cos")
        t |> success(has_ext(words, GLSLstd450.Tan), "tan -> Tan")
        t |> success(has_ext(words, GLSLstd450.Asin), "asin -> Asin")
        t |> success(has_ext(words, GLSLstd450.Acos), "acos -> Acos")
        t |> success(has_ext(words, GLSLstd450.Atan), "atan -> Atan")
        t |> success(has_ext(words, GLSLstd450.Atan2), "atan2 -> Atan2")
        // exponential
        t |> success(has_ext(words, GLSLstd450.Pow), "pow -> Pow")
        t |> success(has_ext(words, GLSLstd450.Exp), "exp -> Exp")
        t |> success(has_ext(words, GLSLstd450.Log), "log -> Log")
        t |> success(has_ext(words, GLSLstd450.Exp2), "exp2 -> Exp2")
        t |> success(has_ext(words, GLSLstd450.Log2), "log2 -> Log2")
        t |> success(has_ext(words, GLSLstd450.InverseSqrt), "rsqrt -> InverseSqrt")
        // rounding / fraction
        t |> success(has_ext(words, GLSLstd450.Floor), "floor -> Floor")
        t |> success(has_ext(words, GLSLstd450.Ceil), "ceil -> Ceil")
        t |> success(has_ext(words, GLSLstd450.Round), "round -> Round")
        t |> success(has_ext(words, GLSLstd450.Trunc), "trunc -> Trunc")
        t |> success(has_ext(words, GLSLstd450.Fract), "fract -> Fract")
        // common
        t |> success(has_ext(words, GLSLstd450.FAbs), "abs -> FAbs")
        t |> success(has_ext(words, GLSLstd450.FSign), "sign -> FSign")
        t |> success(has_ext(words, GLSLstd450.FMin), "min -> FMin")
        t |> success(has_ext(words, GLSLstd450.FMax), "max -> FMax")
        t |> success(has_ext(words, GLSLstd450.FMix), "lerp -> FMix")
        // geometric (vector)
        t |> success(has_ext(words, GLSLstd450.Length), "length -> Length")
        t |> success(has_ext(words, GLSLstd450.Distance), "distance -> Distance")
        t |> success(has_ext(words, GLSLstd450.Cross), "cross -> Cross")
        t |> success(has_ext(words, GLSLstd450.Normalize), "normalize -> Normalize")
        t |> success(has_ext(words, GLSLstd450.Reflect), "reflect -> Reflect")
        t |> success(has_ext(words, GLSLstd450.Refract), "refract -> Refract")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "mathx: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
