# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `followup_general.md`, `followup_vulkan.md`.

## Tests

**Every PR runs the model-free tests under `modules/dasLLAMA/tests/`, plus each suite the change
reaches — nothing runs the whole directory.** A change reaches a suite through a suite file, a
shared helper's behavior, or engine code the suite exercises; a comment-, prose-, or
diagnostic-text-only change reaches none. The suites `run.das` lists (decode, prefill, matrix,
kernels, image, image-vulkan, coverage) run only through it, `--arm`-scoped; it refuses `--full`, and
dastest run directly on one is a defect. Every other test, vulkan included, runs under dastest.

**Every test runs under `-jit`.** Never the interpreter, never AOT. A test invocation without `-jit`
is a defect even if it passes.

**dasLLAMA `[test]` files live under `modules/dasLLAMA/tests/`**, except a self-check by its bench
(`benchmarks/matmul/test_matmul_par.das`) and family live legs by their server
(`utils/dasllama-server/test_openai_server_think.das`); no dasLLAMA test appears in any
`CMakeLists.txt`.

**A test passes or skips explicitly on every platform.** A skip goes through a capability or model
gate; a test that silently vanishes on one platform is a defect.

**A test loading a model over 6 GiB sits behind `model_available`**, run only under
`DASLLAMA_PARITY_FULL=1`: a final pre-PR gate, not the iteration loop. Check what a test loads first.

**A vision oracle artifact is fetched or minted by a recorded command, never produced ad hoc.** Real
image fixtures and mmproj files live in the models dir with `.sha` pins, fetched never generated;
reference dumps live beside them in the oracle directory, each recording the exact `llama-mtmd-debug`
/ `llama-mtmd-cli` invocation that minted it, so regeneration is a command, not archaeology.
Procedural fixtures and their distilled expectations live in-repo and run model-free; a model-gated
vision test skips-as-PASS explicitly.

**Every function whose signature or body changed — new, moved, extracted, or rewritten — ships a
test for the bit itself** (a signature widening with an unchanged body counts: the new receivers are
the new bit) — feed the function, check the bytes; "the model still
runs" is not that test. A platform-fixed predicate has no bytes to feed: test its observable, the argv
it gates or the mode it selects. A moved registration's observable is reachability.

**Every test that compares logits also logs decoded text for both sides.** A red, or a suspicious
green, must be readable as text in the log, not only as an id or float difference.

**A new GPU kernel ships with a small model in the kernel coverage suite** that dispatches it.

**A kernel-unit arm compares its kernel against a CPU oracle.** A GPU-vs-GPU arm is allowed only for
cross-dispatch bit-identity no CPU oracle can witness, and then its output buffers are prefilled with
a sentinel. An IN-PLACE output plane instead pairs its compare with a liveness check.

**A harness that prints output for another tool to compare fails loudly when it has nothing to
print.** A run that ends without its comparison lines — wrong flags, failed load — exits non-zero.

**A test arm's skip gate keys on a device capability or mode predicate (`coopmat_mode`,
`has_coopmat2`, `dn_shared_ok`), never on the existence of a runtime artifact such as a pipeline or a
cached set.** An artifact gate goes permanently false when its producer moves.

**A test suite loads models with `load_model_`, never the image rail.** Image-rail coverage belongs to
the image suites alone (`ARCHITECTURE.md` §2.1).

**A family that gains a live thinking or tool format ships recognition tests in the same change.** The
trigger is a chat template gaining `think_mode` or `tool_mode`, declared in a `dasllama_arch_*.das`
registration or armed through a shared helper (`chatml_chat`, `hermes_tools`). A thinking format adds
the reply wire shape to `tests/test_think_split.das`; a tool format adds wire-codec pins to
`tests/test_tool_formats.das` and render pins to `tests/test_chat.das`; either adds a live leg to
`utils/dasllama-server/test_openai_server_think.das`, gated on the family's smallest GGUF in
`performance/fetch_models.das` and following the server tests' conventions (serving rail,
`DASLLAMA_PARITY_FULL` tier gate, explicit model-gated skips). A vocab lacking the markers exempts the
family; no small-enough local model means the remote leg goes in `THINKING.md`.

