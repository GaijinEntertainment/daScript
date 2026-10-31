# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_MEASUREMENT.md`. Planned work:
`followup_general.md`, `followup_vulkan.md`, `followup_metal.md`. `tests/`, `benchmarks/`,
`performance/` and `harness/` carry their own `REVIEW.md`.

**A change to the tokenizer - `dasllama/dasllama_tokenizer.das`, `dasllama/dasllama_spm.das`,
`dasllama/dasllama_bpe.das` or `dasllama/dasllama_pretok.das` - or to a string any `special_id`
overload receives, wherever that string is written, applies `REVIEW_TOKENIZER.md` (beside this
file) too.**

**A diff that touches a `followup_*.md`, an `ARCHITECTURE*.md` or a `HOW_TO_*.md` under this
folder, removes or rewrites the problem a `followup_*.md` row states, adds a file under `dasllama/`,
or adds a STYLE037/STYLE038 suppression anywhere under this folder applies `REVIEW_DOCS.md` (beside
this file) too.**

**A diff that adds, changes or drops a check in a `REVIEW.das` under this folder, adds a name to a
check's licensed set - the names a check does not flag - or changes a constant a check compares
against, applies `REVIEW_GATES.md` (beside this file) too.**

**A diff that cites a timing as the reason it picks one candidate over another, adds a race,
changes which candidates a race times, or picks a numeric threshold by a race applies
`REVIEW_RACE_PICKS.md` (beside this file) too.**

**Wherever it lives, each of these answers to this folder's `benchmarks/REVIEW.md` beside its own
folder's checklist: code that times a run on its own clock (not a spawned child's) and hands the
wall or rate back as its result (a file printing it, or a function returning it to its caller); a
kernel race - a run timing two kernel variants (arms) against each other in one process; a file
`benchmarks/lcpp_bench.das` requires directly.**

**A diff that writes a measured number down - into `PERF_LEDGER.md`, a checked-in doc outside
`site*/` (repo root), a code comment, checked-in data a run produced, a commit message, or a PR
body - or adds a serving path or moves an existing one onto other code, or changes what a
measured or served run with no flags and no environment variables set computes (a change only to
when, or in what order, it does the same work does not route), applies `REVIEW_MEASUREMENT.md`.**
A serving path is the end-to-end route a run takes from prompt to tokens; its compile tier
(interpreted, JIT, AOT) and its cross target (a build for another platform) are part of it.

**A diff that does any of these, wherever it puts it, applies `REVIEW_HOT_PATH.md` (beside this
file) together with this list: adds a kernel, loop or call path the runtime re-enters once per
generated token, prefill batch, image, video frame, audio chunk, or speech chunk or frame; adds,
moves, renames or removes a `[hot_path]`, `[cold_path]`, `[no_alloc]`, `[no_env]` or `[no_io]`
annotation; adds or changes a test, harness, benchmark or performance-rig function that reaches a
function carrying `[hot_path]`, `[no_alloc]`, `[no_env]` or `[no_io]`.**

**A diff that adds an allocation, changes an allocation's size formula, or adds, changes or
drops a `resize` or an `@exact_size` on a buffer; that adds or changes a module global (an
`[EnvConfig]` field is a declared environment variable, not a global), a call
to a function-typed one, or the `[init]` that sets one; or that adds or changes code a job runs
in a forked context or code reachable from a `team_parallel_*` or `maybe_parallel_for*` body,
wherever the diff puts it, applies `REVIEW_MEMORY.md` (beside this file) too.**

**A change to what enters `performance/records/`, to a model file's provenance - its source url,
byte size, sha256 or conversion recipe, wherever it is declared - or to WHICH model file a recorded
row or a provenance declaration pins, answers to `performance/REVIEW.md`.** A model file here is a
`.gguf`, a `.dlim`, an mmproj (a multimodal projector weight file), or a picture or audio fixture
file. A test or tool merely opening by name a model file the catalog already stocks does not route.

**A change to the sidecar-exchange client (`dasllama/dasllama_exchange.das`, which downloads tune
winners to a box and submits that box's winners back), to a validator it calls (`validate_sidecar`,
`validate_sidecar_submission`, `validate_record_submission`), or to a tune-boot path (a startup path
that loads a tune sidecar) reaching it, applies `performance/REVIEW.md` and `REVIEW_EXCHANGE.md`.**

**A diff that adds a module under `dasllama/` whose code only some of `tests/run.das`'s areas run
gives it a `MODULE_AREAS` row naming those areas and a `MODULE_WITNESSES` row naming every
`model-free` or `stocked` `llm` test file that runs its code, in the same change.** A module with
no row runs every area, one with `llm` and no witness row the core set: safe, and slower each PR.

