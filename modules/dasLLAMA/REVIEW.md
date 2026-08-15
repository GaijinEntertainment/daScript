# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `followup_general.md`, `followup_vulkan.md`.

**`tests/` and `performance/` carry their own checklists, and they govern by KIND, not
location:** a dasLLAMA `[test]` file, wherever the diff puts it, answers to `tests/REVIEW.md`;
an exchange, provenance-manifest, or measuring-rig change answers to `performance/REVIEW.md`.
This file's rules bind the engine.

**Any kernel work bumps `DASLLAMA_VERSION` (`dasllama_version.das`) in the same change.** Kernel
work adds, removes, or edits a compute-kernel body or variant set — CPU tiers, generated GEMM
families, GPU kernel classes, KV-codec and convert kernels — or a tune family's registration.

**A kernel's shape is compile-time; only its data is runtime.** For a given compiled kernel, can
this value change between dispatches? If yes it is data and belongs in a uniform or a kargs
struct; if no it is shape — a block stride, a lane width, an unroll factor, a format selector —
and it must not reach the kernel as a uniform, a kargs field, or a helper parameter.

**A kernel body contains no indirection.** No function pointers, no vtables.

**A claim about a shape constant is checked against the emitted shader, not the das source.**
Read the generated `*_msl` global or the SPIR-V dump and confirm the constant is literal there.

**Twins of a kernel family bind the same kargs type at the same binding**, even where one twin
ignores a field; shifting the other's fields to different slots is a defect.

**Kernel twins share a template.** Same-body single/batch or format twins stamp one
`class template`: a `@template_constant` axis carries body divergence, a `@template_gate` field
a stamp-varying binding. A copy-pasted twin, or a dummy-bound field where a gate serves, is a
defect.

**Every weight, bias, or lookup-table field on a `[metal_dispatch]` / `[vk_dispatch]` kernel
class declares `@role = "weight"` explicitly.** An un-roled weight-shaped field is a defect even
when the kernel compiles and passes parity.

**A kernel declares its dispatch on the class; the builder is generated.** A new kernel class
carries `[metal_dispatch]` / `[vk_dispatch]` with per-field `@binding` / `@role` / `@off` /
`@span` / `@default`. A NEW hand-written `enc_*` body is a defect unless it is a wrapper shape:
a format or twin pick, a default-filling wrapper, or a composite over generated builders.

**Nothing in the engine or a race harness dispatches a kernel except its `enc_*` builder.**

**No value reaches an encoder twice.** A scalar uniform buffer passed alongside the identical
value as a parameter is a defect, as is a kargs field the fields beside it determine. A builder
parameter the `grid=`/`tg=` spec consumes HOST-side may repeat a kargs field read device-side.

**A cache keyed by a host address carries the span and the form in its key.** A hit must cover
the request, and different upload forms live in separate tables.

**A predicate answering "can this run" must not also answer "is this ready".**

**Peak memory wins ties against load cost.** A change to an allocation reached from a load,
bake, or convert path (judge a shared helper at each call site) that trades footprint for speed
ships the measured pair — peak footprint and wall-clock — and an explicit call.

**Platform backends implement narrow registered contracts.** Platform-specific code in a
platform-neutral file is a defect.

**`tests/test_program_roots.das` is the enforcer for program-root discipline — every root's
`options stack = 524288` and every model-loading root's prefill intent — and weakening it is a
defect here.** Roots live outside this folder, so the test is the rule's only reach.

**A long-running root declares its prefill arm — `allow_cpu_prefill()` on the arms that will
hit the CPU-prefill guard — and logs, once, which configuration it ended on.** The guard
panics, and a panic takes every live stream down; `set_metal_mode(x)` with a runtime value
declares nothing (`MetalMode.off` leaves the guard armed).

**No ad-hoc profiling.** A NEW clock read paired with a print or log of the elapsed interval is
a defect in engine code — instrumentation goes through the sanctioned rails, and a clock whose
value feeds logic is marked `// clock: control`. The rails and their homes are
`ARCHITECTURE.md` §2.10.

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
both spawning `benchmarks/lcpp_bench.das`; the one sanctioned sub-model instrument is the
kernel A/B lab. A tutorial's printed wall-clock is teaching output, feeding no board.

