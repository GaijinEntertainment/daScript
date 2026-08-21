# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `followup_general.md`, `followup_vulkan.md`.

**`tests/`, `benchmarks/`, and `performance/` carry their own checklists, and they govern by
KIND, not location:** a dasLLAMA `[test]` file, wherever the diff puts it, answers to this
module's `tests/REVIEW.md`; a timing rig — a script whose output is a measured wall or rate — and any
kernel A/B lab, wherever the diff puts them, answer to `benchmarks/REVIEW.md`; a change to what
enters `performance/records/` or its manifests, and an exchange or provenance-manifest change,
answers to `performance/REVIEW.md`.

**Every `dasllama/` change applies `tests/REVIEW.md` for the test obligation it names.**

**Kind-routed companions sit beside this file:** a GPU kernel, driver, dispatch-class, or
K/V-mirror change applies `REVIEW_GPU.md`. A change to `dasllama/dasllama_audio.das`,
`dasllama/dasllama_audio_io.das`, `dasllama/dasllama_asr.das`, `dasllama/dasllama_asr_types.das`,
`dasllama/dasllama_vad.das`, or an ASR family file — one `dasllama/dasllama_<family>.das`
holding a single speech model family — applies `REVIEW_AUDIO.md`. A change to
`dasllama/dasllama_vision.das`, `dasllama/dasllama_vision_io.das`,
`dasllama/dasllama_vision_embedder.das`, a vision family file — one
`dasllama/dasllama_<family>.das` holding a single vision projector family — or an in-process
path (one that runs inside the program under review, not a spawned child process) that
splices a stream carrying decoded media into a prompt or schedules such a stream, applies
`REVIEW_VISION.md`. A
`dasllama/dasllama_tower.das` change — the shared encoder-tower home — applies
`REVIEW_AUDIO.md` and `REVIEW_VISION.md`; a family file that only CALLS a shared rail does not
thereby pick up the other modality's checklist. A change to the tune sidecar's schema or
emitter, wherever it lands, answers to `modules/dasLLVM/REVIEW.md`. A routed file applies BOTH
the checklist it routes to and this one; every other file under `modules/dasLLAMA/` applies
this one.

**Any kernel work bumps `DASLLAMA_VERSION` (`dasllama/dasllama_version.das`) in the same change.** Kernel
work is whatever changes the compiled compute a sidecar's winners were measured over: a kernel
body, a variant set, or a `[tune]` / `[tune_perm]` / `[tune_companion]` grid. `[tune_scope]`
metadata (`covers=`, `tuner=`, `version_of=`) is not kernel work.

**A `DASLLAMA_VERSION` bump with neither the kernel roster nor sidecar interchangeability
changed is a defect** — equal versions mean an equal kernel roster and an interchangeable
sidecar set (the exchange keys validity on version and box).

**A kernel's shape is compile-time; only its data is runtime.** For a given compiled kernel, can
this value change between dispatches? If yes it is data and belongs in a uniform or a kargs
struct; if no it is shape and must not reach the kernel as a uniform, a kargs field, or a
helper parameter.

**The EMITTED shader contains no indirection.** No function pointers, no vtables. A
`class template` / `def abstract` / `def override` splice is compile-time and conforms —
check the emission, not the das spelling.

**The `*_decline_caps` predicates take only the model and the call shape; window-setup state
is asked by `prefill_decline` / `decode_decline`, never by a caps predicate.** A caps
parameter derived from a live `Session` is a defect unless it describes the CALL (its row
count, its span shape), not the session's setup progress.

**Peak memory wins ties against load cost.** A change to an allocation reached from a load,
bake, or convert path (judge a shared helper at each call site) that trades footprint for speed
ships the measured pair — peak footprint and wall-clock — and an explicit stated decision.