**A change in `dasllama_tokenizer.das`, `dasllama_spm.das`, `dasllama_bpe.das`, or
`dasllama_pretok.das` — pre-tokenizer arm, merge loop, vocab load, decode — records a
`tests/test_tokenizer.das` run with its cases EXECUTED, not skipped, and a new pre-tokenizer family or
backend ships its `corpus_case` arm naming the `ggml-vocab-*.gguf` fixture.** A corpus case asserts
exact reference ids AND lossless round-trip; an ids-only case is a defect.

**A new measuring entry point calls `tune_gate()` (`performance/profile_common.das`) before its first
timed rep**, or it measures fallback kernels silently (`ARCHITECTURE.md` §2.5). Kernel A/B labs are
exempt, as is the `--tok` cell in `benchmarks/lcpp_bench.das`, which dispatches none.

**No new record-grade timing harness is written.** Model-level time is measured by the rigs
`PROFILE.md` documents — `performance/gen_profile.das` and `performance/gen_bench_records.das`
(`--oracle` = the regression gate, bare = the publishing board) — both spawning
`benchmarks/lcpp_bench.das`. A new model-timing harness, a one-off measurement script, or a revived
rig is a defect; the one sanctioned sub-model instrument is the kernel A/B lab. A tutorial's printed
wall-clock is teaching output, feeding no board (`ARCHITECTURE.md` §2.5).

**A new servable capability updates the profiling app in the same arc.** A modality, a family,
or a serving path that a user can wait on gets its cell in the rigs `PROFILE.md` documents — not
a bench beside it, and not a number quoted from a script that no longer exists. The cell is what
makes the measurement reproducible by someone else and comparable to the next one: it states its
quant mode, calls `tune_gate()`, and stamps box and engine provenance, so a number can never
silently describe a format nobody serves or a kernel set nobody ships. A timing figure that
reaches a doc, a ledger, or a PR without a cell behind it is a defect.

**A kernel A/B lab under `benchmarks/` is a decision instrument, not a record rig.** It answers a
kernel-join question by interleaved same-instrument A/B under a bit-exact cross-arm gate; hand-rolled
bind lists are legal there, so a binding renumber fixes or deletes the arms it rots. Adoption deletes
the pair; numbers never enter the record stores or `PERF_LEDGER.md`.

**An out-of-process observer measures only what no in-process rig can observe about itself, and its
numbers are ledger-grade.** The one instance is `benchmarks/asr/mem_census.sh`: macOS-only, numbers in
`PERF_LEDGER.md`, never in the record stores. Wall-clock timing of a FOREIGN reference binary is not
one; a subtraction-based estimate prints both raw walls.

**A change to an entry, or to the verify or fetch logic, in `performance/fetch_models.das` records
its gate run: `fetch_models.das --` ends `0 failed` (`BRINGUP.md` §2).** A comment-only change
does not.

---

## Placement — one file, one rule

Every file states what it holds, and code that belongs to a file and is written anywhere else is a
defect. A rule names its own file's contents, and where two files share a seam it may name the other
side to make the boundary decidable. `ARCHITECTURE.md` §1 carries the boundaries and the carve-outs.

**A new file under `dasllama/` ships with its rule here, its `ARCHITECTURE.md` §1 charter, and its
tests, in the same change.** A new file under `tests/` registers in `tests/CLAUDE.md` instead: its
suite's arm documentation and `tests/run.das` when it joins one, else the model-free/no-arm note.

**A symbol the facade re-exports is required through `dasllama/dasllama` (or
`dasllama/dasllama_transformer`), never from the engine file that defines it.** A symbol it does not
re-export may be required directly; engine internals, in-module tests, harnesses and benchmarks may
too. A split that spreads facade-reachable requires instead of fixing the re-export is a defect.

**An `[init]`-only side-effect require lives in `dasllama_transformer.das`** — arch registrations, GPU
tiers, every module requiring the engine back; it sits in `dasllama_common.das` only if engine code
needs it. Moving one restates every comment and `ARCHITECTURE.md` line naming its old home.

**`dasllama_env.das` — every environment knob's single home.** All `[EnvConfig]` area structs and
their `g_env_*` globals live here; `ENVIRONMENT.md` generates from them, and
`tests/test_env_registry.das` enforces both directions. A knob declared anywhere else is a defect.

