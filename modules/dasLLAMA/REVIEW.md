# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_MEASUREMENT.md` (routed
checklists own the other companions). Planned work:
`followup_general.md` (rig and instrument rows included), `followup_vulkan.md` (engine work on
the Vulkan tier), `followup_metal.md` (engine work on the Metal tier, or CPU engine work
measured on macOS), `PERF_LEDGER.md` (performance; the rest goes to the followup ledgers).

**A change to the tokenizer - `dasllama/dasllama_tokenizer.das`, `dasllama/dasllama_spm.das`,
`dasllama/dasllama_bpe.das` or `dasllama/dasllama_pretok.das`, or the special-token or template
strings any of them look up - applies `REVIEW_TOKENIZER.md` (beside this file) too.**

**A diff that touches a `followup_*.md` or an `ARCHITECTURE*.md` under this folder, adds a file
under `dasllama/`, or adds a STYLE037/STYLE038 suppression anywhere under this folder applies
`REVIEW_DOCS.md` (beside this file) too.**

**A diff that adds, changes or drops a check in a `REVIEW.das` under this folder, or adds a name
to a check's licensed set - the names a check does not flag - applies `REVIEW_GATES.md` (beside
this file) too.**

**A diff that removes the problem a `followup_*.md` row states, by the row's fix or another,
deletes the row - or, when the row lists several items, only the item it resolved - and repoints
every checked-in citation naming that item to where the fact now lives (the architecture doc or
the code) or drops it, dated `PERF_LEDGER.md` entries included.**

**Code that times a run itself and hands the wall or rate back as its result - a file that prints
it, or a function that returns it to whichever file calls it - a kernel race (a run timing two
kernel variants - arms - against each other in one process), or a file `benchmarks/lcpp_bench.das`
requires directly, wherever it lives, answers to this folder's `benchmarks/REVIEW.md` beside its
own folder's checklist.** A driver reading a child's clock is not one.

**A diff that writes a measured number down - into `PERF_LEDGER.md`, a checked-in doc outside
`site*/` (repo root), a code comment, checked-in data a run produced, a commit message, or a PR
body - or adds a serving path or moves an existing one onto other code, or changes what a
measured or served run with no flags and no environment overrides computes, applies
`REVIEW_MEASUREMENT.md`.** A serving path is the end-to-end route a run takes from prompt to
tokens; its compile tier (interpreted, JIT, AOT) and its cross target (a build for another
platform) are part of it.

**A diff that adds a kernel, loop or call path the runtime re-enters once per serving step - a
token, a prefill quantum (one batch of prompt tokens the prefill path processes in a single
pass), one encoded media input (an image, a video frame, an audio chunk), or one synthesized
speech chunk or frame - adds, moves, renames or removes a `[hot_path]`, `[cold_path]`,
`[no_alloc]`, `[no_env]` or `[no_io]` annotation, or adds or changes a test, harness, benchmark
or performance-rig function that reaches a region entry (the outermost function re-entered each
serving step), wherever the diff puts it, applies `REVIEW_HOT_PATH.md` (beside this file)
together with this list.**

**A diff that adds an allocation, changes an allocation's size formula, or adds, changes or
drops a `resize` or an `@exact_size` on a buffer; that adds or changes a module global, a call
to a function-typed one, or the `[init]` that sets one; or that adds or changes code a job runs
in a forked context or code reachable from a `team_parallel_*` or `maybe_parallel_for*` body,
wherever the diff puts it, applies `REVIEW_MEMORY.md` (beside this file) too.**

**A change to what enters `performance/records/`, or to a provenance manifest, answers to
`performance/REVIEW.md`.** A change to WHICH model file a recorded row or a manifest pins
answers to it too. A model file here is a `.gguf`, a `.dlim`, an mmproj (a multimodal projector
weight file), or an image or audio fixture. A test or tool merely opening a stocked model file
by name does not route.

**A change to the sidecar-exchange client (`dasllama/dasllama_exchange.das`) - the code that
downloads tune winners to a box and submits that box's winners back - its schema, or a
tune-boot path (a startup path that loads a tune sidecar) that reaches it, applies
`performance/REVIEW.md` and `REVIEW_EXCHANGE.md`.**

**A diff that adds a module under `dasllama/` whose code only some of `tests/run.das`'s areas
(`audio`, `vision`, `tts`, `llm`, `infra`) run in their tests gives it a `MODULE_AREAS` row
naming those areas, in the same change.** `run.das -- --changed` maps a module with no row to
every area, so a missing row costs every later run the whole suite.

**A diff after which an area's tests run an existing module's code, and that module's
`MODULE_AREAS` row omits the area, adds the area to the row in the same change.** A new caller of
the module counts, wherever it sits. A row missing an area makes `run.das -- --changed` skip that
area's tests, so a regression there goes unrun.

