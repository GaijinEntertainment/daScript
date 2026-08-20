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
| `REVIEW.md` | `/code-review`, and us while writing | criteria checkable against a diff |

**References flow one way: REVIEW cites ARCHITECTURE, never the reverse.** Sections here are
numbered so they can be cited (`ARCHITECTURE §2.2`). Nothing here may cite a REVIEW rule —
those are unnumbered review criteria by design, and a citation to one is a dangling pointer the
moment the checklist is reordered.

**Charters below are boundaries, not descriptions.** README already says what every file is;
repeating that here is the failure mode this split exists to prevent. An entry earns its place by
answering a question a description cannot: what does NOT go here, and where does the adjacent
thing live instead. Files with no contested edge get one short line.

---

## 1. File charters

Every file under `dasllama/` appears here. Two carry generated CONTENT rather than being
generated themselves: `dasllama_env.das` holds the `[EnvConfig]` knob declarations (`ENVIRONMENT.md` is
generated FROM it by `harness/gen_env_doc.das`), and `dasllama_unicode.das` is hand-written
around transcoded data tables (RANGES/WS from llama.cpp's unicode-data.cpp — retranscode,
never hand-edit the tables).

### 1.1 Engine core

- **`dasllama.das`** — the public API facade and nothing else: `load_model` → `create_session` →
  generate, re-exported names, the doc surface. No engine logic; a function that does work belongs
  in the module that owns the concern, and the facade re-exports it.
- **`dasllama_version.das`** — `DASLLAMA_VERSION`, the module's own release counter, and nothing
  else. Decoupled from the daslang version and from `LLVM_JIT_CODEGEN_VERSION`; ANY kernel work
  bumps it (REVIEW.md), so equal versions mean an equal kernel roster — the sidecar exchange
  keys validity on (version, box). A zero-require leaf so the tuner's noise half, the benches,
  and the facade chain (`dasllama_common` re-exports it) all reach it without weight.
- **`dasllama_common.das`** — the engine: `Model`/`Session`/`Config`, the forward loops, the
  override registries, the runtime knobs. **Not** the load walk (§1.3) and **not** GPU residency
  (§1.5) — both left, and the seam each left behind is a registered hook, so neither comes back.
  It remains the module's debt sink; what sits here that is family-specific or platform-specific is
  debt, **not precedent**. Nothing platform-specific may be added; new shared concerns get their own
  file rather than another thousand lines here.
- **`dasllama_transformer.das`** — the block-composition seam, and the require umbrella: the
  `[init]`-only side-effect requires (arch registrations, GPU tiers, and any module that
  requires the engine back, which the umbrella therefore hosts to break the cycle).
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
- **`dasllama_tools.das`** — the per-ToolMode tool wire codecs (definition serializers,
  replay/result text builders, reply parsers; Harmony namespace/channels, gemma-4 DSL, mistral
  control tokens, llama JSON). Every byte of tool wire text is produced here — pure string+JSON
  functions, model-free testable; the chat layer assembles the output into ChatParts and the
  server parses through the parsers. Verbatim-format provenance: `tool_formats_plan.md`.
- **`dasllama_chat.das`** — conversation turns and chat-template application. Per-arch template
  *content* is registered by the arch file (§1.6), not written here.
- **`dasllama_par.das`** — `maybe_parallel_for` and nothing else. Threading policy (job counts,
  thresholds) belongs to the caller that knows the shape.

### 1.2 Formats and data movement

These six own the module's data-shape vocabulary. The boundaries between them are the ones most
often gotten wrong, so each says explicitly where the neighbouring half goes.

- **`dasllama_plane.das`** — the borrowed-plane vocabulary: a plane is a pointer into a prepared
  image plus its element count; the image owns the bytes, a carrier owns nothing but its backing.
  Requires nothing in dasllama — the image rail binds planes, every carrier holds them.
- **`dasllama_kqformat.das`** — format IDENTITY: the `KqFmt` enum, the per-format descriptor table
  (plane strides, block geometry, stream codes), format predicates. It requires nothing else in
  dasllama, because it is the taxonomy everything keys off. ONE id space — the enum; integer ids
  exist only at the IR/kernel-param boundary. `kq_sb` is the superblock-lattice predicate: a
  `fmt != q8` test does not imply the lattice, so branch on the predicate.
