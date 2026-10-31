# dasLLAMA Measurement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_MEASUREMENT*.md` beside this file. Planned work: `followup_metal.md`,
`followup_vulkan.md`, `followup_general.md`.

**A rule here that governs a figure binds only a figure the change writes or changes - in the repo
outside `site*/` (repo root), in a commit message, or in its PR body - never one a changed line
carries unchanged.**

A figure is any number a run produced - a rate, a wall, a count, a byte size, or a ratio or
percentage of two such numbers, produced by the runs its sides came from - or an error or
agreement reading (a cosine, a max-abs error) that a test's tolerance, or the bound an
instrument's bounded-difference compare checks against, is set from, or that a change cites as
evidence that a tolerance set for another cell, model or tier holds for the one it adds or alters.
A test suite's pass, fail and skip tally, the count of kernels a compare of two dumps of
emitted kernel code finds identical or different, and a value inside the `provenance` object of a
`performance/defaults/<class>.tune-defaults.json` profile or a tune sidecar, where it sits in that
file, are not figures; the same value written into other text is one.

A served-turn leg is prefill, decode, a batched decode row, or a whole served request end to end;
the wall of one internal stage (one model component's forward pass, one decoder block) is not a
leg.

An instrument is a file whose printed or returned result is a wall or rate of a run it drove,
read by its own clock or by timestamps a GPU driver returns, wherever the file sits; a file
reading a spawned child process's clock, and a profiler-gated report a serving path prints beside
its served output, are not one. A harness is the `harness/` script that drove
a run, where one did. A route is the end-to-end code path a served turn takes, including the
default route - the one a run given no command-line arguments and no environment overrides takes.

The flags of a serving run are the tier (`-jit` or not), the `DAS_TUNE_POLICY` value in force,
the CPU kernel backend the registry served (the one `DASLLAMA_PIN_BACKEND` pinned, or, with it
unset, the one the auto-selection picked), the GPU backend where one served with its device, on
Vulkan the coopmat mode (`DASLLAMA_COOPMAT`), and on CUDA the prefill GEMM form
(`DASLLAMA_CUDA_MMQ`) and whether a launch overlaps the tail of the one before it
(`DASLLAMA_CUDA_PDL`). The flags of an instrument run are the tier, the `DAS_TUNE_POLICY` value,
the device, `DASLLAMA_COOPMAT` on Vulkan or `DASLLAMA_CUDA_MMQ` and `DASLLAMA_CUDA_PDL` on CUDA,
and the instrument's arguments. A run that serves a model through the engine carries every flag of
a serving run, whatever drives it.

The flags of a test run - a dastest test whose reading sets or checks a tolerance - are the tier,
the `DAS_TUNE_POLICY` value, the device, the CPU kernel backend the registry served, the GPU
backend where one served, and every environment variable it sets and every engine setter it calls
to choose the code path it checks. An environment-variable flag a serving, instrument or test run
left unset is named as unset.

An environment override is an env variable given to the run that changes what it compiles,
tunes, or serves, other than the run's flags.

A board cell is a run whose reading lands as a row of `performance/records/<box>.json` or as a
figure in `PERF_LEDGER.md`.

A spawned cell is a run `performance/gen_bench_records.das` spawns whose reading lands as a row of
`performance/records/<box>.json`; a manual cell is the `benchmarks/lcpp_bench.das` command a
`PROFILE.md` section documents, run by hand, its reading written as such a row.

The `-jit` script is `benchmarks/lcpp_bench.das` run as a script under `-jit`, not as the released
exe; a `-jit` reading is a wall or rate that an instrument - the `-jit` script included - printed
while running under `-jit`, or a ratio of two such prints, whoever computed it.

A rig leg is a child cell `performance/gen_bench_records.das` spawns whose record row has `engine`
`das`, named by that row's `(backend, flavor)` pair whatever its `workload`.

A third-party row is a figure from a run of a third-party program serving the same model on the
same input size.

A cell, script or exe of this repository is a spawned or manual cell, a script in the tree at the
diff's tip, or an exe built from that tree; a script outside the tree is not one, whatever it
calls.

A box can mint a figure from a spawned cell when a rig leg it neither refuses nor skips drives the
path the owed figure measures and `performance/gen_bench_records.das --catalog official` carries the
model; from a manual cell when it runs the documented command's OS and backend and holds its model;
a figure owed for new code when a rig leg or a documented command reaches that code's input. A
figure owed for a route that no rig leg drives and no `PROFILE.md` section documents a command for
is one no box can mint.

A board box is a box with a `performance/records/<box>.json`. The release re-mint is a diff that
runs `performance/gen_bench_records.das` with a release's exe on one or more board boxes and writes
their rows into `performance/records/<box>.json`.

A `held` entry is a `performance/records/annotations.json` entry carrying a board row's `gguf`,
`box`, `engine`, `backend`, `flavor` and `workload`, whose `held` field names the follow-up ledger
row that keeps the row off the board.

**A diff that owes a figure from every board box of the author's lists the author's board boxes in
its PR body.**

**The naming that a rule governing a figure asks for attaches to a figure only by a sentence, a
table heading, a provenance line or a `PERF_LEDGER.md` entry's opening paragraph that
unambiguously covers it, or by a citation of a passage or board row whose provenance covers
it.** A provenance line is a paragraph whose whole content is provenance - the harness, flags, box
and exe; it covers the rows it names or, when it names none, the paragraphs and tables after it
up to the next heading, the next provenance line, or the end of the `PERF_LEDGER.md` entry it
sits in. A `PERF_LEDGER.md` entry is a bullet whose bold head carries a date, or a `###` section
whose heading line carries a date; its head is that bold head or that heading line, and its opening
paragraph is the paragraph its bold head sits in, or the first paragraph under its heading line. A
dated bullet under a dated `###` heading is an entry of its own; every rule reading an entry reads
the innermost one, and the section's entry is the text before its first dated bullet.

**Where a paragraph or table of a `PERF_LEDGER.md` entry carries a figure from a run other than
the one the entry's opening paragraph names, that paragraph or table, or a provenance line
covering it, names the other run.** The provenance the opening paragraph states covers every later
paragraph and table of that entry that no naming of another run covers.

**A tag (`external`, `direction-grade`, `out-of-process`, `debug-jit`) in the dated parenthesis of a
`PERF_LEDGER.md` entry's head covers the whole entry. Anywhere else, a tag covers only the
figures of its own sentence. A tag inside a parenthesis covers only that parenthesis's figures,
and a tag in a parenthesis with no figure covers nothing. A tag outside any parenthesis, in a
figure-free sentence of a provenance line or of an entry's opening paragraph, covers the figures
that line or paragraph covers.**

**A `PERF_LEDGER.md` entry states a turn wall (the wall of one whole served request, whatever the
modality) or a tok/s rate of the engine this repository builds only when one of these produced
it, named in the entry: `benchmarks/lcpp_bench.das`, as the released exe from `daspkg release` or
as the `-jit` script; a spawned or manual cell; for a modality `lcpp_bench.das` has no
command-line flag for, the `harness/` instrument, with its command line; for the browser build,
the parrot example's measure rail (`examples/dasLLAMA/parrot`, the browser and its worker count
named) or the node runner `examples/dasLLAMA/wasm/run_node.js` over the AOT host, with its command
line; for a figure timed
through the scheduler or the server, `harness/served_bench.das` with its command line, or an
out-of-process replay named as the replay (its name, its request count), over a server whose
flags the entry states.** A turn wall or tok/s rate printed by any other instrument stays in that
run's log: the ledger entry carries what the instrument decided - served, declined, how many rows
it counted - never that wall or rate.

