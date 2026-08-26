# dasMetal Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `MASTERPLAN.md`.

**Every change under this folder applies `modules/REVIEW_SHADER_EMITTERS.md` too.**

- **A new emitter capability - an emit site or a newly accepted construct, including the
  derived path a `[metal_kernel]` takes with no `name=` argument - ships a text fixture under
  `tests/msl/` (repo root) and its own census kind - the construct label the emitter records
  at emit time, declared in `declared_msl_census` (`tests/msl/_msl_common.das`, repo root) -
  in the same change.** Two emit shapes sharing one kind lets either vanish unseen.

- **A new construct the MSL emitter rejects at compile time ships a `tests/msl/_fail_closed/`
  (repo root) fixture and asserts its error needle - the substring of the compile error naming
  the rejected construct - in `tests/msl/test_msl_fail_closed.das`, in the same change.**

- **A kernel behavioral change ships a CPU-oracle test under `tests/metal/` (repo root).** A
  dasMetal-only regression must red here.

- **A CPU-oracle test builds a fresh kernel instance per simulated thread.** Kernel state is
  per-thread; sharing one instance across simulated threads is a defect of the test, not of
  the kernel.

- **A new or changed host extern under `modules/dasMetal/src/`, or a changed public function
  in `metal/das_metal_boost.das`, ships a host-side test under `tests/metal/` (repo root)
  that `feint`s when no Metal device is present, in the same change.** A dasMetal-only
  regression must red here, not in a consumer module's suite.

- **Weakening `tests/msl/test_msl_census.das`'s (repo root) two-direction check - every
  declared kind emitted by some fixture, every emitted kind declared - is a defect.** Text
  has no disassembler; the census is the only coverage proxy.

- **Weakening `REVIEW.das`'s descriptor cell - every `matmul2d_descriptor` that
  `metal/msl_emit.das` writes sets `relaxed_precision = true` - is a defect**, including
  narrowing the file it scans or its finding text no longer naming what failed. RP = false
  keeps the op off the tensor-unit fast path.

- **A cooperative tensor that a `matmul2d` `run` accumulates into gets no zero-init element
  walk - `get_destination_cooperative_tensor` hands it back already zeroed.** The walk
  forces every element into real storage before the K loop and costs the op its fast path
  for the whole loop.