**A NEW `[EnvConfig]` area struct is wired into `env_markdown()` AND `registered_env_names()` in the
same change, and `ENVIRONMENT.md` is regenerated.** Those two lists are hand-maintained: a new area
struct whose knobs do not appear in `ENVIRONMENT.md` is a defect.

**A new module file is registered in `.das_module` in the same change.** The install rule is a
directory glob; the `ADD_MODULE_DAS` list in `CMakeLists.txt` is a subset, touched only on join.

**`performance/exchange_schema.das` is the single validator for exchange submissions — record stores
and tune sidecars — and stays engine-free.** A second validator, or a dasLLAMA/dasLLVM require added
to it, is a defect; a change to what the writers emit keeps `tests/test_exchange_schema.das` green.

**`performance/exchange_client.das` is the single exchange client — every HTTP call to the sidecar
exchange (lookup, download, submit) goes through it.** Invariants: everything downloaded passes the
client-side gate (content sha, schema, `DASLLAMA_VERSION`) first; every submission goes through
`exchange_strip_private`; exchange-sourced and foreign-box sidecars are never submitted; a lookup
failure never kills a boot. A second HTTP path, or a submission around the strip, is a defect.

**`performance/fetch_models.das` is the model-provenance manifest** — an entry is the HF repo +
revision pin, canonical bytes + sha256, or a conversion recipe (`ARCHITECTURE.md` §1). A model
referenced anywhere without its entry is a defect. `--fetch` downloads only: a convert, a bench,
or a tune-state write added to it is a defect.

### Engine

- `dasllama.das` — the public API surface and its re-exports.
- `dasllama_version.das` — the `DASLLAMA_VERSION` release counter, and nothing else.
- `dasllama_common.das` — engine types, forward loops, override registries, runtime knobs. No NEW
  platform-specific code or platform-guarded require, and no load walk.
- `dasllama_load.das` — the GGUF load walk: metadata to `Config`, plane layout, format detection, the
  eager and streamed conversion ladders, and the load entry points.
- `dasllama_transformer.das` — block composition, and the require umbrella: the `[init]`-only
  side-effect requires that cannot sit in `dasllama_common.das`.
- `dasllama_blocks.das` — the std/dense/MoE block kernels and the default block sets, reached only
  through the `ArchBlocks` pointers. A block kernel called by name from another engine file is a
  defect.
- `dasllama_moe.das` — MoE routing and expert dispatch, reached only through `moe_ffn_core`.
- `dasllama_attn_prefill.das` — prefill attention. `prefill_attention` is the only entry; a caller of
  a head kernel below it is a defect.
- `dasllama_batch.das` — the batched decode step.
- `dasllama_sampling.das` — sampling and the generation drivers; the engine never calls back in.
- `dasllama_ple.das` — gemma-4 per-layer embeddings and MoE FFN, reached only through its hooks.
- `dasllama_config.das` — every input that changes `.dlim` image bytes, and its identity formatter.
- `dasllama_chat.das` — turn assembly and reply-stream splitting.
- `dasllama_tools.das` — tool wire text, both directions: building definitions and results, parsing
  calls out of replies. A literal lives with its job: what a tool call looks like on the wire is
  tools'; where turns begin and end is chat's.
- `dasllama_scheduler.das` — the continuous-batching serving layer OVER the facade, requiring only
  `dasllama/dasllama`: admission, batched decode, bounded prefill chunks, prefix-page donation,
  `SchedEvent` results. Engine or HTTP/writer logic here is a defect.
- `dasllama_par.das` — the parallel-for macro.
- `dasllama_prefix.das` — the prefix cache for evaluated token history.
- `dasllama_parity.das` — CPU reference caches for parity instruments.
- `dasllama_env.das` — the environment-knob registry; `ENVIRONMENT.md` generates from it.

### Formats and data movement

- `dasllama_kqformat.das` — format identity: the format enum, per-format descriptors, predicates.
- `dasllama_convert.das` — every tensor format conversion, any platform, any caller; also owns what
  the Q8_0/Q4_0 format IS (block geometry, scales) and the quant-quality detector.
