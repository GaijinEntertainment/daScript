options gen2
options indenting = 4

// Phase-2 control-flow gate: mutable locals (Function OpVariable + Load/Store), compound assignment,
// ++/--, comparisons (signed/unsigned/float), logical &&/||/!, and if/else (OpSelectionMerge +
// OpBranchConditional + Branch). One fixture per scalar class so every comparison flavor is asserted;
// every emitted module passes spirv-val (the structured-CFG / define-before-use oracle).

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
def test_uint_control(t : T?) {
    t |> run("uint control flow: var/compound/++/cmp/logical/if-else") <| @(t : T?) {
        var words <- uctrl_words()
        // mutable local + if/else structure
        check_ops(t, words, "uctrl", [SpvOp.Variable, SpvOp.Load, SpvOp.Store,
            SpvOp.SelectionMerge, SpvOp.BranchConditional, SpvOp.Branch])
        // compound assignment arithmetic + ++ (IAdd)
        check_ops(t, words, "uctrl", [SpvOp.IAdd, SpvOp.ISub, SpvOp.IMul, SpvOp.UDiv])
        // unsigned comparisons + equality + logical
        check_ops(t, words, "uctrl", [SpvOp.UGreaterThan, SpvOp.ULessThan, SpvOp.IEqual,
            SpvOp.INotEqual, SpvOp.LogicalAnd, SpvOp.LogicalNot])
        // a Function-storage OpVariable was declared for the mutable locals. OpVariable operands are
        // [result_type, result_id, storage_class, init?]; check the storage class positionally (index 2)
        // so the small Function value can't false-match the result-type/id operands.
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Function)),
            "uctrl: has a Function-storage OpVariable")
        validate(t, words, "uctrl")
        delete words
    }
}

[test]
def test_int_control(t : T?) {
    t |> run("int control flow: signed comparisons") <| @(t : T?) {
        var words <- ictrl_words()
        check_ops(t, words, "ictrl", [SpvOp.SGreaterThanEqual, SpvOp.SLessThanEqual,
            SpvOp.SGreaterThan, SpvOp.SLessThan, SpvOp.LogicalAnd,
            SpvOp.SelectionMerge, SpvOp.BranchConditional])
        validate(t, words, "ictrl")
        delete words
    }
}

[test]
def test_float_control(t : T?) {
    t |> run("float control flow: ordered comparisons") <| @(t : T?) {
        var words <- fctrl_words()
        check_ops(t, words, "fctrl", [SpvOp.FOrdGreaterThan, SpvOp.FOrdLessThan,
            SpvOp.FOrdNotEqual, SpvOp.FOrdEqual, SpvOp.FOrdGreaterThanEqual,
            SpvOp.FOrdLessThanEqual, SpvOp.LogicalOr])
        validate(t, words, "fctrl")
        delete words
    }
}
