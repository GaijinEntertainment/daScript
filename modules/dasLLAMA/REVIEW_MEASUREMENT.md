# dasLLAMA Measurement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `PERF_LEDGER.md`.

The two figure rules below bind the surfaces this module owns - its ledgers, docs, code
comments, and the PR body of a diff under it. A number on a served page answers to the site
checklists (`site/REVIEW.md`, `site-dasllama/REVIEW.md`), which carry their own provenance
duty.

**A self-measured served-turn time entering `PERF_LEDGER.md` comes from the released
`lcpp_bench` exe, never from the `-jit` script.** A served-turn time is a tok/s figure or a
turn wall. The released exe is `benchmarks/lcpp_bench.das` built by `daspkg release`. It is
spawned by `performance/gen_bench_records.das`, or run by hand where the cell's `PROFILE.md`
section says so. A `--for-debug-purposes` row is a debug instrument. A tutorial's printed
wall-clock is teaching output, feeding no board.

**A subtraction of two measured walls written into `PERF_LEDGER.md` carries both raw walls
in the entry.**

**A diff that adds an entry to `PERF_LEDGER.md` names the instrument that produced the entry's
reading.** The rule fires only for a reading no board cell produced. The entry also tags that
reading `direction-grade` when the reading compares across two processes or two commits. The
entry tags the reading `out-of-process` when the wall was measured from outside the benchmark
process. A reading that is neither carries no tag. A figure taken from another project names
its source and the report it came from and is tagged `external` - it grounds a lead worth
chasing, never an adoption decision.

**A diff that adds an entry to `PERF_LEDGER.md` never records the selection timing of one of
our own `harness/` or race A/B labs.** That timing settles its adoption decision in the lab's
own report and in the PR that lands the kernel. The ledger learns the winner only through a
re-measured cell.

**A new servable capability gets its cell in the same change**: a board row spawned by
`performance/gen_bench_records.das`, or a manual `benchmarks/lcpp_bench.das` cell with its own
`PROFILE.md` section. A servable capability is a path that serves a weight format, modality,
family, or backend no existing cell exercises - a q8 or f32 serving lane and a GPU tower
included. A kernel or form that only makes a path an existing cell already serves faster is
not a new capability - it re-mints that cell's row on at least one box instead.

**A timing figure PRESENTED AS A MEASUREMENT of a served turn as a whole - tok/s, latency, a
whole-turn model or engine comparison - is a defect wherever this module writes it down with
no cell behind it: a checked-in doc, a ledger, a code comment, or a PR description.** The
cell states its quant mode and stamps box and engine provenance, so a number can never
silently describe a format nobody serves or a kernel set nobody ships. A figure labeled as a
prediction is not a reading - the prediction log is mandated and needs no cell.

**A figure measuring one engine stage inside a served turn, or any other measured margin -
a lab margin, a kernel-form delta, a gate knee - names the harness and flags that
produced it.** A stage figure is a stage wall, a stage share, a stage speedup, or a
cross-engine comparison of one stage. The rule holds wherever this module writes the figure
down: a checked-in doc, a ledger, a code comment, or a PR description. A figure a committed
board cell produced - any metric of the row: `pp`/`tg`, an `img:enc` wall, an `asr:` wall -
is not a stage figure; naming the record and row is its provenance. The naming sits in the
figure's own sentence, in a table heading that covers the table's rows, or in a
section-level provenance line that covers the paragraphs under it.