**A new GEMM/GEMV call site takes the fastest serving lane that exists for its weights; the
f32 fallback is for correctness rails only.** A new call to an f32 matmul (`matmul_batch`,
`mm_blob_b`, per-head `gemm_f32`, or an f32 GPU mm) where a faster-format twin already serves
the same weights and shape is a defect unless the site is a parity/oracle rail or carries a
comment naming why f32 is load-bearing there. Weights with no faster twin (unquantized
planes) are out of scope.

**Platform-specific code in an engine file (`dasllama/`) lands only in that platform's backend
file.** A platform-neutral engine file carrying it is a defect — rigs and harnesses select
platforms by design and are out of scope; a new shared concern gets its own file, not more of
`dasllama/dasllama_common.das`.

**No ad-hoc profiling.** A NEW clock read paired with a print or log of the elapsed interval is
a defect in engine code — instrumentation goes through the sanctioned rails, and a clock whose
value feeds logic is marked `// clock: control`. The rails, and where free-hand timing is
legal, are `ARCHITECTURE.md` §2.10.

**Every new kernel or mid-runtime loop is COVERED by an annotated region entry** — `[hot_path]`,
any of the `[no_alloc]` / `[no_env]` / `[no_io]` contracts, or `[cold_path]` on its only
reaching entry (a one-time transform is covered by being declared cold). Covered means an annotated entry
reaches it: the contracts arm down the call graph, so an interior function carries nothing of
its own. A region entry is a KERNEL `*_encode` / `*_decode` / step driver; the tokenizer
encode/decode path is out of scope (`ARCHITECTURE.md` §2.11).

**A new function that no annotated entry reaches but the runtime enters — a step driver, or
a backend entry called from dispatch or harness paths — carries its annotation itself; a
rename is not new** (annotations follow the name in the same change).

**A change to `encode`/`bpe_encode` or anything they reach in `dasllama/dasllama_spm.das` /
`dasllama/dasllama_bpe.das` / `dasllama/dasllama_pretok.das` ships before/after `--tok` rows
(`benchmarks/lcpp_bench.das`) for the affected backend** — the instrument is the scaling
ratio across the size ladder, and superlinear is a defect.

**A change reaching `dasllama/dasllama_tokenizer.das`, `dasllama/dasllama_spm.das`,
`dasllama/dasllama_bpe.das`, or `dasllama/dasllama_pretok.das` records a
`tests/test_tokenizer.das` run with its cases EXECUTED, not skipped.**

**An override announces itself where it changes the outcome.** An override is an environment
knob — an env variable or its programmatic setter — that moves a gate, policy, or threshold
off its default; a CLI flag is never an override under this rule. Where one changes an
observable outcome of the run — what it measures, writes, reads, or mints — a printed line
names it by the env spelling; set-but-inert stays silent, per-site repeats are fine. Adding
one, or giving one a new effect, without the announce is a defect.

**A self-measured model time entering `performance/records/<box>.json` or `PERF_LEDGER.md` comes
from the released `lcpp_bench` exe — `benchmarks/lcpp_bench.das` built by `daspkg release`,
spawned by `performance/gen_bench_records.das` or run by hand where the cell's `PROFILE.md`
section says so — never from the `-jit` script** (a `--for-debug-purposes` row is a debug
instrument). A tutorial's printed wall-clock is teaching output, feeding no board.

**A new servable capability gets its cell in the same change**: a board row spawned by
`performance/gen_bench_records.das`, or a manual `benchmarks/lcpp_bench.das` cell with its own
`PROFILE.md` section. A modality, a family, or a serving path a user can wait on counts; a new
family inside an existing cell owes a recorded row on at least one box.

**A timing figure describing served output — tok/s, latency, a model-level comparison — is a
defect wherever it is written down with no cell behind it: a checked-in doc, a ledger, a code
comment, or a PR description.** The cell states its quant mode and stamps box and engine
provenance, so a number can never silently describe a format nobody serves or a kernel set
nobody ships. A rig-internal measurement margin — a crown delta, a noise floor, tuner
timing — is settled by the sidecar or manifest stamp it rides in.

