# dasLLAMA Code Review Checklist

Run this list on every dasLLAMA change before it ships — including changes to this file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code, without prior
knowledge of the module, and without opening another document.** If an entry needs any of
those, it is not a review criterion — move it to `ARCHITECTURE.md` and leave a one-line
criterion here.

**Form, and it is a hard limit:**

- **One rule is one short paragraph.** An entry that needs more than that is describing how to
  write code, not how to review it. Split it or move it.
- **No numbers.** These are criteria, not a spec, and numbering invites citation. Anything that
  needs a stable reference lives in `ARCHITECTURE.md`, which is numbered for that purpose.
- **Cite files by name; cite `ARCHITECTURE.md` by section.** Never cite an entry in this file.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside ("the way the X family does it") has no such excuse: nothing keeps it in
  sync with the code, and a stale example is worse than none.
- **No history, no rationale, no direction of travel.** The reason lives in `ARCHITECTURE.md`;
  planned work lives in the follow-up ledgers.

---

## Tests

**Run `modules/dasLLAMA/tests` before any PR.** Model and GPU suites go through the scoped
runner `modules/dasLLAMA/tests/run.das`; invoking dastest directly on those suites is a defect.

**Every test runs under `-jit`.** Never the interpreter, never AOT. A test invocation without
`-jit` is a defect even if it passes.

**All dasLLAMA tests live under `modules/dasLLAMA/tests/`.** No dasLLAMA test is registered in
any `CMakeLists.txt` — no AOT registration, no ctest wiring.

**A test passes or skips explicitly on every platform.** A skip goes through a capability or
model gate. A test that silently vanishes on one platform is a defect.

**A test that loads a model over 6 GiB sits behind `model_available`**, which runs it only
under `DASLLAMA_PARITY_FULL=1` — a final pre-PR gate, never the iteration loop. Check what a
test loads before launching it.

**Every moved or extracted bit ships a test for the bit itself** — feed the function, check the
bytes. "The model still runs" is not a test for a move.

**Every test that compares logits also logs decoded text for both sides.** A red, or a
suspicious green, must be readable as text in the log, not only as an id or float difference.

**A new GPU kernel ships with a small model in the kernel coverage suite** that dispatches it.

**A test suite loads models with `load_model_`, never the image rail.** Image-rail coverage
belongs to the image suites alone. See `ARCHITECTURE.md` §2.1.

**A measured number proves its kernel provenance, in whatever world it ran.** The bench gate has
one arm per world: a standalone exe checks the sidecar the release shipped beside it, a
`DAS_TUNE_MANIFEST` run checks that file, a plain script checks that every `[tune]` row stamps a
manifest winner. A rig invocation that no arm covers is a defect — it will refuse, or worse,
measure on fallback kernels.

**No new benchmark harness is written.** Performance is measured by
`modules/dasLLAMA/benchmarks/lcpp_bench.das` — one cell directly, or a whole board through
`performance/gen_bench_records.das`. A new timing harness, a one-off measurement script, or a
revived rig is a defect. `PROFILE.md` carries the two commands; the rig's shape is
`ARCHITECTURE.md` §2.5.

---

## Placement — one file, one rule

Every file states what it holds. Code that belongs to a file and is written anywhere else is a
defect, and this section is the whole test — a rule here names only its own file's contents,
never where a neighbouring concern lives. `ARCHITECTURE.md` §1 carries the boundaries and the
carve-outs.

**A new file ships with its rule here, its charter in `ARCHITECTURE.md` §1, and its tests, in
the same change.** A file without its records is a defect.

**A consumer requires the facade, never an engine internal.** Tests, harnesses, benchmarks and
tools require `dasllama/dasllama` or `dasllama/dasllama_transformer`, which re-export the engine;
a direct require of an internal module from outside `dasllama/` is a defect, and so is a split
that adds requires across the tree instead of fixing the facade re-export. Engine internals may
require each other.

**A new module file is registered in `.das_module` and `CMakeLists.txt` in the same change.**

### Engine

- `dasllama.das` — the public API surface and its re-exports.
- `dasllama_common.das` — engine types, forward loops, override registries, runtime knobs. No
  platform-specific code, and no load walk.
