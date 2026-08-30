# src/misc Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `plans/jobque-thread-ladder.md` (repo root).

**A core-tier query answers "no tier" on a topology it cannot read - never "yes".** A platform
arm that cannot name the second tier's kind returns 0 from `JobQue::get_num_perf_cores` and
false from `JobQue::is_slow_tier_compute`, so a pool never extends onto cores nobody vouched
for; a diff that makes either answer optimistically is a defect.
