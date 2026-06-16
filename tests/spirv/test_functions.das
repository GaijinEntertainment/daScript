options gen2
options indenting = 4

// Phase-11 user-defined shader functions. Each function called (transitively) from a shader is emitted
// as its own OpFunction; a value parameter binds an SSA OpFunctionParameter, a by-reference parameter
// binds a Function pointer with caller-side copy-in/copy-out temps. These tests assert the opcode shape
// (function/param/call/return counts), the single-emission of a SHARED helper (one OpFunction even when
// called from several sites), and that every blob is spirv-val-clean — the real oracle for the
// structured-CFG + copy-in/out correctness opcode-presence can't catch.

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
def test_userfn_value(t : T?) {
    t |> run("user functions: value params, transitive + shared helper, vector in/out") <| @(t : T?) {
        var words <- userfn_words()
        // entry + uf_add1 + uf_muladd + uf_double + uf_grade = 5 OpFunctions. uf_add1 is called from the
        // entry AND from inside uf_double, but is emitted exactly ONCE -> the count proves single-emission.
        t |> equal(count_op(words, SpvOp.Function), 5)
        // direct call sites: entry calls add1/muladd/double/grade (4) + uf_double calls add1 twice (2) = 6.
        t |> equal(count_op(words, SpvOp.FunctionCall), 6)
        // parameters: add1(1) + muladd(3) + double(1) + grade(1) = 6 OpFunctionParameter.
        t |> equal(count_op(words, SpvOp.FunctionParameter), 6)
        // every helper returns a value -> 4 OpReturnValue; the void entry keeps OpReturn.
        t |> equal(count_op(words, SpvOp.ReturnValue), 4)
        t |> success(has_op(words, SpvOp.Return), "userfn: void entry still emits OpReturn")
        validate(t, words, "userfn")
        delete words
    }
}

[test]
def test_userfn_ref(t : T?) {
    t |> run("user functions: by-reference params (copy-in / copy-out temps)") <| @(t : T?) {
        var words <- userfn_ref_words()
        // entry + uf_accumulate + uf_scale_inplace = 3 OpFunctions.
        t |> equal(count_op(words, SpvOp.Function), 3)
        // uf_accumulate called twice + uf_scale_inplace once = 3 OpFunctionCall.
        t |> equal(count_op(words, SpvOp.FunctionCall), 3)
        // accumulate(acc&, x) = 2 + scale_inplace(v&, s) = 2 -> 4 OpFunctionParameter.
        t |> equal(count_op(words, SpvOp.FunctionParameter), 4)
        // both helpers are void -> no OpReturnValue (they keep OpReturn).
        t |> equal(count_op(words, SpvOp.ReturnValue), 0)
        // copy-in (Store into temp) + copy-out (Load temp, Store back) leave Loads and Stores present.
        t |> success(has_op(words, SpvOp.Store), "userfn_ref: copy-in/out emit OpStore")
        t |> success(has_op(words, SpvOp.Load), "userfn_ref: copy-out emits OpLoad")
        validate(t, words, "userfn_ref")
        delete words
    }
}

[test]
def test_userfn_global(t : T?) {
    t |> run("user functions: a helper that reads a module global") <| @(t : T?) {
        var words <- userfn_global_words()
        // entry + uf_lane_seed = 2 OpFunctions.
        t |> equal(count_op(words, SpvOp.Function), 2)
        t |> equal(count_op(words, SpvOp.FunctionCall), 1)
        // uf_lane_seed takes no parameters.
        t |> equal(count_op(words, SpvOp.FunctionParameter), 0)
        t |> equal(count_op(words, SpvOp.ReturnValue), 1)
        validate(t, words, "userfn_global")
        delete words
    }
}
