# src/misc Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `plans/jobque-thread-ladder.md` (repo root).

**A second-tier kind the arm does not recognize is not compute.** `JobQue::is_slow_tier_compute`
returns true only for a perflevel name on the list it carries ("Performance", "Super"); a diff
that treats an unmatched name as compute is a defect.

**A core-count query on a topology it cannot read answers 0.** `JobQue::get_num_perf_cores`
returns 0 when the platform reports no tier structure; a diff that makes it guess is a defect.

**A fixed-length copy in `luau_float2string.cpp` stays fixed-length.** `DAS_F2S_MEMCPY` and
`DAS_F2S_MEMSET` write the constant `sizefast` bytes, not the requested `size`; a diff that
switches either to `size` is a defect - the constant length is what the compiler inlines.

**A diff that raises a `sizefast` constant in `luau_float2string.cpp` also raises
`DAS_F2S_BUFFER_SIZE` (`include/daScript/misc/float2string.h`), which sizes every caller's
buffer.**

**Never call `isfinite`, `isnan`, or `signbit` in `luau_float2string.cpp` - classify special
values from the IEEE bits instead.** A build with `-ffinite-math-only` folds those calls to
constants.

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing the files or
lines a check scans, or rewriting a finding text so it no longer names what failed.**
