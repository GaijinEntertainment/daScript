# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_GPU.md`, `ARCHITECTURE_MEDIA.md`,
`ARCHITECTURE_TTS.md`, `ARCHITECTURE_MEASUREMENT.md`. Planned work:
`followup_general.md`, `followup_vulkan.md`, `followup_metal.md` (the Metal tier, and CPU work
measured on macOS), `PERF_LEDGER.md` (performance goes to the perf ledger, everything else to
the followup ledgers).

**A dasLLAMA `[test]` file, wherever the diff puts it, answers to this module's
`tests/REVIEW.md`.**

**A timing rig - a script whose output is a measured wall-clock time or rate - or a kernel race,
wherever the diff puts it (`harness/`, `dasllama/`, a test), answers to this folder's
`benchmarks/REVIEW.md`.**

**A diff that writes a measured number down - into `PERF_LEDGER.md`, a checked-in doc, a
code comment, or a PR body - or adds or changes a serving path, one that serves a weight
format, modality, family, or backend, applies `REVIEW_MEASUREMENT.md`.**

**A change to what enters `performance/records/`, or to a provenance manifest, answers to
`performance/REVIEW.md`.** A change to WHICH model file a recorded row or a manifest pins
answers to it too. A model file here is a `.gguf`, a `.dlim`, an mmproj, or an image or audio
fixture. A test or tool merely opening a stocked model file by name does not route.

**A change to the sidecar-exchange client or schema answers to `performance/REVIEW.md`.** The
sidecar exchange is the code that downloads tune winners to a box and submits that box's
winners back.

**A change to the sidecar-exchange client (`dasllama/dasllama_exchange.das`), or to a
tune-boot path that reaches it, applies `REVIEW_EXCHANGE.md`.**

**Every `dasllama/` change applies this folder's `tests/REVIEW.md` - open it explicitly: the
folder walk does not surface it for a `dasllama/`-only diff.**

**A GPU kernel, driver, dispatch class (a class a `[metal_dispatch]` or `[vk_dispatch]`
declares), or K/V-mirror (the device-side copy of the key/value cache a GPU decode reads and
writes) change - and a GPU kernel A/B race, a knockout (an arm that skips a stage to price
it), or a hand-binding arm (one that writes buffer or kargs (kernel-argument struct) binding
numbers as literals instead of taking the kernel class's declared ones), wherever the diff
puts it - applies `REVIEW_GPU.md`.**

**A kernel body or a function a kernel calls - a `[metal_kernel]` def, a class a
`[metal_dispatch]` / `[vk_dispatch]` declares, or a fixture either emitter compiles - wherever
the diff puts it, applies `modules/REVIEW_SHADER_EMITTERS.md` (repo root) too.**

**A change to the image rail - `dasllama/dasllama_image.das`, or, wherever the diff puts it,
a `.dlim` mint (building a `.dlim` from a gguf), a `.dlim` load, an image identity, or a
flavor (the backend-and-layout variant an image is baked for, one part of its identity) -
applies `REVIEW_IMAGE.md`.**

**A change to `dasllama/dasllama_audio.das`, `dasllama/dasllama_audio_io.das`,
`dasllama/dasllama_audio_embedder.das`, `dasllama/dasllama_asr.das`,
`dasllama/dasllama_asr_types.das`, `dasllama/dasllama_vad.das`, or an ASR family file - one
`dasllama/dasllama_<family>.das` holding a single speech-recognition family - applies
`REVIEW_AUDIO.md`.**

**A change to `dasllama/dasllama_vision.das`, `dasllama/dasllama_vision_io.das`,
`dasllama/dasllama_vision_embedder.das`, a vision family file - one
`dasllama/dasllama_<family>.das` holding a single vision projector family - or an in-process
path (one that runs inside the program under review, not a spawned child process) that
splices a stream carrying decoded media - pixels or audio samples - into a prompt or
schedules such a stream, applies `REVIEW_VISION.md`.**

**A `dasllama/dasllama_tower.das` change - the shared encoder-tower home - applies
`REVIEW_AUDIO.md` and `REVIEW_VISION.md`;** a family file that only CALLS a shared rail does
not thereby pick up the other modality's checklist.

**A change to `dasllama/dasllama_tts.das`, `dasllama/dasllama_tts_types.das`,
`dasllama/dasllama_tts_blocks.das`, `dasllama/dasllama_styletts2.das`, a TTS family file -
one `dasllama/dasllama_<family>.das` holding a single speech-synthesis family - a text
front-end file - one stage of the pass that turns text into phonemes
(`dasllama/dasllama_textnorm.das`, `dasllama/dasllama_postag.das`,
`dasllama/dasllama_g2p.das`) - the front-end packs' mint (`harness/build_g2p_data.py`,
`harness/train_postag.py`, `harness/mint_postag_silver.py`, `performance/build_tts_data.das`),
or a call that pins the TTS weight lane (`set_tts_q8` / `set_styletts2_q8`), wherever the diff
puts it, applies `REVIEW_TTS.md`.**

