# Kernel twins census - the dispatch classes that differ only by shape

Boris (2026-09-07): "we have bunch of vulkan kernels which look like twins - different only by
shape. can we really look at what we have - and see if we can convert them to parametrized
templates. then we can take similar shapes for the new ones. also check what we do with metal."
Two read-only surveys, one per backend; every class read, suspected twins compared line by line.
The conversion work this feeds is a follow-up arc; the shape a NEW family should take is section 3.

## 1. Vulkan - `dasllama/dasllama_vulkan_classes.das`, 137 `[vk_dispatch]` classes

| Bucket | Count | Members |
|---|---|---|
| template stamp (`class template` + `[ \|> template_struct_instance]`) | 78 | RouterGemv (2, `RouterGemvT`); AtAttn (1, `AtAttnTileT`, overrides run); DaAttnB/BF16 (2); DaAttnBH128/F16 (2, `DaAttnBH128T`); DaAttn/F16 (2, `DaAttnT`); FaCm2 H64/H128/H256 leaves (8); 42 cm2 leaves (14 formats x l/m/s) + 13 KHR leaves on `KqCm2BatchT`; RopeKvStore/RopeKvStoreB/QknRopeKv f16/f32 (6) |
| inheritance family, base shell + per-format override | 26 | `KqBatchBase` -> 13 `KqBatch*`; `KqGemvBase` -> 13 `KqGemv*` |
| hand-written twin cluster | 20 | T1-T7 below |
| singleton | 16 | MoeCombine, DecCombine, DnBaGemm, TopK, MoeGatherF16, DnConv, DnTail, DnStepFused, DnScan, DnScanP3, AtPrep, SplitKReduce, QkRms, Q8Gemv, Q8Batch, CmI8Batch |

Out of scope: 16 probe classes in `harness/vk_gemm_probe.das` and 4 fixtures in `tests/_vkd_toy.das`, hand-laid by design.

### Twin clusters