- **`dasllama_convert.das`** — every tensor format CONVERSION: quantize/dequantize/transcode/encode,
  codec byte readers, numeric widen/narrow — regardless of platform or caller; plus the Q8_0/Q4_0
  format identity itself (block geometry, scale layout, the quantization-quality detector).
  Metadata-coupled drivers (GGUF lookup, threading, guards) stay with their containers and
  dispatch in. ONE carve-out: a conversion that IS a KV-cache format's store/read half lives with
  its codec family (§ below).
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
  transform, the CPU repack walkers, the GPU tier gathers, the per-region q8→Metal-34B
  un-repack (`q8_region_to_metal_blob`, the ASR-decoder driver's upload form), and the refusal
  half (`can this model take the blob form`) split out so the image writer can commit without
  loading.
- **`dasllama_tokenizer.das`** — the tokenizer facade: backend selection off the GGUF metadata and
  the one encode/decode/piece surface models and the chat layer call. Re-exports both backends, so
  a consumer requires this file and never picks a backend by hand.
- **`dasllama_spm.das`** — the SentencePiece backend (Llama-2 family, Phi-3, Gemma): score-greedy
  merges over vocab pieces, `<0xXX>` byte fallback.
- **`dasllama_bpe.das`** — the byte-level BPE backend (Llama-3 / tiktoken family): vocab load, the
  GPT-2 byte alphabet, ranked merges, encode/decode. Split from SPM because the two algorithms
  share no state; a third merge algorithm gets a fourth file. Two sanctioned family-name tests
  live here rather than in `dasllama_pretok`: the `pre`-name selector inside `bpe_encode`, and the
  gemma-4 newline-run split in `bpe_encode_spm_space`; `load_bpe_tokenizer_gguf`'s per-family
  metadata defaults are the third and last.
- **`dasllama_pretok.das`** — the pre-tokenizer: one hand-compiled split function per family
  (llama3/qwen2/qwen35, gpt-2, gpt-4o, tekken), selected by the BPE `pre` name. Regex-port growth
  lands here, never in the merge engine — the two change for different reasons (new model family
  vs. algorithm work). Every arm with an on-disk llama.cpp corpus vocab is gated by its case in
  `test_tokenizer.das` (llama3, qwen2, qwen35, gpt-2); tekken has no corpus case, and gpt-4o is
  pinned by frozen ids in `test_parity.das` only.

### 1.3 The load and image rail

**`dasllama_load.das`** is the GGUF load walk: metadata to `Config`, the plane layout, disk-format
detection, the eager and streamed conversion ladders, and the load entry points. It owns nothing the
forward path touches at run time — a loaded `Model` is the whole handoff — and it requires
`dasllama_common` back for `Model`/`Session`, so the transformer umbrella requires it `public` and
breaks the cycle. That re-export is what keeps every consumer on the facade.

- **`dasllama_image.das`** — the prepared-model `.dlim` rail, and it is ONE rail (§2.1). Nothing
  outside this file may read weights into a live carrier; image backings are released only
  through the single release path in `dasllama_common` that the carrier finalizers call — never
  an ad-hoc unmap.

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
| the kernel home<br>`dasllama_metal_kernels`, `dasllama_vulkan_classes` | kernel source, the derived-access/PSO census | device state, engine types |
| `dasllama_<gpu>_common`<br>`dasllama_metal_common`, `dasllama_vulkan_common` | device state, buffer/command plumbing, hazard + capture rail, profiler | driver policy |
| `dasllama_<gpu>_decode`<br>`dasllama_metal_decode`, `dasllama_vulkan_decode` | the resident token-step driver + decode-time arms | kernel bodies |
| `dasllama_<gpu>_prefill`<br>`dasllama_metal_prefill`, `dasllama_vulkan_prefill` | the batched prefill driver + batch arms | kernel bodies |
| `dasllama_<gpu>_shapes`<br>`dasllama_metal_shapes` | PORTABLE servability gates — no GPU C++ require, so any box can bake | device calls |
| the tower driver<br>`dasllama_metal_tower` | one-shot embedder/encoder encodes (gemma4uv chain, the gemma4v ViT and gemma3v SigLIP block loops, the whisper-class block loop, the conv frontends + the qwen3a padded-weight slab) — no session, no KV, no mirror; registers the gemma4uv, gemma4v, gemma3v, encoder_blocks, tower-conv and qwen3a-conv hooks | kernel bodies, decoder state |
| the ASR-decoder driver<br>`dasllama_metal_asr_dec` | the whisper decoder on Metal: the 34B weight blob, the f16 resident cross/self K/V, window-granular cross-KV + decode-step serves; registers the whisper cross-KV and decode hooks (family registries in `dasllama_whisper`) | kernel bodies, LLM session state |
| the kernel-access lens<br>`dasllama_metal_lens` (Metal), `dasllama_vulkan_dispatch` (Vulkan — the `[vk_dispatch]` macro derives access per class) | the kernel-access macro | anything else |

- **Vulkan additionally has an ENTRY, `dasllama_math_vulkan.das`** — capability probe/arm, `.dlim`
  identity source, cross-arm routers, the `[init]` installs. It re-exports the family `public`,
  and its NAME is the transformer umbrella's `?vulkan` require contract (deliberately LAST in the
  umbrella: the vulkan drivers are the hot-edit modules, and require order is the jit obj-cache
  layout): **never rename it.**
