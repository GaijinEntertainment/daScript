# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md` and the `ARCHITECTURE_*.md` companions it indexes. Planned work:
`followup_general.md`, `followup_vulkan.md`, `PERF_LEDGER.md` (performance goes to the perf
ledger, everything else to the followup ledgers).

**A dasLLAMA `[test]` file, wherever the diff puts it, answers to this module's
`tests/REVIEW.md`.**

**A timing rig - a script whose output is a measured wall or rate - wherever the diff puts
it, answers to this folder's `benchmarks/REVIEW.md`.**

**A diff that writes a measured number down - into `PERF_LEDGER.md`, a checked-in doc, a
code comment, or a PR body - or adds a servable capability applies `REVIEW_MEASUREMENT.md`.**

**A change to what enters `performance/records/`, or to a provenance manifest, answers to
`performance/REVIEW.md`.** A change to WHICH model file a recorded row or a manifest pins
answers to it too. A model file here is a `.gguf`, a `.dlim`, an mmproj, or an image or audio
fixture. A test or tool merely opening a stocked model file by name does not route.

**A change to the sidecar-exchange client or schema answers to `performance/REVIEW.md`.** The
sidecar exchange is the code that downloads tune winners to a box and submits that box's
winners back.

**Every `dasllama/` change applies this folder's `tests/REVIEW.md`.**

**A GPU kernel, driver, dispatch-class, or K/V-mirror change applies `REVIEW_GPU.md`.**

**A change to the image rail - `dasllama/dasllama_image.das`, or, wherever the diff puts it,
a `.dlim` mint (building a `.dlim` from a gguf), a `.dlim` load, an image identity, or a
flavor (the backend-and-layout variant an image is baked for, one part of its identity) -
applies `REVIEW_IMAGE.md`.**

**A change to `dasllama/dasllama_audio.das`, `dasllama/dasllama_audio_io.das`,
`dasllama/dasllama_audio_embedder.das`, `dasllama/dasllama_asr.das`,
`dasllama/dasllama_asr_types.das`, `dasllama/dasllama_vad.das`, or an ASR family file - one
`dasllama/dasllama_<family>.das` holding a single speech model family - applies
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

**A routed file applies BOTH the checklist it routes to and this one; every other file under
`modules/dasLLAMA/` applies this one.**

**Any kernel work bumps `DASLLAMA_VERSION` (`dasllama/dasllama_version.das`) in the same
change, and a bump with no kernel work is the same defect.** Kernel work is whatever changes
the generated kernel source, or the set of compiled pipeline variants (PSOs) built from it -
a kernel body, a variant set, or a `[tune]` / `[tune_perm]` / `[tune_companion]` grid. A
host-side bind or dispatch-argument change (an `@off` binding, a uniform value) is not, and
neither is `[tune_scope]` metadata (`covers=`, `tuner=`, `version_of=`).

**A value that cannot change between dispatches of one compiled kernel never reaches that
kernel as a uniform, a kargs field, an `@off` bind offset, or a helper parameter.** A value
that can change between dispatches goes in a uniform, a kargs field, or an `@off` bind offset.

**Never reorder or merge the float multiplies in a function that builds a RoPE angle table
(`dasllama/dasllama_rope.das`) - keep the multiply order the code already has.** A regrouping
moves the angles in the last bits and flips token-exact fixtures.

**A diff that changes a kernel-selection predicate in `dasllama/` is based on timing that ran
both variants interleaved in one process, under one instrument.** The same holds for a
constant in `dasllama/` whose value was chosen by timing two candidates against each other. A
reading taken across two processes, or across two commits, says which way the wall moved, not
which implementation to adopt.

**A change to an allocation reached from a load, bake, or convert path (judge a shared helper
at each call site) that trades footprint for speed ships the measured pair - peak footprint and
wall-clock - and a stated decision.**

**A new call to an f32 matmul (`matmul_batch`, `mm_blob_b`, per-head `gemm_f32`, or an f32 GPU
mm) outside a parity or oracle rail, where a faster-format twin already serves the same weights
and shape, is a defect - call that twin instead.** Weights with no faster twin (unquantized
planes) are out of scope; a site that must stay f32 for another reason is ledgered on its own
file's sec.1 charter line in `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_GPU.md`, or
`ARCHITECTURE_MEDIA.md`, not commented into compliance.

