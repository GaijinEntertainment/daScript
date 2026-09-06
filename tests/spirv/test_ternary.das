options gen2
options indenting = 4

// Phase-8.1 ternary cond ? a : b -> OpSelect (branchless). A scalar select uses the scalar bool
// condition directly; a vector select with a scalar condition splats the bool to a bvecN
// (OpCompositeConstruct of bool) before OpSelect, per the SPIR-V < 1.4 width rule. The blob is spirv-val'd.

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

[test]
def test_ternary_lazy_operand(t : T?) {
    t |> run("ternary / && / || with an ssbo load in an operand -> a branch, pure ones stay OpSelect") <| @(t : T?) {
        var words <- tern_lazy_words()
        // the guarded ternary and the two short-circuits each open a selection; the three pure
        // ternaries (abs-shaped, the two bool-to-float picks) are the module's only OpSelects
        t |> equal(count_op(words, SpvOp.SelectionMerge), 3, "lazy: three branched operands -> three OpSelectionMerge")
        t |> equal(count_op(words, SpvOp.Select), 3, "lazy: the pure ternaries alone stay OpSelect")
        // the short-circuit value lands first as a bool constant, then the merge reloads the temp
        t |> success(has_op(words, SpvOp.ConstantFalse) && has_op(words, SpvOp.ConstantTrue), "lazy: && stores false, || stores true before the branch")
        validate(t, words, "tern_lazy")
        delete words
    }
}
