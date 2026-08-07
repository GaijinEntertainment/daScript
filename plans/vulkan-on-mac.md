# Vulkan on Mac — MoltenVK bring-up, metal oracles, the lens hoist, the asymmetry closure

The mac session ruled in `modules/dasLLAMA/followup_vulkan.md` item 10, scoped 2026-08-07 after
the peek. One PR, long arc, on `bbatkin/vulkan-on-mac`. Order: **(a) MoltenVK bring-up →
(oracles) metal CPU-oracle suite → (c) dispatch-lens hoist → (b) kernel-model asymmetry
closure.** Reification is the follow-on arc; this arc builds the surface it stamps into.

## The peek, as settled (2026-08-07)

Full census of both kernel corpora + a cross-backend body comparison. What it ruled:

- **Metal corpus: 137 classes / 8,267 class-body LOC** (101 kernels.das, 34 prefill.das,
  2 gemm.das; only the 41 in kernels.das are lensed). One kernel per class is universal;
  3 classes use inheritance, everything else is flat. Four two-axis matrices — SqAttn
  {4 formats × 5 tiers}, KqMv {3 batch × 3 k-quants}, RopeStore {4 formats × 2 tiers},
  MulMm {formats × lowerings} — hold 58 classes / 3,634 LOC expressing ~14 distinct bodies.
- **Vulkan corpus (the precedent): inheritance carried the dedup** (~914 LOC saved, 66% of it
  in the two kq families at 49–57%); `family=` saved zero source LOC but collapsed 12 classes
  into 4 generated surfaces and made 15 driver call sites format-agnostic; the multi-kernel
  form went entirely unused — 41 annotations map 1:1 onto 41 classes.
- **The design rule for merging kernels:** type axis → typedef stamp; small format delta
  (K4↔K5 qh merge, ~25 lines) → `static_if`; big format delta (K6 layout, q8/tq4 requant) →
  leaf override; pass axis (Part→Comb, Count/Bucket) → multi-kernel in one class. Merging
  big-delta formats via `static_if` is forced inner-loop spaghetti — don't.
- **Cross-backend body sharing is real for one category only:** elementwise + row-reduction
  kernels are 55–85% identical (the RMS reduce is line-for-line modulo `simd_sum` vs
  `subgroupAdd`); quantized GEMV/GEMM and attention are 10–20% — different algorithms
  (metal f32 activations vs vulkan int8-requant `sdot4`), not different syntax. Blocker for
  the shareable family: `shader_lingua_franca` has zero subgroup content — the ~10 subgroup
  symbols are declared twice under identical GLSL names (metal_builtins vs spirv_builtins).
- **rsqrt vs 1.0/sqrt** ledgered as followup_vulkan item 18 — decide by measurement; until
  then any shared RMS body carries the spelling as an explicit knob.

