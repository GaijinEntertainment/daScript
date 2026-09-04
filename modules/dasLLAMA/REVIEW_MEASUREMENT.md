# dasLLAMA Measurement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `PERF_LEDGER.md`.

Figure rules here bind the surfaces this module owns - its ledgers, its code comments, the PR
body of a diff under it, and a checked-in doc under this module. A number on a served page
answers to `site/REVIEW.md` or `site-dasllama/REVIEW.md` (repo root) instead.

**A `PERF_LEDGER.md` entry never states a tok/s figure or a turn wall that the `-jit` script
produced - a `-jit` A/B pair enters as its ratio, tagged `direction-grade`, with the arms'
absolute rates left in the run's report.** A turn wall is the wall of one whole
prefill-plus-decode run. The served-turn figures the ledger does state come from the released
`lcpp_bench` exe (`benchmarks/lcpp_bench.das` built by `daspkg release`) or a board cell.

**A figure derived from two measured walls - a difference, a ratio, a percentage - written into
`PERF_LEDGER.md` carries both raw walls in the entry, or, for a `-jit` pair, the ratio alone
with the arm it is against named.**

**A diff that adds a `PERF_LEDGER.md` entry whose reading no board cell produced names the
instrument that produced it - the script or exe whose output is that wall or rate.** A board
cell is a row of `performance/records/<box>.json`, minted by `performance/gen_bench_records.das`
or by a manual `benchmarks/lcpp_bench.das` cell its `PROFILE.md` section documents. A ruler
record (`performance/records/mtp/*.json`, written by `harness/mtp_ruler.das`) is a third
artifact: a figure it produced names the record and row like a board cell's, and it discharges
no board duty.

**A `PERF_LEDGER.md` entry tags its reading `direction-grade` when the reading compares across
two processes or two commits, and `out-of-process` when the wall was measured from outside the
benchmark process.**

**A `PERF_LEDGER.md` entry carrying a figure from another project names the source and the
report it came from and tags it `external`.**

**A diff never rests an adoption decision on a figure from another project - the decision
rests on a self-measured board cell.**

**A diff that adds an entry to `PERF_LEDGER.md` never records a selection timing - a timing
that picks a winner between candidate kernel forms.** That timing settles its adoption
decision in the report of the run that took it and in the PR that lands the kernel. The
winner enters the ledger only through a re-measured board cell.

**A diff that makes the engine serve tokens by a route no board cell exercises mints that cell
in the same change.** A route is what a request takes end to end: a weight format, a modality,
a family, a backend, a serving lane (q8 or f32), a GPU tower (a GPU-run vision or audio
encoder), a sampler class (greedy, sampled) through the speculative round, or the path a run
with no flags and no environment overrides takes. A change that only speeds up a route an
existing cell already runs mints nothing here - the re-mint rule below owns it.

**A diff that claims to make an already-served path faster, from an author whose box mints
that path, re-mints a board row (`performance/records/<box>.json`) that exercises that path, in
the same change, and names that row in the PR body.** A box mints a path when
`performance/gen_bench_records.das` mints a row for it on that box rather than refusing or
skipping it. Where no row exercises the path, the diff mints one. The board is the module's
public memory of what serving costs; a kernel win that never lands there is invisible to the
next regression check.

**A diff that claims to make an already-served path faster, from an author whose box does not
mint that path, names the owed row in `PERF_LEDGER.md` and in the PR body.**

**A timing figure PRESENTED AS A MEASUREMENT of a served turn as a whole - tok/s, latency, a
whole-turn model or engine comparison - is a defect wherever this module writes it down with
neither a board cell behind it nor a provenance line, covering its passage or its section,
naming harness, flags, box, and the exe or script that ran it.** The board cell states its
quant mode and stamps box and engine provenance, so a number can never silently describe a
format nobody serves or a kernel set nobody ships. A figure labeled as a prediction is not a
reading, and this rule does not reach it.

**A figure that states how much one thing costs against another - a stage wall or share, a
kernel-form delta, an acceptance rate, a tokens-per-round, a gate knee (the input size at which
a gate flips) - names the harness and flags that produced it.** A figure a committed board cell
or ruler record produced names the record and row instead of the harness and flags. The naming
sits in the figure's own sentence, in a table heading that covers the table's rows, in a
section-level provenance line that covers the paragraphs under it, or in a citation of the
`PERF_LEDGER.md` entry whose provenance line covers it.
