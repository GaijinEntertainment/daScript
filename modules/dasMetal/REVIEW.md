# dasMetal Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `MASTERPLAN.md`.

**Every change under this folder applies `modules/REVIEW_SHADER_EMITTERS.md` too.**

- **A new emitter capability - an emit site or a newly accepted construct, the default
  entry-point path included - ships a text fixture under `tests/msl/` (repo root) and its own
  census kind, in the same change.** Two forms sharing one kind lets either vanish unseen.

- **A new construct the MSL emitter rejects at compile time ships a `tests/msl/_fail_closed/`
  (repo root) fixture and asserts its needle in `tests/msl/test_msl_fail_closed.das`, in the
  same change.**

- **A kernel behavioral change ships a CPU-oracle test under `tests/metal/` (repo root).** The
  oracle builds a fresh kernel instance per simulated thread - kernel state is per-thread;
  sharing one instance is a defect of the test, not of the kernel.

- **A new or changed host extern under `src/`, or a changed public function in
  `metal/das_metal_boost.das`, ships a host-side test under `tests/metal/` (repo root) that
  `feint`s when no Metal device is present, in the same change.** A dasMetal-only regression
  must red here, not in a consumer module's suite.

- **Weakening `test_msl_census.das`'s two-direction check - every declared kind emitted by
  some fixture, every emitted kind declared - is a defect.** Text has no disassembler; the
  census is the only coverage proxy.