| # | Classes (kernel, lines) | What differs | Lines | Proposed axes | Saving | Risk |
|---|---|---|---|---|---|---|
| T1 | `MmBatch` q8_batch_mm_cls 4465-4671; `MmABatch` q8_batch_mm_a_cls 4676-4819; `MmMBatch` q8_batch_mm_m_cls 4824-4943 | tile edge `(pa.d + 127u) / 128u` / `wt * 128u` vs `63u`/`64u`; `warp_r * 64u` vs `32u`; staging `range(8)` vs `range(4)`; LDS `cml_a : uint[2560]` vs `[1280]`; 16 vs 4 `coopmatAcc_f16_16x16`. MmA = MmBatch with the two staging guards and the whole edge-bounce `else` deleted; MmM = MmBatch at half tile. The 84-line MMA block and the 32-line store are byte-identical modulo the constants | 207/144/120 = 471 | `@template_constant BM/BN` (128 or 64); `ALIGNED : bool` gating the guards + store arm; `NFRAG` - the 16-vs-4 accumulator set needs two `static_if` arms (coopmat handles are opaque; a constant-sized array of them must be verified first) | ~230 | low-medium: all three `parity-locked to the shipped kernel`; kernel-unit cells at tests/test_vulkan_kernels.das:1522-1650 dispatch by generated name (names survive); keep the MMA ORDER identical |
| T2 | `FaCm2H64T` 1899-1983; `FaCm2H128T` 2001-2085; `FaCm2H256T` 2099-2192 (8 leaves) | coopmat widths + K step only: `coopmatWgAcc_f32_64x64` vs `64x128` vs `64x256`; `coopmatWgB_f16_64x64` vs `128x32` vs `256x32`; `coopmatLoadTensor(kt, .., j, 64u, kvh * 64u, 64u)` vs `32u, kvh * 128u, 128u` vs `256u`; `j += 64u` vs `32u`; H256T alone carries `GATED` + the `qstride`/`qhs` reads. The 40-line online softmax is character-identical | 85/85/94 = 264 | `HS`, `BC` (64 or 32) constants; 7 coopmat typedefs (QACC/QA/SACC/KB/VB/ONE/OACC) - the cm2 leaves' `BT/ACC/ACCW/FLO` idiom; lift GATED + qstride into the one template (inert at `qstride == 0`) | ~130 | low: cells at tests/test_vulkan_kernels.das:4312-4398, 4527-4620 by name; H64 reads `pa.qd` where H256 reads `qstride` - keep H64's layout |
| T3 | `CmF16Batch` q8_batch_cmf16_cls 4294-4393; `KqQ40CmF16` kq_batch_q40_cmf16_cls 4948-5052 | the 14-line MMA block and the 12-line store are byte-identical; only the two staging loops differ: `nbb = pa.n / 32u` + `nsteps` vs `nsb = pa.n / 256u`; the A-stage scale read per 32-block vs hoisted per 256 (a MEASURED q40 optimisation); B-stage `unpack8` q8 bytes vs the q40 nibble split | 100/105 = 205 | `stage_a` / `stage_b` virtual points on a `CmF16Base` shell (the `KqBatchBase` pattern); `BLKW` (32 or 256) | ~70 | low-medium: both parity-locked; do not unify the hoisted scale read away |
| T4 | `DnRequant` cls_dn_rq 236-268 + `Q8ActRq` q8_actrq_cls 337-369; `Q8kRequant` cls_q8k_rq 272-306 + `Q8kActRq` q8k_actrq_cls 373-403 | Q8_0 pair: DnRequant's tail 255-265 is `Q8ActRq.blk_store` 341-351 verbatim; only the value source `src[eb]` vs `act_mul(gate[eb], up[eb])` and the base `pa.inbase + ..` vs `..` differ. Q8_K pair: `blk_scale` 278-284 duplicated at 374-380 character for character | 33/33 + 35/31 = 132 | `Q8RqT` / `Q8kRqT` with `ACT : bool` gating `def src_val(e)` (`static_if (ACT) act_mul(gate[e], up[e]) else src[e]`) + a `BASE0` constant; the two bind different SSBO sets, so members go behind the axis too | ~42 | medium: crosses the `rq_cls` / `actrq_cls` set-layout families; the `[arch(..#q8-requant-byte-store)]` citations move with the code |
| T5 | `ArAddRmsRq` cls_ar_rq 141-173 + `ClsArAddRmsRqB` cls_ar_rq_b 176-210; `ClsArAddRms` cls_ar 122-136 + `ClsArAddRmsF16B` cls_ar_f16_b 213-227 (all on `ArBase`) | Rq pair 25 of 29 lines identical: `accum_row(0u)` vs `accum_row(rbase)`, `outs[b]`/`outq[base]` vs `outs[qb0 + b]`/`qb0 * 32u + base`. Store pair: `yo[base + k] = ..` vs `outh[rbase + k] = float16(..)`, `base = (gl_WorkGroupID.x + pa.row0) * pa.dim` vs `rbase = gl_WorkGroupID.x * pa.dim` | 15/33/35/15 = 98 | `ROWMAJOR : bool` (the qb0/row0 rebase); `OUT` constant or a `def emit_row` with three `static_if` arms (f32, f16, Q8_0 blocks) | ~25 | low: cls_ar_rq's header pins bit-parity with the split pair - fold order and `q8_quant1` rounding untouched; three `[arch]` citations move |
| T6 | `AtAttnTileT.run` 1676-1764 vs `AtAttn.run` 1776-1852 (`def override run`) | the same flash Q-tile re-authored: `pa.npos` vs `pa.rows`; `qrow = pa.nh * qhs` vs `pa.qd`; `dpl = pa.hs / 32u` vs `pa.dpl`; K/V base `pa.kbase`/`pa.vbase` vs `0u`; gate from `qpanel` vs `outp`; `pa.gated` vs `pa.flags & 1u`; the template guards the rescale/PV with `if (dpl > 1u)` / `> 2u` where AtAttn does o0..o3 unconditionally and guards only `> 4u` | 89 + 77 = 166 | `MIRROR : bool`, `GATE_IN_Q : bool`, one `AtAttnArgs`/`DaAttnBArgs` reconciliation | ~70 | medium-high: the `dpl` guards are an emitted-code difference (4 FMAs vs 1/2/4/8); two different parity bars pinned in comments; only behind both parity cells |
| T7 | `RopeKvStoreT` 6749-6801; `RopeKvStoreBT` 6837-6898; `QknRopeKvT` 6932-7010 (6 leaves) | not full twins (decode token, prefill npos, fused qk-norm), but the `static_if (CLAMP16)` store block is written four times (6781-6791, 6877-6887, 6977-6983, 6997-7007), the `pa.hasb != 0u ? bias[pa.boff + ..] : 0.0` idiom twelve times, the rotation core three times | 53/62/79 = 194 | helper extraction on a common base: `def store_kv2(mo, e0, e1; k0, k1, v0, v1)` carrying the CLAMP16 arm, `def biased(buf; i, boff)` | ~25 | low: inlined helpers leave the emitted code unchanged |

