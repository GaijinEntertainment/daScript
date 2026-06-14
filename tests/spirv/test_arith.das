options gen2
options indenting = 4

// Phase-2 arithmetic gate: every binary/unary scalar op the emitter supports is present in the
// emitted blob (opcode-shape), and each whole module passes spirv-val. Operands are loaded SSBO
// values, so these isolate the arithmetic opcode selection (signed/unsigned/float) from constants
// and conversions (which arrive with control flow).

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

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

[test]
def test_uint_arith(t : T?) {
    t |> run("uint arithmetic: IAdd/ISub/IMul/UDiv/UMod") <| @(t : T?) {
        var words <- uarith_words()
        check_ops(t, words, "uarith", [SpvOp.IAdd, SpvOp.ISub, SpvOp.IMul, SpvOp.UDiv, SpvOp.UMod])
        validate(t, words, "uarith")
        delete words
    }
}

[test]
def test_int_arith(t : T?) {
    t |> run("int arithmetic: SDiv/SRem/SNegate/Not") <| @(t : T?) {
        var words <- iarith_words()
        check_ops(t, words, "iarith", [SpvOp.SDiv, SpvOp.SRem, SpvOp.SNegate, SpvOp.Not])
        validate(t, words, "iarith")
        delete words
    }
}

[test]
def test_float_arith(t : T?) {
    t |> run("float arithmetic: FAdd/FSub/FMul/FDiv/FNegate") <| @(t : T?) {
        var words <- farith_words()
        check_ops(t, words, "farith", [SpvOp.FAdd, SpvOp.FSub, SpvOp.FMul, SpvOp.FDiv, SpvOp.FNegate])
        validate(t, words, "farith")
        delete words
    }
}
