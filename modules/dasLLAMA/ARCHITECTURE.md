# dasLLAMA Architecture

**Who reads this: me.** It is the memory that survives a compaction, a new session, or a
hand-off — the thing to read when the answer to "where does this code go, and why is it shaped
like this" is no longer in context. It must therefore be readable cold, with no session state:
durable facts only, no history, no PR numbers, no "recently".

The three documents divide as follows, and the division is load-bearing — the same fact written
in two of them will drift:

| doc | audience | carries |
|---|---|---|
| `README.md` | users | what each file *is*, one line, plus how to run things |
| `ARCHITECTURE.md` | me | what *belongs* in each file and why the system is shaped this way |
| `CODEREVIEW.md` | `/code-review`, and us while writing | criteria checkable against a diff |

**References flow one way: CODEREVIEW cites ARCHITECTURE, never the reverse.** Sections here are
numbered so they can be cited (`ARCHITECTURE §2.2`). Nothing here may cite a CODEREVIEW rule —
those are unnumbered review criteria by design, and a citation to one is a dangling pointer the
moment the checklist is reordered.

**Charters below are boundaries, not descriptions.** README already says what every file is;
repeating that here is the failure mode this split exists to prevent. An entry earns its place by
answering a question a description cannot: what does NOT go here, and where does the adjacent
thing live instead. Files with no contested edge get one short line.

---

## 1. File charters

Every non-generated file under `dasllama/` appears here. `dasllama_env.das` and
`dasllama_unicode.das` are generated and have no charter — edit their generators.

### 1.1 Engine core

- **`dasllama.das`** — the public API facade and nothing else: `load_model` → `create_session` →
  generate, re-exported names, the doc surface. No engine logic; a function that does work belongs
  in the module that owns the concern, and the facade re-exports it.
- **`dasllama_common.das`** — the engine: `Model`/`Session`/`Config`, the forward loops, the
  override registries, the runtime knobs. **Not** the load walk (§1.3) and **not** GPU residency
  (§1.5) — both left, and the seam each left behind is a registered hook, so neither comes back.
  It remains the module's debt sink; what sits here that is family-specific or platform-specific is
  debt, **not precedent**. Nothing platform-specific may be added; new shared concerns get their own
  file rather than another thousand lines here.
- **`dasllama_transformer.das`** — the block-composition seam only.
- **`dasllama_blocks.das`** — the std/dense/MoE transformer block kernels, decode and prefill,
  plus the two default block sets the arch files bind. `forward()` never names a kernel here — it
  dispatches through the `ArchBlocks` function pointers, which is why the family lives outside the
  hub with no hook.
- **`dasllama_moe.das`** — MoE expert routing and dispatch: the top-k router, per-expert FFN
  accumulation, the shared expert, and the decode instruments. The block kernels reach it only
  through `moe_ffn_core`.
- **`dasllama_attn_prefill.das`** — prefill attention, threaded over heads. `prefill_attention`
  is the only entry; the classic/blocked/flash head kernels and their KV-codec ladders are its
  implementation.
- **`dasllama_batch.das`** — the batched decode step: one pass of the weights over B sessions,
  GEMVs widened to B-row GEMMs, attention still per-(row, head) against each session's own cache.
- **`dasllama_sampling.das`** — token sampling and the generation drivers. A leaf on top of
  `forward`/`eval_batch`; the engine never calls back in.
- **`dasllama_ple.das`** — gemma-4 E-series per-layer embeddings and the gemma4 MoE FFN. The
  forward sequence reaches it only through the hooks it registers at init.
- **`dasllama_config.das`** — `DlimConfiguration`: every input that changes `.dlim` image BYTES,
  in one struct, plus its identity formatter. A knob that does not change image bytes does not
  belong here; a knob that does and is missing is an image-aliasing bug.
- **`dasllama_chat.das`** — conversation turns and chat-template application. Per-arch template
  *content* is registered by the arch file (§1.6), not written here.
- **`dasllama_par.das`** — `maybe_parallel_for` and nothing else. Threading policy (job counts,
  thresholds) belongs to the caller that knows the shape.

### 1.2 Formats and data movement

These five own the module's data-shape vocabulary. The boundaries between them are the ones most
often gotten wrong, so each says explicitly where the neighbouring half goes.

- **`dasllama_kqformat.das`** — format IDENTITY: the `KqFmt` enum, the per-format descriptor table
  (plane strides, block geometry, stream codes), format predicates. It requires nothing else in
  dasllama, because it is the taxonomy everything keys off. ONE id space — the enum; integer ids
  exist only at the IR/kernel-param boundary. `kq_sb` is the superblock-lattice predicate: a
  `fmt != q8` test does not imply the lattice, so branch on the predicate.
