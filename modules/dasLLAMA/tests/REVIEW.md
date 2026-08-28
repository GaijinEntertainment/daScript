# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: this folder's `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`.

**Every PR runs `run.das -- --suite model-free`, plus every test here the change reaches - never
the whole directory.** A change reaches a test when it alters anything the test's result
depends on - the test file, a shared helper, engine code it exercises, an in-tree fixture or
corpus it reads, or a name it asserts on; a comment-only edit reaches none.

**Leaving a test file that no `run.das` model suite lists out of the `model-free` suite is a
defect, when its cells still assert what they claim under `DASLLAMA_CPU_PREFILL=1`.** The model
suites are every suite but `model-free`, and that variable is what the runner arms for
`model-free`. The listing lands in the same change that adds the file, and the file skips
honestly when its models are absent.

**Listing a test file that `DASLLAMA_CPU_PREFILL=1` disarms in any `run.das` suite is a defect,
and so is leaving that fact out of the file's header.** `DASLLAMA_CPU_PREFILL=1` is what the
runner arms for `model-free`.

**Invoking dastest directly on a test file in a `run.das` model suite (every suite but
`model-free`) is a defect; such a file runs only through `run.das`. A `model-free` file runs
through the runner or under plain dastest.**

**Every test RUN runs under `-jit` - never the interpreter, never AOT.** A compile-only CI lane
passes dastest's `--compile-only`. Under the interpreter a model-gated suite's cells skip, and
a run of skips is not the coverage the suite owes.

**A diff that puts a `[test]` file requiring any `dasllama/*` module outside this folder is a
defect.** Out-of-folder instances are ledgered in `CLAUDE.md`'s "Out-of-folder test files" note.

**A diff that registers a test file in this folder in a `CMakeLists.txt` is a defect.**

**A change that re-scopes a test file with a `CLAUDE.md` entry keeps that entry true in the
same change.**

**A new test file listed in `run.das`'s `model-free` suite, or in no `run.das` suite at all,
whose name does not say what it covers, gets a `CLAUDE.md` entry in the same change** -
`run.das`'s `model-free` list is the complete census, the `CLAUDE.md` map is deliberately
partial.

**A diff that adds, renames, or drops an arm name - the literal passed to `arm_on(t, name)`
(`_model_tier.das`), what `--arm` matches - updates the arm census in `CLAUDE.md`'s "Arm
filter mechanics" section in the same change** - an arm the census does not name is
unreachable to whoever is choosing what to run.

**Weakening `test_program_roots.das` - dropping a directory from its `ROOT_DIRS` sweep,
loosening its `options stack = 524288` assert, or relaxing its prefill-intent assert - is a
defect.**

**Weakening `test_env_registry.das` is a defect.** It enforces the knob contract that
`../ENVIRONMENT.md` describes.

**Weakening `test_model_specs.das` is a defect.** It is the gate on the model-set table
(`../performance/model_specs.das`).

**Weakening the softcap, sink and span cells of `test_metal_prefill_kernels.das` - dropping
or loosening a softcap, sink (`hass`) or span arm, or dropping a span shape - is a defect** -
they are what fails when `pf_p_weight` and `metal_attn_rowstat` drift apart.

**Weakening `test_site_records.das` - the gate that byte-compares
`site/files/dasllama/bench_records.json` (repo root) against a fresh `merge_site_records`
run - is a defect.**

**Weakening `test_exchange_schema.das` or `test_bench_records_schema.das` - loosening an
assert, dropping one, or narrowing the corpus either one sweeps - is a defect** - they gate
the real `write_bench_records` output.

**Weakening `test_scheduler.das`'s media-stream bypass check - a media stream attaches no
cached hit at admit (`prefix_attach`) and donates no pages at reap (`donate_stream`) - is a
defect.** Cache keys are token ids, and the KV past the splice does not follow from them.

**A test that silently vanishes on one platform is a defect, and so is a zero-assertion pass -
a test passes or skips explicitly on every platform.** A cell whose whole body is
platform-gated registers `t |> skip` on the platforms where that body compiles out.

**A skip gate keys on a device capability, a run-mode knob's value, or a stocked fixture beside
the models (a model file, an mmproj, an oracle dump - a model gate) - never on the existence of
an artifact this repo's build or a previous test run produced (a mint, a generated binary, a
dump a test wrote).** An artifact gate goes permanently false when its producer moves.

**A test that loads a model over 6 GiB without gating on `DASLLAMA_PARITY_FULL=1` is a
defect** - that gate is a final pre-PR gate, not the iteration loop. In this folder the
spelling is `model_available` (`_model_tier.das`). A test that cannot require
`_model_tier.das` open-codes the same env check.

**A test whose subject is not the `.dlim` image rail never calls `load_model`,
`load_model_cached`, or `load_model_image` - it loads each carrier through that carrier's own
loader.** Decoders load through `load_model_` (`../dasllama/dasllama_load.das`). Towers,
embedders, and union carriers load through their family or carrier loaders.

**A function that gains a parameter, or a parameter that gains an accepted value, ships a
test for the new value.** The test feeds the new value and checks the result. It lives in
this folder, and it lands in the same change. "The model still runs" is not that test.

**A predicate whose value is fixed by the build platform - it cannot differ between two runs on
one machine - is never tested through its own value; test it through the argv it gates or the
mode it selects.**