- **Vulkan additionally has `dasllama_vulkan_seams.das`** — the thin whole-op call seams the tier
  and suites dispatch through (`vk_add_rms`, `vk_rope_kv_store`, `vk_decode_attn`). It exists
  because of a require direction: common cannot require the classes module (classes requires
  common back), so any seam that encodes a class kernel must sit above both. A seam here wraps
  ensure/set/enc — kernel bodies and driver policy stay out.
- **Metal has NO `math_` entry** — the family enters via the transformer's `?das_metal` requires
  plus unconditional shapes. Its below-common piece is **`dasllama_metal_gemm.das`** (the batch
  GEMM donor that common requires `?das_metal`), which owns its device by necessity:
  metal_common → dasllama_common → metal_gemm would cycle.
- **Backend-only capabilities live in their matching ROLE file, not in new grab-bags** — vulkan's
  weight arena, streamed mirrors, heat cache, host-import, coopmat; metal's blob transform and MTP.
- **The tower driver owns NO PSOs.** Its kernels (LN, f32 mul_mm, the two gelu flavors,
  posadd, the gemma4v clamp / rope2d / GEGLU-quick, the gemma3v head restride) live in the kernel home, so `metal_decode_init` compiles and `metal_kernels_release`
  releases them like every other registry PSO; the borrowed prefill builders (`pf_enc_bf16_mm`,
  `enc_add_bias_rows`, the attention trio via `enc_qk_mm`/`enc_av_mm`) come up through
  `metal_prefill_pso_init`, prefill's public bring-up seat, and `plane_buffer` in common is
  public for the same wrap-a-plane reason. The tower's own objects (the ones buffer, its
  scratch pool) release through `metal_tower_shutdown`.
- **The tower driver is a Metal-only role** — Vulkan has no tower twin; audio/vision encodes
  on the Vulkan tier stay CPU (the gemma4v ViT and gemma3v SigLIP block loops included: on
  Vulkan and on plain CPU boxes those towers serve their q8 lanes). Likewise the non-causal media span: Metal serves it through
  `AttnArgs.uend`; the Vulkan resident prefill declines it (`followup_general.md` #23's
  remaining half).
- **Per-layer FFN widths (MatFormer E-series, at most two — `ffn_second_hidden`) serve on Metal
  only**: the decode and prefill drivers bind the width per layer (dense trunks, no MTP; batch
  keeps the layer-0 hoist behind its uniformity decline). The Vulkan tier has no PLE arm, so
  E-series never reaches its width question.
- **Family-shared kernel classes live in `dasllama_metal_kernels`.** The `[metal_dispatch]` lens
  generates `enc_*` builders and MSL globals into the module the class COMPILES in, so co-location
  follows the class — "the builder needs the driver module" is never a placement reason. Prefill's
  prefill-only classes are convergence debt, not precedent.
- **Retired: the single-pass whisper-decoder attention (`MetalWdecAttn`/`enc_wdec_attn`)** —
  deleted in the metal-asr review round (2026-08-17, the `bbatkin/metal-asr` PR; the kernel is
  in git history). The chunked part/comb pair replaced it during bring-up (1470→709 ms on the
  turbo decode) and its `float[1504]` tgmem bound was the only reason for the driver's old
  ta/n_text_ctx≤1504 shape cap, which left with it.
- **Ledgered kernel-binding asymmetries** — a REVIEW rule firing on one of these is expected, and
  this entry is the sanction: the moe mul_mm TENSOR twins (`MetalMoeMulMmQ8T` /
  `MetalMoeMulMmMx4T`) keep the pre-family compact kargs slots while their base classes bind the
  family numbers, so no shared bind path may span the two layouts; and the in-engine moe mul_mm
  A/B race harnesses (`dasllama_metal_prefill.das`) encode through `kn_moe_mm_family_tail`
  rather than a per-class `enc_*` builder.
- **`dasllama_gpu_tier.das`** — the device-cooperation SPI: hook types, install/unset slots,
  route/mark/want/status state, engine-facing forwarders. Vulkan implements it (per-op offload plus
  resident plumbing); Metal deliberately does not, because UMA makes residency moot there and Metal
  integrates as a whole-forward driver through common's override registries (the ASR-decoder
  driver is the one exception: whisper is not a `Model`, so its hooks are family registries in
  `dasllama_whisper`, same decline contract).
