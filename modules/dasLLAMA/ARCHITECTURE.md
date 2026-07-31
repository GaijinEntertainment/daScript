# dasLLAMA Architecture — what goes where

> Companion to `INVENTORY.md` (the as-is census). This document records where functionality
> **BELONGS** and why — placement rules only. **Historical data stays OUT of this file**: no
> move ledgers, dates, PR/commit references, or incident anecdotes — the per-move record lives
> in `history/dasLLAMA/reorg_extraction_ledger.md` and git. When an extraction settles a
> placement rule, the rule lands here, stated timelessly.

## Placement rules

- **Tests live under `modules/dasLLAMA/tests/` — all of them.** `/tests/dasLLAMA` must not exist.
  dasLLAMA inference runs **`-jit` only** — never interpreted, never AOT (no dasLLAMA test joins
  `test_aot`/`test_aot_subset`); every suite runs `-jit` through `tests/run.das`, per
  `tests/CLAUDE.md`. The library panics on a non-`-jit` model run; scaffolding tools (converters,
  batch drivers, debug scripts) may run interpreted (see Inherited invariants).
- **Shared functionality gets pulled into small, correctly-named, single-purpose modules** —
  `dasllama_repack.das` (kernel data repacking), `dasllama_convert.das` (tensor format conversion).
  Never a grab-bag `common`/`families_common`: a module whose name doesn't say what it does is the
  failure mode being unwound.
- **Repacks live under `dasllama_repack.das`** — every disk-order → compute-order kernel-layout
  transform (grp<mr> interleaves, disk-order extractors, panel unpacks), regardless of format or
  platform. Number sources (tune stamps, bake overrides) stay with their owners and pass plain
  parameters in.
- **Conversions live under `dasllama_convert.das`** — every tensor format conversion
  (quantize/dequantize/transcode/encode, codec byte readers, numeric widen/narrow), regardless of
  platform or caller. Metadata-coupled drivers (GGUF lookup, threading, guards) stay with their
  containers and dispatch into the codec. The KV-cache runtime codec is the one carve-out (next
  rule): a conversion that IS a cache format's store/read half lives with its dot/axpy family.
- **The KV-cache runtime codec lives under `dasllama_kv_codec.das`** — one family per cache
  format, kept WHOLE: store (quantize), read (dequant), the attention score dot, and the
  V-accumulate axpy share the format's block byte geometry (34B/17-uint16 q8_0, 18B/9-uint16
  tq4), so a layout change can never span modules. These run per token (store) and per token ×
  cached position (dot/axpy) — `[tuned]` hot kernels, not load-time converters. The tq4 FWHT
  rotation + sign vector live with their family. Codec DISPATCH (`KVDtype`) stays at common's
  `kv_store_row`/`kv_load_row`/`kv_dot`/`kv_axpy` seam; the f16 family's row converts are the
  generic pair in `dasllama_convert` (dual-use beyond the cache: gguf load, wscale plane,
  Accelerate batch prep).
- **The prepared-image rail lives under `dasllama_image.das`, and it is ONE rail** — `build_image`
  walks a carrier's planes into a sink (a `.dlim` file, or a page-aligned memory chunk when there
  is nowhere to write); `parse_image` turns `(base, bytes)` back into borrowed-plane fields and
  does not know which sink produced them. Cold and warm therefore yield the SAME struct, and a
  cold load reaches it by building the image and handing off through the file — write, drop the
  model, map — so the model and its image are never both resident. That handoff costs a close and
  a re-map of a multi-GB file and is the *slower* cold start on purpose, under "peak memory before
  cold-start latency" below. `cache_via_image` is that handoff for every weight carrier; the
  streaming forms transcode planes from the gguf mapping straight into the image so they never
  materialize at all. Nothing outside this file may read weights into a live carrier, and nothing
  outside it may release an image backing (CODEREVIEW 23).
- **Format identity lives under `dasllama_kqformat.das`** — the `KqFmt` enum, the per-format
  descriptor table (plane strides, block geometry, stream codes), and format predicates. It
  requires nothing dasllama (it is the taxonomy everything else keys off): convert reads it for
  codec strides, repack for layouts, layout/common for the loader walks, gemm_schema for the
  kernel side. ONE id space — the enum; int ids exist only at the IR/kernel-param boundary.
