# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

**A kernel-unit cell - a model-less cell that dispatches one kernel class and asserts on its
output - or a gate that hand-dispatches or hand-binds a kernel, wherever the diff puts it, and
a diff that changes a `[metal_dispatch]` or `[vk_dispatch]` class's dispatch geometry, its
kernel-argument struct (`kargs`), that struct's fields, or which branch the class takes on a
value in a buffer it binds, apply `REVIEW_KERNEL_CELLS.md` (beside this file) together with
this list.**

**Every PR runs `run.das -- --suite model-free` and `run.das -- --suite stocked` on a box with
the models stocked, plus every test here the change reaches - never the whole directory.** A
change reaches a test when it alters anything the test's result depends on - the test file, a
shared helper, engine code it exercises, an in-tree fixture or corpus it reads, or a name it
asserts on; a comment-only edit reaches none.

**A PR's `stocked` run carries no `--exclude`** - `--suite stocked --exclude test_ple_modes` is
the iteration form between PRs; a PR that ships on it never ran the PLE coverage.

**A test file - a `.das` in this folder that dastest runs: one carrying at least one `[test]`
function, or one whose `cant_`, `failed_` or `invalid_` prefix makes its compile the
assertion - whose cells cannot hold under `DASLLAMA_CPU_PREFILL=1` says so in its header and
joins the exempt list of `test_run_suites.das`'s suite-membership gate in the same change;
weakening that gate is a defect.** `DASLLAMA_CPU_PREFILL=1` is what the runner arms for every
suite.

**Invoking dastest directly on a test file in a `run.das` suite other than `model-free` and
`stocked` is a defect - run it through `run.das`.**

**`run.das` declares no global whose initializer spawns, logs, writes the environment or
touches the filesystem; a diff that adds one is a defect, and weakening `test_run_suites.das`'s
no-`[init]` check is a defect.** `test_run_suites.das` and `test_run_summary.das` require `run`
by bare same-dir name, so anything that fires on require fires inside every one of those test
processes.

**A cell asserting a chat template's INSTRUCT wire - a closed empty thought block and no
thinking gate - calls `set_thinking(c, false)` on its `ChatSession` before the first turn.**
`ChatTemplate.think_default` is `true` unless a family clears it, so an un-opted-out turn 1
renders the thinking gate and the cell asserts the wrong wire.

**Every test RUN runs under `-jit` - never the interpreter, never AOT.** A compile-only CI lane
passes dastest's `--compile-only`. Under the interpreter a model-gated suite's cells skip, and
a run of skips is not the coverage the suite owes.

**A diff that registers a test file in this folder in a `CMakeLists.txt` is a defect - a
`run.das` suite listing is the only registration these files get.**

**A diff that changes what a file covers - a cell added, removed or moved, its suite, an axis or
bar an existing cell asserts - corrects that file's `CLAUDE.md` census entry, numbers included,
in the same change.** A `{a,b}` shorthand naming several files at once, or a suite roster,
carries nothing to correct.

**A diff that changes the contract a gate pins - what its asserts hold fixed, an axis gained or
lost - updates that gate's entry in this checklist's pinned set in the same change.**

**A diff that adds, changes, or drops a gate's skip condition - the model, fixture, device or
arm it needs - updates the test file's own header - the top comment block carrying every fact a
`t |> skip` in the file keys on (model, tier, device, arm, knob) - in the same change, and adds
or corrects the skip clause in that file's `CLAUDE.md` entry where `CLAUDE.md` carries one.**

**A diff that adds, moves, or removes a `[test]` file outside `modules/dasLLAMA/` that carries
a `require dasllama/...` line of its own adds, corrects, or drops its row, with the reason it
lives there, in `CLAUDE.md`'s "Out-of-folder test files" ledger in the same change.** A file
reaching an engine module through another module's public require is not a row.

