# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_RUNTIME.md`,
`ARCHITECTURE_MEASUREMENT.md` (routed checklists own the other companions). Planned work:
`followup_general.md` (rig and instrument rows included), `followup_vulkan.md` (engine work on
the Vulkan tier), `followup_metal.md` (engine work on the Metal tier, or CPU engine work
measured on macOS), `PERF_LEDGER.md` (performance; the rest goes to the followup ledgers).

**Code that times a run itself and hands the wall or rate back as its result - a file that
prints it, or a function that returns it to whichever file calls it - a kernel race (a run timing
two kernel variants - arms - against each other in one process), or a file
`benchmarks/lcpp_bench.das` requires directly, wherever it lives, answers to this folder's
`benchmarks/REVIEW.md` beside its own folder's checklist.** A driver reading a child's clock is
not one.

**A diff that writes a measured number down - into `PERF_LEDGER.md`, a checked-in doc, a
code comment, checked-in data a run produced, or a PR body - or adds a serving path or moves
an existing one onto other code, or changes what a measured or served run with no flags and no
environment overrides computes, applies `REVIEW_MEASUREMENT.md`.** A serving path is the
end-to-end route a run takes from prompt to tokens; its compile tier (interpreted, JIT, AOT)
and its cross target (a build for another platform) are part of it.

**A diff that adds a kernel or loop the runtime re-enters per token, per frame or per prefill
quantum - one batch of prompt tokens the prefill path processes in a single pass - adds a call
path the runtime re-enters that way, adds, moves, renames or removes a `[hot_path]`,
`[cold_path]`, `[no_alloc]`, `[no_env]` or `[no_io]` annotation, or changes a measurement driver
under `benchmarks/`, `harness/` or `performance/`, wherever the diff puts it, applies
`REVIEW_HOT_PATH.md` (beside this file) together with this list.**

**A change to what enters `performance/records/`, or to a provenance manifest, answers to
`performance/REVIEW.md`.** A change to WHICH model file a recorded row or a manifest pins
answers to it too. A model file here is a `.gguf`, a `.dlim`, an mmproj (a multimodal projector
weight file), or an image or audio fixture. A test or tool merely opening a stocked model file
by name does not route.

**A change to the sidecar-exchange client (`dasllama/dasllama_exchange.das`) - the code that
downloads tune winners to a box and submits that box's winners back - its schema, or a
tune-boot path (a startup path that loads a tune sidecar) that reaches it, applies
`performance/REVIEW.md` and `REVIEW_EXCHANGE.md`.**

**A diff that adds a module under `dasllama/` whose changes reach some of `tests/run.das`'s
areas but not all - `audio`, `vision`, `tts`, `llm`, `infra` - gives it a `MODULE_AREAS` row
naming those areas, in the same change.** A module with no row reaches every area, so the
omission costs every later `run.das -- --changed` the whole suite, never coverage.

**A dasLLAMA `[test]` file, wherever the diff puts it, and every `dasllama/` change answer to this
folder's `tests/REVIEW.md` - open it; the walk does not surface it for a `dasllama/`-only diff.**

**A GPU kernel, driver, dispatch class (a class a `[metal_dispatch]` or `[vk_dispatch]` declares),
or K/V-mirror (the device copy of the K/V cache a GPU decode reads and writes) change, a GPU kernel
A/B race, a call that makes, arms or tears down device-home serving (`create_device_session`,
`set_device_kv`, `moe_gpu_drop_model`), a knockout (an arm that skips a stage to measure that
stage's cost), or a hand-binding arm (one that writes buffer or kargs (kernel-argument struct)
binding numbers as literals), wherever the diff puts it - applies `REVIEW_GPU.md`.**

**A kernel body or a function a kernel calls - a `[metal_kernel]` def, a class a
`[metal_dispatch]` / `[vk_dispatch]` declares, or a fixture either emitter compiles - wherever
the diff puts it, applies `modules/REVIEW_SHADER_EMITTERS.md` (repo root) too.**

