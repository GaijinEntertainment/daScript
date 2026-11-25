# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE.md`, `../ARCHITECTURE_IMAGE_LEDGER.md`. Planned work:
`../followup_general.md`, `../followup_vulkan.md`, `../followup_metal.md`.

**A kernel-unit cell applies `REVIEW_KERNEL_CELLS.md` (beside this file) too, wherever the diff puts it.**

**A diff that touches a cell `REVIEW_PINNED_GATES.md` or `REVIEW_PINNED_KERNEL_GATES.md` (both
beside this file) pins, or any cell of a file either pins whole, or changes which `run.das` suites
list a file holding such a cell, applies `REVIEW_PINNED_GATES.md` too, whose rules bind the entries
of both files.**

**A diff that adds, moves, merges, renames or removes a test file or a `[test]`, or adds an optional
`require ?<mod>` to a file, or adds or changes a function in a file that has one, applies
`REVIEW_FILES.md` (beside this file) too - where a test lives, what the runner's file names keep,
what an optional require allows.**

**A diff that adds or changes an assert whose expected value restates or reads something that can
change while nothing in this repository changes, or whose expected value a checked-in table names as
its evidence, applies `REVIEW_PINNED_GATES.md` (beside this file) too.** A bar on a compare whose
two sides the cell computes is never such a value, whatever model file it was measured on.

**A cell that calls - in its own code or through a helper - a `../dasllama/` call that sets a
module or process global a later load, route choice or kernel dispatch reads (that property is
the trigger, whatever the name; `set_*`, `enable_*`, `disable_*`, `allow_*`, `pin_*`, `clear_*`,
`reset_*`, `restore_*`, `reinstall_*`, `register_*`, `unregister_*`, `select_*_override` and
`unset_*` are the common spellings), or that passes a loader parameter choosing a family's serving
lane (the weight form the family runs: q8 planes, a quantized carrier's own planes, or the f16 or
f32 planes of the exact lane - and whether a weight row stored as half floats serves as halves or
as floats), or whose claim depends on which route or serving lane runs it - a compare of a CPU run
against a GPU run among them - applies `REVIEW_LANE_PINS.md` (beside this file) too.**

**A cell that feeds, preprocesses, or asserts on media bytes an encoder consumes - pixels or
audio samples - compares an encoder's output rows against a second source, or compares ASR
transcripts, applies `REVIEW_MEDIA_CELLS.md` (beside this file) too.**

**A diff that adds or moves a cell that skips or picks its assert or bar on a condition, adds or
changes such a condition, or changes the body of a function in this folder that such a condition
calls, applies `REVIEW_SKIPS.md` (beside this file) too.**

**A diff that adds, moves or changes a cell that loads a model file, a `with_cpu_and_gpu_model`,
`with_model_once` or `with_model_once_at` call, an `own_load` call, a `family_on` call, or a TTS
form helper (`styletts2_form`, `pocket_form`, `styletts2_form_is`, `pocket_form_is`,
`_tts_parity.das`), or changes `REVIEW.das`'s `check_models_load_once`, `check_block_rows`,
`check_family_tags`, `FAMILY_FREE`, `LOAD_ONCE_AREAS` or `LOADERS`, applies `REVIEW_LOAD_ONCE.md`
(beside this file) too - the one block a model file loads on, the family a load is tagged with, and
the load a CPU-run cell takes.**

- A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest. It owns the asserts
  of every helper it calls and, where it runs inside the body of a `with_cpu_and_gpu_model`,
  `with_model_once` or `with_model_once_at` call, the load of the form it runs on - the form loader
  and stage function included - and each assert that body makes on that form outside a subtest.
- A test file is a `.das` dastest runs - one holding a `[test]` function, or one whose `cant_`,
  `failed_` or `invalid_` prefix makes its compile the assertion.
- A gate function is a named function a cell of a `test_*_kernels.das` file calls to compare one
  kernel against its CPU oracle.
- A file's `CLAUDE.md` entry is every `CLAUDE.md` clause stating a fact about the file (named with
  or without its `.das` suffix), one of its cells, or one of its gate functions, wherever in
  `CLAUDE.md` the clause sits; a brace list naming several files at once
  (`test_metal_{prefill,decode}_kernels`) or a suite roster states none.
