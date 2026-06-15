options gen2
options indenting = 4

// Phase-10.5 gate: the compute tier -- shared memory, barriers, and atomics.
//   cshared:  a @workgroup shared array (Workgroup OpVariable), barrier() -> OpControlBarrier,
//             memoryBarrierShared() -> OpMemoryBarrier, atomicAdd onto an SSBO -> OpAtomicIAdd.
//   catomics: the rest of the atomic family on shared int/uint counters -> OpAtomicSMin/UMin/SMax/
//             UMax/And/Or/Xor/Exchange/CompareExchange (both signednesses present).
// Opcode presence is asserted directly; the barrier scope/semantics operands are constant <id>s
// (not literals), so their well-formedness is left to spirv-val -- the structural oracle for the
// barrier-scope and atomic-pointer-storage-class rules.

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
def test_shared_and_barriers(t : T?) {
    t |> run("compute: @workgroup shared memory + barrier() + memoryBarrierShared() + atomicAdd") <| @(t : T?) {
        var words <- cshared_words()
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.GLCompute),
            "cshared: EntryPoint model is GLCompute")
        // @workgroup global -> a Workgroup-storage OpVariable. OpVariable operands:
        // result-type(0), result-id(1), storage-class(2).
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Workgroup)),
            "cshared: shared global is a Workgroup OpVariable")
        t |> success(has_op(words, SpvOp.ControlBarrier), "cshared: barrier() -> OpControlBarrier")
        t |> success(has_op(words, SpvOp.MemoryBarrier), "cshared: memoryBarrierShared() -> OpMemoryBarrier")
        t |> success(has_op(words, SpvOp.AtomicIAdd), "cshared: atomicAdd -> OpAtomicIAdd")
        validate(t, words, "cshared")
        delete words
    }
}

[test]
def test_atomic_family(t : T?) {
    t |> run("compute: atomic family on shared int/uint counters") <| @(t : T?) {
        var words <- catomics_words()
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.GLCompute),
            "catomics: EntryPoint model is GLCompute")
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Workgroup)),
            "catomics: shared counters are Workgroup OpVariables")
        // atomicMin/Max emit the signed (S) opcode for int and the unsigned (U) opcode for uint.
        t |> success(has_op(words, SpvOp.AtomicSMin), "catomics: atomicMin(int) -> OpAtomicSMin")
        t |> success(has_op(words, SpvOp.AtomicUMin), "catomics: atomicMin(uint) -> OpAtomicUMin")
        t |> success(has_op(words, SpvOp.AtomicSMax), "catomics: atomicMax(int) -> OpAtomicSMax")
        t |> success(has_op(words, SpvOp.AtomicUMax), "catomics: atomicMax(uint) -> OpAtomicUMax")
        t |> success(has_op(words, SpvOp.AtomicAnd), "catomics: atomicAnd -> OpAtomicAnd")
        t |> success(has_op(words, SpvOp.AtomicOr), "catomics: atomicOr -> OpAtomicOr")
        t |> success(has_op(words, SpvOp.AtomicXor), "catomics: atomicXor -> OpAtomicXor")
        t |> success(has_op(words, SpvOp.AtomicExchange), "catomics: atomicExchange -> OpAtomicExchange")
        t |> success(has_op(words, SpvOp.AtomicCompareExchange), "catomics: atomicCompSwap -> OpAtomicCompareExchange")
        validate(t, words, "catomics")
        delete words
    }
}
