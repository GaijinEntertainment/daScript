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

## The gap list (from the 2026-07-29 census's Metal<->Vulkan parity map)

Ordered roughly by user-visible value; re-rank against zen2 measurements before starting.

1. **Servability gate module** - the single biggest asymmetry: Vulkan has NO
   `dasllama_metal_shapes` analog (no decline enum, no needs bitfield, no servable predicate;
   `resident_upload` rejects 12 family features ad hoc, by name). Build
   `dasllama_vulkan_shapes` on the Metal pattern: portable, no `vulkan` require, feeds the
   same decline-reason reporting.
2. **Family coverage in the resident driver** - the DECODE half of the deltanet hybrid ladder
   landed (`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2v): the
   token command carries recurrent layers (the fused step over per-layer state slots), gated
   attention and partial rotary, the K/V mirror has one slot per attention layer, and the nextn
   block no longer declines. Qwen3.5-9B UD-Q5_K_XL decodes resident at 49.7 tg (0.88x of
   upstream; was 11.1 on the per-op rails). The PREFILL half followed: the window chain carries
   the recurrent block (conv + chunked scan on the layer's device state), gated attention and
   partial rotary on the batch kernels, and hands the device state to the session for the decode
   (`ARCHITECTURE_GPU_VULKAN.md` sec.2.2ad, `_DECODE.md` sec.2.2v; gate
   `tests/test_gpu_resident_hybrid.das`, one- and two-window cells). Every figure in this item:
   `benchmarks/lcpp_bench.das -m Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf -r 3` (`-p 512 -n 128`) through
   `bin/Release/daslang.exe -jit` under `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1
   DAS_JOBQUE_THREADS=16` (`DASLLAMA_GPU_PROF=1` for the role tables) on the 5060 Ti box (Windows
   11), the upstream control `llama-bench -ngl 99` on the same day's build-vulkan (b10660). The
   9B UD file on the resident driver: pp512 1365 (was 95.7; upstream 2527 - 0.54x), tg128 53.3
   (upstream 56.7, 0.94x).
   WHERE THE GAPS ARE (DASLLAMA_GPU_PROF=1 role tables, 9/5, 5060 Ti; `vk_rdpf attn`/`dn` and
   `vk_rdec ... dn avg/token` are hybrid-aware now):
   - pp512, one 512-row window = 349 ms GPU (upstream ~203): the deltanet two-phase scan was
     141 ms (scan2 97 + scan1 45; 24 layers, ~5.9 ms/layer - P2 ran ONE workgroup per head, 16 of
     36 SMs, and the staged GEMM did two shared loads per FMA with its C tile in a dynamically
     indexed private array). DONE 9/5 (same session): phase 2 per (head, column slice) with a phase
     3 out-norm (`DN_NSP` = 4) and a 2x4 register-tiled GEMM (bit-exact) took the scan to 63 ms
     (scan1 27 + scan2 36), the window to 291 ms, pp512 to 1710 (0.68x). DONE 9/5 (evening): the
     chunked form is gone - `dn_scan_cls` is upstream's shape, the plain per-token recurrence
     with the state in registers (one 32-lane subgroup per (head, column group), 16 rows per lane,
     the token loop inside the kernel): scan 17.7 ms + out-norm 1.2 (was 63), and the f32
     beta/alpha rows as a 16-position tile GEMM (`dn_ba_cls`, P2): 11.7 ms (was 15.5). Window
     221 ms, pp512 2010 (0.80x of 2527), tg128 52.6 (0.93x). DONE 9/5 (later): the host seams
     (window-0 fills in the command, the slot handed to the session - item (e)) took pp512 to
     2282; the dn qkv/z/out GEMMs on the f16-fed cm2 tiles (P4: qkv 21.0 -> 15.4 ms, z 10.2 ->
     7.1, out 10.4 -> 9.2, two f16 converts +0.5) to 2361 (0.93x); the scan's k/q rows staged in
     shared per four-subgroup workgroup (17.5 -> 15.3; staging or the scalars a token ahead measured
     no faster - the dependent chain is what remains) to 2387; the beta/alpha rows f16 on the
     device and the tile GEMM regridded to 16-position x 16-output tiles (9.4 -> 6.1; the bytes
     alone moved nothing, the grid did) to 2439 (0.965x), tg128 54.2 (0.956x), window 200 ms. DONE
     9/6 (P1): the cm2 flash tile stamps at head 256 with a gated epilogue (`fa_cm2_h256_g[_f16]_cls`,
     Br 64 / Bc 32, the 64x256 / 256x32 / 32x256 workgroup shapes in dasSpirv): attention 21.05 ->
     1.29 ms over the 8 layers, window 183 ms, pp512 2660 +- 3 (two r3 pairs: 2659 / 2661) = 1.05x
     of llama.cpp's 2527 - PREFILL PARITY; tg128 53.5 (0.94x; the decode path is untouched, the 0.7
     drift from 54.2 is the box). DONE 9/6 (scan ILP): a lane keeps 16 rows of TWO adjacent columns
     (`dn_scan_cls`, 128 workgroups on the 9B instead of 256; the staged k/q rows feed both
     columns, the two chains interleave): scan 17.0 -> 10.9 ms and the window 200 -> 189 ms on the
     same box state (a same-day llama-bench control is the only valid baseline - the box read ~6%
     slower on every row today: llama.cpp 2488 / 53.1 against its 9/5 2527 / 56.7); pp512 2586
     (1.04x of the same-day 2488), tg128 49.9 (0.94x). What is left in the window, in milliseconds:
     the FFN GEMMs ~85 across both heads (cm2 tiles, at par); the dn GEMMs 29.8; scan 10.9
     (four columns per lane measured 15.3 - 64 workgroups starve the 36 SMs; two columns is the
     shape); ba 5.8; conv 5.3, cls 2, attention 1.3, add+rms/act/converts ~9. DONE 9/6 (D1, the
     k-native deltanet planes): the loader keeps a dense hybrid's qkv/z planes in their file
     formats when the whole-model driver will be attempted (`gpu_want_dn_native`, also an image
     identity bit), the plan tallies, reserves and places the triple by format, the decode's kq
     GEMVs and the prefill's k5/k6 cm2 tiles serve them; a K-quant OUT plane still declines (the
     step's o row is Q8_0), and a layer with a q8 beta/alpha arm beside kq qkv/z re-requantizes
     the row Q8_0 before those GEMVs (one x feed, two forms). 9B UD same-day: tg128 50.1 -> 51.8
     (0.965x of the 53.7 control; decode qkv 2.21 -> 1.70 ms, z 1.13 -> 0.95; ctx
     125655 -> 136087), pp512 2608 -> 2526 (1.02x of 2484): the k5/k6 cm2 tiles run the same
     GEMMs slower than the q8 tile (window qkv 14.8 -> 17.7 ms, z 6.3 -> 8.3) - the kq cm2 decode
     callbacks are the next prefill lever (item 11's K-quant tail), or a q8 twin of the two
     planes for the prefill alone when VRAM allows.
   - tg128 = 18.9 ms GPU/token (host wall 19.4; upstream 17.6): every GEMV role sits at 360-420
     GB/s (bandwidth-bound, at par per byte); the bytes are the gap: the loader's Q8_0 transcode of
     the deltanet qkv (Q5_K in the file, 24 x 33.5M params) and z (Q6_K) planes reads ~400 MB more
     per token than upstream = ~1.0 of the 1.1 ms gap - lever D1 = the k-native dn planes
     (`PERF_LEDGER.md` k4 dn planes; the resident GEMVs already serve every KqFmt); the rest is
     small-dispatch latency: the f32 beta/alpha pair 0.39 ms (2 dispatches x 24 layers at 64 GB/s -
     D2: one dispatch, or f16/q8 rows), add+rms pairs 0.68 ms + requants 0.16 (the fused ar+rq twin
     is off on a hybrid because the f32 GEMVs read xb - D3).
   STILL OPEN in this item:
   (b) the other families Metal serves - MoE (Wave C), gemma4 (PLE, sandwich norms), gpt-oss
   (sinks, swiglu_oai) - stay per-op; (c) the decode-role profiler (`rdq_sample`) has no
   per-role table for the hybrid stamp count - it reports the whole span only, and the prefill's
   per-role table labels a recurrent layer's stamps with the attention head's role names; (d) DONE
   9/5 - the f32 arm is `dn_ba_cls`, gated by `test_vkd_dn_ba` (partial and full tiles) and
   `test_vkd_dn_9b_ba` (the 9B geometry); the scan has `test_vkd_dn_9b_scan` beside the family
   cell, both against the sequential CPU rule; (e) DONE 9/5 - the prefill hands each slot to the
   session (valid, dirty, owned) instead of flushing it home and re-uploading, and window 0 zeroes
   the slots with fills inside the window command: 48 host round trips gone per prefill.
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
   decode: NEVER index unpack8 of a 32-bit word dynamically - shift+mask, or byte2 [i&1]
   (and the byte2 select costs against the lane shift too: item 36's 2026-09-09 status).
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

17. **The residency plan off Windows still guesses.** On Windows the plan sizes against the OS
   video memory budget (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md` sec.2.2n, the `os_video_memory`
   boost helper); elsewhere the 27% headroom share stands, measured on the tagged arm and never
   re-measured. Two tails: (a) Linux NVIDIA fails an allocation past the card with
   `OUT_OF_DEVICE_MEMORY`, which `vk_check` turns into a process death - the arena reserve and
   the mirror allocation should catch it and retry the plan at a shorter context, the remedy
   the plan already carries; (b) AMD's kernel driver evicts buffers the way WDDM does, so the
   Mesa `heapBudget` (kernel accounting, system-wide) should feed the plan the way the Windows
   budget does, and the demotion check needs an AMD reading. The 8 GB-card ladder that set the
   fixed 2 GiB term is also tagged-arm data. The boost creators enable both memory extensions
   whenever the card reports them; `vkSetDeviceMemoryPriorityEXT` stays unreferenced.
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
    split-k heuristic. The real differences: llama.cpp hoists
    Q4_K/Q5_K scales into shared memory as ready (d,m) pairs where ours re-extracts from a second
    SSBO with a variable shift per decoded element, and its scales sit in the quant block where ours
    ride a separate plane. Order of work: (1) split the end-to-end budget - `GGML_VK_PERF_LOGGER=1`
    per-shape MUL_MAT rows against our `pfq_ts` roles decide whether the GEMM is the 30% at all (the
    chain carries 367 barriers per window); (2) the scale hoist; (3) interleave the scale plane into
    the quant block; (4) pad N to the tile width. `harness/vk_gemm_probe.das` already carries the
    isolation arms (`ref` = llama.cpp's own coopmat2 blob in our harness, `k6x flat` = compose
    without scale reads). Boris 2026-08-30: this one bothers him at 0.7.
    CLOSED at the debug rig (2026-09-03, after #3926: the split-k group, the hand-laid twins, the
    32-row last layer, the parallel embed; `lcpp_bench --for-debug-purposes --plen 512 --ngen 0
    --reps 12` on the RTX 5060 Ti bracketed by `llama-bench -p 512 -n 0 -r 6` of the vector build;
    the board rows stay OWED in `PERF_LEDGER.md`): nine of
    ten 1B vehicles at or past llama.cpp on pp512, IQ2_XXS at 0.98. What remains is per tile, not per
    board. Read against llama.cpp's per-role windows (`GGML_VK_PERF_LOGGER=1 llama-bench` beside
    `DASLLAMA_GPU_PROF=1 lcpp_bench`): the iq2xxs
    gate/up tile at 1.15 of llama.cpp's rate (its grid decode still shows where theirs is hidden),
    k3 and iq3s gate/up at 1.05-1.09, down at 1.04-1.14 on the 2048-wide shape. The work for
    those is the memo's delta 1 (scale hoist), delta 2 (scale interleave) and delta 8 (codebook
    and prologue) rows, each behind its own probe A/B.