**A pair of `-jit` readings held against each other keeps each arm's absolute wall or rate
beside the pair, wherever the diff states it - checked-in text, commit message or PR body.**

**A `PERF_LEDGER.md` entry stating a figure no cell, script, or exe of this repository produced
tags it `external`; a figure one of them produced carries no such tag, whatever binary the run
timed.**

**A difference, ratio, or percentage of two measured figures - a wall or a rate - written into
`PERF_LEDGER.md` carries the raw figure of every side no cell, script, or exe of this repository
produced.**

**A ratio of two `-jit` readings, written into `PERF_LEDGER.md`, names the arm - the side of
the compared pair - it is measured against.**

**A `PERF_LEDGER.md` entry tags its reading `direction-grade` when the reading compares across
two processes or two commits.**

**A `PERF_LEDGER.md` entry tags its reading `out-of-process` when the wall was read by a clock in
a process other than the one doing the timed work - a client timing a server among them.**

**A diff that writes a `-jit` reading outside `PERF_LEDGER.md` tags it `debug-jit`, and cites the
committed board row of the same model, served-turn leg and backend where one exists.**

**A published figure that no cell, script, or exe of this repository produced, written down in any
checked-in text, names its source and the report it appears in.**

**A figure a third-party program (an exe or a script) produced, and no cell, script, or exe of this
repository produced, written down in any checked-in text, names its recipe: that program, its
version, the model, the command line, and any environment settings the command line does not
show.** A citation of an architecture section that, together with the `cmd` and `versions` fields
of the cited `performance/records/<box>.json` row where there is one, states that run's recipe in
full - no placeholder in its exe, model or command line - names it.

