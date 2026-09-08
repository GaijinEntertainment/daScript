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
def test_unroll_full(t : T?) {
    t |> run("for [unroll_full] with literal bounds: emitted as copies, indices folded to constants") <| @(t : T?) {
        var words <- ufor_words()
        t |> equal(count_op(words, SpvOp.LoopMerge), 0, "ufor: no loop construct survives the unroll")
        t |> equal(count_op(words, SpvOp.IMul), 0, "ufor: every index multiply folded at emission")
        t |> equal(count_op(words, SpvOp.IAdd), 8, "ufor: one add per copy takes the invocation id; the index adds folded")
        let fc = count_function_storage_chains(words)
        t |> equal(fc.chains, 23, "ufor: 8 + 8 + 7 element chains into the register block")
        t |> equal(fc.dynamic_indices, 0, "ufor: every chain into the block indexes with an OpConstant")
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_3, "ufor: SPIR-V header version stays 1.3")
        validate(t, words, "ufor")
        delete words
    }
    t |> run("for [unroll_full] over a module constant: every integer fold lands, no integer op survives") <| @(t : T?) {
        var words <- ufold_words()
        t |> equal(count_op(words, SpvOp.LoopMerge), 0, "ufold: the named-constant bound unrolls like a literal")
        t |> equal(count_op(words, SpvOp.FAdd), 44, "ufold: 4 copies x 10 terms over the named-constant bound, 3 over urange(2u, 5u), 0 over range(0), 1 over range(1)")
        for (op in [SpvOp.SNegate, SpvOp.Not, SpvOp.ISub, SpvOp.SDiv, SpvOp.SRem, SpvOp.ShiftLeftLogical,
                SpvOp.ShiftRightArithmetic, SpvOp.BitwiseAnd, SpvOp.BitwiseOr, SpvOp.BitwiseXor, SpvOp.IMul, SpvOp.IAdd]) {
            t |> equal(count_op(words, op), 0, "ufold: no {op} survives - every integer op on the induction constant folds at emission")
        }
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_3, "ufold: SPIR-V header version stays 1.3")
        validate(t, words, "ufold")
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
