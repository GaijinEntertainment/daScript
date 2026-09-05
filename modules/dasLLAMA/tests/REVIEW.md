# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

**Every PR runs `run.das -- --suite model-free` and, on a box with models stocked, `--suite
stocked` (`preflight --full` runs both when the diff touches `modules/dasLLAMA/`), plus every
test here the change reaches - never the whole directory.** A change reaches a test when it
alters anything the test's result depends on - the test file, a shared helper, engine code it
exercises, an in-tree fixture or corpus it reads, or a name it asserts on; a comment-only edit
reaches none.

**A PR's `stocked` run carries no `--exclude`** - `--suite stocked --exclude test_ple_modes` is
the iteration form between PRs; a PR that ships on it never ran the PLE coverage.

**A test file - a `.das` in this folder that dastest runs: one carrying at least one `[test]`
function, or one whose `cant_`, `failed_` or `invalid_` prefix makes its compile the assertion
- whose cells cannot hold under `DASLLAMA_CPU_PREFILL=1` says so in its header and sits in no
`run.das` suite; every other test file in this folder sits in one.** `DASLLAMA_CPU_PREFILL=1`
is what the runner arms for every suite.

**Invoking dastest directly on a test file in a `run.das` model suite (every suite but
`model-free` and `stocked`) is a defect - run it through `run.das`.**

**`run.das` runs nothing on require - no `[init]`, and no global whose initializer spawns,
logs, writes the environment or touches the filesystem; a diff that adds one is a defect.**
`test_run_suites.das` and `test_run_summary.das` require `run` by bare same-dir name to read
its tables, so anything that fires on require fires inside every one of those test processes.
Data tables (`AREAS`, `MODULE_AREAS`) are what the tests are there to read.

**A cell asserting a chat template's INSTRUCT wire - a closed empty thought block and no
thinking gate - calls `set_thinking(c, false)` on its renderer before the first turn.**
`ChatTemplate.think_default` is `true` unless a family clears it, so an un-opted-out turn 1
renders the gate and arms the channel-marker stops, and the cell asserts the wrong wire.

**Every test RUN runs under `-jit` - never the interpreter, never AOT.** A compile-only CI lane
passes dastest's `--compile-only`. Under the interpreter a model-gated suite's cells skip, and
a run of skips is not the coverage the suite owes.

**A diff that registers a test file in this folder in a `CMakeLists.txt` is a defect - a
`run.das` suite listing is the only registration these files get.**

**A diff that adds, removes or moves a gate (one test cell, or a file between suites) corrects,
in the same change, every `CLAUDE.md` clause that names that gate's suite, fixture, model, arm
or skip condition.** A clause that only names the file (a brace list, a suite roster) carries
nothing to correct.

**Weakening `test_run_suites.das` is a defect** - it pins the per-PR split (a suite-less file
that reaches a machine-local fixture root - `models_dir()`, `model_available()`,
`llama2c_dir()`, `whisper_dir()` - sits in `stocked`, every other one in `model-free`), the
folder census (every test file sits in a `run.das` suite, the CPU-prefill tripwire the one
declared exemption), the area tables (every per-PR file has an `area_tests` row; every arm an
area names exists; every arm the image suite declares is reachable from an area or listed as
unclaimed) and the `--exclude` filter. `model-free` is the every-change gate and runs the same
on a bare box; `stocked` is the per-PR model coverage and a run of skips without models.

**A new test file listed in `run.das`'s `model-free` or `stocked` suite, or in no `run.das`
suite at all, whose name does not say what it covers, gets a `CLAUDE.md` entry in the same
change** - `run.das`'s `model-free` and `stocked` lists together are the complete census, the
`CLAUDE.md` map is deliberately partial.

**A diff that adds, renames, or drops an arm name - the literal passed to `arm_on(t, name)`
(`_model_tier.das`), what `--arm` matches - updates the arm census in `CLAUDE.md`'s "Arm
filter mechanics" section and, for an image-suite arm, `run.das`'s `area_image_arms` (or its
unclaimed list in `test_run_suites.das`) in the same change** - an arm the census does not
name is unreachable to whoever is choosing what to run, and an arm no area claims never runs
from `--area` or `--changed`.

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
matmul picks for a given width and row count, and whether that dispatch splits its reduction
across partial planes).

**A diff that adds a gate whose failure means a documented contract changed, rather than a
kernel regressing, adds it to the pinned set above in the same change** - as a file when every
cell of it pins, as a named cell otherwise.

**On every platform, a cell that neither asserts nor registers a skip is a defect.** A cell
that returns without asserting - the module is absent,
its models are not stocked, no device answered, a capability declined - registers `t |> skip`
there; `feint` is a print, not a skip.

**A cell's skip condition keys on a fact the box owns - a device capability, a run-mode knob's
value, a host toolchain's presence, a compile-time module-presence check
(`typeinfo builtin_module_exists`) - or on a stocked fixture beside the models (a model
file, an mmproj, an oracle dump - a model condition); never on the existence of an artifact
this repo's build or a previous test run produced (a minted `.dlim`, a generated binary, a
dump a test wrote).** An artifact condition goes permanently false when its producer moves.

