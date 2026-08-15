# Shared emitter rules — dasSpirv and dasMetal

**These rules bind both emitters; each module's `REVIEW.md` links here and applies this
file together with its own list.** `REVIEW_COMMON.md` (repo root) binds this file too.

- **Kernel bodies are ordinary das.** Every kernel, and every function it calls, runs on
  the CPU — that CPU run is the test oracle. What cannot run on the CPU does not enter
  the emitter.

- **Everything outside the lowered subset is a clean compile error naming the construct** —
  never a silently wrong kernel.

- **A shape constant never enters a kernel as a runtime value.** The capability ships a
  specialization path beside it, or the module's `MASTERPLAN.md` records that it cannot.

- **A claim about emitted shape is checked in the emitted words or text, never the das
  source.**

- **A kernel-model capability lands on both emitters, or the asymmetry is recorded** in
  the shared ledger (`modules/dasMetal/MASTERPLAN.md`).

- **A declaration moved into `daslib/shader_lingua_franca` is granted to every shader
  rail.** Each rail lowers it or rejects it by name — with a fail-closed fixture in that
  rail's test home — in the same change.
