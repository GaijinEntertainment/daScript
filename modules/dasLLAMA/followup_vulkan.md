# Vulkan follow-up - grow the resident driver to Metal's contract shape

**Status: PLANNED (2026-07-29, reorg arc). Direction ruled by Boris: there is no residency
question on Metal - Apple Silicon is UMA, memory is memory (`bytesNoCopy` over host pages),
so Metal's whole-graph driver is the contract benchmark. Residency economics (arena, upload,
mirror, hydration) are a discrete-VRAM concern - Vulkan's concern. Vulkan converges toward
Metal's contracts; Metal does not grow partial-offload surfaces.**

## The target contract shape (what Metal implements today)

A GPU backend is a **whole-forward driver** behind the override registry (Contract A in
ARCHITECTURE.md terms) plus a **portable servability gate**:

- `register_prefill_override` / `register_decode_override` / `batch_decode` / `mtp_spec` /
  `mtp_seam` under one backend name - the engine hands the step over or keeps it, per the
  decline ladder.
- A shapes module with **no device requires** (`dasllama_metal_shapes` pattern: decline enum,
  needs bitfield, `*_servable` predicate) so any box can bake/decide without the driver.
- Residency machinery is **driver-internal**, never engine-facing.

The cooperative/partial-offload hook seam (`dasllama_gpu_tier.das`) REMAINS for the
doesn't-fit-in-VRAM tier - the heat-pinned expert cache splitting one layer's experts between
CPU and GPU is a measured win (26B on the 8 GB 3060 Ti) and has no whole-graph equivalent.
Resident (fits) and cooperative (doesn't fit) are complementary Vulkan modes, not rivals.

## The gap list (from the 2026-07-29 census's Metal<->Vulkan parity map - archived as history/dasLLAMA/INVENTORY.md)

Ordered roughly by user-visible value; re-rank against zen2 measurements before starting.

1. **Servability gate module** - the single biggest asymmetry: Vulkan has NO
   `dasllama_metal_shapes` analog (no decline enum, no needs bitfield, no servable predicate;
   `resident_upload` rejects 12 family features ad hoc, by name). Build
   `dasllama_vulkan_shapes` on the Metal pattern: portable, no `vulkan` require, feeds the
   same decline-reason reporting.
2. **Family coverage in the resident driver** - today it serves the llama-family std shape
   only. Metal's drivers serve MoE (Wave C), DeltaNet hybrids (Wave D), gemma4 (PLE,
   sandwich norms), gpt-oss (sinks, swiglu_oai). The vulkan kernels for MoE/deltanet already
   exist in the cooperative tier - the work is resident-driver plumbing, not new shaders.
   Walkthrough datum (2026-08-06): qwen35-0.8B serves CORRECTLY on the per-op rails but the
   per-layer submit+fence cadence caps it at 35% tg / 8% pp of the upstream whole-graph run -
   the hybrid-ladder extension is the fix, and the carrier conversion already made
   dn_step_cls a TokMeta class kernel, so recurrent layers can encode straight into the
   recorded token cmd; what remains is ladder plumbing (recurrent roles in rd_record_token,
   dn state on g_rd).
3. **KV codecs on device** - Vulkan's mirror serves f16 (the armed default) and f32 through
   the codec-templated kernel stamps; Metal additionally carries q8_0/tq4. Port the quant
   codecs next (the CPU truth is `dasllama_convert`'s KV codec functions; the Metal quant
   kernels are the device reference).
4. **Real batched decode** - the resident mirror is single-sequence; batch rows round-trip
   their KV per step (`rdec_sync_kv` in, `rdec_read_kv` out). Metal has a true batched driver
   (P4). Options: multi-sequence mirror slabs, or per-row device KV like Metal's `KVMirror`.
5. **MTP / speculative decode** - Metal-only today (`metal_mtp_spec_eval`, same-slab verify).
   Register the vulkan `mtp_spec`/`mtp_seam` overrides once batch (4) exists - the verify step
   is a B=2 batch.
6. **mx4 / q51 device kernels** - absent on Vulkan (CPU+Metal only). Needed before gpt-oss
   (mx4 experts) or gemma-4-26B (q51 stacks) can go resident on PC.
7. **Tune-race parity** - Metal races tensor-op twins per box (`metal_tensor_race*`); Vulkan
   picks coopmat variants by env (`DASLLAMA_COOPMAT`). Fold the choice into the tune rail.
8. **Compile-time dispatch census** - Metal's `[lint_macro]` manual-dispatch census
   cross-checks every `@role`; Vulkan has only the runtime `hz_masks` panic. Port the census
   to the vulkan lens.
9. **Class-level vulkan kernels - SHIPPED (the `bbatkin/vulkan-class-kernels` arc).** The
   Metal kernel model is ported to the SPIR-V emitter: a kernel is a class with
   `@ssbo`/`@uniform`/`@workgroup` members and ordinary methods (`[spirv_kernel]`), the
   `[vk_dispatch]` lens generates the ensure/set/enc surface per class, and the old world -
   module-global kernels, the `vk_set6` ladders, the `vk_meta` word maps, the shared 6-slot
   layout - is deleted outright. Serving is 100% class kernels (the `coverage-vk` census is
   the evidence); every kernel carries a CPU-oracle parity gate in `test_vulkan_kernels.das`.
   End state across both backends: one way to write a kernel in daslang; the backend is a
   target, not a dialect. Remaining relatives live as their own items: the lens-helper hoist
   + M1 (item 10), cm2 kernel quality (item 11), the reification macro layer (next arc).
10. **The MAC SESSION (ruled 2026-08-06): Vulkan on Mac + the kernel-model asymmetry closure
   + shared dispatch-lens helpers.** Three items that ride together in one session on the
   M-box: (a) make the vulkan tier green on the ssh M1 under MoltenVK - capability-gated
   declines (no coopmat, the 32 KB shared-memory cap declining the dn/at chains), the known
   `ffn_vs_ref` red bisected per-kernel via the model-less kernel-unit suite, portability_subset
   enabled at device create (landed dasVulkan-side); correctness only, Metal stays the fast
   path on that box. (b) Close the kernel-model asymmetry ledger
   (`modules/dasMetal/ARCHITECTURE.md` sec.5 Cross-backend parity): relax `[metal_dispatch]` to
   multi-kernel + `family=` like `[vk_dispatch]`, adopt inheritance in the metal kernel
   corpus where families exist, fix the metal lens's grid-literal infer trap. (c) Hoist the
   ~80 lines of dispatch-lens micro-grammar/validation the `[vk_dispatch]` and
   `[metal_dispatch]` lenses duplicate (`mk_uint_cast`, `is_digit_tok`, `role_ok`,
   `derived_role`, the `mk_grid_dim` core, `param_type`) into `dasllama_kernel_access` -
   INVENTORY's designated shared Metal<->Vulkan component, which both lenses already require.
   They diverged at birth (vulkan's grid folds any integer literal, metal's only "1"); one
   owner ends that, and (b) rides on the hoisted core.

