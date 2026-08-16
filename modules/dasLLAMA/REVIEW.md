# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `followup_general.md`, `followup_vulkan.md`.

**`tests/` and `performance/` carry their own checklists, and they govern by KIND, not
location:** a dasLLAMA `[test]` file, wherever the diff puts it, answers to this module's
`tests/REVIEW.md`; an exchange, provenance-manifest, or measuring-rig change answers to its
`performance/REVIEW.md`.
Three kind-routed companions sit beside this file: a GPU kernel, driver, dispatch-class, or
K/V-mirror change applies `REVIEW_GPU.md`; an audio or ASR change `REVIEW_AUDIO.md`; a vision
or media change `REVIEW_VISION.md`. This file's rules bind the rest of the engine.

**Any kernel work bumps `DASLLAMA_VERSION` (`dasllama_version.das`) in the same change.** Kernel
work is whatever changes the compiled compute a sidecar's winners were measured over: a kernel
body, a variant set, or a `[tune]` / `[tune_perm]` / `[tune_companion]` grid. `[tune_scope]`
metadata (`covers=`, `tuner=`, `version_of=`) is not kernel work. A bump with neither the
kernel roster nor sidecar interchangeability changed is a defect — equal versions mean an equal
kernel roster and an interchangeable sidecar set (the exchange keys validity on version and
box).

**A kernel's shape is compile-time; only its data is runtime.** For a given compiled kernel, can
this value change between dispatches? If yes it is data and belongs in a uniform or a kargs
struct; if no it is shape — a block stride, a lane width, an unroll factor, a format selector —
and it must not reach the kernel as a uniform, a kargs field, or a helper parameter.

**A kernel body contains no indirection.** No function pointers, no vtables.

**A predicate answering "can this run" must not also answer "is this ready".**

**Peak memory wins ties against load cost.** A change to an allocation reached from a load,
bake, or convert path (judge a shared helper at each call site) that trades footprint for speed
ships the measured pair — peak footprint and wall-clock — and an explicit stated decision.

**Platform-specific code lands only in a platform backend file.** A platform-neutral file
carrying it is a defect; a new shared concern gets its own file, not more of
`dasllama_common.das`.

**No ad-hoc profiling.** A NEW clock read paired with a print or log of the elapsed interval is
a defect in engine code — instrumentation goes through the sanctioned rails, and a clock whose
value feeds logic is marked `// clock: control`. The rails, and where free-hand timing is
legal, are `ARCHITECTURE.md` §2.10.

**Every new kernel or mid-runtime loop is COVERED by an annotated region entry** — `[hot_path]`,
or any of the `[no_alloc]` / `[no_env]` / `[no_io]` contracts. A region entry is a KERNEL
`*_encode` / `*_decode` / step driver; the tokenizer encode/decode path is out of scope.
Covered means an annotated entry reaches it: the contracts arm down the call graph, so an
interior function carries nothing of its own. A new entry point, including a kernel-backend
override or a batch donor, carries its annotation itself; a rename is not new — annotations
follow the name in the same change.

**A change to `encode`/`bpe_encode` or anything they reach in `dasllama_spm.das` /
`dasllama_bpe.das` / `dasllama_pretok.das` ships before/after `--tok` rows for the affected
backend** — the instrument is the scaling ratio across the size ladder, and superlinear is a
defect. A change to the cell's own corpus input ships the same rows or a statement that the
bytes are unchanged.

**Weakening `tests/test_env_registry.das` is a defect.** It enforces the rest of the knob
contract: no raw environment access outside `dasllama_env.das` (declare an `[EnvConfig]` field
and read `g_env_*.<field>`; dynamic names go through `env_is_set` / `env_value_of`), no
re-declared env helper, and a checked-in `ENVIRONMENT.md` matching what the declarations
render (regenerate with `harness/gen_env_doc.das`). Sanctioned forms are `ARCHITECTURE.md`
§2.9.

**An override announces itself where it changes the outcome.** An override is a gate escape,
policy override, or threshold recalibration. Where one changes what a run measures, mints, or
emits, a printed line names it by env spelling; set-but-inert stays silent, per-site repeats are
fine. Adding one, or giving one a new effect, without the announce is a defect.

**No new record-grade timing harness is written.** Model-level time is measured by the rigs
`PROFILE.md` documents — `performance/gen_profile.das` and `performance/gen_bench_records.das` —
both spawning `benchmarks/lcpp_bench.das`. A tutorial's printed wall-clock is teaching output,
feeding no board.

**A new servable capability gets its cell in the same change**: a board row spawned by
`performance/gen_bench_records.das`, or a manual `benchmarks/lcpp_bench.das` cell with its own
`PROFILE.md` section. A modality, a family, or a serving path a user can wait on counts.

**A timing figure describing served output — tok/s, latency, a model-level comparison — that
reaches a doc, a ledger, or a PR without a cell behind it is a defect.** The cell states its
quant mode and stamps box and engine provenance, so a number can never silently describe a
format nobody serves or a kernel set nobody ships. A rig-internal measurement margin — a crown
delta, a noise floor, tuner timing — is settled by the sidecar or manifest stamp it rides in.