- **`dasllama_convert.das`** — every tensor format CONVERSION: quantize/dequantize/transcode/encode,
  codec byte readers, numeric widen/narrow — regardless of platform or caller. Metadata-coupled
  drivers (GGUF lookup, threading, guards) stay with their containers and dispatch in. ONE
  carve-out: a conversion that IS a KV-cache format's store/read half lives with its codec family
  (§ below).
- **`dasllama_quant.das`** — the Q8_0 format itself (block geometry, scale layout) and the
  quantization-quality detector. Conversions that USE the format live in `dasllama_convert.das`;
  this file owns what the format IS.
- **`dasllama_repack.das`** — every disk-order → compute-order kernel-LAYOUT transform (grp
  interleaves, disk-order extractors, panel unpacks), any format, any platform. Number sources
  (tune stamps, bake overrides) stay with their owners and pass plain parameters in.
- **`dasllama_kv_codec.das`** — the KV-cache runtime codec, one family per cache format, kept
  WHOLE: store (quantize), read (dequant), the attention score dot, and the V-accumulate axpy all
  share the format's block byte geometry, so a layout change can never span modules. These are
  per-token and per-token×position `[tuned]` hot kernels, not load-time converters. The tq4 FWHT
  rotation and sign vector live with their family. Codec DISPATCH (`KVDtype`) stays at common's
  `kv_store_row`/`kv_load_row`/`kv_dot`/`kv_axpy` seam; the f16 row converts are the generic pair
  in `dasllama_convert` because they are dual-use beyond the cache.
- **`dasllama_rope.das`** — RoPE angle and TABLE GENERATION: the theta schedule, `rope_freqs`,
  fscale/mscale, every materialized layout — pure functions over plain parameters. Model-facing
  wrappers stay in common. APPLICATION kernels stay with their backends: the CPU `rope_scaled_*`
  leaves and the GPU fused rope-store kernels are shape-specialized deliberately, and
  single-sourcing them is generator-rail work, never a hand-merge. Float multiply order in the
  builders is contractual (parity-pinned) — never "unify" it.
- **`dasllama_gguf.das`** — the GGUF container: KV/tensor descriptors, the byte-level reader, the
  mapping. Codecs live in `dasllama_convert`; this file finds bytes, it does not decode them.
- **`dasllama_layout.das`** — disk-format → compute-layout transforms at LOAD scope: the blob
  transform, the CPU repack walkers, the GPU tier gathers, and the refusal half (`can this model
  take the blob form`) split out so the image writer can commit without loading.
- **`dasllama_tokenizer.das`** — SentencePiece (Llama-2 family, Phi-3, Gemma).
- **`dasllama_bpe.das`** — byte-level BPE / tiktoken (Llama-3, Qwen2 pre-tokenizers). Two files
  because the two algorithms share no state; a third tokenizer family gets a third file.

### 1.3 The load and image rail

**`dasllama_load.das`** is the GGUF load walk: metadata to `Config`, the plane layout, disk-format
detection, the eager and streamed conversion ladders, and the load entry points. It owns nothing the
forward path touches at run time — a loaded `Model` is the whole handoff — and it requires
`dasllama_common` back for `Model`/`Session`, so the transformer umbrella requires it `public` and
breaks the cycle. That re-export is what keeps every consumer on the facade.

- **`dasllama_image.das`** — the prepared-model `.dlim` rail, and it is ONE rail (§2.1). Nothing
  outside this file may read weights into a live carrier, and nothing outside it may release an
  image backing.

### 1.4 CPU kernel tiers

- **`dasllama_math.das`** — the numeric ABSTRACTION: typedefs, active backend pointers, public
  wrappers, dispatch shaping. Kernels themselves live in a tier file; a kernel body here is a
  placement defect.
- **`dasllama_math_default.das`** — the portable backend, always registered, always correct,
  out-ranked by any platform tier.
- **`dasllama_math_aarch64_neon.das`** — the arm64 SDOT/tbl tier. Its `[init]` never fires
  off-arch (§3, three-layer safety model), so an intrinsic here needs a correct scalar fallback
  body, not a guard at the call site.
- **`dasllama_math_accelerate.das`** — the Accelerate/BNNS float tier (AMX on M1–M3, SME on M4+),
  for genuinely-float planes only. BLAS-for-quant is ruled out structurally (§3).