**A dasLLAMA `[test]` file, wherever the diff puts it, and every `dasllama/` change answer to this
folder's `tests/REVIEW.md` - open it; the walk does not surface it for a `dasllama/`-only diff.**

**A GPU kernel, driver, dispatch class (a class a `[metal_dispatch]` or `[vk_dispatch]`
declares), or K/V-mirror (the device copy of the K/V cache a GPU decode reads and writes)
change, a change to a `kv_*` function that a GPU driver file (`dasllama_metal*.das`,
`dasllama_vulkan*.das`, `dasllama_gpu*.das`) calls, a GPU kernel timing race (two kernels timed against each other to pick one - not a data
race), a call that makes, arms or tears down device-home serving - a session whose K/V region
lives only on the device (`create_device_session`, `set_device_kv`, `moe_gpu_drop_model`) - a
knockout (an arm that skips a stage to measure that stage's cost), a hand-binding arm (one that
writes buffer or kernel-argument (kargs) binding numbers as literals), or a kernel cell or probe
that fills or binds a `TokMeta` block, wherever the diff puts it - applies `REVIEW_GPU.md`.**

**A kernel body or a function a kernel calls - a `[metal_kernel]` def, a class a
`[metal_dispatch]` / `[vk_dispatch]` declares, or a fixture the Metal or SPIR-V emitter
compiles - wherever the diff puts it, applies `modules/REVIEW_SHADER_EMITTERS.md` (repo root) too.**

**A change to the image rail - `dasllama/dasllama_image.das`, or, wherever the diff puts it, a
`.dlim` mint (building a `.dlim` from a gguf), a `.dlim` load, an image identity, or a flavor
(the backend-and-layout variant an image is baked for, one part of its identity) - applies
`REVIEW_IMAGE.md`.**

**A change to `dasllama/dasllama_audio.das`, `dasllama_audio_io.das`,
`dasllama_audio_embedder.das`, `dasllama_asr.das`, `dasllama_asr_types.das` or `dasllama_vad.das`
(all under `dasllama/`), or to an ASR family file - a `dasllama/` file holding one
speech-recognition family's CPU model - applies `REVIEW_AUDIO.md`.**

**A change to `dasllama/dasllama_vision.das`, `dasllama/dasllama_vision_io.das`,
`dasllama/dasllama_vision_embedder.das`, a vision family file - one `dasllama/dasllama_<family>.das`
holding a single vision projector family - an in-process path (one that runs inside the program
under review, not a spawned child process) that splices a stream carrying decoded media - pixels or
audio samples - into a prompt or schedules such a stream, or a change to how a prefill driver
serves, declines, or passes to the CPU a media span - the rows of one image or audio turn inside a
single model call - applies `REVIEW_VISION.md`.**

**A `dasllama/dasllama_tower.das` change - the backend-independent encoder-tower home, not a
backend tower driver - applies `REVIEW_AUDIO.md` and `REVIEW_VISION.md`.**

**A change to a file `dasllama/dasllama_tts*.das` matches, `dasllama/dasllama_styletts2.das`,
a TTS family file - a `dasllama/dasllama_<family>.das` holding exactly one voice model's code,
never the two-family carrier `dasllama/dasllama_styletts2.das` - a text front-end file - one stage
of the pass that turns text into phonemes (`dasllama/dasllama_textnorm.das`,
`dasllama/dasllama_postag.das`, `dasllama/dasllama_g2p.das`) - the front-end packs' mint
(`harness/build_g2p_data.py`, `harness/train_postag.py`, `harness/mint_postag_silver.py`,
`performance/build_tts_data.das`), the Pocket converter and its card (`harness/convert_pocket.py`,
`harness/tts_model_card.md`), or a call that pins a TTS weight lane (`set_tts_q8` /
`set_styletts2_q8` / `set_pocket_q8`), wherever the diff puts it, applies `REVIEW_TTS.md`.**

**A diff that adds a file under `dasllama/`, or adds or moves a def, a class, a module global or
named constant, or a `require` in a file under `dasllama/`, applies `REVIEW_PLACEMENT.md`** - the
what-lands-where rules.

**A diff that adds or changes a def in a file `REVIEW.das`'s `FACADE_FILES` lists, or adds
`public` to a require, new or existing, in `dasllama/dasllama.das` or in a file it reaches
through `public` requires alone, or adds an `[EnvConfig]` area struct, applies
`REVIEW_FACADE.md` too.**

**A diff that turns a weight-format id - a `KqFmt` member, a GGUF type number, or the int a
generated kernel takes as its format parameter - into plane strides, or reads a per-block or
per-element byte count of one format, wherever it sits, applies `REVIEW_KQ_FORMATS.md`.**