- **RoPE angle/table generation lives under `dasllama_rope.das`** — the theta schedule +
  `rope_freqs`/fscale/mscale in every materialized layout, pure functions over plain params;
  Model-facing wrappers stay in common. Application kernels stay with their backends: the CPU
  `rope_scaled_*` variants and the GPU fused rope-store kernels are shape-specialized on purpose
  (hot-loop branch elimination / fusion) — single-sourcing them is generator-rail work, not a
  hand-merge.
- **A GPU backend is a FAMILY of role files — matching things in matching files across
  backends.** The roles: `dasllama_<gpu>_kernels` (kernel source + the derived-access/PSO census
  — no device state, no engine types), `dasllama_<gpu>_common` (device state, buffer/command
  plumbing, the hazard/capture rail, profiler, shared lazy-state builders; module-level state
  deliberately NON-private — the drivers assign it), `dasllama_<gpu>_decode` (the resident
  token-step driver + decode-time arms), `dasllama_<gpu>_prefill` (the batched prefill driver +
  batch arms), `dasllama_<gpu>_shapes` (PORTABLE servability gates — no GPU C++ requires),
  `dasllama_<gpu>_lens` (the kernel-access macro), and — VULKAN ONLY — the entry
  `dasllama_math_vulkan` (capability probe/arm, `.dlim` identity source, cross-arm routers, the
  `[init]` hook installs — re-exports the family `public`, and its NAME is common's `?vulkan`
  require contract: never rename it). Metal has NO math_ entry: the family enters via the
  transformer's `?das_metal` requires + unconditional shapes, and its below-common piece is
  `dasllama_metal_gemm` (the batch-GEMM donor common requires `?das_metal` — own device by
  necessity: metal_common → dasllama_common → metal_gemm would cycle). Backend-only
  capabilities (vulkan: weight arena, streamed mirrors, heat cache, host-import, coopmat;
  metal: blob transform, MTP) live in their matching ROLE file, not in new grab-bags. Vulkan
  is the deliberately-designed model; metal converges as it's touched. Family-shared kernel
  classes live in `dasllama_metal_kernels` — the `[metal_dispatch]` lens generates enc_*
  builders and MSL globals into the module the class COMPILES in, so co-location follows the
  class ("the builder needs the driver module" is never a placement reason). Prefill's 33
  prefill-only classes are the remaining convergence debt, not precedent.
- **GPU cooperation lives under `dasllama_gpu_tier.das`** — the device-cooperation SPI: hook
  types, install/unset slots, route/mark/want/status state, engine-facing forwarders. Vulkan
  implements it (per-op offload + resident plumbing); Metal deliberately does not — UMA makes
  residency moot there, and Metal integrates as a whole-forward driver through common's override
  registries (which Vulkan's resident driver also registers with). The two-contract reality and
  the direction of travel are recorded in `followup_vulkan.md`; the override registry itself is
  a future seam extraction alongside the loader pull.
- **A new module lands with its records** — the placement rule here, the matching CODEREVIEW.md
  rule, and targeted tests, all in the same change (CODEREVIEW.md "THE PATTERN").
- **Every extraction ships targeted tests for the extracted bits themselves** — unit-level on the
  moved surface, not "run an LLM and see if it still talks". The end-to-end oracles stay the
  bit-identity gate; they are not the extraction's test.
- **Platform backends implement narrow registered contracts** — `KernelBackend` slots, MoE-GPU-tier
  hooks, layout/stream hooks. Platform-specific code must not live in a platform-neutral module (the
  Vulkan bake state machine and Metal knobs now sitting in `dasllama_common.das` are debt, not
  precedent).

## Inherited invariants

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
  disk. Measured on Llama-3.1-8B-Q8_0 (9.6 GB image, M1 Max): planar cold **3.7 s → 4.5 s**, peak
  **12.3 GB → 4.0 GB**; metal cold unchanged in both. We bought a 3x footprint cut with ~20% of one
  cold start. Apply the same tiebreak everywhere — bake, convert, KV growth, GPU staging — and when
  a change goes the other way, it needs the measured pair (peak AND wall) and an explicit call, not
  an assumption that faster is better.
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