- **`dasllama_math_gen.das`** / **`dasllama_gemm_gen.das`** / **`dasllama_gemm_schema.das`** /
  **`dasllama_gemm_register.das`** — the generated GEMM tier: the runtime registration, the tile
  generator, the layout/perm schema shared by generator and runtime, and the `[tune]` family
  registration. A hand-written tile that the generator could emit belongs in the generator.
- **`dasllama_tune.das`** — the per-box loop-hint tuner (`[tuned]` / `[dasllama_grid]`). Tuning
  POLICY lives here; tuned VALUES live in the box's sidecar, never in source.

### 1.5 GPU backends

A GPU backend is a FAMILY of role files — matching things in matching files across backends, so
that a question answered for one backend has an obvious address in the other. The roles:

| role | holds | must not hold |
|---|---|---|
| `dasllama_<gpu>_kernels`<br>`dasllama_metal_kernels`, `dasllama_vulkan_kernels` | kernel source, the derived-access/PSO census | device state, engine types |
| `dasllama_<gpu>_common`<br>`dasllama_metal_common`, `dasllama_vulkan_common` | device state, buffer/command plumbing, hazard + capture rail, profiler | driver policy |
| `dasllama_<gpu>_decode`<br>`dasllama_metal_decode`, `dasllama_vulkan_decode` | the resident token-step driver + decode-time arms | kernel bodies |
| `dasllama_<gpu>_prefill`<br>`dasllama_metal_prefill`, `dasllama_vulkan_prefill` | the batched prefill driver + batch arms | kernel bodies |
| `dasllama_<gpu>_shapes`<br>`dasllama_metal_shapes` | PORTABLE servability gates — no GPU C++ require, so any box can bake | device calls |
| `dasllama_<gpu>_lens`<br>`dasllama_metal_lens`, `dasllama_vulkan_lens` | the kernel-access macro | anything else |

- **Vulkan additionally has an ENTRY, `dasllama_math_vulkan.das`** — capability probe/arm, `.dlim`
  identity source, cross-arm routers, the `[init]` installs. It re-exports the family `public`,
  and its NAME is common's `?vulkan` require contract: **never rename it.**
- **Metal has NO `math_` entry** — the family enters via the transformer's `?das_metal` requires
  plus unconditional shapes. Its below-common piece is **`dasllama_metal_gemm.das`** (the batch
  GEMM donor that common requires `?das_metal`), which owns its device by necessity:
  metal_common → dasllama_common → metal_gemm would cycle.
- **Backend-only capabilities live in their matching ROLE file, not in new grab-bags** — vulkan's
  weight arena, streamed mirrors, heat cache, host-import, coopmat; metal's blob transform and MTP.
- **Family-shared kernel classes live in `dasllama_metal_kernels`.** The `[metal_dispatch]` lens
  generates `enc_*` builders and MSL globals into the module the class COMPILES in, so co-location
  follows the class — "the builder needs the driver module" is never a placement reason. Prefill's
  prefill-only classes are convergence debt, not precedent.
- **`dasllama_gpu_tier.das`** — the device-cooperation SPI: hook types, install/unset slots,
  route/mark/want/status state, engine-facing forwarders. Vulkan implements it (per-op offload plus
  resident plumbing); Metal deliberately does not, because UMA makes residency moot there and Metal
  integrates as a whole-forward driver through common's override registries.
- **`dasllama_gpu_resident.das`** — the WHOLE-MODEL residency rail: bake the device layout offline
  into the flavor image, upload a model's stacks to the tier, and drive decode/prefill entirely on
  device. It is device-AGNOSTIC — it holds no device call and requires no GPU module, reaching the
  hardware only through the `dasllama_gpu_tier` SPI and entering the engine only through common's
  override registries. `"vulkan"` is the tier string it registers under, not a dependency, which is
  why it compiles on every box. It requires common back for `Model`/`Session`, so like the Metal
  drivers it is required from the transformer umbrella, never from common.
- **`dasllama_kernel_access.das`** — the shared body-walk read/write classifier both GPU lenses run
  on. Backend-specific lowering stays in that backend's lens.

**PSO lifecycle — the family shares ONE device and queue** (`metal_common_init`; the second-device
question was surveyed and closed against). The decode PSO set lives as `g_pso_*` in
`dasllama_metal_common`, is compiled by `metal_decode_init` in `dasllama_metal_kernels` and
released by `metal_kernels_release` there — the kernels module owns its set's lifecycle even
though the vars live with the device state. Prefill's `g_pf_pso_*` set is prefill-private end to
end: `metal_prefill_init` compiles, `metal_prefill_shutdown` releases.