- A compare's two sides are the two computations whose mismatch reds the assert; a reference that
  only decides where the compare stops is neither side.
- A bound assert is a compare of two computed sides, or a measured quality figure against a
  tolerance; an exact compare - equality of ids, an argmax or an integer count, with no tolerance in
  either side's computation - is not one; a float compare, bit-exact or not, and a miss count of a
  tolerant compare are. Nor is a floor on a capacity or a configuration, a compare of entry counts
  or roster sizes, an index guard, a route counter, or a timing or rate bound.
- A stocked artifact is a file a test reads from under `models_dir()` that neither this repo's build
  nor its tests produce - model files, mmprojs, front-end packs, image fixtures, oracle dumps.
- An area is a name in `run.das`'s `AREAS` list, standing for the files `run.das`'s `area_tests`
  returns for it.
- A box fact is a condition one box or run mode meets and another does not: the device or its
  memory, whether the build carries a module (`typeinfo module_exists` or `typeinfo
  builtin_module_exists`), the value of an environment variable or runner flag the run reads, the
  value a tune companion returns (a function whose body is the winner the `[tune]` framework
  picked for this box), the CPU kernel backend the box's CPU features pick
  (`active_kernel_backend`), a host toolchain, or a stocked artifact's presence.
- A change reaches a test file or a cell when it alters anything its result depends on - its test
  file, a shared helper, engine code it exercises, an in-tree fixture or corpus it reads, or a name
  it asserts on; an edit that changes only whitespace or comments, or that moves code with its text
  unchanged and adds or drops only the `require` lines the move needs, reaches none.

These definitions bind the `REVIEW_*.md` companions beside this file wherever a companion does not
define the term itself. A kernel-unit cell, a kernel class and a CPU oracle are as
`REVIEW_KERNEL_CELLS.md` (beside this file) defines them; an encoder is as `REVIEW_MEDIA_CELLS.md` defines it.

**A test file in this folder whose cells cannot hold under `DASLLAMA_CPU_PREFILL=1` says so in its
header (its top comment block) and joins the list of files that sit in no suite (`exempt` in
`test_run_suites.das`) in the same change.** `DASLLAMA_CPU_PREFILL=1` is what the runner arms for every suite.

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

**A test RUN outside the `FILES` list of a per-PR lane in `../CMakeLists.txt` runs under `-jit` -
never the interpreter, never AOT.** Under the interpreter a
model-gated suite's cells skip, and a run of skips is not the coverage the suite owes.

**A diff that adds a test file here adds its `CLAUDE.md` entry in the same change, every clause
of it true of the file as added.**

**A diff that adds a cell or a gate function to a file, or a function to a `_*.das` fixture module
in this folder, whose `CLAUDE.md` entry names every one of that kind the file held before the diff,
whether or not the entry claims to, states how many there are, or states a fact of every one of them
("every", "each", "all"), adds the new one to each such list, corrects each such count, and corrects
each such clause the new one makes false, in the same change, every clause it adds or corrects true
of the file as the diff leaves it.**