**A new servable capability gets its cell in the same arc**: a board row spawned by
`performance/gen_bench_records.das`, or a manual `benchmarks/lcpp_bench.das` cell with its own
`PROFILE.md` section. A modality, a family, or a serving path a user can wait on counts.

**A timing figure that reaches a doc, a ledger, or a PR without a cell behind it is a defect.**
The cell states its quant mode and stamps box and engine provenance, so a number can never
silently describe a format nobody serves or a kernel set nobody ships.

**A backend-only capability goes in that backend's matching role file.** A new grab-bag file for
it is a defect.

**A GPU family shares ONE device and queue from `<gpu>_common`'s init.** A module creating its
own is a defect.

**A PSO serving the engine is compiled and released by the file that owns its kernel class**,
through its init/release pair — `metal_decode_init` / `metal_kernels_release` in
`dasllama_metal_kernels.das`, `metal_prefill_init` / `metal_prefill_shutdown` in
`dasllama_metal_prefill.das`. A kernel-unit gate's short-lived pipeline is its own.

**Race code for a kernel family lives beside the family** — kernels races its families, prefill
races its own; the shared scaffolding (`race_buf`, `race_envelope_ok`, `race_pair_ms`) is
`<gpu>_common`'s.

**A Metal decline reason is a `MetalDecodeDecline` / `MetalPrefillDecline` value in
`dasllama_metal_shapes.das`; decline counting lives in `dasllama_metal_common.das`.** A
string-typed metal decline, or a counter beside the decline site, is a defect.

**A diff that changes how Metal and Vulkan differ — adding or removing an asymmetry — lands its
`ARCHITECTURE.md` §1.5 edit in the same change.** §1.5 is the closed list; an asymmetry it does
not carry does not exist.

**A pipeline is created only by a `[vk_dispatch]`-generated `ensure_*` and torn down by
`vk_drop_model_state`.** A hand-written pipeline build anywhere else in the engine is a defect.

**A buffer bound as one SSBO range stays under `vk_max_storage_range()`, checked where its size
is NEGOTIATED, not where it binds.** The bind site cannot shrink a buffer that was sized wrong.

**A resident-driver change ships with `harness/parity.das` GPU-vs-CPU runs on one q8 and one kq
model, with `--kv` matching the armed mirror codec** — the vulkan arm is `DASLLAMA_GPU=1`,
never `--ngl`. The driver declines codec-mismatched sessions silently, so the log must show
`resident driver armed`.

**A kernel that reads or writes the K/V mirrors is stamped from a
`[|> template_struct_instance]` codec template (`typedef KT`) with both f32 and f16 instances,
and an f16 instance's mirror stores clamp to the f16 finite range.** A single-codec mirror
kernel is legal only when a codec-templated sibling serves the other codec and its arming gate
keys on `kv16`.

**Every resident override gates sessions on the armed mirror codec and on the flat (non-paged)
cache before touching the mirror.** Mirror bytes move only between same-codec session rows and
mirror rows; an override that byte-copies across codecs corrupts the host authority.

**A descriptor set cached across dispatches lives in state `vk_drop_model_state` clears** — a
`*_ready` latch or a field inside `g_gpu` / the arena.

**A verb arm in `dasllama_asr.das` is one forwarding call.** A new family touches the facade
only at the union field, the finalize line, the `AsrKind` value, and the one-line arms; a
prompt, a decode loop, a caps value, or a language rule in the facade is a defect.

**A GEMM in a family file goes through a `*_mm` wrapper or `mm_blob_b`.** A hand-written
dot-product loop beside them is a defect.

**Every `*_encode` and `*_log_mel` in `dasllama_audio.das` and the family files carries
`[hot_path]` and lints at zero.** Reused buffers take `@scratch`, debug and profiling legs
`[cold_path]`; a nolint where either fits is a defect.

**A mel frontend builds on the FFT-plan machinery in `dasllama_audio.das`.** A hand-rolled DFT
in a family file is a defect.

**A `create_session` / `transcribe` option that the model's `caps()` does not declare panics at
the call site.** Accepting it and silently ignoring it is a defect.

**A weight plane's element type follows its SOURCE tensor, per tensor.** A carrier reads a bf16
tensor as bf16 and an fp32 tensor as fp32 — never rounds one down to match the other, and never
decides the question for a whole file, because a shipped file mixes them.