**Race and tune code for a kernel family lives beside the family.** The shared scaffolding
(`race_buf`, `race_envelope_ok`, `race_pair_ms`, `MetalTensorRaceResult`) is `<gpu>_common`'s;
each module races its OWN families (`metal_tensor_race_decode` in kernels, `metal_tensor_race`
in prefill) and the tuner calls those public entries.

**Decline REASONS are enum values in the shapes module** (`MetalDecodeDecline`,
`MetalPrefillDecline`); decline COUNTING lives in `<gpu>_common` beside `require_or_panic`, for
both paths.

**The allowed asymmetries between the backends — this list is closed; a new one lands with its
entry here:**

- **Metal sits ABOVE `dasllama_common`** (typed `Model`/`Session` access, umbrella entry via the
  transformer's `?das_metal` requires, shapes unconditional); **Vulkan sits BELOW it** (untyped
  pointer/array seams, the single `?vulkan` entry through the `dasllama_math_vulkan` facade).
  That inversion is why their kernels↔common require directions differ.
- **UMA vs discrete VRAM**: Metal never grows residency machinery (memory is memory); arenas,
  upload economics, mirrors and hydration are Vulkan's alone.
- **Lens depth**: the Metal lens generates `enc_*` builders from kernel classes; Vulkan's
  hand-built `vk_set6`/`vk_write6` ladders stand until the class-kernel arc
  (`followup_vulkan.md` item 9) gives SPIR-V the same interface surface.
- **Vulkan has no shapes module yet** — `resident_upload` declines ad hoc by feature name; the
  gap is `followup_vulkan.md` item 1, not a precedent to copy.

Vulkan is the deliberately-designed model of this shape; Metal converges as it is touched.

### 1.6 Architecture registrations

Thirteen files registering eighteen names:
`dasllama_arch_llama.das` · `dasllama_arch_phi3.das` · `dasllama_arch_qwen2.das` · `dasllama_arch_qwen2moe.das` · `dasllama_arch_qwen3.das` · `dasllama_arch_qwen3moe.das` · `dasllama_arch_qwen35.das` · `dasllama_arch_gemma2.das` · `dasllama_arch_gemma3.das` · `dasllama_arch_gemma4.das` · `dasllama_arch_glm4moe.das` · `dasllama_arch_gptoss.das` · `dasllama_arch_mistral3.das`. They are DECLARATIVE: an arch
file builds an `ArchDesc` (name · `configure` · the `ArchBlocks` fn-ptr quad · `ChatTemplate` ·
`LlmCaps`) and calls `register_arch` at `[init]`. Adding an arch touches no forward loop.

Family behavior is distributed by `Config` flag, not dispatched by name — an `if (arch == "...")`
on a shared path is the anti-pattern. Only a genuinely new dataflow earns its own block pointer.
`register_arch` MOVES the descriptor, so an alias must clone the template first (§3).

### 1.7 Audio and ASR

- **`dasllama_audio.das`** — the shared audio tower: mel front-end, encoder blocks, the pieces every
  audio model composes.
- **`dasllama_audio_io.das`** — decode-any-format → 16 kHz mono f32 PCM. The only file that talks to
  miniaudio.
- **`dasllama_asr.das`** — the ASR facade: capability declaration, timestamp granularity, the
  backend-neutral entry points.
- **`dasllama_whisper.das`** / **`dasllama_parakeet.das`** / **`dasllama_canary.das`** /
  **`dasllama_qwen3a.das`** / **`dasllama_gemma4a.das`** — one file per model family, each owning its
  weights, its decode loop, and its quirks. Shared tower pieces go up into `dasllama_audio`, not
  sideways between families.
- **`dasllama_vad.das`** — Silero-VAD weights and per-stream state.

### 1.8 Instrumentation and support

- **`dasllama_parity.das`** — CPU-reference caches for the parity instruments. Test-facing, but
  library-side because the caches outlive a single suite.
- **`dasllama_prefix.das`** — the prefix/page cache for evaluated token history.

---

## 2. Mechanisms

The "why" behind criteria that CODEREVIEW states in one line each.

### 2.1 There is ONE way to load a model

A weight carrier becomes a live struct through exactly two functions here, and nothing else may
read weights into one:

- **`build_image`** walks a carrier's planes into a sink — a `.dlim` file, or a page-aligned memory
  chunk when there is nowhere to write.
- **`parse_image`** turns `(base, bytes)` back into borrowed-plane fields, and does not know which
  sink produced them.

Cold and warm therefore yield the SAME struct. A cold load reaches it by building the image and
handing off *through the file* — write, drop the model, map — so the model and its image are never
both resident. That handoff costs a close and a re-map of a multi-GB file and is the *slower* cold
start on purpose, under the tiebreak in §3. `cache_via_image` is that handoff for every weight
carrier; the streaming forms transcode planes from the gguf mapping straight into the image so they
never materialize at all.