**A diff that bumps `DASLLAMA_RELEASE` (`dasllama/dasllama_version.das`) cites in the PR body the
maintainer's ruling that rows measured before it can no longer be compared with rows after it.**
Every recorded row, tune sidecar and exchange entry carries the release, so a bump voids them all.

**A change that invalidates only images never bumps `DASLLAMA_RELEASE` - it applies
`REVIEW_IMAGE.md`.**

**A function in `dasllama/dasllama_common.das` that performs work through a hook another module
registers runs its own CPU code for that work when the hook is unset; when it has no CPU code
for that work, it panics with a message naming the module to require.** A function with no
fallback that returns quietly hides which registration a program root forgot.

**A function in `dasllama/dasllama_common.das` that reports whether a hook another module
registers is installed returns false when the hook is unset - never a panic.**

**Weakening the token-exact RoPE fixtures - the tests that pin the angle tables
`dasllama/dasllama_rope.das` builds - is a defect.**

**Weakening `REVIEW.das`'s `check_verify_decline_before_state_move` is a defect.**

**Weakening `REVIEW.das`'s device-creation check is a defect - a device- or queue-creating call
spelling missing from `DEVICE_CREATION_CALLS` weakens it.**

**Weakening `check_ple_gather_sites` in `REVIEW.das` is a defect.**

**A diff that makes any choice in `dasllama/` because one candidate measured faster - a constant
set to a value, a formula that gains or drops a term and a predicate that picks among kernel
variants computing the same result included - takes the winner from a race that timed every
candidate interleaved in one process with one script, each candidate selectable in that script at
the diff's tip by a flag or argument, never by editing the source between runs, and puts that
race's rows, each naming its candidate, in the PR body or the change's dated
`PERF_LEDGER.md` row.** Timings taken in two processes or at two commits also differ by everything
else that changed between the runs, so they cannot pick a candidate.

**A new call that runs a matrix multiply over f32 weight rows - `matmul_batch`, `mm_blob_b`,
`mm_fblob_b`, per-head `gemm_f32` / `gemm_f32_jo`, or an f32 GPU mm - outside a
correctness-comparison path (one whose only job is to produce a reference result to check another
against), where a faster-format twin on the same backend already serves the same weights and
shape, is a defect - call that twin instead.** A site that must stay f32 for another reason is
ledgered on its file's charter line (the line, in the companion `ARCHITECTURE.md#file-charters`
routes to, that says what the file holds); a comment at the call site does not discharge this.

**A caller never re-checks a guard its callee checks - drop the caller's copy, or, where the
caller's check also gates its own work, compute the decision once and pass it to the callee.**
An edit to either copy leaves the caller testing a condition the callee no longer applies.

**Logic or a named constant that two files in one folder both use, and that neither the language
nor the test contract forces them to restate (an enum-and-int pair of one predicate, a test's CPU
oracle of the arithmetic), lands once - in a file both already require, or in a new file both
require when they share none - never as a second copy.** Two spellings drift apart on the first
edit to one.

**A piece that two folders both need, neither containing the other, lands in the folder that owns
the concern, and the other folder requires it - never a copy in each.**

**A boot-path prompt (code that runs at startup, before the first request) that reads stdin
without first proving both stdin and stdout are terminals is a defect - emit the question as
a `@sidecar` event instead.** A supervised or piped boot must never block on input.