- `dasllama_load.das` — the GGUF load walk: metadata to `Config`, plane layout, format detection,
  the eager and streamed conversion ladders, and the load entry points.
- `dasllama_transformer.das` — block composition.
- `dasllama_blocks.das` — the std/dense/MoE block kernels and the default block sets. `forward()`
  reaches them only through the `ArchBlocks` pointers; a block kernel called by name from
  `dasllama_common.das` is a defect.
- `dasllama_moe.das` — MoE routing and expert dispatch, reached only through `moe_ffn_core`.
- `dasllama_attn_prefill.das` — prefill attention. `prefill_attention` is the only entry; a caller
  of a head kernel below it is a defect.
- `dasllama_batch.das` — the batched decode step.
- `dasllama_sampling.das` — sampling and the generation drivers; the engine never calls back in.
- `dasllama_ple.das` — gemma-4 per-layer embeddings and its MoE FFN, reached only through the
  hooks it registers.
- `dasllama_config.das` — every input that changes `.dlim` image bytes, and its identity
  formatter.
- `dasllama_chat.das` — conversation turns and chat-template application.
- `dasllama_par.das` — the parallel-for macro.
- `dasllama_prefix.das` — the prefix cache for evaluated token history.
- `dasllama_parity.das` — CPU reference caches for parity instruments.

### Formats and data movement

- `dasllama_kqformat.das` — format identity: the format enum, per-format descriptors, format
  predicates.
- `dasllama_convert.das` — every tensor format conversion, any platform, any caller.
- `dasllama_quant.das` — what the Q8_0 format IS (block geometry, scales) and the quant-quality
  detector; a conversion that uses the format belongs in `dasllama_convert.das`.
- `dasllama_repack.das` — every disk-order to compute-order kernel-layout transform.
- `dasllama_kv_codec.das` — the KV-cache runtime codec: one family per cache format, kept whole.
- `dasllama_rope.das` — RoPE angle and table generation.
- `dasllama_gguf.das` — the GGUF container and its byte readers.
- `dasllama_layout.das` — disk-format to compute-layout transforms at load scope.
- `dasllama_image.das` — the prepared-model image rail.
- `dasllama_plane.das` — the borrowed-plane types and their accessors, and nothing else. A
  weight carrier holds planes; only this file knows how one is bound, read, or dropped.
- `dasllama_tokenizer.das` — the SentencePiece tokenizer.
- `dasllama_bpe.das` — the byte-level BPE tokenizer.

### CPU kernel tiers

- `dasllama_math.das` — the numeric abstraction: typedefs, active backend pointers, public
  wrappers. No kernel bodies.
- `dasllama_math_default.das` — the portable kernel backend.
- `dasllama_math_aarch64_neon.das` — the arm64 kernel backend.
- `dasllama_math_accelerate.das` — the Accelerate/BNNS float tier.
- `dasllama_math_gen.das` — the generated GEMM tier's runtime registration.
- `dasllama_gemm_gen.das` — the GEMM tile generator.
- `dasllama_gemm_schema.das` — the layout and permutation schema shared by generator and runtime.
- `dasllama_gemm_register.das` — tune-family registration.
- `dasllama_tune.das` — per-box kernel tuning policy. Tuned values live in the box sidecar, never
  in source.

### GPU

A backend is a family of role files, and the role names the contents. `<gpu>` is `metal` or
`vulkan`.

- `dasllama_<gpu>_kernels.das` — kernel source and the dispatch census. No device state.
- `dasllama_<gpu>_common.das` — device state, buffer and command plumbing, the hazard and capture
  rail, the profiler.
- `dasllama_<gpu>_decode.das` — the resident token-step driver and its decode-time arms.
- `dasllama_<gpu>_prefill.das` — the batched prefill driver and its batch arms.
- `dasllama_<gpu>_shapes.das` — model-shape servability gates, portable: no GPU requires, so any
  box can bake.
- `dasllama_<gpu>_lens.das` — the kernel-access macro.
- `dasllama_math_vulkan.das` — the Vulkan family entry: probe, arm, image identity, routers,
  init hooks. Its name is a require contract; renaming it is a defect.