35. **The grid-format tiles still stage their codebooks from the constant composite.** The
    GEMV half is done: the five grid GEMVs stage from the family's grid buffer
    (`ARCHITECTURE_GPU_VULKAN.md` sec.2.2ab) and sit in the k-format band (iq2s 84 -> 388
    GB/s at the 27B gate shape; the serial constant read, not the stage's bytes, was the
    cost). `KqBatch*` and the cm2 `IQ*GRID` axes still stage through the `*_grid_word`
    accessors - amortized over a tile, so no measured loss yet; the buffer form is there
    when a probe row says otherwise. The other lever stands: rows-per-subgroup > 1 for the
    small-d roles (a 27B k/v projection dispatches 1024 two-row workgroups), the shell's
    `rg` loop striding the grid, so a stage covers more rows. Done = a small-d grid GEMV
    row within the k-format band in `harness/vk_gemv_probe.das`.

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
    the `DECVEC` opt-outs are gone.
    2026-09-09 (the MoE arc): the byte selects of those decodes respelled as lane shifts. A
    `unpack8(lane)[i & 1]` byte2 select reads the same 16-bit lane as `(lane >> ((i & 1) * 8))
    & 0xFF` but costs against it, and the IQ2_XXS / IQ3_XXS sign index was built from two
    selected bytes where the aux32 word assembled from its two lanes and shifted serves.
    `harness/vk_gemm_probe.das -- moe:<fmt>` (the 30B expert schedule: 128 buckets of 32
    rows, gate/up d 768 K 2048, per plane, two alternating output planes under fresh hazards,
    RTX 5060 Ti): the shift form iq2xxs 724 us, iq3xxs 569, iq3s 766, iq2s 736, against
    llama.cpp's cm2 `mul_mat_id` tile at 754 / 788 / 870 / 797 on the same uniform buckets (its
    `test-backend-ops perf`), the select form 1.28x / 1.24x / 1.49x / 1.08x of those times;
    iq2xs 651, iq4xs 632, k4 746 and q8 885 already sat under its 744 / 959 / 1009 / 998. The rule in
    `ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2k. The selects left: k5's qh byte in its twin,
    the k3 and K-quant scalar decodes (the edge path and the scalar-callback arm) - the same
    lever, unmeasured.

37. **Device embed gather over a kq tied plane.** `vulkan_embed_gpu_gate` admits a model only
    when `rdec_set_emb` placed a q8 tied plane or the f32 table fit under `RDEC_EMB_F32_CAP`;
    a kq tied plane (Llama-3.2-1B Q4_K_M ties a q6_K classifier) keeps the CPU embed, which the
    host profile put at 2.2 ms of a 25.7 ms window before the embed went parallel (2026-09-02,
    `JOBQUE_PROFILING=1 lcpp_bench --prof`). llama.cpp gathers on the device (GET_ROWS, 6 us in
    its `GGML_VK_PERF_LOGGER=1` op rows) and skips the 4 MB x upload. Lever: an
    `emb_gather_kq_cls` per format reading the arena's
    superblock planes with the cm2 decode's `(blk, bc, cib)` math (the tile classes own those
    methods; a gather class needs the same members or a shared free decode), then
    `rdec_set_emb` for `cls_kq`. Done = the embed bucket gone and the x upload out of prep on
    the Q4_K_M window, parity pregate token-for-token on Q8_0 and a kq vehicle.

38. **Per-session device slots for the deltanet decode step.** The step keeps one resident
    copy of each recurrent layer's state, owned by one session at a time
    (`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2u): two streams decoding turn about on the
    per-op tier pay a flush and a cold upload per recurrent layer per switch - correct since
    2026-09-03 (a user's two concurrent requests on Qwen3.5-9B-MTP had read each other's
    state), slow by construction. Lever: N state slots per layer keyed by owner, the
    scheduler's rows mapped to slots, the fused step kernel taking a slot index - the same
    shape the batched deltanet step needs anyway. Done = two streams' tg on the 0.8B within
    the single-stream rate's band, `test_scheduler_batching_deltanet_gpu` green.