**Suites load with `load_model_`, never the image rail.** `load_model` / `load_model_cached` mint
identity-stamped `.dlim` flavors and GC-purge siblings. A suite child's pinned identity (backend
pin, wscale, tune manifest) differs from the serving rig's, so a suite on the rail both re-mints
multi-GB images the rig cannot use and purges the flavors the rig depends on. Image-rail coverage
belongs to the image suites alone.

### 2.2 Kernel SHAPE is compile-time; only DATA is runtime

The test is one question: *for a given compiled kernel, can this value change between dispatches?*

- **Yes → DATA.** Context depth, row counts, buffer offsets, `kv_dim`, scales, head counts. It
  belongs in a uniform or a kargs struct.
- **No → SHAPE.** A codec's block stride, a scale-plane stride, a lane width, an unroll factor, a
  format selector. It must NOT reach the kernel as a uniform, a kargs field, or a helper parameter.

Shape belongs to the specialization: a separate kernel class and PSO, a per-codec overload, a
monomorphized generic, or a `static_if` on a compile-time witness. Handing a shape constant over as
a value and trusting the shader compiler to fold it back is an assumption, not a guarantee, and it
is worth nothing in the kernels that matter. The same rule bans indirection in a kernel body — no
function pointers, no vtables.

**Verify against the EMITTED shader, never the das source.** Read the `*_msl` global or the SPIR-V
dump and confirm the constant is literal there: `blk * 34u`, not `blk * bstr`. A helper that looks
specialized in das can still lower to a runtime multiply.

**kargs structure.** Twins of a family bind the SAME kargs type at the SAME binding, even where one
twin ignores a field. A twin that carries an extra scalar must not shift the others to different
slots, because that asymmetry propagates into the encoder as a per-form branch. Two tells that a
fold is overdue:

1. *A value reaches the encoder twice* — a pooled scalar uniform BUFFER passed alongside the
   identical value as a parameter (`bd` next to `d`). The buffer is uploaded and released per step
   to carry a number the encoder already holds.
2. *A field is a function of the fields beside it.* An expert plane's block stride is
   `kdim * ndim / blocksize`; a reciprocal scale is `1/sqrt(dim)`. Derive it in the builder — each
   one passed separately is a second place to get it wrong. Likewise, when a kernel's grid IS the
   geometry it reads, take the grid off the kargs rather than re-passing the numbers.

**Nothing dispatches a kernel except its `enc_*` builder.** A hand-rolled bind list elsewhere — a
tune-race harness, a benchmark, a probe — duplicates the builder and desyncs the moment the
family's args change, silently: the slots still exist, the types still compile, and the kernel
reads a struct out of a 4-byte buffer. The dispatch census only catches a builder that binds kargs
on some paths and not others; a duplicate that binds NO kargs is invisible to it.

### 2.3 GPU-resident cache identity

An address-keyed entry carries its SPAN, and a hit must cover the request — a shorter first upload
must never serve a wider later one. Different upload FORMS (plain span vs concat) live in separate
tables so they can never alias; the metal `RegionEntry` rail is the model. Buffers grown out of an
entry retire to a list released only at quiesce boundaries, because unretained command buffers may
still bind them.

### 2.4 Complexity and length lint

STYLE037 (cyclomatic) and STYLE038 (line count) are prompts to look, not orders to refactor. This
module has shapes that are irreducible by design and they take `// nolint:STYLE03x` with a one-line
reason: flat one-call-per-item runs (a registration or release list with one line per kernel), and
GPU kernel bodies whose phases are coupled by barriers, cooperative-matrix ops or register
residency and so cannot cross a function boundary without changing the shader.

Split only where a real seam exists — genuine duplication, a distinct phase, a self-contained arm —
and only when the extracted helper stands on its own. Two corollaries this module keeps tripping
over: **a kargs fold that grows an already-over-cap kernel body is not a reason to abandon the
fold** (unpacking N fields adds N lines; take the growth and ledger the real seam), and **never
suppress a function you have just argued is reducible** — if it is on the follow-up ledger wanting a
dedup, it keeps its warning until the dedup lands.

### 2.5 There is ONE benchmark rig, and the records are the baseline

`benchmarks/lcpp_bench.das` is the only thing that measures performance. It is a *mirror* of
llama.cpp's `llama-bench` — the same test shapes, rep counts and timing boundaries, applied to
our engine — so `pp` is one batched prefill of `-p` tokens from an empty cache per rep and `tg`
is `-n` single-token forwards with no logit read, each row one untimed warmup plus `-r` timed
reps. The real `llama-bench` runs only when `--ref <path>` is passed; that is how the llama.cpp
columns were produced, and they are pinned, not re-measured.

