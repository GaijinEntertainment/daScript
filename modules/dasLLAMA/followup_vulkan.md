# Vulkan follow-up — grow the resident driver to Metal's contract shape

**Status: PLANNED (2026-07-29, reorg arc). Direction ruled by Boris: there is no residency
question on Metal — Apple Silicon is UMA, memory is memory (`bytesNoCopy` over host pages),
so Metal's whole-graph driver is the contract benchmark. Residency economics (arena, upload,
mirror, hydration) are a discrete-VRAM concern — Vulkan's concern. Vulkan converges toward
Metal's contracts; Metal does not grow partial-offload surfaces.**

## The target contract shape (what Metal implements today)

A GPU backend is a **whole-forward driver** behind the override registry (Contract A in
ARCHITECTURE.md terms) plus a **portable servability gate**:

- `register_prefill_override` / `register_decode_override` / `batch_decode` / `mtp_spec` /
  `mtp_seam` under one backend name — the engine hands the step over or keeps it, per the
  decline ladder.
- A shapes module with **no device requires** (`dasllama_metal_shapes` pattern: decline enum,
  needs bitfield, `*_servable` predicate) so any box can bake/decide without the driver.
- Residency machinery is **driver-internal**, never engine-facing.

The cooperative/partial-offload hook seam (`dasllama_gpu_tier.das`) REMAINS for the
doesn't-fit-in-VRAM tier — the heat-pinned expert cache splitting one layer's experts between
CPU and GPU is a measured win (26B on the 8 GB 3060 Ti) and has no whole-graph equivalent.
Resident (fits) and cooperative (doesn't fit) are complementary Vulkan modes, not rivals.

## The gap list (from INVENTORY.md's Metal↔Vulkan parity map)

Ordered roughly by user-visible value; re-rank against zen2 measurements before starting.

1. **Servability gate module** — the single biggest asymmetry: Vulkan has NO
   `dasllama_metal_shapes` analog (no decline enum, no needs bitfield, no servable predicate;
   `resident_upload` rejects 12 family features ad hoc, by name). Build
   `dasllama_vulkan_shapes` on the Metal pattern: portable, no `vulkan` require, feeds the
   same decline-reason reporting.
2. **Family coverage in the resident driver** — today it serves the llama-family std shape
   only. Metal's drivers serve MoE (Wave C), DeltaNet hybrids (Wave D), gemma4 (PLE,
   sandwich norms), gpt-oss (sinks, swiglu_oai). The vulkan kernels for MoE/deltanet already
   exist in the cooperative tier — the work is resident-driver plumbing, not new shaders.
   Walkthrough datum (2026-08-06): qwen35-0.8B serves CORRECTLY on the per-op rails but the
   per-layer submit+fence cadence caps it at 35% tg / 8% pp of llama.cpp's whole-graph run —
   the hybrid-ladder extension is the fix, and the carrier conversion already made
   dn_step_cls a TokMeta class kernel, so recurrent layers can encode straight into the
   recorded token cmd; what remains is ladder plumbing (recurrent roles in rd_record_token,
   dn state on g_rd).