Reification cluster table (the follow-on arc's worklist, ranked by LOC-saved ÷ spaghetti):
SqAttn 23→~6 cls (~700 LOC; f16/f32 is typedef-only — the stage helpers are already
`auto(KT)`-generic), KqMv 9→4 (~450; batch width is a stamp constant: `range(2)`→`range(B)`),
MulMm/MoeMulMm 16→~8 (~430, of which ~230 is verbatim skeleton copies of the existing
`MetalMoeMulMmBase`), RopeStore 10→~5 (~300), MoeGemv 6→base+5 (~200, inheritance not
reification). Ceiling: 137 → ~60 classes, −35–42% LOC.

## Leg (a) — vulkan tier green on this M1 under MoltenVK

Correctness only; Metal stays the fast path on Apple. Environment already present: brew
molten-vk 1.4.1 + vulkan-loader, `vulkaninfo` on PATH; `modules/dasVulkan` checkout moves to
master (593a208 — PR #79 put `append_portability_subset` at all 8 device-create sites; the
local one-off commit is superseded); the module descriptor is parked as
`dasModuleVulkan.shared_module.stale-jul28` and gets renamed back.

Expected shape of the work: build the module on macOS, get a device up, then capability-gated
declines — no coopmat, the 32 KB `maxComputeSharedMemorySize` declining the dn/at chains —
and bisect the known `ffn_vs_ref` red per-kernel via the model-less kernel-unit suite.
Finish line: kernel suite + `test_vulkan_tier*` + `image_vulkan` honest-green (declines are
honest), and a couple of the small models we actually run serving "on vulkan and sane" —
no perf bar. Note followup item 16: vulkan arms need direct dastest + `-load_module`, never
`run.das`.

### Leg (a) bring-up log (2026-08-07)

- Ladder green: boost compute example end-to-end on MoltenVK (daslang-authored SPIR-V,
  256/256 correct); kernel suite first run 27/40.
- LANDED: per-class @workgroup footprint gate in the lens (`vkd_wg_fits` before the pipe
  build — at/dn chains decline by name; suite 37/40, 0 errors; footprints byte-match
  MoltenVK's own numbers). LANDED: first-6 eyeball dumps in the suite compare helpers.
- **MoltenVK 1.4.1 BUG, fixed by 1.4.2 (brew upgrade — PIN ≥1.4.2):** compute dispatch with
  static workgroup memory over ~15.6-16 KB is SILENTLY DISCARDED — no validation error, no
  MVK log, fence signals, copies run, kernel never executes. Bisected via marker-probe
  classes (probe files `tests/_probe_{cls,batch}_m1.das`, delete before PR).
- **RESOLVED — the batch-tile all-zero red was MoltenVK's Metal argument-buffers path**
  (its Apple Silicon default): dispatches launch but device stores never land, silently.
  `MVK_CONFIG_USE_METAL_ARGUMENT_BUFFERS=0` (discrete bindings) fixes it outright; tier
  init forces it via the env rail (`MoltenVkEnv`, tri-state — an explicit user value wins,
  and `=1` is the standing one-env repro: kernel suite 37/40). LANDED with the rail knob +
  ENVIRONMENT.md row + decline feints in the three tier arms that drive dn/attn directly.
- **LEG (a) BAR MET (2026-08-07):** kernel suite 40/40, tier 36/36, cm2 6/6 honest feints,
  image-vulkan 4/4; `harness/parity.das` CPU==GPU GEN_IDS EXACT (32/32 tokens) on
  Llama-3.2-1B Q8_0 AND Q4_K_M native k-quant — resident driver, device prefill + decode,
  at/dn chains honestly on the CPU rail.
- Tails: (1) file the MoltenVK argbuf-miscompile upstream (probe files
  `tests/_probe_{cls,batch}_m1.das` are the repro seed — keep until filed, delete before
  PR); (2) GPU-vs-GPU suite arms (ar twin-vs-seam) are vacuous-prone — when both kernels
  are dead they match trivially (bit us under the 1.4.1 cliff); sentinel-prefill them;
  (3) a `--quant kq` parity run vacuously "passed" on empty streams before the wc -l check —
  the harness should exit non-zero when GEN_IDS was never printed.

## Leg (oracles) — per-class CPU oracles for the metal corpus, loud and eyeballable

Ruled 2026-08-07: "yes, very much so. all the way to text to log. if its broken i wanna know
yesterday. and i wanna be able to eyeball."

Today metal has stage-level oracles only (`test_metal_decode_kernels` — sq_attn stages, f16
codec; `test_metal_prefill_kernels` — rmsnorm/rope/swiglu/attention trio/blob GEMV;
`test_metal_gemm`); the KqMv 9, decode MulMm 4, MoeGemv 6, RopeStore q8/tq4 and the MoE
prefill family ride token-parity/support-matrix/census only — exactly the vacuous-parity
exposure vulkan's item 15 documented. Build the per-class oracle bar the vulkan suite already
has (`test_vulkan_kernels` is the donor pattern back-ported to its own donor):

**Census (2026-08-07, full 137-class cross-reference):** 32 DIRECT-ORACLE (23%),
17 HELPER-ONLY, 88 NONE (64%). Zero-direct families: RopeStore (9), KqGemv/KqMv (13),
batch GEMM q8 + tensor twins (7), KqMulMm (6), fused QKV-GEMV/W13Sw (5), batched GEMV
B-form (4); every batched (`*B*`) decode class and every Metal-4 tensor twin (12) is
NONE/HELPER-ONLY; elementwise/misc decode is 12/15 NONE (AddRms, PreAddRms, QkNorm, Embed,
Argmax, CopyRow, Suppress/Softcap, FwhtUnsign…); MoE prefill plumbing (RouterB, Count,
Bucket, Reduce, SwigluOaiPf) all NONE. DeltaNet is the model family: 8/9 DIRECT already.
Lensing ≠ coverage: 21 of the 41 lensed classes are NONE.

**Suite shape:** shared `_metal_kernel_common.das` fixture module (buf helpers, mismatch
compares with the first-6 gpu/cpu eyeball dump + shape line, synthetic quant planes) + new
per-family files joining the `kernels` suite in run.das: `test_metal_attn_kernels`
(codec twins + batched + D family), `test_metal_rope_kernels` (RopeStore + FwhtUnsign),
`test_metal_gemv_kernels` (KqGemv/KqMv/MoeGemv-kq/B-forms/fused QKV+W13Sw),
`test_metal_gemm_kernels` (batch GEMM, KqMulMm, MulMm/MoeMulMm twins),
`test_metal_misc_kernels` (elementwise + MoE plumbing + prefill misc). Drive =
`pipeline_from_source` on the emitted `metal_*_msl` globals (the established model-less
pattern); Metal-4 tensor twins feint where the device lacks tensor support (M4 box runs
them). Known boundary, ledgered: no kernel-unit arm drives the lensed `enc_*` builders, so
binding order/grid arithmetic stays parity-covered — an enc_-drive via `metal_decode_init`
is a possible follow-on, not this leg's bar.

- Model-less, synthetic data, seconds per cell, standard dastest rail (metal tests run fine
  under `run.das`).
- **A mismatch prints an eyeballable dump to the log**: first-N failing indices with cpu
  value / gpu value / abs+rel delta, plus the arm's shape line — enough to tell tolerance
  drift from garbage at a glance, before any rerun.
- Ordering constraint: this leg lands BEFORE leg (b) restructures metal classes — the suite
  is the safety net for the merge.

**Oracle-leg progress (2026-08-07):** every new file is committed green with a
negative-control run (poisoned oracle → red with dumps) before the poison is reverted.
- ✅ `test_metal_rope_kernels` — RopeStore all 9 + FwhtUnsign (byte-exact q8/tq4 mirrors vs
  quantize_*_row; whole-arena sentinel compares on batched row tables).
- ✅ `test_metal_gemv_kernels` — KqGemv K4/K5/K5C/K6, KqMv B2/B4/B8×K4/K5/K6, MoeGemv kq
  trio (dequant_k*_plane_superblock + double dot; envelope-aware bar 2e-4·Σ|w·x|).
- ✅ `test_metal_misc_kernels` — CopyRow, Argmax (exact ties), EmbedQ8/K6 (bit-exact),
  AddRms/AddRmsB/PreAddRms, QkNorm (in-place + V-fusion), Geglu, Suppress/Softcap.
- ✅ `test_metal_attn_kernels` — the SqAttn matrix is COMPLETE (23/23 classes direct):
  f32/q8/tq4 codec twins (single + split-K + comb), the 9 batched twins (ragged row-table
  arenas; one authoring bug caught — arena row spacing < deepest KV slab), and the D family
  (DF16/DF32/DQ8/DTq4 + parallel CombD; D reuses the batched fixture at nheads=2 kv_mul=2
  hs=128 — same kv_dim; grid (nheads, ceil(nsgs/4), B) × 128).
- ✅ `test_metal_gemm_kernels` — KqMulMm K4/K5/K6 + _t tensor twins, Q8MulMmT,
  Bf16MulMm/T (9). kq/env helpers now shared via `_metal_kernel_common`. **Corrected
  assumption: the M1 toolchain compiles AND correctly executes ALL Metal-4 tensor
  kernels** — the capability-skip (visible to_log; `feint` prints nothing by design, so
  never grep for it) never fired; every tensor twin ran and matched its oracle. The
  race crowns simdgroup on M1 for SPEED; correctness coverage is live here.
- ✅ batch GEMM q8 (7, in `test_metal_gemm_kernels`): Q8GemmB[T] dense + fused-QKV ys
  stride (gap columns sentinel-asserted), Q8GemmBSk[T]+SkReduce as the production
  two-dispatch chain (part planes AND reduced y checked; ksplit 2 + kbn==1), Q8Gemm64B[T].
  Shared q8 fixture q8_blob_fill/q8_x_fill. **Poison-size lesson: an additive oracle
  poison sums like a random walk while the envelope bar grows linearly in k — a +0.01
  poison only tripped the shortest slice; size poisons to beat rel·env at the LONGEST dot.**
- ✅ MoE gathered mul_mm (7, same file): MoeMulMmK4/K5/K6 (k6 d-tail binding),
  MoeMulMmQ8[T], MoeMulMmMx4[T] (exact e8m0 decode in truth; bias fold both ways).
  Padded-CSR MoeFix carries gather=1 bkt indirection with the kernels' last-live-entry
  clamp, a zero-count expert, 2-col-tile shape; twins contiguous-only. All 11 arms
  negative-controlled in one +10 poison of moe_mm_truth.
- ✅ attention GEMM pair (2, `test_metal_gemm_kernels`): AttnQKMm[T]/AVMm[T] — causal +
  window block-skip sentinels, GQA, 2-slab hs; QK arms pin npos==np32 (the tensor twin
  stages K pads RAW where the base guards); the AV npos<np32 arm proves the V pad guard
  with garbage pads.
- ✅ fused QKV rope-store + W13Sw (5, `test_metal_gemv_kernels`): Q8GemvQkvRsF16/F32 (one
  stacked blob at per-segment offsets, norm/neox remap, bias; qo and the f32 mirror
  BIT-exact), Q8GemvW13Sw silu+gelu, GemvW13SwB2/B4. **The B twins' first run caught a
  missing tgmem bind in the GATE — panel kernels read garbage tgmem silently; every
  @workgroup-carrying kernel's gate must mirror the production kn_tgmem.**
- ✅ fixed-B GEMV twins (4, same file): GemvB2/B4 (x-staged panels, BIT-exact),
  Q8MvB2/B4 (streamed skinny, 64-tg; padded xs4, nrows store gate, pad-column clamp).
- ✅ MoE plumbing (8, `test_metal_prefill_kernels`): SwigluOai + Pf twin (ghost rows
  asserted untouched), G4RouterNorm, MoeWScale (uint-bit compare on the mixed sel buffer),
  MoeRouterB, MoeCount→MoeBucket chained (cnt/basep/bkt/inv all exact), MoeReduce.
  Poison lesson: derived-truth compares need their OWN poison (a cnt poison left
  basep/bkt/inv unproven — their wants derive from sel).
- ✅ prefill misc (5, same file): PleGatherQ8/PleFinish, AddBiasRows, PfCat2, DnBa
  (fast stripe + tail). **CENSUS COMPLETE — every metal kernel family now carries direct
  per-class CPU-oracle gates in the kernels suite (7 files, ~180 gates, all
  negative-controlled).**

## Leg (c) — hoist the dispatch-lens micro-grammar into dasllama_kernel_access ✅ DONE 2026-08-07

The ~80 duplicated lines (`mk_uint_cast`, `is_digit_tok`, `role_ok`, `derived_role`, the
`mk_grid_dim` core, `param_type`) — one owner; vulkan's grid folds any integer literal,
metal's only "1", and the hoist ends that divergence. Both lenses already require the module.

Landed: the six helpers are public in `dasllama_kernel_access` (a "dispatch-lens
micro-grammar" section); both lenses lost their privates and call the shared ones. Bonus
dedup found in the target itself: kernel_access's private `all_digits` was a third copy of
`is_digit_tok` — deleted; and the metal census's inline re-derivation now calls
`derived_role`. The shared `mk_grid_dim` is vulkan's superset form, so `[metal_dispatch]`
grids now fold any integer literal (previously only "1" — other literals were an unresolved
variable, so no compiling code changes meaning). Metal `param_type` switched from
`qmacro_type` to direct `TypeDecl` construction (the vulkan spelling, same scalars).
Verified: kernels suite 7/7 files green, vulkan kernels 40/40, tier 36/36, cm2 6/6.

## Leg (b) — kernel-model asymmetry closure (metal catches up to the vulkan lens)

Port `family=` + multi-kernel to `[metal_dispatch]` TOGETHER (the vulkan evidence: family
surface sharing carried the value, multi-kernel is the substrate reification stamps into),
fix the metal lens grid-literal infer trap, and adopt inheritance where the copies are
verbatim today. Wider merging (the pass-sets, the format families) waits for reification;
this leg only builds the mechanism and takes the free wins the oracle suite covers.

**Status 2026-08-07:**
- Grid-literal trap: ✅ fixed by the leg (c) hoist (shared `mk_grid_dim` folds any literal).
- **Multi-kernel: ✅ LANDED as `kernel=`** — N `[metal_dispatch]` instances on one class
  (one bracket group; stacked brackets don't parse), each naming its `[metal_kernel]`
  method; every instance emits its own builder, binds the full field set, and stages ITS
  kernel's derived access; declared roles must COVER every kernel (vk's rule). Proven by
  `tests/_mtl_toy.das` (+ gate in the misc kernels file): ast_dump shows divergent staging
  (shift stages no xs), cover-rule poison refuses at compile, oracle poison reds at runtime,
  suite green under HAZARD_STRICT. `family=` was NOT ported — it is vulkan-specific plumbing
  (the shared VkdClass global + set_ builder + pipe-slot ordinals); metal's enc_ builder is
  the entire generated surface, so there is nothing for a family to share. If reification
  later wants cross-class PSO/source sharing (followup_general #9's duplicate-PSO shape),
  that is a PSO-lifecycle question, not a lens-surface one.
- **MoeMulMm inheritance: NOT the planned free win — needs a ruling.** The in-source truth
  (`followup_general.md` #8 + the class bodies) contradicts "verbatim/zero-risk": K6 caches
  superblock scalars ACROSS loop iterations (`sv`/`dall`, every 8th kb), Q8 walks advancing
  weight pointers, and Mx4 (not in #8) needs a pre-loop hook (vtab staging into @workgroup +
  bias-seeding the accumulator LOCALS). #8's named unlock (pointer-typed helper params,
  `@threadgroup`/device) HAS since landed in msl_emit, and Q8/K6 are also expressible
  without it (index-math / reload-per-kb) — but every route changes hot prefill kernel
  inner loops (measurement discipline applies, oracle gates cover correctness only), and
  Q8/Mx4 joining the base means renumbering their cnt/basep/bkt bindings 6/7/8 → 7/8/9
  (production encoders + oracle gates churn). Options: (i) defer to reification where these
  families get restamped anyway (recommended), (ii) a measured mini-leg now with
  interleaved A/B benches per format.

## Ledger

- Subgroup lingua-franca lift (~10 symbols out of metal_builtins/spirv_builtins into
  `shader_lingua_franca`): in-arc if cheap, unblocks cross-backend sharing of the
  elementwise/reduction family later. The stale "if dasSpirv grows subgroup support"
  comment in metal_builtins.das:17 goes with it.
- rsqrt measurement (followup_vulkan item 18) — separate, needs bench discipline.
- Vulkan-side leftovers spotted by the census, for the reification arc: the 7 `MoeCmBase`
  leaves (799 LOC over a 10-LOC base, three near-identical `Mm*Batch` bodies), and the
  batch-tile shell existing twice (`Q8Batch` standalone vs `KqBatchBase`).
- PR #3644 body overclaims two family shapes vs source: flash attention shares 2 leaves
  (not 3 — the h128 coopmat twin is standalone, corpus comment says so), deltanet is 2-on-base
  + 3 standalone (not a 5-kernel family). Merged PR text stays; recorded here so the
  reification arc doesn't plan against the claim.
- `lens-ify the 96 unlensed metal classes` (prefill + gemm) remains the standing ledger item
  from the role-infer arc — leg (b) does not require it, reification of prefill families does.