11. **cm2 prefill GEMM - close the upstream prefill gap (ACTIVE ARC, ruled 2026-08-06).**
   Origin evidence (zen2, RTX 5060 Ti, class-kernel branch vs upstream b9860 Vulkan,
   tinyllama-1.1B Q8, pp512/tg128 x5): decode tg 291.6 vs 294.7 = **99% - parity**; prefill
   pp 7242 (mm) vs 20900 = 34.6%, and the old fmt-6 cm2 arm sat at 5917 +/- 125 = 28% - the
   gap was KERNEL QUALITY, not coverage.
   DONE in-arc: the l-geometry re-cut on NATIVE fmt-0 two-plane q8 (decode METHOD reads the
   scale plane - the class-method `[spirv_decode]` form), the fast/edge clamp split, the
   m tile + split-k + occupancy-driven selection (`cm2_tile_cols`/`cm2_split_k`,
   `shaderSMCount` via VK_NV_shader_sm_builtins), the decode/prefill format decouple (mode 4
   serves decode on the unchanged q8 GEMV chain - the 14% tg drag is gone), and the fmt-6
   q8n side stack DELETED end-to-end (KqFmt.q8n, the gather, the xf16/actf16 side chain's
   q8n arms, kernels, probe surface, tier tests). The no-split cm2 arms keep LITERAL loop
   bounds - branch-derived k0/k1/ybase in the hot tensor loop measured -27% pp even with
   no-op values.
   STILL OPEN in this item:
   (a) K-quant generalization - extend decode-in-load to k4/k5/k6/q40 (shared-memory
   staging of the scale planes is the model);
   (b) per-device tile tuning - fold the l/m/split picks into the tune rail (item 7);
   (c) mode selection defaults - `has_coopmat2` should pick cm2 by itself instead of the
   `DASLLAMA_COOPMAT` env force (gate: cm2 must first beat mm on the serving models);
   (d) decode_vector (`VK_NV_cooperative_matrix_decode_vector`, V=4 f16vec4 decode) -
   DRIVER-BLOCKED on this box (610.74 lacks the extension; probe 2026-08-07); revisit on a
   driver that exposes it. Emit-time arm only, never SPIR-V patching.
   Methodology (Boris, 2026-08-06): benchmark per CAPABILITY TIER, like-for-like - ours:
   DASLLAMA_COOPMAT=sdot4|f16|int8|mm|cm2 (+ DASLLAMA_CM2_TILE/DASLLAMA_CM2_SPLITK as A/B
   instruments); reference side: GGML_VK_DISABLE_COOPMAT / _COOPMAT2 / _COOPMAT2_DECODE_VECTOR
   (verified in its device-init walk).
   Baseline re-pin (2026-08-27, zen2 / 5060 Ti, driver 610.74, debug-jit vs the reference exe
   b10659; fa + native f16 mirrors now in the default path): llama-3.2-3B Q8 das
   6644.8 +/- 98.4 pp512 / 105.4 +/- 0.2 tg128 vs 7691.0 +/- 42.7 / 110.0 +/- 0.3 =
   86.4% pp / 95.9% tg (was 75.5% / 95.1% at the 8/06 family walkthrough; upstream itself
   did not move b9860 -> b10659). GPU_PROF split of the pp window: ~99% GPU-busy (submit
   75.2 ms, gpu 74.7, prep+record ~2), so the gap is per-GEMM kernel rate, not CPU
   serialization - FFN GEMMs are 63% of the window and gate/up run ~43 TFLOP/s where
   down/q reach ~50-53; attn is 3% post-fa. Decode sits at the bandwidth ceiling (~358 of
   448 GB/s; theirs ~371). Open probes: (e) capture the reference exe's same-shape dispatch under
   ngfx and diff the four counters (ours: tensor 27.4 / L2 45.0 / dram 10.4 / l1tex 19.5);
   (f) why the widest GEMMs (gate/up, d=8192) run ~20% below down at the same M.
   PROBES (e)+(f) ANSWERED (2026-08-27, `harness/vk_gemm_probe.das` - the mm_a serving
   kernel isolated at the 3B role shapes, record-once + timed submits; one arg pins a
   single shape as the ngfx capture window): (f) the kernel is SHAPE-UNIFORM -
   gate/up 48.3 / down 49.3 / q 49.8 TFLOP/s isolated; the in-situ 43-vs-53 split is
   timestamp/stall attribution, not shape behavior. k/v starved ISOLATED (26.5 TFLOP/s at
   32 wgs) but NOT in situ: the chain records q,k,v with no barrier between them, the level
   overlaps on the device, and the merged-k|v GEMM (shipped behind DASLLAMA_VK_KV_MERGE, one
   dispatch over the adjacent planes at d = 2*kvd) measured a WASH on the 3B
   (6571 +/- 78 vs 6555 +/- 68 = +0.24%). The merge stays for the record shortening
   (28 dispatches + 28 copies + a barrier per window gone - CPU record cost, the overlap
   arc's term), not as a GPU win. mm_a at 48 vs the old cm2-l harness's 28.9 also
   re-confirms mm as the right default. (e) their
   isolated q8_0 GEMM op (its per-op perf harness, m=4096 n=512 k=14336, their only q8/n=512
   stock case) = 51.5 TFLOP/s - the isolated kernel-rate gap is <= ~6%. Counter diff
   THE CM2 CLIFF, FOUND AND FIXED (2026-08-27): our cm2 l-tile ran 28-35 TFLOP/s where
   upstream's identical geometry ran 62-66 - root cause was ONE BIT in the SPIR-V emitter:
   `coopmatClamp`'s hand-emitted per-element loop carried `OpLoopMerge ... None`; unrolled
   (glslang spells the same loop `[[unroll]]`), the driver keeps the wg-scope accumulator in
   tensor-register form - left rolled, the dynamic per-element index demotes the coopmat to
   addressable storage FOR THE WHOLE KERNEL. Fix: `spirv_emit.das` coopmatClamp loop control
   None -> Unroll; min-kernel 34.2 -> 57.8 TFLOP/s (+64%). Hunt instruments (all in
   `harness/vk_gemm_probe.das` + envs): `cm2x` arg = decode-cost bisect variants; `their` arg
   = the reference exe's glslc-built coopmat2 GEMM blob dispatched in OUR harness via
   DASLLAMA_VK_SPV_OVERRIDE (spec constants patched by spirv-opt to the l geometry);
   DASLLAMA_VK_SPV_DUMP (new env) = the override's capture half. Bisect ledger (gate shape,
   drain-free): their stock 66.0 / their+external-scale-plane 60 / +our-decode-arithmetic 55.4
   / a GLSL twin of OUR minimal kernel 58.1 / our minimal pre-fix 34.7 - so decode spelling
   costs ~9% (their 16-bit unpack8 form needs Int8 caps we do not emit) and the interleaved
   scale ~5%; everything else was the clamp-loop bit. Post-fix shipped-kernel table
   (cnt=512, drain-free): cm2l 58.1/60.3 gate/down BEATS mm 53.7/54.9; with the probe's
   write-write barriers cm2l 47.8/40.8 vs mm 48.9/52.0 - the smaller cm2 grids (half mm's
   wg count) pay wave-quantization drains. E2E 3B (debug-jit, REBAR=0): mm 7058 +/- 28 pp
   (unchanged serving default), mode-4 cm2 6676 +/- 32 - the kernel now wins isolated but
   the chain packaging (grid sizes, barrier drains, the f16 staging step) still favors mm;
   blunt DASLLAMA_CM2_SPLITK=2 across all GEMMs = 4058 (the reduce tax on well-filled
   shapes). Items (b)+(c) are therefore LIVE again: the l/m/split heuristics were tuned
   against the 2x-slower kernel and must be re-tuned before the mode-4 default flip.
   THE DECODE SPELLING, CLOSED (2026-08-27 late): the last ~15% was the decode-callback
   ARITHMETIC FORM. Probe ladder on their kernel, our two-plane data (gate, drain-free):
   16-bit load + unpack8 + [i&1] lane = 62.7 (their stock 63.3 - the external scale plane
   costs ~1%); 32-bit word + variable shifts = 55.4; 32-bit unpack8 + dynamic 4-lane
   select = 20.3 (VectorExtractDynamic on v4char poisons the block-load path outright).
   The driver pattern-matches THEIR EXACT 16-bit spelling. Shipped: VkQ8Blk is int16[16]
   and decode_q8 is `unpack8(qs[(cib.y & 30) >> 1])[cib.y & 1]` - the das storage-type
   surface (int8/16 SSBO members, unpack8/pack32, Int8/Int16 + storage caps) already
   existed golden-tested in dasSpirv (test_storage_8_16); the ONLY additions were the
   unpack8(int16/uint16) -> byte2/ubyte2 lingua-franca overloads (zero emitter change) and
   the core shaderInt16 device feature across the vulkan_boost storage_8_16 creator family
   (+ the storage_8_16_supported gate; the caps validated only by luck before). RESULTS:
   cm2l drain-free 62.2/64.7 gate/down = par with their blob, +15-18% over mm; E2E 3B
   mode-4 pp 7293.9 +/- 47 - NEW BEST, BEATS mm (7058 +/- 28) by +3.3% = 94.8% of their
   cm2 build; tg 104.4 (decode decoupled). tinyllama mode-4 18102 vs mm 19754 (-9%): the
   small shapes starve the 128x256 l grids (kv = 4 wgs) - item (b)'s re-tune is what the
   (c) default flip waits on, per-model or per-shape. Newly visible after the fix: the
   wg_blk0 Workgroup-storage read in decode costs ~9% (lit 51.6 vs full 47.3 at cnt=512
   with barriers) - a push-constant block base for single-region dense dispatches is the
   next kernel-side lever. OWED from the storage-type plan: small-int SSBO STORE coverage
   (no fixture writes int8/int16 today) - unblocks deleting the hand-rolled word-packing
   in every requant writer kernel.
   (b)+(c) CLOSED (2026-08-27, commit 57437c8f7): cm2_tile_cols rewritten to a
   wave-efficiency comparison (cross-multiplied occupied/allocated wave slots, m only on a
   strict win, ties to l) - probe-fit on all 8 role-shape points; tinyllama mode-4
   18102 -> 20159 on this alone. With that, mode 4 beats mm back-to-back on BOTH serving
   models and `resolve_coopmat_mode` now DEFAULTS to cm2 on coopmat2 hardware. Default-path
   board vs b10659: 3B 7406.8 +/- 310 pp / 105.7 tg = 96.3%/96.1%; tinyllama
   20188.1 +/- 185 / 294.4 = 99.6% pp (inside their row noise), tg ahead. Mode-4 headroom
   still unported: the ar+rq fusion (the fq6 gate skips it), the kvm merge (mode-4
   excluded), the wg_blk0 push-constant base (~9% of the decode callback).
   ar fusion PORTED (2026-08-27, commit 4b690e77e): cls_ar_f16_b - the fused add+rms twin's
   f16 form, bit-identical to the split cls_ar + f16cvt pair (gated). vk_fuse A/B: 3B pp
   7463 -> 7584 (+1.6%), tg +2.6%; tinyllama pp 19978 -> 20374 (+2.0%), tg +4.0% - both
   models' new bests, tinyllama pp now ~100.5% of their row.
   wg_blk0 lever DEAD (same day): the cm2x probe grew a `push` variant (base off pa.ksplit)
   - push is the SLOWEST spelling (gate/up 49.6 vs full 52.5 vs lit 51.4 TF/s; down 41.7 /
   43.9 / 42.8), and lit no longer beats the shipped form either. The old lit-51.6-vs-47.3
   delta predates the 16-bit decode respelling; with the cheap decode the shared wg_blk0
   read is free. Item closed as measured-no.
   kvm merge PORTED to mode 4 (same day, commit 2a4431fb2): the exclusion was pure caution -
   pf_gemm_enc is parametric in (d, blk). vk_kv_merge A/B on cm2: 3B pp 7419 -> 7633 (+2.9%),
   tinyllama +0.5%. fa f16-out stamp (commit 5267a63b1): FaCm2H64/H128 templated
   (OUT16/typedef OT), the O accumulator converts in-kernel and lands the wo feed - the
   per-layer b+6 attn->f16 convert never encodes; bit-exact vs the split pair's own device
   f16cvt (CPU float16() differs on rounding ties - device converts agree with each other).
   A/B: 3B 7669 -> 7737/7708 (+0.7-0.9%), tinyllama 20796 -> 20986 (+0.9%).
   END-OF-DAY BOARD vs b10659: 3B pp 7737.2 +/- 67 = 100.6% - AHEAD of the reference exe for the
   first time; tinyllama pp 20986 +/- 357 = ~103.5%, tg ahead. 3B tg 105.1 = ~95.5% (decode
   chain untouched today).
   Small-int STORE ledger CLOSED (2026-08-28, commit a59d095d9): the 8/16-bit store half got
   its coverage - a golden fixture (narrowing converts + 8/16-bit access-chain stores,
   spirv-val clean) and a live-device exact-bytes cell (test_storage_8_16_store_gpu) - and on
   that foundation every Q8 requant writer stores quants as bytes: q8_pack4 and the q8k
   butterfly (2 subgroup shuffles per element) deleted, outq members array<int8>. Bit-exact
   by the gates; perf-neutral where the writers run hot (mm-mode 3B pair 7077 vs 7061, tg
   equal). Remaining tail: item (a) K-quant generalization, (d) decode_vector driver-blocked.
   Item (a) OPENED with Q4_K (2026-08-28, commit f72694fbe): K4Cm2LBatch/K4Cm2MBatch - the
   Q8 tile geometry with a Q4_K decode callback (nibble + per-32-group scale/min off the
   repacked planes, (1, 256) layout blocks). Oracle-gated 0-off; probe: 35.8-38.2 TF/s vs
   the kq tile's 12.0-12.7 on every Qwen3-4B role shape (~70% of Q8-cm2's rate - the
   nibble+scale extraction). Wiring: pf_f16_feed admits k4, the feed flags are GROUP-wide
   ANDs (a k6 sibling pins its group to the kq route - Q4_K_M mixes k4+k6 in one group).
   Qwen3-4B Q4_K_M mode-3/4 pair: pp 1626 -> 2654 (+63%), tg equal, parity token-exact.
   Q6_K tiles LANDED PINNED (same day, commit d89b74681): oracle 0-off on both tiles, but
   the rate collapsed to 9.3-13.4 TF/s vs the kq tile's 11.9 - unpinned e2e regressed.
   Q6_K CLIFF FOUND AND FIXED (same day, commit 4603a7373): the k6x bisect (nil 59.6 /
   flat 39.7 / ql 47.8 / pair 13.4) proved the two-plane 6-bit compose costs only ~33% -
   the killer was ONE byte4 DYNAMIC select in the sub-scale extract (unpack8(word)[i&3]),
   the same death shape the Q8 chase found; byte2 [i&1] selects are fine. Respelled as
   shift + arithmetic-shift sign extension: 12.8 -> 32.9 TF/s. RULE for every future
   decode: NEVER index unpack8 of a 32-bit word dynamically - shift+mask, or byte2 [i&1].
   k6 UNPINNED: Qwen3-4B Q4_K_M pp 1626 (mode 3) -> 2669 (k4) -> 3188 (k4+k6) = +96%.
   NEXT: k5/q40 stamps (mechanical now the trap is named), then (d) driver-blocked.
   (ngfx GPU Trace, our gate loop vs their GEMM loop; counters now read UNELEVATED):
   ours tensor 44.6 / L2 54.2 / l1tex 44.9 / dram 15.3, theirs tensor 56.1 / L2 23.8 /
   l1tex 27.6 / dram 29.7 - their cm2 keeps the MMA pipe ~26% busier and streams weights
   DRAM->MMA with little cache traffic, while our staged L-tile pays L2/L1 bandwidth as
   overhead (caveat: their 58.7 MB working set cannot sit in L2, ours ~25 MB can, so the
   dram/L2 halves partly reflect working-set size; the tensor-busy delta is the honest
   headline). Their HMMA-per-FLOP is ~18% higher than ours (0.140 vs 0.112 per cycle at
   only 1.066x the FLOP rate) - unexplained, parked. Decomposition of the 14% pp window
   gap: <= ~6% per-GEMM rate + our non-GEMM dispatch chain (~4.4 ms elementwise + ~2 ms
   per-dispatch drain across 452 nodes / 367 barriers per window) - so the levers are
   epilogue fusion / barrier reduction and the k/v grid, before any cm2 chase.

