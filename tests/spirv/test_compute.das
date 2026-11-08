options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

// Phase-10.5 gate: the compute tier -- shared memory, barriers, and atomics.
//   cshared:  a @workgroup shared array (Workgroup OpVariable), barrier() -> OpControlBarrier,
//             memoryBarrierShared() -> OpMemoryBarrier, atomicAdd onto an SSBO -> OpAtomicIAdd.
//   catomics: the rest of the atomic family on shared int/uint counters -> OpAtomicSMin/UMin/SMax/
//             UMax/And/Or/Xor/Exchange/CompareExchange (both signednesses present).
// Opcode presence is asserted directly; the barrier scope/semantics operands are constant <id>s
// (not literals), so their well-formedness is left to spirv-val -- the structural oracle for the
// barrier-scope and atomic-pointer-storage-class rules.
def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "A compute kernel using @workgroup memory, barrier, memoryBarrierShared and atomicAdd emits the shared-memory, barrier and atomic opcodes and passes spirv-val.")]
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

[test(description = "memoryBarrierBuffer followed by barrier in a compute kernel emits the buffer-scope memory barrier for the cross-invocation SSBO handoff.")]
def test_buffer_barrier(t : T?) {
    t |> run("compute: memoryBarrierBuffer() + barrier() (the cross-invocation SSBO handoff)") <| @(t : T?) {
        var words <- cbufbar_words()
        t |> success(has_op(words, SpvOp.ControlBarrier), "cbufbar: barrier() -> OpControlBarrier")
        // memoryBarrierBuffer() -> OpMemoryBarrier (Device scope, UniformMemory|AcquireRelease —
        // the scope/semantics operands are constant <id>s, left to spirv-val per the header note)
        t |> success(has_op(words, SpvOp.MemoryBarrier), "cbufbar: memoryBarrierBuffer() -> OpMemoryBarrier")
        validate(t, words, "cbufbar")
        delete words
    }
}

[test(description = "The atomic family on shared int and uint counters lowers to the matching Op Atomic opcodes and validates.")]
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

// End-to-end Phase-1 gate: the real `[compute_shader] square` lowered by the AST emitter,
// captured into `square_spv`. Asserts the exact opcode shape, then validates the blob with
// spirv-val (the structured-CFG / define-before-use oracle opcode-presence can't catch).
[test(description = "The compute square kernel emits the expected load, multiply and store opcode shape.")]
def test_square_opcodes(t : T?) {
    t |> run("compute square: opcode shape") <| @(t : T?) {
        var words <- square_words()
        t |> success(length(words) > 5, "has words")
        t |> equal(words[0], SPV_MAGIC)
        t |> equal(count_op(words, SpvOp.EntryPoint), 1)
        t |> equal(count_op(words, SpvOp.ExecutionMode), 1)
        t |> equal(count_op(words, SpvOp.Variable), 2)        // gid (Input) + data (StorageBuffer)
        t |> equal(count_op(words, SpvOp.AccessChain), 2)     // gid.x + data[i]
        t |> equal(count_op(words, SpvOp.Load), 1)            // load gid.x
        t |> equal(count_op(words, SpvOp.IMul), 1)            // i * i
        t |> equal(count_op(words, SpvOp.Store), 1)           // data[i] = ...
        t |> success(has_op(words, SpvOp.Return), "has Return")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.GlobalInvocationId)), "BuiltIn = GlobalInvocationId decoration")
        t |> success(op_has_operand_pair(words, SpvOp.ExecutionMode, uint(SpvExecutionMode.LocalSize), 64u), "ExecutionMode LocalSize x = 64")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u), "Binding = 0 decoration")
        delete words
    }
}

[test(description = "The compute square kernel's module is accepted by spirv-val.")]
def test_square_validates(t : T?) {
    t |> run("compute square: spirv-val clean") <| @(t : T?) {
        var words <- square_words()
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// gl_WorkGroupSize gate: the one workgroup builtin SPIR-V spells as a CONSTANT rather than a
// dispatch Input. The emitter folds it to a uvec3 OpConstantComposite of the shader's own
// local_size and decorates THAT with BuiltIn WorkgroupSize -- so it takes no OpVariable and no
// entry-point interface entry, matching glslang. What it buys: a module shared by several shaders,
// each compiled with a different local_size, can read the size of whichever one is compiling it.
// The id OpDecorate ... BuiltIn WorkgroupSize targets (0 = no such decoration).
def private workgroup_size_id(words : array<uint>) : uint {
    var target = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.Decorate) && n >= 3 && w[s + 1] == uint(SpvDecoration.BuiltIn) &&
            w[s + 2] == uint(SpvBuiltIn.WorkgroupSize)) {
            target = w[s]
        }
    }
    return target
}

// Is `id` the result of an instruction with `op` whose result-id sits at operand `idx`?
def private is_result_of(words : array<uint>; op : SpvOp; idx : int; id : uint) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(op) && idx < n && w[s + idx] == id) {
            found = true
        }
    }
    return found
}

// Is `id` one of the entry point's interface ids? OpEntryPoint operands: model(0), entry-id(1),
// the name (a NUL-padded literal string -- "main" is exactly 2 words), then the interface ids.
def private entry_interface_has(words : array<uint>; id : uint) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.EntryPoint)) {
            for (k in range(4, n)) {
                if (w[s + k] == id) {
                    found = true
                }
            }
        }
    }
    return found
}

[test(description = "gl_WorkGroupSize lowers to a WorkgroupSize-decorated OpConstantComposite.")]
def test_workgroup_size_is_a_decorated_constant(t : T?) {
    t |> run("compute: gl_WorkGroupSize -> BuiltIn WorkgroupSize on an OpConstantComposite") <| @(t : T?) {
        var words <- wgsize_words()
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.GLCompute),
            "wgsize: EntryPoint model is GLCompute")
        // ExecutionMode operands: entry-point(0), mode(1), x(2), y(3), z(4).
        t |> success(op_has_operand_pair(words, SpvOp.ExecutionMode,
            uint(SpvExecutionMode.LocalSize), 64u), "wgsize: LocalSize is the declared 64")
        let wgs = workgroup_size_id(words)
        t |> success(wgs != 0u, "wgsize: a BuiltIn WorkgroupSize decoration is emitted")
        // OpConstantComposite operands: result-type(0), result-id(1), constituents(2..).
        t |> success(is_result_of(words, SpvOp.ConstantComposite, 1, wgs),
            "wgsize: the decorated id is an OpConstantComposite")
        // OpVariable operands: result-type(0), result-id(1), storage-class(2). A constant is not a
        // variable, so it must appear in NO OpVariable and in no entry-point interface entry.
        t |> success(!is_result_of(words, SpvOp.Variable, 1, wgs),
            "wgsize: the decorated id is not an OpVariable")
        t |> success(!entry_interface_has(words, wgs),
            "wgsize: the constant is not listed in the entry-point interface")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "wgsize: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