**A diff after which an area's tests, or a `model-free` or `stocked` `llm` test file, run an
existing module's code that its `MODULE_AREAS` or `MODULE_WITNESSES` row omits adds the area or
the file to the row in the same change.** A new caller counts, wherever it sits, and so does an
existing test file that starts loading a GGUF it did not load before: the module it now runs is
the `dasllama/dasllama_arch_*.das` file that registers that GGUF's `general.architecture` string;
a row missing an area or a file makes `run.das -- --changed` skip those tests, so a regression
there goes unrun.

**A `[test]` file with a `require dasllama/...` line of its own, wherever the diff puts it, and
every `dasllama/` change answer to this folder's `tests/REVIEW.md`.**

**A diff that changes the body, parameters or return type of a CPU kernel - a function in
`dasllama/dasllama_math*.das` that runs on the CPU and writes an output buffer from operand
buffers - applies `tests/REVIEW_KERNEL_CELLS.md`.**

**A diff that adds, moves or removes a `[test]` file outside this folder's `tests/` with a
`require dasllama/...` line of its own adds, corrects or drops its row, with the reason it lives
there, in `tests/CLAUDE.md`'s "Out-of-folder test files" ledger in the same change.** A file
reaching an engine module through another module's public require owes no row.

**A new pre-tokenizer family, or a new tokenizer backend, ships a `corpus_case(...)` call for it in
`tests/test_tokenizer.das`, naming its `ggml-vocab-*.gguf` fixture.**

**A diff that adds an ASR family ships a test comparing its transcript token for token against a
reference - another backend's run, an external dump, or the CPU path.**

**A family that gains a thinking or tool-call format the server parses ships, in the same change,
tests asserting the format's exact wire text, its rendered template output, and a server case gated
on the family's smallest GGUF under `LARGE_TIER_BYTES` (`tests/_model_tier.das`).**

**A GPU kernel, driver, dispatch class (a class a `[metal_dispatch]`, `[vk_dispatch]` or
`[cuda_dispatch]` declares), or K/V-mirror (the device copy of the K/V cache a GPU decode reads
and writes) change, or a change to a `kv_*` function that a GPU driver file
(`dasllama/dasllama_metal*.das`, `dasllama/dasllama_*{vulkan,cuda}*.das`,
`dasllama/dasllama_gpu*.das`) calls, wherever the diff puts it, applies `REVIEW_GPU.md`.**

**A call that creates, enables or tears down device-home serving (`create_device_session`,
`set_device_kv`, `moe_gpu_drop_model`), or a call or definition of a `set_*` / `reset_*` function or an `install_moe_gpu_resident_*` declaration
in `dasllama/dasllama_core.das`, `dasllama/dasllama_env.das`, or a
`dasllama/dasllama_{gpu,metal}*.das` or `dasllama/dasllama_*{vulkan,cuda}*.das` file, wherever
the diff puts it, applies `REVIEW_GPU.md`.**

**A GPU kernel timing race (two kernels timed against each other to pick one - not a data race), a
knockout (an arm that skips a stage to measure that stage's cost), a hand-binding arm (one that
writes buffer or kernel-argument (kargs) binding numbers as literals), a cell or probe that fills
or binds a `TokMeta` block, a read of `g_env_gpu.cuda` (`DASLLAMA_CUDA`) or
`g_env_vulkan.cuda_mmq` (`DASLLAMA_CUDA_MMQ`), a call of `gpu_want_cuda`, a branch whose true arm
directly calls a `moe_gpu_*`, `vk_*`, `cud_*` or `rdec_*` function that records, submits or waits
on GPU work, or allocates, uploads to or frees device memory, or a call of `vk_moe_init`,
wherever the diff puts it, applies `REVIEW_GPU.md`.**

**A change to `dasllama/dasllama_image.das`, or, wherever the diff puts it, to a `.dlim` mint
(building a `.dlim` from a gguf), a `.dlim` load, an image identity, a flavor (the
backend-and-layout variant an image is baked for, one part of its identity), or which prepared
image (its tag, lane or flavor) a load requests or mints, applies `REVIEW_IMAGE.md`.**

**A change to what `resident_would_serve` answers, or to an environment variable or setter it or
any function it calls reads, applies `REVIEW_IMAGE.md`.** A trimmed image's mint - one written
without the weight planes the GPU driver holds - reads that answer.

**A change to `dasllama/dasllama_audio.das`, `dasllama_audio_io.das`,
`dasllama_audio_embedder.das`, `dasllama_asr.das`, `dasllama_asr_types.das`, `dasllama_vad.das`
or `dasllama_rnnt.das` (all under `dasllama/`), or to an ASR family file - a `dasllama/` file
holding one speech-recognition family's CPU model - applies `REVIEW_AUDIO.md`.**