**Platform-specific code in an engine file (`dasllama/`) lands only in that platform's backend
file.**

**A diff that adds a new engine concern that is not `Model`/`Session`/`Config` state to
`dasllama/dasllama_common.das` is a defect - give the concern its own file.**

**A boot-path prompt (code that runs at startup, before the first request) that reads stdin
without first proving both stdin and stdout are terminals is a defect - emit the question as
a `@sidecar` event instead.** A supervised or piped boot must never block on input.

**A NEW clock read paired with a print or log of the elapsed interval is a defect in an engine
file (`dasllama/`) outside a cold one-shot load, bake, map, or tokenizer-build progress log** -
instrumentation goes through the sanctioned rails, listed with their reasons in
`ARCHITECTURE_MEASUREMENT.md` sec.2.10.

**A clock value that changes what the program DOES - control flow, eviction, a generated
name; not a reported wall or a best-of reduction over reported walls - is marked
`// clock: control`, in an engine file (`dasllama/`)** - unmarked, it looks like ad-hoc
profiling to the sweep that runs over the same tree.

**Every new kernel or loop the runtime re-enters per token, per frame, or per prefill
quantum is COVERED by an annotated region entry** - `[hot_path]`, any of the `[no_alloc]` /
`[no_env]` / `[no_io]` contracts, or `[cold_path]` on its only reaching entry. Covered means
an annotated entry reaches it: the contracts arm down the call graph, so an interior
function carries nothing of its own. A region entry is the outermost such function (a kernel
`*_encode` / `*_decode`, a step driver, the CPU decoder's `forward_*` entries); a loop
reached only from a load, stage, bake, or convert path is not one.

**A new function that no annotated entry reaches but the runtime re-enters per token, per
frame, or per prefill quantum - a step driver, or a backend entry called from dispatch or
harness paths - carries its annotation itself; a rename is not new** (annotations follow
the name in the same change).

**A change to code or data of `encode`/`bpe_encode` or anything they reach in
`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das` / `dasllama/dasllama_pretok.das`
ships before/after `--tok` rows (this folder's `benchmarks/lcpp_bench.das`) for the affected backend** - the
instrument is the scaling ratio across the size ladder, and superlinear is a defect.

**A change to code or data in `dasllama/dasllama_tokenizer.das`, `dasllama/dasllama_spm.das`,
`dasllama/dasllama_bpe.das`, or `dasllama/dasllama_pretok.das`, or to the special-token or
template strings any of them look up, records a run of this folder's `tests/test_tokenizer.das` with its cases
EXECUTED, not skipped.**

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
`require`) - plus the in-repo rig and tool surface: any output another tool parses, a
console-only diagnostic not being one.

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
lacks is added to `dasllama/dasllama.das`, not escaped around. A `require ... public` that
re-exports an engine module OUT of an escaped file, beyond what that consumer's ruled charter
(`ARCHITECTURE_ENGINE.md` sec.1.8) grants, breaks this rule too.

**Weakening `REVIEW.das` (beside this file) is a defect:** dropping a check, adding a name to
a check's licensed set - the names that check does not flag - or a finding text that no
longer names what failed. What the gate enforces is read from the gate itself; each check's
finding text states its own rule.

**A new `REVIEW.das` check ships its sec.1 charter line in the same change, on the charter of
the file it checks - in `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_MEDIA.md`, or
`ARCHITECTURE_GPU.md`.** The line names the check and the names it licenses. A licensed name
is one that check does not flag. When the check licenses no names, the line says so.

**An upstream mechanism is described in our own terms, not attributed** - no
"lifted/ported verbatim from" and no upstream symbol, header, or constant names in a `.md`
file or a `.das` comment; state what the code does and why its shape wins. Provenance is
not attribution: a path naming where checked-in data is regenerated FROM, an env-knob row
in `ENVIRONMENT.md` whose value locates the reference binary, and a command line or flag
list in `METHODOLOGY.md`, `PROFILE.md`, or `BRINGUP.md`, and a follow-up ledger's board row
naming the build it compares against, all name the binary outright;
every other `.md` line and `.das` comment writes "the reference exe" or "upstream". Legal
attribution lives in `THIRD_PARTY_NOTICES.md` and the `LICENSE.*` files, so prose never
carries it.

**A def of `dasllama/dasllama.das` - and a new OVERLOAD of one - is TAUGHT: demonstrated in
runnable code in a `tutorials/dasLLAMA/*.das` source and narrated on a
`doc/source/reference/tutorials/dasLLAMA_*.rst` page.** The gate matches def NAMES only, so
an overload passes on a sibling's tutorial - the reviewer confirms a tutorial calls the NEW
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
a file beside one that has its own sec.1 charter line lands that edit too. A module-root doc
file - a ledger, a plan, `LAWS.md` - has no charter line and lands free.

**A per-file inventory restated in this checklist is a defect of the checklist.** The sec.1
charters - `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_GPU.md`, `ARCHITECTURE_MEDIA.md` - own the
per-file list. A rule naming what KIND of code lands in which file is the checklist's own.

**A tensor format conversion lands in `dasllama/dasllama_convert.das`.**

**A disk-order -> compute-order transform lands per scope: kernel-layout in
`dasllama/dasllama_repack.das`, load-scope in `dasllama/dasllama_layout.das`.**

**A CPU KV-cache store, read, score dot, or V-accumulate lands in `dasllama/dasllama_kv_codec.das`,
its format family kept whole.** GPU twins land in their backend kernel file.

**A pre-tokenizer split lands in `dasllama/dasllama_pretok.das`; a merge algorithm in its backend file
(`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das`).**

**A kernel body lands in its owner's backend file.** A GPU kernel body lands in the file where
its PSO is compiled and released. A CPU-tier kernel body lands in that tier's
`dasllama/dasllama_math_<tier>.das`. A kernel body never lands in
`dasllama/dasllama_math.das` or in a lens/dispatch macro file.

**A family quirk lands in the family file; a piece two families need moves UP into the
concern's shared file (its own file when none exists)** - never sideways into a sibling.

**A family gaining an arm for a media kind adds that kind's span markers to that family's chat
template, never to a second renderer.** Span markers are the template text that opens and
closes the media rows. A family whose template or vocab lacks them has no arm for that media
kind - `create_chat_` panics at create, not at render.

**No signature in `dasllama/dasllama_tower.das` takes a type that
`dasllama/dasllama_audio.das`, `dasllama/dasllama_vision.das`, or a family file declares.**
`dasllama/dasllama_tower.das` is the shared encoder-tower home, and it requires none of those
files. A doc comment naming the family a helper was built for is fine.

**A `dasllama/dasllama_tower.das` helper with one calling family lands in that family's
file** - a single-caller helper sanctioned as tower-worthy is ledgered on
`ARCHITECTURE_MEDIA.md` sec.1.7's tower charter line, not argued in review.

**A harness whose run can end with zero result rows exits non-zero when it does - wrong
flags, failed load, a device that declines.**

**Tool wire text (the text of a model's tool/function call, built or parsed) is produced only
in `dasllama/dasllama_tools.das`.**

**No engine file (`dasllama/`) other than `dasllama/dasllama_audio_io.das` requires `audio` (the
miniaudio decode module).**

**No engine file (`dasllama/`) other than `dasllama/dasllama_vision_io.das` requires `stbimage`.**
Benchmarks, harnesses, and tests decode their own fixtures.

**Engine, HTTP, or writer logic never lands in `dasllama/dasllama_scheduler.das`** - engine logic in
engine files, HTTP in the server, writer logic in the writer's own file.

**An `[init]`-only side-effect require in an engine file (`dasllama/`) lives in
`dasllama/dasllama_transformer.das`** - arch registrations, GPU tiers, every module requiring
the engine back; it sits in `dasllama/dasllama_common.das` only if engine code needs it. A
program root (test, harness, benchmark, tool) requires the registration module it needs
directly.

**An architecture file (`dasllama/dasllama_arch_*.das`) that changes a forward loop, or tests a
family name on a shared path, is a defect - it carries declarative registration only.**
