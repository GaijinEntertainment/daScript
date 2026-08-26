# Shared emitter rules - dasSpirv and dasMetal

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `modules/dasMetal/MASTERPLAN.md` and `modules/dasSpirv/MASTERPLAN.md`.

**These rules bind both emitters.** Each module's `REVIEW.md` routes here, and a change under
that module applies this file together with the module's own list.

- **Kernel bodies are ordinary das: what cannot run on the CPU does not enter the emitter.**
  Every kernel runs on the CPU, and so does every function it calls. That CPU run is the test
  oracle.

- **A diff that adds or changes an emitter builtin ships a CPU body for it.** The CPU body
  returns what the emitted form returns, argument for argument.

- **Everything outside the lowered subset is a clean compile error naming the construct** -
  never a silently wrong kernel.

- **A shape constant reaches a kernel as a call-site constant, never as a runtime argument.**
  A shape constant is any value that fixes the kernel's tiling: a tile row count, a tile column
  count, a cooperating-simdgroup count, a staged chunk depth.

- **A matmul reduction width known only at run time is passed through the emitter's
  runtime-extent descriptor.** The reduction width is the K dimension - the length of the loop
  the kernel accumulates over. On Metal that descriptor is `dynamic_extent`. On SPIR-V it is a
  `tensorLayout2D` or a `tensorLayout2DPad` whose dimension `tensorLayoutSetDimension` sets. K
  does not fix tiling, so it is not a shape constant.

- **A new kernel capability that would need a runtime shape value ships a specialization
  path.** A specialization path is one compiled variant per constant shape. When the capability
  cannot have one, the module's `MASTERPLAN.md` records that.

- **A claim about emitted shape is checked in the emitted words or text, never the das
  source.**

- **A kernel-model capability lands on both emitters, or the asymmetry is recorded** in
  the shared ledger (`modules/dasMetal/MASTERPLAN.md`).

- **An emitter diff that uses a `daslib/shader_lingua_franca` declaration this emitter does not
  handle ships that emitter's lowering of the declaration, in the same change.** The diff may
  instead ship the fail-closed fixture that rejects the declaration by name. A declaration in
  that module is available to both emitters.
