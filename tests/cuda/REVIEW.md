# dasCUDA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasCUDA/ARCHITECTURE.md`. Shared emitter rules: `modules/REVIEW_SHADER_EMITTERS.md` -
apply that list with this one. A fixture is a `[cuda_kernel]` method in `_cuda_common.das`; its
text is its `<name>_cuda` global, the CUDA C the emitter wrote, and its census is its
`<name>_cuda_census` global, the kinds - construct labels the emitter records at each emit site -
that text holds. The compile gate is the `test_cuda_emit.das` cell that compiles the text of every
fixture through NVRTC.

**Weakening the census check is a defect: a diff that removes either assert from
`test_cuda_census.das` - every declared kind is emitted by some fixture, every emitted kind is
declared - or removes from `declared_cuda_census` (`_cuda_common.das`) a kind the emitter still
records.** Emitted text cannot be read back into constructs, so the census is the only measure of
coverage.

**A diff that adds a fixture adds its census to `all_cuda_censuses` and its text to
`all_cuda_texts` (`_cuda_common.das`) in the same change.** A fixture left out of the first list
emits kinds the census check never sees; one left out of the second never reaches the compile
gate.

**Only a test of the driver, NVRTC or the cubin cache hand-writes a kernel text - a CUDA C source
string a test passes to NVRTC; every other test emits its text from a `[cuda_kernel]` body.** An
emitted text is the one the homes run; a hand-written one tests a kernel no home ships.

**A hand-written kernel text never includes a toolkit header - declare what it needs inline in the
text.** NVRTC resolves no header without the include dir of the toolkit.

**A CPU replay - the thread-by-thread run of a kernel method on a class instance - never reuses
across thread ids a member the kernel writes: every written member is an array the thread
indexes, or the replay builds a fresh instance per thread.** A written scalar member carries one
thread's value into the next; a read-only `@kargs` member is the same for every thread.

**A test that checks the device output of a kernel against its CPU replay uploads each buffer as
it stood before the replay ran, never a member the replay wrote - copy the inputs before
replaying.** The replay mutates the instance in place; the device must read what the instance
read.

**A `[test]` that calls `with_cuda_context` asserts `cuda_log_leaks_if_any()` is zero after its
last `with_cuda_context` call.** Driver objects and device memory are invisible to every das leak
detector; the live-object counter in `das_cuda_boost` is the only witness.

**A `[test]` that calls `with_cuda_context` calls `feint` - the dastest skip, never a failure -
when `cuda_available()` or `cuda_nvrtc_available()` is false or the context handed to it is
null.** The file compiles on every lane; only a box with an NVIDIA driver and NVRTC runs its GPU
arms.