**A change to `dasllama/dasllama_vision.das`, `dasllama/dasllama_vision_io.das`,
`dasllama/dasllama_vision_embedder.das`, or a vision family file - one
`dasllama/dasllama_<family>.das` holding a single vision projector family - applies
`REVIEW_VISION.md`.**

**A change to an in-process path (one that runs inside the program under review, not a spawned
child process) that splices a stream carrying decoded media - pixels or audio samples - into a
prompt or schedules such a stream applies `REVIEW_VISION.md`.**

**A change to how a prefill driver serves, declines, or passes to the CPU a media span - the rows
of one image or audio turn inside a single model call - applies `REVIEW_VISION.md`.**

**A `dasllama/dasllama_tower.das` change - the backend-independent encoder-tower home, not a
backend tower driver - applies `REVIEW_AUDIO.md` and `REVIEW_VISION.md`.**

**A change to a file `dasllama/dasllama_tts*.das` matches, `dasllama/dasllama_styletts2.das`,
a TTS family file - a `dasllama/dasllama_<family>.das` holding exactly one voice model's code,
never the two-family carrier `dasllama/dasllama_styletts2.das` - or a text front-end file - one
stage of the pass that turns text into phonemes (`dasllama/dasllama_textnorm.das`,
`dasllama/dasllama_postag.das`, `dasllama/dasllama_g2p.das`) - applies `REVIEW_TTS.md`.**

**A change to the front-end packs' mint (`harness/build_g2p_data.py`, `harness/train_postag.py`,
`harness/mint_postag_silver.py`, `performance/build_tts_data.das`), to the Pocket converter or its
card (`harness/convert_pocket.py`, `harness/tts_model_card.md`), or to a call that pins a TTS
weight lane (`set_tts_q8` / `set_styletts2_q8` / `set_pocket_q8`), wherever the diff puts it,
applies `REVIEW_TTS.md`.**

**A diff that adds a file under `dasllama/`, or adds or moves a top-level declaration (a `require`
among them) in a file under `dasllama/`, applies `REVIEW_PLACEMENT.md`.**

**A diff that adds or changes a def in a file `REVIEW.das`'s `FACADE_FILES` lists, or adds `public`
to a require, new or existing, in `dasllama/dasllama.das` or in a file it reaches through `public`
requires alone, or adds an `[EnvConfig]` area struct, applies `REVIEW_FACADE.md` too.**

**A diff that does any of these, wherever it sits, applies `REVIEW_KQ_FORMATS.md`: adds a weight
plane a backend's serving call reads, or changes which `KqFmt` members such a plane is served in,
or makes the load accept a `.scale` tensor beside such a plane; makes a backend's serving call read
a weight format in a form other than its disk bytes; or states, in checked-in text, a commit
message or the PR body, that a backend serves a weight format.**

**A diff that does any of these, wherever it sits, applies `REVIEW_KQ_FORMATS.md` too: turns a
weight-format id (a `KqFmt` member, a GGUF type number, or the int a generated kernel takes as its
format parameter) into plane strides; reads a per-block or per-element byte count of one weight
format; writes as a literal a number fixed by one weight format (its kernel format id, its element
or word count per block, a plane stride); or adds or changes a `stamp = "<family>:<fmt>:<form>"`
kernel class, a `kq_mulmm_<fmt>` or `moe_mulmm_<fmt>` race, a dev-W dequant pass (the pass that
dequantizes weights into an f16 panel on the device) or a format's twin pick.**

**A diff that bumps `DASLLAMA_RELEASE` (`dasllama/dasllama_version.das`) cites in the PR body the
maintainer's ruling that rows measured before it can no longer be compared with rows after it.**
Every recorded row, tune sidecar and exchange entry carries the release, so a bump voids them all.

**A change that invalidates only images never bumps `DASLLAMA_RELEASE` - it applies
`REVIEW_IMAGE.md`.**

**A diff that adds or changes a `dasllama/dasllama_common.das` function that reaches, or reports
on, another module's hook (a function-typed global another module's `[init]` sets), moves a
`dasllama/` stage's work onto a GPU hook (a hook a GPU driver module's `[init]` sets), calls a
`set_*_q8` lane setter under `dasllama/` or `harness/`, or adds or changes an override (an environment knob,
a non-private `dasllama/` setter a file outside `tests/` calls, or an on-disk state file a run
writes or a user places, any of them moving a gate, policy or threshold off its default) or its
announce (the line a run prints where the override changes the outcome) applies
`REVIEW_OVERRIDES.md` (beside this file) too.**

**A call site a diff adds - not a call it moved from another site - of a CPU or GPU matrix
multiply whose weight operand is an f32 plane, outside a correctness-comparison path (one only
producing a reference result) or a timing race's reference arm, where the same backend already
serves the same weights and shape on the run's weight lane (the precision a `set_*_q8` setter or
its knob picks) through a kernel on a non-f32 form, is a defect - call that kernel instead.** A
site that must stay f32 for another reason is ledgered on its file's charter line (the line, in
the companion `ARCHITECTURE.md#file-charters` routes to, saying what the file holds); a call-site
comment does not discharge this.