12. **Arena slabs - the 4 GiB storage-range ceiling (LANDED in-arc 2026-08-06; was the
   PR gate - the MAIN FACTOR for MoltenVK/M1 enablement, where maxStorageBufferRange is far
   tighter than 4 GiB).** Walkthrough evidence that motivated it: Llama-3.1-8B Q8's fmt-0
   arena wants 7.5 GB against the device's 4294967295B maxStorageBufferRange - honest
   fail-closed decline, per-op fallback served tg 7.2 vs upstream's 49.8. As built: each
   ArenaFmt carries lazily-opened slabs capped at msr-derived blocks (both plane strides);
   `arena_place` returns `(slab << 32) | local` so the encoding rides every existing seam
   unchanged; `arena_planes(fmt, blk)` binds the tensor's slab; region/schedule metas carry
   the local half; merged k/v splits at a slab boundary; per-call arena seams cache sets per
   slab; a single tensor over one slab declines. Multi-slab correctness gated by
   `test_vulkan_arena_slabs` (forced tiny slab cap). Measured: Llama-3.1-8B Q8 resident
   across two slabs at 93.3% tg / 54.3% pp of upstream (was 14% / 3% on the fallback).
   FALLOUT FOUND AND FIXED: the resident ctx auto-negotiation OVERSUBSCRIBED - uncapped it
   armed at ctx 25590 (weights + KV = 14.5GB of the 16GB card), WDDM demotion took tg to
   3.65. The plan now accounts the driver's own scratch (`rdec_scratch_bytes` - prefill
   window dominates) and the auto arm keeps a 2GiB desktop headroom past the tier's reserve
   (`RDEC_VRAM_HEADROOM`; a pinned DASLLAMA_GPU_VRAM_MB stays the user's contract and fills
   to its brim). Uncapped now arms at ctx 16627 and holds 44.9 tg / 1967 pp - the queried
   heap "used" (2MB against a real desktop) stays untrusted.