`performance/gen_bench_records.das` sweeps a board by spawning that rig once per cell, and
writes `performance/records/<box>.json`. `gen_site_records.das` merges those into the file the
site renders. A stored row carries its own command, sha, version, tune stamp and exec format, so
a number is self-describing rather than a bare figure in a table.

**Regression checking inverts the same rig:** `gen_bench_records.das --oracle --legs metal`
takes the store's das rows as the work list, re-measures each once, and gates one-sided against
its stored mean (fail past 5%, warn past 3%, gains flagged as suspicious). llama.cpp never runs,
the store is never written, and the child runs `--frozen` so a missing image panics instead of
minting. A second harness would produce numbers that cannot be compared to any of this, which is
why writing one is a review defect.

**The tune stamp gates the comparison.** A manifest older than the binary fails every cell, and
an untuned invocation re-execs into a full retune rather than measuring — so re-mint the box
manifest and check its winners against the stored rows' `tune` stamps before trusting a delta.

**The retune re-exec bites scaffolding, and the pin for it is checked in.** Any bare `daslang`
run that requires the engine — a probe, a one-off script, a REPL experiment — re-execs into a
full retune when no manifest is armed. `performance/last_known_good_sidecar.json` exists for
exactly that: a frozen copy of a complete, noise-gated mint, tracked in git (the `*.tune.json`
ignore rule deliberately does not match it). Point `DAS_TUNE_MANIFEST` at it and the framework
never retunes; on a different box the identity mismatch just serves fallbacks. That is the whole
contract — it suppresses the re-exec, it does not tune the box, and a number measured under it
is not a benchmark. Benches and the rig keep minting their own; refresh the copy when a
paranoid re-mint moves the crowns.

### 2.6 Capability questions and readiness questions are different questions

A predicate that mixes them cannot be reused. `prefill_decline` answers "can metal serve this
model" (capability) *and* "is this window staged" (readiness — are the rope tables built). A caller
that runs before the window is staged must ask the capability half only, or it gets "not yet"
forever and its feature silently never runs. Split such predicates rather than reordering the
caller; an optimistic capability answer is safe when the late path has a fallback, and here it does.

### 2.7 Every program root declares the same stack budget

`options stack` is main-module-only: it does not unify up from required modules, so no library in
the forward chain can declare the depth it needs. Every program that drives the engine — each test,
harness, benchmark, and tool — must therefore declare it, and dasLLAMA's frames are deep enough
(by-value `Session`s, the forward/prefill chain, the generated kernel tier) that the default is
never enough.

The budget is **one number in every root**, currently 524288. Per-root numbers do not survive: a
frame that grows past the smallest declared budget breaks only the program that declared least, so
the limit is discovered by crashing — and the program that crashes is whichever one is run
rarest. A measurement rig sized below a test suite is the worst case of this, because the suite
stays green while the rig dies. The cost of the uniform number is reserved address space per
context; the cost of per-root numbers is a runtime crash found by the least-covered program.

---

## 3. Inherited invariants

Durable "why it is built this way" facts harvested from the design docs archived under
`history/dasLLAMA/`. Violating one of these is a bug, not a style choice.

**From `x64_arch.md`:**

- **The LIBRARY never runs interpreted or AOT — the scaffolding may.** Inference — anything that
  runs a model (eval / decode / prefill / generate) — is `-jit`-only, hard stop: an interpreted or
  AOT model run is not "slow but correct", it is out of contract, and the library panics on it.
  Loop hints and intrinsic lowering exist only in the JIT, and the JIT tier is the only one the
  oracles gate. **Scaffolding is exempt:** conversion utilities (`.dlim` bake, image processing),
  debug scripts, and batch/driver tools that spawn the real runner as a child may run interpreted.
  Interim enforcement is `guard_interp_gguf_load` (`dasllama_image.das` — big-load panic +
  `DASLLAMA_ALLOW_INTERP_LOAD` escape for the conversion path); the unconditional library-seam
  check lands with the tuning rework, after the repack/convert pulls make that seam clean. (The
  x64-era "fallback rail" framing — AOT/interp as correct-but-untuned tiers — is retired;
  intrinsic fallback bodies exist for off-ARCH correctness *inside* the JIT, not as runnable
  tiers.)
