# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE.md`, `../ARCHITECTURE_IMAGE_LEDGER.md`. Planned work:
`../followup_general.md`, `../followup_vulkan.md`, `../followup_metal.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest, and owns the asserts
of every helper it calls; a test file is a `.das` dastest runs - one holding a `[test]` function, or
one whose `cant_`, `failed_` or `invalid_` prefix makes its compile the assertion. A gate function
is a named function a cell of a `test_*_kernels.das` file calls to compare one kernel against its
CPU oracle. A kernel class is a class that a GPU dispatch annotation - `[metal_dispatch]`,
`[vk_dispatch]` or `[cuda_dispatch]` - compiles to a device kernel. A file's `CLAUDE.md` entry is
every `CLAUDE.md` clause stating a fact about the file (named with or without its `.das` suffix),
one of its cells, or one of its gate functions, wherever in `CLAUDE.md` the clause sits; a brace
list naming several files at once (`test_metal_{prefill,decode}_kernels`) or a suite roster states
none. A compare's two sides are the two computations whose mismatch reds the assert; a reference
that only decides where the compare stops is neither side. A bound assert is a compare of two
computed sides, or a measured quality figure against a tolerance; a floor on a capacity, a
configuration or a roster size is not one, nor is an index guard or a route counter. A stocked
artifact is a file a test reads from under `models_dir()` that neither this repo's build nor its
tests produce - model files, mmprojs, front-end packs, image fixtures, oracle dumps. An area is a
name in `run.das`'s `AREAS` list, standing for the files `run.das`'s `area_tests` returns for it.
A box fact is a condition one box or run mode meets and another does not: the device or its
memory, whether the build carries a module (`typeinfo module_exists` or `typeinfo
builtin_module_exists`), the value of an environment variable or runner flag the run reads, the
value a tune companion returns (a function whose body is the winner the `[tune]` framework
picked for this box), a host toolchain, or a stocked artifact's presence.

**A kernel-unit cell - one in which test code outside a CPU oracle (the cell, a helper in its file
or a `_*.das` helper module) dispatches a kernel class or calls a `../dasllama/dasllama_math*.das`
kernel (a function writing an output buffer from operand buffers), directly or through a
`../dasllama/` router whose only job is to pick which compiled form of that class or kernel runs -
applies `REVIEW_KERNEL_CELLS.md` (beside this file) too, wherever the diff puts it.** A CPU oracle
is the same computation written in plain code in this checklist's folder - a cell, or a `_*.das`
helper module here; only its operands may be built, quantized, widened or read through
`../dasllama/` functions, and a kernel under `../dasllama/` is never one; a diff that changes this
definition changes the companion's in the same change.

**A diff that touches a test file or cell `REVIEW_PINNED_GATES.md` (beside this file) lists, or
changes which `run.das` suites list one, applies that checklist too.**

**A diff that adds a cell or an assert whose expected value restates, or reads, something
maintained outside the cell, or that a checked-in table names as its evidence, applies
`REVIEW_PINNED_GATES.md` (beside this file) too.**

**A cell that calls - in its own code or through a helper - a `../dasllama/` call that sets a
module or process global a later load, route choice or kernel dispatch reads (a `set_*`,
`enable_*`, `disable_*`, `pin_*`, `reset_*`, `restore_*`, `register_*`, `select_*_override` or
`unset_*` call), or that passes a loader parameter choosing
a family's serving lane (the q8 or float weight form the family runs), or whose claim depends on
which route or serving lane runs it - a compare of a CPU run against a GPU run among them - applies
`REVIEW_LANE_PINS.md` (beside this file) too.**

**A cell that feeds, preprocesses, or asserts on media bytes an encoder consumes - pixels or
audio samples - compares an encoder's output rows against a second source, or compares ASR
transcripts, applies `REVIEW_MEDIA_CELLS.md` (beside this file) too.** An encoder is any stage
of a vision tower, an audio tower or a speech synthesizer - never the language model that reads
their rows.

**Every PR runs `run.das -- --suite model-free`, and `run.das -- --changed` on a box with the
models stocked (every file its `PLAN` line counts), plus every test here the change reaches -
never the whole directory.** A change reaches a test when it alters anything the test's
result depends on - the test file, a shared helper, engine code it exercises, an in-tree fixture
or corpus it reads, or a name it asserts on; a comment-only edit reaches none.

**A PR whose change reaches a cell that skips on, or picks its tolerance or its code path by, a
hardware or build capability the box may lack - a device, a module the build may omit, a kernel the
instruction sets of the box's CPU do not carry - runs that cell on a box that has it, and names
that box in the PR body.** A reached cell run without the capability did not run what the
capability selects.

**A PR that adds a cell loading a model above the large tier (`LARGE_TIER_BYTES`,
`_model_tier.das`), or whose change reaches such a cell, runs that cell under
`DASLLAMA_PARITY_FULL=1` on a box with the model stocked - in a `run.das -- --changed` run whose
`PLAN` holds the cell's file, or in a `run.das --suite` run listing the file, with `--arm` naming
the cell where that suite takes `--arm` - and names the box in the PR body.** A run without
`DASLLAMA_PARITY_FULL=1` skips every such cell and passes.

**The `--changed` run a PR cites carries no `--exclude` and no `--only`; the one `--only` run a
PR cites is the CUDA pass - `--changed --only <tokens>` under `DASLLAMA_GPU=1 DASLLAMA_CUDA=1`,
its tokens matching every file of `run.das`'s `CUDA_WITNESSES` - beside the whole run.** A
trimmed run never ran the coverage it dropped; an excluding run is the iteration form between PRs.

**A PR cites every shard of a sharded run (`--shard i/n`) with its `REPORT` line and its box, and,
where the `--changed` run's `PLAN` line says the engine core changed, a `--suite stocked` run with
no `--exclude` or `--only`.** A shard is a fraction of the run's files; a core change's `--changed`
run holds one stocked test a family and backend, the stocked suite every one.

**A test file in this folder whose cells cannot hold under `DASLLAMA_CPU_PREFILL=1` says so in its
header (its top comment block) and joins the list of files that sit in no suite (`exempt` in
`test_run_suites.das`) in the same change.** `DASLLAMA_CPU_PREFILL=1` is what the runner arms for
every suite.

**A diff that gives `run.das` a global whose initializer spawns, logs, writes the environment or
touches the filesystem is a defect - do that work inside the function that needs it.** Anything
that fires on require fires inside every test that requires `run` by bare same-dir name.

**A diff that gives `_model_tier.das`, or a fixture in this folder it requires, an `[init]` that
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

**A diff that adds a test file here adds its `CLAUDE.md` entry in the same change, every clause
of it true of the file as added.**

**A diff that adds a cell or a gate function to a file whose `CLAUDE.md` entry accounts for every
cell, or every gate function, of that file - by name or by a clause of its own - adds the new one
to that entry in the same change.**

**A diff that removes, moves or renames a cell or a gate function, or changes a cell's suite, its
skip condition, or any fact a `CLAUDE.md` entry states about it - the class it dispatches, the
shapes, lengths, formats or lanes it sweeps, the predicate an assert reads, the bar it holds, its
controls - corrects in the same change the `CLAUDE.md` entry of every test file running the cell,
counts and skip clauses included.**

**A diff that adds, removes or changes a cell that a `CLAUDE.md` clause describes as one of a
class - the cells of one helper, or the arms of one name pattern - keeps that clause's counts and
skip clauses true for every cell in the class.**

**A diff that adds, renames, or drops an arm name - the literal passed to `arm_on(t, name)`
(`_model_tier.das`), what `--arm` matches - updates its suite's backticked arm roster in
`CLAUDE.md`'s "Arm filter mechanics" section in the same change.** An arm the roster does not name
is unreachable to whoever is choosing what to run.

**A diff that adds, changes, or drops a cell's skip condition other than the runner's own
`--arm` / `--family` filter - a `t |> skip` or an early return - updates in the same change the
header of every test file that runs the cell, wherever the cell is defined, so that the header
names every fact the cells that file runs skip on.**

**A diff that changes `run.das`'s flag surface - a flag, a suite name, an area name, or what a
flag does - adds it to or corrects it in `CLAUDE.md`'s "Run suites ONLY through the runner"
block and `../CLAUDE.md` in the same change.** A data row in a table `run.das` looks up -
`MODULE_AREAS`, a suite's or an area's file list - is not the surface. A stale copy sends its
reader to a flag that no longer does what the text says.

**On every platform, a cell that neither asserts nor registers a skip is a defect.** A cell that
returns without asserting registers `t |> skip` there; `feint` is a print, not a skip.

**A cell whose claim holds only where a box fact holds registers its skip on that fact before
the asserts that need it - never a failure.**

**A cell's skip condition, and any condition that picks a cell's assert or bar by something
other than an input the cell sets itself (a format, a shape, a loop value), queries a box fact
directly - a device query, a `typeinfo` module check, an environment read, a tune companion's
return, a toolchain probe, a stocked artifact's presence - and never tests a registry a
`register_*` call fills, or the presence or content of a file that the build, a mint or a test
run writes (a minted `.dlim`, a generated binary, a dump a test wrote).** Once the producer is
renamed, moved or removed, a condition on its product skips the cell on every box, and the suite
still passes.

**A cell that loads or gates on a stocked model file - a `.gguf` carrier, its shards, or an
mmproj - gates through `model_available` (`_model_tier.das`), one call per file; a test that
cannot require `_model_tier.das` open-codes the same two checks: the file and every sibling shard
are present, and their total size is under `LARGE_TIER_BYTES` unless `DASLLAMA_PARITY_FULL=1` is
set.** `DASLLAMA_PARITY_FULL=1` is a final pre-PR switch, not the iteration loop.

**A test - or a program a test builds or spawns - that is not about the `.dlim` model image runs
with `DASLLAMA_IMAGE=0` in its environment, or calls nothing that reaches a baking loader, in its
own code or through a helper.** A test is about the image
when it checks the image's mint, map or identity, including how a lane pin changes that identity (a
lane pin is a call that picks the weight form, the Metal tensor crowns - the pin that compiles a
family's half-precision GEMM twins - or the CPU kernel backend a load serves on). A baking loader
is any load call that writes a `.dlim` beside the model when `DASLLAMA_IMAGE` is unset, itself or
through a loader it calls. A test that sets no `DASLLAMA_IMAGE=0` loads its media carrier in
memory through the family's `stage_*` function paired with its `mint_*`, or through
`cache_via_image_staged` with an empty image path.

**A predicate whose value the BOX decides (a device capability, a policy default) and that
therefore cannot differ between two runs on one machine is never tested through its own
value; test it through the argv it gates or the mode it selects.**

**A test for an added, moved, or edited registration - the `register_*` call, its key, or the
lookup that reads it - reaches the registered thing through its registry, and never calls it
directly.** A registry is the storage a `register_*` call writes and a lookup reads at dispatch -
a table, a list, or a single hook global - or the `[EnvConfig]` env registry.

**A `corpus_case` call in `test_tokenizer.das` that does not assert BOTH the exact reference ids
and a lossless round-trip is a defect.**

**A test that compares tokens, ids or logits a model produced - a generation, a forced feed or a
forward pass - without logging both sides in the most readable form its fixture carries is a
defect: with a tokenizer, each side's decoded text for a token or id compare, one log record a
side with its newlines escaped (`log_gen_texts` in `_model_tier.das` writes that form), and each
side's argmax decoded piece plus the measured max difference for a logits compare; with a raw-id
fixture and no tokenizer, the ids, one log record a side.** A failure, or a pass that looks
wrong, must be readable in the log, not only as an id or float difference.

**A size, depth or row count that a cell's name, a comment on or inside the cell or a helper it
calls, or an assert's text claims, and that the cell does not pass as a literal argument straight
to the kernel or function under test, is asserted in that cell by an assert on a count only that
number can produce.** A value set through a knob setter or a config field (`set_mtp_depth`, any
`set_*` call) is not a literal argument, since the path the cell takes can clamp or ignore it. A
cap, a resize, or a counter showing a route ran is no such count, and a device's geometry
(subgroup width, SM count) is no coverage claim.

**An exact compare of a text model's tokens or ids over a prompt whose continuation can tie states
in the cell what makes its two sides compute bit-identical logits - one shared entry point, an
assert pinning the lane, or a lane pin (a call that picks the weight form, the Metal tensor crowns
or the CPU kernel backend a load serves on) plus why the one storage layout (a flat K/V cache
against a paged one) or batch shape that alone differs leaves the logits bit-identical; a compare no
such statement covers - different lanes, backends or kernel forms among them - is a defect, and
takes the forced-feed logits-tolerance form: the same fixed tokens fed to both sides, the logits
compared within a bar.** A continuation can tie unless every next token is dictated by the prompt's
own sequence (a count, an arithmetic series, a fixed enumeration); prose always can. An ASR
transcript compare answers to `REVIEW_MEDIA_CELLS.md`.

**A test in this folder that loads a stocked artifact whose producer none of the following names
is a defect.** Any one of these names the producer: a row in `../performance/model_specs.das`; a
row's `companions` list; a row in `asr_catalog` (`../performance/profile_common.das`); a convert
script in `../harness/`; for an oracle dump, the mint script stocked beside the dumps under
`models_dir()`, named by the test that loads the dump.

**A diff that adds a cell reading a stocked artifact from a model file's directory names that
artifact in the `companions` of that model file's row in `../performance/model_specs.das`, in the
same change.** A box that fetches the row alone then has what the cell reads.

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
after (the empty string where it was unset).** A set after that process starts is invisible to a
config already read.

**A cell that picks its bar, its route or its expected value by an environment-read knob its own
process has already read - a skip gate on the knob is no such pick - names that knob's value in
the text a red prints - the cell label or the assert.**

**A diff that adds or loosens (lets pass an input the old assert failed) a bound assert in a cell
that is not a kernel-unit cell, or moves one onto another route, lane or backend, ships in the
same change, in each such cell holding the assert, a control that lands outside that bound on that
route.** A bound nothing has exceeded where it is applied is not known to discriminate there.

**A diff that adds, in a cell that is not a kernel-unit cell, an assert on a counter showing a
route ran ships in the same change a control under which the route does not run and the counter
stays at its value from before the route - its own, or an existing one for the same counter in a
cell the same file runs.** A counter that rises on both legs shows nothing about the route.

**A control for a bound assert in a cell that is not a kernel-unit cell changes an input the
computation reads - a zeroed weight region, a poisoned input element, a mechanism disabled - and
re-runs the compare, or holds the output against the reference computed over displaced inputs (a
row or a layer off, a reordered sequence); a value added to the output after the fact is not one.**

**A within-bar float compare that a diff adds, changes, moves or extracts into a helper reds on a
NaN in the output, the reference or their difference: it passes only where its compare against the
bar is true (`d <= bar`, `s >= floor`), counts a miss where it is not, and reads a NaN as the worst
value in any extreme it keeps (a largest difference, a smallest similarity) - or it routes through
`_compares.das`' `check`, `check_count` or `sweep_check`, which do.** `d > bar` is false on a NaN,
and a running `max` or `min` drops a NaN that a later finite element follows, so an output holding
one, or a NaN sentinel the kernel never overwrote, passes.

**A must-exceed assert - one that passes only when a value lands outside its bar, by `d > bar` or
by a miss count above zero, as a control does - that a diff adds, changes, moves or extracts into
a helper asserts every value it reads finite first, or routes through `_compares.das`'
`check_exceeds`, which does.** A NaN read as infinite, or counted as a miss, passes it.

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
