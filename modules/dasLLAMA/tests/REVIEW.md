# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest, and owns the asserts
of every helper it calls; a test file is a `.das` dastest runs - one holding a `[test]` function, or
one whose `cant_`, `failed_` or `invalid_` prefix makes its compile the assertion.

**A kernel-unit cell - one in which its own code, a helper in its file or a `_*.das` helper module
dispatches a `[metal_dispatch]` / `[vk_dispatch]` class or calls a `../dasllama/dasllama_math*.das`
kernel (a function writing an output buffer from operand buffers), directly or through a
`../dasllama/` router whose only job is to pick which compiled form of that class or kernel runs -
applies `REVIEW_KERNEL_CELLS.md` (beside this file) too, wherever the diff puts it.**

**A diff that touches a test file or cell `REVIEW_PINNED_GATES.md` (beside this file) lists, or
changes which `run.das` suites list one, applies that checklist too.**

**A diff that adds a cell or an assert whose expected value must be kept in step with something
maintained outside the cell (a document, a checked-in table, a committed artifact's form, a roster
or a knob list) - never a value the cell's own claim defines - or that a checked-in table names as
its evidence, applies `REVIEW_PINNED_GATES.md` too.** An assert that compares against a
`../dasllama/` constant by name, not against a literal copy of it, keeps nothing in step.

**A cell that calls a lane setter or a driver setter, in its own code or through a helper under
this folder, passes a loader parameter that takes a family's lane, or whose claim depends on which
route or serving lane runs it - a lane pin, a driver hook, a CPU-vs-GPU compare - applies
`REVIEW_LANE_PINS.md` (beside this file) too.** A lane setter picks a family's q8 or float serving
lane (`set_<name>_q8`, `set_asr_fp32`, `set_asr_tower_fp32`, or a facade call making one); a driver
setter is any other `../dasllama/` call writing process-global state - not a module global - that a
later load, route choice or kernel dispatch reads; a loader's own box-profile setters are the
loader's, not the cell's.

**A cell that feeds, preprocesses, or asserts on media bytes an encoder consumes - pixels or
audio samples - compares an encoder's output rows against a second source, or compares ASR
transcripts, applies `REVIEW_MEDIA_CELLS.md` (beside this file) too.** An encoder is any stage
of a vision tower, an audio tower or a speech synthesizer - never the language model that reads
their rows.

**Weakening `test_metal_float_a_gate.das` - the gate that checks the MSL emitter refuses a float A
operand (the activation input) to a `tmm2d_*` tiled matrix-multiply call without the
`[metal_kernel(float_a_ok=true)]` annotation - is a defect.**

**Weakening `st2_source_gate` in `test_metal_prefill_kernels.das` - the check that the StyleTTS2
harmonic-source kernels match the CPU chain under both resample laws - is a defect.**

**Every PR runs `run.das -- --suite model-free`, and `run.das -- --changed` on a box with the
models stocked (the stocked files of the areas the change reaches; a core module with no
`MODULE_AREAS` row reaches every area), plus every test here the change reaches - never the whole
directory, never the whole `stocked` suite where `--changed` reaches fewer than every area.** A
change reaches a test when it alters anything the test's result depends on - the test file, a
shared helper, engine code it exercises, an in-tree fixture or corpus it reads, or a name it
asserts on; a comment-only edit reaches none.

**A PR whose change reaches a cell that skips on, or picks its tolerance or its code path by, a
hardware or build capability the box may lack - a device, a module the build may omit, a kernel the
instruction sets of the box's CPU do not carry - runs that cell on a box that has it, and names
that box in the PR body.** A reached cell run without the capability did not run what the
capability selects.

**A PR that adds a cell loading a model above the large tier (`LARGE_TIER_BYTES`,
`_model_tier.das`), or changes something such a cell's result depends on, also runs that cell
with `DASLLAMA_PARITY_FULL=1` set, on a box with the model stocked, through a `run.das` suite
listing the cell's file - with `--arm` naming the cell when `run.das` accepts `--arm` for that
suite - and names the box in the PR body.** A run without `DASLLAMA_PARITY_FULL=1` skips every
such cell and passes.

