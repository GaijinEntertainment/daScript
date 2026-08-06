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
9. **Class-level vulkan kernels (SEPARATE ARC, separate PR — Boris ruling: not part of the
   reorg megarefactor). IN FLIGHT: `bbatkin/vulkan-class-kernels` — the conversion is
   complete (serving is 100% class kernels; census carries the evidence), the old-kernel
   sweep + the two PRs close it.** Investigate porting the Metal kernel model to the SPIR-V emitter:
   a kernel is a class with `@ssbo`/`@uniform`/`@workgroup` members and ordinary methods,
   free functions and inheritance lower 1:1 (the msl_emit Phase-0 machinery is the model).
   The module-global bindings are the root cause of the hand-written `vk_set6` ladders (no
   per-kernel interface to generate an encoder from), the `vk_meta` word-map convention
   (no named per-kernel params), whole-module binding-slot coupling, and global-state CPU
   oracles. SPIR-V has no classes, so members lower to per-kernel global `OpVariable`s under
   the hood — the class is the namespacing + interface surface, which then makes a generated
   set-builder lens possible. Cost to weigh: per-class descriptor-set layouts replace the one
   shared `VkDescriptorSetLayout`/pool — a real host-side rework. End state across both
   backends: one way to write a kernel in daslang; the backend is a target, not a dialect.
10. **Vulkan on Mac (M1/MoltenVK) + shared dispatch-lens helpers.** Two items that ride
   together: (a) make the vulkan tier green on the ssh M1 under MoltenVK — capability-gated
   declines (no coopmat, the 32 KB shared-memory cap declining the dn/at chains), the known
   `ffn_vs_ref` red bisected per-kernel via the model-less kernel-unit suite, portability_subset
   enabled at device create (the dasVulkan-side sweep); correctness only, Metal stays the fast
   path on that box. (b) Hoist the ~80 lines of dispatch-lens micro-grammar/validation the
   `[vk_dispatch]` and `[metal_dispatch]` lenses duplicate (`mk_uint_cast`, `is_digit_tok`,
   `role_ok`, `derived_role`, the `mk_grid_dim` core, `param_type`) into
   `dasllama_kernel_access` — INVENTORY's designated shared Metal↔Vulkan component, which both
   lenses already require. They diverged at birth (vulkan's grid folds any integer literal,
   metal's only "1"); one owner ends that.

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

## Sequencing

After the reorg arc lands (this doc is a product of it — see ARCHITECTURE.md's extraction
ledger). First measurable milestone: zen2 resident decode/prefill numbers vs the cooperative
tier on the same models, which also decides how hard items 3–4 are pushed. Item 1 (shapes
module) is independent and can land any time — it is pure structure.
