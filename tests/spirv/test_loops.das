options gen2
options indenting = 4

// Phase-2 loop gate: while + range-for lowered to the structured 4-block OpLoopMerge shape, with
// break (-> loop merge) and continue (-> loop continue block). Opcode-shape asserts plus spirv-val,
// which is the real oracle here: an invalid loop CFG (bad merge/continue targets, a block without a
// terminator, code after a terminator) fails validation even when every opcode is "present".

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
def test_while_loop(t : T?) {
    t |> run("while loop with break: OpLoopMerge structured CFG") <| @(t : T?) {
        var words <- wloop_words()
        check_ops(t, words, "wloop", [SpvOp.LoopMerge, SpvOp.BranchConditional, SpvOp.Branch,
            SpvOp.Label, SpvOp.UGreaterThan, SpvOp.IAdd, SpvOp.ISub])
        validate(t, words, "wloop")
        delete words
    }
}

[test]
def test_range_for(t : T?) {
    t |> run("range-for with continue: range(n) + range(lo,hi)") <| @(t : T?) {
        var words <- floop_words()
        check_ops(t, words, "floop", [SpvOp.LoopMerge, SpvOp.BranchConditional, SpvOp.Branch,
            SpvOp.SLessThan, SpvOp.IEqual, SpvOp.IAdd, SpvOp.ISub])
        validate(t, words, "floop")
        delete words
    }
}
