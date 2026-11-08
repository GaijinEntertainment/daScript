options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

// Phase-2 control-flow gate: mutable locals (Function OpVariable + Load/Store), compound assignment,
// ++/--, comparisons (signed/unsigned/float), logical &&/||/!, and if/else (OpSelectionMerge +
// OpBranchConditional + Branch). One fixture per scalar class so every comparison flavor is asserted;
// every emitted module passes spirv-val (the structured-CFG / define-before-use oracle).
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

[test(description = "uint control flow (var, compound assignment, ++, compare, logical, if-else) lowers and validates.")]
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

[test(description = "Signed int comparisons in control flow lower to the signed compare opcodes.")]
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

[test(description = "Float comparisons in control flow lower to the ordered compare opcodes.")]
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

[test(description = "cond ? a : b lowers to OpSelect for scalars and vectors.")]
def test_ternary_select(t : T?) {
    t |> run("ternary: cond ? a : b -> OpSelect (scalar + vector)") <| @(t : T?) {
        var words <- tern_words()
        // both the scalar and the vector ternary lower to OpSelect (>= 2 in the module)
        t |> success(count_op(words, SpvOp.Select) >= 2, "tern: scalar + vector ternary -> 2x OpSelect")
        // the vector select needs a bvecN built from the scalar bool condition (the splat)
        t |> success(has_op(words, SpvOp.CompositeConstruct), "tern: vector select splats bool -> OpCompositeConstruct")
        // OpTypeBool is present (scalar condition) and a bool vector type for the splat condition
        t |> success(has_op(words, SpvOp.TypeBool), "tern: bool condition type emitted")
        t |> success(has_op(words, SpvOp.TypeVector), "tern: bvecN splat type emitted")
        validate(t, words, "tern")
        delete words
    }
}

[test(description = "A ternary, && or || with an SSBO load in an operand becomes a branch while pure operands stay OpSelect.")]
def test_ternary_lazy_operand(t : T?) {
    t |> run("ternary / && / || with an ssbo load in an operand -> a branch, pure ones stay OpSelect") <| @(t : T?) {
        var words <- tern_lazy_words()
        t |> equal(count_op(words, SpvOp.SelectionMerge), 3, "lazy: three branched operands -> three OpSelectionMerge")
        // the fixture's three pure ternaries: the abs-shaped pick and the two bool-to-float picks
        t |> equal(count_op(words, SpvOp.Select), 3, "lazy: the pure ternaries alone stay OpSelect")
        t |> success(has_op(words, SpvOp.ConstantFalse) && has_op(words, SpvOp.ConstantTrue), "lazy: && stores false, || stores true before the branch")
        validate(t, words, "tern_lazy")
        delete words
    }
}