**The `--changed` run a PR cites carries no `--exclude`** - an excluding run is the
iteration form between PRs; a PR that ships on it never ran the coverage it dropped.

**A test file in this folder whose cells cannot hold under `DASLLAMA_CPU_PREFILL=1` says so in its
header (its top comment block) and joins the list of files that sit in no suite (`exempt` in
`test_run_suites.das`) in the same change.** `DASLLAMA_CPU_PREFILL=1` is what the runner arms for
every suite.

**A diff that gives `run.das` a global whose initializer spawns, logs, writes the environment or
touches the filesystem is a defect - do that work inside the function that needs it.** Anything
that fires on require fires inside every test that requires `run` by bare same-dir name.

**A diff that gives `_model_tier.das`, or a `tests/` fixture it requires, an `[init]` that
declares the CPU-prefill intent (`allow_cpu_prefill`) is a defect - the intent is declared in
the `[init]` of the test file that needs it.** `test_cpu_prefill_tripwire.das` requires
`_model_tier` and asserts that the guard trips while the intent is undeclared.

**A cell asserting a chat template's INSTRUCT wire - a closed empty thought block and no
thinking gate - calls `set_thinking(c, false)` on its `ChatSession` before the first turn.**
`ChatTemplate.think_default` is `true` unless a family clears it, so a first turn with no
`set_thinking(c, false)` call renders the thinking gate and the cell asserts the wrong wire.

**Every test RUN runs under `-jit` - never the interpreter, never AOT.** A compile-only CI lane
passes dastest's `--compile-only`. Under the interpreter a model-gated suite's cells skip, and
a run of skips is not the coverage the suite owes.

**A diff that registers a test file in this folder in a `CMakeLists.txt` is a defect - a
`run.das` suite listing is the only registration these files get.**

**A diff that adds a test file here adds its `CLAUDE.md` entry in the same change.** A file's
entry is the clause that describes the file, named with or without its `.das` suffix.

**A diff that adds, removes, moves or renames a cell or a gate function (a named function a cell
of a `test_metal_*_kernels.das` or `test_vulkan_*kernels.das` file calls to compare one kernel
against its CPU oracle), or changes a cell's suite, its skip condition, or any fact a `CLAUDE.md`
entry states about it - the class it dispatches, the shapes, lengths, formats or lanes it sweeps,
the predicate an assert reads, the bar it holds, its controls - corrects in the same change the
`CLAUDE.md` entry of every test file running the cell, counts and skip clauses included; a file
with no entry owes none.** A `{a,b}` shorthand or a suite roster needs no update, and an input row
that adds no fact the entry states changes no claim.

**A diff that adds or changes a cell a `CLAUDE.md` clause describes as one of a class - the cells
of one helper, the arms of one name pattern (`mtp-ff-<tag>`) - keeps that clause's counts and skip
clauses true for every cell in the class.**

**A diff that adds, renames, or drops an arm name - the literal passed to `arm_on(t, name)`
(`_model_tier.das`), what `--arm` matches - updates the arm census in `CLAUDE.md`'s "Arm filter
mechanics" section in the same change.** An arm the census does not name is unreachable to
whoever is choosing what to run.

**A diff that adds, changes, or drops a cell's skip condition other than the runner's own
`--arm` / `--family` filter - a `t |> skip` or an early return - updates in the same change the
header of every test file that runs the cell, wherever the cell is defined, so that the header
names every fact the cells that file runs skip on.**

**A diff that changes `run.das`'s flag surface - a flag, a suite name, an area name, or what a
flag does - adds it to or corrects it in `CLAUDE.md`'s "Run suites ONLY through the runner"
block and `../CLAUDE.md` in the same change.** A data row in a table `run.das` looks up -
`MODULE_AREAS`, a suite's or an area's file list - is not the surface. A copy the code has left
behind sends its reader to a flag that no longer does what the text says.

**On every platform, a cell that neither asserts nor registers a skip is a defect.** A cell that
returns without asserting registers `t |> skip` there; `feint` is a print, not a skip.