**A diff that changes `run.das`'s flag surface - a flag, a suite name, an area name, or what a
flag does - adds it to or corrects it in `CLAUDE.md`'s "Run suites ONLY through the runner"
block and `../CLAUDE.md` in the same change.** A data row in a table `run.das` looks up -
`MODULE_AREAS`, a suite's or an area's file list - is not the surface. Both documents restate
the surface for an agent that reads them cold; a copy the code has left behind sends that agent
to a flag that no longer does what the text says.

**A diff that adds, renames, or drops an arm name - the literal passed to `arm_on(t, name)`
(`_model_tier.das`), what `--arm` matches - updates the arm census in `CLAUDE.md`'s "Arm
filter mechanics" section in the same change** - an arm the census does not name is
unreachable to whoever is choosing what to run.

**A pinned gate's coverage never shrinks - not its asserts, not its bounds, not the corpus or
sweep it covers, and not the set of runs that reach it; a diff that shrinks one is a defect.**
A pinned file that reaches a fixture root sits in `stocked`, where the per-PR run reaches it;
that is not a shrink. The pinned set, each with what it pins:
`test_run_suites.das` (the per-PR split, the folder census, the area tables, the `--exclude`
filter); `test_program_roots.das` (the `ROOT_DIRS` sweep, `options stack = 524288`, prefill intent);
`test_env_registry.das` (the `../ENVIRONMENT.md` knob contract); `test_model_specs.das`
(`../performance/model_specs.das`'s model-set table); `test_metal_prefill_kernels.das`'s
softcap, sink (`hass`) and span cells; `test_site_records.das` (the byte-compare of
`site/files/dasllama/bench_records.json` (repo root) against a fresh `merge_site_records`
run); `test_exchange_schema.das` (the exchange validator's corpus sweeps, and the `[tune_scope]`
wire-key pin read out of `../dasllama/dasllama_tune_scope.das`) and
`test_bench_records_schema.das` (the `write_bench_records` output, corpus sweeps included);
`test_scheduler.das`'s media-stream bypass check (no cached hit at `prefix_attach`, no donated
pages at `donate_stream`); `test_vulkan_kernels.das`'s tile-pick cell (which tile the Vulkan
matmul picks for a given width, row count and coopmat mode, and whether that dispatch splits its
reduction across partial planes); `utils/dasllama-server/test_worker_dispatch.das` (repo root) -
worker-local fork pools, shared queue policy.

**A diff that adds a gate whose failure means a documented contract changed, rather than a
kernel regressing, adds it to the pinned set in the same change** - as a file when every
cell of it pins, as a named cell otherwise.

**On every platform, a cell that neither asserts nor registers a skip is a defect.** A cell that
returns without asserting - whatever the reason - registers `t |> skip` there, and one whose
claim needs a capability the box may lack (a device, a window server, an audio device, a module
the build omits, a stocked model) registers that skip on the fact before it asserts, never a
bare return and never a red; `feint` is a print, not a skip.

**A cell's skip condition keys on a fact the box owns - a device capability, a run-mode knob's
value, a host toolchain's presence, a compile-time module-presence check
(`typeinfo builtin_module_exists`) - or on a stocked fixture beside the models (a model
file, an mmproj, an oracle dump - a model condition); never on the existence of an artifact
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

**A test - or a program a test builds or spawns - whose subject is not the `.dlim` image rail
never mints or maps a MODEL image: it either runs with `DASLLAMA_IMAGE=0` in its environment,
or calls no `load_model`, `load_model_cached`, or `load_model_image`.**

**A predicate whose value the BOX decides (a device capability, a policy default) and that
therefore cannot differ between two runs on one machine is never tested through its own
value; test it through the argv it gates or the mode it selects.**

**A test for an added, moved, or edited registration reaches the registered thing through its
registry, and never calls it directly.** A registry is the storage a `register_*` call writes
and a lookup reads at dispatch - a table, a list, or a single hook global - or the `[EnvConfig]`
env registry.

