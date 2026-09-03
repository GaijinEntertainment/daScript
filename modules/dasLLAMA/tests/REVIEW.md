# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: the parent folder's `../followup_*.md` ledgers.

**Every PR runs `run.das -- --suite model-free`, plus every test here the change reaches - never
the whole directory.** A change reaches a test when it alters anything the test's result
depends on - the test file, a shared helper, engine code it exercises, an in-tree fixture or
corpus it reads, or a name it asserts on; a comment-only edit reaches none.

**Leaving a test file out of every `run.das` suite is a defect, unless the file's header
states why its cells cannot hold under `DASLLAMA_CPU_PREFILL=1`.** `DASLLAMA_CPU_PREFILL=1` is
what the runner arms for every suite.

**Listing a test file whose cells cannot hold under `DASLLAMA_CPU_PREFILL=1` in any `run.das`
suite is a defect, and so is leaving that fact out of the file's header.**

**Invoking dastest directly on a test file in a `run.das` model suite (every suite but
`model-free`) is a defect. A `model-free` file runs through the runner or under plain dastest.**

**Every test RUN runs under `-jit` - never the interpreter, never AOT.** A compile-only CI lane
passes dastest's `--compile-only`. Under the interpreter a model-gated suite's cells skip, and
a run of skips is not the coverage the suite owes.

**A diff that puts a `[test]` file requiring any `dasllama/*` module outside this folder is a
defect.** Out-of-folder instances are ledgered in `CLAUDE.md`'s "Out-of-folder test files" note.

**A diff that registers a test file in this folder in a `CMakeLists.txt` is a defect - a
`run.das` suite listing is the only registration these files get.**

**A diff that adds a gate, removes a gate, or moves a gate to another suite or arm, in a file
whose `CLAUDE.md` paragraph LISTS its gates, updates that paragraph in the same change.** A
paragraph that only names the file (a brace list, a suite roster) carries nothing to update
and does not fire this rule.

**A new test file listed in `run.das`'s `model-free` suite, or in no `run.das` suite at all,
whose name does not say what it covers, gets a `CLAUDE.md` entry in the same change** -
`run.das`'s `model-free` list is the complete census, the `CLAUDE.md` map is deliberately
partial.

**A diff that adds, renames, or drops an arm name - the literal passed to `arm_on(t, name)`
(`_model_tier.das`), what `--arm` matches - updates the arm census in `CLAUDE.md`'s "Arm
filter mechanics" section in the same change** - an arm the census does not name is
unreachable to whoever is choosing what to run.

**Weakening a contract-pinning gate - dropping an assert, loosening a bound, or narrowing the
corpus or sweep it covers - is a defect.** The pinned set, each with what it pins:
`test_program_roots.das` (the `ROOT_DIRS` sweep, `options stack = 524288`, prefill intent);
`test_env_registry.das` (the `../ENVIRONMENT.md` knob contract); `test_model_specs.das`
(`../performance/model_specs.das`'s model-set table); `test_metal_prefill_kernels.das`'s
softcap, sink (`hass`) and span cells; `test_site_records.das` (the byte-compare of
`site/files/dasllama/bench_records.json` (repo root) against a fresh `merge_site_records`
run); `test_exchange_schema.das` and `test_bench_records_schema.das` (the
`write_bench_records` output, corpus sweeps included); `test_scheduler.das`'s media-stream
bypass check (no cached hit at `prefix_attach`, no donated pages at `donate_stream`);
`test_vulkan_kernels.das`'s tile-pick cell (which tile the Vulkan matmul picks for a given
width and row count, and whether that dispatch splits its reduction across partial planes).
A gate whose failure means a documented contract changed, rather than a kernel regressing, joins
this list in the same change - as a file when
every cell of it pins, as a named cell otherwise.

**A test passes or skips explicitly on every platform - a cell that neither asserts nor
registers a skip is a defect.** A cell that returns without asserting -
the module is absent, its models are not stocked, no device answered, a capability declined -
registers `t |> skip` there; `feint` is a print, not a skip.

