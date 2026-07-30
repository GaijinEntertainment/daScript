# dasLLAMA Architecture — what goes where

> Companion to `INVENTORY.md` (the as-is census, 2026-07-29). This document records where
> functionality **BELONGS** and why. It grows one entry at a time as the reorg pulls shared bits
> out of the monoliths — every extraction adds a row to the ledger below and, when it settles a
> placement rule, a bullet to the rules section.

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
  containers and dispatch into the codec.
- **Every extraction ships targeted tests for the extracted bits themselves** — unit-level on the
  moved surface, not "run an LLM and see if it still talks". The end-to-end oracles stay the
  bit-identity gate; they are not the extraction's test.
- **Platform backends implement narrow registered contracts** — `KernelBackend` slots, MoE-GPU-tier
  hooks, layout/stream hooks. Platform-specific code must not live in a platform-neutral module (the
  Vulkan bake state machine and Metal knobs now sitting in `dasllama_common.das` are debt, not
  precedent).

## What went where — the extraction ledger

| date | new home | what moved in | pulled from | notes / why |
|---|---|---|---|---|
| 2026-07-29 | `dasllama/dasllama_convert.das` | the pure codec layer: LE byte readers (`rd_u16`/`rd_u32`/`rd_f32`), `e8m0_to_f32_half`, K-quant superblock dequants + pointer twins, superblock→plane transcodes, plane dequants, the fp32→Q5_K encoder (`quantize_k5_plane`), the plane stride constants, `guard_dst` | `dasllama_gguf.das` (641 lines) | GGUF-coupled drivers (metadata lookup, threading, per-tensor guards) stay in gguf as thin wrappers; gguf does `require dasllama_convert public` so the byte-reader surface keeps reaching the ASR bin readers unchanged. Fixes INVENTORY A21 (encoder in the "reader") and A23 (misleading `gguf:` panic prefix on non-tensor use). Gated by `test_gguf_quant` (7, requires convert directly) + `test_kquant` (105) + `test_repack`'s cross-reader arm (24). Follow-ups in the same pull: the batch requants (`requant_rows_q8`/`_q8k_bs`) moved here as pure forms with an explicit `par` flag — common keeps timed threshold wrappers, audio's drifted copy became a delegation; and the loader's transcode decision tree (`load_big` vs `fill_stream_plane` — two copies the old comment claimed "cannot drift") collapsed onto single-source `transcode_kq_tensor`/`transcode_q8_tensor` dispatchers in common. Deliberately NOT moved yet: `wscale_convert_f16` (Model-coupled — loader pull), the `cvt_*`/KV-codec `[tuned]` kernels in math (tune-manifest churn — tune phase), the gguf omnibus decoder split (2 pre-existing size lints — queued). |
| 2026-07-29 | `dasllama/dasllama_repack.das` | the 7 `repack_*_grp` transforms (q8q8/mx4/q51/k4/k5/k6/q40), the disk-order extractors `k45_nib`/`k5_hbit`/`k6_nib`, `unpack_kq_panel_grp` | `dasllama_math_gen.das` (implementations); `dasllama_layout.das` (byte-identical extractor duplicates, deleted) | Pure transforms parameterized by `(mr, wbias, kgroup)` — number sources stayed with their owners (`_gen` twins in math_gen read tune stamps; `_bake` selectors in math read override getters; lane contexts can't read math's globals). Scratch pre-copies became `memcpy`. `k4_sc_mn` deliberately stayed in `math_default` — hot in the kq dots and auto-inline is same-module-only. Gated by `tests/test_repack.das`: semantics-first extractor writers, inverse-map byte checks, the cross-reader dequant gate (grp reader vs disk reader), chained repack→unpack. |

## Inherited invariants

Durable "why it is built this way" facts harvested from the design docs, which are archiving to
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
  doc's "BNNS not pursued" tail is superseded by the AMX arc (#3562) — `dasllama_math_accelerate.das`
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