39. **CLOSED - `tests/test_scheduler.das` under the armed tier.** Two causes, both landed with
    the session-ownership fix. The order dependence (a model loaded before the deltanet cell
    moved its first decode step; the MTP cell red behind the SmolLM cells) was the model-swap
    defect: a deleted model's device state stayed installed and the next load uploaded beside
    it, so the offset-keyed stack lookup served the earlier model's planes - the upload rail
    now drops a still-installed model first (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md` 2.2o,
    `tests/test_gpu_model_swap.das`). The SmolLM reds (chunk sizes, batching, evict, media
    splice, mrope) were cross-lane numerics, not defects: the resident driver's device prefill,
    its batch decode and the CPU prefill agree on every argmax over a few steps and drift by
    about a logit on a scale of twenty, enough for a 135M model to flip a near-tie; those cells
    claim bit-exact mechanics, so they pin the CPU lane (`moe_gpu_drop_model` after the load).
    Done = the file green, twice in a row, under `DASLLAMA_GPU=1` on the 5060 Ti, the cell
    order free.

40. **The 2026-09-05 hang hunt's residue (the Khronos layer under GPU-assisted validation, safe
    mode, robustness OFF, on the 9B pp512 window).** The hang itself was the emitter's eager
    `?:` (`modules/dasSpirv/ARCHITECTURE.md` sec.3.4, "Operand laziness follows the language"):
    `qk_rms_cls` read `krows` at every q-row offset, 12.6 MB past a 4 MiB binding, and faulted
    the card once the overshoot left mapped VRAM (512 rows dead, 128 rows fine, the 0.8B fine,
    the kernel-unit cells fine - a CPU oracle cannot see an out-of-range READ). Three things the
    same run reported that are not yet fixed: (a) `q8_batch_mm_a_cls`'s
    `OpCooperativeMatrixLoadKHR` carries a Stride the spec wants 16-byte aligned (VUID
    RuntimeSpirv-OpCooperativeMatrixLoadKHR-08986; the driver tolerates it today); (b) DONE
    2026-09-06 - the coopmat2 device creator (`modules/dasVulkan/daslib/vulkan_boost.das`) names
    `shaderIntegerDotProduct` on the 1.3 struct whenever that struct is chained and keeps the
    standalone struct for a device below 1.3 (VUID VkDeviceCreateInfo-pNext-06532 was the two
    side by side); (c) validation messages name a shader module by "internal ID n" only -
    `vkd_class_pipe` has the kernel name in hand, so a `VK_EXT_debug_utils` object name on
    every class pipeline and module would make the next report self-identifying (today the map
    is `DASLLAMA_VK_SPV_DUMP`'s write order, 1-based). (d) found 2026-09-06 on the same window:
    the cm2 tiles' decode callbacks take the weight block as a PhysicalStorageBuffer pointer the
    driver forms from the bound plane, and no dasLLAMA buffer is created with the
    SHADER_DEVICE_ADDRESS usage (VUID RuntimeSpirv-PhysicalStorageBuffer64-11819, first seen on
    `q8_batch_cm2l_cls` once the deltanet GEMMs took the cm2 tiles; the K-quant cm2 tiles read the
    same way). The driver serves the reads; the layer's safe mode ZEROES them, so a validated 9B
    prefill lands garbage logits (top logit 13.37 against 7.76 plain) while the plain run is right -
    a validation run's numerics are not evidence until (d) lands. DONE 2026-09-06: every storage
    buffer (`make_device_buf`, `make_host_buf` with storage) carries the SHADER_DEVICE_ADDRESS
    usage, chains `VkMemoryAllocateFlagsInfo` (device_address) on its allocation - both of the
    device path's allocations and the pinned host mirrors - and queries its device address once
    after the bind (`note_device_address`): the usage bit alone left the report standing, because
    the layer learns a buffer's range only from `vkGetBufferDeviceAddress`. The mapped-image import
    stays a transfer source (no kernel reads it); the device already enabled `bufferDeviceAddress`
    (the coopmat2 creator sets it and gates coopmat2 on it). Acceptance held: the validated 9B
    window logs no VUID at all and lands the plain run's logits (7.70 vs 7.76). One residue: under
    the layer the process then wedges at exit (one core spinning, the card idle) - kill it; a plain
    run exits clean. (e) The tier's kernels assume a 32-lane subgroup - the shuffle reductions'
    xor ladders, the scan's lane-pair arithmetic (`lanes_per_pair`), the GEMV's rows per workgroup
    - and the init refuses a smaller `subgroupSize`; a wider one (a wave64 device) runs those
    arms unmeasured. The KHR kq tile (`KqCm2BatchT`'s `KHR` arm, 2026-09-07) is the one site
    that GATES on it instead: eight subgroups own its 128 weight rows, so `pf_f16_feed` admits
    a kq format in mode 3 only at subgroup 32 (`khr_kq_tile_on`), and a wave64 card keeps its kq planes on
    the sdot4 batch tile - the wave64 twin (four subgroups, 32 rows each) is owed with the
    wave64 run. Done = a validation message naming `qk_rms_cls` by name, and a wave64 run
    of the kernel-unit suite. The emitter's operand laziness itself - eager `select` / bool `&`
    `|` as language surface, the purity heuristic dropped, the 12 kernel sites converted - is
    its own SPIR-V emitter follow-up.
41. **`tests/test_vulkan_kernels.das`'s device-absent cells feint instead of skipping.** The file's
    idiom (its header: "every check feints cleanly") predates `tests/REVIEW.md`'s rule that a cell
    with nothing to assert registers `t |> skip`, so on a box without a Vulkan device every cell of
    the model-free suite reports PASS having done nothing - the hybrid ladder's fa/dn cells follow
    the file's idiom. Done = a file-wide conversion to `t |> skip` on the no-device and no-cm2 paths.
42. **End of the 27B arc: the cm2-disabled sweep.** Every 27B row on the board is a cm2 row
    (NV_cooperative_matrix2 on the 5060 Ti). Boris (2026-09-07): measure each file with the
    tensor tiles off, both engines - ours `DASLLAMA_COOPMAT=mm` (KHR coopmat mul_mm) and
    `DASLLAMA_COOPMAT=sdot4` + `DASLLAMA_VK_FA=0` (no cooperative matrix at all), the
    reference exe `GGML_VK_DISABLE_COOPMAT2=1` and `GGML_VK_DISABLE_COOPMAT=1` - to learn
    what cm2 buys, where we stand against llama.cpp on the same arm, and whether every
    format's non-cm2 tile (the `KqBatch*` sdot4 tier serves every kq format) decodes and
    prefills correctly - the bench's sanity argmax and logit must match the cm2 run's. The
    real proof of a lower target is a run on one (a Turing or Ampere card, an RDNA card): the
    knob arms exercise the kernels, not the device-creation path a card without the extension
    takes. Done = the four arms on UD-IQ4_XS, UD-Q3_K_XL and i1-IQ3_S in the PR body. The row
    from a non-5060 card is the real-hardware pass, a follow-up arc after every family works
    here (Boris 2026-09-07): rented
    boxes - AWS g4dn (T4, the KHR arm on NVIDIA's driver) and g6e/p4d (48-80 GB, the fully
    resident 27B Q8 and 35B MoE), an RDNA3/4 card from a GPU marketplace (AWS's AMD parts predate
    cooperative matrix) - each with a written plan of what to run where.
    FIRST PAIR (UD-IQ4_XS, 5060 Ti, pin 14000, 2026-09-07): decode is flat on every arm (ours
    23.4 on all three, llama.cpp 24.1) and the sanity argmax holds (pp 13, tg 5709). Prefill:
    cm2 860.8 vs 813.7; KHR coopmat 221.2 vs the reference exe's `mul_mm.comp` 677.5 (0.33x);
    no coopmat 215.7 vs 312.6 (0.69x). The cause of the 0.33x: the kq formats have no
    KHR-coopmat tile - `DASLLAMA_COOPMAT=mm` serves only the q8 planes on the mul_mm L-tile
    and every kq plane on the `KqBatch*` sdot4 tile, so mm and sdot4 read the same on a
    kq-only file. A card without NV_coopmat2 (every AMD and Intel part, the GTX line)
    prefilled a 27B at a third of the reference exe's rate. The fix was the format decode on
    the mul_mm L-tile, the way it moved onto the cm2 template.
    THE KHR kq TILE LANDED (2026-09-07 evening, `ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2ae): the
    cm2 template's KHR arm, every kq format, 0 of 89600 off on all thirteen; the 4B Q4_K_M mm
    row 1564 -> 2378 (0.56x of the reference exe's `mul_mm.comp` 4221), the 27B UD-IQ4_XS
    221 -> 395 (0.59x of the same tile's 675), decode unchanged. Its three measured steps: the
    decode call on the plane element (the block copy ran the tile at a third), 16-byte
    activation loads, a 32-deep k step (the 64-deep one blew the 49152 B workgroup cap on the
    iq2 grids). THE SLAB (the same evening): the probe's copy of the
    tile body read 33 TFLOP/s where the shipped class read 25.6, and the difference was the 8 KB
    edge-store slab the shipped class carried beside its two 10 KB staging arrays - a probe arm
    of that evening (since retired), the copy plus that one array touched on a path no dispatch
    takes, read 28.5: 8 KB more shared memory per workgroup is one workgroup fewer per SM (the
    two-array footprint is now the tile's own, `ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2ae). The staging arrays are
    now `uint` f16 pairs (the activation words stored as they arrive, no unpack) and the edge
    tile's f32 fragments bounce through the weight array once the k loop is done, the row guards
    hoisted out of the loop: the shipped class 25.6 -> 32.1 / 23.9 -> 31.4 / 25.6 -> 32.2 on the
    gate / down / q shapes, the kernel suite 108 of 108 with its edge-tile cells, the hybrid file
    10 of 10 on the KHR arm, the 4B Q4_K_M mm row 2378 -> 3051 (0.72x of the reference exe's
    `mul_mm.comp` 4221; the probe's khr row 32.6 / 31.8 / 32.6 on a box holding 1.3 GB for
    other processes). The 27B UD-IQ4_XS row's re-measure is owed: at 1.2 GB held by other
    processes the resident plan's KV room fell to 147 MB, under the 2048-position minimum,
    and the driver declined - the row needs the box the board's 395.2 was taken on (about
    650 MB held).
    WHERE THE REST IS (the 2026-09-07 reading of `quantize_y` was wrong; corrected 2026-09-08 from
    `ggml_vk_load_shaders` and `ggml_vk_get_mul_mat_mat_pipeline`): on a device with KHR cooperative
    matrix llama.cpp creates no integer tile - `CREATE_MMQ` sits only in the shader loader's two
    scalar arms, the q8_1 pipeline set is empty, and every mat-mat product falls to the f16
    KHR-coopmat `mul_mm.comp` tile with the format decoded into shared memory (`load_a_to_shmem`),
    the 4221 and 675 t/s bars included. That tile on the NVIDIA card (`warptile_mmq` for a quantized
    A, the `aligned` variant on every shape of ours, split-k off): 128 threads, a 128 x 128 x 32 block,
    four subgroups as 2 x 2 tiles of 64 x 64 over 16 x 16 x 16 fragments (sixteen accumulators per
    subgroup, one B stage shared by all four), f16 accumulators (`coopmat_acc_f16_support` with
    default precision picks the f16acc pipeline: 64 accumulator registers per lane), two 128-row
    stages at a 20-word stride - our stride - plus a 2 KB store stage, 22 KB in all, and a direct
    `coopMatStore` of a whole tile; the per-shape rates its perf logger (`GGML_VK_PERF_LOGGER=1`)
    reads (43-46 TFLOP/s-eq on the 9728/4096 x 2560 shapes, 27-31 on the 80-workgroup ones, the 4B
    prefill on the 5060 Ti) are that tile's, against our f16 KHR tile's 32 on the large shapes
    (`harness/vk_gemm_probe.das -- khrx`, same card, after the slab fix above). The same hardware
    path, the same shared footprint and barrier cadence. The reading of the difference (mul_mm.comp):
    their four subgroups each load 4 A + 4 B fragments per 16 MMAs over one shared B stage, where
    our eight each reloaded all 8 B fragments against 1 A - 144 fragment loads per workgroup per k
    step against their 64 to 96 - and their accumulators are f16, 64 registers per lane against our
    f32's 128. Our road to parity: the subgroup tiling and the accumulator width. The reading of the
    coopmat-less arm (ggml-vulkan.cpp, `GGML_VK_DISABLE_COOPMAT=1`): its mat-mat runs the integer
    MMQ tile `matmul_q4_k_q8_1` - 128 threads, 128 x 128 x 32 over four staged k blocks, Q8_1
    activations with a per-32 (d, d x sum), a 4 x 32 f32 register block per thread over
    dotPacked4x8, the sub-block (d*sc, dmin*m) folded per 32-k block; no IQ format has one - whose
    rates on this card are unmeasured: the perf logger names no pipeline, and
    `GGML_VK_PIPELINE_STATS=q8_1` is what proves the tile was reached. Our sdot4 mode's lever is an
    integer tile of that shape.
    THE FIRST INTEGER-TILE PROTOTYPES (2026-09-07, `harness/vk_gemm_probe.das -- mmqx`, the sdot4
    arm's lever): three register-block shapes over the sdot4 k4 staging (64 x 32 at 4 x 8 per
    thread staged per superblock; the reference exe's one-block stage at 64 x 128, 4 x 16; that
    stage at 4 x 4 as named scalars) all read 8.8-9.2 TFLOP/s against the shipped tile's 12.2, and
    the named-scalar twin with CONSTANTS in place of the plane reads read 11.0 - the inner loop
    caps the shape, not the memory: per block a thread issues 24 shared loads and 160 sdot4 for 16
    outputs, the reference exe's `matmul_q4_k_q8_1` 4 x 32 block 40 loads for 1024. Two
    prerequisites before the next prototype of either tile: (a) the SPIR-V emitter unrolls
    `for [unroll_full] (i in range(N))` at emission,
    so a fragment or accumulator block written as a fixed array chains constant indices the
    driver promotes to registers - the coopmat tile's sixteen accumulators need it as much as
    the integer tile's 128; (b) a hardware profile (Nsight) of our KHR tile beside the
    reference exe's on one shape, not another blind bisect.
    THE TILE (2026-09-08, `harness/vk_gemm_probe.das -- khrx` and Nsight GPU Trace on the RTX 5060 Ti):
    the profile of the shipped KHR tile read the load-store pipe at 81% of its peak - the four-wide
    decode callback's eight 16-bit lane loads and three scale words per 16 values - and the register
    file at 99%, two workgroups of eight warps per SM; neither the f16 accumulators alone (32.7
    against 33.4 TFLOP/s on the 4B gate shape) nor the two-by-four subgroup tiling alone (35.5)
    moved it, and our tile in the reference exe's geometry ran at 22 (about 245 registers per
    lane, one workgroup per SM). The stage went first: every format's `khr_stage16` reads its
    16-value run as one or two words of the quant plane (37.6), then f16 accumulators (54.0),
    then the two-by-four tiling (54.9) - the shipped class 53.8 / 59.8 / 57.9 on the gate /
    down / q shapes against 32.6 / 31.7 / 32.5 before it and the reference exe's
    `mul_mm.comp` at 43-46. THE PER-LEVER
    ROWS (`-- khrx` at the arc's tip, the RTX 5060 Ti, the 4B gate / down / q shapes, TFLOP/s,
    each arm the shipped tile with one lever moved back): the shipped class 54.2 / 59.5 / 57.6 and
    its probe copy 58.8 / 63.9 / 60.3 (the class carries the region and split-k arithmetic the copy
    omits; the copy is bit-exact against it); staging through the four-wide decode callback in
    place of the words - eight 16-bit lane loads and three scale words per 16 values - 35.6 / 36.0
    / 35.2 (bit-exact); f32 accumulators 42.7 / 44.1 / 43.6 (within 0.01 of the f16 class); the
    16-row strip tiling 54.5 / 57.4 / 55.7; the reference exe's `mul_mm.comp` geometry - four subgroups of 64 x
    64 in a 128-thread workgroup, sixteen accumulators each - 53.5 / 57.1 / 57.5, no better; a
    constant fill in place of the weight stage 65.6 / 66.3 / 64.6, the loop's ceiling; the sdot4
    kq tile 12.3 / 12.2 / 12.2. The shipped class holds 0 of 768 sampled outputs off the kernel
    cell's bar against the k4 CPU oracle on every shape. The partial-window row (the gate shape at
    300 tokens, where the last token tile takes the edge store; only the two clamping tiles run
    there): the shipped class 45.0 (54.2 at 512 - 57.6 per computed row, so the padded rows and not
    the bounce are the cost), the sdot4 tile 11.6 (12.3). The kernel suite holds 108 of 108 with
    its thirteen KHR cells at 0 of 89600 off, every other kernel's
    SPIR-V is byte-identical, and the board's KHR rows read: the 4B Q4_K_M 3051 -> 4764 t/s
    (1.13x of the reference exe's `mul_mm.comp` 4221), the 27B UD-IQ4_XS 395 -> 741 under a
    14000 MB pin (1.10x of the same tile's 675), decode unchanged (116.8 and 23.2). Beside it
    the same day, the cm2 k4 l tile on the same shapes and the same un-barriered submit
    (`-- k4`, the `k4lnb` rows): 59.8 / 66.0 / 62.0 - the KHR tile reads 90% of the
    NV_coopmat2 tile's rate, and the 4B's end-to-end ratio (4764 against 5150) reads 92%; the
    cm2 tile's own levers are the driver's (decode-in-load, its tiling), so the word stage is
    not a cm2 change. THE ONE ROW UNDER PARITY (the 9B UD-Q5_K_XL at 1788 against the
    reference exe's `mul_mm.comp` 2098) was a gate, not the tile: the recurrent head decided
    its f16 feed per LAYER over qkv, z and out together, and
    that file's out plane is Q8_0 - a format the KHR arm's f16 feed does not admit - so every
    recurrent layer's K-quant qkv and z fell to the sdot4 tile (the per-role profile,
    `DASLLAMA_GPU_PROF=1`, its `vk_rdpf dn` lines: qkv 72.9 ms and z 35.1 ms per window at 11
    TFLOP/s against 19.7 / 10.4 on the Q5_K_M file, whose out plane is Q5_K). The x feed (qkv, z)
    and the o feed (out) are now decided apart, each by the planes that read it (`pf_dnx6` /
    `pf_dno6`; the attention and FFN heads already split so): qkv 19.7 ms, z 10.5, the row 2531 /
    56.66 (1.21x / 0.98x). The witness is the hybrid parity file's mixed twin
    (`Qwen3.5-0.8B-Q4_K_M-q8out.gguf`, a Q8_0 out plane beside K-quant qkv/z) holding, on a
    coopmat feed, that the prefill never requantized the block input to Q8_K. Still open under this
    item: the no-coopmat arm - ours `DASLLAMA_COOPMAT=sdot4 DASLLAMA_VK_FA=0` against the reference
    build's coopmat-less arm (the knob in the 2026-09-07 row above), where the prototypes above are
    the road - the wave64 twin of the KHR tile, and the real-hardware pass.
43. **The Vulkan tier covers every carrier the module already serves.** Ruled 2026-09-08 (Boris,
    after the MoE fit survey, `followup_general.md` item 122): after the 0.6.4 release and before
    any new family, every carrier dasLLAMA serves today gets its Vulkan arm at parity - the
    existing families, and the vision, audio and TTS towers, whose GPU drivers are Metal today.
    The known gaps on the family side are this ledger's items 3 (the quant KV codecs), 4 (batched
    decode), 5 (the speculative round), 6 (the mx4 and q51 device kernels behind gpt-oss and
    gemma-4-26B), the gemma4 pre/post-norm attention decline, and the fully-resident MoE chain:
    the 5060 Ti holds Qwen1.5-MoE Q6_K, Qwen3-30B-A3B UD-Q3_K_XL, Qwen3.6-35B-A3B UD-IQ3_S and
    gemma-4-26B-A4B UD-IQ4_XS whole (12.8 to 13.8 GB), the resident driver declines every MoE by
    construction, and the per-op tier reads 0.14x prefill / 0.75x decode against the reference
    engine on the one such row measured (the arc board's row 20, both engines fully resident).
    Done = every family and tower row on the board with a Vulkan column at parity, and the
    serving census (`coverage-vk`) with no carrier the tier declines. The fully-resident MoE
    chain landed 2026-09-09: the whole-model driver admits a MoE whose expert stacks fit the arena
    (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md` sec.2.2n), the window chain's routed block
    (`ARCHITECTURE_GPU_VULKAN_MOE.md` sec.2.2af) and the token command's (same document,
    sec.2.2ag); the per-op tier keeps the files that stream. The 30B and 35B rows ride the driver
    whole (the plan forgoes the per-op reserves for a fitting file), the hybrid MoE registers its
    routed block after the deltanet head, and the expert tiles took two levers: the grid decodes'
    lane shifts (item 36's 2026-09-09 status) and the schedule's tile ladder (a bucket past the s
    column takes m columns, the last partial: `ARCHITECTURE_GPU_VULKAN_MOE.md` sec.2.2af; the real
    window's skew put 4096 rows in 175 s tiles where the ladder runs 85). Every pp512 / tg128
    rate under this item is a `benchmarks/lcpp_bench.das` reading on the RTX 5060 Ti box
    (`bin/Release/daslang.exe -jit benchmarks/lcpp_bench.das -- -m <file> -o md
    --for-debug-purposes -r 3 -p 512 -n 128 -t 16` under `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0
    DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048 DAS_JOBQUE_THREADS=16`), debug-jit, and
    every reference rate beside one is `llama-bench -ngl 99 -fa 1 -t 16 -r 3` at b10660
    (build-vulkan-357) on that box; every window and token millisecond under this item is the
    same bench's `DASLLAMA_GPU_PROF=1 ... -n 32 --prof --jobque-profiling` profile - its
    `vk_rdpf` window lines and its `vk_rdec gpu avg/token` / `vk_rdec moe avg/token` lines. The
    rows, pp512 / tg128: Qwen3-30B-A3B UD-IQ2_XXS 3242.0 / 123.6
    (3520.0 / 116.6: 0.92x / 1.06x; the window 153.9 ms against 142.3, the expert tiles 97.7),
    Qwen3.6-35B-A3B UD-IQ2_XXS 2837.7 / 95.9 (2853.1 / 71.6: 0.99x / 1.34x), the Qwen1.5-MoE
    twin 5152.9 / 142.5 (5099.8 / 173.8; its window 94.5 ms, the first measured rep after the
    warmup reads 101 on every row here, a driver warm-up the bench's one warmup does not absorb).
    One more pass the same day: the shared expert's K-quant planes ride beside its q8 transcode
    on a load with a GPU tier armed (a CPU-only load keeps the transcode alone) and the
    whole-model driver places those (the twin's token 7.06 -> 6.12 ms, tg128 162.7 =
    0.94x), the schedule's slot walks are an atomic tally and cursor (8.1 -> 0.46 ms on the 30B
    window), the router tile prefetches its stage as a 64 x 32 tile (6.6 -> 4.3 ms): the 30B
    3448.9 / 124.6 (0.98x / 1.07x, the window 144.9 ms against 142.3), the 35B 2946.0 / 95.2
    (1.03x / 1.33x), the twin 5348.3 / 163.4 (1.05x / 0.94x). The remainder pass's first
    lever: the routed combine rides the residual step in both chains (`ClsArComb`, sec.2.2af /
    sec.2.2ag), one dispatch per layer fewer, in the two kernels' sum order so the MoE files'
    bars keep their calibration; then the router tile's float4 stage (its scalar stage was
    bank-conflict bound: 4.3 -> 2.4 ms per 30B window), the residual step's slot groups (eight
    rows in flight, then four: the twin's one-row step 360 -> 290 us per token) and the
    FFN-norm requant skipped on a layer with no shared expert (540 us per 30B window that
    nothing read); then the decode GEMV family's lanes per row (`ARCHITECTURE_GPU_VULKAN_GEMM.md`
    sec.2.2ah: a subgroup over one, two or four rows by the row length, the expert rows of a MoE
    being the short ones - iq2s at K 768 148 -> 337 GB/s on the probe). The rows on the final
    kernels, pp512 / tg128: the 30B 3482.6 / 132.0 (0.99x / 1.13x, the window 143.4 ms against
    142.3, the token 6.79 ms), the 35B 2962.8 / 107.1 (1.04x / 1.50x), the twin 5395.3 / 166.8
    (1.06x / 0.96x), the dense 4B Q4_K_M unchanged at 115-117 (its token a wash on either
    form). Still open under this item: the 30B prefill's last 1% (the expert tiles ~99 ms
    against the reference's ~88 - a 64-wide column for the 33-64-row buckets - the router's 2.4
    ms against 1.2 on 32 workgroups over 36 SMs, the act 2.8 and the gather 1.6; the profile
    stamp after the down tiles absorbs their tail, so the residual step's own cost does not read
    there), the twin's decode 4% (a token's expert dispatch is 3-4 MB, where launch and ramp cost
    what the transfer does: the 30B's e_down reads 806 us over 48 layers = 240 GB/s effective
    against the probe's 337 steady - fewer, larger dispatches, or the gate and up planes in one,
    are the next form), the fused add+rms twin that also stores the normed row
    (the router's feed, so a MoE could take the fused rail; ar1 reads 1.8-2.0 ms of the 30B
    window), the CPU chain's shared expert on the same K-quant planes (it reads the q8
    transcode, so the resident-vs-CPU bar carries the two forms' rounding), an LPT order for
    the device schedule's pieces (the m dispatch already leads the s one), and the two probe
    arms the checklist's race rule asks for - the router tile's float4 stage against its scalar
    stage and the residual step's slot groups against the plain loop, the old bodies kept as
    probe twins (both ranked on before/after `DASLLAMA_GPU_PROF=1` profiles across processes,
    4.3 -> 2.4 ms per 30B window and 490 -> 440 us per twin token; ruled 2026-09-09 to ship as
    stated claims).

44. **The whole-model driver admits K-quant planes only off a repacked load.** Every plane the
    driver places - the attention quads, the deltanet planes, the expert stacks and the shared
    expert's triple - passes `kq_servable`, and that predicate (`dasllama_common.das`) is the CPU
    fused chains' rule: a superblock format serves only when `t.kq_repacked` is set, which
    `select_matmul_backend_for_load_` sets for a repacking backend alone. On a box that selects
    the portable backend a K-quant model never reaches the driver - the Qwen3.6-35B-A3B
    UD-IQ2_XXS declines on "layer 0's shared expert carries a format (2/2/3) the dense rail does
    not serve" (a reviewer's reading on Linux, 2026-09-10) - while the device gather
    (`moe_gpu_gather_stack_kq`) takes the layout flag as an argument and reads the disk order
    too. The driver wants a device-side servability predicate (a format the tier has kernels
    for, in either layout) in place of the CPU chains' rule at its four sites; done when a
    portable-backend load of a K-quant model arms the driver and its resident-vs-CPU parity
    file holds, which needs a box whose backend does not repack (none here: this box's backend
    is x64-gen).

45. **The resident MoE chain does not scale with the card.** Measured 2026-09-10 on a rented Linux
    RTX 5080 (driver 580.173, 8 vCPU, `-t 8`) against the RTX 5060 Ti (Windows, driver 616.56),
    both on the Qwen3.6-35B-A3B UD-IQ2_XXS (sha256 2e8f5f70..., the model-set row's), the bench's
    `-jit --for-debug-purposes -r 3 -p 512 -n 128`: llama.cpp b10660 5352.6 / 147.7 there against
    2853.1 / 71.6 here (1.88x / 2.06x for a 2.14x bandwidth step, 36 -> 84 SMs), ours 3361.1 / 139.8
    against 2995.8 / 107.7 (1.12x / 1.30x) - 0.63x / 0.95x of llama.cpp on the 5080 where the 5060 Ti
    reads 1.05x / 1.50x (b10886 5461.2 / 148.1 on the same box: the version moves 2%). The per-role
    profile (`DASLLAMA_GPU_PROF=1`, both boxes) splits it three ways.
    (a) The four-wide decode twin is off on Linux: `cooperative_matrix2_decode_vector_supported`
    reads `VK_NV_cooperative_matrix_decode_vector`, which the 580.173 driver does not list (it lists
    `VK_NV_cooperative_matrix`, `_matrix2` and `_vector`; the Windows 616.56 driver lists it), so
    every cm2 tile runs the scalar arm - and so do llama.cpp's there: its shaders carry the extension
    and it strips them at pipeline creation when the driver lacks it (`ggml_vk_strip_decode_vector`),
    running its expert tiles' k step at 32 on that arm and 64 with the four-wide one (`mmqid_bk`; its
    PR 23991 raised the step together with the four-wide B loads, neither alone consistently faster).
    Same box, same source (b10660), the 5060 Ti, the 35B pp512: llama.cpp four-wide 2828.9 (the
    record's 2853.1); scalar at k step 32 2385-2393 (`GGML_VK_DISABLE_COOPMAT2_DECODE_VECTOR=1` on
    either build below); scalar at k step 64 2080 - the DEFAULT of a build whose glslc does not know
    the extension (Vulkan SDK 1.4.350's; 1.4.357's does): its device flag reads the driver's extension
    list, not the shader build, so that build keeps the 64-deep step over a scalar decode, under-reads
    the reference by 27% and says `NV_coopmat2` in its device banner where the four-wide build says
    `NV_coopmat2v`. Ours: twin 2995.8, scalar (k step 64, `DASLLAMA_VK_DECVEC=0`) 2049, the window
    160181 -> 236883 us. Per gate/up plane at the 30B's shape (its `GGML_VK_PERF_LOGGER=1`
    `MUL_MAT_ID iq2_xxs` row against our `moe:iq2xxs` probe at uniform buckets): theirs 612 / 829 /
    1014 us (four-wide / scalar k32 / scalar k64), ours 730 / 1330-1380 (twin / scalar k64); in the
    window's real schedule our e_gate plane reads 695 twin and 1229 scalar. Arm- and k-step-matched
    the two engines read the same whole model (2049 against 2080), so llama.cpp's whole Linux edge on
    this axis is the 32-deep k step on its scalar arm (1.15x for it), while the tiles alone stay
    1.2-1.3x behind at either matched arm (695 against 612, 1229 against 1014): its scalar decode
    computes a pair's shared work once and selects the element last - the form the driver's own
    two-wide commoning relies on (its PR 23541) - where ours reads its scale from the `ws` plane and
    shifts per element. The expert-tile probe reads the same 0.71 ms per iq2xxs gate/up plane on the
    5080 no-twin as the 5060 Ti twin-on (0.73; twin-off 1.33); arm-matched, the tiles scale 1.80-1.90x,
    qkv 1.85, conv 1.94, gather 1.82 (SMs 2.33x). The k step landed per format on the expert tiles -
    the s stamp and the e stamp (`CM2_TC_E`, the expert schedule's m column) of the five grid-codebook
    formats run 32-deep, every other stamp keeps 64: the k4 twin (Qwen1.5-MoE Q4_K_M) read 4330 pp512
    at 32 against 5074 at 64 on the 5060 Ti (its ~34-row buckets are all s pieces, and a light decode
    is step-bound), the 35B 3236 / 2391 on its two arms with the five formats at 32 against 3101 /
    2277 with only iq2xxs and iq3s there (the uniform `moe:` probe put iq3xxs at 64; the real schedule
    disagrees, so a whole-model row settles a step); the dense l and m stamps keep 64, which the
    deltanet projections want (0.86-0.93x at 32), and so does the per-op tier's expert chain
    (`record_ffn_cm2_cmd` keys the l/m/s columns by its batch tile): pp512 2049 -> 2391 on the scalar
    arm (1.00x of the reference's scalar arm) and 2996 -> 3236 with the twin (1.14x of its four-wide
    arm); on a Linux RTX 5080 (driver 580.173, the real scalar arm, `-t 8`, 16 vCPU) the merged code
    read 3304.5 / 130.7 and the slice 3835.5 / 131.2 with every expert stamp at 32 against the
    reference exe's 5217.0 / 146.2 on that host (0.63x -> 0.74x pp), the GEMM companion's sec.2.2l and
    `PERF_LEDGER.md` carry it. The reference build's k step is one number for every quant type -
    `mmqid_bk` follows `coopmat2_decode_vector` alone; its mul_mat_id tiles are BM 128 with BN 128 past
    64 routed rows and 64 below, never split-k; its expert k loop is `[[dont_unroll]]`, so a halved
    step doubles the loads and MMAs; its scalar decoders are pair-form (compute two, `ret[idx & 1]`)
    for q4_0, q8_0, q2_K, q5_K, q6_K and the grid formats, single-element for q3_K, q4_K and iq4_*, with
    a shared-scale cache (`shAscales`, refreshed per 256 k) for q4_K and q5_K alone - so a k step per
    format is new ground, and on its scalar arm its q4_K experts run 32-deep too. Left on this axis:
    the decode body's pair form (the tiles alone still read 1.2x behind at the matched scalar arm: our
    uniform gate/up plane 0.99 ms against the 0.83 ms of the same `MUL_MAT_ID iq2_xxs` row above), a
    shared-scale cache for the K-quants (we read the scale row per element on every format), and the
    k step by arm - on the 5080's scalar arm the k4 s tile reads 0.471 ms at 32 against 0.499 at 64
    (`moe:k4` there), the opposite of the four-wide arm, so a stamp per (format, arm) is the follow-on.
    The tier warns at device init and prints its extension roster.
    (b) Three window roles stay flat on the wider card whatever the arm: the deltanet scan (13037 ->
    12654 us, a serial recurrence over chunks - 8.7% of the 5080's window), the shared expert's
    k5/k6 tiles (sh_gate 3518 -> 3379, sh_down 1582 -> 1452 - one dispatch per layer whose grid is a
    single wave; the tile pick at 84 SMs is the first suspect) and the router (2276 -> 2371: 32 fixed
    workgroups). The routed tiles' real schedule (87 buckets, the largest 448 rows, 106 column
    tiles over the ladder, one workgroup per column tile per 128-row weight tile - about 640
    workgroups on a gate plane, 1700 on a down plane) scales 1.80-1.90x arm-matched against the
    2.33x in SMs; an LPT order over the pieces is the lever left there.
    (c) Decode is the per-dispatch floor: the expert GEMV reads 3.3 MB per layer in 8.5 us on the
    5080 (3.4 us of transfer at 960 GB/s, ~5 fixed) and 11.3 us on the 5060 Ti (7.4 + 3.9); some 600
    dispatches per token carry it - about 3 ms of a 7.8 ms token - and every small kernel reads
    SLOWER on the 5080 (sh_gate/sh_up/sh_down 0.87/0.84/0.92, ar2 0.87, topk 0.93, the deltanet step
    0.76, attention 0.88), while the GEMV probe scales exactly with bandwidth (iq2xxs 410 -> 881 GB/s
    at the expert shape). The lever is fewer dispatches per layer: gate and up in one, router and
    top-k in one, the shared expert's three GEMVs as extra slots of the expert dispatch, the
    activation folded into the down GEMV; the host is 176 us of the token on the pod (2%).
    The pod's card ran 2910-2925 MHz at 250-263 W of its 307 W limit under the probe, so no clock
    cap stands behind (b) or (c). Research before any kernel work; each of (b) and (c) is a slice
    of its own.
    (d) The second Linux pass (2026-09-10, a second RTX 5080 pod, driver 580.173.02, 84 SMs, 8
    vCPU) re-read (b). First the instrument: the Linux driver drains the queue at a bottom-of-pipe
    timestamp (`ts:k5`: two 16-workgroup m stamps 103 us plain, 210 with the profile's stamp between
    them, 209 with a barrier; the Windows 616 driver reads 58 / 58 / 106), so every pair the hazard
    rail co-runs reads serialized under the profile - k 143 and v 113 us after q, sh_up 84 after
    sh_gate, z 99 after qkv - and the profiled window runs 4.6% past the served one (pp512 3833 ->
    3661; the 5060 Ti's profile costs nothing). The window companion's chunk paragraph carries the
    reading rule. Then the shared expert: its gate and up (k5, 512 rows over K 2048, 16 m tiles each)
    fell under the split-k group rule at 84 SMs - the pair (32 workgroups) fills under half the device,
    so each split in two (32 workgroups of K 1024) and the two serialized through the one scratch
    plane, the worst of both forms; the probe reads the m stamp at 105 us alone (16 workgroups, K
    2048), 27 us at 64 workgroups of K 512 (`cm2:k6 shdown`, warm or cold), and the reference exe's
    logger reads its q5_K GEMM at 23 and its q6_K at 27 on the same shapes. The chunk count now
    follows the role's own grid (four chunks, 64 workgroups): pp512 3833 / 3854 -> 3907, the profiled
    sh_gate + sh_up 206 -> 136 us per layer (`PERF_LEDGER.md`). What stays open on that card: (1) the
    window's small cm2 GEMMs read 20-40 us past the probe's figure for the same stamp, grid and shape
    (sh_down 56 against 27 at 64 workgroups; the split gate 88 and up 48 against 31 + a 4 us reduce; k
    143 against 105) while the elementwise roles read 3-15 us and the 5060 Ti's engine matches its
    probe - and no probe form reproduces it: not L2-cold planes (+5%), not the schedule in host memory,
    not the weight plane at the far end of a 3 GB slab, not 8192-row feed planes, not a barrier or a
    pipeline change between stamps, not the l/m/s stamps taking turns over cold planes, not ReBAR off
    (`DASLLAMA_VK_REBAR=0` reads the same profile), and the card holds 2827 MHz through both; the
    reference exe's per-op logger stamps the same way (`eAllCommands`) and reads no such excess, so it
    was the engine's. FOUND: a stamp before every cm2 dispatch (a scratch experiment on the pod's copy,
    never committed) read the barriers at 0 us - the excess sits inside the dispatch - and the probe's
    flush row (a 128 MB reduce before each dispatch, so the stamp's code, descriptors and planes are
    out of every cache, as they are in the window after ~250 MB of weights per layer) reproduced it:
    the 64-workgroup k6 m stamp 26.7 us warm or plane-cold, 43.0 after the flush on the pod, 34.5
    against 31.7 on the 5060 Ti. The cost is the stamp's CODE refetched: the k loop was unrolled eight
    steps with the decode inlined per copy. The unroll is now a stamp constant, one superblock per
    block (`UNR` 4 at BK 64, 8 at BK 32; `REVIEW.das` holds BK x UNR to the template's): the pod sweep
    (u8/4/2/1 on k6) read the l tiles best at 4 (+5%), the m tiles +10-14% at 4 or 2, the 64-deep s
    tiles even, the flush penalty gone at 4 and below, and the 32-deep iq2xxs e stamps alike at 8 and
    1, 2% behind at 2, 12% at 4; pp512 3906.6 -> 3986.8 on the pod, the 5060 Ti's twin arm within
    noise (3201 eight-copy against 3191 / 3166), its scalar arm flat, the k4 twin +2.3%
    (`PERF_LEDGER.md`). Two facts about the driver's own unrolling came out of it: `[unroll]` and
    `[partial_count = 4]` on a runtime-bound `while` leave the loop rolled (the k6 tiles at half rate;
    the SPIR-V dump shows the control on `OpLoopMerge`), while the hand form - an outer runtime loop
    over an inner literal-bound `[unroll]` loop - unrolls, so the driver honours `Unroll` only on a
    constant trip count; and a plain `while (k < k1)` over the general `k0`/`k1`/`ybase` bounds also
    drops the no-split arm's literal-bound form (sec.2.2l), so that experiment confounded the two -
    a clean hint re-test on the no-split path alone is still owed. The iq2s k step was re-asked under
    the new unroll, since the skewed probe read the 64-deep m stamp 4% ahead of the 32-deep e stamp on
    the pod (7-14% on the 5060 Ti): the 35B's down plane at 64 read pp512 3903 against 3987 and e_down
    21.9 against 20.1 ms, so the five grid formats keep 32 - the whole-model row settles a step, the
    uniform probe does not. The pair-form scalar decode of (a) landed for iq2xxs and iq2s: e+s
    0.357 -> 0.316 / 0.428 -> 0.380 ms on the skewed schedule, pp512 3987 -> 4201 (0.805x); then for
    iq2xs, iq3s, iq3xxs, k5 and k6 (the K-quants also off the byte2 lane select): k6 l 85.6 -> 94.3
    TFLOP/s, the k5 shared-expert m stamp 105 -> 81 us, pp512 4236 +- 33 at five reps (0.812x). The
    small hybrids reproduce the rest of the gap without any MoE: on the pod Qwen3.5-0.8B-Q8_0 reads
    pp512 21639 against the reference's 29957 (0.72x) and tg32 325 against 480, the 9B UD-Q5_K_XL
    ~3000 against 5666; the 0.8B's per-role window (22989 us against the reference's 16177) puts 5.4
    of the 6.8 ms in the deltanet scan alone (8854 us, 492 per layer, against `GATED_DELTA_NET` 17 x
    202), conv 0.9 ms (93 per layer against 27-47), the out GEMM 0.8 (63 against 31), down 0.4 -
    so the scan is the next lever, with the 0.8B as its ten-second loop. The scan then took upstream's
    shape (one column per lane cluster, k and q per lane from the conv plane, no staging, no barrier):
    8854 -> 8084 us and pp512 21639 -> 22354 on the 0.8B, the 35B flat at 4217 +- 44; its first cut read
    19004 us until `conv` and `smalls` carried `@readonly` - the emitter decorates NonWritable from that
    annotation alone, so every other binding in the kernel file is declared writable and the driver
    orders its loads behind every store. The dispatch macro now stamps `readonly` on every binding its
    classifier finds unwritten: the 35B 4217 -> 4673 +- 38 (0.896x), tg32 129.2 -> 132.8, the 0.8B 22354
    -> 23310 and tg32 320 -> 338, conv 1685 -> 1344 us, the logits bit-identical. The scan's lane shards
    four wide (a lane's 16 rows contiguous, four float4 loads per operand, `dot` trees) read the scan
    8084 -> 8668 and the 0.8B 20047 - dropped; upstream's `gated_delta_net.comp` interleaves its rows
    (`r * LANES_PER_COLUMN + lane`) as the scalar form does. What upstream has that the scan lacks:
    ds as a spec constant (immediate load offsets, a folded `subgroupClusteredAdd` instead of three
    branch-guarded shuffles in the dependency chain), one exp per token (its g and beta arrive
    pre-activated from the graph; ours spends exp, log, exp, exp and a divide per token per lane), one
    warp per workgroup. The scan still reads 448 us per layer against its 202. Three scratch ablations
    on the 0.8B split that: ds as literals (immediate offsets, branch-free shuffles) 8060 -> 7816 us, the
    gates as constants 7816 -> 5156, the cluster reductions removed 7816 -> 6585 - so the gates were a
    third and the reductions a sixth. The gates are now computed once per workgroup into shared memory
    before the token loop: scan 8060 -> 5240 us, the 0.8B pp512 23310 -> 26722 +- 68 (0.892x), the 35B
    4673 -> 4776 +- 39 at five reps (0.915x). Left on the scan: ds as a template constant (3%), one
    warp per workgroup; `subgroupClusteredAdd` at a literal cluster size in place of the three shuffles
    read the scan 5240 -> 6962 us on the 580 driver and is out. The 35B's profile at that state put the
    rest of the gap in two small f32 GEMMs on scalar tiles - the deltanet beta/alpha rows (2.8 ms over 30
    layers, 94 us each for 0.13 GFLOP) and the router (2.1 ms over 40) - so both ride the cm2 tile as f16
    GEMMs now (`F16GemmCm2`, eight k chunks into the split-k scratch, the reduce): ba 2828 -> 1170 us,
    router 2125 -> 1621, pp512 4776 -> 4961 +- 50 at five reps (0.951x), the sanity argmax's logit 0.02
    apart. What the profile still shows over the reference exe: the scan 10.2 ms against 7.5, conv 2.7
    against 1.0, the shared expert 5.7 against 4.0, e_down 20.7 against 19.6, e_gate+e_up 32.9 against
    31.5, the router+select chain 3.3 against 1.7, and a host gap of ~3 ms (wall 103 ms, the drained
    GPU sum 101; the record phase is 1.5 ms of host time the chunked submit already overlaps). The conv
    then went channel-major (a thread's channel sliding over 32 positions, the per-head norm by shuffles
    and one shared row): the 0.8B's conv 1351 -> 824 us, pp512 26741 -> 27524 +- 88 (0.919x), the 35B's
    conv 2740 -> 1660 and pp512 4961 -> 5010 +- 47 at five reps (0.960x). The scan at one warp per
    workgroup (upstream's shape) read the 0.8B +1% (scan 5258 -> 5084 us) but the 35B 5010 -> 4719 +- 40,
    so the four-warp workgroup stays. THE PICTURE on the pod at that state (five reps, ours / the reference
    exe's, pp512 and tg32): Qwen3.6-35B-A3B UD-IQ2_XXS 5010 / 5217 (0.960x) and 132.5 tg32; Qwen3-30B-A3B
    UD-IQ2_XXS 5926 / 6005 (0.987x), 173.6 / 219.1 (0.79x); Qwen3.8-27B UD-IQ4_XS 1496 / 1677 (0.892x),
    39.3 / 47.4 (0.83x); Qwen3.8-27B UD-Q3_K_XL 1469 / 1663 (0.883x), 41.2 / 49.3 (0.835x); Qwen3.5-9B
    UD-Q5_K_XL 4369 / 5751 (0.760x), 91.7 / 112.8 (0.81x); Qwen3.5-0.8B Q8_0 27524 / 29957 (0.919x), 336 /
    480 (0.70x). The MoEs are at parity, the dense hybrids are not: the 9B's window (116 ms against 89) puts
    69 of its ms in the FFN's K-quant tiles and 29 in the K-quant projections (q5_K/q6_K at 512 rows on the
    scalar arm run ~70 TFLOP/s against the reference exe's 93-96 - the K-quant decode rate of (a), the whole
    remaining prefill lever for the dense files), the 27B UD-IQ4_XS's (342 ms against 305) 207 of its ms in
    the IQ4_XS FFN tiles; every tg32 sits at 0.7-0.84x: the GEMV family reads at ~820 GB/s where the
    reference exe reads ~900, and the ~13 small dispatches a layer (the rq / ar / actrq / ba / step roles)
    cost their floor - a decode arc of its own. Pod mechanics the rows needed: the Linux driver's
    memory-budget query counts a just-exited process's VRAM for a while, so a run right after another
    declines the resident driver (the 2048 MB headroom fallback) - `DASLLAMA_GPU_VRAM_MB=15500` pins the
    cap and a 20 s pause clears it; the 27B Q4_K_M (16.6 GB) does not fit the 5080. The K-quant lever
    landed as the reference exe's `shAscales` form: the k4, k5 and iq4xs tiles stage the sub-block scales
    per superblock (GEMM companion sec.2.2k) - `cm2:k5` gate l 83.9 -> 116.2 TFLOP/s and m 62.7 -> 99.9,
    `cm2:k4` gate l 119.5 - past the reference exe's 93-96 - and `cm2:iq4xs` gate l 109.4, m 102.9, q/wo
    l 91.4 (upstream carries no cache for IQ4_XS); the 35B 5010 -> 5091 +- 52 (0.970x of the day's 5247),
    the 9B 4369 -> 5133 +- 8 (0.892x; its window 116 -> 99 ms against the reference's 89 - what is left
    there: the scan 8.2 ms, out 4.1, z 5.5, the q/k/v/wo 6.9), the 27B UD-IQ4_XS 1496 -> 1708.5 +- 1.6
    (1.014x of 1684.9 +- 1.6; tg32 39.35, decode untouched), the 27B UD-Q3_K_XL 1469 -> 1596.3 +- 0.8
    (0.959x of 1665.2 +- 2.9; tg32 41.3). THE 9B WINDOW against the reference exe's concurrent logger (86.2 ms
    of GPU time; ours 98.8 profiled, ~94 unprofiled): the K-quant tiles read at its rates on the filled
    shapes (`cm2d:k5` on the pod: down 107 TFLOP/s against its q5_K 105, qkv 103 against 102, z/out 101
    against 91-98) and 0.84x on the FFN gate shape (104 against 124; k6 84.5 against 112) - 192 workgroups
    are 2.3 waves of the 5080's 84 SMs, and the `cm2w:` sweep says the l stamp runs ONE workgroup per SM
    with a partial wave costing a whole one (84 workgroups 0.170 ms, 126 0.333, 168 0.379, 336 0.746), so the
    grid costs three waves; the reference exe co-runs gate and up in one barrier group (384 workgroups in
    five waves against three and three), which the hazard rail's co-run gives us only where the driver
    overlaps the two dispatches. Its other window terms: GATED_DELTA_NET 250 us a layer against our scan
    342, ADD + RMS_NORM 49 us a layer against our ar1 + ar2 71; ours shorter: ba + conv 2.8 ms against 5.5,
    act 1.2 against 2.0, the out-norm 0.5 against 1.9. DECODE (the reference exe's logger, one token): the
    9B 8.40 ms against our 11.0 profiled, per recurrent layer 254 us against 321 - our step 23.5 us against
    its GATED_DELTA_NET 4.1 + SSM_CONV 5.2 (one 256-thread workgroup a head with single-thread loops over the
    128 columns, against 4096 32-lane workgroups, a state column each), ba 17 against ~10 (two `RouterGemvF16`
    dispatches of 32 workgroups with a hazard barrier between them), out 30.7 against ~20, up 49.6 against 42,
    ar1 + ar2 26 against 18; every GEMV over 12 MB runs at 700-820 GB/s on both. The same step and ba floors
    on every hybrid (step 23-25 us a layer, ba 12-26): 19% of the 0.8B's token, 9.5% of the 35B's, 6% of the
    9B's and 27B's - the decode arc's first lever, LANDED: the step's delta rule column-per-thread in registers
    with straight-line loads and subgroup-add norms (isolated 8.8 us; the 0.8B's step role 25.3 -> 12.3 us a
    layer), the beta/alpha rows one dispatch (isolated 4.6 us) - tg32 0.8B 336 -> 354, 9B 91.7 -> 96.1, 35B 132.5
    -> 141.5 (0.975x of 145.2); the `dec` probe arm says the kernels read 4.6 and 8.8 us where the token profile
    bills 17 and 25, so the rest of a small role's figure is the chain around it: a role after a GEMV reads ~10 us
    more than the same kernel after a small dispatch (ar1 12 after out, ar2 14 after down, rq_x 4 after ar2, rq_f 4
    after ar1) - the GEMV's wave tail and the barrier the successor waits at. LANDED next: the qkv and z GEMVs
    shared VHZ_DNP and serialized on a WAW hazard the per-op tier already splits (VHG_Y1 / VHG_Y2) - the resident
    decode's z rides VHZ_DNZ: tg32 0.8B 354 -> 370.5, 9B 96.1 -> 97.6, 35B 141.5 -> 143.9 (0.991x of 145.2). Next
    decode levers: the residual add + norm + requant pairs (ar1 + rq_f, ar2 + the next rq_x) as one kernel on every
    model - the fused twin `cls_ar_rq` exists but `rd_fuse_gates` keeps it off hybrids (the beta/alpha GEMV reads
    the normed f32 row), MoE (the router reads it) and any Q8_K feed (the twin quantizes Q8_0 only): a Q8_K twin
    and a store-the-row arm lift all three; then the GEMV family on the 4096-wide planes (ours 500 GB/s in the token against the reference
    exe's 610-780; its form: 2 rows a workgroup, 16 lanes a superblock, f32 x read as vec4, no shared memory). Left
    for the dense files' prefill: the scan (2.2 ms on the 9B), the 27B UD-Q3_K_XL's q3_K roles (k3 reads a 6-bit
    split scale per element and stages no cache yet). Found on the way, not
    of this lever: the iq2xxs cm2 stamps' modules fail spirv-val's OpVariable placement check ("All
    OpVariable instructions in a function must be the first instructions in the first block") in a
    decode function - the emitter hoists a kernel body's locals to its entry block but not a
    `[spirv_decode]` body's; the driver accepts the module and the tiles match the oracle, so the fix
    is the emitter's (a `collect_locals` pass over decode functions), owed before the emitter suite's
    validator run covers the kernel file. (2) The q5_K stamps run 0.74x of the reference's rate on the
    big shapes there (`cm2:k5 gate` l 70.6 TFLOP/s against its 93-96; k6 85.3 against its 72.7, so the
    q6_K stamp is already ahead): the reference's q5_K decoder beats its own q6_K by 1.18x through the
    `shAscales` shared-scale cache, ours trails k6 by 1.2x - the K-quant shared-scale lever of (a),
    worth ~4 ms across the 35B's q5_K roles (qkv, z, the attention q, the shared expert's gate and
    up). (3) The rest of (b) as measured: the deltanet scan 13.2 ms against the reference's 7.5
    (`GATED_DELTA_NET` 30 x 250 us), conv 3.5 against 1.0, the expert down plane (iq2_s, K 512) 26.6
    against 19.6, the routed gate and up 30.5 against 31.5 profiled - under the drain, so the served
    figure is lower - and the host side of the window about 7 ms of the 134 (the 5060 Ti's is nil).