**Runtime serves weights out of a mapped `.dlim`.** A live carrier's planes point into
`parse_image`'s mapping, and going live does no real work — repacking, quantizing, folding,
permuting belong to the mint. A transform on the go-live path is a defect.

**A missing `.dlim` is minted first, and the model is served from what was minted.** A load
path that falls back to reading weights out of the source file into a live carrier is a defect,
as is one serving a carrier the mint did not produce.

**There is one way to mint, and one way to load.** A weight carrier becomes live through
`build_image` and `parse_image` in `dasllama_image.das`. Reading weights into a live carrier,
or releasing an image backing, anywhere else is a defect — and a second mint path, per family,
per format, or per backend, is a defect even where its output is identical.

**A DECODER mint never holds the whole model.** It sizes the image before the first byte goes
out and writes each plane as it is produced. Keeping the source model resident to write from is
a defect, and a mint that is slower in exchange for a lower peak is correct.

**A carrier mint stages (`cache_via_image_staged`) only while its source file is under 1 GiB;
at or past the line it streams like a decoder.**

**A `.dlim` is box- and config-specific, not a portable format.** `image_identity` names the
box profile, the knobs, and the flavor a file was baked for, and a mismatch declines loudly. A
path that reinterprets a mismatched image, or widens an identity so that more files match, is a
defect.

**A bake reaps only its own lane** — an identity's (quant, tag) pair. A save drops AT MOST that
lane's dead siblings plus BROKEN/version-stale images in any lane, nothing else.

**Only a process that can recompute an image's identity may judge it dead, and the one
`dlim_wipe` caller is `gen_bench_records.das`.** Reaping an image whose identity the code
cannot recompute — another flavor's, another family's — is a defect.

**A plane split that follows the source FILE rather than a runtime knob takes ONE image tag**,
with the meta flags describing the layout — a per-tensor type split is not a second flavor.

**An image carries only what its flavor uses.** A plane the target platform or config never
reads is not written — the mint decides, not the load. A flavor takes its file through
`image_path_for` and its tag through `register_image_family_tag`; carrying another flavor's
planes is a defect.

**A change to user-facing API updates every place it is shown.** User-facing means anything a
consumer outside this repo calls or types — exported facade functions, CLI flags, environment
knobs, file formats, defaults — plus the in-repo rig and tool surface: any output another tool
parses, a console-only diagnostic not being one. A tutorial source, `.rst` page, docstring,
help string, `README.md`, or checked-in document still showing the old call, flag, or default
is a defect of the change, not of the docs.

**A symbol the facade re-exports is required through `dasllama/dasllama` (or
`dasllama/dasllama_transformer`) by code outside the module; engine internals, in-module tests,
harnesses and benchmarks require engine files directly.** A split that spreads facade-reachable
requires instead of fixing the re-export is a defect.

**A NEW `[EnvConfig]` area struct is wired into `env_markdown()` AND `registered_env_names()`
in the same change.** Both lists are hand-maintained, and a struct in neither is invisible to
every test.

**A new module file is registered in `.das_module` in the same change.**

**`dasllama_unicode.das`'s RANGES/WS tables are generated — retranscoded from llama.cpp's
`unicode-data.cpp`; hand-editing them is a defect.**

**Placement truth — what each file holds, the seams, the carve-outs — lives in
`ARCHITECTURE.md` §1 and nowhere else; a diff that adds a file, moves code between files, or
changes what a file owns lands the §1 edit that keeps the charters true, in the same change.**
A file inventory restated in this checklist is a defect of the checklist.

**A tensor format conversion lands in `dasllama_convert.das`.**

**A disk-order → compute-order transform lands per scope: kernel-layout in
`dasllama_repack.das`, load-scope in `dasllama_layout.das`.**

**A KV-cache store, read, score dot, or V-accumulate lands in `dasllama_kv_codec.das`, its
format family kept whole.**

**A pre-tokenizer split lands in `dasllama_pretok.das`; a merge algorithm in its backend file
(`dasllama_spm.das` / `dasllama_bpe.das`).**

**A kernel body lands in its tier or backend kernel file** — never in `dasllama_math.das` or a
lens/dispatch macro file.

**A family quirk lands in the family file; a piece two families need moves UP into the
concern's shared file (its own file when none exists)** — never sideways into a sibling.

**Tool wire text — building or parsing — is produced only in `dasllama_tools.das`.**

**Nothing outside `dasllama_audio_io.das` requires `audio` (the miniaudio decode module).**

**Nothing outside `dasllama_vision_io.das` requires `stbimage`.**

**Engine, HTTP, or writer logic never lands in `dasllama_scheduler.das`** — engine logic in
engine files, HTTP in the server, writer logic in the writer's own file.

**An `[init]`-only side-effect require lives in `dasllama_transformer.das`** — arch
registrations, GPU tiers, every module requiring the engine back; it sits in
`dasllama_common.das` only if engine code needs it.

**An architecture file (`dasllama_arch_*.das`) is declarative registration only.** An
architecture that changes a forward loop, or tests a family name on a shared path, is a defect.
