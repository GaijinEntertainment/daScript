# dasLLAMA Code Review Checklist

Run this list on every dasLLAMA change before it ships.

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
- **No examples naming current functions, kernels, or types.** Nothing keeps them in sync with
  the code, so a stale example is worse than none. State the shape, not an instance.
- **No history, no rationale, no direction of travel.** The reason lives in `ARCHITECTURE.md`;
  planned work lives in the follow-up ledgers.

---

## Tests

**Run `modules/dasLLAMA/tests` before any PR.** Model and GPU suites go through the scoped
runner `modules/dasLLAMA/tests/run.das`; invoking dastest directly on those suites is a defect,
because the runner's environment is part of the suite's contract.

**Every test runs under `-jit`.** Never the interpreter, never AOT. A test invocation without
`-jit` is a defect even if it passes.

**All dasLLAMA tests live under `modules/dasLLAMA/tests/`.** No dasLLAMA test is registered in
any `CMakeLists.txt` — no AOT registration, no ctest wiring.

**A test passes or skips explicitly on every platform.** A skip goes through a capability or
model gate. A test that silently vanishes on one platform is a defect.

**A test that loads a model over the large-model threshold sits behind the large-model gate**
and runs only as a final pre-PR gate, never in the iteration loop. Check what a test loads
before launching it.

**Every moved or extracted bit ships a test for the bit itself** — feed the function, check the
bytes. "The model still runs" is not a test for a move.

**Every test that compares logits also logs decoded text for both sides.** A red, or a
suspicious green, must be readable as text in the log, not only as an id or float difference.

**A new GPU kernel ships with a small model in the kernel coverage suite** that dispatches it.
A kernel no suite reaches is unreviewable.

**A test suite loads models with `load_model_`, never the image rail.** Image-rail coverage
belongs to the image suites alone. See `ARCHITECTURE.md` §2.1.

---

## Placement — one file, one rule

Every file states what it holds. Code that belongs to a file and is written anywhere else is a
defect, and this section is the whole test — a rule here names only its own file's contents,
never where a neighbouring concern lives. `ARCHITECTURE.md` §1 carries the boundaries and the
carve-outs.

**A new file ships with its rule here, its charter in `ARCHITECTURE.md` §1, and its tests, in
the same change.** A file without its records is a defect.

### Engine

- `dasllama.das` — the public API surface and its re-exports.
- `dasllama_common.das` — engine types, forward loops, override registries, the load walk. No
  platform-specific code.
- `dasllama_transformer.das` — block composition.
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
- `dasllama_repack.das` — every disk-order to compute-order kernel-layout transform.
- `dasllama_kv_codec.das` — the KV-cache runtime codec: one family per cache format, kept whole.
- `dasllama_rope.das` — RoPE angle and table generation.
- `dasllama_gguf.das` — the GGUF container and its byte readers.
- `dasllama_layout.das` — disk-format to compute-layout transforms at load scope.
- `dasllama_image.das` — the prepared-model image rail.
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

A backend-only capability goes in that backend's matching role file. A new grab-bag file for it
is a defect.

### Model families

- `dasllama_arch_*.das` — one file per architecture family, declarative registration only: build
  the descriptor, register it at init. An architecture that changes a forward loop, or a name
  test on a shared path, is a defect.

### Audio and ASR

- `dasllama_audio.das` — the shared audio tower.
- `dasllama_audio_io.das` — audio decode to PCM. The only file that talks to the audio library.
- `dasllama_asr.das` — the ASR facade and capability declaration.
- `dasllama_whisper.das`, `dasllama_parakeet.das`, `dasllama_canary.das`, `dasllama_qwen3a.das`,
  `dasllama_gemma4a.das` — one file per model family: its weights, its decode loop, its quirks.
  Shared tower pieces move up into `dasllama_audio.das`, never sideways between families.
- `dasllama_vad.das` — voice-activity detection weights and stream state.

### Generated

- `dasllama_env.das`, `dasllama_unicode.das` — generated. Editing one by hand is a defect; edit
  its generator.

---

## Implementation

**A kernel's shape is compile-time; only its data is runtime.** The test is one question: for a
given compiled kernel, can this value change between dispatches? If yes it is data and belongs in
a uniform or a kargs struct. If no it is shape — a block stride, a lane width, an unroll factor,
a format selector — and it must not reach the kernel as a uniform, a kargs field, or a helper
parameter. See `ARCHITECTURE.md` §2.2.

**A kernel body contains no indirection.** No function pointers, no vtables.

**Claims about a shape constant are checked against the emitted shader, not the das source.**
A helper that looks specialized in das can still lower to a runtime multiply.

**Twins of a kernel family bind the same kargs type at the same binding**, even where one twin
ignores a field. A twin that shifts the other's fields to different slots is a defect.

**No value reaches an encoder twice.** A scalar uniform buffer passed alongside the identical
value as a parameter is a defect; so is a kargs field the fields beside it already determine.

**Nothing dispatches a kernel except its builder.** A hand-rolled bind list anywhere else is a
defect — it duplicates the builder and desyncs silently when the family's arguments change.

**A cache keyed by a host address carries the span and the form in its key.** A hit must cover
the request, and different upload forms live in separate tables.

**There is one way to load a model.** Nothing outside the image rail may read weights into a
live carrier or release an image backing. See `ARCHITECTURE.md` §2.1.

**A predicate answering "can this run" must not also answer "is this ready".** A caller that
runs before setup finishes gets a permanent no, and the feature silently never runs.

**Peak memory wins ties against cold-start latency.** A change going the other way ships the
measured pair — peak and wall-clock — and an explicit call, not an assumption that faster is
better.

**A complexity or length warning is a prompt to look, not an order to split.** An irreducible
shape takes a suppression with a one-line reason. Splitting where no seam exists — helpers that
exist only to lower a number — is a defect, and so is suppressing a function already ledgered as
wanting a dedup.

**New code meets the complexity and length limits from the first line.** The suppression is for
shapes that cannot reduce, not for code written oversized.

**Platform backends implement narrow registered contracts.** Platform-specific code in a
platform-neutral file is a defect.
