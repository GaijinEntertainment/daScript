# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_RUNTIME.md`,
`ARCHITECTURE_MEASUREMENT.md` (routed checklists own the other companions). Planned work:
`followup_general.md` (rig and instrument rows included), `followup_vulkan.md` (engine work on
the Vulkan tier), `followup_metal.md` (engine work on the Metal tier, or CPU engine work
measured on macOS), `PERF_LEDGER.md` (performance; the rest goes to the followup ledgers).

**A dasLLAMA `[test]` file, wherever the diff puts it, answers to this module's
`tests/REVIEW.md`.**

**A timing rig (a file that times a run itself and reports a wall-clock time or rate as its
result, printed or returned to a caller that prints it - a driver reading a child's clock is
not one), a kernel race (a run timing two kernel variants - arms - against each other in one
process), or a function a `benchmarks/lcpp_bench.das` cell's timed body calls, wherever it
lives, answers to this folder's `benchmarks/REVIEW.md` beside its own folder's checklist.**

**A diff that writes a measured number down - into `PERF_LEDGER.md`, a checked-in doc, a
code comment, checked-in data a run produced, or a PR body - or adds a serving path or moves
an existing one onto other code, or changes what a measured or served run with no flags and no
environment overrides computes, applies `REVIEW_MEASUREMENT.md`.** A serving path is the
end-to-end route a run takes from prompt to tokens; its compile tier (interpreted, JIT, AOT)
and its cross target (a build for another platform) are part of it.

**A change to what enters `performance/records/`, or to a provenance manifest, answers to
`performance/REVIEW.md`.** A change to WHICH model file a recorded row or a manifest pins
answers to it too. A model file here is a `.gguf`, a `.dlim`, an mmproj, or an image or audio
fixture. A test or tool merely opening a stocked model file by name does not route.

**A change to the sidecar-exchange client (`dasllama/dasllama_exchange.das`) - the code that
downloads tune winners to a box and submits that box's winners back - its schema, or a
tune-boot path that reaches it, applies `performance/REVIEW.md` and `REVIEW_EXCHANGE.md`.**

**A diff that adds a module under `dasllama/` whose changes reach some of `tests/run.das`'s
areas but not all - `audio`, `vision`, `tts`, `llm`, `infra` - gives it a `MODULE_AREAS` row
naming those areas, in the same change.** A module with no row reaches every area, so the
omission costs every later `run.das -- --changed` the whole suite, never coverage.

**Every `dasllama/` change applies this folder's `tests/REVIEW.md` - open it explicitly: the
folder walk does not surface it for a `dasllama/`-only diff.**

**A GPU kernel, driver, dispatch class (a class a `[metal_dispatch]` or `[vk_dispatch]`
declares), or K/V-mirror (the device-side copy of the key/value cache a GPU decode reads and
writes) change - and a GPU kernel A/B race, a knockout (an arm that skips a stage to measure
that stage's cost), or a hand-binding arm (one that writes buffer or kargs (kernel-argument
struct) binding numbers as literals instead of taking the kernel class's declared ones),
wherever the diff puts it - applies `REVIEW_GPU.md`.**

**A kernel body or a function a kernel calls - a `[metal_kernel]` def, a class a
`[metal_dispatch]` / `[vk_dispatch]` declares, or a fixture either emitter compiles - wherever
the diff puts it, applies `modules/REVIEW_SHADER_EMITTERS.md` (repo root) too.**

**A change to the image rail - `dasllama/dasllama_image.das`, or, wherever the diff puts it,
a `.dlim` mint (building a `.dlim` from a gguf), a `.dlim` load, an image identity, or a
flavor (the backend-and-layout variant an image is baked for, one part of its identity) -
applies `REVIEW_IMAGE.md`.**

**A change to `dasllama/dasllama_audio.das`, `dasllama_audio_io.das`, `dasllama_audio_embedder.das`,
`dasllama_asr.das`, `dasllama_asr_types.das` or `dasllama_vad.das` (all under `dasllama/`), or to
an ASR family file - one `dasllama/dasllama_<family>.das` holding a single speech-recognition
family - applies `REVIEW_AUDIO.md`.**