**A new pre-tokenizer family or backend ships its `corpus_case` arm in `test_tokenizer.das`,
naming the `ggml-vocab-*.gguf` fixture.**

**A `corpus_case` arm that does not assert BOTH the exact reference ids and a lossless
round-trip is a defect.**

**A test that compares generated tokens, ids, or logits without logging a human-readable form
of BOTH sides is a defect: for a token or id compare the decoded text (`log_gen_texts` in
`_model_tier.das`, or one line per side), for a logits compare each side's argmax index and
value.** A red, or a suspicious green, must be readable in the log, not only as an id or float
difference.

**A cell whose name, comment, arm, or fixture claims a size, depth, or row count asserts that
number.** A cap, a resize, or a counter showing the path ran is not evidence that the number was
reached.

**A freeform token-parity cell whose two sides can round differently - different lanes,
backends, batch shapes or kernel forms - is a defect: use the forced-feed logits-tolerance
form, the same fixed tokens fed to both sides and logits compared within a bar.** A counting
cell - one whose prompt forces a continuation that cannot tie, so greedy tokens are fixed -
stays token-exact.

**A token-exact freeform compare states in the cell what makes its two sides one code path -
the shared entry point, or an assert pinning the lane.**

**An ASR family with no token-for-token oracle cell is a defect** - the cell compares a
transcript against a reference leg, external dump or CPU control alike.

**A test in this folder that loads a stocked artifact whose producer the test cannot name is a
defect.** A stocked artifact is anything the test reads out of `models_dir()` that this repo's
build does not produce - model files, mmprojs, front-end packs, image fixtures, oracle dumps.
Any one of these names the producer: a row in `../performance/model_specs.das`; a row's
`companions` list; a row in `asr_catalog` (`../performance/profile_common.das`); a convert
script beside `model_specs.das` in `../performance/`; for an oracle dump, the mint script stocked
beside the dumps under `models_dir()`, named by the test that loads the dump.

**A test that reads a vision encode oracle dump without naming the minting arm in its header -
the backend, the flash-attention setting, and the mmproj precision the dump came from - is a
defect.**

**A cell sets every process-wide driver setter and serving-lane knob its claim depends on, even
when the claim needs the knob at its DEFAULT value.**

**A cell returns with every family pin unset - whether or not this cell set one - and every
driver setter it touched back at its default; `reset_<family>_q8` is the restore.** Why a hook
left set changes what the next cell measures is `CLAUDE.md`'s "Metal fixtures".

**A cell claiming a family serving lane that does not pin it through the family's own lane
knobs - `set_<family>_q8` / `reset_<family>_q8`, canary's `set_canary_enc_q8` /
`reset_canary_enc_q8`, whisper's `set_asr_fp32` / `set_asr_tower_fp32` - or through a loader
parameter that takes the lane, is a defect.** A runtime decline standing in for a pin measures
whichever lane the box's policy picked.

**A cell that loads a media carrier under a lane pin - a `set_<family>_q8`-class knob, or a
`set_metal_tensor_crowns` / `pin_metal_tensor_crowns` pin - and whose subject is not that lane
knob itself mints in memory through the family's `stage_*` + `mint_*` pair, never through a
`.dlim`-baking loader (`load_<family>_tower` / `load_<family>_encoder` /
`load_<carrier>_model` / `load_tts_model` / `load_styletts2` / `load_model` /
`load_model_cached` / `load_model_image`).** A disk bake under a pinned lane GC-purges the
serving lane's `.dlim` beside the model, and the next direct-image load in another suite panics
on the wrong identity.

**A cell whose subject IS the lane knob (`load_asr_model` under `set_asr_tower_fp32`) loads
through the `.dlim`-baking loader, never around it.** The pin is part of what the image
identity records.