13. **qwen2 bias arm - the cheapest family unlock (after-sweep follow-up commit, ruled
   2026-08-06: "unsupported family, easy to support").** The resident gate declines
   `attn_qkv_bias` alone for the whole qwen2 line; everything downstream is the std shape the
   ladder serves. Walkthrough row (Qwen2.5-1.5B Q8): das 35.6 tg / 465 pp on the per-op
   fallback vs upstream 202.4 / 14986 - 18% / 3%. Fix shape: per-row `+ b[row]` epilogue in
   the qkv class GEMV (bias rows ride the arena or one extra binding, offset in the push),
   same arm for q/k/v; then drop `attn_qkv_bias` from the :534 gate.

14. **The gemma cluster arms - medium family unlock covering gemma2 AND gemma3 (walkthrough
   2026-08-06: both UNSUPPORTED, per-op fallback; gemma2-2b 22.8 tg / 327 pp vs upstream
   124.1 / 9566, gemma3-1b 58.5 / 636 vs 236.2 / 21540).** The shared base: sandwich norms
   (`pre_post_norm` - an extra norm role per layer) + sliding-window alternation (a
   window-start word in decode attention - TokMeta has room). That pair alone unlocks
   gemma3; gemma2 additionally needs the two softcap clamps (attn epilogue tanh + cls
   epilogue); gemma4-dense sits on the same base (plus PLE - its own story). Biggest
   family-count unlock on the board after qwen2's one-flag bias.

