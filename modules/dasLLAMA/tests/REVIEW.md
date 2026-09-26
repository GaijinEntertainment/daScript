# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`, `../followup_metal.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's
asserts belong to every cell that calls it.

**A cell, probe, or harness that dispatches or binds a kernel class - a `[metal_dispatch]` or
`[vk_dispatch]` class, or a CPU kernel in `../dasllama/dasllama_math*.das` - applies
`REVIEW_KERNEL_CELLS.md` (beside this file) together with this list, wherever the diff puts the
file.**

**A diff that touches a cell in the pinned set, or adds a cell or an assert whose expected value
must be kept in step with something maintained outside the cell (a document, a checked-in table, a
committed artifact's form, a roster, a knob list) or that a checked-in table names as its
evidence, applies `REVIEW_PINNED_GATES.md` (beside this file) together with this list.**

**Weakening `test_metal_float_a_gate.das` - the gate that checks the MSL emitter refuses a float A operand (the activation input) to a `tmm2d_*` tiled matrix-multiply call without the `[metal_kernel(float_a_ok=true)]` license - is a defect.**

**Every PR runs `run.das -- --suite model-free` and `run.das -- --suite stocked` on a box with
the models stocked, plus every test here the change reaches - never the whole directory.** A
change reaches a test when it alters anything the test's result depends on - the test file, a
shared helper, engine code it exercises, an in-tree fixture or corpus it reads, or a name it
asserts on; a comment-only edit reaches none.

**A PR that adds or changes a cell loading a model above the large tier (`LARGE_TIER_BYTES`,
`_model_tier.das`) also runs that cell with `DASLLAMA_PARITY_FULL=1` set, on a box with the model
stocked, through a `run.das` suite listing the cell's file - with `--arm` naming the cell when
`run.das` accepts `--arm` for that suite (every suite but `model-free` and `stocked`) - and names
the box in the PR body.** A run without `DASLLAMA_PARITY_FULL=1` skips every such cell and passes.

**The `stocked` run every PR owes carries no `--exclude`** - an excluding run is the iteration form
between PRs; a PR that ships on it never ran the coverage it dropped.

**A test file - a `.das` in this folder that dastest runs: one carrying at least one `[test]`
function, or one whose `cant_`, `failed_` or `invalid_` prefix makes its compile the assertion -
whose cells cannot hold under `DASLLAMA_CPU_PREFILL=1` says so in its header - the file's top
comment block - and joins the exempt list of `test_run_suites.das`'s suite-membership gate in
the same change; weakening that gate is a defect.** `DASLLAMA_CPU_PREFILL=1` is what the runner
arms for every suite.

**`run.das` declares no global whose initializer spawns, logs, writes the environment or
touches the filesystem, and weakening `test_run_suites.das`'s no-`[init]` check is a defect.**
`test_run_suites.das` and `test_run_summary.das` require `run` by bare same-dir name, so anything
that fires on require fires inside every one of those test processes.

**A diff that gives `_model_tier.das`, or a `tests/` fixture it requires, an `[init]` that
declares the CPU-prefill intent (`allow_cpu_prefill`) is a defect - the intent is declared in
the `[init]` of the test file that needs it.** `test_cpu_prefill_tripwire.das` requires
`_model_tier` and asserts that the guard trips while the intent is undeclared.

**A cell asserting a chat template's INSTRUCT wire - a closed empty thought block and no
thinking gate - calls `set_thinking(c, false)` on its `ChatSession` before the first turn.**
`ChatTemplate.think_default` is `true` unless a family clears it, so an un-opted-out turn 1
renders the thinking gate and the cell asserts the wrong wire.

**Every test RUN runs under `-jit` - never the interpreter, never AOT.** A compile-only CI lane
passes dastest's `--compile-only`. Under the interpreter a model-gated suite's cells skip, and
a run of skips is not the coverage the suite owes.

**A diff that registers a test file in this folder in a `CMakeLists.txt` is a defect - a
`run.das` suite listing is the only registration these files get.**

**A diff that adds a test file here, or adds, removes, moves or renames a cell, or changes its
suite, its skip condition, or what it claims - a shape, a length, a format or a lane the cell
sweeps, or a tolerance it holds - corrects or adds, in the same change, the `CLAUDE.md` entry of
every test file running the cell, counts and skip clauses included.** A file's entry is the
clause that describes the file, named with or without its `.das` suffix; a `{a,b}` shorthand or
a suite roster needs no update.

**A diff that adds, renames, or drops an arm name - the literal passed to `arm_on(t, name)`
(`_model_tier.das`), what `--arm` matches - updates the arm census in `CLAUDE.md`'s "Arm filter
mechanics" section in the same change.** An arm the census does not name is unreachable to
whoever is choosing what to run.

**A diff that adds, changes, or drops a cell's skip condition other than the runner's own
`--arm` / `--family` filter - a `t |> skip` or an early return - updates in the same change the
header - the file's top comment block - of every test file that runs the cell, wherever the cell
is defined, so that the header names every fact the cells that file runs skip on.**

**A diff that adds, moves, or removes a test file outside this folder that carries a
`require dasllama/...` line of its own adds, corrects, or drops its row, with the reason it
lives there, in `CLAUDE.md`'s "Out-of-folder test files" ledger in the same change.** A file
reaching an engine module through another module's public require is not a row.

**A diff that changes `run.das`'s flag surface - a flag, a suite name, an area name, or what a
flag does - adds it to or corrects it in `CLAUDE.md`'s "Run suites ONLY through the runner"
block and `../CLAUDE.md` in the same change.** A data row in a table `run.das` looks up -
`MODULE_AREAS`, a suite's or an area's file list - is not the surface. Both documents restate
the surface for an agent that reads them cold; a copy the code has left behind sends that agent
to a flag that no longer does what the text says.

**On every platform, a cell that neither asserts nor registers a skip is a defect.** A cell that
returns without asserting - whatever the reason - registers `t |> skip` there, and one whose
claim needs a capability the box may lack (a device, a window server, an audio device, a module
the build omits, a stocked model) registers that skip on the fact before it asserts, never a
bare return and never a red; `feint` is a print, not a skip.

**A cell's skip condition keys on a fact the box owns - a device capability, a run-mode knob's
value, a host toolchain's presence, a compile-time module-presence check
(`typeinfo builtin_module_exists`) - or on a stocked fixture beside the models (a model
file, an mmproj, an oracle dump); never on the existence of an artifact
this repo's build or a previous test run produced (a minted `.dlim`, a generated binary, a
dump a test wrote).** An artifact condition goes permanently false when its producer moves.