**A diff that adds a file under `dasllama/`, moves code between files, or lands a kernel,
codec, transform, tokenizer, tool-wire, media-IO or registration concern in a new place
applies `REVIEW_PLACEMENT.md`** - the what-lands-where rules.

**A routed file applies BOTH the checklist it routes to and this one; every other file under
`modules/dasLLAMA/` applies this one.**

**`DASLLAMA_RELEASE` (`dasllama/dasllama_version.das`) is bumped only on a declared release -
a maintainer ruling that bench comparability is broken - and the bump's diff carries the
`LAWS.md` entry recording that ruling.** Recorded performance rows and tune sidecars stay valid across code changes, and
per-change invalidation lives in the finer mechanisms - `IMAGE_VERSION` and
`layout_fingerprint()` (`dasllama/dasllama_image.das`).

**A value that cannot change between dispatches of one compiled kernel never reaches that
kernel as a uniform, a kargs field, an `@off` bind offset, or a helper parameter.** A value
that can change between dispatches goes in a uniform, a kargs field, or an `@off` bind offset.

**Landing a function-typed global with a declaration initializer where `REVIEW.das`'s
restore-check walk over `dasllama/` does not reach is a defect.** A serialized exe restores
globals as data, so the function value arrives null and the first invoke to reach it dies at
exe runtime while every `-jit` gate stays green; the walk requires every such global to join
its file's boot-restore `[init]`.

**Never reorder or merge the float multiplies in a function that builds a RoPE angle table
(`dasllama/dasllama_rope.das`) - keep the multiply order the code already has.** A regrouping
moves the angles in the last bits and flips token-exact fixtures.

**A diff that changes a predicate in `dasllama/` picking between kernel forms that both
produce the right answer is based on timing that ran both forms interleaved in one process,
under one instrument.** The same holds for a constant in `dasllama/` whose value was chosen by
timing two candidates against each other; a predicate a kernel's declared contract forces is
settled by that contract. A reading taken across two processes, or across two commits, says
which way the wall-clock time moved, not which implementation to adopt.

**A change to an allocation reached from a load, bake, or convert path (judge a shared helper
at each call site) that trades footprint for speed ships the measured pair - peak footprint and
wall-clock - and a stated decision.**

**A new call to an f32 matmul (`matmul_batch`, `mm_blob_b`, per-head `gemm_f32` /
`gemm_f32_jo`, or an f32 GPU mm) outside a correctness-comparison path (one whose only job is
to produce a reference result to check another against), where a faster-format twin already
serves the same weights and shape, is a defect - call that twin instead.** A site that must
stay f32 for another reason is ledgered on its own file's sec.1 charter line in
`ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_GPU.md`, `ARCHITECTURE_MEDIA.md`, or
`ARCHITECTURE_TTS.md`, not commented into compliance.

**A boot-path prompt (code that runs at startup, before the first request) that reads stdin
without first proving both stdin and stdout are terminals is a defect - emit the question as
a `@sidecar` event instead.** A supervised or piped boot must never block on input.

**A NEW clock read paired with a print or log of the elapsed interval is a defect in an engine
file (`dasllama/`) outside a cold one-shot load, bake, map, or tokenizer-build progress log** -
instrumentation goes through the profiling rails - `profile_tag` / `profile_marker`, `prof_add`,
`asr_prof_add`, the Vulkan tier's `vk_prof()`-gated ledgers - and the rails and their reasons
are `ARCHITECTURE_MEASUREMENT.md` sec.2.10.

**A clock value that changes what the program DOES - control flow, eviction, a generated
name; not a reported wall or a best-of reduction over reported walls - is marked
`// clock: control`, in an engine file (`dasllama/`)** - unmarked, it looks like ad-hoc
profiling to the sweep that runs over the same tree.