- **`dasllama_gpu_resident.das`** — the WHOLE-MODEL residency rail: bake the device layout offline
  into the flavor image, upload a model's stacks to the tier, and drive decode/prefill entirely on
  device. It is device-AGNOSTIC — it holds no device call and requires no GPU module, reaching the
  hardware only through the `dasllama_gpu_tier` SPI and entering the engine only through common's
  override registries. `"vulkan"` is the tier string it registers under, not a dependency, which is
  why it compiles on every box. It requires common back for `Model`/`Session`, so like the Metal
  drivers it is required from the transformer umbrella, never from common.
- **`dasllama_kernel_access.das`** — the shared body-walk read/write classifier both GPU lenses run
  on, plus the dispatch-lens micro-grammar (the grid/tg/params spec tokenizers and the shared
  AST-emission core: `is_digit_tok`, `role_ok`, `derived_role`, `mk_uint_cast`, `mk_call1`,
  `mk_grid_dim`, `param_type`). One owner by design — the two lenses' grammars drifted apart
  when each carried a private copy (metal folded only the literal "1" where vulkan folded any
  integer). Backend-specific lowering stays in that backend's lens.

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

- **Metal sits ABOVE `dasllama_common`** (typed `Model`/`Session` access, shapes unconditional);
  **Vulkan sits BELOW it** (untyped pointer/array seams — the family never requires common).
  Both tiers ENTER from the transformer umbrella (`?das_metal` requires; the single `?vulkan`
  require of the `dasllama_math_vulkan` facade); the inversion that remains is the
  kernels↔common require DIRECTION, and it is why their seam shapes differ.
- **UMA vs discrete VRAM**: Metal never grows residency machinery (memory is memory); arenas,
  upload economics, mirrors and hydration are Vulkan's alone.
- **Lens depth**: both lenses generate `enc_*` builders from kernel classes — Metal via
  `[metal_dispatch]`, Vulkan via `[vk_dispatch]` (per-class set layouts + push constants; the
  class-kernel arc retired the hand-built 6-slot set ladders outright) — and both speak the
  multi-kernel form (`kernel=` names the method, one macro instance per kernel, declared roles
  must cover every kernel).
- **`family=` is Vulkan-only.** A vulkan family shares the per-class surface — the `VkdClass`
  global, the `set_*` builder, the pipe slots — across classes with one binding layout. Metal's
  `enc_*` builder is the entire generated surface, so there is nothing for a family to share;
  cross-class PSO/source sharing on Metal is a PSO-lifecycle question, not a lens one.
- **`@default` is Metal-only.** A `[metal_dispatch]` field may name a fallback global
  (`@default = g_one`) that the generated builder binds when the caller passes null;
  `[vk_dispatch]` has no counterpart — vulkan callers pass a real buffer at every slot. If
  vulkan grows an optional-bind shape, it lands as this same annotation, not a new spelling.
- **The workgroup-footprint gate is Vulkan-only.** `[vk_dispatch]` sums a class's `@workgroup`
  members and its generated `ensure_*` declines by name (`vkd_wg_fits`) before the pipeline
  build, because MoltenVK's over-cap failure is an opaque `INITIALIZATION_FAILED` — and the
  resident driver declines residency with it (`vk_rdec_prepare`). `[metal_dispatch]` has no
  footprint gate: Metal's own pipeline compile fails loudly with the footprint in the error.
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

### 1.7 Encoder towers, audio, and ASR

- **`dasllama_asr_types.das`** — the ASR floor: the capability/segment/timestamp types every
  family file and the facade share (`AsrCaps`/`AsrTimestamps`/`TranscribeSegment`, plus
  `asr_ctx_guard`). Family modules require this instead of each other — a shape needed by two
  families moves up here, never sideways.
- **`dasllama_tower.das`** — the family-neutral encoder-tower home, composed by every audio AND
  vision tower: the oracle-exact activations (erf / tanh-LUT / quick-LUT gelu, swiglu, geglu),
  the LayerNorm/RMS row forms, bias and residual row adds, the
  `mm_blob_b`/`mm_bf16_b`/`mm_plane_b` GEMM wrappers, `Clamp`/`read_clamp`,
  `im2col_rgb_patches`, `rope_neox_2d_rows`, `avg_pool2d_rows`, blocked `attention_bidir`, and
  the encode-stage prof rail. The one home: a family file that re-implements one of these is a
  defect, and nothing here names a family type.