**A print or log of an elapsed interval whose site is in an engine file (`dasllama/`), outside
the profiling rails (`profile_tag` / `profile_marker`, `prof_add`, `asr_prof_add`, the Vulkan
tier's `vk_prof()`-gated ledgers and logs), a cold one-shot load, bake, map or tokenizer-build
progress log, or the first-start kernel race report of a fat exe (one built `DAS_TUNE_MODE=fat`,
shipping its tune profile), is a defect - route it through a rail
(`ARCHITECTURE_MEASUREMENT.md#sanctioned-instrumentation-rails`).**

**In an engine file (`dasllama/`), a clock value that changes what the program DOES - control
flow, eviction, a generated name; not a reported wall-clock time or a best-of reduction over
reported wall-clock times - is marked `// clock: control`** - unmarked, it cannot be told
apart from the ad-hoc profiling an engine file may not carry.

**A diff that adds an override, or changes what one does - a value it now clamps or ignores
included - without the announce is a defect.** An announce is the line the run prints where the
override changes the outcome. An override is an environment knob, a runtime setter the facade
(`dasllama/dasllama.das`) exports, a command-line flag of a dasLLAMA tool, or
an on-disk state file - one a run writes or a user places, never data a build ships - that moves
a gate, policy, or threshold off its default and so changes which code the run takes or what it
writes, reads, mints, or computes. A measured time, the run's own duration, or a different
moment at which the same work happens is not such a change; a CLI flag is never an override.

**An announce names the override by the spelling a user would set - the env variable, the sidecar
or file key, the setter's name - and, for one on unless turned off, the spelling that turns it off
(none: it says so).**

**A tutorial source, `.rst` page, docstring, help string, `README.md`, or any other checked-in
document, all outside this folder, left showing the old call, flag, default, or stated behaviour
after a change to user-facing API is the change's defect.** User-facing is anything a consumer
outside this repo can depend on - what it calls, types, requires or parses (facade functions, CLI
flags, environment knobs, file formats, defaults, what the installed SDK lets a program
`require`) - plus the in-repo rig and tool surface: any output another tool parses. A
console-only diagnostic is not user-facing.

**A diff that falsifies a statement in checked-in text under this folder - docs, `//!` docstrings,
`//` comments, or string data, any language - or in a document outside this folder whose own
checklist routed this diff here, updates that text in the same change** - no lint reads text no
`[arch]` cites.

**Weakening `dasllama_lint` (`dasllama/dasllama_lint.das`) - the compile-time check that a
consumer requires only this module's public entry modules, matched by the resolved file's
path under `modules/dasLLAMA/` - is a defect:** the path match dropped or narrowed, an error
text that no longer names the facade to require instead, or a module added to its allowed set
without both halves of the pair that makes it an entry module - the `ARCHITECTURE_ENGINE.md`
charter line naming it a sanctioned public entry point, and the DASLLAMA001 error text
naming it beside the facade. The allowed set is the table in the lint.

**`options _dasllama_internal` belongs only in a file whose job is to reach engine
internals: an engine file under `dasllama/`, a test, harness, benchmark, or rig this module
owns, or a consumer `ARCHITECTURE_ENGINE.md#instrumentation-and-support` names as ruled** - a
symbol the facade lacks is added to `dasllama/dasllama.das`, not obtained by adding this option to
the consumer.

**A file whose entry under `ARCHITECTURE_ENGINE.md#instrumentation-and-support` rules its
`options _dasllama_internal` re-exports an engine module with `require ... public` only where
that entry names the re-export; any other re-export is a defect - the file's own requirers reach
the symbol through `dasllama/dasllama.das` instead.**

**Checked-in text - docs, comments, or string data, any language - that describes a mechanism of
the reference build (any third-party engine, library or runtime whose figure a sentence compares
with this module's own), or names that build, its binaries or symbols, wherever the diff puts it -
and the commit message or PR body of such a diff - applies `REVIEW_UPSTREAM.md`.**

**A diff that changes what authoring a new weight format entails - a step added or dropped, a
file the author must touch, a fixture or probe entry the format must supply, or a gate it must
pass - updates `HOW_TO_ADD_A_FORMAT.md` in the same change.** The how-to is the next format
author's whole brief: a step dropped there is a step the next format silently skips.

**Legal attribution - a third party's copyright line, licence name, or licence text - lives in
a model card (the provenance-and-licence page beside a released model or pack), this folder's
`THIRD_PARTY_NOTICES.md`, the `LICENSE.*` files, or a ledger row naming a licence as a reason to
adopt or reject a model, a dataset, or a dependency; anywhere else in prose it is a defect.**

**A diff that moves a family encode stage - a `dasllama/dasllama_<family>.das` stage that turns
input into embeddings - onto a GPU hook leaves the CPU form in place and changes none of its
arithmetic.** The CPU form serves every box with no driver.

**A call to a `set_*_q8` lane setter - one that picks whether a model family's weights run the
q8 or the float path - in a file under `dasllama/` or `harness/`, outside the body of another
`set_*_q8` setter, is followed at once by a `defer()` calling its `reset_*_q8` twin.** A pin
still set after its
caller returns silently changes the lane of the next model the process loads.

**A diff that writes a CPU feature name in a `[tune_perm]` `requires=` argument that
`TUNE_KNOWN_FEATURES` (`modules/dasLLVM/daslib/llvm_tune.das`, repo root) does not list adds it
there in the same change.** The `features` fingerprint saved with every sidecar is this box's
pass/fail over that list, so a name outside it is never recorded and a box adopting a shipped
profile re-runs the tuning the profile was meant to save.

**A diff that names a `[tune]` family's generated `<fn>_variants()` registry in a file under
`dasllama/` puts that reference inside a `static_if (typeinfo module_exists(llvm_tune))` branch
whose other branch compiles without it.** The tune framework generates those registries only when
dasLLVM is built, so a build without it - the doc lane - fails to compile the engine module.