**A diff that removes, moves or renames a cell or a gate function, or changes a cell's suite, the
model-size tier it runs under (the files it loads total at or below `MODEL_TIER_CEILING_BYTES`, or
above it), its `--category`, whether it loads a layer cut (a model file holding a subset of a
carrier's blocks), its skip condition, or any fact a `CLAUDE.md` entry states about it - the class
it dispatches, the shapes, lengths, formats or lanes it sweeps, the predicate an assert reads, the
bar it holds, its controls - corrects in the same change the `CLAUDE.md` entry of every test file
running the cell, counts, skip clauses and tier, category and cut wording included.**

**A diff that adds a function to a `_*.das` fixture module in this folder, removes or renames one,
changes what one does, or removes its last caller makes every `CLAUDE.md` clause stating a fact
about the function true of it as the diff leaves it, in the same change.**

**A diff that adds, removes or changes a cell that a `CLAUDE.md` clause describes as one of a
class - the cells of one helper, or the arms of one name pattern - keeps that clause's counts and
skip clauses true for every cell in the class.**

**A diff that adds, renames, or drops an arm name - the literal passed to `arm_on(t, name)`
(`_model_tier.das`), what `--arm` matches - updates its suite's backticked arm roster in
`CLAUDE.md`'s "Arm filter mechanics" section in the same change.** An arm the roster does not name
is unreachable to whoever is choosing what to run.

**A diff that adds, changes, or drops a cell's skip condition other than the runner's own
`--arm` / `--family` / `--category` filter - a `t |> skip` or an early return - updates in the same change the
header of every test file that runs the cell, wherever the cell is defined, so that the header
names every fact the cells that file runs skip on.**

**A diff that changes `run.das`'s surface - a flag, a suite name, an area name, what a flag does,
which suite or area a run with no flag runs, or an environment knob the runner reads - adds it to
or corrects it in `CLAUDE.md`'s "Run suites ONLY through the runner" block and `../CLAUDE.md` in the
same change.** A data row in a table `run.das` looks up - `MODULE_AREAS`, a
suite's or an area's file list or arm list (`area_image_arms`) - is not the surface. A stale copy
sends its reader to a flag that no longer does what the text says.

**On every platform, a cell that neither asserts nor registers a skip is a defect.** A cell that
returns without asserting registers `t |> skip` there; `feint` is a print, not a skip.

**A cell whose claim holds only where a box fact holds registers its skip on that fact before
the asserts that need it - never a failure.**

**A cell that loads or gates on a stocked model file - a `.gguf` carrier, its shards, or an mmproj -
gates through `model_available` (`_model_tier.das`), one call per file; a test that cannot require
`_model_tier.das` open-codes its checks: the file and every sibling shard are present, their total
size is under `MODEL_TIER_CEILING_BYTES` (`../dasllama/dasllama_gguf.das`) unless
`DASLLAMA_PARITY_FULL=1` is set or the run names the file's category, and neither the run's
category filter nor its family filter skips the file (`category_skip_reason`,
`family_skip_reason`, `_model_categories.das`).** `DASLLAMA_PARITY_FULL=1` belongs to a cited run - an arc's final
PR's, or one an intermediate PR's scope report names - never to the iteration loop.

**A cell - or a program a cell builds or spawns - that is not about the `.dlim` model image calls
nothing that reaches a baking loader, in its own code or through a helper.** A cell is about the
image when it checks the image's mint, map or identity, including how a lane pin changes that
identity (a lane pin picks the weight form, the Metal tensor crowns - the pin that compiles a
family's half-precision GEMM twins - or the CPU kernel backend a load serves on). A baking loader is
any load call that writes a `.dlim` beside the model when `DASLLAMA_IMAGE` is unset, itself or
through a loader it calls. Such a cell loads a GGUF through a load that bakes nothing (`load_model_`
is one), and a media carrier in memory through the family's `stage_*` paired with its `mint_*`, or
`cache_via_image_staged` with an empty image path; the cells that break this by design are ledgered
at `../ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`.

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

**A size, depth or row count - other than a block or layer count of the carrier the cell loads -
that a cell's name (its `t |> run` label), a comment on or inside the cell or a helper it calls, or
an assert's text claims, and that the cell does not pass as a literal argument straight to the
kernel or function under test, is asserted in that cell by an assert on a count only that number can
produce; one that a `[test]`'s description claims is asserted so by at least one cell that test
runs.** A value set through a knob setter or a config field (`set_mtp_depth`, any `set_*` call) is
not a literal argument, since the path the cell takes can clamp or ignore it. A cap, a resize, or a
counter showing a route ran is no such count, and a device's geometry (subgroup width, SM count) is
no coverage claim.

**A block or layer count that a cell's name, a `[test]`'s description, a comment on or inside the
cell or a helper it calls, an assert's text, or a carrier's file name states of the carrier a cell
loads is asserted on the loaded carrier's own field as loaded or on a count the run produced.**

**An exact compare of a text model's tokens or ids over a prompt whose continuation can tie states
in the cell what makes its two sides compute bit-identical logits - one shared entry point, an
assert pinning the lane, or a lane pin (a call that picks the weight form, the Metal tensor crowns
or the CPU kernel backend a load serves on) plus why the one storage layout (a flat K/V cache
against a paged one) or batch shape that alone differs leaves the logits bit-identical; a compare no
such statement covers - different lanes, backends or kernel forms among them - is a defect, and
takes the forced-feed logits-tolerance form: the same fixed tokens fed to both sides, the logits
compared within a bar.** A continuation can tie unless every next token is dictated by the prompt's
own text (a count, an arithmetic series, a fixed enumeration spelled out in it); prose always can.
An ASR transcript compare answers to `REVIEW_MEDIA_CELLS.md`.

**A test in this folder that loads a stocked artifact whose producer none of the following names
is a defect.** Any one of these names the producer: a row in `../performance/model_specs.das`; a
row's `companions` list; a row in `asr_catalog` (`../performance/profile_common.das`); a convert
script in `../harness/`; for an oracle dump, the mint script stocked beside the dumps under
`models_dir()`, named by the test that loads the dump.

**A diff that adds a cell reading a stocked artifact from a model file's directory names that
artifact in the `companions` of that model file's row in `../performance/model_specs.das`, in the
same change - with no `url`, `bytes` or `sha256` where another row's entry carries them.** The row
then names everything the cell reads; an artifact's url and checksum sit in one row.

**A test that reads a vision encode oracle dump without naming the minting arm in its header - the
backend, the flash-attention setting and the mmproj precision the dump came from - is a defect.**

**A diff that weakens `test_large_rows_have_a_category` (`test_model_specs.das`) or
`test_large_stocked_models_have_a_category` (`test_run_suites.das`), which hold every model file
past `MODEL_TIER_CEILING_BYTES` to a category, is a defect.**

**A diff that adds or moves a batched-vs-sequential parity cell - one comparing the batched
stack against a per-session sequential forward whose sequential side runs on the CPU - onto a
carrier above `MODEL_TIER_CEILING_BYTES` (`../dasllama/dasllama_gguf.das`) is a defect.** The batched code paths get
their parity on small models, through pins; a cell whose two sides both run on the device
streams nothing beside the device's bytes, and the tier does not bind it.

**Only a cell whose claim is about a fresh read of an environment-read knob - one the running config
reads once, at context init - (a config it builds itself with `env_config`) sets it in its own
process, reads that fresh config and restores the previous value right after. Any other cell that
needs another value passes it to a child it spawns, either as the spawn's environment argument or,
for a spawn that inherits the parent's environment, by setting the knob right before the spawn and
restoring the previous value right after. Restoring a knob that was unset sets it to the empty
string.** A set after that process starts is invisible to a config already read.

**A cell that picks its bar, route or expected value (an expected counter among them) by an
environment-read knob, or by the GPU home the run armed (`DASLLAMA_GPU`, `DASLLAMA_CUDA`), names
each such knob and its value in the cell label or the assert text; a skip gate on a knob is not such
a pick.**

**A diff that adds or loosens (lets pass an input the old assert failed) a bound assert in a cell
that is not a kernel-unit cell, or moves one onto another route, lane or backend, ships in the
same change, in each such cell holding the assert, a control that lands outside that bound on that
route.** A bound nothing has exceeded where it is applied is not known to discriminate there.

**A diff that adds, in a cell that is not a kernel-unit cell, an assert on a counter showing a
route ran ships in the same change a control under which the route does not run and the counter
stays at its value from before the route - its own, or one in a cell the same file runs that reads
the assert's engine counter on every build.** A counter that moves on both legs shows nothing.

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
a helper fails on a NaN in any value it reads: it asserts each value finite first, counts a NaN as
inside the bar, or routes through `_compares.das`' `check_exceeds`.** A NaN read as infinite, or
counted as a miss, passes it.

**A poison control on a tower a GPU driver serves - a run of the gate with a block's weights
zeroed, which must fail - zeroes that block's GEMM weights in every plane the served route reads
them from.** A poison the served route never reads passes on a broken kernel.