- **`dasllama_audio.das`** — the audio encoder tower: the mel front-ends (mtmd and whisper.cpp
  flavors), `AudioTower` with its staging, q8-quantize, and image rails, `EncoderState`, and the
  whisper-class encode + block loop with its GPU hooks. Composes `dasllama_tower.das`.
- **`dasllama_audio_io.das`** — decode-any-format → 16 kHz mono f32 PCM. The only file that talks to
  miniaudio.
- **`dasllama_asr.das`** — the ASR facade: capability declaration, timestamp granularity, the
  backend-neutral entry points.
- **`dasllama_whisper.das`** / **`dasllama_parakeet.das`** / **`dasllama_canary.das`** /
  **`dasllama_qwen3a.das`** / **`dasllama_gemma4a.das`** — one file per model family, each owning its
  weights, its decode loop, and its quirks. Shared tower pieces go up into `dasllama_tower`, not
  sideways between families.
- **`dasllama_vad.das`** — Silero-VAD weights and per-stream state.

### 1.7b Vision

- **`dasllama_vision.das`** — the image preprocessing rail: dynamic-resolution geometry, the
  letterbox resize (aspect-preserving bilinear onto a centered black canvas), u8→f32 normalize,
  and the `DASLLAMA_VISION_DUMP` PPM writer. The only preprocessing home.
- **`dasllama_vision_io.das`** — image decode to RGB8, from a file or a byte blob. The only file
  that talks to stbimage; re-exported by the facade (like `dasllama_audio_io`) so consumers
  decode through one seam — the engine itself takes decoded pixels.
- **`dasllama_gemma4uv.das`** — the gemma4uv embedder (gemma-4 dense): mmproj load and the
  im2col → LayerNorm → GEMM → position-table → projection forward. One file per vision
  projector family, following the audio tower pattern; shared pieces move up into
  `dasllama_tower.das` (the encoder-tower home), never sideways.
  A shipped mmproj mixes element types per tensor — gemma-4's "BF16" file stores the patch
  embedder as F32 and only the projection as BF16 — which is why a weight plane's element type
  follows its source tensor, per tensor, never a per-file verdict.
- **`dasllama_gemma4v.das`** — the gemma4v ViT tower (gemma-4 E-series): mmproj load (bf16 GEMM
  planes as the file has them, the clamp sidecars as a blob table) and the 16-block pre-norm RMS
  forward — clamped GEMMs, per-head q/k RMS, two-axis NEOX rope, weightless V RMS, unscaled
  bidirectional attention, GEGLU-quick — then the 3×3 pool, RMS and projection. Composes the
  `dasllama_tower.das` tower pieces; owns only its layout and the block loop.