**A change to `dasllama/dasllama_vision.das`, `dasllama/dasllama_vision_io.das`,
`dasllama/dasllama_vision_embedder.das`, a vision family file - one
`dasllama/dasllama_<family>.das` holding a single vision projector family - or an in-process
path (one that runs inside the program under review, not a spawned child process) that
splices a stream carrying decoded media - pixels or audio samples - into a prompt or
schedules such a stream, applies `REVIEW_VISION.md`.**

**A `dasllama/dasllama_tower.das` change - the shared encoder-tower home - applies
`REVIEW_AUDIO.md` and `REVIEW_VISION.md`;** a family file that only CALLS a shared tower
function does not thereby pick up the other modality's checklist.

**A change to `dasllama/dasllama_tts.das`, `dasllama/dasllama_tts_types.das`,
`dasllama/dasllama_tts_blocks.das`, `dasllama/dasllama_styletts2.das`, a TTS family file - one
`dasllama/dasllama_<family>.das` holding a single speech-synthesis family - a text front-end
file - one stage of the pass that turns text into phonemes (`dasllama/dasllama_textnorm.das`,
`dasllama/dasllama_postag.das`, `dasllama/dasllama_g2p.das`) - the front-end packs' mint
(`harness/build_g2p_data.py`, `harness/train_postag.py`, `harness/mint_postag_silver.py`,
`performance/build_tts_data.das`), the Pocket converter and its card (`harness/convert_pocket.py`,
`harness/tts_model_card.md`), or a call that pins a TTS weight lane (`set_tts_q8` /
`set_styletts2_q8` / `set_pocket_q8`), wherever the diff puts it, applies `REVIEW_TTS.md`.**

**A diff that adds a file under `dasllama/`, or adds or moves a def, a `require`, or a module
global in a file under `dasllama/`, applies `REVIEW_PLACEMENT.md`** - the what-lands-where rules.

**A `[test]` file that requires any `dasllama/*` module and sits under `modules/dasLLAMA/`
outside `tests/` (beside this file) is a defect - move it into `tests/`.**

**`DASLLAMA_RELEASE` (`dasllama/dasllama_version.das`) is bumped only on a declared release -
a maintainer ruling that bench comparability is broken.** Recorded performance rows and tune
sidecars stay valid across code changes, and per-change invalidation lives in the finer
mechanisms - `IMAGE_VERSION` and `layout_fingerprint()` (`dasllama/dasllama_image.das`).

**A value that cannot change between dispatches of one compiled kernel never reaches that
kernel as a uniform, a kargs field, an `@off` bind offset, or a helper parameter - stamp it
into the class as a `@template_constant` instead.**

**A function-typed global a serialized exe must re-establish lands in a `dasllama/` file with
the `[init]` that establishes it at boot; landing one where `REVIEW.das`'s restore-check walk
over `dasllama/` cannot reach it is a defect.** A serialized exe restores globals as data, so
a declaration initializer arrives null and dies at the first invoke while every `-jit` gate
stays green; a global another file's `[init]` arms (`set_runtime_race_hook`) has no
initializer, and its null default is the declared "no hook".

**Never reorder or merge the float multiplies in a function that builds a RoPE angle table
(`dasllama/dasllama_rope.das`).** A regrouping moves the angles in the last bits and flips
token-exact fixtures.

**A diff that changes a predicate in `dasllama/` picking between kernel forms that both produce
the right answer, or a constant there chosen by timing two candidates against each other, rests
on timing that ran both forms interleaved in one process under one instrument, and puts that
race's rows, each naming its arm, in the PR body.** A reading taken across two processes or two
commits says which way the wall clock moved, not which implementation to adopt.

**A change to an allocation reached from a load, bake, or convert path (judge a shared helper
at each call site) that trades footprint against wall-clock, either way, ships the measured
pair - peak footprint and wall-clock - in `PERF_LEDGER.md` with the decision it settles.**