15. **mm L-tile zeroes its whole dispatch on a non-128-multiple weight dim (latent, found by
   the CPU-oracle conversion).** The mode-3 `MmBatch` L-tile GEMM writes all-zeros for the
   ENTIRE dispatch when the weight dim `d` is not a multiple of 128 - and the retired
   hand-written kernel did exactly the same, which is why old-vs-new parity stayed green on
   it (zeros == zeros; the vacuous-parity failure mode the oracle conversion exists to
   catch). Serving is unaffected today because every served model dim is a 128-multiple,
   and the suite fixture pins row-edge coverage only. Repro: give the `v2` mm arm a fixture
   with `d = 192` - the whole output zeroes. Fix shape: a d-edge guard/masked tail in the
   L-tile stage (check `MmABatch`/`MmMBatch` for the same edge), then widen the fixture
   back to a non-128-multiple `d`.

16. **`run.das` cannot arm vulkan - GPU coverage needs direct dastest.** The runner spawns
   without `-load_module dasVulkan` and injects `DASLLAMA_CPU_PREFILL`, so any vulkan-armed
   arm under it silently runs CPU (the census suite's `coverage-vk` arm is the standing
   example - it must be run via direct dastest + `-load_module`). Candidate fix: teach the
   runner a GPU lane (propagate `-load_module`, drop the CPU_PREFILL injection for
   vulkan-tagged tests) or make it refuse loudly when a test declares a vulkan requirement
   it cannot satisfy.

17. **VK_EXT_pageable_device_local_memory - the missing half of the residency shield.** The
   tier chains VK_EXT_memory_priority (priority 1.0 on every device allocation, the armed
   "residency shield"); the companion extension - runtime `vkSetDeviceMemoryPriorityEXT` +
   the pageable-aware device-local signal WDDM wants - has zero references in the tree.
   Small addition: enable when present, and consider demoting cold stacks' priority instead
   of only boosting everything.
   MEASURED INCIDENT (2026-08-27, zen2): the ReBAR weight arena (mapped
   host-visible|device-local heap, "uploads write direct to VRAM") LOST WDDM residency
   mid-session - every weight-reading role fell to PCIe speed (decode 254 ms/token = 13.4
   GB/s exactly; 3B tg 105 -> 3.9, pp 6645 -> 395) while attn/rope/elementwise stayed at
   rate and the reference exe in the same minutes stayed healthy (its weights are UNMAPPED
   device-local; it also ships priority 1.0 - #17624 - and no pageable extension, no
   heartbeat). Priority 1.0 did not hold the mapped heap; `DASLLAMA_VK_REBAR=0` (staged
   uploads, unmapped device-local) restored 6584 immediately, same session. The morning
   half of the session served the mapped heap at full speed, so the hazard ARMS with some
   driver/desktop state (ngfx profiling sessions and the Parsec virtual display both ran
   that day). Design consequence to rule on: long-lived weight planes out of the mapped
   heap by default (ReBAR kept for transient staging), with this item's runtime priority
   as the second layer and a Metal-style residency heartbeat in reserve.

18. **`rsqrt` vs `1.0/sqrt` - the RMS-norm parity spelling (ledgered 2026-08-07, found by
   the cross-backend similarity audit).** The three rails spell the same inverse norm two
   ways: CPU `1.0 / sqrt(ss)` (`dasllama_math.das` rmsnorm_template), Vulkan pinned to that
   exact form for CPU==GPU bit-parity (`dasllama_vulkan_classes.das` RmsWgBase, the comment
   says so), Metal `rsqrt(..)` - and Metal's divergence SURVIVES its token-for-token decode
   parity arms, so the bit-pin may be softer than assumed. Until settled, any shared or
   reified RMS body must carry the spelling as an explicit policy knob - never silently
   unify. DECIDE BY MEASUREMENT, not taste: (a) the cost of `1.0/sqrt` vs `rsqrt` on each
   GPU rail (one op per row - expected noise, but measured is measured); (b) the cost and
   golden-output shift of switching the CPU reference itself to rsqrt form. If the CPU
   switches, both backends unify on `rsqrt` and the vulkan pin dissolves; if not, the knob
   stays and documents why.

## Sequencing

**The osmosis principle (Boris, 2026-08-06, mid-walkthrough):** older/dense families are
carried "by osmosis" - good kernels (the cm2 arc), good cache strategy, overall rail
goodness - their per-family arms are mechanical one-flag/one-cluster work, done
opportunistically, never a focus. The focus after plumbing is NEW model shapes: MoE, MTP,
hybrids - where design room actually exists. On MoE specifically the 3060-era record had
das WINNING (better MoE strategy: heat cache, expert residency, async shexp - no upstream
analog); the walkthrough shows its remaining edge is the cm2 prefill kernel alone, so
tensor kernels + our MoE strategy = the expected win condition on coopmat2 hardware too.

After the reorg arc lands (this doc is a product of it - see ARCHITECTURE.md's extraction
ledger). First measurable milestone: zen2 resident decode/prefill numbers vs the cooperative
tier on the same models, which also decides how hard items 3-4 are pushed. Item 1 (shapes
module) is independent and can land any time - it is pure structure.

19. **The dry bake cannot see the workgroup cap.** `vk_moe_init`'s dry path returns before
    `vkGetPhysicalDeviceProperties`, so `g_gpu.max_wg_bytes` stays 0 and `DlimVulkanConfig`
    has no twin field - an offline-baked plan cannot record which class kernels the target
    device will decline (`vkd_wg_fits` declines everything under a 0 cap if a generated
    `ensure_*` runs dry). The live path declines residency in `vk_rdec_prepare`; the plan
    side needs `max_wg_bytes` in the probed config + the dlim identity, mirroring
    `max_storage_range`.

20. **A coopmat2-class analogue for the tmm2d family.** dasMetal exposes a tensor-op family
    the kernel classes call directly - `tmm2d_tg_begin` / `tmm2d_tg_step` / `tmm2d_tg_store`
    plus the per-format decode arms (`tmm2d_q8_f32`, `tmm2d_q8u_f32`, `tmm2d_f16w_f32`,
    `tmm2d_f32_bf16_f32`, ...), all access-classified in `dasllama_kernel_access.das`. Vulkan
    has no twin: the cm2 work (item 11) is a prefill GEMM arm on
    `VK_NV_cooperative_matrix2`, not a builtin family a kernel class composes. Decide whether
    the tmm2d shape maps onto coopmat2 tiles, and if it does, expose it as the same builtin
    family on the SPIR-V side so one class body serves both backends - rather than a second,
    Vulkan-only GEMM dialect beside cm2.

