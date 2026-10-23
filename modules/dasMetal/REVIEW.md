# dasMetal Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**Every change under this folder applies `modules/REVIEW_SHADER_EMITTERS.md` too.**

**A CPU-oracle or host-side test file answers to `tests/metal/REVIEW.md` (repo root), wherever
the diff puts it.**

**An emitted-text or fail-closed fixture answers to `tests/msl/REVIEW.md` (repo root), wherever
the diff puts it.**

- **A diff that changes the MSL text any `[metal_kernel]` body can produce, with or without a
  `name=` argument, ships a `tests/msl/` (repo root) fixture asserting the new text, in the same
  change.**

- **A diff that adds an emit shape to `metal/msl_emit.das` gives it a census kind no other emit
  shape records.** An emit shape is one distinct text one emitter code path can produce - the
  same text under another name is a second shape; a census kind is the construct label the
  emitter records at emit time, declared in `declared_msl_census` (`tests/msl/_msl_common.das`,
  repo root). Two shapes sharing one kind lets either one go untested.

- **A new or changed `[metal_kernel]` annotation argument that declares a module-level global -
  the kernel MSL text or a compile option - ships a `tests/msl/` (repo root) fixture asserting
  that global, in the same change.**

- **A diff that adds an MSL emitter rejection a `[metal_kernel]` body can reach, widens an
  existing one's condition, or changes a rejection's message text ships a
  `tests/msl/_fail_closed/` (repo root) fixture asserting that rejection's needle, in the same
  change.** An error needle is the substring of the compile error that names the rejected
  construct.

- **A kernel behavioral change ships a CPU-oracle test under `tests/metal/` (repo root).** A
  kernel behavioral change is a change to what an existing kernel computes, or a diff that
  makes the emitter produce arithmetic, indexing, or synchronization it did not produce before;
  a CPU-oracle test compares the GPU result against a CPU-computed expectation.

- **Never commit a `.msl` file and never hand-write MSL outside `metal/msl_emit.das`'s emitted
  text - emit it from a `[metal_kernel]` body instead.**

- **A new or changed host extern - a function `src/dasMetal.mm` binds into daslang - or a new or
  changed public function in `metal/das_metal_boost.das` ships a host-side test under
  `tests/metal/` (repo root) in the same change.**

- **Weakening `REVIEW.das`'s descriptor check is a defect.** That check requires every
  `matmul2d_descriptor` written in `metal/msl_emit.das` to set `relaxed_precision = true`.
  Scanning less than all of `metal/msl_emit.das` weakens it. So does making its finding text
  stop naming what failed. `relaxed_precision = false` keeps the op off the tensor-unit fast
  path.

- **Weakening `REVIEW.das`'s fail-closed sync check is a defect.** That check requires every
  `tests/msl/_fail_closed/` fixture to have a `check_rejects` call in
  `tests/msl/test_msl_fail_closed.das` (repo root), and every such call to have a fixture.

- **Weakening the `REVIEW.das` shim pool check (`ARCHITECTURE.md#shim-autorelease-pools`) is a
  defect.** That check requires every Objective-C property read in `src/dasMetal.mm` to sit
  inside an `@autoreleasepool` block. Scanning less than all of `src/dasMetal.mm` weakens it, and
  so does adding to its `SCALAR_GETTERS` list a getter that returns an object or a pointer into
  the storage of an object.

- **A diff that makes `metal/msl_emit.das` emit a write to an element of a tensor
  `get_destination_cooperative_tensor` returned, before the first `run` into that tensor, is a
  defect - drop the write; the call already returns the tensor zeroed.**

- **A diff that changes `emit_tmm2d_tg_step_deva`'s `ldb` row stride (`metal/msl_emit.das`) also
  changes `ldb` in `tmm2d_tg_step_deva` (`metal/metal_builtins.das`), in the same change.** The
  CPU body of `tmm2d_tg_step_deva` repeats the emitted tile indexing, so a one-sided change makes
  the CPU run read the wrong f16 elements with no compile error.

- **A diff that changes the threadgroup row stride `tmm2d_helper_text` writes (its `dstp`
  index and the row stride in the W tensor's `metal::array<int32_t, 2>{1, N}` argument,
  `metal/msl_emit.das`) changes the `wt` size documented on `tmm2d_q8u_f32`
  (`metal/metal_builtins.das`) in the same change.** `wt` is the staged weight tile - the
  dequantized W the emitter stores in threadgroup memory - and the documented size is the
  caller's only statement of the stride.

- **A diff that changes the threadgroup row stride `tmm2d_helper_text` writes (its `dstp`
  index and the row stride in the W tensor's `metal::array<int32_t, 2>{1, N}` argument,
  `metal/msl_emit.das`) also resizes every `@workgroup` staging array a `tests/msl/` or
  `tests/metal/` (repo root) fixture sizes for that stride, in the same change.** Nothing checks
  a fixture's allocation against the stride, so a one-sided change overruns it silently.

- **A diff that makes `metal/msl_emit.das` pick a builtin's lowering from anything other than the
  das function name and, for a pointer operand, its pointee type is a defect.** Nothing else
  tells the builtins apart, and the generic parameters of the builtins let the compiler accept
  any operand, so only the emitter rejects a wrong one.