**A new call to an f32 matmul (`matmul_batch`, `mm_blob_b`, per-head `gemm_f32` /
`gemm_f32_jo`, or an f32 GPU mm) outside a correctness-comparison path (one whose only job is
to produce a reference result to check another against), where a faster-format twin already
serves the same weights and shape, is a defect - call that twin instead.** A site that must
stay f32 for another reason is ledgered on its own file's sec.1 charter line in an
`ARCHITECTURE_*.md` companion, not commented into compliance.

**A boot-path prompt (code that runs at startup, before the first request) that reads stdin
without first proving both stdin and stdout are terminals is a defect - emit the question as
a `@sidecar` event instead.** A supervised or piped boot must never block on input.

**A print or log of an elapsed interval whose site is in an engine file (`dasllama/`), outside a
cold one-shot load, bake, map or tokenizer-build progress log and the fat exe's first-start
kernel race report, is a defect** - instrumentation goes through the
profiling rails (`profile_tag` / `profile_marker`, `prof_add`, `asr_prof_add`, the Vulkan
tier's `vk_prof()`-gated ledgers), `ARCHITECTURE_MEASUREMENT.md` sec.2.10.

**In an engine file (`dasllama/`), a clock value that changes what the program DOES - control
flow, eviction, a generated name; not a reported wall-clock time or a best-of reduction over
reported wall-clock times - is marked `// clock: control`** - unmarked, it cannot be told
apart from the ad-hoc profiling an engine file may not carry.

**Every new kernel or loop the runtime re-enters per token, per frame, or per prefill
quantum - one batch of prompt tokens the prefill path processes in a single pass - is reached
by an annotated region entry: `[hot_path]`, any of the `[no_alloc]` / `[no_env]` / `[no_io]`
contracts, or `[cold_path]` on its only reaching entry; a renamed per-token function is not
new, and its annotation moves with the name in the same change.** An annotation binds every
function the entry calls; an unreached loop has no contract (`ARCHITECTURE_RUNTIME.md` sec.2.11).

**The annotation sits on the region entry - the outermost function the runtime re-enters per
token, per frame, or per prefill quantum: a kernel `*_encode` / `*_decode`, a step driver, the
CPU decoder's `forward_*` entries, or a function reached only through a registered function
value - and an interior function carries an annotation only when it is a `[cold_path]` on a
rarely-taken branch (a guard that logs once).**

**A loop reached only from a load, stage, bake, or convert path is not a region entry: it
carries no `[hot_path]` and none of the `[no_alloc]` / `[no_env]` / `[no_io]` contracts; a
`[cold_path]` may sit on it.**

**A driver that calls the `forward_*` entries and is reached only by a measurement - a
benchmark row, a rig's loop - and never by a served request carries `[cold_path]`, which
covers only the functions below it that carry no annotation of their own.**

**A change to `encode`/`bpe_encode`, or to a function they call, in
`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das` / `dasllama/dasllama_pretok.das`,
ships before/after `--tok` rows (this folder's `benchmarks/lcpp_bench.das`) at two or more
input sizes, for a model using the affected tokenizer.** A change confined to the load path -
a metadata default the encode reads as a value - does not fire this rule.

**A tokenizer wall-clock time that grows faster than linearly with input size is a defect.**

**A change to code or data in `dasllama/dasllama_tokenizer.das`, `dasllama/dasllama_spm.das`,
`dasllama/dasllama_bpe.das`, or `dasllama/dasllama_pretok.das`, or to the special-token or
template strings any of them look up, records a run of this folder's
`tests/test_tokenizer.das` with its cases EXECUTED, not skipped.**

**A diff that adds an override, or gives one a new effect, without the announce is a defect.**
An override is an environment knob, an exported runtime setter, or an on-disk state file - one
a run writes or a user places, never data a build ships - that moves a gate, policy, or
threshold off its default and so changes what the run writes, reads, mints, or computes (a
timing knob is one only when it moves a number other than a measured time); a CLI flag never
is one. The announce is a line printed where the override changes the outcome, naming it by
the spelling a user would set (the env variable, the sidecar or file key, the setter's name)
and, for one that is on unless turned off, the spelling that turns it off; one with no off
spelling says so. Per-site repeats are fine; a set-but-inert override is silent.

**A tutorial source, `.rst` page, docstring, help string, `README.md`, or checked-in document
outside this folder left showing the old call, flag, or default after a change to user-facing API
is the change's defect, not the docs'.** User-facing is anything a consumer outside this repo can
depend on - what it calls, types, requires or parses (facade functions, CLI flags, environment
knobs, file formats, defaults, what the installed SDK lets a program `require`) - plus the in-repo
rig and tool surface: any output another tool parses. A console-only diagnostic is not user-facing.

**A diff that falsifies a statement in checked-in text under this folder - docs, `//!` docstrings,
`//` comments, or string data, any language - or in a document outside this folder whose own
checklist routed this diff here, updates that text in the same change** - no lint reads text no
`[arch]` cites; only the reviewer does.

**Weakening `dasllama_lint` (`dasllama/dasllama_lint.das`) - the compile-time check that a
consumer requires only this module's public entry modules, matched by the resolved file's
path under `modules/dasLLAMA/` - is a defect:** the path match dropped or narrowed, an error
text that no longer names the facade to require instead, or a module added to its allowed set
without both halves of the pair that makes it an entry module - the `ARCHITECTURE_ENGINE.md`
sec.1.8 charter line naming it a sanctioned public entry point, and the DASLLAMA001 error text
naming it beside the facade. The allowed set is the table in the lint.

**A STYLE037 or STYLE038 suppression, `// nolint:` or the file's `options _function_length` /
`_cyclomatic_complexity`, on a function a follow-up ledger entry says can be shortened or split
is a defect - land the ledgered split instead.** The warning keeps the entry visible; an entry
asking that twin bodies merge into one template does not fire this rule.

**`options _dasllama_internal` belongs only in a file whose job is to reach engine
internals: an engine file under `dasllama/`, a test, harness, benchmark, or rig this module
owns, or a consumer `ARCHITECTURE_ENGINE.md` sec.1.8 names as ruled** - a symbol the facade
lacks is added to `dasllama/dasllama.das`, not obtained by adding this option to the consumer. A
`require ... public` that re-exports an engine module OUT of a file carrying this option,
beyond what that consumer's ruled charter grants, breaks this rule too.

**Weakening `REVIEW.das` (beside this file) is a defect:** dropping a check, rewriting a
finding text so it no longer names what failed, or adding a name to a check's licensed set -
the names that check does not flag. A name joins a licensed set only when that check's own
finding text names the set as its extension point and the diff lands the paired architecture
edit that text asks for. What the gate enforces is read from the gate itself; each check's
finding text states its own rule.

**A new `REVIEW.das` check ships its line in the companion section that owns the mechanism the
check guards - an `ARCHITECTURE_*.md` companion, never `ARCHITECTURE.md` - in the same change.**
The line names the check and the names it licenses. A licensed name is one that check does not
flag. When the check licenses no names, the line says so.

**Checked-in text under `modules/dasLLAMA/` - docs, comments, or string data, any language -
that describes a mechanism of the reference build, or names that build, its binaries or its
symbols, wherever the diff puts it, applies `REVIEW_UPSTREAM.md`.** The reference build is the
third-party engine this module measures itself against - `benchmarks/setup_lcpp_ref.das` pins it.

**A diff that changes what authoring a new weight format entails - a step added or dropped, a
file the author must touch, a fixture or probe entry the format must supply, or a gate it must
pass - updates `HOW_TO_ADD_A_FORMAT.md` in the same change.** The how-to is the next format
author's whole brief: a step dropped there is a step the next format silently skips.

**Legal attribution - a third party's copyright line, licence name, or licence text - lives in
`THIRD_PARTY_NOTICES.md`, the `LICENSE.*` files, a model card (the provenance-and-licence page
published beside a released model or pack), or a ledger row naming a licence as a reason to
adopt or reject a model, a dataset, or a dependency; anywhere else in prose it is a defect.**

**A def of a facade file - one whose defs reach a consumer through `require dasllama/dasllama`;
`REVIEW.das`'s `FACADE_FILES` is the list - and a new OVERLOAD of one, is TAUGHT: demonstrated
in runnable code in a `tutorials/dasLLAMA/*.das` source and narrated on a
`doc/source/reference/tutorials/dasLLAMA_*.rst` page.**
`REVIEW.das`'s `check_tutorial_floor` matches def NAMES only, so an overload passes on a
sibling's tutorial - the reviewer confirms a tutorial calls the NEW signature, and a mention
that only names it (a comment, a passing reference) does not count.

**A diff that makes another file's defs reach a consumer through `require dasllama/dasllama`
adds that file to `REVIEW.das`'s `FACADE_FILES` in the same change.**

**A NEW `[EnvConfig]` area struct is rendered by `env_markdown()` in the same change.** A struct
the renderer never emits is absent from `ENVIRONMENT.md` and every test; one it emits but the
registry does not, `tests/test_env_registry.das` catches.

**Hand-editing `dasllama/dasllama_unicode.das`'s RANGES/WS tables is a defect - regenerate them
by retranscoding `$LCPP/src/unicode-data.cpp` (the reference checkout) instead.**

**A diff that adds a file under `dasllama/`, adds or moves a def, a `require`, or a module
global in a file there, or gives a file a weight format, a serving lane (the quant form a
tensor serves from) or a data structure its charter does not name, lands the sec.1 edit that
keeps the charters true - in an `ARCHITECTURE_*.md` companion, never `ARCHITECTURE.md` - in the
same change.** A file added beside files with their own sec.1 charter lines gets one too; a
module-root ledger has none.

**A follow-up ledger row whose work landed in this change is deleted, and the rows below it
keep their numbers** - checked-in text cites rows by number, and no lint follows the citations.

**A diff that adds, removes, or moves a section of an `ARCHITECTURE_*.md` companion, or adds
or removes a companion, lands `ARCHITECTURE.md`'s index line and section range, the
companion's own opening (its range and the sections it names), and every repointed prose
`sec.N` / file citation of the moved sections, in the same change.** Prose citations are not
LINT026-gated, so one naming a section that left its file sends the reader to nothing.

**A diff that moves a family encode stage onto a GPU hook leaves the CPU form in place and
changes none of its arithmetic.** The CPU form serves every box with no driver.

**A diff that writes a CPU feature name in a `[tune_perm]` `requires=` argument that
`TUNE_KNOWN_FEATURES` (`modules/dasLLVM/daslib/llvm_tune.das`, repo root) does not list adds it
there in the same change.** The `features` fingerprint saved with every sidecar is this box's
pass/fail over that list, so a name outside it is never recorded and a box adopting a shipped
profile re-runs the tuning the profile was meant to save.

**A value that a team-lane kernel reads - anything reachable from a `team_parallel_for` /
`team_parallel_for_indexed` / `team_parallel_stages` body (`daslib/jobque_boost.das`, repo
root) - is a `def` returning it, never a module global with a declaration initializer (`let`
or `var`).** A team lane never runs global initializers, so the global reads zero there.

**A `resize` in `dasllama/` of a buffer whose element count scales with a model dimension (a
count the model file sets: layers, dim, experts, vocab, positions) is preceded by a `reserve` of
the same count - a `dasllama/dasllama_common.das` sizing helper that reserves before it grows
(`reserve_resize`, `grow_resize`, `ensure_length`, `overwrite_resize`, `zeroed_resize`), the
builtin `scratch_resize` on a `@scratch` carrier, or the pair spelled out - whatever the size
looks like at today's shapes.** Such a count is unbounded, and a bare grow past the heap's
unreserved-size cap (64 MB) panics the load on the first big model rather than at the call site.