**A stage-level figure — a speedup of one engine stage, not a served turn — names the harness
and flags that produced it in the same sentence, wherever it is written down: a checked-in
doc, a ledger, a code comment, or a PR description.**

**Runtime serves weights out of a mapped `.dlim`.** A live carrier's planes point into
`parse_image`'s mapping, and going live does no real work — repacking, quantizing, folding,
permuting belong to the mint. A transform on the go-live path is a defect.

**A missing `.dlim` is minted first, and the model is served from what was minted.**

**There is one way to mint, and one way to load.** A weight carrier becomes live through
`build_image` and `parse_image` in `dasllama/dasllama_image.das`. Reading weights into a live carrier,
or releasing an image backing, anywhere else is a defect — and a second mint path, per family,
per format, or per backend, is a defect even where its output is identical.

**A DECODER mint never holds the whole model.** It sizes the image before the first byte goes
out and writes each plane as it is produced. Keeping the source model resident to write from is
a defect, and a mint that is slower in exchange for a lower peak is correct.

**A staged carrier mint (`cache_via_image_staged`) either refuses a source file at or past
1 GiB, naming that file in the refusal, or streams it like a decoder** — the staged form holds
source and image at once, and that doubled peak is what the line caps.

**A `.dlim` is box- and config-specific, not a portable format.** `image_identity` names the
box profile, the knobs, and the flavor a file was baked for, and a mismatch declines loudly. A
path that reinterprets a mismatched image, or widens an identity so that more files match, is a
defect.

**A bake reaps only its own lane** — an identity's (quant, tag) pair. A save drops AT MOST that
lane's dead siblings plus BROKEN/version-stale images in any lane, nothing else.

**Only a process that can recompute an image's identity may judge it dead, and the one
`dlim_wipe` caller is `performance/gen_bench_records.das`.** Reaping an image whose identity the code
cannot recompute — another flavor's, another family's — is a defect.

**A plane split that follows the source FILE rather than a runtime knob takes ONE image tag**,
with the meta flags describing the layout — a per-tensor type split is not a second flavor.

**An image carries only what its flavor uses.** A plane the target platform or config never
reads is not written — the mint decides, not the load. A flavor takes its file through
`image_path_for` and its tag through `register_image_family_tag`; carrying another flavor's
planes is a defect.

**A change to user-facing API updates every place it is shown.** User-facing means anything a
consumer outside this repo can depend on — what it calls, types, requires, or parses
(facade functions, CLI flags, environment knobs, file formats, defaults, what the installed
SDK lets a program `require`) — plus the in-repo rig and tool surface: any output another tool
parses, a console-only diagnostic not being one. A tutorial source, `.rst` page, docstring,
help string, `README.md`, or checked-in document still showing the old call, flag, or default
is a defect of the change, not of the docs.

**Weakening `dasllama_lint` (`dasllama/dasllama_lint.das`) — the compile-time check that a
consumer requires only this module's public entry modules, matched by the resolved file's
path under `modules/dasLLAMA/` — is a defect:** a module added to its allowed set, the path
match dropped or narrowed, or an error text that no longer names the facade to require
instead. The allowed set is the table in the lint; the entry modules are the facade, the
scheduler, and the exchange pair.

**`options _dasllama_internal` belongs only in a file whose job is to reach engine
internals: an engine file under `dasllama/`, a test, harness, benchmark, or rig this module
owns, or a consumer `ARCHITECTURE.md` §1 names as ruled.** Any other file carrying it is a
defect — a symbol the facade lacks is added to `dasllama/dasllama.das`, not escaped around.
A `require ... public` that re-exports an engine module OUT of an escaped file, beyond what
that consumer's ruled charter (`ARCHITECTURE.md` §1) grants, is the same defect wearing a
different line.

**Weakening `REVIEW.das` (beside this file) — the gate that fails when a `def` of
`dasllama/dasllama.das` appears in no `tutorials/dasLLAMA/*.das` (repo root) or in no
`doc/source/reference/tutorials/dasLLAMA_*.rst` page — is a defect:** dropping either
check, exempting a def, or a finding text that no longer names what failed.