**A change to the image rail - `dasllama/dasllama_image.das`, or, wherever the diff puts it, a
`.dlim` mint (building a `.dlim` from a gguf), a `.dlim` load, an image identity, or a flavor
(the backend-and-layout variant an image is baked for, one part of its identity) - applies
`REVIEW_IMAGE.md`.**

**A change to `dasllama/dasllama_audio.das`, `dasllama_audio_io.das`,
`dasllama_audio_embedder.das`, `dasllama_asr.das`, `dasllama_asr_types.das` or `dasllama_vad.das`
(all under `dasllama/`), or to an ASR family file - one `dasllama/dasllama_<family>.das` holding
one speech-recognition family - applies `REVIEW_AUDIO.md`.**

**A change to `dasllama/dasllama_vision.das`, `dasllama/dasllama_vision_io.das`,
`dasllama/dasllama_vision_embedder.das`, a vision family file - one `dasllama/dasllama_<family>.das`
holding a single vision projector family - or an in-process path (one that runs inside the program
under review, not a spawned child process) that splices a stream carrying decoded media - pixels or
audio samples - into a prompt or schedules such a stream, applies `REVIEW_VISION.md`.**

**A `dasllama/dasllama_tower.das` change - the backend-independent encoder-tower home, not the
Metal tower driver - applies `REVIEW_AUDIO.md` and `REVIEW_VISION.md`.**

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

**A diff that turns a weight-format id - a `KqFmt` member, a GGUF type number, or the int a generated
kernel takes as its format parameter - into plane strides, or reads a per-block or per-element byte
count of one format, wherever it sits, applies `REVIEW_KQ_FORMATS.md`.**

**`DASLLAMA_RELEASE` (`dasllama/dasllama_version.das`) is bumped only on a declared release -
a maintainer ruling that bench comparability is broken.** Recorded performance rows and tune
sidecars stay valid across code changes, and per-change invalidation lives in the finer
mechanisms - `IMAGE_VERSION` and `layout_fingerprint()` (`dasllama/dasllama_image.das`).

**A function-typed global a serialized exe must re-establish, or that a job (a forked context)
invokes, lands in a `dasllama/` file beside the `[init]` that establishes it at boot, and that
`[init]` re-establishes it when it reads null.** A serialized exe and a forked context restore
globals as data, so a declaration initializer alone arrives null and dies at the first invoke
while every `-jit` gate stays green; a global only the main context invokes, armed by another
file's `[init]`, may carry its null default as the declared "no hook".

**Never reorder or merge the float multiplies in a function that builds a RoPE angle table
(`dasllama/dasllama_rope.das`).** A regrouping moves the angles in the last bits and flips
token-exact fixtures.

**A diff that sets a constant in `dasllama/` to the value a timing of candidate values chose, or
that adds or changes a predicate there that picks among kernel variants computing the same result
because one measured faster (not because it is the smallest variant that fits the dispatch), takes
the winner from a race that timed every candidate interleaved in one process with one script, and
puts that race's rows, each naming its candidate, in the PR body or the change's dated
`PERF_LEDGER.md` row.** Timings taken in two processes or at two commits also differ by everything
else that changed between the runs, so they cannot pick a candidate.

**A diff after which an existing allocation's size starts or stops growing with a count the model
file sets, with how many tokens one step computes at once, or with how many regions one buffer is
split into (the K/V cache's device copy, one region per request served at once; an MoE dispatch's
expert regions) ships the measured pair - peak footprint and wall-clock - in `PERF_LEDGER.md` with
the decision it settles.**

**A diff that adds an allocation whose size grows with a count the model file sets, with how many
tokens one step computes at once, or with how many regions one buffer is split into (the K/V
cache's device copy, one region per request served at once; an MoE dispatch's expert regions)
states that size in bytes, at the largest model shape the code path serves, in a `PERF_LEDGER.md`
row.**