**A figure an instrument outside the tree produced, written down in any checked-in text, names
that instrument by its role, the model, the box and the flags it ran under; a private instrument
is never named by its repository or command line - its role names it.**

**A figure the diff's text gives as the reason it ships one value, formula or kernel over
another comes from a script in the tree at the diff's tip, or an exe built from that tree, named
with its command line.** A figure from a script outside the tree cannot be measured again.

**A diff never changes a `PERF_LEDGER.md` entry beyond repairing a link or anchor; a new figure,
caveat or correction goes in a new dated entry.**

**A diff whose figures contradict a dated `PERF_LEDGER.md` entry's figures or conclusion - a
sentence of the entry stating what its figures show - adds a new dated entry that carries them and
names the old entry and the figure or conclusion it replaces.**

**A `PERF_LEDGER.md` entry never carries a figure given as the reason to pick a value, formula or
kernel that the same diff neither lands nor names as the Done of a `followup_*.md` row citing that
entry - that figure goes in the PR that lands the code.**

**A diff that lets a served turn, or a media encode a turn takes (an image, a video frame or an
audio chunk), reach code no spawned or manual cell's input reaches - a new dispatch arm, a
window-size arm inside a path a cell drives at other sizes, or a changed default route - adds a
spawned cell whose input reaches it, writes the new cell's figures from every board box of the
author's that can mint them into a dated `PERF_LEDGER.md` entry, in the same change, and names in
its PR body the `followup_general.md` row under which the release re-mint owes its rows.**

**A change that owes a ledger figure no box of the author's can mint names instead, in the same
change, an artifact that shows the new code ran end to end - a record, a gate output, an
instrument's print - or the profile output that shows the change is faster
(`benchmarks/lcpp_bench.das`'s `forward_profile` rows, or the tier's `DASLLAMA_GPU_PROF=1` token
ledger), with the flags that run took.**

**A diff that changes `dasllama/dasllama_metal_kernels.das` (the kernel library
`metal_decode_init` compiles), `performance/profile_common.das`, or a `dasllama/` module
`benchmarks/lcpp_bench.das` requires directly copies into the PR body the `sanity:` lines and the
`tune gate:` line of an `lcpp_bench` run of the fat exe built from the diff's tree, or says that
run printed none.** A fat exe is what `daspkg release --fat <class>` builds (`DAS_TUNE_MODE=fat`,
`ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start`).

**A diff whose PR body, or a line it adds or changes, states that the diff makes an existing
route - one the tree served before the diff - run faster or do less work, where a rig leg runs the
changed lines, writes that leg's walls from every board box of the author's that can mint it into
a dated `PERF_LEDGER.md` entry naming the exe's commit and the command, and names in the PR body
the `followup_general.md` row under which the release re-mint owes every board box's row.** The
board is the module's committed record of what a release's exe costs to serve; a kernel win that
lands in no ledger entry is invisible to the next regression check, and a row minted between
releases shows a speed no shipped binary gives
(`ARCHITECTURE_MEASUREMENT_RELEASE.md#board-rows-release-boundary`).

**The release re-mint re-mints, on each board box it runs on, every row a `followup_general.md` row
owes it there, deletes any `held` entry holding one, names each row in its PR body, and puts each
re-minted row's last board figure beside the new one in a dated `PERF_LEDGER.md` entry in the same
change.**

**No diff but the release re-mint writes a row into `performance/records/<box>.json`; a figure any
other diff owes lands in a dated `PERF_LEDGER.md` entry.**

