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
  `dasllama_<gpu>_lens` (the kernel-access macro), and the entry `dasllama_math_<gpu>`
  (capability probe/arm, `.dlim` identity source, cross-arm routers, the `[init]` hook installs
  — re-exports the family `public`, and its NAME is the `?<gpu>` require contract in common:
  never rename it). Backend-only capabilities (vulkan: weight arena, streamed mirrors, heat
  cache, host-import, coopmat; metal: blob transform, MTP) live in their matching ROLE file, not
  in new grab-bags. Vulkan is the deliberately-designed model; metal converges as it's touched
  (`metal_llama` → `metal_decode` done; the family-shared kernel classes live in
  `dasllama_metal_kernels` — the `[metal_dispatch]` lens generates enc_* builders and MSL
  globals into the module the class COMPILES in, so co-location follows the class; the old
  "prefill must host them" claim was wrong. Prefill's 33 prefill-only classes are the
  remaining convergence debt, not precedent).
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

## What went where — the extraction ledger

| date | new home | what moved in | pulled from | notes / why |
|---|---|---|---|---|
| 2026-07-30 | `dasllama/dasllama_metal_kernels.das` (M1 of the metal family convergence) | the 45 family-shared `[metal_kernel]` classes BOTH drivers dispatch (~2,685 lines with docs): MetalRmsNorm/Add/Swiglu/Geglu/SwigluOai/Sigmul/AxpySig ew, MetalQ8MulMm + the kq mul_mm K4/K5/K6, the MoE chain (Router/Select/Combine/WScale/G4RouterNorm) + its GEMV six-pack (q8/k4/k5/k6/mx4/q51), the kq GEMV family (k4/k5/k5c/k6) + the B2/B4/B8 batch forms × 3 codecs, MetalQkNorm, MetalSuppressRow/SoftcapRow, the DeltaNet chain (DnConv/DnConvHist/DnL2Norm/DnScan/DnGate/DnDeint) | `dasllama_metal_prefill.das` (8,240 → 5,507 lines) | Verbatim moves. The `[metal_dispatch]` lens generates enc_* builders + MSL globals into `compiling_module()`, so everything lands where the class compiles — the "co-location is forced onto prefill" claim was survey-disproven. Requires go one-directional: kernels and decode both DROP `require metal_prefill`, prefill ADDS `require metal_kernels` (+ kernels gains `daslib/math_bits` for the mx4 GEMV) — final shape shapes → common → kernels → {prefill, decode} sibling drivers. The 33 prefill-only classes (mul_mm T-forms, PLE, attention GEMM trios, MoE mul_mm set, rope, DnBa) stay in prefill — ledgered debt. Kernel-lab benchmarks re-pointed at the moved MSL: gemv/gemm labs + bench_metal_ew swap `require metal_prefill` → `metal_kernels`; moe_lab + occupancy_report require BOTH (they also bind prefill-only mul_mm/attn sources). Gates: family + harness + benchmark compile sweep; kernels suite 2/2 + prefill `base` + decode `arm1-basic`; full oracle metal leg vs the stored board. |
| 2026-07-30 | shared helpers INSIDE the Vulkan family (the post-split dedup phase, 7 commits `1cbe51df1..1c720cbd4`, each zen2-gated on `test_vulkan_tier` 34 + `test_vulkan_tier_cm2` 6) | (a) `vk_set6`/`vk_write6` in common — `rd_set6` generalized into the descriptor plumbing; ALL 19 hand-rolled 6-binding set builds converted (`ensure_*` family, `ffn_cmd_for`/`qkv_cmd_make`/`dnd_step_make`, arena scratch); chain-private set hazard bits moved to build time (role-fluid `raw_set`s still re-declare per record). (b) `wg_rms_inv` + `ar_accum_row` in kernels — the 5× verbatim subgroup rms reduce is one source; kernel `ar_add_rms` DELETED (≡ `ar_add_rms_b` at row 0; every dispatch was already 1 wg) and the two ar pipeline fields collapsed. (c) the kq batch-tile stages `kq_bt_load_x/_w4/_w5/_w6/_xs/_dm`, `kq_bt_dots` (`int4&` ref accumulator), per-block `kq_bt_fma_dm`/`_fma_q40` (float4 lane-wise ≡ the scalar quadruple, per-block `+=` keeps float order), `kq_bt_store_y`. (d) `kq_gemv_dm` + `q40_blk_d`. (e) `q8_blk_pack`, `q8k_blk_scale` + `q8k_pack_word`. (f) the flash Q-tile core `fa_load_q`/`fa_load_k`/`fa_qk_score`/`fa_softmax_update` (at_attn ↔ da_attn_b). (g) `vk_region_rec` — the 4-word region-map record decoded in 12 kernels, one source. | copy-paste inside `dasllama_vulkan_kernels/common/decode/prefill` | Mechanism: shared das functions the shader compiler inlines (the `k5_dep` pattern). Shader-fn ABI: fixed-array params REJECTED (50501); scalar/vector/matrix + plain struct, by value or ref (`spirv_emit` copy-in/out temps), vector returns fine. Float-order discipline: helpers preserve verbatim op order (the `ar_rq`/`qkn_rope_kv` BIT-IDENTICAL promises hold); per-block `+=` granularity so accumulation order never changes. What deliberately STAYS separate: the coopmat mm pair + `da_attn_b_h128` (deliberate variants — tune_perm territory), the GEMV quartet skeletons (dispatch identity), q40/k6 scale stages + k6's split-half dot (per-format math), the batch `_dm` float-first fold vs GEMV's int-exact `sc*idot` (different by design), the two V-tile loads (different bindings). |
| 2026-07-30 | the Vulkan backend FAMILY: `dasllama_vulkan_kernels.das` (~3,630), `dasllama_vulkan_common.das` (~2,730), `dasllama_vulkan_decode.das` (~1,220), `dasllama_vulkan_prefill.das` (~1,520), `dasllama_math_vulkan.das` (~300, keeps its name — common's `?vulkan` require contract) | the whole 9,340-line blob, split by ROLE: 43 `[compute_shader]` kernels + the derived-access spec → kernels; GpuState/g_gpu (190 fields), buffer/descriptor/command plumbing, upload/stacks/host-import, weight arena, hazard rail + lens registry, profiler, the shared fill-meta family, the single-op seams, shared `ensure_*` builders → common; RDec setup + `rd_record_token`/`vk_rdec_token`, KV sync/read, dnd step, FFN GEMV/qkv/cls arms, heat cache → decode; `pf_setup`/`vk_rdec_prefill`, arena FFN seams, batched GEMM chains, dn + attention dispatch, streamed mirrors → prefill; probe/arm/dry-bake/dlim identity/FFN router/`[init]` → math_vulkan (re-exports the family `public` — zero consumer churn) | `dasllama_math_vulkan.das`, previously one file | Verbatim moves — dedup is the NEXT phase, per-step on zen2. Cut lines follow the audited coupling, not the (lying) section headers: the decode driver sat under the "lens" header, the real plumbing under the "prefill" header. Key constraints honored: the lens `[call_macro]` scans the CALLING module, so the spec is built in kernels (`vk_kernels_access_spec`) and consumed by common's rail; g_gpu/g_rd + structs lose `private` (metal_common precedent — drivers assign them); dep chain kernels ← common ← decode ← prefill ← math_vulkan (prefill needs decode's `vk_moe_dn_step`). Kernel region verified self-contained (zero g_gpu/vulkan-API refs; requires only spirv+math). Gates: family + common + 6 consumer compiles, test_gpu_tier 5/5; mac test_vulkan_tier fails IDENTICALLY pre/post split (MoltenVK runtime — the open vulkan-on-mac goal, not this change); zen2 runs the tier suites for real. |
| 2026-07-30 | `dasllama/dasllama_metal_decode.das` (RENAME of `dasllama_metal_llama.das`) | — | — | Role names over historical accident: the vulkan twin is `dasllama_vulkan_decode`, matching things get matching names. 18 references updated; INVENTORY.md stays frozen. Gates: 9-file compile + metal decode `--arm arm1`. |
| 2026-07-30 | `dasllama/dasllama_convert.das` (module RETIRED: `dasllama_quant`) | the whole of `dasllama_quant.das` (350 lines): `quantize_q8_0(_into/_into_ptr/_bs_into/_bs_into_ptr)`, `quantize_q8_k_into(_ptr)`, `quantize_q4_0(_into)`, the `Q8Tensor`/`Q4Tensor` reference forms + dequants, `quant_error_stats` | `dasllama_quant.das`, deleted — the third conversion home folded into the one the rule names | No shim module (a name that only re-exports is a zombie): 27 requirers swapped `quant`→`convert`, 2 dropped the line (already required convert), `.das_module` deregistered, `[tune_scope]` covers dropped `dasllama_quant` (its two `[tuned]` kernels now live in convert, already covered). Dupe check (Boris asked): NO byte-identical dupes — the q8 quantize core (two-float4-lane amax + `round(src*id)`) is the same MATH in 4 places and q4_0 semantics match `quantize_tq4kv_row`, but each writes different contractual bytes (split-plane f32-scale weights / bs variants / interleaved f16-scale KV blocks) — a shared private amax helper inside convert is a same-module cleanup candidate (auto-inline safe), ledgered not done. Gates: 43-file compile sweep; quant/kquant/kv_codec/matmul_batch/groupn/batch_grid green; parity 45/45. |
| 2026-07-30 | `dasllama/dasllama_par.das` | the dispatch-observability counters (`count_parallel_dispatch(_thru)`, `count_inline_run`, `get_disp_count/chunks/inline`) | `dasllama_math.das` §1170–1191 | Kills the documented require-inversion: the `maybe_parallel_for` macro emits these calls resolving in the CALLER's scope, so quant/convert/gguf had to `require dasllama_math` just to compile the expansion — now the emitted calls resolve through the `dasllama_par` require every caller already has. gguf dropped its math require outright; convert's stays for `get_dispatch_lanes` (the old counters-only comment was incomplete). |
| 2026-07-30 | `dasllama/dasllama_kv_codec.das` | the KV-cache runtime codec, whole families: f16 (`dot_f16`/`axpy_f16`), q8_0 (`dot_q8kv`/`dot_q8q8kv`/`axpy_q8kv`/`cvt_q8kv_to_f32`/`quantize_q8kv_row`), tq4 (`dot_q8tq4kv`/`axpy_tq4kv`/`cvt_tq4kv_to_f32`/`quantize_tq4kv_row` + `fwht_signs_row`/`fwht_unsign_row`/`tq4_fill_signs`) | `dasllama_math.das` (~330 lines); the generic f16 row converts `cvt_f32_to_f16`/`cvt_f16_to_f32` went to `dasllama_convert` instead (dual-use beyond the cache: gguf bulk dequant, the wscale plane, Accelerate batch prep) | Families kept WHOLE — store/read/dot/axpy share block geometry (34B q8_0, 18B tq4), so the earlier "encode to convert, dots stay in math" plan would have put one byte layout in two modules. These are HOT per-token kernels (`[tuned]`), not load-time converters — that's why they're tuned and why convert isn't their home. Sidecars key BARE kernel names → no re-tune on any box; `[tune_scope]` covers gained `dasllama_kv_codec;dasllama_convert`. `dot_bf16` stayed in math (weight-side dot, not KV). 7 consumers now require the codec directly — math cannot re-export it (convert already requires math). Gated by NEW byte-level units in `tests/test_kv_codec.das` (fwht orthonormality + inverse, sign determinism + prefix, tq4 round-trip + idempotent re-quantize, every dot/axpy vs a scalar block reference — integer-accumulating dots bit-exact on any perm) + the existing kv_store_row/layout suites + an 11-file compile sweep. |
| 2026-07-29 | `dasllama/dasllama_gpu_tier.das` | the device-cooperation SPI (~1,035 lines): `MoeGpu*`/`Rdec*` hook types + unset slots, `install_moe_gpu_tier`/`install_moe_gpu_resident`, the `rdec_*` and `matmul_moe_gpu_*` forwarders, heat/shexp/qkv/marks state, `GpuTierWant`/status + ~40 accessors, dry-bake/slice-window/would-accept hooks | `dasllama_math.das` §1390–2424 (math drops 4,330 → 3,296 lines and stops being two modules wearing one name) | `math` re-exports it `public` (112 consumers unchanged); vulkan (implementer) + common (caller) require it directly. Zero perf delta: the hot callers live in common and were already one cross-module hop through the same fn pointers. Charter is honest about the two-contract reality (Metal = override registries only; see `followup_vulkan.md`). Gated by NEW `tests/test_gpu_tier.das` (uninstalled-declines, rdec double routing, want round-trip, arm gate) + parity 45/45 + an 11-file compile sweep. |
| 2026-07-29 | `dasllama/dasllama_rope.das` | RoPE angle/table generation: `build_rope_tabs`, `build_rope_tabs_rows`, `build_rope_row_packed`, the shared `rope_freq_j` schedule (theta + `rope_freqs` divisor) | the three builder bodies in `dasllama_common` (now thin Model-facing wrappers) | Float multiply order preserved per layout and made contractual (tabs: `pos*fscale*freq`; packed row: `pos*freq*fscale`) — parity pins both. Application kernels deliberately NOT moved (CPU `rope_scaled_*` leaves + GPU fused rope-store are shape-specialized hot loops; single-sourcing = tune-phase generator rail). Gated by the new angle-gen pins in `tests/test_rope.das` (pos-0 identity, ff-ones ≡ no-ff, cross-layout equality at fscale=1, rows≡tabs twin, mscale linearity) + parity 45/45. Perf note: the inv-freq cache opportunity ledgered in PERF_LEDGER.md. |
| 2026-07-29 | `dasllama/dasllama_kqformat.das` | the `KqFmt` enum, `kq_sb`, the plane stride truth (`K4_QSB`…`Q51_SB` + typed `kq_qsb`/`kq_ssb`), `kq_elems`, `kq_stream_code`, and the ONE enum→kernel-id bridge `kq_schema_id` | enum + `kq_sb` from `dasllama_common` (which re-exports, zero churn); stride constants from `dasllama_convert`; the bridge from `dasllama_layout`; `gemm_schema`'s int-id stride forms now DELEGATE here (one numeric truth) | Kills the two-id-space footgun at the root (`6` = Q6_K in kernel-id space vs q8n in enum space — pinned by `tests/test_kqformat.das`); `moe_gpu_fmt_kq` (byte-identical `kq_sb` twin) deleted, 11 call sites unified. Still queued in this step: the format if/elif ladder rewrites in common/layout onto the taxonomy; vulkan's private `vk_kq_schema_id` twin (its `fmt` fields are ints — dies when they take the enum). |
| 2026-07-29 | `dasllama/dasllama_convert.das` | the pure codec layer: LE byte readers (`rd_u16`/`rd_u32`/`rd_f32`), `e8m0_to_f32_half`, K-quant superblock dequants + pointer twins, superblock→plane transcodes, plane dequants, the fp32→Q5_K encoder (`quantize_k5_plane`), the plane stride constants, `guard_dst` | `dasllama_gguf.das` (641 lines) | GGUF-coupled drivers (metadata lookup, threading, per-tensor guards) stay in gguf as thin wrappers; gguf does `require dasllama_convert public` so the byte-reader surface keeps reaching the ASR bin readers unchanged. Fixes INVENTORY A21 (encoder in the "reader") and A23 (misleading `gguf:` panic prefix on non-tensor use). Gated by `test_gguf_quant` (7, requires convert directly) + `test_kquant` (105) + `test_repack`'s cross-reader arm (24). Follow-ups in the same pull: the batch requants (`requant_rows_q8`/`_q8k_bs`) moved here as pure forms with an explicit `par` flag — common keeps timed threshold wrappers, audio's drifted copy became a delegation; and the loader's transcode decision tree (`load_big` vs `fill_stream_plane` — two copies the old comment claimed "cannot drift") collapsed onto single-source `transcode_kq_tensor`/`transcode_q8_tensor` dispatchers in common. Deliberately NOT moved yet: `wscale_convert_f16` (Model-coupled — loader pull), the gguf omnibus decoder split (2 pre-existing size lints — queued). The `cvt_*`/KV-codec `[tuned]` kernels moved the next day (see the kv_codec row — the "tune-manifest churn" rationale here was WRONG: sidecars key bare kernel names, a move costs no re-tune). |
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
