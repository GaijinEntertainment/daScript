options gen2
options indenting = 4

// The `precise` shader property: every float arithmetic result the module emits - the called function's
// included, the vector product too - carries NoContraction, one decoration per float op and each on a float
// op's result id; the plain twin of the same body carries none; the class-kernel form carries the property.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

//! the result ids of the float arithmetic ops, and the NoContraction targets, over one module
def private float_ops_and_marks(words : array<uint>) : tuple<ops : table<uint>; marks : array<uint>> {
    var ops : table<uint>
    var marks : array<uint>
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.FAdd) || opcode == uint(SpvOp.FSub) || opcode == uint(SpvOp.FMul) || opcode == uint(SpvOp.FDiv) ||
                opcode == uint(SpvOp.FRem) || opcode == uint(SpvOp.FNegate) || opcode == uint(SpvOp.VectorTimesScalar)) {
            ops |> insert(w[s + 1])   // result type, RESULT ID, operands
        } elif (opcode == uint(SpvOp.Decorate) && n >= 2 && w[s + 1] == uint(SpvDecoration.NoContraction)) {
            marks |> push(w[s])       // target, decoration
        }
    }
    return <- (ops = ops, marks = marks)
}

def private validate(t : T?; words : array<uint>; lbl : string) {
    t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_3, "{lbl}: SPIR-V header version stays 1.3")
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

def private check_marked(t : T?; words : array<uint>; lbl : string; least : int) {
    var fm <- float_ops_and_marks(words)
    t |> success(length(fm.ops) >= least, "{lbl}: {length(fm.ops)} float ops emitted (at least {least})")
    t |> equal(length(fm.marks), length(fm.ops))
    var on_ops = 0
    for (id in fm.marks) {
        on_ops += key_exists(fm.ops, id) ? 1 : 0
    }
    t |> equal(on_ops, length(fm.marks))
    t |> success(has_op(words, SpvOp.FunctionCall), "{lbl}: the two-float step stays a called function")
    validate(t, words, lbl)
    delete fm
}

[test]
def test_precise_marks_every_float_op(t : T?) {
    t |> run("precise: NoContraction on every float op, the callee's and the vector product included") <| @(t : T?) {
        var words <- precise_words()
        check_marked(t, words, "precise", 8)
        t |> success(has_op(words, SpvOp.VectorTimesScalar), "precise: the float2 * scalar product is emitted")
        delete words
    }
}

[test]
def test_precise_class_kernel(t : T?) {
    t |> run("precise: the [spirv_kernel] form carries the property") <| @(t : T?) {
        var words <- precise_kern_words()
        check_marked(t, words, "precise_kern", 5)
        delete words
    }
}

[test]
def test_plain_twin_unmarked(t : T?) {
    t |> run("the plain twin: the same float ops, no NoContraction") <| @(t : T?) {
        var words <- plain_sum_words()
        var fm <- float_ops_and_marks(words)
        t |> success(length(fm.ops) >= 8, "plain: {length(fm.ops)} float ops emitted")
        t |> equal(length(fm.marks), 0)
        delete fm
        validate(t, words, "plain")
        delete words
    }
}
