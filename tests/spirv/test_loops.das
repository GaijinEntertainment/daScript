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

// ===== Phase-6 control-flow coverage (constructs the Phase-2 fixtures left unexercised). The real
// proof is spirv-val (structured-CFG validity) plus the byte-exact golden gate (test_golden.das) that
// confirms each is identical between the hand-walker and the AstVisitor. =====

[test]
def test_nested_loops(t : T?) {
    t |> run("nested for-in-while with break/continue in both loops") <| @(t : T?) {
        var words <- nloop_words()
        // two loops -> two OpLoopMerge; the merge/continue branch targets must validate
        check_ops(t, words, "nloop", [SpvOp.LoopMerge, SpvOp.BranchConditional, SpvOp.Branch,
            SpvOp.UGreaterThan, SpvOp.IEqual, SpvOp.SLessThan])
        t |> success(count_op(words, SpvOp.LoopMerge) == 2, "nloop: emits exactly two OpLoopMerge (nested loops)")
        validate(t, words, "nloop")
        delete words
    }
}

[test]
def test_elif_chain(t : T?) {
    t |> run("if/elif/elif/else -> nested OpSelectionMerge") <| @(t : T?) {
        var words <- echain_words()
        check_ops(t, words, "echain", [SpvOp.SelectionMerge, SpvOp.BranchConditional, SpvOp.Branch,
            SpvOp.SLessThan])
        // four arms -> three condition tests -> three OpSelectionMerge (nested in each if_false)
        t |> success(count_op(words, SpvOp.SelectionMerge) == 3, "echain: three OpSelectionMerge (elif chain)")
        validate(t, words, "echain")
        delete words
    }
}

[test]
def test_early_return_in_loop(t : T?) {
    t |> run("early return inside a for-loop (terminated then-block + live merge)") <| @(t : T?) {
        var words <- eret_words()
        check_ops(t, words, "eret", [SpvOp.LoopMerge, SpvOp.SelectionMerge, SpvOp.Return,
            SpvOp.SGreaterThan])
        // the in-loop early return + the function's trailing return
        t |> success(count_op(words, SpvOp.Return) == 2, "eret: two OpReturn (early + trailing)")
        validate(t, words, "eret")
        delete words
    }
}

[test]
def test_runtime_for_and_bool_local(t : T?) {
    t |> run("runtime urange(0u, n) bound + bool Function OpVariable") <| @(t : T?) {
        var words <- rfor_words()
        check_ops(t, words, "rfor", [SpvOp.LoopMerge, SpvOp.BranchConditional, SpvOp.ULessThan,
            SpvOp.TypeBool, SpvOp.UGreaterThan])
        validate(t, words, "rfor")
        delete words
    }
}
