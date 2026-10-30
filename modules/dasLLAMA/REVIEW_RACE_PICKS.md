# dasLLAMA Race Picks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_MEASUREMENT.md`, `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`. Planned work:
`followup_general.md`.

**A diff that cites a timing (in a `PERF_LEDGER.md` row, a `followup_*.md` row, a commit message or
the PR body) as the reason it picks one candidate over another - a constant's value, a body
replacing another that computes the same quantity, a predicate or a dispatch order choosing among
kernels that compute the same result - takes the winner from one script that races every candidate
in one process, interleaved, each chosen by a flag or argument in the code the diff ships, or
carried as an arm of the race script where the diff deletes it** - two commits or two processes
differ by everything else that changed, so their timings pick nothing.

**A diff that adds a race - a run that times every candidate in one process, interleaved: kernel
variants, function bodies, or values of one constant, cutoff or compiled size - or changes which
candidates a race times, puts the race's output lines, one per candidate and each naming it, in
the PR body or in a `PERF_LEDGER.md` row the diff adds.**

**A diff whose race picks a numeric threshold - a cutoff a predicate compares against, or a size a
kernel variant is compiled for: a row count, a byte size, a lane width - between arms that differ
in that number alone times the chosen value and, on each side of it where the device admits a
value, at least one such value, in that race.**