- **`dasllama_gemma3v.das`** — the gemma3 SigLIP tower (size-invariant across the gemma-3 line;
  4B and 12B are the tested pairs): mmproj load (the file's f16 planes widen exactly to f32;
  the ffn pair serves at the layout's padded 4352 width, so every q8-lane GEMM quantizes) and the 27-block pre-norm
  LayerNorm forward — biased GEMMs, learned position add, scaled bidirectional attention,
  GELU-tanh — then post-LN, the 4×4 pool, weighted RMS and projection. The canvas is FIXED at
  896² (the learned table covers exactly that grid), and the family's image_mean/std (0.5) is
  PREPROCESSING, not graph — `encode_image_` scales, `gemma3v_encode` takes planes raw like the
  mtmd fixtures do. Composes `dasllama_tower.das`; owns only its layout and the block loop.
  Two sanctioned exceptions to the family-quirk placement rule live outside this file: the
  image span markers sit on the SHARED `gemma_chat` template in `dasllama_common.das` (template
  detection cannot tell gemma3 from gemma2 — both spell `<start_of_turn>` — and the chat
  layer's `image_vocab_ok` gate refuses a text-only vocab); and the carrier deliberately serves
  ONE f32 plane (F32/F16/BF16 widen exactly; anything else refuses by name at stage) instead of
  gemma4v's per-tensor plane split — the Metal leg runs the f32 mulmm, so there is no bf16
  plane to preserve.
- **`dasllama_vision_embedder.das`** — the vision carrier: `VisionEmbedder` / `VisionState`, the
  `AsrModel` shape for vision — one union through every seam, the family sniffed from the mmproj
  (`clip.vision.projector_type`, or a `.dlim`'s baked tag) at load, one-line arms. Outside a
  family's own file, a family type is named only here, in `dasllama_metal_tower.das`'s family
  hooks, and in files under `tests/`.

Vision oracle provenance (the convention `REVIEW.md`'s fixture rule points at): real image
fixtures and mmproj files live in the models dir with `.sha` pins, fetched never generated
(their `performance/fetch_models.das` entries are the checkable pins); the mtmd reference dumps
live beside them in `gemma4-vision-oracle/` and `gemma3-vision-oracle/`, whose `mint.sh`
(gemma4uv), `mint_e2b.sh` / `mint_e4b.sh` (gemma4v) and `mint_gemma3.sh` (gemma3v) record the
exact `llama-mtmd-debug` / `llama-mtmd-cli` invocation that minted each
dump, so regeneration is a command, not archaeology. An encode oracle dump is minted on the
CPU, `-fa off`, from the f32-widened mmproj twin — the only true-f32 reference arm (the
reference's Metal "f32" GEMM stages half operands, its flash-attention path casts K/V to f16,
and the shipped bf16 mmproj rounds activations to bf16; llama.cpp's own four arms spread
≤ 6.5e-3 on the gemma4v tokens).

### 1.8 Instrumentation and support

- **`dasllama_parity.das`** — CPU-reference caches for the parity instruments. Test-facing, but
  library-side because the caches outlive a single suite.
- **`dasllama_prefix.das`** — the prefix/page cache for evaluated token history.
- **`dasllama_lint.das`** — the facade boundary as a compile-time lint (DASLLAMA001): every
  engine module carries it, so a consumer requiring anything under `modules/dasLLAMA/` but the
  entry modules (facade, scheduler, exchange pair) fails to compile. Escape:
  `options _dasllama_internal = true` — engine files, this module's own tests/harnesses/
  benchmarks/rigs, and the ruled consumers: `utils/dasllama-convert` (the bake tool reads
  the mint rail), `utils/dasllama-server/test_openai_server*.das` (env-registry test knobs),
  `modules/dasLLVM/daslib/llvm_user_modules.das` + `modules/dasLLVM/tests/test_{grid,tune,tuned}.das`
  (the tune-generator contract).
- **`performance/model_specs.das`** — the ONE model-set table: per carrier, the profiled-
  catalog fields, the official-board flag, the provenance pin (exact HF repo + revision +
  sha256, or the on-box conversion recipe; companions hang off their owning entry), and the
  frozen parity evidence as data (pinned ids + arms). `llm_catalog` / `official_catalog` /
  `models_provenance` are views over it.
- **`performance/fetch_models.das`** — the fetch/verify driver over the provenance view.
  Verify by default, `--fetch` downloads; it never converts on `--fetch`, never benches, and
  it touches no tune state (`tune_policy(missing="fallback")`). BRINGUP.md §2 is the runbook.
- **`dasllama/dasllama_exchange_schema.das`** — engine-free validation for exchange submissions
  (record stores + tune sidecars); the dasllama.io ladder service builds on it.
- **`dasllama/dasllama_exchange.das`** — the sidecar-exchange client (a sanctioned public entry point): the boot-time
  lookup/apply (llvm_tune's scope resolver — a verified per-box match downloads instead of a
  ~20-minute tune), the privacy-stripped submit rails, and the control-page surface
  dasllama-server serves at `/exchange`.
- **`benchmarks/asr/mem_census.sh`** — the peak-memory census (`/usr/bin/time -l` around one
  asr_bench process per cell; macOS only) — the interim footprint instrument until a footprint
  leg lands in `gen_bench_records`; its numbers live in `PERF_LEDGER.md`, never the stores.

---

### 1.9 Serving

- **`dasllama_scheduler.das`** — the continuous-batching scheduler, the serving layer over the
  facade (its one engine require is `dasllama/dasllama`). One synchronous thread: each
  `scheduler_step` admits queued requests, runs one `eval_batch` decode step over every
  decoding stream, then at most one bounded prefill chunk FCFS; paged serving donates finished
  streams' KV pages to the prefix cache. Results flow out as `SchedEvent`s — no HTTP here.
  `utils/dasllama-server` owns the writers; `tutorials/dasLLAMA/13_serving.das` is the
  teaching consumer; `tests/test_scheduler.das` gates it against `generate()` references.

## 2. Mechanisms

The "why" behind criteria that REVIEW states in one line each.

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

**An image is judged only where its identity can be recomputed — the LANE rule.** An identity's
(quant, tag) pair is its lane, and lanes coexist by design: a bake proves dead only its OWN lane's
siblings, plus BROKEN and version-stale images anywhere. Everything else is `FOREIGN` — kept, and
counted in the log. Two contexts produce lanes this process cannot reproduce: a GPU flavor (a blob
bake selects its own backend and winners, so the string differs even where the tag matches:
`portable|q8 mr4` against a planar `arm64-gen|q8 mr8`), and a family tag whose module is not loaded
(the tag registry is filled by each family's `[init]`, so a process that never required whisper has
no way to know a whisper image is current). A process that cannot recompute an identity has no
standing to call it dead — the orchestrator of a sweep least of all, since its cells measure through
an exe carrying its own baked winners. The one owner carve-out is the batch lifecycle: the board
rigs own the model dirs for a whole run, so `dlim_wipe` (verdict-blind, `dasllama_image.das`)
clears them behind the exe gate at batch start and after each model's last cell, with every image
re-baked from its gguf on demand. Judging stays forbidden; owning the directory for the batch is
what licenses deletion without judgment.

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
the store is never written, and the timed child runs `--frozen` — a prepare pass bakes and warms
each cell's image first (the batch starts wiped), so the cell itself never converts. A second
harness would produce numbers that cannot be compared to any of this, which is
why writing one is a review defect.

**The tune stamp gates the comparison.** A manifest older than the binary fails every cell, and
an untuned invocation re-execs into a full retune rather than measuring — so re-mint the box
manifest and check its winners against the stored rows' `tune` stamps before trusting a delta.

**A measured number proves its kernel provenance through `tune_gate()`
(`performance/profile_common.das`), one arm per world it can run in.** Three worlds, because
`tune_status()` populates in exactly one of them: a standalone exe checks the sidecar the
release shipped beside it; a `DAS_TUNE_MANIFEST` run checks that file; a plain script checks
that every `[tune]` row stamps a manifest winner. An invocation no arm covers refuses — or
worse, measures on fallback kernels — which is why every measuring entry point calls the gate
before its first timed rep. Two rig shapes fall outside "measuring entry point" by the
property itself, ledgered here: a kernel A/B lab dispatches its variants through its own arms
(never the `[tune]` selection), and `lcpp_bench.das`'s `--tok` cell dispatches no kernels at
all — neither can measure a fallback silently.

**The retune re-exec bites scaffolding, and the pin for it is checked in.** Any bare `daslang`
run that requires the engine — a probe, a one-off script, a REPL experiment — re-execs into a
full retune when no manifest is armed. `performance/last_known_good_sidecar.json` exists for
exactly that: a frozen copy of a complete, noise-gated mint, tracked in git (the `*.tune.json`
ignore rule deliberately does not match it). Point `DAS_TUNE_MANIFEST` at it and the framework
never retunes; on a different box the identity mismatch just serves fallbacks, and a copy minted
before the current `DASLLAMA_VERSION` serves fallbacks on any box — the compile says which with
one `WARNING DAS_TUNE_MANIFEST` line per scope. That is the whole
contract — it suppresses the re-exec, it does not tune the box, and a number measured under it
is not a benchmark. Benches and the rig keep minting their own; refresh the copy when a
re-mint moves the crowns or `DASLLAMA_VERSION` bumps.

### 2.6 Capability questions and readiness questions are different questions

A predicate that mixes them cannot be reused. `prefill_decline` answers "can metal serve this
model" (capability) *and* "is this window staged" (readiness — are the rope tables built). A caller
that runs before the window is staged must ask the capability half only, or it gets "not yet"
forever and its feature silently never runs. Split such predicates rather than reordering the
caller; an optimistic capability answer is safe when the late path has a fallback, and here it does.

### 2.7 A quantized activation carries its scale lattice (Vulkan)

Two activation quant forms ride the vulkan rail, and they differ in the SCALE LATTICE, not the
int8 payload: the Q8_0 form scales per 32 values, the superblock form per 256 (with per-32
sub-scales inside). A compiled kernel indexes ONE lattice — the q8 GEMV/GEMM rail reads per-32
scales; the k-quant (k4/k5/k6/q40) kernels index the per-256 lattice. `kq_sb(fmt)` is the
predicate (§1.2), and it answers for the WEIGHT plane the dispatch consumes.

Three consequences the code is shaped around:

- **A quant/act encoder is picked by the CONSUMING plane's format, never by a rail-wide
  default.** Every site that encodes activations for a GEMV/GEMM keys its encoder (and its
  grid: superblock counts are `n/256`) on the consumer's `kq_sb`. The failure mode is silent
  per-dispatch: the wrong lattice indexes garbage scales, outputs stay finite, and nothing
  panics — only end-to-end token parity (`harness/parity.das`) catches it, so a resident
  change is witnessed only by parity runs over both a q8 and a k-quant model.
- **The fused add-rms+requant twin exists only for the per-32 form.** The rail gates it on
  "every consumer of this buffer is Q8_0-scaled" (`rd_x_quants_b32`), and the profiler stamp
  shape must ride the SAME gate, or profiles desync from what actually dispatched.
- **A GEMV group sharing one activation buffer must be lattice-homogeneous.** q/k/v share one
  quantized x; gate/up share another. Resident arming classifies each member's consumer form
  and DECLINES a mixed group rather than serving one member wrong scales.

### 2.8 Every program root declares its stack budget and its prefill intent

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

The second declaration is prefill intent. A model-loading root declares `allow_cpu_prefill()`
on the arms that hit the CPU-prefill guard — `set_metal_mode` with a runtime value declares
nothing, since `MetalMode.off` leaves the guard armed — and logs, once, which configuration it
ended on. The guard panics, and a panic takes every live stream down, so an undeclared root is
a serving outage waiting on its first long prompt. Both halves of root discipline are enforced
by `tests/test_program_roots.das`; weakening that test is a review defect
(`tests/REVIEW.md`).

### 2.9 Environment knobs

A knob is an `[EnvConfig]` field in `dasllama_env.das`, read as `g_env_*.<field>`; the field is
also what generates its `ENVIRONMENT.md` row, so a knob declared anywhere else is invisible to
the documentation and to the registry test. The sanctioned forms beyond a plain typed field:

- **Tri-state knobs** — presence matters, or the effective default is computed at runtime — are
  `Option<T>` fields.
- **Dynamic names** — a variable named by data, not by code — go through `env_is_set` /
  `env_value_of`; there is no field to declare because the name is not known at compile time.
- **The config loads once at context init**, so `set_env_variable` mid-process is invisible to
  the running config: arm a child process's environment instead.
- **A write of a foreign library's knob** (`set_env_variable` with a literal name) is allowed
  only before that library first reads it, and the name must be a declared `[EnvConfig]` knob —
  the registry test scans writes too, so a re-spelled name fails it.

`tests/test_env_registry.das` enforces the lot in both directions (declared ↔ documented,
read ↔ registered, writes included).

**Override announces.** A knob that is a gate escape, a policy override, or a threshold
recalibration — one whose presence makes a run measure, mint, or emit something its defaults
would not (`DASLLAMA_ALLOW_UNTUNED`, the `DAS_TUNE_NOISE_*` pair and `DAS_TUNE_POLICY` read
through llvm_tune's accessors) — announces at the point it changes the outcome: at least one
printed or logged line naming the knob by its env spelling. Set-but-inert stays silent;
per-site repeats are correct (the same knob may legitimately announce at its `[init]` wire, at
a gate it bypasses, and in a tool banner). Ordinary behavior knobs (thread counts, rail
selection, formats) are not overrides under this contract — their state belongs in the tools'
existing config/status lines, not in per-knob announces.

### 2.10 Sanctioned instrumentation rails

Engine timing goes through the rails that aggregate and tag it: the `jobque_profile` markers
(`profile_tag` / `profile_marker` and the `trace_*` wrappers in `dasllama_math.das`), the
`prof_add` / `forward_profile_*` decode buckets (`dasllama_common.das`), and the `asr_prof_add`
encode buckets (`dasllama_audio.das`). A new clock read paired with a print of the elapsed
interval bypasses the aggregation — it measures one call site once, is not filterable, and rots
where a rail entry would keep serving. Where a timed line IS the deliverable — `benchmarks/`,
`performance/`, `harness/`, and cold one-shot load/mint progress logs (image bake/map, load
stages, tokenizer build) — the rails do not apply. A clock whose value feeds logic is control
flow, not instrumentation; it is marked `// clock: control` so the sweep and any future lint
leave it alone.

### 2.11 The [hot_path] coverage model

`[hot_path]` sits at the REGION ENTRY — the `*_encode` / `*_decode` / step drivers — and its
`[no_alloc]` / `[no_env]` / `[no_io]` contracts arm transitively down the call graph, so
interior kernels stay bare. A new function needs the annotation itself only when no annotated
entry reaches it: a new entry point carries it, and a new backend entry (kernel-backend
override, batch donor) carries it too, because backends are also reached from un-annotated
harness paths. Reused buffers take `@scratch`; debug and profiling legs take `[cold_path]`.

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
  Enforcement is `guard_interp_gguf_load` (`dasllama_image.das` — big-load panic +
  `DASLLAMA_ALLOW_INTERP_LOAD` escape for the conversion path) plus `guard_interp_inference`
  (`dasllama_math.das`), which panics at the library seams: `make_run_state`, ASR session
  creation, and `vad_chunk_prob`. (The
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
