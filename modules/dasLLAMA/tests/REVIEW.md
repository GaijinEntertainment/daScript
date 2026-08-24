# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`.

**Every PR runs `run.das -- --suite model-free`, plus every test here the change reaches - never
the whole directory.** A change reaches a test when it alters anything the test's result
depends on - the test file, a shared helper, engine code it exercises, an in-tree fixture or
corpus it reads, or a name it asserts on; a comment-only edit reaches none.

**A test file with at least one cell that RUNS (not skips) with no model file present is
listed in `run.das`'s `model-free` suite in the same change it is added.**

**A test file in no `run.das` model suite (every suite but `model-free`), whose every cell is
model-gated, is listed in the `model-free` suite too and skips honestly without its models** - 
the per-PR gate then runs it wherever the models are stocked.

**A test file in a `run.das` model suite (every suite but `model-free`) runs only through
`run.das`; dastest invoked directly on such a file is a defect. A `model-free` file runs
through the runner or under plain dastest.**

**Every test RUN runs under `-jit`** - never the interpreter, never AOT. A compile-only CI lane
passes dastest's `--compile-only`; a model-gated suite run interpreted, with skips standing in
for the guard, is a defect.

**A `[test]` file that requires any `dasllama/*` module lives in this folder.** Out-of-folder
instances are ledgered in `CLAUDE.md`'s "Out-of-folder test files" note.

**A test file in this folder is registered in no `CMakeLists.txt`.**

**A change that re-scopes a test file with a `CLAUDE.md` entry keeps that entry true in the
same change.**

**A new test file listed in `run.das`'s `model-free` suite, or in no `run.das` suite at all,
whose name does not say what it covers, gets a `CLAUDE.md` entry in the same change** -
`run.das`'s `model-free` list is the complete census, the `CLAUDE.md` map is deliberately
partial.

**A new, renamed, or dropped arm name - the literal passed to `arm_on(t, name)`
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

**Weakening `test_exchange_schema.das` or `test_bench_records_schema.das` - loosening an
assert, dropping one, or narrowing the corpus either one sweeps - is a defect** - they gate
the real `write_bench_records` output.

**Weakening `test_scheduler.das`'s media-stream bypass check - a media stream attaches no
cached hit at admit (`prefix_attach`) and donates no pages at reap (`donate_stream`) - is a
defect.** Cache keys are token ids, and the KV past the splice does not follow from them.

**A test passes or skips explicitly on every platform.** A test that silently vanishes on one
platform is a defect, and so is a zero-assertion pass - a cell whose whole body is
platform-gated prints a skip or feint on the platforms where that body compiles out.

**A skip gate keys on a device capability, a run-mode knob's value, or a stocked fixture beside
the models (a model file, an mmproj, an oracle dump - a model gate) - never on the existence of
an artifact this repo's build or a previous test run produced (a mint, a generated binary, a
dump a test wrote).** An artifact gate goes permanently false when its producer moves.

**A test loading a model over 6 GiB runs only under `DASLLAMA_PARITY_FULL=1`** - a final pre-PR
gate, not the iteration loop. Here the spelling is `model_available` (`_model_tier.das`); a
serving leg, which cannot require this folder's fixtures, open-codes the same gate. Check what a
test loads first.

**Every suite but `test_model_image.das` and `test_model_image_vulkan.das` - the image-rail
coverage pair (mint, map, GC, flavors) - loads each carrier through its own loader, never the
`.dlim` image rail (`load_model`, `load_model_cached`, `load_model_image`):** decoders through
`load_model_` (`../dasllama/dasllama_load.das`); towers, embedders, and union carriers through
their family or carrier loaders.

**A function that gains a parameter, or a parameter that gains an accepted value, ships a
test feeding the new value and checking the result.** "The model still runs" is not that
test.

**A predicate whose value is fixed by the build platform - it cannot differ between two runs on
one machine - is tested through the argv it gates or the mode it selects**, never through the
predicate's value.

**An added, moved, or edited registration's test observes reachability** - the registered
thing is reached through its registry, not called directly. The registries this governs:
the arch registrations (`register_decode_override` and its sibling `register_*` hooks), the
`[EnvConfig]` env registry, and the format/backend dispatch tables. A `[metal_dispatch]`
declaration is not one of them.

**A hand-bound kernel gate dispatches the geometry and threadgroup memory its production
encoder does** - a change to anything a kernel dispatches with, binds, or reads from its
kargs updates every gate that hand-binds that kernel in the same change, and a new in-body
branch keyed on a kargs field needs a gate cell that arms the field (the mechanism - why a
missed tgmem fails silently - is `CLAUDE.md`'s "Arm filter mechanics" section, the
kernel-gate paragraph).