**A skip gate keys on a fact the box owns - a device capability, a run-mode knob's value, a
host toolchain's presence, a compile-time module-presence check
(`typeinfo builtin_module_exists`) - or on a stocked fixture beside the models (a model
file, an mmproj, an oracle dump - a model gate); never on the existence of an artifact this
repo's build or a previous test run produced (a mint, a generated binary, a dump a test
wrote).** An artifact gate goes permanently false when its producer moves.

**A test that loads a model above the large tier (`LARGE_TIER_BYTES`, `_model_tier.das`)
without gating on `DASLLAMA_PARITY_FULL=1` is a defect** - that gate is a final pre-PR gate,
not the iteration loop. In this folder the
spelling is `model_available` (`_model_tier.das`). A test that cannot require
`_model_tier.das` open-codes the same env check.

**A test - or a program a test builds or spawns - whose subject is not the `.dlim` image
rail never calls `load_model`, `load_model_cached`, or `load_model_image` - it loads each
carrier through that carrier's own loader.** Decoders load through `load_model_`
(`../dasllama/dasllama_load.das`). Towers, embedders, and union carriers load through their
family or carrier loaders.

**A predicate whose value the BOX decides (a device capability, a policy default) and that
therefore cannot differ between two runs on one machine is never tested through its own
value; test it through the argv it gates or the mode it selects.**

**A test for an added, moved, or edited registration reaches the registered thing through its
registry, and never calls it directly.** The registries this governs: the arch registrations
(`register_decode_override` and its sibling `register_*` hooks), the `[EnvConfig]` env
registry, and the format/backend dispatch tables. A new registry joins that list in the same
change.

**A diff that changes a kernel's dispatch geometry - its grid divisor, threadgroup size, or
threadgroup-memory length - updates every gate that hand-dispatches that kernel, in the same
change.** A hand-dispatched gate encodes the geometry itself, so a moved divisor leaves the
gate dispatching the wrong shape with no error.

**A diff that changes a kernel's kargs - the kernel-argument struct, or any buffer binding -
updates every gate that hand-binds that kernel, in the same change.** A stale hand bind reads
the wrong buffer and passes on garbage that happens to compare.

**A kernel that gains a kargs field whose non-default value changes what it computes or which
elements it reads or writes - a branch selector, a row or element base, a stride - ships, in
the same change, a gate cell that sets that field to a non-default value.** At the default the
new field has no visible effect: a CPU oracle that ignores it and the kernel that honors it
agree.

**A new pre-tokenizer family or backend ships its `corpus_case` arm in `test_tokenizer.das`,
naming the `ggml-vocab-*.gguf` fixture.**

**A `corpus_case` arm missing either an exact-reference-id assert or a lossless round-trip
assert is a defect.**

**A test that compares generated tokens, ids, or logits without logging a human-readable form
of BOTH sides is a defect: the decoded text where the model carries a vocab (`log_gen_texts`
in `_model_tier.das`, or one line per side), otherwise each side's argmax index and value.** A
red, or a suspicious green, must be readable in the log, not only as an id or float
difference.

**A fixture claiming a size or depth property whose test does not assert the actual number is
a defect.** A resize cap is not evidence.

**A freeform token-parity cell is a defect.** Freeform coverage uses the forced-feed
logits-tolerance form - the same fixed tokens fed to both sides, logits compared within a bar.
Counting cells - those whose prompt forces a continuation that cannot tie, so greedy tokens
are fixed - stay token-exact.

**A kernel-unit cell - a model-less cell that dispatches one kernel class and asserts on its
output - missing a compare against a CPU oracle that can witness the cell's property is a
defect.**

**A kernel-unit cell fills a GPU output buffer with a sentinel before every dispatch whose
output it then reads.** An unprefilled output can pass by staying stale - the previous
dispatch's values, or garbage that happens to sit inside the tolerance bar.

