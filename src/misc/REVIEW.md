# src/misc Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A diff that makes `JobQue::is_slow_tier_compute` treat a perflevel name outside the list it
carries ("Performance", "Super") as compute is a defect.**

**A diff that makes `JobQue::get_num_perf_cores` return nonzero on a platform that reports fewer
than two CPU performance levels is a defect.**

**A fixed-length copy in `luau_float2string.cpp` stays fixed-length.** `DAS_F2S_MEMCPY` and
`DAS_F2S_MEMSET` write the constant `sizefast` bytes, not the requested `size`; a diff that
switches either to `size` is a defect - the constant length is what the compiler inlines.

**A diff that raises a `sizefast` constant in `luau_float2string.cpp` also raises
`DAS_F2S_BUFFER_SIZE` (`include/daScript/misc/float2string.h`), which sizes every caller's
buffer.**

**Never classify a float in `luau_float2string.cpp` through libc - `isfinite`, `isnan`,
`isinf`, `fpclassify`, `signbit` - read the IEEE bits instead: the exponent bits for NaN and
infinity, the sign bit for the sign.** A build with `-ffinite-math-only` folds a NaN or infinity
test to a constant.

**A diff in `daScriptC.cpp` that hands a writing `SerializationStorageVector`'s `buffer` out to
a caller calls `flush()` on the storage first.** The writer grows the vector by doubling and
counts the written bytes in `writePos`, so before a flush the vector is longer than the stream
and the caller gets trailing garbage.

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing the files or
lines a check scans, or rewriting a finding text so it no longer names what failed.**

**A diff that makes `dep_recorder.cpp` write more than one rule per depfile is a defect - a
process merges its paths into the rule already in the depfile, under the lock file beside it.**
Several rules naming one target is ninja's deprecated `depfilemulti`, and a parallel sweep whose
workers each wrote their own would produce one per worker.

**A diff that has `dep_recorder.cpp` record anything but a source the compile read is a
defect** - a file read at runtime, a directory listing, a build artifact. Ninja builds whatever
a depfile names, so a named build artifact makes ninja build it before the compile, and an
artifact the build rewrites makes ninja re-run the compile every time.

**A diff that makes `das_dep_flush` (`dep_recorder.cpp`) name a path without first checking that
the file exists is a defect**: a path under a temp directory the run removed would be named, and
ninja treats a missing dependency as dirty forever.
