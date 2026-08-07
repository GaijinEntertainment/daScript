# dasSpirv Code Review Checklist

**This file reviews itself: a rule a reviewer cannot apply as written is a defect of this
file.** Mark it like any other finding — a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

1. **Any new bit of functionality ships with test coverage for EVERY new scenario it
   introduces.** The emitter's test home is `tests/spirv/`: word-level fixtures (`_golden/`
   disassembly, equivalence gates), the opcode census, and a `_fail_closed/` fixture for
   every new rejection path. Behavioral coverage — the kernel runs on a device and matches
   its CPU oracle — lives downstream (dasVulkan `tests/integration`, the dasLLAMA vulkan
   kernel suite), and a new capability that serves them lands with an arm there. A new
   emitter capability with no new fixture, no census presence, or no fail-closed twin for
   its error paths is a review defect.
2. **The opcode census (gate B) is two-directional.** The union of opcodes the emitter
   produces across every fixture must equal the declared supported set — a new opcode joins
   the declared set AND some fixture must actually emit it. Raw words have no
   reviewer-readable form — the census is the emitter's coverage proxy.
3. **Kernel bodies are ordinary das.** Every kernel — and every function it calls — must run
   on the CPU with identical semantics; that CPU run IS the test oracle
   (`daslib/shader_lingua_franca` carries the exact CPU bodies of the GPU builtins). A
   construct that cannot run on the CPU has no place in the emitter.
4. **Fail closed, specifically.** Anything outside the lowered subset must produce a clean
   compile error naming the construct — never a silently wrong blob. A new error path gets a
   `_fail_closed/` fixture and its needle asserted in `tests/spirv/test_fail_closed.das`.
5. **An emitter feature must not make dynamic dispatch easy.** The consumer rule is dasLLAMA
   CODEREVIEW's "A kernel's shape is compile-time; only its data is runtime" — and the
   emitter is where that is won or lost. A capability that carries a shape constant into the
   kernel as a value (a size parameter, a selector field, anything the driver compiler would
   have to inline-and-fold to recover) needs a specialization path beside it: per-type
   overloads, monomorphized generics, `static_if` on a compile-time witness, devirtualized
   class overrides. Where the emitter cannot specialize, say so in `MASTERPLAN.md` rather
   than shipping the value form as the answer. Assert the LITERAL in the emitted
   words/disassembly, never the das source.
6. **The two emitters mirror: a kernel-model capability lands on both backends, or its
   asymmetry is recorded.** dasSpirv and the MSL emitter (`modules/dasMetal`) lower the same
   kernel model — classes with resource members, ordinary methods, free-function callees,
   inheritance with devirtualized overrides. A capability added to the kernel model here
   lands on the Metal side too (and the reverse), unless it is genuinely target-specific
   (cooperative matrix vs Metal-4 tensors, subgroup vs simdgroup semantics); a deliberate
   asymmetry is recorded in `MASTERPLAN.md`. A diff that grows the kernel model on one side
   with no sibling change and no ledger entry is a defect.