**A diff whose PR body, or a line it adds or changes, states that the diff makes an existing
route - one the tree served before the diff - run faster or do less work, where no rig leg runs
the changed lines, states in its PR body that no rig leg runs them and names the instrument that
does, with its command line.**

**A diff that changes what a board cell loads, runs, counts or times - the model or corpus it reads,
the model components (encoder, decoder, tower) it runs or the backend that serves them, the tokens
or rows it divides the wall by, or the reference build it times (`DEFAULT_REF_SHA` in
`benchmarks/setup_lcpp_ref.das`, or anything else deciding which reference binary or environment the
run measures) - deletes each affected row or gives it a `held` entry, writes the cell's new figures
from every board box of the author's that can mint it into a dated `PERF_LEDGER.md` entry beside the
old ones, and names in its PR body the `followup_general.md` row under which the release re-mint
owes the cell's rows.**

**A `PERF_LEDGER.md` entry that withdraws an earlier entry's figure states the withdrawal's
reason.**

**A rate or wall of any served-turn leg, or a ratio of two such rates or walls, written down as a
measurement rather than as a prediction is a defect unless it cites the committed board row it came
from, or names harness, flags, environment overrides, box, the exe or script that ran it, and - for
a figure aggregated over more than one timed run or input - the number of runs or inputs and the
spread of the per-run figures: the standard deviation, or the min and max - or, where the instrument
prints no spread, the `followup_general.md` row owing it.**

**A diff that records a measured number an instrument prints - a time, a rate, or a figure
computed from one - or changes what such a number measures, ships that number's alternate in the
same change: the same instrument's row on the same fixture and input size with exactly one flag or
environment override changed; the same instrument's row on the same fixture, input size, flags and
overrides at a named other commit, tagged `direction-grade`; the other arm of the race the number
comes from; or a third-party row that cites the architecture section holding its recipe - all of
it, less what the row's own `cmd` and `versions` fields carry when it is a
`performance/records/<box>.json` row.** A number with no alternate beside it cannot
be compared to anything.

**A diff that records a third-party row lands, in the same change, in the
`ARCHITECTURE_MEASUREMENT*.md` section that describes the instrument the row is compared with -
adding that section where none exists - the row's recipe: the program, its version, the model, the
command line, and any environment settings the command line does not show. A row of
`performance/records/<box>.json` may leave out what its own `cmd` and `versions` fields carry; any
other third-party figure lands the whole recipe.** The next entry re-runs the
reference from the section, not from the earlier entry's prose.

**A figure a cell, script or exe of this repository produced that is not a served-turn leg, whose
value depends on the box it ran on, names the harness, the flags, the environment overrides, the
box and the exe or script that produced it - or names the committed record file and the row or key
it sits under, when that record is a board record or its own fields name that harness, flags, box
and exe.** A reading from a GPU, or from a reduction split across lanes or threads, depends on the
box.

**A figure whose value is the same on every box names the build, fixture, or command that
reproduces it.**

**A race a diff adds to a shipped exe's startup - a timed run-off between candidate
implementations whose winner sets a knob - runs on synthetic inputs only, never loading a model,
never spawning a child process.** A shipped exe carries no vehicle model (a real model file a
harness run drives) and no harness script, so a model or a child there is a hang or a silent
skip.

**A shipped exe's startup race races only a GPU pipeline-state twin - two precompiled pipeline-state
objects the dispatcher picks between at run time, raced to set a runtime knob - never a `[tune]`
kernel family** (`ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start`). A `[tune]` winner needs
a recompiled clone the shipped exe does not carry.

**A diff never adds a confirm outside `harness/` - it goes there instead.** A confirm is an
end-to-end A/B: it serves a vehicle model in a spawned child, runs two implementations of this
engine the run could adopt, and picks the winner.

**A diff that pins the CPU kernel backend - `pin_kernel_backend`, `select_kernel_backend`, or
`DASLLAMA_PIN_BACKEND` - pins it before the load it governs.** The loader repacks weights into
the pinned backend's layout, so a pin set after the load never reaches them.

**A diff that adds or changes a CPU kernel-backend pin path makes the run refuse or skip when
`kernel_backend_pin_refusal` names a reason - a backend the box has not registered, a batch-only
backend, or one the box cannot run - never fall through to the default.** A misspelled pin that
falls through measures the default backend under the pinned one's name.
