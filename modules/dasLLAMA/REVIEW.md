# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `followup_general.md`, `followup_vulkan.md`.

**`tests/` and `performance/` carry their own checklists, and they govern by KIND, not
location:** a dasLLAMA `[test]` file, wherever the diff puts it, answers to `tests/REVIEW.md`;
an exchange, provenance-manifest, or measuring-rig change answers to `performance/REVIEW.md`.
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

**`tests/test_program_roots.das` is the enforcer for program-root discipline — every root's
`options stack = 524288` and every model-loading root's prefill intent — and weakening it is a
defect here.** The contract it enforces: a model-loading root declares `allow_cpu_prefill()` on
the arms that hit the CPU-prefill guard (`set_metal_mode` with a runtime value declares
nothing — `MetalMode.off` leaves the guard armed) and logs, once, which configuration it ended
on; the guard panics, and a panic takes every live stream down. Roots live outside this folder.

**No ad-hoc profiling.** A NEW clock read paired with a print or log of the elapsed interval is
a defect in engine code — instrumentation goes through the sanctioned rails, and a clock whose
value feeds logic is marked `// clock: control`. The rails, and where free-hand timing is
legal, are `ARCHITECTURE.md` §2.10.

**Every new kernel or mid-runtime loop is COVERED by an annotated region entry** — `[hot_path]`,
or any of the `[no_alloc]` / `[no_env]` / `[no_io]` contracts. A region entry is a KERNEL
`*_encode` / `*_decode` / step driver; the tokenizer encode/decode path is out of scope. A new
function is a defect only when no annotated region entry reaches it — a new entry point,
including a kernel-backend override or a batch donor, carries an annotation itself.

**A change to `encode`/`bpe_encode` or anything they reach in `dasllama_spm.das` /
`dasllama_bpe.das` / `dasllama_pretok.das` ships before/after `--tok` rows for the affected
backend** — the instrument is the scaling ratio across the size ladder, and superlinear is a
defect. A change to the cell's own corpus input ships the same rows or a statement that the
bytes are unchanged.

**No raw environment access outside `dasllama_env.das` — declare a knob there instead.** A knob
is an `[EnvConfig]` field, read as `g_env_*.<field>`; `get_env_variable` / `has_env_variable` /
`set_env_variable` / literal-name `env_config_*` elsewhere is a defect. Sanctioned forms are
`ARCHITECTURE.md` §2.9.

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

**A timing figure that reaches a doc, a ledger, or a PR without a cell behind it is a defect.**
The cell states its quant mode and stamps box and engine provenance, so a number can never
silently describe a format nobody serves or a kernel set nobody ships.

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

**A change to user-facing API checks every tutorial and document that touches it.** User-facing
means anything a consumer outside this repo calls or types — exported facade functions, CLI
flags, environment knobs, file formats, defaults — plus the in-repo rig and tool surface: any
output another tool parses, a console-only diagnostic not being one. The check covers the
tutorial `.das` sources, their `.rst` pages, and `BRINGUP.md` / `PROFILE.md` /
`METHODOLOGY.md` / `ARCHITECTURE.md`; one still showing the old call, flag, or default is a
defect of the change, not of the docs.

**A changed default is restated everywhere the old default was stated.** Every docstring, help
string, and doc line that named the old value names the new one in the same diff.

**A symbol the facade re-exports is required through `dasllama/dasllama` (or
`dasllama/dasllama_transformer`) by code outside the module; engine internals, in-module tests,
harnesses and benchmarks require engine files directly.** A split that spreads facade-reachable
requires instead of fixing the re-export is a defect.

**A NEW `[EnvConfig]` area struct is wired into `env_markdown()` AND `registered_env_names()`
in the same change, and `ENVIRONMENT.md` is regenerated.** Those two lists are hand-maintained.

**A new module file is registered in `.das_module` in the same change** (membership of the
`ADD_MODULE_DAS` list in `CMakeLists.txt` is `ARCHITECTURE.md` §1's to define).

**`ENVIRONMENT.md` and `dasllama_unicode.das`'s RANGES/WS tables are generated; hand-editing
either is a defect.** `ENVIRONMENT.md` regenerates via `harness/gen_env_doc.das` in the same
change as whatever moved its inputs (`tests/test_env_registry.das` fails on drift); the
unicode tables retranscode from llama.cpp's unicode-data.cpp.

**Placement truth — what each file holds, the seams, the carve-outs — lives in
`ARCHITECTURE.md` §1 and nowhere else; a diff that adds a file, moves code between files, or
changes what a file owns lands the §1 edit that keeps the charters true, in the same change.**
A file inventory restated in this checklist is a defect of the checklist. A new file under
`tests/` registers in `tests/CLAUDE.md`.

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

**Nothing outside `dasllama_audio_io.das` calls the audio decode library; nothing outside
`dasllama_vision_io.das` calls stbimage.**

**Engine, HTTP, or writer logic never lands in `dasllama_scheduler.das`** — engine logic in
engine files, HTTP in the server, writer logic in the writer's own file.

**An `[init]`-only side-effect require lives in `dasllama_transformer.das`** — arch
registrations, GPU tiers, every module requiring the engine back; it sits in
`dasllama_common.das` only if engine code needs it.

**An architecture file (`dasllama_arch_*.das`) is declarative registration only.** An
architecture that changes a forward loop, or tests a family name on a shared path, is a defect.

**Every environment knob lives in `dasllama_env.das`** — all `[EnvConfig]` area structs and
their `g_env_*` globals; `tests/test_env_registry.das` enforces both directions.
