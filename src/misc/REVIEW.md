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

**A diff that raises a `sizefast` constant in `luau_float2string.cpp` also raises the caller
buffer minimum stated in `include/daScript/misc/float2string.h`, in the same change.** Those
copies overshoot past the digits they write, and that stated minimum is what keeps the
overshoot inside the caller's buffer.