**A test that loads a model above the large tier (`LARGE_TIER_BYTES`, `_model_tier.das`)
without gating on `DASLLAMA_PARITY_FULL=1` is a defect** - `DASLLAMA_PARITY_FULL=1` is a final
pre-PR switch, not the iteration loop.

**A cell that gates on a stocked model file - a `.gguf` carrier, its shards, or an mmproj - gates
through `model_available` (`_model_tier.das`), one call per file; a test that cannot require
`_model_tier.das` open-codes the same two checks: the file and every sibling shard are present,
and their total size is under `LARGE_TIER_BYTES` unless `DASLLAMA_PARITY_FULL=1` is set.** Every
other stocked fixture gates on its own presence.

**A test - or a program a test builds or spawns - whose subject is not the `.dlim` image rail (a
cell whose subject is a lane knob's effect on the image identity has the rail as its subject)
never mints or maps a MODEL image: it either runs with `DASLLAMA_IMAGE=0` in its environment, or
calls no loader that, with `DASLLAMA_IMAGE` unset, writes a `.dlim` beside the model - `load_model`,
`load_model_cached`, `load_model_image`, `load_<family>_tower`, `load_<family>_encoder`,
`load_<family>_embedder`, `load_<carrier>_model`, `load_asr_model`, `load_whisper_model`,
`load_vision_embedder`, `load_audio_embedder`, `load_tts_model`, `load_styletts2` (`load_pocket`, and
`load_tts_model` on a Pocket file, bake nothing), a diff adding such a loader adding it here; such a
test loads a media carrier in memory from the family's `stage_*` staging - its `mint_*` twin, or
`cache_via_image_staged` with an empty image path.** A disk bake under a lane pin (a
`set_<family>_q8`-class knob or a Metal tensor-crowns pin) purges the serving lane's `.dlim` beside
the model, and the next direct-image load in another suite panics on the wrong identity.

**A predicate whose value the BOX decides (a device capability, a policy default) and that
therefore cannot differ between two runs on one machine is never tested through its own
value; test it through the argv it gates or the mode it selects.**

**A test for an added, moved, or edited registration reaches the registered thing through its
registry, and never calls it directly.** A registry is the storage a `register_*` call writes
and a lookup reads at dispatch - a table, a list, or a single hook global - or the `[EnvConfig]`
env registry.

**A new pre-tokenizer family, or a new tokenizer backend (byte-level BPE or SPM), ships its
`corpus_case` arm in `test_tokenizer.das`, naming the `ggml-vocab-*.gguf` fixture.**

**A `corpus_case` arm that does not assert BOTH the exact reference ids and a lossless
round-trip is a defect.**

**A test that compares generated tokens, ids, or logits without logging both sides in the most
readable form its fixture carries is a defect: with a tokenizer, the decoded text for a token or
id compare (`log_gen_texts` in `_model_tier.das`, or one line per side) and each side's argmax
decoded piece plus the measured max difference for a logits compare; with a raw-id fixture and no
tokenizer, the ids, one line per side.** A red, or a suspicious green, must be readable in the
log, not only as an id or float difference.

**A size, depth, or row count that a cell's name, a comment inside the cell, or an assert's text
claims about what the cell exercises is asserted in that cell - by an assert on the count, or, for
a count the cell passes as a literal argument to the kernel it dispatches, by comparing every
output element that count covers against the oracle over an output filled with a sentinel before
the dispatch.** A count the dispatch pads, clamps or derives from that literal gets its own assert.
A cap, a resize, or a counter showing the path ran is not evidence the number was reached; a
device's geometry (subgroup width, SM count) is no coverage claim.

**A freeform token-parity cell - a token-exact compare over a text-generation prompt whose
continuation can tie - stays token-exact only when its two sides run one code path, and states in
the cell what makes them one (the shared entry point, or an assert pinning the lane); a freeform
cell whose sides can round differently (different lanes, backends, batch shapes or kernel forms)
uses the forced-feed logits-tolerance form - the same fixed tokens fed to both sides, logits
compared within a bar.** A counting cell - one whose prompt forces a continuation that cannot tie,
so greedy tokens are fixed - stays token-exact.

**An ASR family with no token-for-token oracle cell is a defect** - the cell compares a
transcript against a reference leg, external dump or CPU control alike.

**A test in this folder that loads a stocked artifact whose producer the test cannot name is a
defect.** A stocked artifact is anything the test reads out of `models_dir()` that this repo's
build does not produce - model files, mmprojs, front-end packs, image fixtures, oracle dumps.
Any one of these names the producer: a row in `../performance/model_specs.das`; a row's
`companions` list; a row in `asr_catalog` (`../performance/profile_common.das`); a convert
script beside `../performance/model_specs.das`; for an oracle dump, the mint script stocked
beside the dumps under `models_dir()`, named by the test that loads the dump.

**A test that reads a vision encode oracle dump without naming the minting arm in its header -
the backend, the flash-attention setting, and the mmproj precision the dump came from - is a
defect.**

**A cell, or the `[init]` of the file where the cell is defined, sets every driver setter - a
`set_*` / `pin_*` call in `dasllama/` that changes the driver's route, the serving lane or the
engage mode for the rest of the process - whose value the cell's claim depends on, even when
the claim needs it at its DEFAULT value; a family serving lane is pinned by the cell itself, never
by the file's `[init]`, through a lane setter - a call whose value the family's loader reads to
pick its lane, or a facade call that makes that call (`set_<family>_q8`; `set_styletts2_q8` for
kitten and kokoro; `set_tts_q8`, which pins every TTS family; `set_canary_enc_q8`; whisper's
`set_asr_fp32` / `set_asr_tower_fp32`) - or through a loader parameter that takes the lane, and a
claim that needs the lane unset establishes it with the unset call paired with the setter it pins
through - `reset_<name>_q8` for a `set_<name>_q8` (`reset_tts_q8` for the TTS facade), whisper's
`set_asr_fp32(false)` and `set_asr_tower_fp32(false)`.** A cell that counts on the box declining
the other lane, instead of pinning, measures whichever lane the box's policy picked.

**A cell that sets a family pin or a driver setter - directly, through a helper it calls, or
through a loader parameter that takes the lane - returns with every pin it set unset through its
paired unset call, and every setter it set back where it found it; where the driver exposes no
getter, back at the value the file's `[init]` sets, else at its default.** A pin left set makes
the next cell measure this cell's lane, not its own.

**A cell asserting the unpinned default lane compares against the predicates the family's
`*_serves_q8` accessor reads for its unpinned default (whatever its body calls), never against a
hardcoded lane.** The default lane differs per box.

**An image-suite cell whose subject IS the lane knob loads through the `.dlim`-baking loader,
never around it.** The pin is part of what the image identity records.

**A CPU-vs-GPU cell on Metal serving an LLM `Model` through a decode or prefill override - the
Metal driver hook that runs the model's decode or prefill stages on the GPU - one comparing the
two routes' outputs, not one whose subject is the GPU route's decline, runs its CPU stages on a
PLANAR model (the non-blob form, the only one CPU inference reads) and the stages the override
selects on that model's blob twin (`blob_twin(t, path, seq_cap)`, `_metal_blob_twin.das`), in one
session.** The planar model and its blob twin share one shape, so one session serves both.

**A diff that adds a model-loading block to a file of a `run.das` suite that accepts `--arm` -
every suite but `model-free` and `stocked` - tags it with the carrier family it loads: one `family_on(t, name)` check
per carrier family (`_model_tier.das`), the token that family's entry in `CLAUDE.md`'s family filter list (a family with
no entry adds one in the same change), the checks in sequence so a block loading several families runs only under a
filter naming every one of them.** Kitten and kokoro are two families of one architecture; an untagged block runs under every `--family` filter.

**A diff that adds or moves a batched-vs-sequential parity cell - one comparing the batched
stack against a per-session sequential forward - onto a carrier above `LARGE_TIER_BYTES`
(`_model_tier.das`) is a defect.** The batched code paths get their parity on small models,
through pins.

**A cell sets an environment-read knob - one the running config reads once, at context init -
before the process that reads it starts: the child the cell spawns, or the runner's own.** A set
after that process starts is invisible to a config already read.

**A cell that cannot set an environment-read knob before its reader starts names that knob's
value in the text a red prints - the cell label or the assert.**

**A cell that runs with no model loaded and encodes, preprocesses, or asserts on media bytes
an encoder consumes - pixels or audio samples, not a `.dlim` model image - builds its fixture
procedurally and pins its expectations in-repo.**

**An image a test feeds an embedder that the test does not build, and that
`DASLLAMA_VISION_DUMP` cannot preview, is a defect** - a red never requires adding
instrumentation before a human can see what the model consumed.

**An audio clip a test feeds an embedder that the test does not build, that the repository does
not track, and that is not one of the clips stocked beside the models (`jfk.wav`,
`gemma4a_test2.wav`), is a defect** - a clip nobody else can play makes a red unreadable. A newly
stocked clip joins this list in the same change.

**A media fixture an embedder-parity cell regenerates in-test and compares against an oracle
dump, with no exact-value generator - one whose values are exactly representable floats, so
every box produces the same bytes - is a defect.** A generator running libm transcendentals is
not exact-value: it is not float-portable.

**An embedder-parity cell - one comparing an encoder's output rows against a second source, another
encode chain (the CPU exact, the CPU q8 or the device chain) or an oracle dump - that does not
name its fixture in its label or a logged line, or does not log the measured maxdiff on green as
well as red, is a defect.**

**A diff that adds or loosens an assert holding a figure the run measures - the difference
between two computed sides, a rate, an error, or a count the run decides - within a nonzero
tolerance, or past a floor or ceiling, ships in the same change a control that lands outside that
bound in every cell that holds it; a control changes an input the computation reads (a zeroed
weight region, a poisoned input element, a mechanism disabled) and re-runs the compare, so a value
added to the output after the fact is not one.** A bound nothing has exceeded where it is applied
is not known to discriminate there.

**A family that gains a live thinking or tool format ships its recognition tests in the same
change** - the wire-shape pins, the render pins, and a live server case gated on the family's
smallest GGUF that sits under `LARGE_TIER_BYTES` (`_model_tier.das`).

**A poison control on a tower the Metal driver serves - a run of the gate with the tower's
weights zeroed, which must RED - zeroes every weight buffer the served route reads.** Which
buffers those are depends on the route: a twin-W route reads only the baked halfword copy of
the GEMM weights (`wblob`), so zeroing that buffer alone is a valid control there; a route
that also reads the f32 plane (`fblob`) needs both zeroed. A poison the served route never
reads passes on a broken kernel.

**An ASR cell comparing transcripts across two serving lanes - a lane is a weight format the
family serves, one chain (CPU or device) over one format, or one kernel form of one format -
asserts WORD equality when the pair is a crowned kernel form and its tensor twin; TOKEN equality
when the two sides differ only in the decoder (one encoder's rows feed both); and otherwise TOKEN
equality, or the transcripts' text equal with the encoder output rows held either within a
tolerance of the other side's or by the twin bar - the device chain's distance from the exact
chain within a stated multiple of the CPU q8 chain's own.** A crowned kernel form is the one the tuner measured fastest and
armed as the serving one; its tensor twin is the same kernel written on Metal's tensor primitives,
and the twins' rounding legitimately flips tokens.

**An ASR transcript cell that cannot assert the equality its comparison calls for converts to
a forced-feed logits compare within a tolerance bar, or to equal transcript text with the encoder
output rows held within a tolerance of the other side's or by the twin bar - never to a text
compare alone.**

**A function in a file of this folder that requires a module behind an optional `require ?<mod>`
never names that module's types in its signature - leave a parameter that would carry one
untyped, and drop a return type that would name one.** A signature cannot sit inside a
`static_if`, so a build without the module fails the compile on it.

**A function in a file of this folder that requires a module behind an optional `require ?<mod>`,
and that has no untyped parameter, names anything that module declares only inside a
`static_if (typeinfo builtin_module_exists(<mod>))` body.** In a build without the module a
fully typed function is inferred anyway; one with an untyped parameter is inferred only at a call
site, which its caller has already guarded.
