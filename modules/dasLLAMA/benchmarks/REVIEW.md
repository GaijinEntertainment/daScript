# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_MEASUREMENT.md`, `../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`. Planned
work: `../followup_metal.md` for Metal, `../followup_vulkan.md` for Vulkan,
`../followup_general.md` otherwise.

An instrument is a file whose result is a time or rate it clocked itself (own clock or GPU
timestamps), printed or returned for printing; a serving path's profiler report beside its served
output is not one, and a `[test]` cell is not an instrument: its result is pass or fail. An
instrument's timed body is the statements between its clock reads, and the functions its own file
defines that they call.

A lever is a flag, an environment switch, a runtime setter or a profile key that picks what the run
executes - never an input's size or content, and never a choice among the forms of one `[tune]`
family. A mode is one selectable run of the file, chosen by
its own flag or argument; a file with none is one mode. A switch that only reorders a mode's arms
is not a lever and does not select another mode. A diff that makes an instrument refuse one value
of an input changes that input.

A form is one compiled variant of a kernel. A GPU kernel emitter is code that generates a GPU
kernel or its dispatch builder - a class or function that carries a `*_dispatch` or `*_kernel`
annotation, or a global holding kernel source text. A race times two implementations of one
computation in one process, each one either a form the engine serves or a candidate an instrument
defines - a function, or a kernel a GPU kernel emitter generates (two values of one lever are not
two implementations; the forms of one `[tune]` family are, even where a profile key picks among
them); an A/B is timed runs in one process differing only in one lever's value, arm
order aside. An arm is every timed run of one implementation or one lever value; a round is one
timed run of one arm, and a warmup round is a round the instrument's header comment says the
report row drops; a compared arm is one whose output, from any run of it, the run reads back and
measures against another arm's output or a CPU reference over the compare region, the output
elements the instrument's header comment names; the baseline arm is the one the header comment
names as baseline. An instrument's header comment is the comment at the top of its file; in a
file that holds more than one race or A/B, the header comment of each is the file's top comment
together with the doc comment of every function between `main` and its rounds; a fact stated in
either counts. A run of an arm refuses when it produces no figure or output, or runs on a backend
other than the backend name the report row records.

A board cell is a run whose reading lands as a records row of `../performance/records/<box>.json`
or a figure in `../PERF_LEDGER.md`.

A report row is one line an instrument prints carrying an arm's figure; it covers the arm's warmup
rounds, the rounds it is computed over and the run whose output the compare reads - and a line an
instrument prints for one round covers that round alone.

A round's codec is the number format its weights, activations or KV cache run in; its session
shape is the batch size, context length and prompt length it runs at.

**A diff that changes a board cell's input corpus or the pinned reference build
(`DEFAULT_REF_SHA` in `setup_lcpp_ref.das`, or anything else deciding which reference binary or
environment the run measures) applies `../REVIEW_MEASUREMENT.md` too.**

**Code that dispatches a GPU kernel to measure it rather than to serve a call, wherever the diff
puts it, applies `../REVIEW_GPU_RACE.md` too.**