**A test that loads a model above the large tier (`LARGE_TIER_BYTES`, `_model_tier.das`)
without gating on `DASLLAMA_PARITY_FULL=1` is a defect** - `DASLLAMA_PARITY_FULL=1` is a final
pre-PR switch, not the iteration loop. In this folder the spelling is `model_available` (`_model_tier.das`).
A test that cannot require `_model_tier.das` open-codes the same env check.

**A test - or a program a test builds or spawns - whose subject is not the `.dlim` image
rail never calls `load_model`, `load_model_cached`, or `load_model_image` - it loads each
carrier through that carrier's own loader.** Decoders load through `load_model_`
(`../dasllama/dasllama_load.das`); every other carrier loads through its own
`load_<family>_tower` / `load_<family>_encoder` / `load_<carrier>_model`.

**A predicate whose value the BOX decides (a device capability, a policy default) and that
therefore cannot differ between two runs on one machine is never tested through its own
value; test it through the argv it gates or the mode it selects.**

**A test for an added, moved, or edited registration reaches the registered thing through its
registry, and never calls it directly.** A registry is the storage a `register_*` call writes
and a lookup reads at dispatch - a table, a list, or a single hook global - or the `[EnvConfig]`
env registry.

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
backends, batch shapes or kernel forms - is a defect, and a token-exact freeform compare
states in the cell what makes its two sides one code path: the shared entry point, or an
assert pinning the lane.** Freeform coverage across a pair that rounds differently uses the
forced-feed logits-tolerance form - the same fixed tokens fed to both sides, logits compared
within a bar. Counting cells - those whose prompt forces a continuation that cannot tie, so
greedy tokens are fixed - stay token-exact.

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

**A test in this folder that loads a stocked artifact whose producer the test cannot name is a
defect.** Stocked artifacts are model files, mmprojs, image fixtures, and oracle dumps. Any
one of these names the producer: a row in `../performance/model_specs.das`; a row's
`companions` list; a row in `asr_catalog` (`../performance/profile_common.das`); a convert
script beside `model_specs.das` in `../performance/`. For an oracle dump, the mint script
stocked beside the dumps under `models_dir()` also counts, and the test that loads the dump
names it.

**A test that reads a vision encode oracle dump without naming the minting arm in its header -
the backend, the flash-attention setting, and the mmproj precision the dump came from - is a
defect.**

**A cell that does not establish every process-wide driver setter and serving-lane knob its
claim depends on, and restore each knob it set in-process to the value it had on entry before
returning, is a defect.** This holds even when the claim needs the knob at its DEFAULT value.
The mechanism (why a hook left set silently changes which implementation the next cell
measures) is `CLAUDE.md`'s "Metal fixtures" section.

**A cell claiming a family serving lane that does not pin it through the family's own lane
knobs - `set_<family>_q8` / `reset_<family>_q8`, canary's `set_canary_enc_q8` /
`reset_canary_enc_q8`, whisper's `set_asr_fp32` / `set_asr_tower_fp32` - or through a loader
parameter that takes the lane, is a defect.** A runtime decline standing in for a pin measures
whichever lane the box's policy picked.

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

**A CPU-vs-GPU arm on Metal - one comparing the two lanes' outputs, not one whose subject is
the GPU lane's decline - runs its CPU stages on a PLANAR model (the non-blob form, the only
one CPU inference reads) and the stages a decode override selects on that model's blob twin
(`blob_twin(t, path, seq_cap)`, `test_metal_decode_parity.das`), in one session.** The planar
model and its blob twin share one shape, so one session serves both.

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

**A cell that cannot set an environment-read knob before its reader starts, and whose text
that prints with a red - the cell label or the assert - does not name the value it asserts
under, is a defect.** An environment-read knob is one
the running config reads once, at context init.

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
dump, with no exact-value generator, is a defect.** A generator running libm
transcendentals is not exact-value: it is not float-portable.

**An embedding-parity cell that does not name its fixture, or does not log the measured
maxdiff on green as well as red, is a defect.**

**A kernel-unit cell that dispatches a `[metal_dispatch]` or `[vk_dispatch]` class no cell
dispatched before ships a control that reds it in the same change.** A control is a run of
the same gate that must RED - a poisoned input, a poisoned expectation, a disconnected
mechanism, or a second independent lane; a gate's own reference is never its control.

**A cell that adds or loosens a tolerance bar ships, in the same change, a control that lands
outside the new bar.** A bar nothing has ever exceeded is not known to discriminate.

**A family that gains a live thinking or tool format ships its recognition tests in the same
change** - the wire-shape pins, the render pins, and a live server case gated on the family's
smallest GGUF that sits under `LARGE_TIER_BYTES` (`_model_tier.das`) (the file homes are
`CLAUDE.md`'s "Model-free / no-arm tests" and "Out-of-folder test files" notes). A family whose vocab carries no thinking
or tool markers has no format to test.

**A kernel-unit cell whose kernel reads f16 operands and whose oracle is wider-precision
feeds inputs that are exact in f16.** Otherwise the compare measures input rounding, and the
bar has to be loosened until it no longer discriminates.

**A gate for a kernel that attends inside a restricted horizon - a window, a sliding span, a
block-diagonal range - writes its CPU oracle to attend strictly inside that horizon.** A leak
then reds the ordinary compare, so the gate needs no separate leak control.

**A cell whose only compare is bit-identity between two kernel forms also compares one of the
two against a CPU oracle, in the same cell.** Two forms can be bit-equal and both wrong.

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