- **Correctness before speed, token-for-token.** The engine is validated against external oracles
  (llama2.c + llama.cpp `simple_ids`) plus per-arch parity fixtures. A new kernel passes the suite
  *and* the oracles with the new backend active before any perf claim.
- **Peak memory before cold-start latency — a DELIBERATE trade, and the standing tiebreak.** When a
  load-time choice pits footprint against wall-clock, dasLLAMA takes the smaller footprint. The
  asymmetry is not close: overshooting RAM on a big model is fatal (the OOM killer, or swap that
  makes the whole box unusable), while a slower cold start costs seconds *once per process* and
  costs the warm path — the common one — nothing at all. The prepared-image rail is where this bites
  hardest and where the shape is set: a cold load writes its `.dlim`, drops the model, and maps the
  file back rather than serving the copy it already has in RAM. That is a real close/reopen and a
  real re-fault of a multi-GB file, and on an image larger than the page cache it is a re-read from
  disk. Measured on Llama-3.1-8B-Q8_0 (9.6 GB image, M1 Max, `phys_footprint_peak`): planar cold
  **3.7 s → 4.5 s**, peak **12.3 GB → 5.4 GB**. We bought a 2.3x footprint cut with ~20% of one cold
  start. Apply the same tiebreak everywhere — bake, convert, KV growth, GPU staging — and when a
  change goes the other way, it needs the measured pair (peak AND wall) and an explicit call, not an
  assumption that faster is better.
