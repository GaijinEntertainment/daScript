# dasMetal Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist. Shared
emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` — apply that list with this one.**
Architecture doc: `MASTERPLAN.md`.

- **A new emitter capability — an emit site or a newly accepted construct, the default
  entry-point path included — ships a text fixture under `tests/msl/` and its own census
  kind, in the same change.** Two forms sharing one kind lets either vanish unseen.

- **A new rejection path ships a `tests/msl/_fail_closed/` fixture and asserts its needle
  in `tests/msl/test_msl_fail_closed.das`, in the same change.**

- **A behavioral change ships a CPU-oracle test under `tests/metal/`.** The oracle builds a
  fresh kernel instance per simulated thread — kernel state is per-thread; sharing one
  instance is a defect of the test, not of the kernel.

- **Every emit site has a census kind: a new site's kind joins `declared_msl_census`, a
  removed site's kind leaves it, everywhere it is named.** `test_msl_census.das` checks both
  directions — every declared kind is emitted by some fixture, every emitted kind is
  declared. Text has no disassembler; the census is the only coverage proxy.