### Near misses - same shape, different algorithm; do not template

| Pair | Why not |
|---|---|
| `Q8Batch` 2607-2725 vs `KqBatchBase.tile_shell` 3480-3511 | Q8_0's `nbb = n / 32` with a `nsteps` outer loop and a `b0 + bu < nbb` tail vs Q8_K's `nsb = n / 256` with no tail; no block-sum term; 4 scalar accumulators vs `float4` + `bs` + two scale planes. A 14th `KqBatch*` leaf would force a superblock alignment the q8 path deliberately lacks |
| `Q8Gemv` 7038-7081 vs `KqGemvBase.gemv_shell` 7171-7195 | two lanes per 32-block (`b = bi0 + lane / 2u`, a `subgroupShuffleXor(idot, 1u)` fold) vs one lane per block (`bb = b0 + lane`); merging changes the workhorse GEMV's access pattern |
| `AtAttnTileT` (SIMT) vs `DaAttnBH128T` (coopmat) | staged f32 tiles at stride 257 + an 8-row online softmax vs `coopmatA_f16_16x16` fragments + a 4-thread-per-row shuffle softmax |
| `MoeCombine` 512-538 vs `DecCombine` 546-566 | windowed read-modify-write with a slot-window predicate, one wg per position, vs a one-row residual add, one thread per element |
| `DnRequant` vs `Q8kRequant` (and the ActRq pair) | 8 lanes x 4 quants with a 3-shuffle amax vs 32 lanes x 8 with a 5-shuffle amax and a `float[8]` stash - different formats; the CROSS pairing is the twin (T4) |
| `DnScanP3` 1437-1468 vs `DnStepFused`'s out-norm tail 1103-1120 | a batched pass over npos rows off a work surface vs a per-head in-register epilogue |
| `EmbGather` 454-476 vs `EmbGatherF32` 479-498 | 5 differing lines, the f32 arm skips binding 1; a template costs more than it saves |
| `ActF16` 407-425 vs `F16Cvt` 429-444 | 8 shared lines, different arity |

### Ranked order (saving x inverse risk)