- `dasllama_repack.das` — every disk-order to compute-order kernel-layout transform.
- `dasllama_kv_codec.das` — the KV-cache runtime codec: one family per cache format, kept whole.
- `dasllama_rope.das` — RoPE angle and table generation.
- `dasllama_gguf.das` — the GGUF container and its byte readers.
- `dasllama_layout.das` — disk-format to compute-layout transforms at load scope.
- `dasllama_image.das` — the prepared-model image rail.
- `dasllama_plane.das` — borrowed-plane types and accessors; only this file binds, reads, drops one.
- `dasllama_tokenizer.das` — the tokenizer facade: backend selection and the encode/decode/piece
  surface. A backend algorithm here is a defect.
- `dasllama_spm.das` — the SentencePiece backend.
- `dasllama_bpe.das` — the byte-level BPE backend: vocab, byte alphabet, ranked merges, and the
  `pre`-name selector inside `bpe_encode`. A pre-tokenizer split here is a defect, except the gemma-4
  newline-run split in `bpe_encode_spm_space`.
- `dasllama_pretok.das` — the pre-tokenizer: one split function per family. A family-name test
  elsewhere in `dasllama/` is a defect, except the `bpe_encode` selector and
  `load_bpe_tokenizer_gguf`'s metadata defaults.
- `dasllama_unicode.das` — the transcoded unicode RANGES/WS tables and their lookups.

### CPU kernel tiers

- `dasllama_math.das` — the numeric abstraction: typedefs, active backend pointers, public wrappers,
  dispatch shaping (jobque caps, OS-conditional scheduling defaults). No kernel bodies.
- `dasllama_math_default.das` — the portable kernel backend.
- `dasllama_math_aarch64_neon.das` — the arm64 kernel backend.
- `dasllama_math_accelerate.das` — the Accelerate/BNNS float tier.
- `dasllama_math_gen.das` — the generated GEMM tier's runtime registration.
- `dasllama_gemm_gen.das` — the GEMM tile generator.
- `dasllama_gemm_schema.das` — the layout and permutation schema shared by generator and runtime.
- `dasllama_gemm_register.das` — tune-family registration.
- `dasllama_tune.das` — per-box kernel tuning policy; tuned values live in the sidecar, not source.

### GPU

A backend is a family of role files, and the role names the contents. `<gpu>` is `metal` or `vulkan`.

- the kernel home — kernel source, no device state: `dasllama_metal_kernels.das` (Metal),
  `dasllama_vulkan_classes.das` (Vulkan, each kernel a `[spirv_kernel]`/`[vk_dispatch]` class). Census
  tables and report are `<gpu>_common`'s.
- `dasllama_vulkan_dispatch.das` — the `[vk_dispatch]` structure macro: the `ensure_*`/`set_*`/`enc_*`
  surface, per-binding access, the census seed. Macro code only; a kernel body or device call here is
  a defect.
- `dasllama_vulkan_seams.das` — the per-op class-rail seams (`vk_add_rms`, `vk_rope_kv_store`,
  `vk_decode_attn`).
- `dasllama_<gpu>_common.das` — device state, buffer/command plumbing, hazard/capture rail, profiler.
- `dasllama_<gpu>_decode.das` — the resident token-step driver and its decode-time arms.
- `dasllama_<gpu>_prefill.das` — the batched prefill driver and its batch arms.
- `dasllama_metal_shapes.das` — model-shape servability gates, portable: no GPU requires, so any box
  can bake. Vulkan has no shapes file (`ARCHITECTURE.md` §1.5).
- `dasllama_metal_lens.das` — the `[metal_dispatch]` structure macro: generates the `enc_*` builders,
  derives per-field access (`kernel=` picks the method; one lens instance per kernel), runs the
  manual-dispatch census. Macro code only; a kernel body or device call here is a defect.
- `dasllama_math_vulkan.das` — the Vulkan family entry: probe, arm, image identity, routers, init
  hooks. Its name is a require contract; renaming it is a defect.
- `dasllama_metal_gemm.das` — the Metal batch-GEMM donor.
- `dasllama_gpu_tier.das` — the device-cooperation SPI: hook types, install slots, status.
- `dasllama_kernel_access.das` — the shared read/write classifier both lenses run on, plus the
  dispatch-lens micro-grammar (grid/tg/params spec parsing and emission). A grammar helper duplicated
  into a lens file is a defect.
- `dasllama_gpu_resident.das` — the whole-model GPU residency rail: flavor bake, stack upload,
  device-resident overrides. No device call, GPU require, or device arm (`ARCHITECTURE.md` §1.5).