**A diff that adds or changes the timed body of an instrument that requires
`../performance/profile_common.das` and dispatches a `[tune]` family's kernel or a `[tuned]`
kernel through the form the tune sidecar (the `.tune.json` beside the file or the box's) or
`DAS_TUNE_MANIFEST` picks - a direct call of one `_variants()` registry row is not that - calls
`tune_gate()` before its first round.** Without the gate the instrument measures fallback
kernels silently.

**A diff that adds or changes the timed body of an instrument that dispatches a `[tune]`
family's kernel or a `[tuned]` kernel through the form the tune sidecar or `DAS_TUNE_MANIFEST`
picks - a direct call of one `_variants()` registry row is not that - and does not require
`../performance/profile_common.das` prints, on every report row, the tune manifest
`DAS_TUNE_MANIFEST` names, the class profile (`../performance/defaults/<class>.tune-defaults.json`)
or the `.tune.json` sidecar path the run compiled against.**

**A diff that adds a compared arm to an instrument, or changes a compared arm's run or report row,
makes the instrument's header comment name the baseline arm and the compare region.**

**A diff that adds or changes a race times its arms in turn - one round of each arm, then the
next round of each - never all of one arm's rounds before the next arm's.**

**A diff that adds or changes a race reports each arm's best round, warmup rounds dropped, as
that arm's report row.**

**A diff that adds or changes a race reports each arm's report row on its own - never two arms'
numbers on one report row.**

**A diff that adds a compared arm other than the baseline, or changes its run or report row,
prints one compare on that report row: `bit-exact vs <baseline>` when the arm's result over the
compare region is bit-identical to the baseline's, otherwise a bounded-difference compare against
the baseline arm or the CPU reference with the bound it passed.** How the arm orders its sums, and
whether its multiply-adds fuse, decide bit-identity - not the declared precision.

**A diff that adds a compared arm, or changes its run or report row, makes its report row print no
time when the arm misses its bound - only the miss and the bound.**

**A diff that adds a compared arm to a race or an A/B, or changes a compared arm's run or report
row, checks that race's or A/B's baseline arm against a CPU reference in the same process, over
the compare region, and prints that compare on the baseline's report row, bit-exact or bounded with
the bound it passed.** Two arms can agree and both be wrong; only the reference makes the winner right.

**A diff that adds a race arm that is not a compared arm, or changes its run or report row,
makes that arm carry the literal token `timing-only` on its report row.**

**A diff that adds a mode that times two or more arms and has no compared arm - its arms in one
process, or each in a process the mode spawns - or changes such a mode's arms, makes that mode
carry the literal text `ATTRIBUTION SWEEP` on its own header-comment line, naming the mode and
what its arms attribute.** Without the line a reader takes the mode's arms for an adoption
decision it never made.

**A new instrument that puts its own clock around a turn this repository's engine serves - one
whole request, a prefill-plus-decode run, or a transcription or synthesis end to end - for a
modality `lcpp_bench.das` has a flag for is a defect: add a run
`../performance/gen_bench_records.das` spawns, or a `lcpp_bench.das` command a `../PROFILE.md`
section documents, instead.** A second instrument's numbers cannot be compared to any row the
board already carries.

**A new instrument that clocks a served turn for a modality `lcpp_bench.das` has no flag for goes
under `../harness/`, never under this folder.**

**An out-of-process observer - a script watching a benchmark process from outside - never
measures what that process can measure about itself; that measurement goes inside the process
instead.**

**A diff that adds or changes a data file an instrument reads or writes that holds a third-party
wall - a wall-clock time measured for a binary this repository does not build - outside
`../performance/records/` and `../PERF_LEDGER.md` keeps that file untracked, owned by exactly one
instrument, re-derivable from a command in that instrument's header comment, and never an input
to a board cell.** A tracked or shared copy of a third-party wall becomes a stale baseline nobody
re-derives.

**A diff that adds an instrument that prints the difference of two wall-clock times, or changes
what it times or that report row, also prints both of those times on that report row.** A
plain elapsed-time report row - one clock pair, no attribution across stages - is not a
difference.

**A diff that adds a GPU kernel emitter in an instrument ships rows in `../PERF_LEDGER.md` for a
board cell or instrument that times the kernel.**

**A diff that changes a GPU kernel emitter in an instrument either ships before/after rows in
`../PERF_LEDGER.md` for a board cell or instrument that times the kernel, or names in the PR body
the compare showing the emitted kernel code byte-identical before and after: its emitted source
text, or the binary it builds into (for SPIR-V, the words `DASLLAMA_VK_SPV_DUMP` writes).**

**A diff that adds a result-row mode - one printing report rows carrying a time, a rate, or a
per-kernel occupancy count - to an instrument, or changes how such a mode reports or exits, makes
every result-row mode of that instrument exit non-zero on a run that prints no report row
carrying a figure - a line that prints a refusal or a bound miss in place of the figure does not
count - whatever stopped it: wrong flags, a failed load, a device that declines, a gate that
rejected every arm.** A run that matched nothing and reported success leaves a sidecar or a
record untouched, and its caller cannot tell.

**A diff that adds an A/B arm, adds or changes a lever an instrument's A/B arm reads - a lever in
a file under this folder - or changes how such an arm reports or exits, makes that instrument exit
non-zero when the lever leaves what the run executes unchanged - or, when the check runs before
the arm, print a warning naming the inert lever.** A lever that silently no-ops prints a 1.00x
report row nobody can tell from a real tie.

**A diff that adds an A/B arm of an instrument over a prompt corpus, or changes the lever such an
arm reads or how the arm reports, makes that arm print one report row per prompt, never one
aggregate ratio alone.** Prompts differ in how much the lever helps, so a per-prompt loss hides
inside a winning mean.

**A diff that adds a report row measured over rounds, or changes the instrument's timed body, an
input picking a round's backend, codec or session shape, or how the report row is computed or
reported, states in the instrument's header comment how many warmup rounds a report row drops,
zero included, and computes every number the report row reports over the rounds after them.**

**A diff that adds a report row measured over rounds, or changes the instrument's timed body, an
input picking a round's backend, codec or session shape, or how the report row is computed or
reported, prints no number on the report row when any run the report row covers refuses - only
the refusal and its reason.** A partial report row reads like a measured one and is a different
quantity.