21. **Metal-side grid-literal validation.** `[vk_dispatch]` rejects a divide-form `grid`
    whose referenced name is not a declared `int64` param, and says why in the message (the
    generated ceil-div math is int64, and daslang has no promotion). `[metal_dispatch]`
    checks only that a grid dim carries at most one `/`, so the same mistake on Metal
    surfaces as an infer error inside generated code instead of a lens diagnostic. Both
    lenses already call the shared `mk_grid_dim` in `dasllama_kernel_access.das` - move the
    check there too, so it fires once for both.

22. **Metal kernel-corpus inheritance dedup.** The Vulkan classes factor their families into
    base + leaves - `KqGemvBase` with `KqGemvK4/Q40/K5/K6`, `KqBatchBase` with its batch
    leaves, `MoeCmBase` with the cm/mm leaves, `DnScanBase` with `DnScanP1/P2`. Metal does
    this in places (`MetalMoeMulMmK6` and its siblings sit on `MetalMoeMulMmBase`), but its
    GEMV families do not: `MetalKqGemvK4/K5/K5C/K6` and
    `MetalMoeGemvQ8/K4/K5/K6/Mx4/Q51` are flat classes repeating the identical
    `x`/`y`/`ndim`/`ddim` binding block, differing only in the weight-plane views and the
    decode. Give those two families a base the way `MetalMoeMulMmBase` already does, so a
    binding or epilogue fix lands once per family instead of once per variant.

