# dasLLAMA Measurement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `PERF_LEDGER.md`.

Figure rules here bind any text this change adds - in the repo or in its PR body - outside the
pages a `site*/` or `utils/` folder checklist owns (repo root); those pages answer to their own
checklist. The ledger rules key on `PERF_LEDGER.md`. The naming a figure rule asks for sits in
the figure's own sentence, in a table heading that covers the table's rows, in a section-level
provenance line that covers the paragraphs under it, or in a citation of the passage whose
provenance line covers it.

**A `PERF_LEDGER.md` entry never states a tok/s figure or a turn wall that the `-jit` script
produced - a `-jit` A/B pair enters as its ratio, with the arms' absolute rates left in the
run's report.** A served turn is one whole request the engine serves - a prefill-plus-decode
run, or one synthesis; a turn wall is its wall.

**A `PERF_LEDGER.md` entry states a served-turn figure only when the released `lcpp_bench` exe
(`benchmarks/lcpp_bench.das` built by `daspkg release`) or a board cell produced it.**

**Only a reference cell of `performance/gen_bench_records.das` - a cell that times, over a board
workload, a binary this repository does not build - writes that binary's wall into
`PERF_LEDGER.md`.** A wall taken any other way stays in the report where it was taken.

**A difference, ratio, or percentage of two measured walls, neither produced by the `-jit`
script, written into `PERF_LEDGER.md` carries both raw walls in the entry.**

**A `-jit` ratio written into `PERF_LEDGER.md` names the arm it is measured against.**

**A diff that adds a `PERF_LEDGER.md` entry whose reading no board cell produced names the
instrument that produced it - the script or exe whose output is that wall or rate.** A board
cell is a run `performance/gen_bench_records.das` spawns, or a manual
`benchmarks/lcpp_bench.das` cell its `PROFILE.md` section documents; its reading lands as a row
of `performance/records/<box>.json`. A ruler record (`performance/records/mtp/*.json`, written
by `harness/mtp_ruler.das`) is not a board cell.

**A ruler record's third-party row is written by the same `harness/mtp_ruler.das` run that
wrote the das row it pairs with.** A wall pasted in from another run measures a different
prompt, drafter and settle state, and the acceptance rate it implies is not the pair's.

**A `PERF_LEDGER.md` entry tags its reading `direction-grade` when the reading compares across
two processes or two commits, and `out-of-process` when the wall was measured from outside the
benchmark process.**

**A checked-in document that states a `--for-debug-purposes` reading for a model and arm that a
committed board row covers cites that row and marks the reading debug-jit.** A
`--for-debug-purposes` row is the `-jit` script's own output.

**A `PERF_LEDGER.md` entry carrying a figure this repository did not measure names the source
and the report it came from and tags it `external`.**

**A diff never rests an adoption decision about what the engine serves on a figure from
another project - that decision rests on a self-measured board cell.**

**A diff that adds an entry to `PERF_LEDGER.md` never records a selection timing - a timing
that picks a winner between candidate kernel forms.** That timing settles its adoption
decision in the report of the run that took it and in the PR that lands the kernel. The
winner enters the ledger only through a re-measured board cell.

**A diff that makes the engine run end to end - serving a client, or measuring - by a route no
board cell exercises mints that cell in the same change.** A route is anything that changes
which code runs a whole prefill-plus-decode pass end to end, including the path a run with no
flags and no environment overrides takes.

**A change that owes a board cell for a route no leg of `performance/gen_bench_records.das`
mints a row for names instead, in the same change, the record or gate output that proves the
route ran end to end.**

**A diff that makes the fat exe - a shipped exe carrying its tune profile
(`ARCHITECTURE_MEASUREMENT.md` sec.2.42a) - run end to end names `tune_gate`'s fat-world report
(`performance/profile_common.das`) and the `sanity:` lines of its `lcpp_bench` run.**

**A diff that claims to make an already-served path faster, from an author whose box mints
that path, re-mints a board row (`performance/records/<box>.json`) that exercises that path, in
the same change, and names that row in the PR body.** A box mints a path when
`performance/gen_bench_records.das` mints a row for it on that box - a leg its `stored_row_leg`
(`performance/profile_common.das`) admits - rather than refusing or skipping it. Where no row
exercises the path, the diff mints one. The board is the module's committed record of what
serving costs; a kernel win that never lands there is invisible to the next regression check.

**A timing figure of a served turn as a whole - tok/s, latency, a whole-turn model or engine
comparison, the 512-token prefill (pp512) and 128-token decode (tg128) rates, a synthesis's
real-time factor (RTF) - written down as a measurement rather than as a prediction - is a
defect without either a board cell behind it or a provenance line naming harness, flags,
environment overrides, box, and the exe or script that ran it.** Flags are the tier (`-jit` or
not), the `DAS_TUNE_POLICY` value in force, and the kernel backend the run served on. The board
cell states its quant mode and stamps box and engine provenance, so a number can never silently
describe a format nobody serves or a kernel set nobody ships.

**A figure that covers less than one whole served turn and whose value depends on the box it
ran on - timing or not - names the harness, the flags and the box that produced it.** A figure
a committed board cell or ruler record produced names the record and row instead.

**A figure whose value is the same on every box names the build, fixture, or command that
reproduces it.**

**A diff that adds a race - a timed run-off between candidate implementations at startup whose
winner sets a knob - to a shipped exe's startup races on synthetic inputs only, never loading a
model, never spawning a child process.** A shipped exe carries no vehicle model - a real model
file a harness run drives - and no harness script, so a model or a child there is a hang or a
silent skip.

**A shipped exe's startup race never races a `[tune]` kernel family; a GPU pso twin race, which
only sets a runtime knob, is what a first start may do** (`ARCHITECTURE_MEASUREMENT.md`
sec.2.42a). A `[tune]` winner needs a recompiled clone the shipped exe does not carry.

**A diff never adds a confirm - an end-to-end A/B served on a vehicle model in a spawned
child - outside `harness/`.**

**A diff that pins the kernel backend - `pin_kernel_backend`, `select_kernel_backend`, or
`DASLLAMA_PIN_BACKEND` - pins it before the load it governs.** The loader repacks weights into
the pinned backend's layout, so a pin set after the load never reaches them.

**A pin naming a backend the box has not registered refuses or skips the run rather than
falling through to the default.** A misspelled pin that falls through measures the default
backend under the pinned one's name.
