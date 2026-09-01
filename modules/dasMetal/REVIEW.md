# dasMetal Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**Every change under this folder applies `modules/REVIEW_SHADER_EMITTERS.md` too.**

**A CPU-oracle or host-side test file answers to `tests/metal/REVIEW.md` (repo root), wherever
the diff puts it.** An emitted-text fixture answers to `tests/msl/REVIEW.md` (repo root).

- **A new emitter capability ships a text fixture under `tests/msl/` (repo root) and a census
  kind per emit shape, in the same change.** A new emitter capability is a new emit site or a
  newly accepted construct, including the path a `[metal_kernel]` takes when it has no `name=`
  argument. A census kind is the construct label the emitter records at emit time, declared in
  `declared_msl_census` (`tests/msl/_msl_common.das`, repo root); an emit shape is one distinct
  text the site can produce (a renamed twin is a second shape). Two emit shapes sharing one
  kind lets either one go untested.

- **A new or changed `[metal_kernel]` annotation argument ships a `tests/msl/` (repo root)
  fixture in the same change.** The fixture asserts what the argument changes: the global the
  macro declares for it - a module-level global holding the kernel's MSL text or a compile
  option - or the difference it makes to the emitted text.

- **A diff that adds an MSL emitter rejection, widens an existing one's condition, or changes a
  rejection's message text ships a `tests/msl/_fail_closed/` (repo root) fixture asserting that
  rejection's needle, in the same change.** An error needle is the substring of the compile
  error that names the rejected construct; `REVIEW.das` checks the fixture-and-assert pairing
  both directions (`check_fail_closed_sync`, `tests/msl/test_msl_fail_closed.das` (repo root)).

- **A kernel behavioral change ships a CPU-oracle test under `tests/metal/` (repo root).** A
  kernel behavioral change is a change to what an existing kernel computes, or a diff that
  makes the emitter produce arithmetic, indexing, or synchronization it did not produce before
  - a newly accepted construct that lowers to a read, an index, or a barrier; a CPU-oracle
  test compares the GPU result against a CPU-computed expectation.

- **A change visible only in the emitted text ships a `tests/msl/` (repo root) fixture.** The
  fixture asserts the emitted text that the change alters.

- **Never commit a `.msl` file and never hand-write MSL outside `metal/msl_emit.das`'s emitted
  text - emit it from a `[metal_kernel]` body instead.**

- **A new or changed host extern under `modules/dasMetal/src/` ships a host-side test under
  `tests/metal/` (repo root) in the same change.** A changed public function in
  `metal/das_metal_boost.das` fires this rule too. The test reports a `feint` - dastest's skip
  result - when no Metal device is present. A regression in dasMetal alone must fail here, not in a consumer module's tests.

- **Weakening `REVIEW.das`'s descriptor check is a defect.** That check requires every
  `matmul2d_descriptor` written in `metal/msl_emit.das` to set `relaxed_precision = true`.
  Narrowing the file it scans weakens it. So does making its finding text stop naming what
  failed. `relaxed_precision = false` keeps the op off the tensor-unit fast path.

- **Weakening `REVIEW.das`'s fail-closed sync check is a defect.** That check requires every
  `tests/msl/_fail_closed/` fixture to have a `check_rejects` call in
  `test_msl_fail_closed.das`, and every such call to have a fixture - a fixture without its
  call leaves a rejection unpinned, and a call without its fixture is a rejection the emitter
  no longer makes.

- **Never zero a cooperative tensor element by element before a `matmul2d` `run` accumulates
  into it - `get_destination_cooperative_tensor` already hands it back zeroed.** That walk
  forces every element into real storage before the accumulation loop, and that costs the op
  its fast path for the whole loop.

- **A diff that changes `emit_tmm2d_tg_step_deva`'s `ldb` extent (`metal/msl_emit.das`) also
  changes `ldb` in `tmm2d_tg_step_deva` (`metal/metal_builtins.das`), in the same change.** That
  das body replays the same tile indexing, so a one-sided change reads the wrong halves with no
  compile error.

- **A diff that changes the threadgroup row stride `tmm2d_helper_text` writes (its `dstp`
  index and the matching tensor extent in `metal/msl_emit.das`) changes the `wt` size
  documented on `tmm2d_q8u_f32` (`metal/metal_builtins.das`) in the same change.** `wt` is the
  staged weight tile - the dequantized W the emitter stores in threadgroup memory - and the
  documented size is the caller's only statement of the stride.

- **A diff that changes the threadgroup row stride `tmm2d_helper_text` writes
  (`metal/msl_emit.das`) also resizes every `@workgroup` staging array a `tests/msl/` or
  `tests/metal/` (repo root) fixture sizes for that stride, in the same change.**
  Nothing checks a fixture's allocation against the stride, so a one-sided change overruns
  it silently.