**Every new kernel or loop the runtime re-enters per token, per frame, or per prefill
quantum - one batch of prompt tokens the prefill path processes in a single pass - is COVERED
by an annotated region entry** - `[hot_path]`, any of the `[no_alloc]` /
`[no_env]` / `[no_io]` contracts, or `[cold_path]` on its only reaching entry. Covered means
an annotated entry reaches it: the contracts arm down the call graph, so an interior
function carries nothing of its own. A region entry is the outermost such function (a kernel
`*_encode` / `*_decode`, a step driver, the CPU decoder's `forward_*` entries); a loop
reached only from a load, stage, bake, or convert path is not one.

**A new function that no annotated entry reaches but the runtime re-enters per token, per
frame, or per prefill quantum - a step driver, or a backend entry called from dispatch or
harness paths - carries its annotation itself.** A renamed function is not new: its
annotation moves with the name in the same change.

**A change to code or data of `encode`/`bpe_encode` or anything they reach in
`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das` / `dasllama/dasllama_pretok.das`
ships before/after `--tok` rows (this folder's `benchmarks/lcpp_bench.das`) for the affected
backend, and a wall-clock time that grows faster than linearly with input size is a defect.**

**A change to code or data in `dasllama/dasllama_tokenizer.das`, `dasllama/dasllama_spm.das`,
`dasllama/dasllama_bpe.das`, or `dasllama/dasllama_pretok.das`, or to the special-token or
template strings any of them look up, records a run of this folder's
`tests/test_tokenizer.das` with its cases EXECUTED, not skipped.**

**A diff that adds an override, or gives one a new effect, without the announce is a defect.**
An override is an environment knob, an exported runtime setter, or an on-disk state file that
moves a gate, policy, or threshold off its default and thereby changes what the run writes,
reads, mints, or computes - a timing knob included when it moves computed numerics. A knob
that changes only WHEN work happens is not one, and a CLI flag is never one. A default-ON knob
announces on the default path, naming the spelling that turns it off; a default-OFF knob
announces when it is set. The announce is a line the run prints where the override changes the
outcome, naming it by the spelling a user would set - the environment variable name, the
sidecar or file key, or the setter's function name. Per-site repeats are fine; a set-but-inert
override stays silent.

**A change to user-facing API updates every place it is shown: a tutorial source, `.rst` page,
docstring, help string, `README.md`, or checked-in document still showing the old call, flag, or
default is a defect of the change, not of the docs.** User-facing means anything a consumer
outside this repo can depend on - what it calls, types, requires, or parses (facade functions,
CLI flags, environment knobs, file formats, defaults, what the installed SDK lets a program
`require`) - plus the in-repo rig and tool surface: any output another tool parses. A
console-only diagnostic is not user-facing.

**Weakening `dasllama_lint` (`dasllama/dasllama_lint.das`) - the compile-time check that a
consumer requires only this module's public entry modules, matched by the resolved file's
path under `modules/dasLLAMA/` - is a defect:** a module added to its allowed set, the path
match dropped or narrowed, or an error text that no longer names the facade to require
instead. The allowed set is the table in the lint.

**A `// nolint:STYLE037` or `// nolint:STYLE038` on a function a follow-up ledger entry says
can be shortened or split is a defect - land the ledgered split instead.** The warning is what
keeps the ledger entry visible. A ledger entry asking for a dedup across bodies - one template
for several twins - does not fire this rule: the one body left still carries its length.

**`options _dasllama_internal` belongs only in a file whose job is to reach engine
internals: an engine file under `dasllama/`, a test, harness, benchmark, or rig this module
owns, or a consumer `ARCHITECTURE_ENGINE.md` sec.1.8 names as ruled** - a symbol the facade
lacks is added to `dasllama/dasllama.das`, not reached around with this option. A
`require ... public` that re-exports an engine module OUT of a file carrying this option,
beyond what that consumer's ruled charter (`ARCHITECTURE_ENGINE.md` sec.1.8) grants, breaks
this rule too.

**Weakening `REVIEW.das` (beside this file) is a defect:** dropping a check, adding a name to
a check's licensed set - the names that check does not flag - unless the check's own finding
text names that set as its extension point and the diff lands the paired architecture edit it
asks for, or a finding text that no longer names what failed. What the gate enforces is read
from the gate itself; each check's finding text states its own rule.

**A new `REVIEW.das` check ships its sec.1 charter line in the same change, on the charter of
the file it checks - in `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_MEDIA.md`, or
`ARCHITECTURE_GPU.md`.** The line names the check and the names it licenses. A licensed name
is one that check does not flag. When the check licenses no names, the line says so.

**Checked-in prose this module owns - docs and comments, any language - that is not locating,
patching, or reproducing work against the reference build describes an upstream mechanism in
our own terms: no "lifted/ported verbatim from", and no name belonging to the reference build -
symbol, header, constant, binary, project or organization - write "the reference exe" or
"upstream" instead.** The reference build is the
third-party engine this module measures itself against - the checkout
`benchmarks/setup_lcpp_ref.das` pins. A symbol the file carrying that prose calls or holds as
a value is its own name, not attribution.

**A file or line whose job is to locate, patch, or reproduce work against the reference build
names that build's binaries and symbols outright.** The job decides, not the artifact kind - a
regeneration path, an env-knob row, a command line in a methodology or how-to document, a
ledger row whose subject is a reading of the reference build (the compared row, the command
that reproduces it), and a source patch applied TO the reference build all qualify. A row that
cites upstream while proposing our own work is not a reading of the reference build, so it
names no upstream symbol.

**A diff that changes what authoring a new weight format entails - a step added or dropped, a
file the author must touch, a fixture or probe entry the format must supply, or a gate it must
pass - updates `HOW_TO_ADD_A_FORMAT.md` in the same change.** The how-to is the next format
author's whole brief: a step dropped there is a step the next format silently skips.

**Legal attribution never appears in prose - it lives in `THIRD_PARTY_NOTICES.md` and the
`LICENSE.*` files.**

**A def of `dasllama/dasllama.das` - and a new OVERLOAD of one - is TAUGHT: demonstrated in
runnable code in a `tutorials/dasLLAMA/*.das` source and narrated on a
`doc/source/reference/tutorials/dasLLAMA_*.rst` page.** `REVIEW.das`'s `check_tutorial_floor`
matches def NAMES only, so an overload passes on a sibling's tutorial - the reviewer confirms a tutorial calls the NEW
signature, and a mention that only names it (a comment, a passing reference) does not count.

**A NEW `[EnvConfig]` area struct is rendered by `env_markdown()` in the same change.** A
struct the renderer never emits is absent from `ENVIRONMENT.md` and invisible to every test;
a struct the renderer emits but the registry does not is caught by
`tests/test_env_registry.das`.

**Hand-editing `dasllama/dasllama_unicode.das`'s RANGES/WS tables is a defect - regenerate them
by retranscoding `$LCPP/src/unicode-data.cpp` (the reference checkout) instead.**

**A diff that adds a file under `dasllama/`, moves code between files, or changes what a file
owns lands the sec.1 edit that keeps the charters true - in `ARCHITECTURE_ENGINE.md`,
`ARCHITECTURE_GPU.md`, or `ARCHITECTURE_MEDIA.md` - in the same change.** A diff that adds
a file to any folder where another file has its own sec.1 charter line lands the new file's
charter line too. A module-root doc
file - a ledger, a plan, `LAWS.md` - has no charter line and needs no charter edit.

**A diff that adds or removes an `ARCHITECTURE_*.md` companion, or moves a section between
companions, lands `ARCHITECTURE.md`'s index line and section range and repoints every prose
`sec.N` / file citation of the moved sections, in the same change.** The `[arch]` citations are
LINT026-gated; the prose ones are not, and a prose citation of a section that left its file
sends the reader to nothing.

**A diff that moves a family encode stage onto a GPU hook leaves the CPU form in place and
changes none of its arithmetic.** The CPU form serves every box with no driver.

**A diff that writes a CPU feature name in a `[tune]` or `[tune_perm]` `requires=` argument that
`TUNE_KNOWN_FEATURES` (`modules/dasLLVM/daslib/llvm_tune.das`, repo root) does not list adds it
there in the same change.** The `features` fingerprint saved with every sidecar is this box's pass/fail over
that list, so a name outside it is never recorded and a box adopting a shipped profile re-runs
the tuning the profile was meant to save.

**A value that a team-lane kernel reads - anything reachable from a `team_parallel_for` /
`team_parallel_for_indexed` / `team_parallel_stages` body (`daslib/jobque_boost.das`, repo
root) - is a `def` returning it, never a module global with a declaration initializer (`let`
or `var`).** A team lane never runs global initializers, so the global reads zero there while
every single-threaded run reads the right value.

**A `resize` in `dasllama/` of a buffer whose element count scales with a model dimension is
preceded by a `reserve` of the same count - a `dasllama/dasllama_common.das` sizing helper that
reserves before it grows (`reserve_resize`, `grow_resize`, `ensure_length`, `overwrite_resize`,
`zeroed_resize`), the builtin `scratch_resize` on a `@scratch` carrier, or the pair spelled
out - whatever the size looks like at today's shapes.** A model dimension makes
the count unbounded, and a bare grow past the heap's unreserved-size cap (64 MB) panics the
load on the first big model rather than at the call site.