**A new pre-tokenizer family or backend ships its `corpus_case` arm in `test_tokenizer.das`,
naming the `ggml-vocab-*.gguf` fixture.**

**A `corpus_case` arm asserts exact reference ids AND lossless round-trip.**

**Every test that compares generated tokens, ids, or logits logs a human-readable form of
BOTH sides: the decoded text where the model carries a vocab (`log_gen_texts` in
`_model_tier.das`, or one line per side), otherwise each side's argmax index and value.** A
red, or a suspicious green, must be readable in the log, not only as an id or float
difference.

**A GPU kernel whose MSL entry symbol the census in `test_kernel_coverage.das` has not seen
before ships with a small model in that file's coverage suite** that dispatches it.

**A kernel-unit arm whose property a CPU oracle can witness compares its kernel against that
oracle.**

**A kernel-unit arm testing cross-dispatch bit-identity - a property no CPU oracle can
witness - compares GPU against GPU with its output buffers prefilled with a sentinel.**

**A kernel-unit arm whose output plane is its input plane, and whose CPU oracle does not
differ from the input by construction, pairs its compare with an assert that the output
differs from the input at a known index** - an in-place kernel that never ran leaves the
input, which can wrongly satisfy a tolerant compare.

**Every ASR family has a token-for-token oracle cell** - one comparing a transcript against a
reference leg, external dump or CPU control alike.

**A stocked artifact a test in this folder loads - a model file, mmproj, image fixture, or
oracle dump - has a row in `../performance/model_specs.das` (or `asr_catalog` in
`../performance/profile_common.das`), rides a row's `companions` list, has a convert script
checked in beside the table in `../performance/`, or (an oracle dump) has a mint script that
regenerates it, beside the dumps under `models_dir()`, named in the test that loads the
dump.**

**A test that reads a vision encode oracle dump names the minting arm in its header - the
backend, the flash-attention setting, and the mmproj precision the dump came from.**

**A cell establishes every driver hook and serving-lane knob its claim depends on, and
restores it before returning** - a hook here is any process-wide setter with no read-back,
so a cell that pins one OFF sets it back ON, and a cell whose claim needs the DEFAULT
establishes that default too (the environment can carry the knob either way);
a family serving-lane pin `set_<family>_q8` is undone with `reset_<family>_q8` - never a
runtime decline standing in for a pin. The mechanism (why the hooks flip legs silently) is
`CLAUDE.md`'s "Metal fixtures" section.

**A knob a cell can reach only through the environment is armed in the environment of a
process started after the arming - a child the cell spawns, or the runner's own - and needs
no restore; a cell that cannot arm it names in its assert text the value it asserts under.**
An in-cell set is invisible to the running config, which is read once at context init.

**A cell asserting the UNPINNED default lane compares against the predicates the lane policy
itself consults - `float_batch_override_active()` and the family's `<family>_gpu_would_serve()`
where one exists - never against a hardcoded lane**; the accelerate tier and the GPU tower move the
default per box, and the assert is on the lane the policy selects, not on one predicate's
own value.

**A cell that encodes, preprocesses, or asserts on media bytes an encoder consumes - pixels
or audio samples, not a `.dlim` model image - with no model loaded builds its fixture
procedurally and pins its expectations in-repo.**

**Any media a test feeds an embedder - an image or an audio clip - is a fixture the test
builds, or is previewable via `DASLLAMA_VISION_DUMP` (images)** - a red never requires adding
instrumentation before a human can see what the model consumed.

**A tier-1 media fixture - one an embedder-parity cell regenerates in-test and compares
against an oracle dump - has an exact-value generator.** A generator running libm
transcendentals is not float-portable and its cell is a defect; orientation coverage uses
shaped exact fixtures.

**An embedding-parity cell names its fixture and logs the measured maxdiff on green as well
as red.**

**A new gate, or a new or loosened tolerance bar, ships a control that reds it - a poison, a
knockout, a disconnected mechanism, or a cross-lane witness - in the same change.** A bar
nothing has ever exceeded is not known to discriminate, and a gate that reads state the same
code path wrote can be a tautology - only the control proves either can fail.

**A family that gains a live thinking or tool format ships its recognition tests in the same
change** - the wire-shape pins, the render pins, and a live server leg gated on the family's
smallest GGUF that runs on the small tier (the file homes are `CLAUDE.md`'s "Model-free /
no-arm tests" and "Out-of-folder test files" notes). A family whose vocab lacks the markers
has no format to test.