3. **KV codecs on device** — Vulkan's mirror is f32-only; Metal carries f16/f32/q8_0/tq4
   through every attention and rope-store kernel. Port the codec seams (the CPU truth is
   `dasllama_convert`'s KV codec functions; the Metal kernels are the device reference).
4. **Real batched decode** — the resident mirror is single-sequence; batch rows round-trip
   their KV per step (`rdec_sync_kv` in, `rdec_read_kv` out). Metal has a true batched driver
   (P4). Options: multi-sequence mirror slabs, or per-row device KV like Metal's `KVMirror`.
5. **MTP / speculative decode** — Metal-only today (`metal_mtp_spec_eval`, same-slab verify).
   Register the vulkan `mtp_spec`/`mtp_seam` overrides once batch (4) exists — the verify step
   is a B=2 batch.
6. **mx4 / q51 device kernels** — absent on Vulkan (CPU+Metal only). Needed before gpt-oss
   (mx4 experts) or gemma-4-26B (q51 stacks) can go resident on PC.
7. **Tune-race parity** — Metal races tensor-op twins per box (`metal_tensor_race*`); Vulkan
   picks coopmat variants by env (`DASLLAMA_COOPMAT`). Fold the choice into the tune rail.
8. **Compile-time dispatch census** — Metal's `[lint_macro]` manual-dispatch census
   cross-checks every `@role`; Vulkan has only the runtime `hz_masks` panic. Port the census
   to the vulkan lens.
9. **Class-level vulkan kernels — SHIPPED (the `bbatkin/vulkan-class-kernels` arc).** The
   Metal kernel model is ported to the SPIR-V emitter: a kernel is a class with
   `@ssbo`/`@uniform`/`@workgroup` members and ordinary methods (`[spirv_kernel]`), the
   `[vk_dispatch]` lens generates the ensure/set/enc surface per class, and the old world —
   module-global kernels, the `vk_set6` ladders, the `vk_meta` word maps, the shared 6-slot
   layout — is deleted outright. Serving is 100% class kernels (the `coverage-vk` census is
   the evidence); every kernel carries a CPU-oracle parity gate in `test_vulkan_kernels.das`.
   End state across both backends: one way to write a kernel in daslang; the backend is a
   target, not a dialect. Remaining relatives live as their own items: the lens-helper hoist
   + M1 (item 10), cm2 kernel quality (item 11), the reification macro layer (next arc).
10. **The MAC SESSION (ruled 2026-08-06): Vulkan on Mac + the kernel-model asymmetry closure
   + shared dispatch-lens helpers.** Three items that ride together in one session on the
   M-box: (a) make the vulkan tier green on the ssh M1 under MoltenVK — capability-gated
   declines (no coopmat, the 32 KB shared-memory cap declining the dn/at chains), the known
   `ffn_vs_ref` red bisected per-kernel via the model-less kernel-unit suite, portability_subset
   enabled at device create (landed dasVulkan-side); correctness only, Metal stays the fast
   path on that box. (b) Close the kernel-model asymmetry ledger
   (`modules/dasMetal/MASTERPLAN.md` §Cross-backend parity): relax `[metal_dispatch]` to
   multi-kernel + `family=` like `[vk_dispatch]`, adopt inheritance in the metal kernel
   corpus where families exist, fix the metal lens's grid-literal infer trap. (c) Hoist the
   ~80 lines of dispatch-lens micro-grammar/validation the `[vk_dispatch]` and
   `[metal_dispatch]` lenses duplicate (`mk_uint_cast`, `is_digit_tok`, `role_ok`,
   `derived_role`, the `mk_grid_dim` core, `param_type`) into `dasllama_kernel_access` —
   INVENTORY's designated shared Metal↔Vulkan component, which both lenses already require.
   They diverged at birth (vulkan's grid folds any integer literal, metal's only "1"); one
   owner ends that, and (b) rides on the hoisted core.

11. **cm2 prefill GEMM across formats — close the llama.cpp prefill gap (NEXT ARC, ruled
   2026-08-06).** Walkthrough evidence (zen2, RTX 5060 Ti, class-kernel branch vs llama.cpp
   b9860 Vulkan, tinyllama-1.1B Q8, pp512/tg128 x5): decode tg 291.6 vs 294.7 = **99% —
   parity**; prefill pp 7242 vs 20900 = **34.6% — 2.9x behind**. Their prefill runs
   NV_coopmat2 matrix-core GEMMs; ours ran the default mode-3 mul_mm hand-staged L-tile.
   The stack below is NOT the gap: dasSpirv emits SPV_KHR_cooperative_matrix AND
   SPV_NV_cooperative_matrix2 (tensor addressing, workgroup-scope tiles, `[spirv_decode]`
   decode-in-load), dasVulkan enables both extensions, and the tier detects
   `has_coopmat`/`has_coopmat2`. What is missing, in order:
   (a) the cm2 decode-in-load GEMM exists ONLY for fmt-6 q8n stacks (mode 4, an experiment
   arm with its own xf16/actf16/f16cvt side chain) — generalize it to the formats serving
   actually uses (q8, k4/k5/k6, q40);
   (b) tile-shape quality/tuning — per-device shapes like llama.cpp's, fold into the tune
   rail (item 7 composes);
   (c) mode selection defaults — `has_coopmat2` should pick the cm2 path by itself instead
   of the `DASLLAMA_COOPMAT` env force.
   PROBE RESULT (same box/model/shapes, DASLLAMA_COOPMAT=cm2): pp512 5917 ± 125 — our cm2
   arm loses to our own mm tile (7242) and sits at 28% of llama.cpp's cm2 (20900). The gap
   is KERNEL QUALITY, not coverage — (b) is the arc's center, (a) rides it. Also: mode 4
   drags decode onto the q8n GEMV chain and costs 14% tg (291.6 -> 250.7) — the prefill-GEMM
   mode must decouple from the decode-GEMV format pick.
   Methodology for the arc (Boris, 2026-08-06): benchmark per CAPABILITY TIER, not just
   flagship-vs-flagship — force both sides down the ladder and compare like-for-like
   (pretend the 5060 Ti is a 3060: no coopmat2, then no coopmat). Ours: DASLLAMA_COOPMAT=
   sdot4|f16|int8|mm|cm2. Theirs: INVESTIGATE llama.cpp's switches — ggml-vulkan has
   GGML_VK_DISABLE_COOPMAT / GGML_VK_DISABLE_COOPMAT2-style envs (verify names/behavior on
   their current master); if they hold, every tier gets an apples-to-apples baseline.

12. **Arena slabs — the 4 GiB storage-range ceiling (LANDED in-arc 2026-08-06; was the
   PR gate — the MAIN FACTOR for MoltenVK/M1 enablement, where maxStorageBufferRange is far
   tighter than 4 GiB).** Walkthrough evidence that motivated it: Llama-3.1-8B Q8's fmt-0
   arena wants 7.5 GB against the device's 4294967295B maxStorageBufferRange — honest
   fail-closed decline, per-op fallback served tg 7.2 vs llama.cpp's 49.8. As built: each
   ArenaFmt carries lazily-opened slabs capped at msr-derived blocks (both plane strides);
   `arena_place` returns `(slab << 32) | local` so the encoding rides every existing seam
   unchanged; `arena_planes(fmt, blk)` binds the tensor's slab; region/schedule metas carry
   the local half; merged k/v splits at a slab boundary; per-call arena seams cache sets per
   slab; a single tensor over one slab declines. Multi-slab correctness gated by
   `test_vulkan_arena_slabs` (forced tiny slab cap). Measured: Llama-3.1-8B Q8 resident
   across two slabs at 93.3% tg / 54.3% pp of llama.cpp (was 14% / 3% on the fallback).
   FALLOUT FOUND AND FIXED: the resident ctx auto-negotiation OVERSUBSCRIBED — uncapped it
   armed at ctx 25590 (weights + KV = 14.5GB of the 16GB card), WDDM demotion took tg to
   3.65. The plan now accounts the driver's own scratch (`rdec_scratch_bytes` — prefill
   window dominates) and the auto arm keeps a 2GiB desktop headroom past the tier's reserve
   (`RDEC_VRAM_HEADROOM`; a pinned DASLLAMA_GPU_VRAM_MB stays the user's contract and fills
   to its brim). Uncapped now arms at ctx 16627 and holds 44.9 tg / 1967 pp — the queried
   heap "used" (2MB against a real desktop) stays untrusted.

