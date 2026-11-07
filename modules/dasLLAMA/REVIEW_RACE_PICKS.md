# dasLLAMA Race Picks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_MEASUREMENT.md`, `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`. Planned work:
`followup_general.md`.

A pick is a diff's change to which candidate ships, new code's first choice included; a diff that
times a candidate and keeps the shipped code picks nothing.

**A diff that cites a timing (in a `PERF_LEDGER.md` row, a `followup_*.md` row, a commit message or
the PR body) as the reason it picks one candidate over another - a constant's value, or another way
to compute the same quantity: a kernel body, a predicate, a dispatch order, how kernels are launched
(graph replay or direct launch, one stream or several, the shared-memory carveout, where data moves
between devices) - takes the winner from a race: one script, in one process, timing every candidate
once per round.** Timings from two commits or processes pick nothing.

**A diff that picks a candidate by a race leaves every candidate it raced selectable by a flag or
argument - in the code the diff ships, or in a race script in the tree at the diff's tip when the
shipped code cannot select it.**

**A diff whose pick rests on a race, wherever the race ran, or that adds a race script to the tree
or changes which candidates one times, puts the race's output lines, one per candidate and each naming it, in the PR body or in a
`PERF_LEDGER.md` row the diff adds.**

**A diff whose race picks a numeric threshold - a cutoff a predicate compares against, or any
integer compiled into a kernel variant as a constant - between candidates that differ in that
number alone times the chosen value and, on each side of it where the device admits a value, at
least one such value, in that race.**