**A new call to an f32 matmul (`matmul_batch`, `mm_blob_b`, `mm_fblob_b`, per-head `gemm_f32` /
`gemm_f32_jo`, or an f32 GPU mm) outside a correctness-comparison path (one whose only job is
to produce a reference result to check another against), where a faster-format twin already
serves the same weights and shape, is a defect - call that twin instead.** A site that must
stay f32 for another reason is ledgered on its own file's sec.1 charter line - the line naming
what that file holds - in an `ARCHITECTURE_*.md` companion; a comment at the call site does not
discharge this.

**A caller never re-checks a guard its callee checks - drop the caller's copy.**

**A boot-path prompt (code that runs at startup, before the first request) that reads stdin
without first proving both stdin and stdout are terminals is a defect - emit the question as
a `@sidecar` event instead.** A supervised or piped boot must never block on input.

**A print or log of an elapsed interval whose site is in an engine file (`dasllama/`), outside
the profiling rails (`profile_tag` / `profile_marker`, `prof_add`, `asr_prof_add`, the Vulkan
tier's `vk_prof()`-gated ledgers and logs), a cold one-shot load, bake, map or tokenizer-build
progress log, or the first-start kernel race report of a fat exe (one built `DAS_TUNE_MODE=fat`,
shipping its tune profile), is a defect - route it through a rail (`ARCHITECTURE_MEASUREMENT.md` sec.2.10).**

**In an engine file (`dasllama/`), a clock value that changes what the program DOES - control
flow, eviction, a generated name; not a reported wall-clock time or a best-of reduction over
reported wall-clock times - is marked `// clock: control`** - unmarked, it cannot be told
apart from the ad-hoc profiling an engine file may not carry.

**A change to `encode`/`bpe_encode`, or to a function they call at encode time (not one that only
supplies a metadata default at load) in `dasllama/dasllama_spm.das`, `dasllama/dasllama_bpe.das`
or `dasllama/dasllama_pretok.das` - `encode` being `dasllama_spm.das`'s and `bpe_encode`
`dasllama_bpe.das`'s; the dispatcher in `dasllama/dasllama_tokenizer.das` owes a
`tests/test_tokenizer.das` run, not rows - ships before/after `--tok` rows
(`benchmarks/lcpp_bench.das`) at
two or more input sizes on a model using that tokenizer; a time growing faster than linearly with
input size is a defect.**

**A change to code or data in `dasllama/dasllama_tokenizer.das`, `dasllama/dasllama_spm.das`,
`dasllama/dasllama_bpe.das`, or `dasllama/dasllama_pretok.das`, or to the special-token or
template strings any of them look up, names in the PR body a run of this folder's
`tests/test_tokenizer.das` with its cases EXECUTED, not skipped.**

**A diff that adds an override, or gives one a new effect, without the announce is a defect.** An
announce is the line the run prints where the override changes the outcome. An override is an
environment knob, an exported runtime setter, or an on-disk state file - one a run writes or a
user places, never data a build ships - that moves a gate, policy, or threshold off its default
and so changes which code the run takes or what it writes, reads, mints, or computes. A measured
time, the run's own duration, or a different moment at which the same work happens is not such a
change; a CLI flag is never an override.

**An announce names the override by the spelling a user would set - the env variable, the sidecar
or file key, the setter's name - and, for one on unless turned off, the spelling that turns it off
(none: it says so).**

**A tutorial source, `.rst` page, docstring, help string, `README.md`, or any other checked-in
document, all outside this folder, left showing the old call, flag, default, or stated behaviour
after a change to user-facing API is the change's defect, not the docs'.** User-facing is
anything a consumer outside this repo can depend on - what it calls, types, requires or parses
(facade functions, CLI flags, environment knobs, file formats, defaults, what the installed SDK
lets a program `require`) - plus the in-repo rig and tool surface: any output another tool
parses. A console-only diagnostic is not user-facing.

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

**A diff that adds a `followup_*.md` entry saying a function can be shortened or split - other
than one asking twin bodies onto one template - drops that function's STYLE037/STYLE038
suppression (`// nolint:`, `options _function_length` / `_cyclomatic_complexity`) or lands the
split in the same change; adding such a suppression to a function an entry names is a defect.**

**`options _dasllama_internal` belongs only in a file whose job is to reach engine
internals: an engine file under `dasllama/`, a test, harness, benchmark, or rig this module
owns, or a consumer `ARCHITECTURE_ENGINE.md` sec.1.8 names as ruled** - a symbol the facade
lacks is added to `dasllama/dasllama.das`, not obtained by adding this option to the consumer. A
`require ... public` that re-exports an engine module OUT of a file carrying this option,
beyond what that consumer's ruled charter grants, breaks this rule too.

**An edit that stops a check of any `REVIEW.das` under this folder firing without fixing what it
flagged is a defect:** dropping a check, narrowing what the gate walks - a file or folder it stops
reading - rewriting a finding text so it no longer names what failed, adding a name to a check's licensed
set - the names that check does not flag - or re-stamping a pinned
hash, count or list, where that check's own finding text does not sanction the addition or the
re-stamp; the gate itself says what it enforces.

**A new check in any `REVIEW.das` under this folder, or a check whose licensed set gains a name,
names in its finding text the rule it enforces and ships its line in the `ARCHITECTURE_*.md`
companion section carrying the charter of the feature the check guards - not the file its sites
sit in; never `ARCHITECTURE.md` - in the same change.** The
line names the check and the names it licenses; when the check licenses no names, the line says so.

**Checked-in text under `modules/dasLLAMA/` - docs, comments, or string data, any language - that
describes a reference-build mechanism, or names that build, its binaries or symbols, wherever the
diff puts it - and the commit message or PR body of such a diff - applies `REVIEW_UPSTREAM.md`.**

**A diff that changes what authoring a new weight format entails - a step added or dropped, a
file the author must touch, a fixture or probe entry the format must supply, or a gate it must
pass - updates `HOW_TO_ADD_A_FORMAT.md` in the same change.** The how-to is the next format
author's whole brief: a step dropped there is a step the next format silently skips.

**Legal attribution - a third party's copyright line, licence name, or licence text - lives in
a model card (the provenance-and-licence page beside a released model or pack), this folder's
`THIRD_PARTY_NOTICES.md`, the `LICENSE.*` files, or a ledger row naming a licence as a reason to
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

**A diff that adds a file under `dasllama/`, or gives a file there anything its sec.1 charter
line does not cover, keeps the charters true in the same change - in an `ARCHITECTURE_*.md`
companion, never `ARCHITECTURE.md`.** `ARCHITECTURE.md`'s sec.1 routing block names the
companion holding each file's charter line.

**A `followup_*.md` row whose work landed in this change is deleted and every checked-in
citation of it repointed or dropped; no other row is renumbered (text cites rows by number),
and a row listing several items keeps its number and strikes the one that landed.**

**A row a diff adds to a `followup_*.md` takes a number higher than every number that file
carries - a deleted row's number is never reused.**

**A diff that adds, removes, or moves a section of an `ARCHITECTURE_*.md` companion, or adds
or removes a companion, lands `ARCHITECTURE.md`'s index line and section range, the
companion's own opening (its range and the sections it names), and every repointed prose
`sec.N` / file citation of the moved sections, in the same change; a new section takes a
number no other section in this folder's `ARCHITECTURE*.md` set uses.** Prose citations are
not LINT026-gated, so one naming a section that left its file sends the reader to nothing.

**A diff that moves a family encode stage - a `dasllama/dasllama_<family>.das` stage that turns
input into embeddings - onto a GPU hook leaves the CPU form in place and
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

**A buffer in `dasllama/` whose element count scales with a model dimension - any count the model
file sets - is declared `@exact_size`, and every `resize` of it follows a `reserve` of the SAME
count - a `dasllama/dasllama_math.das` sizing helper (`reserve_resize`, `grow_resize`,
`ensure_length`, `overwrite_resize`), the builtin `scratch_resize` on a `@scratch` carrier, or the
pair spelled out - however small the count looks.** PERF032 flags a `resize` with no `reserve` or
`ensure_capacity` earlier in the function and never compares the counts; a bare grow past the
heap's unreserved-size cap panics the load on the first big model, not at the call site.
