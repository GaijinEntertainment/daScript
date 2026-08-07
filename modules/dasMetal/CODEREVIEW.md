# dasMetal Code Review Checklist

**This file reviews itself: a rule a reviewer cannot apply as written is a defect of this
file.** Mark it like any other finding; the fix is a rewrite or a move, not silent tolerance.

1. **Any new bit of functionality ships with test coverage for EVERY new scenario it
   introduces.** The emitter has two test homes and a change usually needs both:
   `tests/msl/` — text-level fixtures (`_msl_common.das` kernels), per-construct census
   kinds, the census gate, and a `_fail_closed/` fixture for every new rejection path —
   and `tests/metal/` — behavioral, the CPU-oracle-vs-GPU pattern (the kernel method runs
   on the CPU as ordinary das; the GPU result must match it). A new emitter capability
   with no new fixture, no census kind, or no fail-closed twin for its error paths is a
   review defect.
2. **The census gate (gate B) is two-directional.** Every new emit site records a census
   kind; the kind joins `declared_msl_census` AND some fixture must actually emit it.
   Text has no disassembler — the census is the emitter's only coverage proxy.
3. **Kernel bodies are ordinary das.** Every kernel — and every function it calls — must
   run on the CPU with identical semantics; that CPU run IS the test oracle. A construct
   that cannot run on the CPU has no place in the emitter.
4. **Fail closed, specifically.** Anything outside the lowered subset must produce a
   clean compile error naming the construct — never a silently wrong kernel. New error
   paths get their needle asserted in `tests/msl/test_msl_fail_closed.das`.
5. **An emitter feature must not make dynamic dispatch easy.** The consumer rule is
   dasLLAMA CODEREVIEW #21 — kernel SHAPE is compile-time, only DATA is runtime — and the
   emitter is where that is won or lost. A capability that carries a shape constant into
   the kernel as a value (a size parameter, a selector field, anything the shader compiler
   would have to inline-and-fold to recover) needs a specialization path beside it:
   per-type overloads, monomorphized generics, `static_if` on a compile-time witness. Where
   the emitter cannot specialize, say so in `MASTERPLAN.md` rather than shipping the value
   form as the answer. Assert the LITERAL in an emitted-text fixture, not the das source.