**A test for an added, moved, or edited registration reaches the registered thing through its
registry, and never calls it directly.** The registries this governs: the arch registrations
(`register_decode_override` and its sibling `register_*` hooks), the `[EnvConfig]` env
registry, and the format/backend dispatch tables. A new registry joins that list in the same
change. A `[metal_dispatch]` declaration is not one of them.

**A diff that changes anything a kernel dispatches with, binds, or reads from its kargs - the
kernel's kernel-argument struct - updates every gate that hand-binds that kernel, in the same
change.** A hand-bound gate dispatches the geometry and threadgroup memory its production
encoder does. The mechanism - why a missed threadgroup-memory length fails silently - is
`CLAUDE.md`'s "Metal kernel gates" section.

**A kernel that gains an in-body branch keyed on a kargs field ships, in the same change, a
gate cell that sets that field to the value selecting the new branch.**

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
logits-tolerance form. Counting cells stay token-exact.

**A diff that adds a metal kernel class under `../dasllama/` - a `[metal_kernel]` def or a
new instance of a template carrying one - covers that class in `test_kernel_coverage.das`,
one of two ways.** Either a census row there dispatches the class, or the diff names it in
that file's `CENSUS_NEVER_DISPATCHED`, with the reason no row can reach it. A dispatching
row runs on a small model - one the suite runs without `DASLLAMA_PARITY_FULL=1`. Naming a
class a census row could dispatch is a defect.

**A kernel-unit cell missing a compare against a CPU oracle that can witness the cell's
property is a defect.**

**A kernel-unit cell testing cross-dispatch bit-identity that does not compare GPU against GPU,
with its output buffers prefilled with a sentinel, is a defect.** No CPU oracle can witness
that property.

**A kernel-unit cell whose output plane is its input plane, and whose compare is not paired
with an assert that the output differs from the input at a known index, is a defect.** This
applies when the cell's CPU oracle does not differ from the input by construction. An in-place
kernel that never ran leaves the input, which can wrongly satisfy a tolerant compare.

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

**A cell that does not establish every driver hook and serving-lane knob its claim depends on,
and restore each to its default before returning, is a defect.** This holds even when the
claim needs the knob at its DEFAULT value. A hook is any process-wide setter with no
read-back. The environment can carry a knob either way. The mechanism (why the hooks flip legs
silently) is `CLAUDE.md`'s "Metal fixtures" section.

**A cell claiming a family serving lane that does not pin it with `set_<family>_q8` and undo
the pin with `reset_<family>_q8` is a defect.** A runtime decline standing in for a pin
measures whichever lane the box's policy picked.

**A CPU-vs-GPU arm that does not run a PLANAR model for its CPU stages, and that model's
`blob_twin(t, path, seq_cap)` for override-selected stages, is a defect.** One session spans
both models, because sessions are geometry-bound. A decline-reason cell keeps the planar
model.

**A diff that adds a model-loading block tags it with its family.** The family tag is the
token passed to `family_on(t, name)` (`_model_tier.das`). An untagged block silently joins
every family's gate.

**No CPU-control batch parity runs against the 70B.** Its batch coverage is ENGAGE-only in the
support matrix. The batched code paths get their parity on small models, through pins.

**Setting a knob a cell can reach only through the environment after the process that reads it
starts is a defect - set it before that process starts.** That process is a child the cell
spawns, or the runner's own. Such a knob needs no restore. An in-cell set is invisible to the
running config, which is read once at context init.

**A cell that cannot set an environment-read knob before its reader starts, and whose assert
text does not name the value it asserts under, is a defect.** An environment-read knob is one
the running config reads once, at context init.

**A cell asserting the UNPINNED default lane never compares against a hardcoded lane - it
compares against the predicates the lane policy itself consults, `float_batch_override_active()`
and the family's `<family>_gpu_would_serve()` where one exists.** The accelerate tier and the
GPU tower move the default per box, so the assert is on the lane the policy selects, not on
one predicate's own value.

**A cell that encodes, preprocesses, or asserts on media bytes an encoder consumes - pixels
or audio samples, not a `.dlim` model image - with no model loaded is a defect when it does
not build its fixture procedurally and pin its expectations in-repo.**

**Media a test feeds an embedder - an image or an audio clip - that the test does not build,
and that `DASLLAMA_VISION_DUMP` cannot preview (images), is a defect** - a red never requires
adding instrumentation before a human can see what the model consumed.

**A tier-1 media fixture - one an embedder-parity cell regenerates in-test and compares
against an oracle dump - with no exact-value generator is a defect.** A generator running libm
transcendentals is not exact-value: it is not float-portable. Orientation coverage uses shaped
exact fixtures.

**An embedding-parity cell that does not name its fixture, or does not log the measured
maxdiff on green as well as red, is a defect.**

**A new gate, or a new or loosened tolerance bar, ships a control that reds it - a poison, a
knockout, a disconnected mechanism, or a cross-lane witness - in the same change.** A bar
nothing has ever exceeded is not known to discriminate, and a gate that reads state the same
code path wrote can be a tautology - only the control proves either can fail.

**A family that gains a live thinking or tool format ships its recognition tests in the same
change** - the wire-shape pins, the render pins, and a live server leg gated on the family's
smallest GGUF that runs on the small tier (the file homes are `CLAUDE.md`'s "Model-free /
no-arm tests" and "Out-of-folder test files" notes). A family whose vocab lacks the markers
has no format to test.