23. **Command-chain overlap + record-once for the re-recording tiers (ruled 2026-08-10,
    post-#3681; parked behind the jit-infra work then - this entry is the durable copy of
    that ruling).** The resident DENSE decode ladder already records once per
    set_layer/set_cls epoch (`rd_record_token`; `--rerecord-ab` prices the re-encode
    delta - it is why decode wins tg). What still re-records: PREFILL (the full window
    chain, one submit per window) and the chunked/MoE `g_gpu` tier
    (`ffn_gemv_prep`/qkv per token). Upstream re-records everything every graph evaluation
    but overlaps CPU recording with GPU execution via incremental submits every
    ~200 GFLOP. The plan: (a) prefill overlap - split the window chain into a few
    submits, fence at the end, pipeline across windows (also hides `embed_row`);
    (b) MoE-tier record-once - routing already rides the `fill_stack_sched_rows`
    meta-buffer CONTENT; the two leaks are the stack binding (`find_stack` per token ->
    bind the slab union / sched carries the stack id) and the streamed-miss arm (stays a
    dynamic prelude, the slow path); CPU top-k is the natural chain split; MoE prefill
    grids vary per window -> overlap only there.
    ACCEPTANCE (Boris, 2026-08-10): the O0-vs-O3 pp512 delta IS the CPU-on-critical-path
    share (measured then on tinyllama: 18200.64 O3 vs 14785.32 O0 = -18.8%, ~6.5 ms
    CPU/window; tg free at O0 - record-once decode has no per-token CPU); overlap
    succeeds when the two rows CONVERGE. Two bench rows, no profiler, drift-cancelling.
    SIZING DATUM (2026-08-27, llama-3.2-3B Q8 GPU_PROF): the 3B prefill window is ~99%
    GPU-busy - on small dense models the lever is per-GEMM kernel rate (item 11), not
    overlap; overlap pays where per-token CPU still rides the chain (the MoE/chunked
    tier, long multi-window prefill, and the O0-class boxes the acceptance test prices).
    SHIPPED 2026-08-27 (both halves measured, REBAR=0 protocol, debug-jit):
    (a) chunked submits (`DASLLAMA_VK_OVERLAP`, 1,2,4,8-layer ramp, cmd ring, one fence on
    the last chunk) - tinyllama pp 18074 -> 18463 (+2.2%), 3B +0.7%: exactly the record
    wall, as the GPU_PROF datum predicted; the O0/O3 pair had read 34%/15% CPU share but
    most of that is O0-inflated record cost.
    (b) device-side embed gather (`DASLLAMA_VK_GPU_EMBED`, the ids-form prefill seam +
    engine embed gate with CPU backfill; the q8 arm gathers from the tied cls plane (a
    tied Q8 table only), the f32 arm uploads the raw fblob table, 512 MB cap +
    budget-guarded) - 3B pp 6664 -> 7062 (+6.0%), tinyllama 18158 -> 19527 (+6.8%);
    tg untouched (a one-run tinyllama tg dip re-measured as box state). Footprint of the
    trade, by construction: a tied q8 model places nothing (the cls plane is reused - the 3B
    case); a raw-f32 table costs vocab x dim x 4 bytes of device memory (tinyllama: 32000 x
    2048 x 4 = 262 MB) and the residency plan counts it before it picks the context cap, so
    a box that cannot afford it keeps the CPU embed rather than a shorter context. Decision:
    taken - the +6% pp buys the table on every box the plan clears.
    (c) the prefill batch ar+rq fusion (`ClsArAddRmsRqB`, one wg per row, verbatim
    reduce/amax fold - bit-exact vs the split pair by suite gate; rides `DASLLAMA_VK_FUSE`;
    both sites, the last layer keeps split ar for fin_rq's xb) - tinyllama pp 19527 ->
    19989 (+2.4%), 3B a wash (its elementwise share was already small).
    DAY-END STANDINGS vs the reference exe b10659 (same box, back-to-back): tinyllama
    19989 +/- 60 pp / 291.9 tg vs 20277 +/- 260 / 291.4 = **98.6% pp (inside their row
    noise), tg AT PAR** - the llama family is effectively closed on this box; 3B
    7071 +/- 84 / 105.4 vs 7691 / 110.0 = 91.9% pp / 95.8% tg - the 3B residual is
    per-GEMM rate (this item's (e)/(f) counters), not chain shape. Still-serial per
    window: cos rows + their upload, prep (~0.45 ms total - the last ~1.4% of tinyllama).

24. **The cm2 tiles stamp from one class template (ruled 2026-08-28 at the vkclass PR round:
    a follow-up PR, not this one).** `Q8Cm2LBatch`/`Q8Cm2MBatch`, `K4Cm2LBatch`/`K4Cm2MBatch`
    and `K6Cm2LBatch`/`K6Cm2MBatch` are six hand-stamped bodies over two axes (tile width
    128/256, decode format) - nine since the MoE s tiles (`*Cm2SBatch`, 32-row columns with
    the semi-fast partial-column path) joined them, generated from the m bodies by a script;
    `REVIEW_GPU.md`'s twin rule asks for one `class template` with a
    `@template_constant` for the width, typedefs for the block/coopmat types, and a
    `def override decode_*` per format - the shape `harness/vk_gemm_probe.das`'s `K6PxBase`
    already proves. Gate: the nine oracle cells in `tests/test_vulkan_kernels.das` stay 0-off,
    the probe's l/m rows stay within noise. The k5/q40 stamps (item 11's NEXT) land on the
    template, not as more copies. Measured 2026-08-30 on the 1B (`lcpp_bench`, 5060 Ti): the
    formats with no cm2 tile prefill at 5161 (iq4xs) and 5174 (k3) t/s against k4's 13144 on
    the same box (llama.cpp 17060 / 17509 / 19719) while their decode sits at parity, so the
    template is also the lever that puts every new format on the tile path; k4's own 0.67x is
    the 1B-shape tier gap, a separate item. DONE 2026-08-30 (the iquant arc): `KqCm2BatchT`
    stamps all nine originals (suite + the Q4_K_M e2e row within noise), and k5/q40/iq4xs/k3
    joined as format templates - twelve more stamps, iq4xs pp512 15334 (0.90x llama.cpp,
    above the k4 control), k3 14031 (0.80x). Item 11's k5/q40 stamps landed with it.

25. **Try `VK_NV_cooperative_vector` for decode GEMV on real hardware (Boris, 2026-08-28).**
    cm2 has no matrix-vector op - its seven feature bits are all tile-shaped, minimum tile 16 -
    but the separate cooperative-vector extension (`OpCooperativeVectorMatrixMulNV` and kin,
    vendored in `modules/dasVulkan` headers/bindings, absent from the SPIR-V emitter, unused by
    upstream's matvec shaders too) is the inference matvec path. Every decode matmul
    on the tier today is an sdot4 subgroup kernel (`Q8Gemv`, `KqGemvK4/Q40/K5/K6`), sitting at
    the bandwidth ceiling by the bandwidth oracle. Boris's ruling: the bandwidth-only oracle
    has lied too many times - measure on the hardware, not the model. The arm: (a) emitter
    support for the cooperative-vector ops (`spirv_builtins.das` markers + `spirv_emit.das`
    lowering, the device feature bit in `vulkan_boost`'s creator family), (b) one probe kernel
    in `harness/vk_gemm_probe.das` at the decode shapes (3B gate/down rows, the 30B expert
    rows), quant weights decoded to the vector op's f16/int8 forms, (c) a back-to-back tg pair
    against the sdot4 GEMV on the 5060 Ti. Driver 610.74 exposes the extension or it does not -
    the probe says which. Keep or kill on the pair, never on the oracle.

26. **DONE 2026-08-29 - the whole-token decode span** (`ARCHITECTURE_GPU_VULKAN_DECODE.md`
    sec.2.2t): the resident suffix as one submit per token; router + top-k on the device write the
    FFN chain's metas. The board row lives in the PR that landed it; parity 40/40 on the 30B.

27. **VRAM accounting for the decode-era scratch.** `carved_budget` carves the stream slots and
    the decode mirrors (`set_moe_gpu_dat_need`); the prefill scratch (batch state ~250 MB, the
    combine planes ~135 MB, the xf plane 64 MB, at ~90 MB, hq/hs 42 MB) still comes out of the
    desktop reserve after placement, so the tier sits one allocation from the eviction cliff
    (the 741 us FFN submit). Carve them too, and size the mirror cap from the session's context
    (`DAT_MIRROR_ROWS` 2048 is 402 MB on the 30B; 1024 frees a half layer). Every layer moved
    from the CPU streamed set to the device is ~0.43 ms/token on the 30B (the streamed layers'
    CPU FFN is 6.9 ms of the 18 ms token at the DDR4 wall).

28. **Model-free coverage the decode arc still owes (the TDD audit's untested set).** The device
    half is pinned (`test_vulkan_dec_tail`, `test_vulkan_moe_cm2`, `test_vulkan_kernels`); these
    arms are not: (a) the cm2 expert chain's multi-chunk loop and `MoeGatherF16`'s window guard -
    `ffn_cm2_chunk_rows(2048, 512)` is 8064 rows and the largest cell is 600, so nothing chunks;
    a cell past the cap (or a chunk-cap knob for tests) makes the guard load-bearing; (b) the
    top-k kernel's `norm == 0` and `wscale` arms and its lowest-index tie rule - the span cell
    records once per process, so a variant needs its own quad pair; (c) a `qk_norm` quad
    (`set_qkn_rope_f16_cls`) in the block cell; (d) the pure host tables: `kq_bytes_per_weight`,
    the split's LPT claim loop (extract it to a function first), `add_batch`; (e) the composed
    model path - GPU prefill fills the mirrors, one claim per prefill, SERVE with no hydrate,
    tail, span - belongs in `test_parity.das` as an A/B arm over `DASLLAMA_GPU_DEC_SPAN` /
    `_DEC_TAIL` / `_MOE_SPLIT` (today the session's 40/40 parity is the only evidence).
    Lint candidate from (a): a kernel window pair (`r0`/`r1`) whose only test caller passes the
    full range is a guard that ships undistinguished.

29. **The split's waste, both halves.** The CPU tail gathers, activates and requantizes ALL `nk`
    bucket rows (`moe_gather_rows(s, 0, nk)`, `gate_batch(.., nk)`, `requant_rows_q8k_bs(.., nk)`)
    while only its tail regions reach the GEMMs; the GPU head gathers every row and runs the
    activation over the whole window while only the head regions' GEMMs run. Neither is wrong
    (the combine skips zero-weight slots) but each caps the split's win and biases its cost model,
    which also ignores that floor. Scope both to their share, then re-fit the three constants; the
    LPT loop can also take one post-loop step (`max(t_gpu(i+1), t_cpu(i+1)) < t_cpu(i)`).

30. **A paranoid-vs-normal hazard cell.** The span and the tail carry six hand-declared fences on
    the edges the derived masks cannot see; `DASLLAMA_VK_HAZARD_PARANOID=1` barriers before every
    node. A cell that runs the span twice, once under each, and compares rows is the one cheap
    check that separates "the chain's math is right" from "the fences are complete" - it needs the
    knob readable per run (today it is read once at init).

31. **`feint` is a zero-assertion pass.** `tests/test_vulkan_kernels.das` carries ~70 `feint(..)`
    + `return` gates from before the `t |> skip` rule; the s-tile cell and the two new files now
    skip. Sweep the family in one change.

32. **A top-k fixture over the dark arms.** `topk_cls` runs in the suite at one shape (4 experts,
    k = 2, one subgroup live, renorm on, scale 1). A fixture driving the kernel alone against
    `moe_select_core` over several `ne` / `k` shapes, exact ties, `norm == 0` and `wscale != 1`
    covers the cross-subgroup argmax, the tie rule and both weight arms (the Metal twin has one:
    `test_metal_prefill_kernels.das`'s select cell).

33. **The vulkan prefill override never fills `x_b`, so a whole-plane consumer reads garbage.**
    `rdec_prefill` takes `x_batch` non-`var` (`dasllama/dasllama_gpu_tier.das`, the
    `rdec_prefill` typedef) and the override returns true without writing any residual row
    (`dasllama/dasllama_gpu_resident.das`, `vulkan_resident_prefill`), while the
    `PrefillOverrideFn` contract says "x_b holds the final residual stream" on true. A
    `/v1/embeddings` turn on a vulkan-resident box pools token embeddings - or, when the
    device embed-gather gate armed (`g_rdec_emb_gpu`), the `resize_no_init` bytes of
    `forward_prefill_alloc` - and `Session.keep_hidden` (the Metal narrowing's opt-out) is
    never consulted. Done = the override either writes the final residual stream into `x_b`
    (readback or device fill) or declines when the session demands the plane
    (`keep_hidden`), plus an override-capability row so a whole-plane consumer can test for
    it; a cell that embeds through the vulkan override and compares against the CPU pool
    proves it.

34. **The pp512 gap on every sb format (0.67x-0.90x of llama.cpp on 1B shapes) - not one band, and
    not the tile.** The board spreads 1.34x (IQ4_XS 0.90, Q2_K 0.87, Q3_K 0.80, IQ4_NL 0.78, IQ2_XS
    0.77, IQ3_S/IQ2_S 0.70, IQ3_XXS 0.69, Q4_K 0.67) while the Q8 rows sit at 1.00x+, so the format
    decode is in it. The cm2 tile itself matches llama.cpp's design point for point - geometry,
    workgroup, decode-in-load callback, one coopMatMulAdd per k-step, no k-loop barriers, the same
    split-k heuristic (`plans/kernel_parity_research_vk.md`). The real differences: llama.cpp hoists
    Q4_K/Q5_K scales into shared memory as ready (d,m) pairs where ours re-extracts from a second
    SSBO with a variable shift per decoded element, and its scales sit in the quant block where ours
    ride a separate plane. Order of work: (1) split the end-to-end budget - `GGML_VK_PERF_LOGGER=1`
    per-shape MUL_MAT rows against our `pfq_ts` roles decide whether the GEMM is the 30% at all (the
    chain carries 367 barriers per window); (2) the scale hoist; (3) interleave the scale plane into
    the quant block; (4) pad N to the tile width. `harness/vk_gemm_probe.das` already carries the
    isolation arms (`ref` = llama.cpp's own coopmat2 blob in our harness, `k6x flat` = compose
    without scale reads). Boris 2026-08-30: this one bothers him at 0.7. Plan: `plans/kernel_parity_pass.md`.

35. **The grid-format GEMV workgroup re-stage is a fixed per-workgroup cost - amplified on
    small models.** Every u64-grid gemv (iq2s 8 KB, iq2xs 4 KB) stages the codebook into
    workgroup memory per 2-row workgroup, so tg pays a fixed latency the row length must
    amortize. On the 3B i1 vehicle iq2s tg landed 0.81x llama.cpp (~350 GB/s effective);
    the 1B IQ2_XS vehicle lands 0.54x (188.7 vs 349.9 t/s = ~84 GB/s effective - latency-
    bound, while its cm2 pp512 sits at a healthy 0.77x). Levers, in likely order: persist
    the staged grid across the row loop (one stage per SM residency, not per workgroup),
    widen rows-per-workgroup for grid formats, or fold the grid into a device-buffer read
    the L2 serves. Done = 1B-class grid-format tg within the k-format band on the same
    vehicle.

36. **Hand-laid four-wide decode twins for iq3s, iq2s and iq2xxs.** The emitter's synthesized
    twin (four calls of the scalar decode) loses on three formats (`harness/vk_gemm_probe.das --
    cm2:<fmt>` on the RTX 5060 Ti, driver 616.56, `DASLLAMA_VK_DECVEC` 1 vs 0: iq3s -4..9%,
    iq2s -1..11%, iq2xxs -13..17%, while iq2xs gains 15-31% and iq3xxs reads flat), so those
    three override the tile template's `DECVEC` axis off. One grid word carries four (iq3s) to
    eight (iq2xxs) weights, which is exactly what a hand-written `half4` body shares across its
    lanes: one grid read, one sign extraction, four scaled results. Lever: a second
    `[spirv_decode]` method returning `half4` in `coopmatLoadTensorDecode`'s tenth slot, the
    scalar body kept as the fallback; gate the tile oracle and the probe row against the scalar
    rate. Done = the three formats' probe rows at or above their scalar rate with the twin on.
    Related: the e2e A/B on the same box (`benchmarks/lcpp_bench.das`, ten reps) spreads 3-11%
    per row, so the probe row is the verdict and the e2e is the confirmation, never the reverse.
    CLOSED (2026-09-02, the hand-laid twins commit on bbatkin/vk-mirror-bench): every kq format
    carries a `decode_v4` (the template's `DECV4` axis), not just the three; the grid formats'
    twins do one grid lookup per four elements and beat the scalar arm by 30-65% at the tile
    (iq3s 43-48 vs 29-30 TF/s at the gate shape, iq2s 49-50 vs 30-32, iq2xxs 44-46 vs 34-35);
    the `DECVEC` opt-outs are gone. Rows in `plans/kernel_parity_pass.md`.

37. **Device embed gather over a kq tied plane.** `vulkan_embed_gpu_gate` admits a model only
    when `rdec_set_emb` placed a q8 tied plane or the f32 table fit under `RDEC_EMB_F32_CAP`;
    a kq tied plane (Llama-3.2-1B Q4_K_M ties a q6_K classifier) keeps the CPU embed, which the
    host profile put at 2.2 ms of a 25.7 ms window before the embed went parallel (2026-09-02,
    `JOBQUE_PROFILING=1 lcpp_bench --prof`). llama.cpp gathers on the device (GET_ROWS, 6 us)
    and skips the 4 MB x upload. Lever: an `emb_gather_kq_cls` per format reading the arena's
    superblock planes with the cm2 decode's `(blk, bc, cib)` math (the tile classes own those
    methods; a gather class needs the same members or a shared free decode), then
    `rdec_set_emb` for `cls_kq`. Done = the embed bucket gone and the x upload out of prep on
    the Q4_K_M window, parity pregate token-for-token on Q8_0 and a kq vehicle.
