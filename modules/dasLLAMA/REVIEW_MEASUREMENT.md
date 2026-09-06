# dasLLAMA Measurement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `PERF_LEDGER.md`.

Figure rules here bind the surfaces this module owns - its code comments, the PR body of a diff
under it, and a checked-in doc under this module. A number on a served page answers to
`site/REVIEW.md` or `site-dasllama/REVIEW.md` (repo root) instead.

**A `PERF_LEDGER.md` entry never states a tok/s figure or a turn wall that the `-jit` script
produced - a `-jit` A/B pair enters as its ratio, with the arms' absolute rates left in the
run's report.** A served turn is one whole prefill-plus-decode run; a turn wall is its wall.
The served-turn figures the ledger does state come from the released `lcpp_bench` exe
(`benchmarks/lcpp_bench.das` built by `daspkg release`) or a board cell.

**A difference, ratio, or percentage of two measured walls, neither produced by the `-jit`
script, written into `PERF_LEDGER.md` carries both raw walls in the entry.**

**A `-jit` ratio written into `PERF_LEDGER.md` names the arm it is measured against.**

**A diff that adds a `PERF_LEDGER.md` entry whose reading no board cell produced names the
instrument that produced it - the script or exe whose output is that wall or rate.** A board
cell is a run `performance/gen_bench_records.das` spawns, or a manual `benchmarks/lcpp_bench.das`
cell its `PROFILE.md` section documents; its reading lands as a row of
`performance/records/<box>.json`. A ruler record (`performance/records/mtp/*.json`, written by
`harness/mtp_ruler.das`) is not a board cell: a diff cannot mint or re-mint one in place of a
board row.

**A `PERF_LEDGER.md` entry tags its reading `direction-grade` when the reading compares across
two processes or two commits, and `out-of-process` when the wall was measured from outside the
benchmark process.**

**A `--for-debug-purposes` reading never overturns a committed board row - the board answers
to a re-minted board cell.** A `--for-debug-purposes` row is the `-jit` script's own output.

**A `PERF_LEDGER.md` entry carrying a figure from another project names the source and the
report it came from and tags it `external`.**

**A diff never rests an adoption decision on a figure from another project - the decision
rests on a self-measured board cell.** A board cell is one `performance/gen_bench_records.das`
spawns, or a manual cell of this folder's `benchmarks/lcpp_bench.das` that `PROFILE.md`
documents in a section.

**A diff that adds an entry to `PERF_LEDGER.md` never records a selection timing - a timing
that picks a winner between candidate kernel forms.** That timing settles its adoption
decision in the report of the run that took it and in the PR that lands the kernel. The
winner enters the ledger only through a re-measured board cell.

**A diff that makes the engine serve tokens by a route no board cell exercises mints that cell
in the same change; a route the board cannot carry names, in the same change, the artifact that
stands in for the cell - the record or gate output that proves it served.** A route is anything
that changes which code serves the tokens end to end - the format, modality, family, backend,
serving lane, GPU tower, sampler class, compile tier, cross target, or the path a run with no
flags and no environment overrides takes. The fat exe's stand-in is `tune_gate`'s fat-world
report (`performance/profile_common.das`) plus the `sanity:` lines of its `lcpp_bench` run.

**A diff that claims to make an already-served path faster, from an author whose box mints
that path, re-mints a board row (`performance/records/<box>.json`) that exercises that path, in
the same change, and names that row in the PR body.** A box mints a path when
`performance/gen_bench_records.das` mints a row for it on that box rather than refusing or
skipping it. Where no row exercises the path, the diff mints one. The board is the module's
public memory of what serving costs; a kernel win that never lands there is invisible to the
next regression check.

**A timing figure of a served turn as a whole - tok/s, latency, a whole-turn model or engine
comparison, the 512-token prefill (pp512) and 128-token decode (tg128) rates - that this module
writes down as a measurement is a defect without either a board cell behind it or a provenance
line, covering its passage or its section, naming harness, flags, box, and the exe or script
that ran it.** The board cell states its quant mode and stamps box and engine provenance, so a
number can never silently describe a format nobody serves or a kernel set nobody ships. A
figure labeled as a prediction is not a reading, and this rule does not reach it.

**A measured figure that is not a whole served-turn reading - a stage wall or share, a
kernel-form delta, an acceptance rate, a tokens-per-round count, a gate knee (the input size at
which a gate flips) - names the harness and flags that produced it.** A figure a committed
board cell or ruler record produced names the record and row instead of the harness and flags.
The naming sits in the figure's own sentence, in a table heading that covers the table's rows, in a
section-level provenance line that covers the paragraphs under it, or in a citation of the
`PERF_LEDGER.md` entry whose provenance line covers it.

**A diff that adds a race to a shipped exe's startup races on synthetic inputs only - never
loading a model, never spawning a child process - and never races a `[tune]` kernel family, a
race whose winner needs a recompiled clone; a GPU pso twin race, which only sets a runtime
knob, is what a first start may do** (`ARCHITECTURE_MEASUREMENT.md` sec.2.42a). A shipped exe
carries no vehicle model and no harness script, so a model or a child there is a hang or a
silent skip, and a `[tune]` winner there would name a clone the baked exe does not carry.

**A diff never adds a confirm - an end-to-end A/B served on a vehicle model in a spawned
child - outside `harness/`.**

**A diff that pins the kernel backend - `pin_kernel_backend`, `select_kernel_backend`, or
`DASLLAMA_PIN_BACKEND` - pins it before the load it governs, and on a name the box has not
registered refuses or skips rather than running on the default.** The loader repacks weights
into the pinned backend's layout, so a pin set after the load never reaches them, and a
misspelled pin that falls through measures the default backend under the pinned one's name.
