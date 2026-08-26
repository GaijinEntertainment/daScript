# Shared emitter rules - dasSpirv and dasMetal

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `modules/dasMetal/MASTERPLAN.md` and `modules/dasSpirv/MASTERPLAN.md`.

**These rules bind both emitters; each module's `REVIEW.md` routes here and applies this file
together with its own list.**

- **Kernel bodies are ordinary das: what cannot run on the CPU does not enter the emitter.**
  Every kernel, and every function it calls, runs on the CPU - that CPU run is the test
  oracle.

- **A diff that adds or changes an emitter builtin ships a CPU body whose result matches the
  emitted form's, argument for argument.**

- **Everything outside the lowered subset is a clean compile error naming the construct** -
  never a silently wrong kernel.

- **A shape constant reaches a kernel as a call-site constant, never as a runtime argument** -
  a shape constant is a tile row count, tile column count, cooperating-simdgroup count, staged
  chunk depth, or any other value that fixes the kernel's tiling.

- **A matmul reduction width - the K dimension, the length of the loop the kernel accumulates
  over - that is known only at run time is passed through the emitter's runtime-extent
  descriptor** (`dynamic_extent` on Metal; a `tensorLayout2D` / `tensorLayout2DPad` whose
  dimension `tensorLayoutSetDimension` sets on SPIR-V). K does not fix tiling, so it is not
  a shape constant.

- **A new kernel capability that would need a runtime shape value ships a specialization path -
  one compiled variant per constant shape - or the module's `MASTERPLAN.md` records that it
  cannot.**

- **A claim about emitted shape is checked in the emitted words or text, never the das
  source.**

- **A kernel-model capability lands on both emitters, or the asymmetry is recorded** in
  the shared ledger (`modules/dasMetal/MASTERPLAN.md`).

- **An emitter diff that uses a `daslib/shader_lingua_franca` declaration this emitter does not
  handle ships that emitter's lowering of it, or the fail-closed fixture that rejects it by
  name, in the same change.** A declaration in that module is available to both emitters.
