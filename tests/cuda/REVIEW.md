# dasCUDA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasCUDA/ARCHITECTURE.md`. A diff that touches a `[cuda_kernel]` method under this folder
applies `modules/REVIEW_SHADER_EMITTERS.md` too.

**A diff that removes or weakens an assert in `test_cuda_census.das` is a defect.** That test is
the only coverage measure: emitted text cannot be read back into constructs, so the emitter records
a kind - a construct label - for each fixture (a `[cuda_kernel]` method in `_cuda_common.das`) it
writes, and the test checks those kinds against `declared_cuda_census` in both directions.

**A diff that adds a fixture - a `[cuda_kernel]` method in `_cuda_common.das` - adds its
`<name>_cuda_census` global to `all_cuda_censuses` and its `<name>_cuda` global (the CUDA C the
emitter wrote) to `all_cuda_texts`, in the same change.** A fixture left out of the first list emits
kinds the census check never sees; one left out of the second is never compiled through NVRTC
(`test_cuda_emit.das`).

**Only `test_cuda_plumbing.das` hand-writes a kernel text - a CUDA C source string a test passes to
NVRTC; every other test emits its text from a `[cuda_kernel]` body.** An emitted text is the one
shipped code runs; a hand-written one tests a kernel nothing ships.

**A hand-written kernel text never includes a toolkit header - declare what it needs inline in the
text.** NVRTC resolves no header without the include dir of the toolkit.

**A CPU replay - the thread-by-thread run of a kernel method on a class instance - never reuses
across thread ids a member the kernel writes: every written member is an array the thread
indexes, or the replay builds a fresh instance per thread.** A written scalar member carries one
thread's value into the next.

**A test that checks the device output of a kernel against its CPU replay uploads each buffer as
it stood before the replay ran, never a member the replay wrote - copy the inputs before
replaying.** The replay mutates the instance in place; the device must read what the instance
read.

**A `[test]` that calls `with_cuda_context` asserts `cuda_log_leaks_if_any()` is zero after its
last `with_cuda_context` call.** Driver objects and device memory are invisible to every das leak
detector; the live-object counter in `das_cuda_boost` is the only witness.

**A `[test]` that calls `with_cuda_context` calls `feint` (the dastest skip) when `cuda_available()`
is false or the context handed to its block is null, and - for each kernel text it compiles - when
`cuda_nvrtc_available()` is false or the device's compute capability is below what that text
needs.** Every CI lane runs the file, and a lane whose GPU cannot run the test must not fail.

**A diff that removes or weakens `test_cuda_no_include` (`test_cuda_emit.das`), the check that no
emitted CUDA text carries an `#include`, is a defect.**