13. **qwen2 bias arm — the cheapest family unlock (after-sweep follow-up commit, ruled
   2026-08-06: "unsupported family, easy to support").** The resident gate declines
   `attn_qkv_bias` alone for the whole qwen2 line; everything downstream is the std shape the
   ladder serves. Walkthrough row (Qwen2.5-1.5B Q8): das 35.6 tg / 465 pp on the per-op
   fallback vs llama.cpp 202.4 / 14986 — 18% / 3%. Fix shape: per-row `+ b[row]` epilogue in
   the qkv class GEMV (bias rows ride the arena or one extra binding, offset in the push),
   same arm for q/k/v; then drop `attn_qkv_bias` from the :534 gate.

14. **The gemma cluster arms — medium family unlock covering gemma2 AND gemma3 (walkthrough
   2026-08-06: both UNSUPPORTED, per-op fallback; gemma2-2b 22.8 tg / 327 pp vs llama.cpp
   124.1 / 9566, gemma3-1b 58.5 / 636 vs 236.2 / 21540).** The shared base: sandwich norms
   (`pre_post_norm` — an extra norm role per layer) + sliding-window alternation (a
   window-start word in decode attention — TokMeta has room). That pair alone unlocks
   gemma3; gemma2 additionally needs the two softcap clamps (attn epilogue tanh + cls
   epilogue); gemma4-dense sits on the same base (plus PLE — its own story). Biggest
   family-count unlock on the board after qwen2's one-flag bias.