1. T1 the Mm trio (~230 lines; MmABatch is MmBatch with guards deleted; cells cover all three by name). DONE - `MmBatchT` (BT/EDGE/NP/ASZ; the L and M fragment sets stay two `static_if` arms).
2. T2 the FaCm2 head-size templates (~130; the cleanest axis set - 7 typedefs + 2 constants). DONE - `FaCm2T` (HS/BC + 8 typedefs; the h256 pair's SPIR-V byte-identical, the h64/h128 gain the inert qstride/qhs selects).
   Gate used for both: the kernel suite on the committed and the converted file, a `DASLLAMA_VK_SPV_DUMP` of every kernel diffed across (only the converted kernels differ), and the harness probe rows within noise.
   Batch B, the detect-dupe finds (`utils/detect-dupe` over the file: 224 records, 16 exact clusters, 38 fuzzy pairs - it sees the copied helpers and the thin shells, not a twin whose bodies differ by a guard, and its "exact" collapses LITERALS, so every clone was read before deletion): `iq3s_signed` a free function (ten copies gone), `iq4_dot` a free function (two), `KqBatchIq3Base` carrying the iq3-row scale stage for iq3s/iq3xxs/iq2xxs, `KqBatchIq2Base` carrying the unsigned-strip stage and fold for iq2s/iq2xs, k3 inheriting k6's fold through a one-line `q_off` override (the bodies differed only by the 32 vs 4 literal), `KqGemvIq4nl` on q40's base, and `MoeCmBase` as the one region rail under `Q8Batch` and `KqBatchBase` too. DONE - suite 108 of 108, the kq probe rows within noise on k3 / iq2s / iq2xs / iq3s / iq4nl, 21 kernels' SPIR-V re-emitted (the three families whose bases moved) and every other kernel byte-identical.
   Batch C: T3 - `CmF16Base` carries the f16 32x32 shell (prologue, the 16-MMA step, the scratch bounce store) with `stage` as the leaf's point and `body(nsteps, nblk)` taking the step and block counts the leaf's format reads (q8's eight blocks per step, q40's one superblock); T5's requant pair - `ArRqBase.rq_row(rbase, qb0)` is the loop, the one-row leaf calls it at (0, 0), the batched leaf at its workgroup's row and block base (the arithmetic is the batched form's, the one-row form's `+ 0` folds).
   Not done, by ruling here: T4 crosses the `rq_cls` / `actrq_cls` set layouts (the block store references bindings the two families number differently - a base method cannot name a leaf's binding); T6 changes AtAttn's emitted code below hs 128; T7's shared store helper needs a second base under `QknRopeKvT`, which already derives from `RmsWgBase`, and the bias idiom shortens twelve lines without removing one. The Q40 / Iq4nl batch scale stage stays twice: different bases, a shared-array write no free function can carry.
3. T3 CmF16Batch/KqQ40CmF16 (~70; produces the `stage_a`/`stage_b` shape a per-format family wants).
4. T7 the rope store/bias helpers (~25; opportunistic with any rope touch).
5. T4 / T5 (~67 together; T4 crosses two set-layout families, T5 carries a bit-parity contract).
6. T6 AtAttn vs its own template - only behind both parity cells, or leave it.

## 2. Metal - `dasllama_metal_kernels.das` (175) + `dasllama_metal_prefill.das` (121), 296 `[metal_dispatch]` classes

| Bucket | Count |
|---|---|
| stamps of a `class template` (68 templates) | 173 |
| derived from a plain base (`def abstract` scaffold: `MetalMmTileBase`, `MetalMoeMulMmBase`, `MetalRopeStoreQ8Base`, `MetalRopeStoreTq4Base`) | 14 |
| hand-written twin-cluster members | ~44 |
| singletons | ~65 |
| hand-bound `[metal_kernel]`-only (`dasllama_metal_gemm.das`, `options _metal_manual_dispatch`) | 2 |
| dispatch classes in the tower / ASR / MTP / TTS / whisper files | 0 (pure drivers over generated `enc_*`) |

Three authoring generations coexist: gen-0 copy-paste per format (`MetalKqGemv*`, `MetalKqMvB8*`, `MetalMoeGemv*`); gen-1 one template with one `bool` constant per format in a nested `static_if/else` ladder (`MetalKqMulMmK45T`, 12 bools, 11 levels); gen-2 a shell with one overridable stage (`MetalKqMulMmSplitTensorBase.stage16`, `MetalMoeMulMmBase.stage_a`; `ARCHITECTURE_GPU_PREFILL.md` sec.2.2aa / 2.2g) - the shape Vulkan's `KqCm2BatchT` mirrors. Gen-0 and gen-1 are already banned by `REVIEW_GPU.md` ("kernel twins stamp one `class template`"); the clusters are standing debt.

### Twin clusters (lines in `dasllama_metal_kernels.das` unless `[pf]`)

| # | Cluster | Members | Lines | What differs | Axis | Save | Risk |
|---|---|---|---|---|---|---|---|
| 1 | KqMv B2/B4 per-format templates | 12 templates `MetalKqMvK2T` 8368, `K3T` 8615, `Iq3xxsT` 9385, `Iq3sT` 9555, `Iq2sT` 9727, `Iq2xsT` 9899, `Iq2xxsT` 10071, `Iq4xsT` 10237, `Iq4nlT` 10398, `K4T` 10561, `K6T` 10771, `K5T` 10890 (24 stamps) | 80-99 each, 1062 | Iq4xs vs Iq4nl: ONE functional line (`let dl = float(kdh[blk]) * float((int(kscb[blk*16u+tx]) ^ 128) - 128)` vs `float(kdh[blk*8u+tx])`); Iq2s vs Iq2xs: 8 lines; the NR/NRU/TILED shell, `sumf : float[NR]` and both TILED writeback arms byte-identical | one `MetalKqMvT` shell + `def abstract stage_w16`; per-format `def override`; NR/NRU/TILED stay | ~830 | low-med: TILED branch-duplicates the b-loop for a measured +2% (k4) / +0.5% (k6); "B4 stamps byte-identical" must be proven on the generated MSL |
| 2 | KqGemv single-column per format | 14 standalone `K4` 7985, `K5` 8066, `K5C` 8140, `K6` 8219, `K2` 8287, `K3` 8548, `Iq4xs` 8805, `Iq4nl` 8862, `Iq3s` 8917, `Iq3sF4` 8973, `Iq2s` 9040, `Iq2xs` 9108, `Iq3xxs` 9264, `Iq3xxsF4` 9319 (`MetalKqGemvIq2xxsFam` already templated) | 55-81 each, ~1030 | Iq4xs vs Iq4nl one line; the x-stage / `sumy` / simd reduce / writeback shell (~45 lines) identical in all 14 | one shell + `def abstract dot_block`; F4 as `@template_constant F4` + `@workgroup @template_gate = F4 gridf` (`MetalKqGemvIq2xxsFam` proves it) | ~630 | med: `K5C`, `Iq3sF4`, `Iq3xxsF4` are per-box crowns (`race_kq_rows`, `race_gemv_f4_twin`) - separate stamps; the F4 forks bundled a second change (`first_row * 2u` -> `* 4u`, `sumf[2]` -> `[4]`, `ib += 4u` -> `2u`) = two constants (F4 + ROWS) |
| 3 | KqMvB8 per format | 10 standalone `B8K2` 8477, `B8K3` 8727, `B8Iq3xxs` 9489, `B8Iq3s` 9660, `B8Iq2s` 9826, `B8Iq2xs` 9998, `B8Iq2xxs` 10167, `B8Iq4xs` 10339, `B8Iq4nl` 10500, `B8K6` 11102 (`MetalKqMvB8K45T` templated) | 59-79 each, ~800 | the same deltas as cluster 1; the tg-staged X-panel shell byte-identical | extend `MetalKqMvB8K45T` into the format-abstract shell; `stage_w16` shared with cluster 1 | ~470 | low |
| 4 | SqAttn single / batched / decode families | 10 templates: `SqAttnKvT` 501 / `BKvT` 2452 / `DKvT` 3110; `QuantT` 558 / `BQuantT` 2516 / `DQuantT` 3335; `PartKvT` 626 / `PartBKvT` 2588; `PartQuantT` 692 / `PartBQuantT` 2663; `RopeStoreKvT` 1420 / `RopeStoreBKvT` 3249 | ~45 each, ~450 | the batched arm adds `@ssbo @binding = 4 @off = "rtoff" rt : array<uint4>`, `let rrow = rt[bb]` / `cnt = rrow.w`, `qstride` addressing - 31 of 45 lines differ, one axis | `@template_constant BATCHED` + `@template_gate = BATCHED rt` - `MetalSqAttnCombT` ships exactly this | ~250 | low-med: the D forms are a third arm |
| 5 | MoE GEMV per format | 6 standalone `Q8` 6613, `K4` 7293, `K5` 7401, `K6` 7486, `Mx4` 7566, `Q51` 7644 | 65-108, 483 | the dot loop line-identical to `MetalKqGemv*`; delta = the gather prologue (`eb = sel[st*ss+slot]*esb`, `xoff`, `y0`), `MoeGemvArgs`, the `sel` binding | cluster 2's shell + `GATHERED : bool` + `@template_gate = GATHERED sel` | ~230 | med: MoeGemvK4's `float4` x view is a measured 2.25x (142 -> 321 wGB/s) - its own axis |
| 6 | KQ tensor mul_mm K45/K6 x Db `[pf]` | `K45TensorT` 472, `K45DbT` 592, `K6TensorT` 703, `K6DbT` 782 (13 stamps) | 63-85, 296 | K45 vs K6 = the decode; Db = `twb : float16[6144]` vs `[9216]` + the decode moved verbatim into `stage_chunk` with a ping-pong offset - the k4/k5/k6 decode exists twice | derive all four from `MetalKqMulMmSplitTensorBase`; `DBUF : bool` + a `TWBN`-sized `@workgroup`; the format's `stage16` once | ~165 | med: the Db forms carry `[metal_kernel(float_a_ok=true)]` and pin `XT = float16` (the sanctioned float-A list, `ARCHITECTURE_GPU.md` sec.2.2b) |
| 7 | dense mul_mm base shells | `MetalF32MulMm` 5847, `MetalQ8MulMm` 5924, `MetalBf16MulMm` [pf] 39, `MetalKqMulMmK45T` 6001 | 63-348, ~565 | the 32x64 shell (`mBase = gl_WorkGroupID.x * 32u` .. `acc_quad(aB, bB, ma, mb, mc)` .. the 8-way `simdgroup_store`) repeated four times verbatim | `MetalMoeMulMmBase` already factors this shell into `run` + `def abstract stage_a`; the four dense classes derive from a twin base | ~145 | low |
| 8 | `MetalKqMulMmK45T`'s 12-format ladder | 1 template 6001-6347 + 12 stamps 6348-6416 | 348 | 12 independent bools (`QH`, `SIXBIT`, `IQ4XS`, `K3`, `IQ3S`, `IQ3XXS`, `IQ4NL`, `K2`, `IQ2S`, `IQ2XS`, `IQ2XXS`) in an 11-deep nest; `MetalKqMulMmIq4nl` must set `IQ4XS` AND `IQ4NL` | `def abstract stage16` (the sec.2.2aa shape); each format one ~18-line override | ~120 + removes the coupled-bool trap | med: the base kernel every box falls back to; parity-locked per format by the kernel-unit gates |
| 9 | rope-store q8/tq4 batched | `RopeStoreQ8` 1657 / `BQ8` 3414; `RopeStoreTq4` 1762 / `BTq4` 3477 | 48/59, 67/72 | `q8_rope_q(.., 0u, 0u)` vs `(.., qbase, tcb)`; the batched binds `rt : array<uint4>` | `BATCHED` + `@template_gate` on the existing bases | ~100 | low |
| 10 | argmax rows | `MetalArgmax` 1090 / `MetalArgmaxRows` 1169 | 52/55 | `lg[j]` vs `lg[base + j]`, `tok[0]` vs `tok[row]` | `ROWS : bool` | ~50 | low |
| 11 | Q8 mv B2/B4 | `MetalQ8MvB2` 1957 / `MetalQ8MvB4` 2018 | 57/64 | lane maps and strides (`% 16u` vs `% 8u`, `*34u` vs `*17u`, `acc : float[2]` vs `[4]`) | the sibling `MetalGemvB24T` (NR/PANEL/WIDE) exists next door - the q8 form was never converted | ~55 | low |
| 12 | `MetalQ8GemmBSplitT` / `MetalQ8Gemm64B` | 2182 / 2364 | 99/83 | `tw : float16[1024]` vs `[2048]`, `mc[4]`/`ma[2]` vs `mc[8]`/`ma[4]` | `TILE_N` - `MetalQ8GemmTensorT` already parameterizes this | ~40 | med: the hot staging loop |
| 13 | dequant K4/K5 vs K6 `[pf]` | `MetalDequantK45T` 223 / `MetalDequantK6H` 279 | 50/40 | the kmask sc/mn decode vs the split d x int8 sub-scale | a `SIXBIT` arm, or the sec.2.2aa scaffold | ~30 | low |
| 14 | cross V f32/f16 | `MetalCrossVx` 5480 / `MetalCrossVxH` 5515 | 32/32 | ONE statement (`vx[..] = v` vs the f16 clamp) + the array type | `typedef VT` + `CLAMP16` (as `MetalQ8GemvQkvRsF16/F32`) | ~30 | none |
| 15 | q8 mul_mm tensor / double-buffered `[pf]` | `MetalQ8MulMmTensorT` 102 / `MetalQ8MulMmDbT` 148 | 26/27 | `twb : float16[6144]` vs `[9216]`; `tmm2d_q8u_f32(.., 64u)` vs `128u` | `BK` + a `TWBN`-sized `@workgroup` (`MetalGemvB24T`'s `txp : float4[PANEL]` proves template-sized tgmem) | ~22 | low |
| 16 | spectrum mag/power `[pf]` | `MetalG4aMag` 3442 / `MetalQ3aPow` 3599 | 26/30 | `sqrt(re*re + im*im)` vs `re*re + im*im` | `SQRT : bool` | ~22 | none |
| 17 | bias elementwise | `MetalBiasAddRes` 5781 / `MetalBiasGeluLut` 5800 | 16/17 | `x += y + b` vs `x = gelu_lut(x + b)`; an extra `y` binding | `def abstract bias_op` + `@template_gate = HASRES y` | ~14 | none |
| 18 | `dasllama_metal_gemm.das` pair | `MetalQ8Gemm` 39 / `MetalQ8Gemm64` 129 | 90/92 | `ta/tb : float16[1024]` vs `[2048]`, `ntileN`, `c00..c11` vs `mc[16]` | `TILE` - but hand-dispatched, no lens benefit | ~40 | med, lowest value |

Weak, do not merge: `MetalSuppressRow` 11315 / `MetalSoftcapRow` 11333 share a 6-line guard only.

### Near misses - do not template

`MetalAttnQK` / `MetalAttnQKMm` / `MetalAttnQKMmTensorT` (three algorithms); `MetalTowerFlash` vs the QK/Rowstat/AV slab trio (sec.2.2w rules them distinct); `MetalG4aAttn` vs `MetalCnAttn` (causal window + two-pass softmax + softcap vs bidirectional online softmax with biases); `MetalG4aDw` vs `MetalCnDw` (radius, boundary policy and epilogue differ); `MetalRopeStoreQ8Base` vs `MetalRopeStoreTq4Base` (thread-per-block amax vs a threadgroup FWHT stage); `MetalEmbedQ8` vs `MetalEmbedK6`; `MetalSqAttnCombT` vs `MetalSqAttnCombD` (two-pass with a sink vs single-pass log-sum-exp); `MetalRmsNormT` vs `MetalLayerNorm`; `MetalCrossKx` vs `MetalCrossVx` (inverted layouts); `MetalDnConv` vs `MetalDnConvHist` (opposite data flow); `MetalQkNorm` vs `MetalDnL2Norm`; `MetalQ8Gemv` vs `MetalQ8GemvT` (reduction split across simdgroups vs one row per simdgroup); `MetalWdecAttnPart/Comb` vs `MetalSqAttnPart*/Comb*`; `MetalG4aLogMel` vs `MetalQ3aLogMelT` (a `readwrite` role cannot flip to read + write bindings); `MetalMoeMulMmQ8TensorT` vs `MetalMoeMulMmKqTensorBase` (ruled in sec.2.2g).

### Ranked order

| Rank | Action | Save | Risk |
|---|---|---|---|
| 1 | KqGemv + KqMvB2/B4 + KqMvB8 onto ONE format-abstract shell (clusters 1+2+3): one `def abstract` decode per format, three shells consuming it; do the 12 templates of cluster 1 first | ~1930 | low-med |
| 2 | SqAttn `BATCHED` axis (cluster 4) - a mechanical repeat of `MetalSqAttnCombT` | ~250 | low |
| 3 | `MetalKqMulMmK45T` ladder -> `stage16` scaffold (8) + tensor K45/K6 x Db (6) | ~285 | med |
| 4 | dense mul_mm shells onto a `MetalMoeMulMmBase` twin (7) | ~145 | low |
| 5 | MoE GEMV `GATHERED` axis (5), the x view its own axis | ~230 | med |
| 6 | the zero-risk singles (14, 11, 10, 9, 15, 13, 16, 17) | ~325 | none-low |

Total addressable about 3000 lines, ~44 hand classes into ~10 shells.

### Cross-backend notes

Copy from Metal: `@template_gate` on `@workgroup` fields gives per-stamp threadgroup sizing (a `daslib/typemacro_boost` facility, so Vulkan's `vkd_wg_fits` footprint gate is per-stamp for free - `KqCm2BatchT` already uses it); the constant-table hoisting (`iq2s_gw` .. `iq2xxs_gw` are all-literal `fixed_array` locals lowered to program-scope constants, no binding, no tgmem - confirm the SPIR-V emitter's behaviour before another staged slab; on Vulkan the constant composite read lane-serially, hence `kq_grid_dev`); the overridable-stage granularity is chosen (sec.2.2aa: `stage16` = the base GEMV arm's own granularity, so a format's decode ports verbatim - the same idea as Vulkan's `decode` / `decode_v4`).

Avoid: the `MetalKqMulMmK45T` shape (one bool per format in an 11-deep nest; `static_elif` exists - `src/parser/ds2_parser.ypp:193` - but a 12-way format axis wants the abstract-stage scaffold, not a flatter ladder); the gated-field ternary rule bites both backends (a `@template_gate`d field may be named only inside a `static_if` arm on its own axis - the price is byte-duplicated arms, e.g. `MetalKqMvK4T`'s TILED); Metal's `tgmem=` string couples to `<LeafClass>_<method>_msl_tgmem`, so converting a hand class to a stamp changes it - Vulkan has no such coupling and must not grow one.

### Lint opportunity

`REVIEW_GPU.md` bans copy-pasted twins in prose; nothing mechanical catches them and 44 standing violations survive. A `modules/dasLLAMA/dasllama/REVIEW.das` gate flagging two dispatch classes in one module whose kernel bodies exceed a normalized-token similarity threshold (identifier-renaming-insensitive, the `detect_duplicates` canonical form scoped to kernel bodies) without sharing a base would have caught every cluster above at the diff that introduced it.

## 3. The shape a new GEMM family takes (the integer MMQ tile) - from `KqCm2BatchT`

| Need | Copy from | Spelling |
|---|---|---|
| 128x128x32 and a 64-token variant | `KqCm2BatchT.BN`, plus the `BM` axis the Mm trio lacks | `@template_constant BN` AND `BM`; bake only BK - `KqCm2BatchT` fixes BM = 128 and paid for it in the Mm fork |
| the partial-column path | `STILE` + `FLO` | `STILE : bool` selecting the padded layout, as `Q8Cm2SBatch` 6147-6153 does |
| aligned vs edge tiles | `MmABatch` (today a whole class) | `ALIGNED : bool` gating the staging guards and the store arm - one class |
| fragment / accumulator types | the cm2 leaf stamps 6129-6181 | `typedef BT / ACC / ACCW` per leaf; the MMQ adds `typedef DEC4` (the packed-int8 word the decode returns, where cm2 returns `half4`) |
| 13 per-format decode arms | `K4Cm2T` .. `Iq2xxsCm2T` 5509-6126 | one `class template <Fmt>Cm2T : <Base>` per format: `typedef BLK` (16-bit-lane block struct), `typedef ST`, `override BLKW`, a `decode` method; the leaf carries only width typedefs |
| per-format codebook LDS that must not cost the others | 5127-5138 | `IQ3GRID : bool` + `@workgroup @template_gate = IQ3GRID iq3s_gridc : uint[512]`; stage from `kq_grid_dev()` (7097-7131), never a constant composite |
| two whole tile bodies in one class | the `KHR` axis 5205-5209 | `static_if (KHR) { khr_tile(..) } else { cm2_tile(..) }` - only the reachable body materializes |
| MoE region walk + split-k | `MoeCmBase.region_rec` 4284-4288 and 5151-5165 | inherit `MoeCmBase`; the `ksplit`/`ybase`/`ptiles` block verbatim; `SplitKReduce` unchanged |
| the four-wide escape hatch | `DECVEC` / `DECV4` 5125-5126 | an int8-lane decode wants the same per-format switch from day one |