**A cross-dispatch bit-identity compare - comparing the outputs of two dispatches - runs GPU
against GPU.** No CPU oracle can witness that property.

**A kernel-unit cell whose output buffer is its input buffer, and whose CPU oracle does not
differ from that input by construction, pairs its compare with an assert that the output
differs from the input at a known index.** An in-place kernel that never ran leaves the input,
which can wrongly satisfy a tolerant compare.

**An ASR family with no token-for-token oracle cell is a defect** - the cell compares a
transcript against a reference leg, external dump or CPU control alike.

**A test in this folder that loads a stocked artifact not traceable to something checked in is
a defect.** Stocked artifacts are model files, mmprojs, image fixtures, and oracle dumps. Any
one of these counts as traceable: a row in `../performance/model_specs.das`; a row's
`companions` list; a row in `asr_catalog` (`../performance/profile_common.das`); a convert
script beside `model_specs.das` in `../performance/`. For an oracle dump, a mint script that
regenerates it also counts - it sits beside the dumps under `models_dir()`, and the test that
loads the dump names it.

**A test that reads a vision encode oracle dump without naming the minting arm in its header -
the backend, the flash-attention setting, and the mmproj precision the dump came from - is a
defect.**

**A cell that does not establish every process-wide driver setter and serving-lane knob its
claim depends on, and restore each knob it set in-process to the value it had on entry before
returning, is a defect.** This holds even when the claim needs the knob at its DEFAULT value.
The mechanism
(why the hooks flip legs silently) is `CLAUDE.md`'s "Metal fixtures" section.

**A cell claiming a family serving lane that does not pin it through the family's own lane
knobs - `set_<family>_q8` / `reset_<family>_q8`, canary's `set_canary_enc_q8` /
`reset_canary_enc_q8`, whisper's `set_asr_fp32` / `set_asr_tower_fp32` - or through a loader
parameter that takes the lane, is a defect.** A family whose knob has no reset twin restores
it to the documented default. A runtime decline standing in for a pin measures whichever
lane the box's policy picked.

**A cell that loads a media carrier under a lane pin - a `set_<family>_q8`-class knob, or a
`set_metal_tensor_crowns` / `pin_metal_tensor_crowns` pin - and whose subject is not that lane
knob itself mints in memory through the family's `stage_*` + `mint_*` pair, never through a
`.dlim`-baking loader (`load_<family>_tower` / `load_<family>_encoder` / `load_<carrier>_model` /
`load_model` / `load_model_cached` / `load_model_image`).** A disk bake under a pinned lane
GC-purges the serving lane's `.dlim` beside the model, and the next direct-image load in
another suite panics on the wrong identity.

**A cell whose subject IS the lane knob (`load_asr_model` under `set_asr_tower_fp32`) loads
through the `.dlim`-baking loader, never around it.** The pin is part of what the image
identity records.