- `dasllama_metal_gemm.das` — the Metal batch-GEMM donor.
- `dasllama_gpu_tier.das` — the device-cooperation SPI: hook types, install slots, status.
- `dasllama_kernel_access.das` — the shared body-walk read/write classifier both lenses run on.
- `dasllama_gpu_resident.das` — the whole-model GPU residency rail: the flavor bake, the stack
  upload, and the device-resident decode/prefill overrides. No device call and no GPU require
  belongs here; a device-specific arm is a defect. See `ARCHITECTURE.md` §1.5.

A backend-only capability goes in that backend's matching role file. A new grab-bag file for it
is a defect.

**A GPU family shares ONE device and queue**, created by `<gpu>_common`'s init. A module
creating its own device is a defect.

**A PSO set is compiled and released by the module that owns its kernels.** Decode:
`metal_decode_init` / `metal_kernels_release` in `dasllama_metal_kernels`. Prefill:
`metal_prefill_init` / `metal_prefill_shutdown` in `dasllama_metal_prefill`. A PSO compiled or
released anywhere else is a defect.

**Race code for a kernel family lives beside the family** — kernels races its families, prefill
races its own; the shared scaffolding (`race_buf`, `race_envelope_ok`, `race_pair_ms`) is
`<gpu>_common`'s. A race harness anywhere else is a defect.

**A decline reason is an enum value in the shapes module; decline counting lives in
`<gpu>_common`.** A string-typed decline reason, or a counter beside the decline site, is a
defect.

**The backend asymmetries are the closed list in `ARCHITECTURE.md` §1.5.** A diff that makes
Metal and Vulkan differ in a new way lands with its entry there, or it is a defect.

### Model families

- `dasllama_arch_*.das` — one file per architecture family, declarative registration only: build
  the descriptor, register it at init. An architecture that changes a forward loop, or a name
  test on a shared path, is a defect.

### Audio and ASR

- `dasllama_asr_types.das` — the shared ASR floor: `AsrCaps`/`AsrTimestamps`/`TranscribeSegment`.
- `dasllama_audio.das` — the shared audio tower, the mel/FFT machinery, the encoder block loop.
- `dasllama_audio_io.das` — audio decode to PCM. The only file that talks to the audio library.
- `dasllama_asr.das` — the ASR facade: the sniffing loaders, the model/session unions, one-call
  dispatch arms. Family names never appear in its public API.
- `dasllama_whisper.das`, `dasllama_parakeet.das`, `dasllama_canary.das`, `dasllama_qwen3a.das`,
  `dasllama_gemma4a.das` — one file per model family: its weights, its encoder, its session
  bundle, its caps sheet, its transcribe driver, its quirks. Shared tower pieces move up into
  `dasllama_audio.das`, never sideways between families.
- `dasllama_vad.das` — voice-activity detection weights and stream state.

**A verb arm in `dasllama_asr.das` is one forwarding call.** A new family touches the facade
only at the union field, the finalize line, the `AsrKind` value, and the one-line arms; a
prompt, a decode loop, a caps value, or a language rule in the facade is a defect.

**A GEMM in a family file goes through a `*_mm` wrapper or `mm_blob_b`.** A hand-written
dot-product loop beside them is a defect.

**Every `*_encode` and `*_log_mel` carries `[hot_path]` and lints at zero.** Reused buffers
take `@scratch`; debug and profiling legs take `[cold_path]`. A nolint where either annotation
fits is a defect.

**A mel frontend builds on the FFT-plan machinery in `dasllama_audio.das`.** A hand-rolled DFT
in a family file is a defect.

**Every family has a token-for-token oracle cell, and every oracle cell logs its transcript
as an `eyeball:` line.** An id-only comparison is a defect.

**An option `caps()` does not declare panics at the call site.** Silently ignoring one is a
defect.

### Generated

- `dasllama_env.das`, `dasllama_unicode.das` — generated. Editing one by hand is a defect; edit
  its generator.

---

## File and memory model