A backend-only capability goes in that backend's matching role file. A new grab-bag file for it is a
defect.

**A GPU family shares ONE device and queue** from `<gpu>_common`'s init, except
`dasllama_metal_gemm.das`'s second device+queue (`ARCHITECTURE.md` §1.5). Any other module creating
one is a defect.

**A PSO is compiled and released by the file that owns its kernel class**, through its init/release
pair — `metal_decode_init` / `metal_kernels_release` in `dasllama_metal_kernels.das`,
`metal_prefill_init` / `metal_prefill_shutdown` in `dasllama_metal_prefill.das`. A PSO built or
released elsewhere is a defect; a kernel-unit gate that compiles and releases its own short-lived
pipeline is exempt.

**Race code for a kernel family lives beside the family** — kernels races its families, prefill races
its own; the shared scaffolding (`race_buf`, `race_envelope_ok`, `race_pair_ms`) is `<gpu>_common`'s.
A race harness anywhere else is a defect.

**A Metal decline reason is a `MetalDecodeDecline` / `MetalPrefillDecline` value in
`dasllama_metal_shapes.das`; decline counting lives in `dasllama_metal_common.das`.** A string-typed
metal decline, or a counter beside the decline site, is a defect.

**The backend asymmetries are the closed list in `ARCHITECTURE.md` §1.5.** A diff that changes how
Metal and Vulkan differ — adding or removing an asymmetry — lands its §1.5 edit in the same change.

### Model families

- `dasllama_arch_*.das` — one file per architecture family, declarative registration only. An
  architecture that changes a forward loop, or tests a name on a shared path, is a defect.

### Audio and ASR

- `dasllama_asr_types.das` — the shared ASR floor: `AsrCaps`/`AsrTimestamps`/`TranscribeSegment`.
- `dasllama_audio.das` — the shared audio tower, the mel/FFT machinery, the encoder block loop.
- `dasllama_audio_io.das` — audio decode to PCM. The only file that talks to the audio library.
- `dasllama_asr.das` — the ASR facade: the sniffing loaders, the model/session unions, one-call
  dispatch arms. Family names never appear in its public API.
- `dasllama_whisper.das`, `dasllama_parakeet.das`, `dasllama_canary.das`, `dasllama_qwen3a.das`,
  `dasllama_gemma4a.das` — one file per model family: weights, encoder, session bundle, caps sheet,
  transcribe driver, quirks. Shared tower pieces move up into `dasllama_audio.das`, never sideways.
- `dasllama_vad.das` — voice-activity detection weights and stream state.

### Vision

- `dasllama_vision.das` — the preprocessing rail: dynamic-resolution geometry, the letterbox
  resize, normalize. The only preproc home.
- `dasllama_vision_io.das` — image decode to RGB8 and the debug PNG dump. The only file that talks
  to stbimage.
- `dasllama_gemma4uv.das` — the gemma4uv embedder: mmproj load, im2col, the norm/GEMM/pos-table
  forward. One file per vision projector family, the audio tower pattern.

**A weight plane's element type follows its SOURCE tensor, per tensor.** A carrier reads a bf16
tensor as bf16 and an fp32 tensor as fp32 — never rounds one down to match the other, and never
decides the question for a whole file (gemma-4's shipped "BF16" mmproj is F32 for the patch
embedder and BF16 only for the projection). A plane split that follows the FILE rather than a
runtime knob takes ONE image tag, with the meta flags describing the layout.

**A vision embedding-parity cell names its fixture and logs the measured maxdiff on green as well
as red**, and every image a test feeds an embedder is either a procedural fixture the test builds
or previewable via the `DASLLAMA_VISION_DUMP` knob — a red never requires adding instrumentation
before a human can see what the model saw.

**A tier-1 vision fixture has an exact-value generator.** The das test regenerates every fixture
from its formula, so a generator running libm transcendentals (atan2, sin) is not float-portable
and its cell is a defect; orientation coverage uses shaped exact fixtures instead.

**A media turn's prompt renders as two token spans around the soft-token splice**
(`render_prompt_media`) — never one stream with a placeholder — so BPE merges cannot cross the
media, and a new media kind adds its marker pair to the chat template rather than a second
renderer. A family whose template declares no marker pair has no arm for that media kind, and
`create_chat_` panics at create.