**A cell whose claim holds only on some boxes or run modes - it depends on the device or its
memory (a card too small for the shape the cell asks), the build, a run-mode knob's value, the
value a tune companion (a function whose body the `[tune]` stamp in force selects, like
`kq_tileform_of`) returns, a host toolchain or the stocked files - registers its skip on that fact
before the asserts that need it, never a bare return and never a failure.**

**A cell's skip condition, and any condition that picks a cell's assert or bar by something
other than an input the cell sets itself (a format, a shape, a loop value), keys on a fact the
box owns - a device capability, a run-mode knob's value, the value a tune companion returns on
this box, a host toolchain's presence, a compile-time module-presence check (`typeinfo
builtin_module_exists`) - or on a stocked fixture beside the models (a model file, an mmproj, an
oracle dump); never on the existence of an artifact this repo's build or a previous test run
produced (a minted `.dlim`, a generated binary, a dump a test wrote).** An artifact condition goes
permanently false when its producer moves.

**A cell that loads or gates on a stocked model file - a `.gguf` carrier, its shards, or an
mmproj - gates through `model_available` (`_model_tier.das`), one call per file; a test that
cannot require `_model_tier.das` open-codes the same two checks: the file and every sibling shard
are present, and their total size is under `LARGE_TIER_BYTES` unless `DASLLAMA_PARITY_FULL=1` is
set.** `DASLLAMA_PARITY_FULL=1` is a final pre-PR switch, not the iteration loop.

**A test - or a program a test builds or spawns - whose subject is not the `.dlim` image rail (a
cell whose subject is a lane knob's effect on the image identity has the rail as its subject)
never mints or maps a MODEL image: it either runs with `DASLLAMA_IMAGE=0` in its environment, or
calls no baking loader - `load_<family>_tower`, `load_<family>_embedder`, `load_model_cached`,
`load_tts_model`, each of which writes a `.dlim` beside the model when `DASLLAMA_IMAGE` is
unset.** Such a test loads a media carrier in memory from the family's `stage_*` staging - its
`mint_*` twin, or `cache_via_image_staged` with an empty image path.

**A predicate whose value the BOX decides (a device capability, a policy default) and that
therefore cannot differ between two runs on one machine is never tested through its own
value; test it through the argv it gates or the mode it selects.**

**A test for an added, moved, or edited registration reaches the registered thing through its
registry, and never calls it directly.** A registry is the storage a `register_*` call writes
and a lookup reads at dispatch - a table, a list, or a single hook global - or the `[EnvConfig]`
env registry.

**A `corpus_case` call in `test_tokenizer.das` that does not assert BOTH the exact reference ids
and a lossless round-trip is a defect.**

**A test that compares generated tokens, ids, or logits without logging both sides in the most
readable form its fixture carries is a defect: with a tokenizer, each side's decoded text for a
token or id compare, one log record a side with its newlines escaped (`log_gen_texts` in
`_model_tier.das` writes that form), and each side's argmax decoded piece plus the measured max
difference for a logits compare; with a raw-id fixture and no tokenizer, the ids, one log record
a side.** A failure, or a pass that looks wrong, must be readable in the log, not only as an id or
float difference.

**A size, depth, or row count that a cell's name, a comment inside the cell or in a helper it
calls, or an assert's text claims about what the cell exercises, and that the cell does not pass
as a literal argument to the kernel or function under test that it itself dispatches or calls, is
asserted in that cell by an assert on the count.** A cap, a resize, or a counter asserted to show
a route ran is not evidence the number was reached; a device's geometry (subgroup width, SM count)
is no coverage claim.

**An exact token or id compare over a prompt whose continuation can tie, whose two sides run
different lanes, backends, batch shapes or kernel forms, is a defect - it takes the forced-feed
logits-tolerance form: the same fixed tokens fed to both sides, the logits compared within a
bar. A counting cell, whose prompt forces a continuation that cannot tie, stays exact on any two
sides.**

**An exact token or id compare over a prompt whose continuation can tie, whose two sides run one
code path or the same kernels over another storage layout (a flat K/V cache against a paged
one), states in the cell what makes them compute bit-identical logits - the shared entry point,
an assert pinning the lane, or a lane pin plus the layout that alone differs.**

**A test in this folder that loads a stocked artifact whose producer none of the following names
is a defect.** A stocked artifact is anything the test reads out of `models_dir()` that this
repo's build does not produce - model files, mmprojs, front-end packs, image fixtures, oracle
dumps. Any one of these names the producer: a row in `../performance/model_specs.das`; a row's
`companions` list; a row in `asr_catalog` (`../performance/profile_common.das`); a convert
script in `../harness/`; for an oracle dump, the mint script stocked beside the dumps under
`models_dir()`, named by the test that loads the dump.

**A test that reads a vision encode oracle dump without naming the minting arm in its header - the
backend, the flash-attention setting and the mmproj precision the dump came from - is a defect.**

**A diff that adds a model-loading block to a file of a `run.das` suite that accepts `--arm` tags
it with the carrier family it loads: one `family_on(t, name)` check per carrier family
(`_model_tier.das`), the token that family's entry in `CLAUDE.md`'s family filter list (a family
with no entry adds one in the same change), the checks in sequence so a block loading several
families runs only under a filter naming every one of them.** Kitten and kokoro are two families
of one architecture; an untagged block runs under every `--family` filter.

**A diff that adds or moves a batched-vs-sequential parity cell - one comparing the batched
stack against a per-session sequential forward whose sequential side runs on the CPU - onto a
carrier above `LARGE_TIER_BYTES` (`_model_tier.das`) is a defect.** The batched code paths get
their parity on small models, through pins; a cell whose two sides both run on the device
streams nothing beside the device's bytes, and the tier does not bind it.

**A cell never changes an environment-read knob - one the running config reads once, at context
init - for its own process once that process has read it; it passes the value to a child it
spawns, either as the spawn's environment argument or, for a spawn that inherits the parent's
environment, by setting the knob right before the spawn and restoring the previous value right
after (unsetting it if it was unset).** A set after that process starts is invisible to a config
already read.

**A cell whose claim depends on an environment-read knob its own process has already read names
that knob's value in the text a red prints - the cell label or the assert.**

**A diff that adds or loosens (lets pass an input the old assert failed) a bound assert in a cell
that is not a kernel-unit cell, or moves one onto another route, lane or backend, ships in the
same change, in each such cell holding the assert, a control that lands outside that bound on that
route.** A bound assert holds a computed difference, rate, error or run-decided count within a
nonzero tolerance or past a floor or ceiling - never an index guard or a route counter. A bound
nothing has exceeded where it is applied is not known to discriminate there.

**A diff that adds, in a cell that is not a kernel-unit cell, an assert on a counter showing a
route ran ships in the same change a control under which the route does not run and the counter
stays at its value from before the route.** A counter that rises on both legs shows nothing about
the route.

**A control for a bound assert in a cell that is not a kernel-unit cell changes an input the
computation reads - a zeroed weight region, a poisoned input element, a mechanism disabled - and
re-runs the compare; a value added to the output after the fact is not one.**

**A float compare against a bar that a diff adds, changes, moves or extracts into a helper reads a
NaN in the output, the reference, or their difference as outside the bar: it passes on
`d <= bar`, counts a miss on `!(d <= bar)`, and any largest difference it keeps reads a NaN
element as infinite - or it routes through `_compares.das`' `check`, `check_count` or
`sweep_check`, which do.** `d > bar` is false on a NaN, and a running `max` drops a NaN that a
later finite element follows, so an output holding one, or a NaN sentinel the kernel never
overwrote, passes.

**A poison control on a tower a GPU driver serves - a run of the gate with a block's weights
zeroed, which must fail - zeroes that block's GEMM weights in every plane the served route reads
them from.** A poison the served route never reads passes on a broken kernel.

**A function in a file of this folder that requires a module behind an optional `require ?<mod>`
never names that module's types in its signature - leave a parameter that would carry one
untyped, and drop a return type that would name one.** A signature cannot sit inside a
`static_if`, so a build without the module fails the compile on it.

**A function in a file of this folder that requires a module behind an optional `require ?<mod>`,
and that has no untyped parameter, names anything that module declares only inside a
`static_if (typeinfo builtin_module_exists(<mod>))` body.** In a build without the module a
fully typed function is inferred anyway; one with an untyped parameter is inferred only at a call
site, which its caller has already guarded.
