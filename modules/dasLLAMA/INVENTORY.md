# dasLLAMA Inventory — where things are NOW

> Snapshot 2026-07-29 at `b0049dc52` (post site-rework #3589), branch `bbatkin/dasllama-reorg`.
> This document is the **as-is census**: what exists, where it lives, what's duplicated, what's stale.
> Its companion `ARCHITECTURE.md` records where things GO — it grows one entry per extraction as the
> reorg pulls shared bits out. Line numbers below are from this snapshot and will drift as the reorg
> proceeds; they anchor the extraction PRs, not eternity.

## Module at a glance

- **197 `.das` files, ~128,900 lines** under `modules/dasLLAMA/`, plus `utils/dasllama-server/` (16 files) and the untracked `modules/dasVulkan/` backend.
- Directory roles: `dasllama/` engine (58 files, ~72,600 lines) · `tests/` (59, module-owned, NEVER AOT) · `benchmarks/` (59) · `harness/` (78, hand-run probes/tuners/oracles) · `performance/` (33, the publishable rig) · `models/` (silero weights only) · 18 root `.md` docs.
- The five monoliths: `dasllama_common.das` 13,684 · `dasllama_math_vulkan.das` 9,339 · `dasllama_metal_prefill.das` 8,240 · `dasllama_metal_kernels.das` 7,281 · `dasllama_math.das` 4,330.
- Hub files by require in-degree: `dasllama_math` 112 · `dasllama_common` 53 · `dasllama_transformer` 30 · `dasllama_quant` 29 · `dasllama_env` 27 (full table in the Math backends section).
- Audit method: six parallel deep-read passes (common anatomy / math backends / GPU dispatch / families / tools-rig-server / root docs), 2026-07-29. Duplication percentages marked "measured" come from line diffs, not eyeballing.

## dasllama_common.das — the 13,684-line monolith

`module dasllama_common shared public` — the whole engine in one file: types, GGUF loader, layout/format machinery, GPU-tier upload walks, CPU decode+prefill block kernels, KV-cache codecs, MoE dispatch, MTP, sampling, arch registry.

Flag legend: **[a]** kernel data repacking (→ `dasllama_repack.das`), **[b]** tensor format conversion (→ `dasllama_convert.das`), **[c]** platform-specific in "common", **[d]** model-family-specific in "common".

### Section map

| lines | section | flags | description | key symbols |
|---|---|---|---|---|
| 1–29 | module header + requires | c | 24 requires, five PATH-GUARDED backends (`?llvm`, `?das_metal`, `?das_accelerate`, `?vulkan`) + unconditional `dasllama_math_aarch64_neon`. Platform fan-out decided here. | — |
| 30–199 | core enums + `Config` | b, d | Format/mode taxonomy + 85-field hyperparameter struct; ~60% per-family flags (gemma2/3/4, qwen, phi3, gpt-oss, glm4moe, MTP). | `QuantMode`, `KqFmt`, `FfnAct`, `MoeGate`, `KVDtype`, `TQ4B`, `Config` |
| 200–323 | block-kernel typedefs + override registries | c, d | Six fn typedefs + five name-keyed override tables (prefill/decode/batch-decode/MTP-spec/MTP-seam) + metal-servable gate. | `*OverrideFn`, `register_*_override`, `select_prefill_override`, `register_metal_servable` |
| 324–386 | disk→compute layout hooks | **a** | The repack/gather seam: four `ModelRepackFn` slots + three gather typedefs, registered by `dasllama_layout` at `[init]`; unset hooks panic. **First extraction anchor.** | `ModelRepackFn`, `GatherStack*Fn`, `register_model_layout`, `repack_{q8,kq}_weights`, `repack_{mx4,q51}_stacks` |
| 387–622 | vulkan-flavor image bake + GPU gather wrappers | **c**, a | Collect/slice state machine recording GPU-layout gathers into `Model.vkblob` + replayable plan. Pure Vulkan concept in common. | `VkPlanEntry`, `VkBakeRole`, `vulkan_bake_*`, `vk_bake_*`, `moe_gpu_gather_stack*`, `SliceWindow` |
| 623–705 | `trim_model_planes` (P3 trim) | **a**, **b**, c | Repacks emb/cls region into `embq`/`embs`; frees 17 weight families. Vulkan-plan-gated. | `trim_model_planes` |
| 706–786 | decode-override select + CPU-prefill tripwires | c | `CPU_PREFILL_PANIC_NPOS=32` Metal-build guard, `blob_only_panic`. | `select_decode_override`, `allow_cpu_prefill`, `blob_only_panic` |
| 787–860 | `MetalMode` + decode rope-row API | **c** | Metal engagement knob, logits-done flags, public rope-row builders. | `MetalMode`, `set/get_metal_mode`, `build_decode_rope_row(_swa)` |
| 861–1149 | `ArchBlocks` + `Model` + finalizer | a, b, c, d | 226-line `Model`: 24 weight-plane arrays, `kq_repack_*` widths, per-kind `KqFmt` tag arrays, family-specific offsets. | `ArchBlocks`, `Model`, `finalize(Model)` |
| 1150–1215 | chat template + arch descriptor types | d | Data-only chat/tool template model. | `ChatPart*`, `ChatTemplate`, `LlmCaps`, `ArchDesc` |
| 1216–1562 | layout cursors + `layout_offsets` | b, d | Six-plane cursor allocator + 257-line walk assigning every weight offset; riddled with family branches. | `LayoutSizes`, `KqCursors`, `kq_take`, `fmt_at`, `layout_offsets` |
| 1563–1644 | `load_checkpoint` (llama2.c) | d | Legacy fp32 checkpoint reader; hardcodes llama. | `load_checkpoint` |
| 1645–1714 | conversion-time accounting | **b** | Per-conversion-kind timing buckets (`DASLLAMA_CONV_PROF`); bucket names ARE the conversion taxonomy. | `ConvBucket`, `conv_note`, `conv_profile_report` |
| 1715–1867 | streaming plan/fill types + hooks | **a**, **b** | `FillJob` (replayable conversion) + `StreamReg` (repack region), collector hooks + repack dispatcher registered by layout. | `FillJob`, `StreamReg`, `StreamCollectFn`, `StreamRepackFn`, `register_stream_layout`, `stream_*` |
| 1868–2025 | `fill_stream_plane` | **a**, **b** | The streamed converter: transcode per job, land scales, apply repack regions, append, `wscale_convert_f16` epilogue. **Both extraction targets braided here.** | `fill_stream_plane`, `fill_plan_begin/take` |
| 2026–2144 | `load_big` + `load_dn_grouped` | **b**, d | Eager conversion ladder (SAME branch tree as `fill_stream_plane`, written independently) + qwen3next deltanet permute. | `load_big`, `load_dn_grouped` |
| 2145–2207 | arch registry | d | Name-keyed `ArchDesc` table. | `register_arch`, `resolve_arch`, `rebind_arch_blocks` |
| 2208–2284 | load-time A/B knob globals | b | Five load-time format knobs. | `set/get_wscale_f16`, `set/get_kquant_native`, `set/get_kq_q40/q50/q51_native`, `set/get_resilient_load` |
| 2285–2321 | `DlimConfiguration` sources | a, c | `[init]` cpu-identity source publishes active repack widths (`q8_mr`, `kq_mr4/5/6/40`, `q51_mr`…) + metal source. | `dlim_cpu_source_impl`, `dlim_metal_source_impl` |
| 2322–2352 | load self-report | b | Per-format tensor tallies. | `tally_fmt`, `log_load_report` |
| 2353–2499 | disk-type → format-tag detection | **b**, c, d | `kq_fmt_of` GGML→`KqFmt`; alignment demotions; `detect_kq_formats` fills 13 tag arrays; family- and platform-aware (dn demotes when Metal/GPU armed L2462–2464). | `kq_fmt_of`, `detect_kq_formats` |
| 2500–2548 | fmt predicates + `wscale_convert_f16` | **b** | fp32 weight-scale plane → raw binary16, frees f32 plane. | `wscale_convert_f16` |
| 2549–2593 | GPU-tier format predicates + gather-upload | **a**, c | q8→q8n promotion; `moe_gpu_gather_upload` (every upload goes through it). | `moe_gpu_fmt_kq`, `moe_gpu_gather_upload` |
| 2594–2753 | `ResidentPlan` + resident state + `resident_place` | **c**, a | Vulkan resident-driver sizing/decline API. | `ResidentPlan`, `resident_place`, `rdec_*` |
| 2754–2892 | `resident_upload` | **c**, a, d | 136-line all-or-nothing whole-stack Vulkan arm; rejects 12 family features by name. | `resident_upload` |
| 2894–3191 | vulkan resident overrides + marks swap | **c** | Three registered "vulkan" overrides with hand-rolled rope rows + multi-model marks save/restore/drop. | `vulkan_resident_decode/prefill/batch_decode`, `GpuModelMarks`, `moe_gpu_*` |
| 3193–3626 | `moe_gpu_upload_resident` | **a**, **c**, d | 430-line GPU upload walk: seven rail walks, rollback on partial groups, `VkBakeRole` stamps. | `moe_gpu_upload_resident` |
| 3628–3804 | load entries + weights epoch | c | `load_gguf`, `load_gguf_streaming`, `load_gguf_impl`. | `weights_epoch`, `load_gguf*` |
| 3805–4689 | `load_gguf_parsed` | **b**, **a**, **d**, c | **882-line monster**: metadata→Config (~500 lines per-family key parsing), plane sizing, reads/transcodes, repack stage (L4638–4672), upload. | `load_gguf_parsed` |
| 4691–4718 | `quantize_weights` / `_q4` | **b**, **a** | Post-hoc fp32→Q8_0/Q4_0 + repack + upload. | `quantize_weights(_q4)` |
| 4720–5013 | `MemFootprint` + `KVPool` + `Session` | b, d | 226-line `Session` (~120 scratch arrays, family-partitioned). | `MemFootprint`, `KVPool`, `Session` |
| 5015–5231 | session construction + `eval_` | b, d | 144-line `make_run_state`, paged twin. | `make_run_state`, `create_session_` ×2, `eval_` |
| 5232–5370 | box-profile runtime knobs + `load_model_` | **c** | ~30 JSON knobs incl. `metal_tensor` crowns; tokenizer facade. | `apply_box_profile_runtime`, `load_model_` |
| 5372–5555 | matmul/rms/rope wrappers + rope tables | d | Blob+offset kernel wrappers; rope table builders. | `mm_at`, `rms_batch`, `rope_batch*`, `build_rope_table*` |
| 5556–5759 | KV codec seam + paged pool | b | Codec byte arithmetic (f32/f16/q8_0 34B/tq4 18B), `KVRun`, pool lifecycle. | `kv_row_bytes`, `kv_base_k/v`, `create_kv_pool_`, `kv_ensure` |
| 5760–5923 | `kv_store_row` / `kv_load_row` + codec glue | **b** | KV write/read conversion seam + 5 overload families — 24 small conversion/dispatch fns. | `kv_store_row`, `kv_load_row`, `kv_dot`/`kv_axpy`/`kv_row_to_f32`/`kv_score` sets |
| 5924–6286 | codec-generic decode attention + flash witnesses | b, d | 4×4 K×V codec dispatch matrix longhand; recompute witnesses. | `attn_head_decode(_d)`, `kv_slice_*`, `flash_decode_check_g/d` |
| 6287–6393 | `kv_store_batch` + tq4 rotation + gate | **b**, a, d | Threaded batched KV store; tq4 FWHT basis change; fused activation+gate. | `kv_store_batch`, `tq4_rotate_batch`, `gate_batch` |
| 6395–6470 | q8/q4 matmul wrappers + `mm_qkv` fuse | b, c, d | `mm_qkv` 5 arms incl. GPU submit + gemma4 V-from-K. | `mm_at_q8`, `mm_qkv` |
| 6472–6734 | pre-quantized / q51 / native-kq matmul dispatch | **a**, b | 20 dispatch wrappers. `kq_servable` = repack-state gate (L6691). | `mm_at_*_pre`, `mm_at_*_groupn`, `kq_servable`, `kq_plane_q/s` |
| 6736–6829 | `mm`/`mm_b` dispatch + batch requant | **b** | Two threaded batch activation quantizers (Q8_0; Q8_K). | `mm`, `requant_rows_q8`, `requant_rows_q8k_bs`, `mm_b` |
| 6831–6941 | per-layer attention geometry | **d** | 18 accessors: gemma4 sliding/global, deltanet masks, PLE, dual-rope. | `layer_is_sliding`, `layer_is_recurrent`, `dn_*`, `layer_*` |
| 6942–7422 | fused attention chain (decode) | c, d | 11-clause gate + 437-line team chain with flash-decode slicing. | `fused_attn_ok`, `attention_chain_decode` |
| 7423–7604 | fused FFN chain (decode) | b, c | Fused gate/up→activate→requant→down team chain. | `fused_ffn_ok`, `ffn_chain_decode` |
| 7605–7798 | `attention_std_decode` | d | Unfused reference twin. | `attention_std_decode` |
| 7799–8200 | qwen35 deltanet + gated attention (decode) | **d**, c | Recurrent block incl. GPU arm; entirely qwen35/qwen3-next. | `dn_*`, `deltanet_decode(_gpu)`, `attention_qwen35_*` |
| 8201–8413 | dense FFN decode + MoE router | d | Three gating funcs, selection bias, top-k renorm, shexp gate. | `ffn_dense_decode`, `moe_select(_core)` |
| 8414–8697 | expert-reuse instrument + heat-cache policy | c | Two INTERLEAVED instruments (see anomalies). | `ReuseLayer`, `HeatLayer`, `heat_*`, `expert_reuse_*` |
| 8698–9204 | MoE expert dispatch (decode) | **a**, b, c, d | 315-line `moe_experts_apply` across q8/mx4/q51/kq + GPU heat-hit split. | `moe_experts_chain/apply`, `moe_ffn_core` |
| 9205–9318 | gemma-4 E-series PLE | **d** | Per-layer-embedding side input. | `ple_*` |
| 9319–9469 | gemma4 MoE FFN + `ffn_moe_decode` | **d** | gemma4 custom router, sandwich norms, output scale. | `gemma4_*`, `ffn_moe_decode` |
| 9470–9623 | embedding-row dequant | **b**, a, c | `dequant_q8_row` (incl. mblob Metal arm), `embed_row` gathering through repacked grp<mr> layout. | `dequant_q8_row`, `dequant_embq_row`, `embed_row` |
| 9625–9723 | classifier matmul + softcap chain | b, c | `mm_cls` 4 arms + trim fail-closed panic. | `mm_cls`, `cls_softcap_chain` |
| 9725–9809 | `forward` (single-token decode) | c, d | `[no_alloc, no_env, no_io]` decode entry. | `forward` |
| 9811–10104 | MTP/NextN self-spec decode | **d**, c | Whole speculative-decode rail. | `forward_mtp`, `mtp_*`, `generate_mtp_greedy` |
| 10105–10331 | section profiler + thresholds + A/B knobs | — | Six par-thresholds, nine A/B knobs, timing buckets. | `g_*_par_threshold`, `forward_profile_*`, `prof_add` |
| 10332–10923 | prefill attention head kernels | b, d | Six head kernels (flash/blocked/classic × f32/`_d`). | `attn_head_*(_d)`, `prefill_attention` |
| 10924–11173 | std/dense prefill blocks | c, d | GPU prefill arm + gate; `attention_std_prefill`. | `attn_qkv_rope_prefix`, `attention_gpu_prefill` |
| 11174–11734 | deltanet prefill + qwen35 prefill + dense FFN | **d**, c | 137-line chunked delta-rule head; GPU arm. | `dn_chunked_head`, `deltanet_prefill*`, `ffn_dense_prefill` |
| 11735–11885 | MoE prefill gather + mx4→q8 expansion | **a**, **b** | `moe_gather_rows` + **`expand_mx4_region_q8`** (MXFP4→exact Q8 directly in backend's interleaved layout, or row-major + `repack_q8q8_weight`). **Densest repack+convert function in file.** | `moe_gather_rows`, `expand_mx4_region_q8` |
| 11886–12329 | `ffn_moe_prefill_grouped` | a, b, c, d | 400-line CSR-bucketed grouped MoE prefill, LPT-ordered GEMM chains, GPU gates. | `ffn_moe_prefill_grouped` |
| 12330–12651 | gemma4 batched router + gemma4 MoE prefill | **d** | 253-line grouped MoE prefill + naive reference. | `gemma4_router_batch`, `gemma4_moe_prefill_grouped`, `ffn_moe_prefill` |
| 12652–12820 | prefill entries + embedding API | d | `forward_prefill`, embedding twin, public surface. | `forward_prefill(_embd)`, `embed_*` |
| 12821–12972 | prefill scratch alloc + body + `forward_batch` | c, d | Family-partitioned scratch sizer; CPU-prefill-on-Metal panic (L12921). | `forward_prefill_alloc/body`, `forward_batch` |
| 12974–13326 | batched decode | b, d | `BatchWorkspace` (cache-less scratch Session per row). | `BatchWorkspace`, `attention_batch_decode`, `eval_batch_` |
| 13327–13592 | greedy generate + sampling | — | `argmax`, `generate`, penalties, top-k/p/min-p, streaming generators. | `generate(_)`, `SamplingParams`, `sample_` |
| 13594–13685 | stats + shared arch-registration surface | **d** | Chat-part builders, Hermes tools, **five per-family block sets** — the family taxonomy in code. | `Stats`, `std_blocks`, `moe_blocks`, `gemma4_blocks`, `qwen35_blocks`, `qwen35moe_blocks` |

### Extraction-target roll-up

**(a) repack candidates, dependency order:** L324–386 (hook contract) → L387–622 (bake + gathers) → L2549–2593 (`moe_gpu_gather_upload`) → L2705–2727 (`resident_place`) → L1715–1867 + 1868–2025 (`StreamReg` + per-job repack application) → L623–705 (`trim_model_planes`) → L11786–11884 (`expand_mx4_region_q8`). Repack-state readers: `kq_servable` (6691), `mm_at_kq_pre` (6539), `embed_row` grp gather (9548–9580), `Model.kq_repack_mr4/5/6/40` (973–976), `dlim_cpu_source_impl` (2287–2305).

**(b) convert candidates:** L1645–1714 (taxonomy+accounting), L1897–1942 + L2026–2083 (the TWO copies of the transcode tree), L2353–2499 (`detect_kq_formats`), L2500–2548 (`wscale_convert_f16`), L4691–4718 (`quantize_weights*`), L5760–5923 (KV codec seam), L6287–6340 (tq4 basis change), L6746–6791 (batch requants), L9470–9623 (embedding-row dequant).

**(c) platform-specific in common:** Vulkan L387–622, L2594–3191, L3193–3626 (~1,100 lines). Metal L307–323, L787–823, L5242–5260 + mblob arms + CPU-prefill panic L12921. GPU arms inside CPU kernels: `deltanet_decode_gpu` (7917), `deltanet_prefill_gpu` (11450), `attention_gpu_prefill` (11031), heat-cache split-FFN (8435–8541).

**(d) family-specific in common (~2,500–3,000 lines):** `Config` (113–198), `load_gguf_parsed` config parse (3809–4300), qwen35 deltanet (~900 lines: 7799–8200, 11174–11697), gemma4 PLE (9205–9318) + gemma4 MoE (9319–9469, 12330–12611), MTP/NextN (9811–10104), `load_checkpoint` (1563–1644), `*_blocks` sets (13648–13684).

### Consumers

36 direct requires. `utils/dasllama-server/` has ZERO direct — all via `dasllama/dasllama`. Engine-internal 18 (incl. `dasllama_transformer.das:9` `require ... public` — the umbrella that breaks the arch↔engine cycle; `dasllama_layout.das:8` registers the repack/gather hooks back; 5 metal drivers; 6 ASR towers all just for `apply_box_profile_runtime`). Arch registrations 13 (all line 6). Benchmarks 4 ([init]-only). Harness/perf/tests 5. Also by name: `.das_module:8`, `CMakeLists.txt:13`, **`tests/test_facade_docs.das:18` (doc-coverage gate — extraction must update this list)**.

### Anomalies

**Dead code (zero callers repo-wide):**
1. **`resident_prefill` (L2952–2986)** — public, zero callers; near-dup of registered private `vulkan_resident_prefill` (L2991–3033).
2. `get_conv_profile` (L1677); 3. `heat_report_reset` (L8475); 4. `expert_reuse_top` (L8679); 5. `embed_forward` (L12770, only caller is `embed_`).

**Duplicated blocks:**
6. **4× hand-rolled rope cos/sin loops** L2930–2938, L2972–2980, L3013–3023, L3078–3086 — byte-identical; all bypass `build_rope_table` (5464) and `build_decode_rope_row` (847).
7. `kq_sb` (L62) ≡ `moe_gpu_fmt_kq` (L2551) byte-identical predicates.
8. `kq_repack_mr*` ternary copy-pasted L2582–2585 + L2715–2717; hand-rolled variant L9545–9547.
9. **~13 independent `KqFmt`→plane/stride if/elif ladders**: L656–681, 1239–1268, 1786–1800, 1806–1817, 1822–1833, 1946–1958, 1963–1985, 2033–2054, 2354–2373, 6539–6560, 6667–6684, 9558–9580, 9590–9616. A `KqFmt`-keyed plane-descriptor record collapses all.
10. **`load_big` (2026–2083) and `fill_stream_plane`'s per-job arm (1897–1942) = same decision tree twice.** Comment at 1717–1718 claims they "cannot drift" — they can.
11. `dequant_q8_row` mblob arm (9476–9487) ≡ `dequant_embq_row` (9519–9531) modulo source array.
12. Fused/unfused decode twins (`attention_chain_decode`/`attention_std_decode`, `ffn_chain_decode`/`ffn_dense_decode`) — ~800 lines of deliberate parallel logic under `g_fused_decode`; largest maintenance-hazard pair.

**Misplaced/stale comments:** 13. GGUF-load `//!` at 3628–3630 sits above `g_weights_epoch` global. 14. Orphaned `load_big` comment at 1645–1647 (function is 380 lines later). 15. `//!` on private fns 2894, 3035, 13327. 16. Section dividers braid reuse-instrument and heat-cache (8414/8435/8542). 17. Unreachable panics 5790, 5817.

**Stale line refs from sibling docs into this file:** `kv_cache_compression.md:30` (:1429 → actually 4853), `:150` (:2838 → actually 5898); `qwen2_audio_plan.md:56` (:4184 → actually 12652); `utils/dasllama-server/CONTROL_PAGE_PLAN.md:32` (:7155 wrong); `API_REWORK.md:1021` calls the file "~1940 lines" (7× drift).

**Scale:** >250-line functions: `load_gguf_parsed` 882, `attention_chain_decode` 437, `moe_gpu_upload_resident` 430, `ffn_moe_prefill_grouped` 400, `moe_experts_apply` 315, `layout_offsets` 257, `gemma4_moe_prefill_grouped` 253. 226-line structs: `Session`, `Model`. 29 in-file `nolint` suppressions — any split must carry them.

## Math backends (11 files, 20,869 lines)

Scope note: GPU-side *load-time* layout rearrangement (`moe_gpu_gather_stack`, `_q8n`, `_kq`, `convert_model_to_metal_blob`) lives in `dasllama_layout.das`; both GPU backends register no-op repacks.

### Per-file census

**`dasllama_math.das` — 4,330, `shared public`.** The hub; required by ~115 files (every dasllama module, all tests/harness/benchmarks/performance).
1–27 header · 29–131 `[tuned]` scalar primitives (`dot`/`axpy`/`dot_f16`/`axpy_f16`/`dot_bf16`/`cvt_f32_to_f16`/`cvt_f16_to_f32`) · 134–255 Q8 KV codec (`dot_q8kv`, `dot_q8q8kv`, `axpy_q8kv`, `cvt_q8kv_to_f32`, `quantize_q8kv_row`) · 257–410 FWHT/TQ4 KV codec · 412–478 elementwise · 480–637 jobque knobs + trace tags · 585–818 dispatch shaping (`lanes_for_work`, `matmul_chunks`, `matmul_grid`, `lpt_order`, `chain_stage_chunks`) · 819–973 fp32 matmul + accelerate override hooks · 974–1087 fp32 micro-GEMM · 1089–1148 `dot_q8`/`matmul_q8` · 1149–1190 token-block width/L2 budget · 1192–1389 **KernelBackend registry** (30 fn typedefs, struct at 1272–1350, 40 `g_mm_*` slot globals) · 1390–2005 **MoE-GPU-tier hook surface** (`MoeGpu*` typedefs + unset stubs + route/mark setters; `Rdec*` resident hooks 1485–1573; `MoeGpuMarks` 1873–1937) · 2006–2424 GPU tier want/status/arm/vram/bake wrappers · 2425–2810 backend activation (registry, identity repacks 2441–2457, q51 expand 2459–2688, cross-box bake overrides 2702–2810) · 2812–3252 panic stubs, `kernel_backend_has_*`, `repack_*_weight` · 3253–3535 the public matmul overload wall (f32 + f16-scale twins) · 3536–3600 `dot_q4`/`matmul_q4` · 3601–4330 activations/norms/rope (`rmsnorm`, `softmax(_sink)`, `silu`/`gelu`/`swiglu`/`geglu`/`swiglu_oai` + 4-wide twins, `softcap`, `rope*`).

**`dasllama_math_default.das` — 892, "portable" backend.** Required by 25. 20–65 `[tuned] dot_q8q8(_f16s)` · 67–144 idot4x4 · 146–177 dispatch profiler · 179–285 kernel/batch/rows/group3 · 286–371 `_s16` twins · 373–601 native K-quant dots + rows kernels · 603–733 `dequant_kq_row_grp`, `matmul_kq(_groupn)` · 734–880 MXFP4 · 881–892 `[init]` registers **"portable"**.

**`dasllama_math_aarch64_neon.das` — 471, "arm64-sdot" backend.** Required by 12. idot4x4 26–80 · `[tuned] dot_mx4q8` 82–124 · groupn 125–176 · laneq dots 177–341 · rows 343–350 · `_s16` twins 352–437 · `[init]` 451–471.

**`dasllama_math_accelerate.das` — 250.** NOT a KernelBackend — installs float-batch override hooks. 32–46 strips · 48–134 BNNS f16 lane (bf16→f16 cached plane convert w/ sampled fingerprint) · 136–175 `accel_bf16_f16_lane` · 184–250 batch overrides + `[init]`.

**`dasllama_metal_gemm.das` — 442, "metal" backend at priority −1.** (Its byte-identical rename leftover `dasllama_math_metal.das` was deleted in the reorg.) 37–211 two GEMM shader classes (`MetalQ8Gemm` 32×32, `MetalQ8Gemm64`) · 216–218 `gemm_use64` · 220–343 host driver · 348–403 `metal_q8q8_batch` (the one live slot; CPU fallback on small/odd shapes) · 405–442 probe + 3 panic stubs + `metal_repack_noop` + `[init]`. Consumers: `dasllama_metal_prefill:14`, `benchmarks/matmul/occupancy_report`.

**`dasllama_math_vulkan.das` — 9,339, "vulkan" backend at priority −1.** Pulled only via `require ?vulkan` (`dasllama_common:17`) + 2 tier tests. Split: header 1–50 / **shaders 51–3659 (3,609 lines)** / **host 3661–9339 (5,679 lines)**.
Shaders: 65–113 `q8_moe_groupn` · 115–228 `q8_moe_batch` (sdot4 32×32) · 230–873 coopmat variants (`_cmf16`/`_cmi8`/`_mm`/`_mm_a`/`_mm_m`) · 875–975 `kq_moe_batch_q40_cmf16` · 977–1102 cm2 twins · 1104–1117 `q8_moe_xf16` · 1119–1233 FFN mid-chain (`act_mul`, `q8_moe_actrq`, `q8k_moe_actrq`, `q8_moe_actf16`, `rd_f16cvt`) · 1235–1259 `moe_combine` · 1261–1834 deltanet · 1836–2064 attention (`at_prep_*`, `at_attn`) · 2066–2914 resident residual-stream (`ar_add_rms(_b/_rq)`, `qk_rms`, `rope_kv_store(_b)`, `qkn_rope_kv`, `da_attn(_b/_b_h128)`, `q8k_requant`) · 2916–3150 kq decode GEMV family · 3152–3659 kq batch tiles (k4/q40/k5/k6).
Host: 3661–3823 capacity consts + tile routers · 3824–4156 structs (`HostBuf`…`GpuState` 218 lines) · 4158–4349 counters + buffer makers + ReBAR + `resolve_coopmat_mode` · 4351–4466 heap/budget + `vulkan_dry_bake_arm` · 4468–4708 `vk_moe_init` · 4710–5065 upload/stacks/import · 5067–5162 `vk_drop_model_state` · 5164–5345 arena · 5347–5621 standalone seams (`vk_add_rms`, `vk_decode_attn`, `vk_rope_kv_store`) · **5623–7040 resident whole-stack decode+prefill driver** (incl. GPU profiler 6085; **hazard-mask rail 6147–6395**; `rd_record_token` once-per-epoch command recording 6409) · 7041–7358 arena/cmd/submit helpers · 7360–7843 FFN batch+dense · 7845–8305 deltanet dispatch · 8307–8539 attention dispatch · 8541–8735 decode GEMV + qkv group · 8737–8896 heat-pinned expert cache · 8898–9058 streamed prefill · 9060–9120 classifier · 9122–9339 registration (17 hook installs).

**`dasllama_math_gen.das` — 2,207, "arm64-gen"/"x64-gen" backends.** Required by 11 directly (apps get it via `dasllama_common` behind `?llvm`). 1–51 the eight-function-stamp doctrine · 52–97 `[tune_scope]` + layout companions · 99–521 gemv/tile `[tune]` stubs per family (each with `[tune_perm]` grids) · **526–1045 the 7 `repack_*_grp` functions** (q8q8/mx4/q51/k4/k5/k6/q40) + `_gen`/`_bake` twins + `k45_nib`/`k5_hbit`/`k6_nib` extractors (818–838) · 1050–1184 `unpack_kq_panel_grp` + `kq_grp_row_dot_b` · 1185–1892 cell walkers (generic/amx/neon-laneq × q8/mx4/kq) · 1893–2142 `_s16` twins · 2143–2207 `[init]`.

**`dasllama_gemm_gen.das` — 2,492, `shared private`.** LLVM-IR emitter for the `[tune]` stubs; consumed only by `dasllama_gemm_register`. 61–229 perm parsing + `perm_declines` · 230–302 `TileEmit` · 303–552 IR helpers (`sdot_lane`, `emit_block`, scale folds) · 553–611 smmla · 612–730 mx4 · 731–812 q51 · 813–1196 kqv2 · 1197–1356 `emit_one_block`/`emit_slice` · 1357–1886 AMX (tmm config, `emit_amx_tile(_pipelined)`) · 1887–2467 tile/gemv/layout generators · 2469–2492 `[macro_function]` registration.

**`dasllama_gemm_register.das` — 17, shim.** Pulled by `modules/dasLLVM/daslib/llvm_user_modules.das:7` (`require ?dasllama/...`) so dasLLVM never hard-depends on dasLLAMA. **0 in-degree by plain grep — NOT dead.**

**`dasllama_gemm_schema.das` — 79, no requires, required by 9.** THE layout-descriptor source: `Q8RepackType`, `q8q8_repack_type`, block consts (`Q8_QPB/SPB`, `KQ_SUPERBLOCK_ELEMS`, `Q51_QPB/SPB`, `Q8N_BPB=34`), `kq_qsb(fmt)`/`kq_ssb(fmt)`.

**`dasllama_quant.das` — 350, required by 30.** The convert nucleus: `quantize_q8_0(_into/_into_ptr/_bs_into/_bs_into_ptr/_k_into/_k_into_ptr)`, `dequantize_q8_0`, `quantize/dequantize_q4_0`, `quant_error_stats`.

### Cross-platform duplication clusters

| cluster | files+lines | verdict | notes |
|---|---|---|---|
| **(a1) load-time weight repack skeleton** | `math_gen` 526–576, 593–635, 660–707, 843–891, 893–945, 947–1000, 1002–1045 | **copy-paste ×7** | One skeleton: stride calc → resize 2 scratch arrays → byte-by-byte copy of whole tensor → 3-deep gather loop; only the innermost gather (~10–25 ln) differs. ~340 lines where a parameterized walker + 7 gather lambdas would be ~150. Plus 14 near-identical `_gen`/`_bake` 2-line twins. |
| **(a2) stamped-vs-bake selector pairs** | `math_gen` 581–592, 639–646, 711–718, 1158–1184; `math.das` 2762–2810 | copy-paste ×4 both sides | Reason real (lane contexts can't read math's globals — div0 comment 527–531) but the shape is one generic. |
| **(a3) disk-order nibble extractors** | `math_gen` 818–838 (`k45_nib`, `k5_hbit`, `k6_nib`) vs `layout.das` 677–701 (`kq_disk_nib45`, `kq_disk_hbit5`, `kq_disk_nib6`) | **byte-for-byte copy, renamed** | Both `def private` so neither sees the other. **Cleanest single `dasllama_repack.das` candidate.** |
| **(a4) per-dispatch repack/expand** | `math.das` 2491–2570 (`expand_q51(_grp)_row`, `gather_q51_grp_scales`); `math_gen` 1051–1102 (`unpack_kq_panel_grp`) | same-algo | Runtime tier of the same family. |
| **(a5) GPU repack** | `metal_repack_noop` (metal:433), `vulkan_repack_noop` (vulkan:9206); real gathers in `layout.das` 543–836 + `convert_model_to_metal_blob` 107–198 | two distinct tiers | **Hypothesis partially refuted:** repack is not one shared op across platforms — CPU grp<mr> interleave vs GPU device-plane gather share only the nibble extractors (a3) + `gemm_schema` strides. Within each tier duplication is severe (7 copies CPU, 3 gathers GPU); across tiers it is thin. |
| **(b1) Q8_0 activation quantize (CPU)** | `quant.das` 79–105, 122–156, 172–204 | copy-paste ×3 | Identical two-lane float4 amax reduction verbatim (82–96 ≡ 125–137 ≡ 175–187); `_bs` adds per-16 sums, `_k` widens to 256. Comment admits it (178). |
| **(b2) Q8_0 requant (vulkan intra-file)** | 1134–1164 vs 1805–1834 vs inlined 2196–2224 | ~75–86% identical ×3 | Delta = `act_mul` vs plain read. |
| **(b3) Q8_K requant (vulkan intra-file)** | 1169–1205 vs 2880–2914 | ~86% identical | Comment says "mirrors … minus the act". |
| **(b4) f32↔f16 conversion** | `math.das` 106–131; `accelerate` 75–151; vulkan 1106–1233 (`q8_moe_xf16` vs `q8_moe_actf16` vs `rd_f16cvt` — 2-copy pair inside vulkan too) | same-algo, divergent tiers | CPU intrinsics vs accelerate clamp+plane-cache vs in-shader. |
| **(c) flash attention prefill (vulkan intra-file)** | `at_attn` 1931–2064 vs `da_attn_b` 2500–2637 | **~76% identical (64 diff ln / 272)** | Entire flash core byte-identical; deltas = meta unpack + naming + guards. **Strongest single consolidation target in the file.** |
| **(d) kq batch tiles (vulkan intra-file)** | 3160–3277 (k4), 3281–3393 (q40), 3398–3520 (k5), 3526–3658 (k6) | **~91% identical ×4** | Verbatim 40-line preamble + activation stage + 4×sdot4 block + 20-line store; deltas = nibble compose (~15 ln) + scale fold (~8 ln). ~490 lines where ~180 would do. |
| **(e) kq decode GEMVs (vulkan intra-file)** | 2985–3023, 3028–3064, 3068–3106, 3112–3150 (+ `q8_moe_groupn` 75–113, `q8n_moe_groupn` 1061–1102) | **~87–95% identical — 6 copies** | Subgroup-per-row region-list GEMV shell. |
| **(f) region-list meta unpack (vulkan)** | 12 copies: 131–142, 240–252, 343–355, 411–425, 619–631, 764–777, 881–892, 1001–1011, 3163–3172, 3284–3293, 3401–3410, 3529–3538 | copy-paste ×12 | 10–14 lines each; tile edge E varies. |
| **(g) subgroup partial-sum reduce (vulkan)** | 5 copies: 2092–2107, 2136–2151, 2180–2195, 2247–2261, 2338–2352 | copy-paste ×5 | Verbatim in the ar_* / qk_rms / qkn_rope_kv family. |
| **(h) Q8·Q8 GEMV/GEMM core, cross-backend** | default, neon, gen(+gemm_gen IR), metal, vulkan | **same algorithm, different intrinsics — NOT consolidatable** | Five implementations: portable scalar/int4 → NEON sdot/tbl → LLVM-IR (sdot/maddubs/vpdpbusd/smmla/amx) → Metal simdgroup → Vulkan sdot4/coopmat. This divergence is the product. |
| **(i) K-quant dequant-dot formula, cross-backend** | default 394–517; math_gen 158–292 + 1107–1157; vulkan 2934–3658; metal_prefill 373–3738 (6 kernel families) | same-algo, **~30 independent implementations of ~4 formulas** | `w = d·sc·q − dmin·mn`; `d·(q−8)`; k5 5th-bit deposit; k6 signed per-16 sub-scale −32. Strides shared via `gemm_schema`; the FORMULA is not. Largest single duplication surface in the module. |
| **(j) activation/gated-FFN elementwise** | `math.das` 3822–4135 (8 scalar + 6 four-wide); vulkan `act_mul` 1127–1129; metal_prefill 5 kernels | same-algo, three unrelated factorings | |
| **(k) hazard/barrier rail** | vulkan 6147–6395 vs `metal_common` bar_dep/kn_* layer | declared port, same-algo | Vulkan has THREE region-bit namespaces (`VHZ_*`/`VHB_*`/`VHG_*`, 40 constants) for arguably one. |
| **(l) lazy device-state `ensure_*` (vulkan)** | 18 copies (4795, 5321, 5348, 5418, 5497, 5524, 6096, 7108, 7362, 7389, 7424, 7457, 7485, 7853, 8080, 8309, 8900, 9065) | copy-paste ×18 | `if (ready) return` → make bufs → alloc desc set → writes → `hz_set_bits` → ready. The 6×`write_buf_desc` boilerplate appears 19×; `rd_set6` (5718–5731) is the one abstraction, other 18 predate it. |

### math_vulkan vs metal overlap

| bucket | ~lines | ~% |
|---|---|---|
| Functional mirror of metal (same job, different API/intrinsics) | ~5,100 | ~55% |
| Vulkan-only capability (coopmat/cm2, ReBAR/host-import, dry-bake, arena, streamed-mirror) | ~1,900 | ~20% |
| Vulkan API mechanics with no metal counterpart (descriptors, cmd buffers, barriers, handles) | ~2,300 | ~25% |
| **Textual copy-paste with any metal file** | **~0** | **0%** |

Biggest mirrored blocks: resident driver 5623–7040 ↔ whole `metal_llama.das`; kq batch tiles 3152–3659 ↔ `metal_prefill` MulMm families; attention 1836–2064+2384–2828 ↔ metal SqAttn (metal carries 4 KV codecs × 5 tiers, vulkan f32 only); deltanet 1261–1834 ↔ metal 7-kernel decomposition (vulkan fuses decode step into one kernel); coopmat tiles ↔ simdgroup tiles; rope+KV store ↔ metal ×5 codecs.
Vulkan lacks: mx4/q51 kernels, tq4/q8 KV codecs, Argmax/EmbedQ8/EmbedK6, MoE routing kernels (does routing host-side), PLE, Softcap/Suppress, FwhtUnsign, bf16 GEMM, MTP. Metal lacks: cm2 decode-in-load, aligned-L guard-free pipeline, fused `ar_add_rms_rq`/`qkn_rope_kv`, streamed-mirror + host-pointer import, offline dry-bake, per-dispatch GPU profiler, heat-cache hooks (registered only by vulkan, `math.das:1783`/`vulkan:9335`).

### Anomalies

1. **Dead PSOs:** `metal_prefill` 4922–4923/5320–5321 compiles `g_pso_gemm`/`g_pso_gemm64` from `math_metal`'s MSL and never dispatches them — the only thing `metal_prefill` takes from `math_metal`, so that require currently buys two dead pipeline compiles.
2. **`GpuState.stream_reserve` (vulkan 4108) declared, read at 8916, never written** — ternary always takes legacy 1.1 GB constant; the `ensure_stream_slots` assert validates against the wrong number vs `carved_budget()` (GLM-Air-class geometries affected).
3. `batch_y2` (vulkan 4080) — 64 MB allocated every batch-tier arm, self-documented "idle since the FFN fold".
4. Stale comment `math_metal:213–215` — `gemm_use64` "shared with the resident-prefill driver" is false (only `metal_q8q8_batch` + one test call it).
5. Vulkan file header (23–49) undersells file by half — never mentions the 1,600-line resident driver + arena.
6. `math_vulkan:17` comment names `DlimConfiguration`; actual type used is `DlimVulkanConfig`.
7. `math_gen` `[tune_scope]` covers-set omits `dasllama_math_gen` itself (10 `[tune_perm]` stubs).
8. **Two incompatible format-ID spaces:** `gemm_schema` uses 4/5/6/40 (k4/k5/k6/q40); vulkan uses `int(KqFmt)` = 0/1/2/3/4/6 — `4` and `6` mean different formats in each; bridge fn `vk_kq_schema_id` (5215–5222) panics out-of-range.
9. `arena_block_bytes` (5231–5240) re-derives q8/q8n strides locally despite the "strides live in one source" comment at 4773.
10. **~1,030 lines of GPU-tier hook surface live in the CPU math hub** (`math.das` 1390–2424) — structurally an interface header, placed there only because math.das is the one module both sides already require. Largest single non-math region of the largest-fan-in file.
11. `math_vulkan` and `math_gen` disable size lints wholesale (`options _cyclomatic_complexity = 0` / `_function_length = 0`) — exactly where duplication is densest.
12. All 7 `repack_*_grp` copy the whole tensor with a **byte-at-a-time loop** (dominant load-time repack cost, duplicated 7×); vulkan has `par_memcpy` (7524–7542) for exactly this shape.
13. Stale comment `dasllama_common:17` — "stage-0 skeleton, kernels land with the offload rail" describes the 9,339-line Vulkan tier.
14. `asr_requant_rows_q8` (`audio.das:807`) is a near-verbatim copy of common's private `requant_rows_q8` (6749) — copied because the original is private; the copy lost the par-threshold gate + profiling. Sibling wrappers `tw_repack`/`pk_repack`/`wm_repack` (audio:885/parakeet:270/whisper:203) + their quantize drivers are the same 8-line recipe copy-adapted ×3 over shared `repack_q8q8_weight`.

### Refinements (final agent pass)

**Hypothesis verdicts.** (a) repack "mostly the same across platforms" — *refined, not confirmed*: near-identical WITHIN the CPU rail (7 copy-paste `_grp` fns + 8 wrapper twins + 3 byte-identical extractors ≈ clean ~300-line reduction, zero cross-platform risk); NOT the same across platforms (GPU = gather-into-device-planes, registered as `*_repack_noop`). (b) convert — *strongly confirmed*: **11 implementations of "amax → d=amax/127 → round → pack"** (quant.das ×3 pasted incl. the "LLVM won't auto-vectorize fmax" comment ×3; math.das `quantize_q8kv_row`/`quantize_tq4kv_row`; vulkan ×5 with comments admitting equivalence). Most duplicated formula in the slice.

**Additional clusters:**
- **A5. kq scale-plane decode** — the `[f16 d][f16 dmin][8 sc][8 mn]` 20B decode written 4+ times in 3 byte orders: `math_gen` (inside repack_k4/k5), `math_default` (`k4_sc_mn` 379–392 + `dequant_kq_row_grp`), `layout.das` (`compact_kq_scale_strip` 186 + gather_kq), vulkan in-shader ×4. `KQ_DEV_SSB` names the stride; nothing names the *decode*.
- **B3 f32↔f16 = best-behaved cluster** — CPU genuinely funnels through `f16_cvt`; only accelerate's bf16→f16 hand-rolls (different source format, justified).
- **B4** `quantize_q4_0` vs `quantize_q4_0_into` (quant.das 233–263 vs 268–293) — 90%+ copy-paste pair inside one file.
- **C6 rope** — the NORM-vs-NEOX pair selection (`e0 = neox ? j : j*2`) written out ≥14 times across math.das 7 variants + vulkan 3 + metal 10.
- **C9** vulkan `q8_moe_batch_mm` (410–608) vs `_mm_a` (618–753): 136-line deliberate pair (guarded path costs ~20%) — legitimate reason, wrong mechanism; this is exactly what the `[tune_perm]` generator rail does for CPU kernels.

**Additional anomalies:**
- **Dependency inversion:** `dasllama_quant.das` requires the 4,330-line `dasllama_math` hub ONLY for `maybe_parallel_for`'s counters (`count_inline_run`/`count_parallel_dispatch_thru`, own comment L9) — those counters belong in `dasllama_par`.
- **`dasllama_math.das` is two modules wearing one name:** kernel primitives (29–478, 3536–4330) vs pure GPU-tier/backend registry (1192–2424, ≈1,230 lines, 28%) — the registry half is why 112 files require it.
- The kq format-ID bridge exists on BOTH sides: `vk_kq_schema_id` (vulkan 5215–5222) and `kq_schema_id` (`layout.das` 22–29). `6` = q8n in KqFmt-space but Q6_K in schema-space — live footgun.
- 26–52-line architectural preambles at `math_gen:23–48`, `gemm_gen:1–52`, `math_vulkan:23–49` violate comment-hygiene rule 2; content belongs in the design doc.
- `math_accelerate` registers no KernelBackend (installs float-batch override) — `math_<backend>.das` naming misleading; `g_wf32_scratch` is live, not dead.
- Repack scratch: 7× full-tensor alloc+free per call, fanned across jobque lanes by `repack_regions` (layout:215) → peak transient = lanes × 2 × tensor_bytes; no `@scratch`, no reuse.

**Dedup-MCP targets (highest S/N):** `dasllama_math_gen.das` (A1/A2 in-file) → `dasllama_math_vulkan.das` (C1–C4 + B1 requants in-file) → `math_gen`+`layout` pair (A3 exact cross-file) → `dasllama_quant.das` (B1/B4 in-file). `math_metal`/`math_accelerate` low yield.

### Require-graph in-degree (whole module, 2026-07-29)

112 math · 53 common · 30 transformer · 29 quant · 27 env · 24 math_default · 23 dasllama · 22 gguf · 21 audio · 18 par · 17 image · 15 math_gen · 14 tokenizer · 13 metal_prefill · 12 math_aarch64_neon · 9 gemm_schema · 8 tune/metal_llama/asr · 7 math_metal/layout/config · 6 whisper/vad · 5 bpe · 4 math_accelerate · 3 metal_kernels/metal_common/math_vulkan · 2 unicode/prefix/parity/parakeet/metal_shapes/metal_lens/kernel_access/audio_io · 1 vulkan_lens/qwen3a/gemma4a/gemm_gen/chat/canary + each arch_* (exactly 1 = the dispatcher) · 0 gemm_register (optional-require false positive).

## GPU dispatch layer (metal_* + lenses + kernel_access)

### Per-file census

**`dasllama_metal_common.das` — 1,888, `shared public`.** Carries `options _metal_manual_dispatch = true` (L3) — the only permanent census opt-out (this module IS the capture rail). Re-exports `dasllama_metal_shapes public`.
25–117 driver state (`MetalDecodeStepTrace` 75, `KVMirror` 94, arena globals) · 119–231 **all decode PSO globals (~110 `g_pso_*`)** — deliberately here so `[metal_dispatch]`-generated builders can name their pipeline · 233–269 arena allocator (16B-aligned free-list w/ coalescing) · 271–348 decline machinery (`decline`, `batch_decline`, `require_or_panic`, stats) · 350–450 spec-chain state, skip rail, `metal_decode_trace_arm/take` · 452–536 `declines_line`, stage report, `mirror_release`, `upload_region(_cat3)` · 538–599 whole-plane resident buffers (`plane_buffer`, `blob_of`, `bf16_of`, `mx4_of`) · 602–640 zero-copy logits (bytesNoCopy) · 642–709 kq/q51 plane binds + uniforms · 715–947 KV-mirror arena (`mirror_prepare` 849 = the watermark contract, `arena_rebuild`, `mirror_defrag`) · 951–1078 DeltaNet state mirrors (`DnMirror`, `signs_buffer`) · 1085–1188 **the range tracker** (hazard oracle: `MemRange`, `hz_read`/`hz_write` ×4, `hz_gate`, `hz_arm`) · 1190–1292 **the step graph** (capture rail: `KNode`, `KBind`, `kn_*`, `gr_arm`, `graph_flush`) · 1294–1458 **the load-time schedule** (`KSched`, `gr_shape_hash`, `gr_schedule` ASAP leveling, `graph_flush_sched`, `hz_coverage_check`) · 1461–1594 pending-step encode-ahead (~24 `g_lp_*`, `finish_pending_step`, `metal_decode_flush`) · 1597–1700 init + weights-epoch guard + release · 1702–1758 `MetalPrefillDecline` + tables (moved here in W1 split to break cycle) · 1760–1888 Metal-4 tensor-race scaffolding.

**`dasllama_metal_kernels.das` — 7,281, `shared public`.** 56 kernel classes: 24 lensed (`[metal_dispatch]`), 32 hand-dispatched. 318 `@ssbo` / 236 `@role` / 3 `@span`.
24–2552 **single-stream decode set, 100% lensed**: single-query attention × 4 KV codecs (F16 29, F32 201, Q8 369, Tq4 553) + split-K part twins (730–1312) + comb (1507); fused QKV+rope GEMV (1554/1676); W13+swiglu (1795); CopyRow/Argmax/EmbedQ8/EmbedK6 (1846–1945); AddRms/PreAddRms (1989/2042); rope-store F16/HF16/F32/Q8/Tq4 (2119–2417); FwhtUnsign (2525) · 2553–5847 **batched-step P4 set, 0% lensed**: fixed-B GEMV/mv/w13sw B2+B4 (2560–2955); batch GEMM + split-K + Metal-4 tensor twins (2956–3328); batch attention B/PartB/CombB/D/CombD × 4 codecs (3329–5184+); batched rope-store (5185–5688); AddRmsB (5799) · 5849–6019 PSO lifecycle (`metal_decode_init` 5873, ~135 compile_pso calls incl. 3 `metal_tensor_crowned` forks) · 6025–6223 single-stream encoders · 6224–6417 MoE routing encoders (Wave C) · 6418–6979 batched-step encoders (561 lines hand-written `kn_*`) · 6982–7161 `metal_kernels_release` (76 `release_pso` + 26 open-coded blocks) · 7164–7281 decode tensor races.

**`dasllama_metal_prefill.das` — 8,240, `shared public`.** 78 kernel classes, 17 lensed. 328 `@ssbo` / 250 `@role`.
37–368 RmsNorm + mul_mm family (Q8/Bf16 + `T` tensor twins) + PLE gather/finish · 369–876 K-quant mul_mm twins (K4/K4T/K5/K5T/K6T/K6) · 877–1924 MoE routing Wave C (Router/RouterB/Select/Gemv × q8/k4/k5/k6/mx4/q51, Combine, SwigluOai, G4RouterNorm, MoeWScale) · 1925–2703 MoE prefill CSR + gathered mul_mm (Count/Bucket/Reduce/MulMm × formats) · 2704–3025 K-quant site GEMVs · 3026–3485 kq small-batch mv twins B=2..4 · 3486–3738 kq B=5..8 single-pass twins · 3739–4540 Rope/AddBiasRows/QkNorm/AttnQK/Softmax/AV(+MmT/Mm)/Swiglu/Geglu/Add/PfCopy/PfCat2/SuppressRow/SoftcapRow · 4541–4888 DeltaNet Wave D (9 classes, 6 lensed) · 4889–5291 module state (~90 `g_pf_*` + `PF_NEEDS_OK`) + `metal_prefill_shutdown` · 5293–5550 `pf_compile_pso`, `metal_prefill_init`, pf-uniforms, `prefill_decline` (5488) · 5553–6260 prefill + MoE + deltanet encoders · 6261–6295 weights-epoch guard + PLE gate · **6296–7526 `metal_prefill_forward` — 1,231 lines in one function** (~150-line hand-unwound `pool_release` tail at 7370–7514) · 7528–7553 `[init]` registers "metal" prefill override + PLE gate · 7555–8240 prefill tensor races (7 families, `metal_tensor_race` 8214).

**`dasllama_metal_llama.das` — 3,252, `shared public`.** Pure driver, no kernel classes.
46–137 shutdown, `decode_decline`, `spec_cls_capable`, site encoders · 142–1102 per-step resource bundle + encode-ahead (`StepRes` 147–300 with ~150 fields; `acquire_step` 312; `encode_layer` 546–962; `encode_cls_tail`; draft-step machinery; `metal_mtp_draft_forward` 1065) · 1103–1555 MTP B=2 same-slab verify (`encode_verify_layer` 1142–1324, `metal_mtp_spec_eval` 1427) · 1556–2033 single-stream decode driver (`encode_step` 1592, `finish_step` 1648, `release_step` 1710 hand-releasing ~150 fields, `pre_encode_next` 1889, `metal_decode_forward` 1963) · **2034–3203 batched-decode driver (`metal_batch_decode_forward` 2154–3198, ~1,045 lines in one function)** · 3205–3252 `[init]` registers decode/batch_decode/mtp_spec/mtp_seam overrides under "metal".

**`dasllama_metal_lens.das` — 580, `shared private`.** The `[metal_dispatch]` structure-macro: validates name/pso/tg/grid micro-grammar (261–296), collects `@ssbo`/`@uniform` in `@binding` order (298–331), **derives read/write axis from kernel body** via `classify_field_access` + cross-checks declared `@role` (333–417; `weight`/`alias` stay declared; `access_force=true` escape has ZERO in-tree uses), emits pipeline→tgmem→binds→hz_reads→hz_writes→hz_gate→kn_dispatch (419–442). Plus 491–580 the manual-dispatch census `[lint_macro]`: bans direct `metal_dispatch_threadgroups` in `dasllama*metal*` modules; cross-checks `@role` on un-lensed classes.

**`dasllama_metal_shapes.das` — 361, `shared public`.** Requires only `dasllama_common` + `dasllama_math` — NO `das_metal` (so a non-Apple box can bake an M1 `.dlim`). `MetalDecodeDecline` (22 values, 4 RETIRED), `MetalNeed` bitfield, `metal_needs`/`record_needs`, kq-fmt GPU-support predicates, `kv_share_ok`, `dn_metal_ok`, `moe_site_ok`/`moe_metal_ok`, `decode_shape_decline` (296–343), `[init]` → `register_metal_servable`. Consumed by `metal_common` (public re-export) + `dasllama_transformer:24` — **unconditionally** (no `?das_metal` guard; deliberate portability, means the enums compile into every non-Apple build).

**`dasllama_vulkan_lens.das` — 139, `shared private`.** `[vk_access]` inert marker + `[call_macro] vk_kernel_access_spec`: scans `@ssbo @binding` globals, walks every `[compute_shader]` fn with `classify_global_access`, emits `"kernel=rmask,wmask;…"` const string consumed at `math_vulkan:6225` (`ensure_kernel_access`). Single consumer.

**`dasllama_kernel_access.das` — 371, `shared public` — the ONE genuinely shared Metal↔Vulkan component.** `AccessResult`, `AccessVisitor` (models coopmat/simdgroup/tmm2d/tg_store builtins; strictness ratchet at 247–264: tracked buffer passed whole → err), `classify_access` (same-module callee recursion), `classify_global_access` (vulkan) / `classify_field_access` (metal).

**`modules/dasVulkan` (untracked).** Vulkan bindings generated from Khronos `vk.xml` over volk, own generator in `generator/`. Layers: `vulkan` (raw API, C++ module) + `vulkan_boost` (RAII handles, struct views, auto `sType`, named args). `daslib/` 17 files ~1.7 MB (`vulkan_structs.das` 832 KB, `vulkan_ctors.das` 326 KB generated); `spirv_vulkan_shader.das` = the compute-shader emitter dasLLAMA uses. Vendors volk + Vulkan-Headers @ sdk-1.4.350.0; builds without Vulkan SDK, runs on any loader/driver (MoltenVK on macOS). dasLLAMA consumption: exactly 5 require sites, all `?vulkan`-guarded or perf-scratch (`math_vulkan:14–15`; `common:17`; 2 tier tests + `test_gpu_slot_swap` gated on `builtin_module_exists`; `performance/coopmat_mulmm_{reference,port}.das`); `deploy-jit.ps1:42` lists it in the deploy set; NO `utils/` .das requires it.

### Metal↔Vulkan parity map (dispatch/lens/shapes layer)

| Capability | Metal | Vulkan | Verdict |
|---|---|---|---|
| Body-walk read/write classifier | `classify_field_access` | `classify_global_access` | **SHARED** — one impl (`kernel_access.das`) |
| Lens output | generates whole `enc_*` builder as AST | emits spec STRING, runtime re-parses | structurally different |
| Escape hatch | `access_force=true` (0 uses) | `[vk_access(...)]` (0 uses) | mirrored, unexercised |
| Hazard granularity | exact byte ranges (`MemRange`, `@span`) | region BITS (3 namespaces × 6 bindings) | metal finer; vulkan coarser/cheaper |
| Per-dispatch gate | `hz_gate` | `vhz_dep` (6353) | mirrored incl. `_PARANOID` env twin |
| Strict/coverage rail | `g_hz_strict` panic + `hz_coverage_check` | panic in `hz_masks` + `DASLLAMA_VK_HAZARD_TRACE` | mirrored |
| Manual-dispatch census (compile-time) | `[lint_macro]` census | **none** — runtime panic only | **absent on vulkan** |
| Capture/replay | `KNode`/`KSched` ASAP schedule | once-per-epoch cmd-buffer recording (`rd_record_token`) | different mechanism, same goal |
| KV mirror + watermark | `mirror_prepare` w/ LRU + defrag | pinned mirror / streamed walk | different — no watermark contract on vulkan |
| Host-pointer import | none (bytesNoCopy page wrap) | `VkMemoryHostPointerPropertiesEXT` rail | vulkan-only |
| **Portable model-shape gate module** | `dasllama_metal_shapes.das` (decline enum, needs bitfield, servable predicate) | **NOTHING** (0 hits) | **ABSENT on vulkan — biggest asymmetry** |
| MTP/speculative decode | full rail (`metal_mtp_spec_eval`, DnMirror dual-store) | 0 hits | metal-only |
| Per-dispatch GPU profiler | stage stats only | `DASLLAMA_GPU_PROF` + `pfq_ts` | vulkan-only |
| Tensor-op tune races | `metal_tensor_race(_decode)` 7 families | env-selected (`DASLLAMA_COOPMAT`), not raced | metal-only |

### Repack/convert sightings

**Repack: nothing to extract from this slice** — every "repack" here is a *gate refusing repacked input* (`backend_repack` declines: shapes:24/308–309, prefill:5516–5518, common:1718; `t.mx4_repacked` gate shapes:209 "CPU laneq repack has no GPU carry"). True in-slice rearrangements are not tensor repack: PLE host transpose (prefill 6479–6487), `MetalDnDeint` (4771–4794, deinterleaves packed `[q|gate]`), in-kernel `@workgroup` staging transposes. NOTE: the `*T` kernel-name suffix = Metal-4 *tensor* twin, NOT transpose — naming collision.

**Convert: real dispersed cluster, all GPU-side, contractually bit-matched to named CPU oracles:**
| lines | what | CPU oracle |
|---|---|---|
| kernels 2119–2306 (RopeStore F16/HF16/F32) | f32→f16 with explicit ±65504 clamp (6 sites) | `cvt_f32_to_f16` |
| kernels 2307–2416 (RopeStoreQ8) | per-32-block f32→block_q8_0 | `quantize_q8kv_row` |
| kernels 2417–2524 (RopeStoreTq4) | f32→rotated 4-bit FWHT | `fwht_signs_row`/`quantize_tq4kv_row` "bit-for-bit" |
| kernels 1665–1670 | f16 clamp fused in QKV epilogue | `cvt_f32_to_f16` |
| kernels 1921–1988 (EmbedQ8/EmbedK6) | Q8/Q6_K row dequant for spec embed gather | `dequant_k6_plane_superblock` exact |
| kernels 369–725 + batch twins | inline dequant on attention read path | — |
| kernels 2956–3328, prefill 87–368 | f32→f16 workgroup staging (~40 sites) | — |
| **kernels 5940–5947 ≡ prefill 5417–5424** | MXFP4 nibble→float value table, **byte-identical duplicate** | — |

### Anomalies

**A. Orphaned/stale comments from the W1 split + shapes extraction** (doc detached from code):
- common:499–501 docstring for `metal_decode_shutdown` (lives at llama:49 where the same 3 lines appear verbatim) sits above `upload_region`.
- common:711–713 describes `decode_shape_decline` (shapes:296) above `mirror_evict_to_cap`.
- common:1075–1076 describes `MetalRopeStoreTq4` (kernels:2417) above `moe_fmt_at`.
- common:1080–1084 — **comment split across two files**: its other half is shapes:179 above `moe_site_ok`.
- kernels:6021–6024 describes `upload_region` (common:504); near-identical text also at prefill:5434–5440.
- kernels:6171–6172 describes `signs_buffer` (common:1057–1073).
- llama:57–59 dangling continuation (head sentence left with `decode_shape_decline`); also names "attention depth" which shapes:34–35 marks RETIRED.
- llama:139–141 describes `mirror_evict_to_cap` (common:715).
- prefill:6069 swiglu/add comment belongs to `pf_enc_ew2` at :6228 (duplicated from kernels:6404).

**B. kernels:6596–6598 stale mechanism description** — "each scratch buffer owns a bit" tracker is gone; common:1085 documents the current exact-range system as "since P3 the batch rail's ONLY hazard system".

**C. Dead code:** `race_time_ms` (common:1871–1888, 0 callers — superseded by interleaved `race_pair_ms` per the A/B lesson at 1837–1838); `g_pso_gemm`/`g_pso_gemm64` (prefill, compiled+released, never dispatched); `g_max_npos` (prefill:4911, var with no setter, one read, path only reachable for planar test fixtures); `vk_dump_kernel_access` (math_vulkan:6236, 0 callers — vulkan lens's only debug rail unreachable).

**D. Duplicated dispatch paths between metal files:**
- **Whole DeltaNet family has TWO dispatchers and TWO PSO sets**: lensed `enc_dn_*`/`g_pso_dn_*` (decode side only) vs hand-written `pf_enc_dn_*`/`g_pf_pso_dn_*` (prefill only). Same seven kernels, same MSL, two pipelines each; `pf_enc_dn_conv` ≡ generated `enc_dn_conv` line-for-line.
- `enc_rms` ×4 geometries over one MSL, two PSOs (1024/1, 1024/nrows, 256/nrows, 256/1) — undocumented why.
- Systematic `pf_` near-twins differing only in device/pool globals: `compile_pso`↔`pf_compile_pso`, `upload_region`↔`pf_upload_region`, `uniform_u32/f32`↔`pf_uniform_*`, `enc_ew2`↔`pf_enc_ew2`, `enc_gemm_mm_b`↔`enc_gemm_mm`. `g_one`/`g_zero` init blocks and mx4 value tables byte-identical ×2.
- `empty(a) ? KqFmt.q8 : a[l]` exists 4× under 3 names: `moe_fmt_at` (common:1078 AND shapes:196 — same name, two modules), `kq_fmt_at` (llama:91), `pf_fmt_at` (prefill:5573).

**E. Lens coverage is a clean split, not a gradient:** kernels single-stream 24/24 lensed, batched P4 0/32 (boundary = the P4 divider); prefill 17/78. Naming diverges: kernels classes emit `enc_*_c` + thin wrapper, prefill classes emit bare `enc_*` (except lone `enc_gemv_c`). `@span` used 6× of 646 `@ssbo` fields — the "finer than lcpp" range tracker runs whole-buffer almost everywhere. `access_force` 0 uses.

**F. Two >1,000-line functions:** `metal_prefill_forward` (6296–7526) and `metal_batch_decode_forward` (2154–3198). `StepRes` ~150 fields hand-released in `release_step`.

**G. Three PSO-release styles in two functions** (76 `release_pso` + 26 open-coded in `metal_kernels_release`; array-literal loop + 21 `= null` in `metal_prefill_shutdown`).

**H.** prefill:5485–5488 annotation split by comments mis-parses in `outline` (reports the comment as the declaration).

**I. Registration asymmetry:** transformer:24 requires `metal_shapes` unconditionally (deliberate — portable bake) vs :25/:26 guarded. Confirm intentional.

## Model families & text plumbing (34 files, 14,749 lines)

13 `arch_*` = 450 lines; the two `*a` audio towers = 1,270; core plumbing = 13,029.

### The 13 `dasllama_arch_*.das` — declarative registration ONLY

Every file has the identical 3-part shape: `configure_<x>(var c : Config)` flipping Config bools → `[init] register_arch_<x>()` building an `ArchDesc` (blocks = one of 5 shared sets, chat parts, stop tokens) → `register_arch(name, d)`. **Zero forward/kernel/loader code in any of them.** One require each (`dasllama_common`). **18 arch names ship from 13 files** (aliases: qwen2vl, qwen3vl, qwen35moe+qwen3next, qwen3vlmoe). Sizes 23–49 lines. `llama` and `mistral3` have empty configures (`pass`). `gptoss.das` registers `"gpt-oss"` — the one module-name/arch-string mismatch.

### `dasllama_gemma4a.das` (786) / `dasllama_qwen3a.das` (484) — why outside `arch_`

They are **audio-encoder tower loaders + forwards**, not decoder architectures — `a` = audio. Neither calls `register_arch`; both call `register_image_family_tag`. Consumer: `dasllama_asr.das` only (1 require each). qwen3a: `Qwen3aTower` wrapping shared `AudioTower core`, own log-mel flavor (L261–345), `conv2d_s2` (366–394), encode via shared `encoder_blocks` (453). gemma4a: fully self-contained 12-block Conformer — own HTK filterbank (223–254, a SECOND filterbank beside audio's Slaney), per-op activation-quant `Clamp` calibration, own mel/conv/ffn/encode; shares only leaf row kernels.

### Core plumbing census

| file | lines | role | consumers |
|---|---|---|---|
| `dasllama_transformer.das` | 26 | pure umbrella: `require dasllama_common public` + 16 side-effect requires (layout hooks, 13 arch files, metal_shapes unconditional, 2× `?das_metal` overrides — cycle-break documented L25) | 32 |
| `dasllama.das` | 307 | public facade: 40 defs, all 1-line delegations to `_`-suffixed engine twins (verified: zero dead wrappers). `dasllama_audio_io` deliberately ABSENT from umbrella | 23 |
| `dasllama_layout.das` | 844 | repack hook registry → mounted into common at `[init]` (11 fn pointers). Sections: metal-blob transform 31–197, CPU repack walkers 199–536, GPU tier gathers 538–835, registration 837–845 | 10 |
| `dasllama_config.das` | 144 | explicit `.dlim` bake config (`DlimCpuConfig`/`DlimVulkanConfig`/`DlimMetalConfig`), 3 registrable sources, `dlim_config_current` = the ONE host-state read, pure formatters. No family logic | 8 |
| `dasllama_gguf.das` | 1,716 | arch-name-BLIND GGUF v2/v3 reader + the complete quant transcode/dequant library (44 public defs). K-quant superblock dequants + ptr twins 535–746; native-plane transcodes + **the fp32→Q5_K ENCODER 844–1046** (only encoder in codebase); stride consts 1136–1148; bulk transcodes 1201–1352; `gguf_read_tensor_f32` 10-type omnibus 1354–1529; direct transcode exits 1531–1716. **56% of file undivided** (last banner L748) | 23 |
| `dasllama_image.das` | 1,206 | `.dlim` rail: IMAGE_VERSION=7, peek/inventory/GC (`register_image_family_tag` 140, `dlim_gc_stale` 200), save 548–864, load 866–1206 (`load_image` 9-gate decline ladder, `load_model_cached` 1085). No family logic — `t.arch` opaque | 18 |
| `dasllama_tokenizer.das` | 269 | SPM tokenizer + 2-backend facade (`load_tokenizer_auto` sniffs `tokenizer.ggml.model`: gpt2/gemma4→BPE else SPM) | 14 |
| `dasllama_bpe.das` | 664 | byte-level BPE + SPM-style BPE; **four pre-tokenizers** (llama3/qwen2/qwen35 parameterized 131–227, gpt4o 250–340, tekken 345–434) | 5 |
| `dasllama_unicode.das` | 248 | GENERATED data (RANGES ~1,100 entries = 58% of file, transcoded from llama.cpp) + UTF-8 codec | 2 |
| `dasllama_chat.das` | 600 | per-model format as DATA: 3 hand templates + `detect_chat_template` 11-arm Jinja sniff (detection only, never executed) + renderer + public API | 1 |
| `dasllama_prefix.das` | 233 | vLLM-style page-granular prefix cache (FNV-1a chained page hash, LRU evict) | 2 |
| `dasllama_audio_io.das` | 39 | ONE function (`load_audio_16k_mono` over miniaudio); deliberately outside the umbrella | 2 (both server) |

### The ASR pile

| file | lines | what |
|---|---|---|
| `dasllama_asr_types.das` | ~45 | the shared ASR floor: `AsrCaps`/`AsrTimestamps`/`TranscribeSegment` + `asr_ctx_guard` — what family files and the facade both see (arms-home move, 2026-08) |
| `dasllama_asr.das` | ~330 | facade ONLY since the arms-home move: sniffing loaders + model/session unions + one-call dispatch arms (if/elif on private `AsrKind` in caps/create_session/transcribe/feed-drain-flush). Every pipeline lives in its family file. Model sniff: vocab<32768→parakeet, 51864–51999→whisper; mmproj tag/projector_type routes |
| `dasllama_audio.das` | 1,565 | **the hub (20+ dependents)**: mel flavors ×2, FFT plan, `AudioTower`, shared row kernels, tower Q8 rail, loader, `EncoderState`, `enc_attention`, `encoder_blocks` (1422–1491 — the ONE shared transformer loop), 3 projector tails |
| `dasllama_whisper.das` | 1,520 | whisper-proper over ggml-*.bin: decoder Q8 rail, cross-KV, self/cross attn, batch decode, greedy driver + whisper.cpp logit-filter suite, window driver, lang table |
| `dasllama_parakeet.das` | 1,641 | Parakeet-TDT: FastConformer + LSTM predictor + TDT decode; frame-major mel; 4 hand-vectorized conv-subsample kernels; 24-block conformer loop |
| `dasllama_canary.das` | 759 | Canary-Qwen audio front end, fp32 by design (no Q8 rail). Consumers: **1** — no test, no harness, no bench |
| `dasllama_vad.das` | 478 | self-contained Silero-VAD v6 (learned-basis STFT → conv1d ×4 → LSTM → sigmoid). Only ASR file with no `.dlim` facade; not required by asr.das — sibling rail via umbrella |

### Family-structure census — the key findings

**LLM side: `arch_` files hold ~0% of forward logic** — a config diff, a block-set choice, a chat template. Mechanism: (1) `Config` flags (~60) branched on by generic kernels; (2) `ArchBlocks` fn pointers — only **5 block sets** exist for 18 arch names (`std`/`moe`/`gemma4`/`qwen35`/`qwen35moe_blocks`, common:13648–13684); (3) the registry (`resolve_arch` / `rebind_arch_blocks` — the latter must NOT re-run configure; E2B sliding_window clobber documented at 2190–2192).

**The GGUF loader is architecture-name-blind** — zero arch branches in gguf.das; `load_gguf_parsed` decides everything via `"{arch}."` metadata keys + tensor-presence sniffs (`ffn_up_shexp`, `attn_sinks`, `nextn`, `rope_freqs`, `attn_v`). Family logic is *distributed by flag*, not *dispatched by name* — a real design property, but an `arch_` file is a poor guide to where its family runs.

**Buried family logic (family → where it actually lives):**
- **qwen35/qwen3next: ~1,500 lines in common** (deltanet decode 7799–8199, prefill 11174–11750, `load_dn_grouped` 2088, geometry accessors) + layout deltanet repack set + metal DnMirror/Wave-D + `dn_metal_ok` + prefix-cache exclusion.
- **gemma4: ~600 lines in common** (per-layer geometry 6837–6941, PLE 9205–9318, MoE decode 9319–9469, prefill router 12330+, ~10 loader sniffs) + layout PLE/dense-shexp + metal touches ×12 + shapes ×5.
- gpt-oss: sinks sniff L4127, biases, swiglu_oai, YaRN + metal_prefill touches ×8.
- glm4moe: dense-lead, `exp_probs_b` sniff, NextN block + layout ×5.
- phi3: LongRoPE synthesis 4174–4215. llama: the defaults ARE llama; only arch-name string literal in common is `resolve_arch(t,"llama")` L1641 (llama2.c path). mistral3: live chat template is NOT in its arch file (`mistral_v7_tekken_template` in chat.das:81–90; arch-file template reachable only when GGUF lacks `tokenizer.chat_template`).

**ASR side: family behavior lives in family files, the facade routes** (the arms-home move; CODEREVIEW §Audio carries the rules). Still no registry/interface — the union carrier + one-line if/elif arms in 4 verbs are the deliberate shape (a fn-pointer `AsrDesc` needs type erasure the union already provides for free); image-tag registration is the only `[init]`; the shared contracts are `dasllama_asr_types` (caps/segment) and the `AudioTower`+`EncoderState`+`encoder_blocks` data shape (whisper + qwen3a use it; parakeet/canary/gemma4a/vad share only leaf kernels).

### Cross-family duplication clusters (dispositions 2026-08-02)

**arch_ files:** ✅ C1 ChatML ×5 + C2 think_suppress ×5-of-6 → `chatml_chat()`; ✅ C3 Gemma turn ×2 → `gemma_chat()` (gemma4's channel format and glm4moe's suppress stay divergent by design) · C4 clone-before-register idiom ×5 — KEPT, it IS the documented move-semantics idiom · C5 audio-marker placement asymmetry — INTENTIONAL, now stated at both qwen3-family sites (qwen2's audio models convert base-arch, qwen3's as vl) · C6 gemma config accretion — DECLINED: each configure_* is that arch's declarative truth; a shared core would hide which arch sets what.

**ASR pile** (Boris ruling 2026-08: together-over-shared, no conformer library — the cn_/pk_ twins STAY twins until a backend port forces the seam):
- **C7 log-mel/STFT ×6 independent copies** — parakeet↔canary = COPY-PASTE (preemph/pad/normalize line-for-line; only DFT engine differs); audio's two divergent by design (different oracles); + 2 filterbank generators (Slaney vs HTK).
- C8 conv subsampling ×3 (parakeet↔canary same topology, divergent impl — f4/threaded vs naive scalar; canary adds length masking parakeet lacks); output-length arithmetic `(l-1)/2+1` ×4.
- **C9 conformer block loop ×2 = COPY-PASTE with systematic bias delta** (canary adds add_bias_rows everywhere, drops Q8).
- C10 attention ×5: whisper self↔cross = COPY-PASTE (54 ln each); parakeet↔canary rel-pos = COPY-PASTE.
- C11 sinusoidal pos table ×2 verbatim (9 lines).
- **C12 greedy argmax decode ×4 copy-pasted loops all inside asr.das** + logsumexp block ×2 + prompt-assembly scaffold as a 5th near-identical block. No CTC, no beam anywhere.
- **C13 bin container readers — densest cluster:** `bytes_str` byte-for-byte ×4 (whisper/parakeet/vad/canary); `*_read` ×3; record structs ×4.
- **C14 prepared-image loader facades ×6** (~30 lines each: .dlim branch → box profile → load_image → panic; env → eager → save → gc), same log wording modulo one noun.
- C15 Q8 GEMM rails ×3 acknowledged in-source ("tw_repack's twin", "parakeet's v1 recipe").
- C16 finalize apply-walkers ×6. C17 image_post_load + meta-tripwire pair ×6. C18 `*_ensure` grow helpers ×5 with 4 different semantics. C19 conv2d im2col core COPY-PASTE ×2 (qwen3a/gemma4a), divergent tails.

**Plumbing:** C20 `q4k_scale_min` ×3 (gguf array + gguf ptr + **math_default `k4_sc_mn` byte-identical**); superblock dequants array+ptr twins (~110 ln); plane dequants re-derive same math a 4th time; Q5_1 split logic ×2 · **C21 `save_image` vs `save_model_image_streaming` ~130 near-identical lines; the streaming twin OMITS the nested-weight-carrier arm — silent plane-drop if a Model ever gains a nested image_map carrier** · C22 `moe_gpu_gather_stack` vs `_q8n` ~50 verbatim lines.

### Repack/convert sightings (this slice)

**Repack:** the whole CPU repack registry (`layout.das:199–536`: `push_repack` chunking, `repack_regions` 14-pointer hoist + 4-way fmt dispatch, collectors per format, `stream_repack_one`) · kq scale-strip compactions (157–197) · GPU gathers 538–835 (un-interleave grp<mr> → row-major; **k-nibble re-pairing + k5 qh bit-transpose + 6-bit scale decode to KQ_DEV_SSB**) · conv1d torch→im2col permute COPY-PASTE ×2 (audio:959–971 ≡ whisper:312–324) · parakeet tap-major conv repack · BN folds ×2 (different formulas! see A40) · whisper cross-K 4×4 blocked transpose + cross-V fan-out · twiddle transpose · feature permutes ×2.

**Convert:** the gguf codec library (above) incl. the fp32→Q5_K encoder · BF16 paths · E8M0 denormal · q8-planar→34B-blob (layout:124–151, f16 clamp ±65504 — clamp also at layout:578/643) · whole-blob Q8 quantize ×2 (audio/whisper) · parakeet direct file→Q8 · `asr_requant_rows_q8` · GGML_GELU_FP16 round-trip + 65536-entry f16 gelu LUT (audio) · canary int64-from-2×u32. **Not present:** fp16 storage in ASR pile, int4 there, canary quantization (fp32 by design).

### Anomalies (A1–A52)

**Behavioral bugs:**
- **A1 gemma4 `system_prompt=false` is a lie** — arch file ships a real system turn (gemma4.das:38) and the renderer takes the system arm (chat.das:247); comment copied from gemma2/3 where it IS true. `caps()` misreports.
- **A2 `hermes_tools` applied to only 3 of 5 ChatML arches** — qwen2moe + qwen35 missing → `set_tools()` silently no-ops on qwen2moe/qwen35/qwen35moe/qwen3next. Reads as omission, not decision.
- **A29 `transcribe_qwen3a` is the only pipeline with no `asr_ctx_guard`** (long clip → opaque cache-overrun panic the guard exists to prevent) **and no `asr_prof_add`** (profile runs silently empty).
- **A30 `AsrCaps.streaming` always false and contract inverted** — whisper is the ONLY backend accepting feed/drain/flush yet reports `streaming=false`; a trusting caller never streams the one model that can.
- **A37 parakeet key mask provably dead** (`n_len_org == n_len` at L780 ⇒ mask never fires) at n_head×n_layer×tt² compares — acknowledged in-comment and left in.
- **A39 parakeet mel NaN-unsafe** (`/valid`, `/valid−1` unguarded) where canary's twin guards — clip ≤1 hop → NaN/Inf.
- **A40 two BN folds, different formulas** — parakeet omits epsilon, canary includes it.
- **A27 `respond_` pushes `strip_think(reply)` to history but returns un-stripped reply** — transcript and return diverge on every thinking reply; unterminated `<think>` → `""`.

**Dead code:** A36 six dead debug rails, some paying hot-loop cost — whisper `whisper_transcribe_run` + `whisper_detokenize` (0 callers), whisper trace (`g_whisper_trace` is `var private` + setter has no caller ⇒ provably unreachable branches), parakeet nan-scan (8 `pk_scan` sites in hot loop), parakeet decode trace, audio witness rail (8 `diff_stage` sites in hottest loop, callers only in plan-doc prose) · A38 `ParakeetToken.p` computed with ~1–8k `exp` per emitted token, never read · A31 `AsrTimestamps.word` never produced · A46 `SV_CHUNK` public, zero external uses.

**Misplaced:** A47 in audio.das — FFT plan's only consumer is parakeet; generic row kernels (`layernorm`, `add_bias_rows`, `mm_blob_b`…) belong in math; `asr_requant_rows_q8` exists only because original is private; whisper mel types reused as parakeet's container (hard-panic on n_fft≠400); projector tails = 3 model families unreachable from asr.das (only chat.das constructs one) · A48 38% of asr.das facade is implementation (4 in-facade pipelines) · A21 203-line Q5_K ENCODER inside the gguf "reader" (sole reason it requires math; belongs in quant) · A22 `load_model_cached` = 122 lines of flavor/backend POLICY in the image byte-format module (+ `guard_interp_gguf_load` JIT guard there too).

**Structural:** A16 `t.mx4_repacked` set by `repack_q8_weights`, NOT by `repack_mx4_stacks` — name contradicts semantics at both write sites + read site · A18 10 stride constants defined twice (gguf:1137–1148 vs gemm_schema — which claims "the ONE stride source"); layout.das uses BOTH in one file · A25 `kq_disk_nib*` self-declared second copy (math_gen mounts conditionally — can silently diverge) · A43 canary blob layout determined by converter's tensor ORDER (name-table lookup; .dlim only valid for same-order bins — uncommented coupling) · A5 alias-clone hazard restated ×4 (registry should own the clone) · A49 `serialize_image_meta` visibility rule enforced only by comment.

**Doc/comment staleness:** A4 mistral3 header claims v7-tekken but live path never uses the file's template · A14 `gguf_read_tensor_f32` doc wrong twice (decodes 8 quant types it claims not to; panic contract misdescribed) · A15 layout.das undercounts declines + hook registrations · A17 3-hop circular plane-layout comment (no authoritative one exists) · A32 asr.das header names 2½ of 6 backends · A33 comment on wrong branch · A34 audio.das:582 "q8 fast path is ledgered" — implemented in same file · A35 `options _function_length = 0` justification copy-pasted from a different file (audio has no transcribe) · A9 unicode GENERATED with no regeneration script named.

**Minor:** A3 gpt-oss name mismatch · A6 two empty configures · A7 audio_io outside umbrella (documented) · A8 image listed among arch files in `.das_module` · A10 `cpt_is_whitespace` linear scan in pre-tokenizer inner loops · A11 SPM merge O(n²·V) (documented reference algo) · A12 both tokenizer backends always materialized (fat-struct pattern ×2) · A13 `parse_special` accepted and ignored in 3 layers · A19 9 of 44 gguf publics exist only for tests/fixtures · A20 `GGUFTensorInfo.dims` = 1,000 small allocs/model, single reader is gguf_dump · A23 misprefixed panic · A24 2 of 7 transcodes unthreaded + skip guard_dst · A26 prefix_evict_lru 4× `[]` on same key · A44 `s.pooled` 4.6 MB allocated for towers that never use it · A45 unused requires (vad `strings`, canary `strings_boost`) · A42 twin mel functions have MIRRORED argument orders · A50 duplicated constants (ggml magic ×4, mel floor ×3 under 3 names, hop=160 ×3, log10_e ×2 same file, sigmoid open-coded ×6…) · A52 divider coverage uneven (gguf 56% undivided, image 45%).

**Positive baseline:** A51 — zero TODO/FIXME/HACK/XXX and zero commented-out code across all 34 files.

## Tools / rig / server / tests

### harness/ — 78 tracked files (manual verification/eval/oracle beds + per-box tuners; not shipped, nothing is a `[test]`)

**`tune_kernels.das` 2,133** — per-box `[tuned]` tuner: 20 perms × **25 kernels** (`TUNED_KERNEL_COUNT = 25`, L110), correctness-gated vs f64 ref, writes tune sidecar. Sections: 37–90 `[dasllama_fallbacks]` + 25 `[dasllama_grid]` structs · 98–124 budgets (ROUNDS=80, REPS=2000) · 126–253 reporting · 260–1907 per-kernel benches (dot 260, binary/axpy 325, **f16 codec 455–690**, scale/softmax/rms 691, dot_q4/q8q8 905, **q8kv codec 1038–1391** incl. `build_q8kv_row` packer 1046, mx4 1392, **quantize exact gates 1474–1630**, rope/gemm 1631, laneq 1796) · 1908–2133 main under `[tune_policy(missing="fallback")]`.

**`gen_tune_probe.das` 1,655** — the `[llvm_code]` grid gate (`DAS_TUNE_MODE=test|tune`). 57–262 q8q8 Fixture + runners · 263–340 mx4 fixture · **341–690 kq companions: `pack_kq_scale_header` 364, `pack_kq_disk_block` 382, `repack_kq_grp_fmt` 421, `build_kq_fixture` 433** (re-implements the production repack rail) · 691–845 q51 family · 847–1173 test/tune families + tolerances · 1193–1363 e2e confirm pass (spawns daslang children, CONFIRM_MARGIN=1.02) · 1364–1655 main.

Rest: 2 tuners' wrappers (`dasllama_tuner.das` 81, `tuner_cli.das` 28), parity oracles (`parity.das/sh`, `pk_gate.sh`, `gptoss_parity_probe`, silero/whisper/canary/qwen2audio py oracles), converters (**`convert_canary_to_ggml.py` 299**, **`convert_silero_vad.py` 82**), one-off probes (~30 files: tq4_* 1026 lines research, wh_*, trace_*.py, smmla, dispatch, flash_stage2, kv_range, mem, k4_kernel, accel_*, bnns_hgemm.c, gemm_1core, q3omni…), emission checks (`gen_*_emission_*`), ref-engine patches ×4, `gen_env_doc.das` 27 (**writes ENVIRONMENT.md**), C++/LLVM oracles under `oracle/`.

### performance/ — 33 files (publishable profiling rig)

**`profile_common.das` 1,800, `shared public`**: 23–100 roots/box/threads · 102–298 LLM catalog · 300–555 platform block (`run_and_stream`, tune_summary/gate, `gather_platform`) · 556–814 hardware block · 815–889 sha identity · 890–1006 baselines TSV + profile JSON · 1007–1406 ASR catalog + reference runners + version capture · 1407–1517 LibriSpeech set + ASR profiles · **1518–1783 public bench records** (`BenchRun`/`BenchModel`/`BenchAnnotation`, read/write/upsert) · 1784–1800 `run_llama_bench`.

Other: `gen_bench_records.das` 623 (the one-rig record driver, das cell as child, upsert-per-cell) · `gen_results.das` 358 (renders `results_llm.md` + GEN:asr blocks) · `gen_site_records.das` 59 · `gen_profile.das` 239 · `establish_baselines.das` 149 · `setup_asr_rig.das` 445 · `coopmat_mulmm_port.das` 561 + `coopmat_mulmm_reference.das` 419 (**"scratch probe" pair, Windows-only hardcoded paths, ~400 shared lines — parked in the publishable dir**) · `BRINGUP.md` 212.

**Generated-vs-hand:** `results_llm.md` GENERATED (marker L3) · `benchmarks/asr/results.md` PARTLY generated (20 GEN:asr fences; prose hand) · `records/{m1,m4,zen2,m3air}.json` (10,684 lines) GENERATED-but-tracked, **no marker** · `annotations.json` hand by design · `profile_*.json`, `baseline_llm_*.tsv` generated · **`baseline_metal_*.tsv` (5) + `results_metal_{3b,gemma,qwen35,qwen3_4b,qwen3moe}.md` (5) HAND-AUTHORED despite "living doc — CURRENT numbers only" banners — no writer exists.**

### benchmarks/ — 59 files, 7 subdirs (`.das_package` releases `lcpp_bench` as `dasllama-bench` exe)

Root: `lcpp_bench.das` 606 (llama-bench mirror, `-o json` records) · `setup_lcpp_ref.das` 169 · `emission_bench` 164 · `decode_prof` 139 · `decode_step_trace` 216 · `batch_parity_probe` 312 · `verify_batch_probe` 212 · `write_cliff_probe` 70.

`matmul/` 13 files: **`bench_metal_gemv_kernels.das` 4,661** (15–86 header verdict log dated M1 Max 2026-07-14..18; 87–3713 = 55 `Metal*Lab*` shader classes across 5 rounds incl. K-quant/ext arms; 3714–4532 driver with `fill_exact_q8/k4/k5/k6` + cpu refs; 4533+ `[benchmark]`) · **`bench_metal_gemm_kernels.das` 4,439** (30 shader classes 30–3501; **3583–3975 = 4 raw inline MSL strings, hand-copies of v22's EMITTED text**; 3976–4047 34B q8-blob builders; driver + `[benchmark]`) · `bench_metal_moe_lab.das` 2,174 · **`matmul_variants.das` 1,712 — self-declared production duplicate** ("winner gets promoted into dasllama_math"; requires NO dasllama modules; `repack_q8q8_4` :458 vs production `repack_q8q8_grp`, `dot_q8q8_laneq4` :501, `dequant_row` :212 vs `dequant_q8_row`, ~30 matmul/dot bodies) · `bench_gemm_smallm` 538 · `bench_gemv_decode` 387 · 7 smaller + `dump_msl.das` 75 + `occupancy_report.das` 59 + `run_gemm_iso.das` 16 (wrapper existing because "dastest compiles the bench away").

`attn/`: `bench_metal_sq_attn` 1,561 · **`attn_gemm_variants.das` 477 — same promote-the-winner pattern, requires only jobque+math** · `bench_attn_gemm` 282.
`asr/` 26 files: `conv_sub_bench` 959 (**"engine-verbatim loop forms"** — copies of engine loops + own `quantize_repacked` :677) · `conv_module_bench` 430 · `cross_kv_bench` 259 · `asr_bench_cell` 237 vs `asr_bench` 127 (two timing drivers, same shape, different output protocol) · py refs + Makefile + `side_by_side.sh` 133 · **`zen2_sweep.sh` 22 — dead: hardcoded laptop path writing a nonexistent tsv against `E:\whisper.cpp`** · results.md 357 · tsv fragments (some 1–2 data rows).
`ew/` + `rope/`: 2 metal microbenches.

### tests/ — 59 files (module-owned; NEVER AOT; `run.das` = the only sanctioned runner for metal suites)

>1,500 lines: `test_vulkan_tier.das` **2,944** · `test_metal_prefill_kernels.das` **1,982** · `test_kquant.das` 1,609 · `test_metal_support_matrix.das` 1,582. Then ~55 more from 1,080 down to 33 (census in agent transcript; notable: `test_facade_docs.das` 121 = facade drift detector; `test_env_registry.das` 232 keeps ENVIRONMENT.md honest; `test_bench_records_schema.das` 115). Earliest bring-up tests (`test_matmul` 33, `test_rmsnorm` 41, `test_rope` 54, `test_softmax` 53, `test_unicode` 45) comment-less, never revisited.

### models/ — the module's only checked-in weights
`silero_vad.bin` (1.2 MB, MIT) + LICENSE — deliberate exception to "models are gitignored" (README:203) so `test_vad` is self-contained. Consumed by `dasllama_vad.das:16,36`, tests, server `POST /vad`, 3 harness probes. Not a model cache.

### utils/dasllama-server/ — 16 files
`openai_server.das` **3,140** (multi-slot 38–347: `ModelSlot`, `add_model`, `set_model`, `bind_gpu_slot`, `/v1/models/activate`; bench button 348–529 spawning child lcpp_bench+llama-bench; bake button 530–731 (.dlim converter child); `/v1/images` 732–773; chat/completions/embeddings/audio/vad/stats/streams/config/gc/shutdown) · `control.html` **1,924** (CSS 9–221, body §01–§11, JS 405–1924) · `main.das` 904 (CLI+TOML precedence, lifecycle, `[tune_policy(missing="restart")]`) · `llm_scheduler.das` 490 (continuous batching, one thread, `scheduler_step` 306, zero HTTP) · tests 808+571+422 · `server_bench.das` 285 · `ask.das` 133 · `demo_load.das` 107 · `wav2txt.das` 106 · `deploy-jit.ps1` 73 · README 305 · CONTROL_PAGE_PLAN.md 256.
**No chat/scheduler duplication vs engine found** — both reach the facade only. One deliberate re-implementation: `utf8_trim_partial_end` + `ref_generate` (scheduler diff oracle).

### Stale factual claims (verified against tree)

**LOC truth (measured):** `dasllama/*.das` = **72,639** · ASR subset (9 files) = **8,042** · Metal (7) = **22,044** · Vulkan (2) = **9,478** · core remainder = **33,075** · server non-test = **7,089**.

| file:line | claim | actual |
|---|---|---|
| control.html:389 | engine "~26,000 lines" | 33,075 core / 72,639 total |
| control.html:391 | speech "~7,000" | 8,042 |
| control.html:391–392 | Metal+Vulkan "~15,000" | **31,522 (2.1× off)** |
| control.html:392 | server+page "~4,000" | 7,089 |
| CONTROL_PAGE_PLAN.md:100 | "honest line counts (~49k engine + ~4k server)" | doubly stale |
| CONTROL_PAGE_PLAN.md:255 | "V3 (if ever): multi-LLM serving" | **SHIPPED** (`ModelSlot`, `/v1/models/activate`, README documents roster) |
| CONTROL_PAGE_PLAN.md:88–98 | "S8 DEFERRED … resume via revert-of-revert" | **SHIPPED** (bench button live: openai_server:348–529, control.html §10) |
| CONTROL_PAGE_PLAN.md:22–52 | "ENGINE CRASH … (2026-07-19, OPEN)" | 10 days, no closure marker — verify or move to tracked issue |
| CONTROL_PAGE_PLAN.md:53–57 | "WATCHDOG GAP … follow-up terminate child" | same class, unverified |
| CONTROL_PAGE_PLAN.md:252 | "V2 GATED on 0.6.4 release" | tree IS 0.6.4 (CMakeLists:2) — gate ambiguous |
| server README:3 | "CPU inference" | GPU first-class in same README (`gpu = metal\|vulkan`, per-slot backend) |
| server README:123 | "38 kernel families" | no arithmetic gives 38 (25 `[tuned]` + 8 `[llvm_code]` grids + 18 `[tuned]` decls) |
| server README:41 | "--mtp ~2× decode" | no date/box/record pointer |
| server README:302–304 | "Not yet implemented" list | neighbors already went stale; re-verify |
| tune_kernels.das:3–6 | "every `[tuned]` kernel (…enumeration…)" | omits f16 codec set, q8kv set, dot_mx4q8 — 25 actual |
| tests/run.das:31 | `--family` lists 8 tokens | tests/CLAUDE.md lists **21** |
| tests/run.das:27 | `--suite` omits `image-vulkan` | accepted at :64, listed in error at :118; `--suite all` silently excludes it |
| tests/run.das:117–120 | `--full` unconditionally rejected | but parsed and threaded through envs/tmo — dead-but-live path |
| results_llm.md:5 | "daslang 0.6.3, 2026-07-22" | tree is 0.6.4; "CURRENT numbers only" banner |
| benchmarks/asr/results.md:16 | "0.6.3, 2026-07-09" | same |
| results_metal_*.md:1 ×5 | "living doc — CURRENT numbers only" | **no generator exists** — suspect by construction |
| harness/README.md | documents 7 of 78 files; "~9 minutes" 4k prefill; "11/11 SWA" | severely stale; perf claim predates flash prefill + metal override |
| tests/README.md:5–7 | leads with the forbidden direct-dastest invocation | contradicts run.das:4 + tests/CLAUDE.md |
| kv_cache_compression.md:30,150,325 | line refs into common.das + "re-verified 2026-07-09" | refs point at unrelated code now |
| API_REWORK.md:1021 | "dasllama_common.das (~1940 lines)" | 13,684 — 7× drift |
| qwen2_audio_plan.md:56 | `s.x_b` at ":4184" | actual 12652 |
| CONTROL_PAGE_PLAN.md:32 | panic at "common:7155" | wrong code there |

### Duplication sightings (slice-level)

1. `matmul_variants.das` — self-declared production-duplicate ledger (see above).
2. `attn_gemm_variants.das` — same pattern.
3. `bench_metal_gemm_kernels.das:3583–3975` — 4 frozen copies of production *emitted* MSL.
4. `conv_sub_bench.das` — engine-verbatim loop copies + own `quantize_repacked`.
5. `coopmat_mulmm_port/reference` pair — ~400 shared scaffold lines.
6. `gen_tune_probe.das` re-implements the repack rail (`repack_kq_grp_fmt` :421 mirrors `repack_k4/k5/k6_grp`; `pack_kq_disk_block` :382 hand-builds GGUF superblock layout) — partial, reuses transcode fns.
7. `tune_kernels.das:1046` `build_q8kv_row` re-packs the KV codec's q8_0 row layout.
8. `asr_bench` vs `asr_bench_cell`; `lcpp_bench` vs `gen_bench_records` (layered, not duplicated — the seam the single-rig consolidation targets).
GGUF-superblock **layout knowledge re-encoded in ≥4 places in this slice** (gen_tune_probe, both metal labs, test_kquant) on top of gguf.das/math_gen.das.

### Anomalies

- **Generated-but-tracked, no marker:** records/*.json (10,684 lines), profile_*.json, baseline_llm_*.tsv — hand-edit undetectable.
- **"Living docs" with no generator:** results_metal_*.md ×5, baseline_metal_*.tsv.
- **Repo-tree venvs:** `benchmarks/asr/.venv-nemo/`, `.venv-onnx/` (~GB-class; gitignored via `.venv-*/`; recreated by `setup_asr_rig --venvs`).
- **9 orphaned `.tune.json`** manifests naming deleted benchmarks (probe_logits_ab, batch_decode_perf, prefill_perf, prefill/decode_metal_chase, decode_real_bench, zz_attrib, kq_scale_bench, _argv_probe). `performance/m1.tune.json` NOT an orphan.
- **Abandoned scripts:** zen2_sweep.sh; coopmat pair; ~29 harness files referenced nowhere (probes are expected unreferenced, but README covers 7/78 so live-vs-dead is undecidable).
- **Machine-local absolute paths as defaults, 12 sites** — worst: `establish_baselines.das:54–55` llama-bench paths with NO env override; server tests' `models_dir()` default; every `cmd` string in records/*.json.
- Near-empty tracked TSVs (1–2 data rows) committed as baselines.
- `run_gemm_iso.das` 16-line wrapper works around dastest compiling benches away.

## Root docs audit (feeds the /history/dasLLAMA archiving)

Root inventory: 18 `.md` + `box_profile.epyc9654.json` + CMakeLists + `.das_module`. Only ENVIRONMENT.md is generated at root (marker L3, writer `harness/gen_env_doc.das`, gate `tests/test_env_registry.das`). Also generated elsewhere: `performance/results_llm.md`, `benchmarks/asr/results.md` GEN blocks.

### Disposition table

| doc | arc | disposition | live info to transfer | inbound refs (PQ = path-qualified, must update) |
|---|---|---|---|---|
| README.md | living index | **STAYS + freshness pass** | receives: full root-doc index (lists 4 of 18); layout tree refresh (16 missing files); GPU/Vulkan reality in line 3 ("CPU stack"); fix 2 self-contradictions (L306 `exp_probs_b` "panics" vs GLM-Air row L178 shipping it; L306 "Mamba/hybrid deferred" vs qwen35 shipped) | many bare + tutorial PQ |
| ENVIRONMENT.md | env-registry #3580 | **STAYS — GENERATED, never hand-edit** | — | RST `.. include::` (hard build dep), CLAUDE.md, skills, code ×many |
| METHODOLOGY.md | site-rework #3589 | **STAYS** (published) | receives record-schema + upsert-key material from PUBLIC_BENCH_PLAN | RST include (hard dep), BRINGUP:21, profile_common:59, site/README:275 |
| API_REWORK.md | API rework + perf ledger | **SPLIT**: plan (1–338, 1005–1042) → history; **ledger (339–1005, ≈47 entries, standing "note HERE" rule) → new root `PERF_LEDGER.md`** | Tier scope table (corrected) + arch-registry/kernel-seam prose → ARCHITECTURE.md | PQ: tutorial RST :80 + tutorial das :86; bare: README ×3, audio plans, CLAUDE.md:373 |
| tune_for_this_box.md | tune framework | **STAYS — hard blocked** (CMakeLists:58 installs into SDK; doc_archiving.md:16 canonical example) | fix in place: "15/16 kernels" → **25**; `box_profile.json` naming → `<app>.tune.json` | install + 5 PQ doc/skill refs + many bare |
| THINKING.md | thinking/reasoning arc | **STAYS** — verified NOT started (`strip_think` still hardcoded; `reasoning_content` appears nowhere else) | none — it IS the live plan | zero |
| PUBLIC_BENCH_PLAN.md | public-bench #3535 | **ARCHIVE after transfer** | record schema JSON + field semantics (flavor/tune/hardware/source/upsert key, replace-in-place) → METHODOLOGY.md or performance/RECORDS.md; "no shipped das binaries — [tune] rules them out" → METHODOLOGY/BRINGUP; v2 backlog → PERF_LEDGER.md | 4 code refs cite it as SCHEMA SPEC (lcpp_bench:27, setup_lcpp_ref:8, profile_common:556,1518) — transfer then re-point |
| gemm_generator_plan.md | GEMM generator (shipped; header "pre-implementation" stale) | **ARCHIVE** | AMX CLOSED + smmla verdicts → PERF_LEDGER + ARCHITECTURE ISA table; emission-only cross-ISA proof method → skills/llvm_tune.md; check "Still open in M4 proper" items vs llvm_tune.das first | **`harness/oracle/reduce_experiment.ll:7` relative link `../../gemm_generator_plan.md` breaks on move**; ~15 bare provenance refs in code headers |
| x64_arch.md | x64 port (self-marked HISTORICAL) | **ARCHIVE** | ground rules (-jit is the ONLY tier — never interp, never AOT; the doc's "AOT correct-but-untuned" framing is RETIRED, do not re-import); 3-layer x64 safety model; u8×s8 sign-trick identity → ARCHITECTURE.md kernel chapter (best existing draft of one) | bare sibling links only |
| get_x64_going.md | x64 bring-up (steps 4–6 describe DELETED hand backends) | **ARCHIVE** | **8-item Gotcha ledger is the live payload**: autocrlf corrupting ggml-vocab fixtures → skills/wsl_ci_repro; .jitted_scripts staleness → build_and_debug (verify); never-bench-interpreted → tune_for_this_box; empty-array addr fault → perf_lint/CLAUDE; DAS_JOBQUE_THREADS facts → environment_variables (verify) | bare sibling links |
| kernel_access_lens_metal.md | role-infer #3570 (IMPLEMENTED banner) | **ARCHIVE** | Vulkan-round gotchas (new .das must be in `.das_module` → dynamic_modules; `shared private` hides symbols → CLAUDE/daslib_modules; `var out : string` by-value → CLAUDE; const-table-index → CLAUDE) + macro derivation lessons (pre-infer bodies, location-keyed claims) → das_macros.md | zero |
| kv_cache_compression.md | KV codec (SHIPPED; header "ACTIVE, not started" = most misleading line in slice) | **ARCHIVE** | ragged KV layout invariants + "convert the query, not the cache" + prefill/decode asymmetry → ARCHITECTURE.md; BLAS-ruled-out verdict → ARCHITECTURE/PERF_LEDGER **with note its "BNNS not pursued" tail was superseded by #3562** | harness/kv_range_probe:10 bare, turboquant:5; stale line refs :30/:150 (see common anomalies) |
| turboquant_research.md | tq4 codec (shipped) | **ARCHIVE** — textbook case | skip-QJL verdict + "benchmark vs q4_0+Hadamard, not f16" → one ¶ in ARCHITECTURE KV section or `//!` on `quantize_tq4kv_row` | zero |
| whisper_plan.md | whisper (shipped) | **STAYS** — operative spec (`dasllama_whisper.das:26` cites it for container layout); matches history/README:7 carve-out | optional: perf-ledger tail (194–210, header says "fold into API_REWORK at PR time" — never done) → PERF_LEDGER.md | code refs operative |
| qwen2_audio_plan.md | qwen2-audio (shipped) | **STAYS** — operative (`dasllama_audio.das:24` DFT-as-GEMM rationale) | 2 duplicate perf-ledger sections (124–144) → PERF_LEDGER, dedup; **near-tie verdict method** (top-2 logit gap walk) → README "verify a new model" | code refs operative; stale line ref :56 |
| parakeet_plan.md | parakeet (shipped) | **STAYS** — operative (`dasllama_parakeet.das:26`) | nothing structural | operative |
| silero_vad_plan.md | VAD (shipped) | **STAYS** — strongest operative case (converter's container spec: `convert_silero_vad.py:10,17`) | nothing | operative PQ |
| audio_models_plan.md | audio wave A–F (all [x]) | **ARCHIVE** | Findings section is the payload: ggml build_stack padding never materializes → whisper_plan or audio.das `//!`; projection_dim lies for ultravox → same; f16-mmproj oracle rule → README verify section; register_arch MOVES ArchDesc (clone before alias) → ARCHITECTURE/skills; **⚠ OPEN item: v0.3 Mistral `[INST]` derived close is ` [/INST]` — wrong multi-turn, should be `</s>` — flagged for Boris, must land in PERF_LEDGER or an issue or it's lost** | README:190 bare only |
| box_profile.epyc9654.json | EPYC campaign (closed) | **ARCHIVE** beside `history/dasLLAMA/epyc9654_measurements.md` | nothing (portable-pin finding already in `dasllama_math.das:560`) | zero |

### /history mechanics

- Existing: `history/README.md` (index + **Archive log**, one line per doc, `- YYYY-MM-DD \`old\` → \`history/new\` — hook`, match the `→` style) + per-area dirs; `dasLLAMA/` already has 3 docs + `benchmarks/` subdir (10 retired bench scripts) — companions go in mirrored subdirs.
- history/README.md:6–7 already carries the exemption this depends on ("*_plan.md specs still cited from code stay") — honor or amend, don't contradict.
- Lines 14–17 = the dasLLAMA area bullet — extend it. Module-side note lives at modules/dasLLAMA/README.md:82 (extend, don't duplicate).
- Precedent: path-qualified refs in CODE comments get rewritten too (linq.das:2766 etc. → history/dasSQLITE/API_REWORK.md).

### Adjacent finding

`modules/dasLLAMA/CMakeLists.txt` `ADD_MODULE_DAS` list (5–36) omits ~20 shipped `dasllama/*.das` files (arch_glm4moe, arch_mistral3, config, env, gemm_*, image, kernel_access, layout, math_accelerate/gen/metal/vulkan, metal_*, par, parity, prefix, tune, vulkan_lens). The `install(DIRECTORY … "*.das")` at :40 ships them all — may be intentional post-never-AOT; needs an owner check.