**A CPU-vs-GPU arm that does not run a PLANAR model - the non-blob form, the only one CPU
inference reads - for its CPU stages, and that model's blob twin (`blob_twin(t, path,
seq_cap)`, `test_metal_decode_parity.das`) for the stages a decode override selects, is a
defect.** One session spans the planar model and its blob twin, because sessions are
geometry-bound. A cell whose subject is why the GPU lane declined keeps the planar model and
needs no twin.

**A diff that adds a model-loading block to a `run.das` MODEL suite (every suite the
`--family` filter reaches - not the model-free suite) tags it with its family.** The family
tag is the token passed to `family_on(t, name)` (`_model_tier.das`). An untagged block
silently joins every family's gate.

**No CPU-control batch parity runs against `Llama-3.3-70B-Instruct-Q4_K_M.gguf`.** The
batched code paths get their parity on small models, through pins.

**Setting a knob a cell can reach only through the environment after the process that reads it
starts is a defect - set it before that process starts.** That process is a child the cell
spawns, or the runner's own. An in-cell set is invisible to the running config, which is read
once at context init.

**A cell that cannot set an environment-read knob before its reader starts, and whose assert
text does not name the value it asserts under, is a defect.** An environment-read knob is one
the running config reads once, at context init.

**A cell asserting the UNPINNED default lane never compares against a hardcoded lane - it
compares against the predicates the lane policy itself consults, `float_batch_override_active()`
and the family's `<family>_gpu_would_serve()` where one exists.** The accelerate tier and the
GPU tower move the default per box, so the assert is on the lane the policy selects, not on
one predicate's own value.

**A cell that encodes, preprocesses, or asserts on media bytes an encoder consumes - pixels
or audio samples, not a `.dlim` model image - with no model loaded builds its fixture
procedurally and pins its expectations in-repo.**

**An image a test feeds an embedder that the test does not build, and that
`DASLLAMA_VISION_DUMP` cannot preview, is a defect** - a red never requires adding
instrumentation before a human can see what the model consumed.

**An audio clip a test feeds an embedder that the test does not build, and that is not one of
the checked-in fixtures beside the models (`jfk.wav`, `gemma4a_test2.wav`), is a defect** - a
clip nobody else can play makes a red unreadable. A newly checked-in clip joins this list in
the same change.

**A tier-1 media fixture - one an embedder-parity cell regenerates in-test and compares
against an oracle dump - with no exact-value generator is a defect.** A generator running libm
transcendentals is not exact-value: it is not float-portable.

**An embedding-parity cell that does not name its fixture, or does not log the measured
maxdiff on green as well as red, is a defect.**

**A kernel-unit cell that dispatches a kernel class no cell dispatched before, and a cell
that adds or loosens a tolerance bar, each ship a control that reds them in the same
change.** A control is a run of the same gate that must RED - a poisoned input, a poisoned
expectation, a disconnected mechanism, or a second independent lane; a gate's own reference is
never its control. A bar nothing has ever exceeded is not known to discriminate, and a gate
that reads state the same code path wrote can be a tautology - only the control proves either
can fail.

**A family that gains a live thinking or tool format ships its recognition tests in the same
change** - the wire-shape pins, the render pins, and a live server leg gated on the family's
smallest GGUF that runs on the small tier (the file homes are `CLAUDE.md`'s "Model-free /
no-arm tests" and "Out-of-folder test files" notes). A family whose vocab carries no thinking
or tool markers has no format to test.

**A kernel-unit cell whose kernel reads f16 operands and whose oracle is wider-precision
feeds inputs that are exact in f16.** Otherwise the compare measures input rounding, and the
bar has to be loosened until it no longer discriminates.

**A gate for a kernel that attends inside a restricted horizon - a window, a sliding span, a
block-diagonal range - writes its CPU oracle to attend strictly inside that horizon.** A leak
then reds the ordinary compare, so the gate needs no separate leak control.

**A cell whose only compare is bit-identity between two kernel forms also compares one of the
two against a CPU oracle, in the same cell.** Two forms can be bit-equal and both wrong.

**A poison leg on a tower the Metal driver serves reaches every weight buffer the served route
reads.** Which buffers those are depends on the route: a twin-W route reads the baked halfword
twin (`wblob`), so poisoning that buffer alone is a valid control there, while a route reading
both buffers needs both zeroed. A poison the served route never reads passes on a broken
kernel.

**An ASR cell comparing transcripts across two serving lanes, other than a tune-armed kernel
form against its Metal-tensor twin, asserts TOKEN equality.**

**An ASR cell comparing a crowned lane against its tensor twin asserts WORD equality** - the
twins' rounding legitimately flips tokens. A crowned lane is the raced kernel form a tune
sidecar arms as the serving one; its tensor twin is the same kernel written on Metal's tensor
primitives.

**An ASR transcript cell that cannot assert the equality its comparison calls for converts to
a forced-feed logits compare within a tolerance bar - never to a looser text compare.**
