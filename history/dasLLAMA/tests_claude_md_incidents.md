# Incidents behind two modules/dasLLAMA/tests/CLAUDE.md rules

Archived 2026-08-21 under the rules-carry-rules-not-history clause; the rules stand in
`modules/dasLLAMA/tests/CLAUDE.md` without them.

## Why "run suites ONLY through the runner" is enforcement, not advice

One session spent 5.75 of its 6 hours re-running full Metal suites to verify one-arm fixes.
The Metal suites are wall-time-expensive — model loads dominate, and a full pass holds 40 GB
GGUFs — so ad-hoc full-suite invocations turn a one-arm fix into an afternoon. The runner's
`--arm` / `--suite` narrowing exists so that never happens again.

## Why "grep afterwards, never at capture time"

A capture-time filter (a `grep` in the run pipeline) once hid the exact proof line a
verification run existed to produce — the run looked silent-clean, and the missing evidence
read as success. Complete logs, filtered after the fact, cannot lose the line you came for.