**A def of `dasllama/dasllama.das` is TAUGHT where the gate finds it named — demonstrated in
runnable code in a `tutorials/dasLLAMA/*.das` source and narrated on a
`doc/source/reference/tutorials/dasLLAMA_*.rst` page.** A mention that only names it (a
comment, a passing reference) is a defect the gate cannot see.

**A NEW `[EnvConfig]` area struct is rendered by `env_markdown()` in the same change.** A
struct the renderer never emits is absent from `ENVIRONMENT.md` and invisible to every test;
a struct the renderer emits but the registry does not is caught by
`tests/test_env_registry.das`.

**A new `.das` under `dasllama/` is registered in `.das_module` in the same change.** Module
files shipped from other subfolders resolve by relative path and register nothing.

**`dasllama/dasllama_unicode.das`'s RANGES/WS tables are generated — retranscoded from llama.cpp's
`unicode-data.cpp`; hand-editing them is a defect.**

**A diff that adds a file, moves code between files, or changes what a file owns lands the
`ARCHITECTURE.md` §1 edit that keeps the charters true, in the same change.** A per-file
inventory restated in this checklist is a defect of the checklist (the per-kind landing rules
below are the checklist's own).

**A tensor format conversion lands in `dasllama/dasllama_convert.das`.**

**A disk-order → compute-order transform lands per scope: kernel-layout in
`dasllama/dasllama_repack.das`, load-scope in `dasllama/dasllama_layout.das`.**

**When placement rules disagree on one function, `ARCHITECTURE.md` §1's charter decides; a
diff that adds or moves such a function lands the charter line with it.**

**A CPU KV-cache store, read, score dot, or V-accumulate lands in `dasllama/dasllama_kv_codec.das`,
its format family kept whole.** GPU twins land in their backend kernel file.

**A pre-tokenizer split lands in `dasllama/dasllama_pretok.das`; a merge algorithm in its backend file
(`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das`).**

**A kernel body lands in its tier or backend kernel file** — never in `dasllama/dasllama_math.das` or a
lens/dispatch macro file.

**A family quirk lands in the family file; a piece two families need moves UP into the
concern's shared file (its own file when none exists)** — never sideways into a sibling.

**Nothing in `dasllama/dasllama_tower.das` — the shared encoder-tower home — names a family: no
signature there takes a type `dasllama/dasllama_audio.das` or a family file declares, and it
requires neither.** The file every tower composes must serve every tower; a helper shaped for
one lands in that tower's own file.

**A weight plane's element type follows its SOURCE tensor, per tensor.** A carrier reads each
tensor at the element type that tensor is stored as and never picks one type for the whole
file (a shipped file mixes them); a type it cannot serve is refused in a message naming that
tensor and its element type, never converted silently.

**A harness that prints output for another tool to compare fails loudly when it has nothing to
print.** A run that ends without its comparison lines — wrong flags, failed load — exits
non-zero.

**Tool wire text — building or parsing — is produced only in `dasllama/dasllama_tools.das`.**

**No engine file (`dasllama/`) other than `dasllama/dasllama_audio_io.das` requires `audio` (the
miniaudio decode module).**

**No engine file (`dasllama/`) other than `dasllama/dasllama_vision_io.das` requires `stbimage`.**
Benchmarks, harnesses, and tests decode their own fixtures.

**Engine, HTTP, or writer logic never lands in `dasllama/dasllama_scheduler.das`** — engine logic in
engine files, HTTP in the server, writer logic in the writer's own file.

**An `[init]`-only side-effect require lives in `dasllama/dasllama_transformer.das`** — arch
registrations, GPU tiers, every module requiring the engine back; it sits in
`dasllama/dasllama_common.das` only if engine code needs it.

**An architecture file (`dasllama_arch_*.das`) is declarative registration only.** An
architecture that changes a forward loop, or tests a family name on a shared path, is a defect.