**A scheduler stream carrying media rows neither reads nor writes the prefix cache.** Cache keys
are token ids and the KV past the splice does not follow from them, so a media stream skips
`prefix_attach` at admit and `donate_stream` at reap. Its rows also eval as ONE prefill quantum:
the non-causal flag is per call, so a chunk boundary inside the span would change the mask.

**A media splice is expressed as two token spans plus a row block, everywhere it appears.** The
engine's `render_turn_image_`, the scheduler's `(prompt, media_at, media)` triple and the server's
request path all carry the same shape; a second representation — a placeholder token, a
pre-flattened embedding buffer at the seam — is a defect. A new media kind adds its render seam
beside the existing one, not a parallel prefill path.

**A verb arm in `dasllama_asr.das` is one forwarding call.** A new family touches the facade only at
the union field, the finalize line, the `AsrKind` value, and the one-line arms; a prompt, a decode
loop, a caps value, or a language rule in the facade is a defect.

**A GEMM in a family file goes through a `*_mm` wrapper or `mm_blob_b`.** A hand-written dot-product
loop beside them is a defect.

**Every `*_encode` and `*_log_mel` in `dasllama_audio.das` and the family files carries `[hot_path]`
and lints at zero.** Reused buffers take `@scratch`, debug and profiling legs `[cold_path]`; a nolint
where either fits is a defect.

**A mel frontend builds on the FFT-plan machinery in `dasllama_audio.das`.** A hand-rolled DFT in a
family file is a defect.

**Every family has a token-for-token oracle cell, and every oracle cell logs its transcript as an
`eyeball:` line.** An id-only comparison is a defect.

**A `create_session` / `transcribe` option that the model's `caps()` does not declare panics at the
call site** (`dasllama_asr.das`). Accepting it and silently ignoring it is a defect.

### Generated

- `ENVIRONMENT.md` — generated by `harness/gen_env_doc.das`; it is regenerated in the same change
  as whatever moved its inputs (`tests/test_env_registry.das` fails on drift).
  Hand-editing it is a defect, as is editing
  `dasllama_unicode.das`'s RANGES/WS tables, transcoded from llama.cpp's unicode-data.cpp;
  retranscode.

---

## File and memory model

**Runtime serves weights out of a mapped `.dlim`.** A live carrier's planes point into `parse_image`'s
mapping, and going live does no real work — repacking, quantizing, folding, permuting belong to the
mint. A transform on the go-live path is a defect (`ARCHITECTURE.md` §2.1).

**A missing `.dlim` is minted first, and the model is served from what was minted.** A load path that
falls back to reading weights out of the source file into a live carrier is a defect, as is one
serving a carrier the mint did not produce.

**There is one way to mint, and one way to load.** A weight carrier becomes live through `build_image`
and `parse_image` in `dasllama_image.das`. Reading weights into a live carrier, or releasing an image
backing, anywhere else is a defect — and a second mint path, per family, per format, or per backend,
is a defect even where its output is identical (`ARCHITECTURE.md` §2.1).

**A mint never holds the whole model.** It sizes the image before the first byte goes out and writes
each plane as it is produced. Keeping the source model resident to write from is a defect, and a mint
that is slower in exchange for a lower peak is correct (`ARCHITECTURE.md` §3).

---

## Image identity and contents

**A `.dlim` is box- and config-specific, not a portable format.** `image_identity` names the box
profile, the knobs, and the flavor a file was baked for, and a mismatch declines loudly. A path that
reinterprets a mismatched image, or widens an identity so that more files match, is a defect.

**A bake reaps only its own lane** — an identity's (quant, tag) pair. A save may drop that lane's dead
siblings plus BROKEN/version-stale images in any lane, nothing else (`ARCHITECTURE.md` §2.1).

**Only a process that can recompute an image's identity may judge it dead.** Reaping an image whose
identity the code cannot recompute — another flavor's, another family's — is a defect, with one
carve-out: `dlim_wipe` from `gen_bench_records.das`. Any other `dlim_wipe` caller is a defect.

**An image carries only what its flavor uses.** A plane the target platform or config never reads is
not written — the mint decides, not the load. A flavor takes its file through `image_path_for` and its
tag through `register_image_family_tag`; carrying another flavor's planes is a defect.