- **Cold builds STREAM; nothing materializes a model to write one.** The rail that makes the trade
  above cheap: a cold build transcodes each plane from the gguf mapping straight into the image, so
  the model never exists in RAM at all and the peak is a few per-tensor temps over the small
  RAM-resident scale planes. This covers the planar flavor and the metal blob flavor alike — the
  blob's gguf-native 34B blocks assemble per tensor on the way out, and because a blob forbids CPU
  repack, it does not even need the resident `qscales` plane the planar stream keeps. Metal cold on
  the 8B: **19.5 GB → 4.8 GB, 4.1 s → 3.1 s** (faster AND smaller — the eager rail had been
  splitting gguf's interleaved blocks apart only to reassemble them). The vulkan flavor rides it
  too: its bake collects from the GPU walk of a model that is **already serving**, so it needs a
  mapping rather than a load — which is what the warm path always did, and now the cold path does
  the same instead of loading eagerly (**15.4 GB → 6.6 GB**). A new flavor joins the streamed rail
  or explains why it cannot.
- **Token-exact oracle tests pin the bit-exact path** (classic attention, scalar activation);
  approximate/fast paths get separate tolerance tests. Rerouting an oracle test through a
  non-bit-exact default makes it pass on the machine it was frozen on and flip elsewhere.
- **The three-layer safety model that makes a new ISA free:** (1) *registration gate* — an ISA's
  `[init]` never fires off-arch, so `portable` stays active and every run is correct; (2) *scalar
  fallbacks* — an intrinsic called off-target computes the right answer from its fallback body, only
  the JIT emitter lowers it to hardware; (3) *tests drive public wrappers*, asserting only that *a*
  row-major default is active, never an arch-specific name. A new backend is a file plus an `[init]`.
- **The u8×s8 sign trick, and which operand takes the `|a|`.** ARM `sdot` is signed×signed; x64
  `vpdpbusd` is *unsigned×signed*. The exact integer identity is
  `dot(a,b) == dot(|a| as u8, sign(a) applied to b)` — and `|a|` goes on the **unsigned** operand,
  because `|-128| = 128` fits u8 but not s8. Products fit s16, four-product sums fit s32: bit-exact,
  no saturation.

**From `kv_cache_compression.md`:**

- **The KV layout is ragged per-layer, not flat.** `kv_row_total` *sums* per-layer `layer_kv_dim`
  (layers differ under SWA/global splits), and a layer's base offset is a **prefix sum**
  (`kv_row_prefix[l] * seq_len`). A codec keys strides and offsets off the prefix array — never off
  one flat `kv_dim`.
- **Cross-layer KV sharing stores nothing.** When `kv_src[l] != l` both store sites are skipped and
  the layer's offset **aliases the source layer's region**. Any codec must preserve that aliasing.
- **Convert the query, not the cache.** The query is `O(head_dim)` per step; the cache is
  `O(context × head_dim)`. Dequantizing the cache on the decode read costs *read compressed + write
  f32 + read f32* ≈ 2× the f32 baseline — strictly worse than storing uncompressed.
- **Prefill and decode are asymmetric on purpose.** Prefill is compute-bound and tolerates external,
  one-layer-at-a-time decompression (a fresh prompt reads the f32 `k_b`/`v_b` scratch and
  decompresses nothing); the decode read is memory-bound and **must fuse** dequant in-loop.
  Attention consumes one layer at a time — never materialize all layers at once.
- **BLAS is ruled out structurally, not incidentally.** `cblas_sgemm` is f32-only (no hgemm, no
  int8), so any quantized operand must expand 4× into f32 scratch — and the weight matmul is bound on
  bytes streamed, so quantized-native `sdot`/VNNI wins regardless of FLOPs. **Correction:** that
  doc's "BNNS not pursued" tail is superseded by the AMX arc — `dasllama_math_accelerate.das`
  ships an Accelerate float-batch override plus a BNNS-f16 lane for bf16 planes (`DASLLAMA_ACCEL` /
  `DASLLAMA_ACCEL_F16`). BLAS-for-quant stays ruled out; Accelerate/BNNS on genuinely-float planes is
  live and measured.

**From `turboquant_research.md`:**

- **Stage-2 QJL is skipped deliberately** in the tq4 KV codec — its unbiasedness buys variance that
  softmax amplifies, and the damage grows with context. Five independent groups plus vLLM upstream
  converged on Stage-1 only (rotation + Lloyd-Max Beta codebooks + per-block norm). Do not "restore"
  QJL as a completeness fix.
- **Benchmark tq4 against `q4_0` + Hadamard, not f16.** f16 is the comparison the method wins by
  construction; `q4_0`+Hadamard is the one it has repeatedly *lost* on small/dense models and long
  contexts. Quoting an f16 ratio is the mistake this doc exists to prevent.
- **Two corrections that travel with the codec:** the estimator is **asymmetric** — the query is never
  quantized, so a QJL dot is sign-XOR over fp lanes, not popcount (popcount needs the query binarized
  too, an off-paper accuracy trade); and "3.5-bit" is **mixed per-channel bit allocation** over
  head_dim, not fractional coding.

**From `API_REWORK.md`:**

- **The Tier-1/2/3 scope model.** Tier 1 = scalar `Config` knobs (RoPE variant/scaling, norm kind and
  placement, activation, QKV bias, softcaps, sliding window, embed scale). Tier 2 = block swaps that
  change dataflow (MoE, QK-norm, per-layer attention patterns, shared KV, PLE, sinks, parallel
  attn+FFN). Tier 3 = deep forks needing a pluggable KV/attention core. **CORRECTED:** the hybrid
  Gated-DeltaNet lane that doc lists as deferred Tier-3 **SHIPPED** — `dasllama_arch_qwen35.das`
  registers `qwen35`/`qwen35moe`/`qwen3next`, gated by `tests/test_deltanet.das`. The arch registry
  (18 names across 13 `dasllama/dasllama_arch_*.das` files) is the truth for what exists.
- **Arch files are declarative registrations.** An arch file builds an `ArchDesc`
  (name · `configure` · the `ArchBlocks` fn-ptr quad · `ChatTemplate` · `LlmCaps`) and calls
  `register_arch` at `[init]`. Adding an arch touches no forward loop.
- **Family behavior is distributed by `Config` flag, not dispatched by name.** GeGLU-vs-SwiGLU is an
  `ffn_act` flag *inside* the shared dense FFN block; only a genuinely new dataflow (MoE) earns its
  own `ffn_*` block pointer. An `if (arch == "...")` on a shared path is the anti-pattern.
- **`register_arch` MOVES the `ArchDesc`** (`g_arch_registry[name] <- desc`). Clone the template
  **before** registering an alias, or the second call registers a gutted descriptor — the live
  pattern in the qwen2 / qwen3 / qwen3moe / qwen35 files, each with the comment saying so.
- **What rides where:** `ArchBlocks` (copyable fn-ptrs) is copied onto the `Model` at load; the full
  `ArchDesc` (non-copyable — it holds a `ChatTemplate`) stays in the registry. Image loads re-bind
  blocks via `rebind_arch_blocks` and must **not** re-run `configure` — the serialized config already
  carries configure's flags *plus* the GGUF's overrides.
- **The kernel/ISA seam is symmetric with the arch seam, plus one extra rule.** `dasllama_math.das`
  owns the abstraction (typedefs, active pointers, public wrappers); ISA modules self-register at
  `[init]`. Two selection tiers: `register_kernel_backend` auto-activates only the best **no-repack**
  backend, so direct callers (tests, benches, row-major weights) always get a kernel that works on
  unrepacked data; `select_matmul_backend_for_load()` is the **only** path that activates a
  `needs_repack` backend — eagerly activating one runs an interleaved kernel over row-major data,
  i.e. silent garbage.