**A CPU-vs-GPU arm on Metal - one comparing the two lanes' outputs, not one whose subject is
the GPU lane's decline - runs its CPU stages on a PLANAR model (the non-blob form, the only
one CPU inference reads) and the stages a decode override selects on that model's blob twin
(`blob_twin(t, path, seq_cap)`, `test_metal_decode_parity.das`), in one session.** The planar
model and its blob twin share one shape, so one session serves both.

**A diff that adds a model-loading block to a `run.das` suite other than `model-free`,
`stocked` and `kernels` tags it with its family.** The family tag is the token passed to
`family_on(t, name)` (`_model_tier.das`). An untagged block silently joins every family's
gate.

**No CPU-control batch parity runs against `Llama-3.3-70B-Instruct-Q4_K_M.gguf`.** The
batched code paths get their parity on small models, through pins.

**Setting a knob a cell can reach only through the environment after the process that reads it
starts is a defect - set it before that process starts.** That process is a child the cell
spawns, or the runner's own. An in-cell set is invisible to the running config, which is read
once at context init.

**A cell that cannot set an environment-read knob before its reader starts names that knob's
value in the text a red prints - the cell label or the assert.** An environment-read knob is
one the running config reads once, at context init.

**A cell asserting the UNPINNED default lane never compares against a hardcoded lane - it
compares against the predicates the lane policy itself consults, `float_batch_override_active()`
and the family's `<family>_gpu_would_serve()` where one exists.** The default lane differs
per box, so the assert is on the lane the policy selects, not on one predicate's own value.

**A cell that runs with no model loaded and encodes, preprocesses, or asserts on media bytes
an encoder consumes - pixels or audio samples, not a `.dlim` model image - builds its fixture
procedurally and pins its expectations in-repo.**

**An image a test feeds an embedder that the test does not build, and that
`DASLLAMA_VISION_DUMP` cannot preview, is a defect** - a red never requires adding
instrumentation before a human can see what the model consumed.

**An audio clip a test feeds an embedder that the test does not build, and that is not one of
the clips stocked beside the models (`jfk.wav`, `gemma4a_test2.wav`), is a defect** - a clip
nobody else can play makes a red unreadable. A newly stocked clip joins this list in the same
change.

**A media fixture an embedder-parity cell regenerates in-test and compares against an oracle
dump, with no exact-value generator - one whose values are exactly representable floats, so
every box produces the same bytes - is a defect.** A generator running libm transcendentals is
not exact-value: it is not float-portable.

**An embedding-parity cell that does not name its fixture, or does not log the measured
maxdiff on green as well as red, is a defect.**

**A cell that adds or loosens a tolerance bar ships, in the same change, a control that lands
outside the new bar.** A bar nothing has ever exceeded is not known to discriminate.

**A family that gains a live thinking or tool format ships its recognition tests in the same
change** - the wire-shape pins, the render pins, and a live server case gated on the family's
smallest GGUF that sits under `LARGE_TIER_BYTES` (`_model_tier.das`).

**A poison control on a tower the Metal driver serves - a run of the gate with the tower's
weights zeroed, which must RED - zeroes every weight buffer the served route reads.** Which
buffers those are depends on the route: a twin-W route reads only the baked halfword copy of
the GEMM weights (`wblob`), so zeroing that buffer alone is a valid control there; a route
that also reads the f32 plane (`fblob`) needs both zeroed. A poison the served route never
reads passes on a broken kernel.

**An ASR cell comparing transcripts across two serving lanes, other than a crowned lane
against its tensor twin, asserts TOKEN equality.** A crowned lane is the kernel form the tuner
measured fastest and armed as the serving one; its tensor twin is the same kernel written on
Metal's tensor primitives.

**An ASR cell comparing a crowned lane against its tensor twin asserts WORD equality** - the
twins' rounding legitimately flips tokens.

**An ASR transcript cell that cannot assert the equality its comparison calls for converts to
a forced-feed logits compare within a tolerance bar - never to a looser text compare.**