---

## Documentation

**A change to user-facing API checks every tutorial and document that touches it.** User-facing means
anything a consumer outside this repo calls or types — exported facade functions, CLI flags,
environment knobs, file formats, defaults — plus the in-repo rig and tool surface: flags, inputs, any
output another tool parses (console-only diagnostics excepted). The check covers the tutorial `.das`
sources, their `.rst` pages, and `BRINGUP.md` / `PROFILE.md` / `METHODOLOGY.md` / `ARCHITECTURE.md`;
one still showing the old call, flag, or default is a defect of the change, not of the docs.

**A changed default is restated everywhere the old default was stated.** Every docstring, help string,
and doc line that named the old value names the new one in the same diff.

## Implementation

**Any kernel work bumps `DASLLAMA_VERSION` (`dasllama_version.das`) in the same change.** Kernel work
adds, removes, or edits a compute-kernel body or variant set — CPU tiers, generated GEMM families, GPU
kernel classes, KV-codec and convert kernels — or a tune family's registration.

**A kernel's shape is compile-time; only its data is runtime.** For a given compiled kernel, can this
value change between dispatches? If yes it is data and belongs in a uniform or a kargs struct; if no
it is shape — a block stride, a lane width, an unroll factor, a format selector — and it must not
reach the kernel as a uniform, a kargs field, or a helper parameter (`ARCHITECTURE.md` §2.2).

**A kernel body contains no indirection.** No function pointers, no vtables.

**A claim about a shape constant is checked against the emitted shader, not the das source.** Read the
generated `*_msl` global or the SPIR-V dump and confirm the constant is literal there.

**Twins of a kernel family bind the same kargs type at the same binding**, even where one twin ignores
a field; shifting the other's fields to different slots is a defect. Recorded exception: the moe
mul_mm TENSOR twins (`MetalMoeMulMmQ8T` / `MetalMoeMulMmMx4T`) keep the pre-family compact slots while
their base classes bind the family numbers; no shared bind path may span them.

**Kernel twins share a template.** Same-body single/batch or format twins stamp one `class template`:
a `@template_constant` axis carries body divergence, a `@template_gate` field a stamp-varying binding.
A copy-pasted twin, or a dummy-bound field where a gate serves, is a defect.

**Every weight, bias, or lookup-table field on a `[metal_dispatch]` / `[vk_dispatch]` kernel class
declares `@role = "weight"` explicitly.** An un-roled weight-shaped field is a defect even when the
kernel compiles and passes parity. Un-lensed lab classes in `benchmarks/` are out of scope.

**A kernel declares its dispatch on the class; the builder is generated.** A new kernel class carries
`[metal_dispatch]` / `[vk_dispatch]` with per-field `@binding` / `@role` / `@off` / `@span` /
`@default`. A NEW hand-written `enc_*` body is a defect unless it is a wrapper shape: a format or twin
pick, a default-filling wrapper, or a composite over generated builders.

**No value reaches an encoder twice.** A scalar uniform buffer passed alongside the identical value as
a parameter is a defect, as is a kargs field the fields beside it determine. Carve-out: a builder
parameter the `grid=`/`tg=` spec consumes HOST-side may repeat a kargs field read device-side.

**Nothing in the engine or a race harness dispatches a kernel except its `enc_*` builder.** A
hand-rolled bind list in `dasllama/` or `performance/` is a defect; labs under `benchmarks/` are the
recorded exception, and `tests/` kernel-unit gates hand-bind on purpose. Carve-out: the IN-ENGINE moe
mul_mm A/B race harnesses (`dasllama_metal_prefill.das`) encode through `kn_moe_mm_family_tail`.

**A cache keyed by a host address carries the span and the form in its key.** A hit must cover the
request, and different upload forms live in separate tables.

**A predicate answering "can this run" must not also answer "is this ready".**

**Peak memory wins ties against load cost.** A change to an allocation reached from a load, bake, or
convert path (judge a shared helper at each call site) that trades footprint for speed ships the
measured pair — peak footprint and wall-clock — and an explicit call (`ARCHITECTURE.md` §3).