**Runtime serves weights out of a mapped `.dlim`.** A live weight carrier's planes point into the
mapping `parse_image` produced, and going live does no significant work — repacking, quantizing,
folding, and permuting belong to the mint. A transform on the path that makes a carrier live is a
defect. See `ARCHITECTURE.md` §2.1.

**A missing `.dlim` is minted first, and the model is served from what was minted.** A load path
that falls back to reading weights out of the source file into a live carrier is a defect, and so
is one that serves a carrier the mint did not produce.

**There is one way to mint, and one way to load.** A weight carrier becomes live through
`build_image` and `parse_image` in `dasllama_image.das`, and nothing else. Code anywhere else that
reads weights into a live carrier, or releases an image backing, is a defect — and a second mint
path, whether per family, per format, or per backend, is a defect even where it produces an
identical file. See `ARCHITECTURE.md` §2.1.

**A mint never holds the whole model.** It sizes the image before the first byte goes out and
writes each plane as it is produced. Keeping the source model resident to write from is a defect,
and a mint that is slower in exchange for a lower peak is correct. See `ARCHITECTURE.md` §3.

---

## Image identity and contents

**A `.dlim` is box- and config-specific, not a portable format.** `image_identity` names the box
profile, the knobs, and the flavor a file was baked for, and a mismatch declines loudly. A path
that reinterprets a mismatched image, or widens an identity so that more files match, is a defect.

**A bake reaps only its own lane.** An identity's (quant, tag) pair is its lane; a save may drop
that lane's dead siblings plus BROKEN/version-stale images in any lane, and nothing else. Code
that reaps an image whose identity it cannot recompute — another flavor's, another family's, or
any image seen from a sweep orchestrator — is a defect. See `ARCHITECTURE.md` §2.1.

**An image carries only what its flavor uses.** A plane the target platform or config never reads
is not written — the mint decides that, not the load. A flavor takes its own file through
`image_path_for` and its own tag through `register_image_family_tag`; carrying another flavor's
planes alongside its own is a defect.

---

## Implementation

**A kernel's shape is compile-time; only its data is runtime.** The test is one question: for a
given compiled kernel, can this value change between dispatches? If yes it is data and belongs in
a uniform or a kargs struct. If no it is shape — a block stride, a lane width, an unroll factor,
a format selector — and it must not reach the kernel as a uniform, a kargs field, or a helper
parameter. See `ARCHITECTURE.md` §2.2.

**A kernel body contains no indirection.** No function pointers, no vtables.

**A claim about a shape constant is checked against the emitted shader, not the das source.**
Read the generated `*_msl` global or the SPIR-V dump and confirm the constant is literal there;
a helper that looks specialized in das can still lower to a runtime multiply.

**Twins of a kernel family bind the same kargs type at the same binding**, even where one twin
ignores a field. A twin that shifts the other's fields to different slots is a defect.

**No value reaches an encoder twice.** A scalar uniform buffer passed alongside the identical
value as a parameter is a defect; so is a kargs field the fields beside it already determine.

**Nothing dispatches a kernel except its `enc_*` builder.** A hand-rolled bind list anywhere
else — a race harness, a benchmark, a probe — is a defect.

**A cache keyed by a host address carries the span and the form in its key.** A hit must cover
the request, and different upload forms live in separate tables.

**A predicate answering "can this run" must not also answer "is this ready".**

**Peak memory wins ties against the cost of loading a model.** A load-time change that trades
footprint for speed ships the measured pair — peak footprint and wall-clock — and an explicit
call. See `ARCHITECTURE.md` §3.

**A complexity or length warning is a prompt to look, not an order to split.** An irreducible
shape takes a suppression with a one-line reason. Splitting where no seam exists — helpers that
exist only to lower a number — is a defect, and so is suppressing a function already ledgered as
wanting a dedup.

**New code meets the complexity and length limits from the first line.** The suppression is for
shapes that cannot reduce, not for code written oversized.

**Platform backends implement narrow registered contracts.** Platform-specific code in a
platform-neutral file is a defect.

**Every program root declares `options stack = 524288`.** A test, harness, benchmark, or tool
that picks its own number — larger or smaller — is a defect, and so is a new root that omits the
declaration. See `ARCHITECTURE.md` §2.7.