**A new media kind adds its marker pair to the chat template, never a second renderer.** A
family whose template or vocab lacks the pair has no arm for that media kind — `create_chat_`
panics at create, not at render.

**A scheduler stream carrying media rows neither reads nor writes the prefix cache.** Cache
keys are token ids and the KV past the splice does not follow from them, so a media stream
skips `prefix_attach` at admit and `donate_stream` at reap.

**A media stream's rows eval as ONE prefill quantum.** The non-causal flag is per call, so a
chunk boundary inside the span would change the mask.

**A media splice is expressed as two token spans plus a row block, everywhere it appears** — so
BPE merges never cross the media. The engine, the scheduler, and the server all carry the same
shape; a second representation — a placeholder token, a pre-flattened embedding buffer at the
seam — is a defect, as is a new media kind growing a parallel prefill path.

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
flags, environment knobs, file formats, defaults — plus the in-repo rig and tool surface. The
check covers the tutorial `.das` sources, their `.rst` pages, and `BRINGUP.md` / `PROFILE.md` /
`METHODOLOGY.md` / `ARCHITECTURE.md`; one still showing the old call, flag, or default is a
defect of the change, not of the docs.

**A changed default is restated everywhere the old default was stated.** Every docstring, help
string, and doc line that named the old value names the new one in the same diff.

**A symbol the facade re-exports is required through `dasllama/dasllama` (or
`dasllama/dasllama_transformer`), never from the engine file that defines it.** Engine
internals, in-module tests, harnesses and benchmarks require engine files directly. A split
that spreads facade-reachable requires instead of fixing the re-export is a defect.

**A NEW `[EnvConfig]` area struct is wired into `env_markdown()` AND `registered_env_names()`
in the same change, and `ENVIRONMENT.md` is regenerated.** Those two lists are hand-maintained.

**A new module file is registered in `.das_module` in the same change.** The install rule is a
directory glob; the `ADD_MODULE_DAS` list in `CMakeLists.txt` is a subset, touched only on
join.

**`ENVIRONMENT.md` and `dasllama_unicode.das`'s RANGES/WS tables are generated; hand-editing
either is a defect.** `ENVIRONMENT.md` regenerates via `harness/gen_env_doc.das` in the same
change as whatever moved its inputs (`tests/test_env_registry.das` fails on drift); the
unicode tables retranscode from llama.cpp's unicode-data.cpp.

**Placement truth — what each file holds, the seams, the carve-outs — lives in
`ARCHITECTURE.md` §1 and nowhere else; a diff that adds a file, moves code between files, or
changes what a file owns lands the §1 edit that keeps the charters true, in the same change.**
A new file under `dasllama/` also ships its tests; a new file under `tests/` registers in
`tests/CLAUDE.md`. Routing, keyed by what the hunk IS — each kind has one legal home, and that
kind of code anywhere else is a defect:

- A tensor format conversion lands in `dasllama_convert.das`.
- A disk-order → compute-order transform: kernel-layout in `dasllama_repack.das`, load-scope in
  `dasllama_layout.das`.
- A KV-cache store, read, score dot, or V-accumulate lands in `dasllama_kv_codec.das`, its
  format family kept whole.
- A pre-tokenizer split lands in `dasllama_pretok.das`; a merge algorithm in its backend file
  (`dasllama_spm.das` / `dasllama_bpe.das`).
- A kernel body never lands in `dasllama_math.das` or a lens/dispatch macro file.
- A family quirk lands in the family file; a piece two families need moves UP into the shared
  file, never sideways into a sibling.
- Tool wire text — building or parsing — is produced only in `dasllama_tools.das`.
- Nothing outside `dasllama_audio_io.das` calls the audio decode library; nothing outside
  `dasllama_vision_io.das` calls stbimage.
- Platform-specific code never lands in `dasllama_common.das`; a new shared concern gets its
  own file, not more of common.
- Engine, HTTP, or writer logic never lands in `dasllama_scheduler.das`.
- An `[init]`-only side-effect require lives in `dasllama_transformer.das`; it sits in
  `dasllama_common.das` only if engine code needs it.
- An architecture file (`dasllama_arch_*.das`) is declarative registration only — one that
  changes a forward loop, or tests a family name on a shared path, is a defect.
- Every environment knob lives in `dasllama_env.das` — all `[EnvConfig]` area structs and
  their `g_env_*` globals; `tests/test_env_registry.das` enforces both directions.