15. **mm L-tile zeroes its whole dispatch on a non-128-multiple weight dim (latent, found by
   the CPU-oracle conversion).** The mode-3 `MmBatch` L-tile GEMM writes all-zeros for the
   ENTIRE dispatch when the weight dim `d` is not a multiple of 128 — and the retired
   hand-written kernel did exactly the same, which is why old-vs-new parity stayed green on
   it (zeros == zeros; the vacuous-parity failure mode the oracle conversion exists to
   catch). Serving is unaffected today because every served model dim is a 128-multiple,
   and the suite fixture pins row-edge coverage only. Repro: give the `v2` mm arm a fixture
   with `d = 192` — the whole output zeroes. Fix shape: a d-edge guard/masked tail in the
   L-tile stage (check `MmABatch`/`MmMBatch` for the same edge), then widen the fixture
   back to a non-128-multiple `d`.

16. **`run.das` cannot arm vulkan — GPU coverage needs direct dastest.** The runner spawns
   without `-load_module dasVulkan` and injects `DASLLAMA_CPU_PREFILL`, so any vulkan-armed
   arm under it silently runs CPU (the census suite's `coverage-vk` arm is the standing
   example — it must be run via direct dastest + `-load_module`). Candidate fix: teach the
   runner a GPU lane (propagate `-load_module`, drop the CPU_PREFILL injection for
   vulkan-tagged tests) or make it refuse loudly when a test declares a vulkan requirement
   it cannot satisfy.

17. **VK_EXT_pageable_device_local_memory — the missing half of the residency shield.** The
   tier chains VK_EXT_memory_priority (priority 1.0 on every device allocation, the armed
   "residency shield"); the companion extension — runtime `vkSetDeviceMemoryPriorityEXT` +
   the pageable-aware device-local signal WDDM wants — has zero references in the tree.
   Small addition: enable when present, and consider demoting cold stacks' priority instead
   of only boosting everything.

## Sequencing

**The osmosis principle (Boris, 2026-08-06, mid-walkthrough):** older/dense families are
carried "by osmosis" — good kernels (the cm2 arc), good cache strategy, overall rail
goodness — their per-family arms are mechanical one-flag/one-cluster work, done
opportunistically, never a focus. The focus after plumbing is NEW model shapes: MoE, MTP,
hybrids — where design room actually exists. On MoE specifically the 3060-era record had
das WINNING (better MoE strategy: heat cache, expert residency, async shexp — no llama.cpp
analog); the walkthrough shows their remaining edge is the cm2 prefill kernel alone, so
tensor kernels + our MoE strategy = the expected win condition on coopmat2 hardware too.

After the reorg arc lands (this doc is a product of it — see ARCHITECTURE.md's extraction
ledger). First measurable milestone: zen2 resident decode/prefill numbers vs the cooperative
tier on the same models, which also decides how hard items 3–4 are pushed. Item 1 (shapes
module) is independent and can land any time — it is pure structure.