**A caller and a callee whose every caller sits under this folder never both check one guard: the
caller keeps the check and the callee drops it when the callee is `[cold_path]`; where the caller's
check also gates its own work, the caller computes the decision once and passes it to the callee;
otherwise the caller's copy goes.**
An edit to either copy leaves the caller testing a condition the callee no longer applies.

**Logic or a named constant that two files in one folder both use, and that neither the language nor
the test contract forces them to restate (a test's CPU reference copy of the arithmetic it checks is
forced; one predicate written over an enum and again over its int value is not), lands once - in a
file both already require, or in a new file both require when they share none - never as a second
copy.** Two spellings drift apart on the first edit to one.

**A piece that two folders both need, neither containing the other, lands in the folder that owns
the concern, and the other folder requires it - never a copy in each.**

**A boot-path prompt (code that runs at startup, before the first request) that reads stdin
without first proving both stdin and stdout are terminals is a defect - emit the question as
a `@sidecar` event instead.** A supervised or piped boot must never block on input.

**A print or log of an elapsed interval in an engine file (`dasllama/`) outside the profiling
rails (`profile_tag` / `profile_marker`, `prof_add`, `asr_prof_add`, or a log `DASLLAMA_GPU_PROF`
gates; `ARCHITECTURE_MEASUREMENT.md#sanctioned-instrumentation-rails`) that can fire more than
once per model load, or more than once per process where it runs outside any load, is a defect -
route it through a rail.**

**In an engine file (`dasllama/`), a clock value that changes what the program DOES - control
flow, eviction, a generated name; not a reported wall-clock time or a best-of reduction over
reported wall-clock times - is marked `// clock: control`** - unmarked, it cannot be told
apart from the ad-hoc profiling an engine file may not carry.

**A tutorial source, `.rst` page, docstring, help string, `README.md`, recorded server response
fixture (`utils/dasllama-server/tests/fixtures/`), or any other checked-in document, all outside
this folder, left showing the old call, flag, default, response or stated behaviour after a change
to user-facing API is the change's defect.** User-facing is anything a consumer
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

**`options _dasllama_internal` belongs only in a file whose job is to reach engine internals: an
engine file under `dasllama/`, a test, harness, benchmark, or rig this module owns, or a consumer
`ARCHITECTURE_ENGINE.md#instrumentation-and-support` names as ruled** - a symbol the facade lacks is
added to `dasllama/dasllama.das`, not obtained by adding this option to the consumer.

**A file whose entry under `ARCHITECTURE_ENGINE.md#instrumentation-and-support` rules its
`options _dasllama_internal` re-exports an engine module with `require ... public` only where
that entry names the re-export; any other re-export is a defect - the file's own requirers reach
the symbol through `dasllama/dasllama.das` instead.**

**Checked-in text - docs, comments, or string data, any language - that describes a mechanism of
the reference build (any third-party engine, library or runtime whose figure any checked-in text
of this module compares with this module's own), or names that build, its binaries or symbols,
wherever the diff puts it - and the commit message or PR body of such a diff - applies
`REVIEW_UPSTREAM.md`.**

**A diff that changes the work a `HOW_TO_*.md` tells its author to do - a step added or dropped, a
file the author must touch, a table, fixture or probe entry the work must supply, or a gate it must
pass - updates that HOW_TO in the same change.** A how-to is the next author's whole brief: a step
dropped there is a step the next author skips without knowing.

**Legal attribution - a third party's copyright line, licence name, or licence text - lives in
a model card (the provenance-and-licence page beside a released model or pack), this folder's
`THIRD_PARTY_NOTICES.md`, the `LICENSE.*` files, or a ledger row naming a licence as a reason to
adopt or reject a model, a dataset, or a dependency; anywhere else in prose it is a defect.**

**A diff that writes a CPU feature name in a `[tune_perm]` `requires=` argument that
`TUNE_KNOWN_FEATURES` (`modules/dasLLVM/daslib/llvm_tune.das`, repo root) does not list adds it
there in the same change.** The `features` fingerprint saved with every sidecar is this box's
pass/fail over that list, so a name outside it is never recorded and a box adopting a shipped
profile re-runs the tuning the profile was meant to save.

**A diff that names a `[tune]` family's generated `<fn>_variants()` registry in a file under
`dasllama/` puts that reference inside a `static_if (typeinfo module_exists(llvm_tune))` branch
whose other branch compiles without it.** The tune framework generates those registries only when
dasLLVM is built, so a build without it - the doc lane - fails to compile the engine module.