**A complexity or length warning is a prompt to look, not an order to split.** An irreducible shape
takes a suppression with a one-line reason. Splitting where no seam exists — helpers that only lower a
number — is a defect, as is suppressing a function already ledgered as wanting a dedup.

**New code meets the complexity and length limits from the first line.** The suppression is for shapes
that cannot reduce, not for code written oversized.

**Platform backends implement narrow registered contracts.** Platform-specific code in a
platform-neutral file is a defect.

**Every program root declares `options stack = 524288`.** A test, harness, benchmark, or tool that
picks its own number is a defect, and so is a new root that omits the declaration. Scope is roots that
require `dasllama/` and export `main`; `tests/test_program_roots.das` enforces exactly that, plus
prefill intent for model-loading roots (`ARCHITECTURE.md` §2.8).

**No ad-hoc profiling.** A NEW clock read paired with a print or log of the elapsed interval is a
defect in engine code — instrumentation goes through the sanctioned rails, and a clock whose value
feeds logic is marked `// clock: control`. The rails and the carve-outs (`benchmarks/`,
`performance/`, `harness/`, cold one-shot load/mint progress logs) are `ARCHITECTURE.md` §2.10.

**Every new kernel or mid-runtime loop is COVERED by an annotated region entry** — `[hot_path]`, or
any of the `[no_alloc]` / `[no_env]` / `[no_io]` contracts. A region entry is a KERNEL
`*_encode` / `*_decode` / step driver; the tokenizer encode/decode path is out of scope. A new
function is a defect only when no annotated region entry reaches it — a new entry point, including a
kernel-backend override or a batch donor, carries an annotation itself (`ARCHITECTURE.md` §2.11).

**A change to `encode`/`bpe_encode` or anything they reach in `dasllama_spm.das` / `dasllama_bpe.das`
/ `dasllama_pretok.das` ships before/after `--tok` rows for the affected backend — the instrument is
the scaling ratio across the size ladder, and superlinear is a defect.** A change to the cell's own
corpus input (`tok_read_seed`, the `TokCorpus` seeds) ships the same rows or a statement that the
bytes are unchanged.

**No raw environment access outside `dasllama_env.das`.** A knob is an `[EnvConfig]` field there, read
as `g_env_*.<field>`; `get_env_variable` / `has_env_variable` / `set_env_variable` / literal-name
`env_config_*` elsewhere is a defect. Sanctioned forms are `ARCHITECTURE.md` §2.9.

**An override announces itself where it changes the outcome.** An override is a gate escape, policy
override, or threshold recalibration (`ARCHITECTURE.md` §2.9). Where one changes what a run measures,
mints, or emits, a printed line names it by env spelling; set-but-inert stays silent, per-site repeats
are fine. Adding one, or giving one a new effect, without the announce is a defect.

### Vulkan

**A pipeline is created only by a `[vk_dispatch]`-generated `ensure_*` and torn down by
`vk_drop_model_state`.** A hand-written pipeline build anywhere else in the engine is a defect.

**A buffer bound as one SSBO range stays under `vk_max_storage_range()`, checked where its size is
NEGOTIATED, not where it binds.** The bind site cannot shrink a buffer that was sized wrong.

**A resident-driver change ships with `harness/parity.das` GPU-vs-CPU runs on one q8 and one kq model,
with `--kv` matching the armed mirror codec** — the vulkan arm is `DASLLAMA_GPU=1`, never `--ngl` (the
Metal arm, which panics without Metal). The driver declines codec-mismatched sessions silently, so the
log must show `resident driver armed`.

**A kernel that reads or writes the K/V mirrors is stamped from a `[|> template_struct_instance]`
codec template (`typedef KT`) with both f32 and f16 instances, and an f16 instance's mirror stores
clamp to the f16 finite range.** A single-codec mirror kernel is legal only when a codec-templated
sibling serves the other codec and its arming gate keys on `kv16`; otherwise it, or an f16 store
without the clamp, is a defect.

**Every resident override gates sessions on the armed mirror codec and on the flat (non-paged) cache
before touching the mirror.** Mirror bytes move only between same-codec session rows and mirror rows;
an override that byte-copies across codecs corrupts the host authority and is a defect.

**A descriptor set cached across dispatches lives in state `vk_drop_model_state` clears** — a
`*_ready` latch or a field inside `g_gpu` / the arena.
