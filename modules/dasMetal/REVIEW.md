# dasMetal Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`MASTERPLAN.md`.

**Every change under this folder applies `modules/REVIEW_SHADER_EMITTERS.md` too.**

**A CPU-oracle or host-side test file answers to `tests/metal/REVIEW.md` (repo root), wherever
the diff puts it.** An emitted-text fixture answers to `tests/msl/REVIEW.md` (repo root).

- **A new emitter capability ships a text fixture under `tests/msl/` (repo root) and its own
  census kind, in the same change.** A new emitter capability is a new emit site or a newly
  accepted construct, and it includes the derived path a `[metal_kernel]` takes with no
  `name=` argument. A census kind is the construct label the emitter records at emit time,
  declared in `declared_msl_census` (`tests/msl/_msl_common.das`, repo root). Two emit shapes
  sharing one kind lets either vanish unseen.

- **A new or changed `[metal_kernel]` annotation argument ships a `tests/msl/` (repo root)
  fixture in the same change.** The fixture asserts what the argument changes: the published
  global it names, or the difference it makes to the emitted text.

- **A new construct the MSL emitter rejects at compile time ships a `tests/msl/_fail_closed/`
  (repo root) fixture in the same change.** The same change asserts that construct's error
  needle in `tests/msl/test_msl_fail_closed.das`. An error needle is the substring of the
  compile error that names the rejected construct.

- **A kernel behavioral change ships a CPU-oracle test under `tests/metal/` (repo root).** A
  dasMetal-only regression must red in this module's own suites.

- **A change visible only in the emitted text ships a `tests/msl/` (repo root) fixture.** The
  fixture asserts the emitted text that the change alters.

- **A new or changed host extern under `modules/dasMetal/src/` ships a host-side test under
  `tests/metal/` (repo root) in the same change.** A changed public function in
  `metal/das_metal_boost.das` fires this rule too. The test `feint`s when no Metal device is
  present. A dasMetal-only regression must red here, not in a consumer module's suite.

- **Weakening `REVIEW.das`'s descriptor cell is a defect.** That cell checks every
  `matmul2d_descriptor` that `metal/msl_emit.das` writes, and each one sets
  `relaxed_precision = true`. Narrowing the file the cell scans weakens it. So does making the
  cell's finding text stop naming what failed. `relaxed_precision = false` keeps the op off
  the tensor-unit fast path.

- **A cooperative tensor that a `matmul2d` `run` accumulates into gets no zero-init element
  walk.** `get_destination_cooperative_tensor` hands the tensor back already zeroed. The walk
  forces every element into real storage before the K loop, and that costs the op its fast
  path for the whole loop.
