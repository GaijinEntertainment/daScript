# Metal kernel dedup audit — 12-agent family sweep (2026-08-08)

Boris's hypothesis: "we missed at best a lot." Confirmed. Twelve Sonnet auditors read every
non-template kernel class family in dasllama_metal_kernels.das + dasllama_metal_prefill.das
plus the enc_ wrapper surface, graded per pair: shared-skeleton %, divergence class
(statement / member / algorithmic), mechanism fit, est. LOC, YES/MAYBE/NO for review.

Census scorecard — of the six previously-CLOSED verdicts re-audited fresh, three were wrong
or measured on the wrong axis: rope-store quants (bundled cross-codec, missed the two
same-codec single/batch twins), MoeGemv K4/K5 ("~10 shared lines" — missed a 13-line
byte-identical block recurring 5x file-wide), KqMulMmK6 ("own scale layout" — 58%
byte-identical to the K45T template, field types match 1:1, never measured). Held: KqGemv
K4/K5 (binding TYPE differs), K5C (verified no twin anywhere), Q8Gemm vs Q8Gemm64
(extended: the batch pair diverges the same way).

## Tier 1 — graded YES (ranked by payoff x confidence)

| # | Item | Mechanism | Est. LOC | Notes |
|---|---|---|---|---|
| 1 | ✅ **MetalDnBa ≡ MetalQ8Gemv** (prefill vs kernels) | template `BATCHED` axis | −47 landed | DONE 87b5769f5 (+ f39e6c46d msl_emit const-select fold, the enabler): Q8Gemv MSL byte-identical; DnBa differs only by `p` inlined as gl_WorkGroupID.y; kernels suite green |
| 2 | ✅ **accumulate-quad helper** | base-class method (free-`def` blocked: emitter rejects simdgroup/threadgroup params) | −83 landed | DONE 30b203548: MetalMmTileBase owns ta/tb + acc_quad; SEVEN sites (audit missed Q8MulMm); all 13 kernels statement-identical modulo splice braces |
| 3 | ✅ **KqMulMmK6 joins K45T** | 3rd `static_if` arm (SIXBIT) | −75 landed **+ perf WIN** | DONE: the perf-verify found the cached sv/dall hoist cost an occupancy tier (896→1024 max_threads) — reload-per-kb is −1.7..−10.5% per shape (bench_metal_kq_mm_lab, 3 launches, bit-exact); K4/K5 stamps byte-identical |
| 4 | ✅ **Q8GemmB/BSk** (the eyeball find) | template `IS_SK` (MetalQ8GemmBSplitT) | −69 landed | DONE: B stamp byte-identical (const-select folds); SK delta = sl/kbn/kb0 inlined (uniform, LICM); kernels suite 11/11 |
| 5 | ✅ **RopeStore Q8/BQ8 + Tq4/BTq4** | base classes w/ PARAMETERIZED splice methods (beats free helpers: methods take @workgroup buf + writable ssbo params; member args substitute) | −108 landed | DONE: rope oracle green all four; needed a kernel_access fix — method-call args now resolve through method param dirs (was silently deriving 'read' through splice calls) |
| 6 | ✅ **MetalAddRmsB delete** | row axis on AddRms (RmsNorm precedent) | −61 landed | DONE: misc oracle green both modes; enc_add_rms_b redispatches the same PSO |
| 7 | ✅ **KqMulMmK4T/K5T** | template `BLK/QH` (MetalKqMulMmK45TensorT) | −46 landed | DONE: both stamps byte-identical |
| 8 | ✅ **K4/K5 scale-decode block** | free `def kmask_scales` (uint4 in/out) | −37 landed | DONE: 5 sites; occupancy unchanged (704/512 max_threads before and after); kernels suite 11/11 |
| 9 | ✅ **MetalPfCopy delete** | enc_copy_row at both sites | −33 landed | DONE: gemma4e coverage cell serves metal_copy_row x3 (V-from-K); MTP-warm site inspection-gated (CPU-prefill-only in tests); copy oracle repointed |
| 10 | ✅ **enc_qk/av/qk_mm/av_mm dispatch helper** | enc_attn3 | −28 landed | DONE: encoder-only, zero GPU text change |
| 11 | ✅ **Swiglu/Geglu/Add/Sigmul quad** | MetalEw2T + abstract combine override (method splice beat @template_call — reads ascale) | −29 landed | DONE: stamps verified; enc_ew2 untouched (slot 3 already bound) |
| 12 | ✅ **Q8Gemm tensor triple (BT/BSkT/64BT)** | `TILE_N` + `IS_SK` + `LDC_ND` (MetalQ8GemmTensorT) | −27 landed | DONE: BT/64BT byte-identical, SkT = locals inlined |
| 13 | ⏸ **Comb/CombB** | RE-GRADED: instances cannot add fields — the batch's rt table + args struct would dummy-ripple through the lensed single's builder + call sites | fold into the LENS arc | the lens migration reshapes these encoders anyway; merging first would double-touch |
| 14 | ▶ **Router/RouterB** | DECIDED, next session's opener: NR-width template (KqMv `float[NR]` precedent) — Router is ALREADY stream-batched (grid.y); RouterB is the 8-position-BLOCKED prefill form. Template carries ns (single's lensed builder gains one param; call sites pass 1). Single is latency-trivial by its own comment — NR=1 stamp shape change is safe; the merge materializes the bit-identical-logits invariant structurally | −15..20 | |
| 15 | ⏸ **SwigluOai/SwigluOaiPf** | RE-GRADED: gate answered — instances canNOT add bindings; the Pf twin adds 4 (cnt/basep/ne/nfe), so the single would carry dummy binds | fold into the LENS arc | net ≈ −8 after encoder additions; not worth pre-lens |
| 16 | ✅ **enc_rms_last delete** | default `xoff` param on pf_enc_rms | −14 landed | DONE |

Tier-1 CLOSED 2026-08-08 (single session, 8 commits f39e6c46d..d963f6480): 13 of 16 landed
at −584 LOC net; 13/15 re-graded into the lens arc (instances cannot add fields — the audit's
estimates assumed they could); 14 decided + queued as the next opener. Riders: TWO tool fixes
(msl_emit const-select fold = the value-position static_if invariant; kernel_access resolves
buffer access through splice-method calls) and ONE measured production win (k6 prefill GEMM
reload-per-kb: the cached superblock scalars cost an occupancy tier — −1.7..−10.5% per shape,
bench_metal_kq_mm_lab is the standing guard).

## Tier 2 — MAYBE (judgment calls)

- **QKMm/AVMm** family-base hooks (MoeMulMmBase pattern; −40..45; needs a causal-bound hook design). QKMmT/AVMmT same split, leaning YES (−27).
- **MoeGemvBase** over K4/K5/K6/Mx4/Q51 (−50..55, but per-format lane geometry makes the hook surface ≈ the shared skeleton; census verdict half-holds). Q8 excluded (different row mapping).
- **rms reduction/fold primitive** x5 across AddRms/PreAddRms/RmsNorm (−50..60) — gated on an emitter probe: can one `def` helper serve both SSBO- and @workgroup-backed callers?
- **MulMm(T) vs Gemm64B(T)** cross-file grid-addressing axis (couples batch and general-M families; opportunistic only).
- **enc_gemm_b/gemm64_b** tile-width bool (−12; sk_b stays out — different output contract). **enc_rms_b ≈ pf_enc_rms** cross-file (tg 1024 vs 256 only; module-visibility question).
- Small helper extractions: rope_store_b bind prologue, DnConv tap-read ternary, DnL2Norm/DnGate reduction stanza (−4..6 each).

## Confirmed NO (census held, or fresh-verified distinct)

CombD (subgroup-cooperative, different algorithm), RopeStoreHF16 (thread-per-element +
fused QK-RMS), cross-codec rope stores, KqGemv K4/K5 (binding TYPE + bit-pack differ),
KqGemvK5C (no twin — select-form is unique), KqGemv/KqMvB8 K6 (geometry + field types),
Embed Q8/K6, PleGather vs Embed, Bf16MulMm vs T (raced by design), MoeCount/Bucket/Reduce
(pipeline stages, not variants), MoeSelect, WScale/G4RouterNorm, DnScan/DnDeint,
SuppressRow~Sigmul + Sigmul~AxpySig (metric artifacts / field mismatch), enc_ rope-store
singles, enc_moe_gemv_mx4 (required-field mismatch), enc_gemv cluster (already collapsed
on their own axes).

## Systemic notes

1. **The enc_ layer is migration debt, not merge fodder** (Boris's "verbose EDSL" call):
   [metal_dispatch] generates builders from @binding/@role/@off/@span — 41 classes lensed in
   kernels.das, ZERO in prefill.das, 24+22 encoders still hand-bound. Lens gaps found by the
   audits: builder-time dynamic offsets (CombB's rtoff), format-axis dispatchers, multi-view
   arena binds, instance-added bindings. Closing those + migrating prefill deletes most of
   the hand-written enc_ surface outright.
2. **Single-file dedup tooling misses the best finds** — DnBa/Q8Gemv (cross-file), B/BSk
   (scattered small edits under the shingle threshold), Swiglu quad (short bodies). A
   cross-file corpus run + a lower-threshold scaled-twin pass are detector work worth doing.
3. The single-vs-batch "row-indirection" delta is THE recurring family shape (RopeStore,
   AddRms, Router, Comb, SqAttn, DnBa) — the codebase already has both idioms for it
   (row-base-param helpers, row-index-collapses-at-grid-1 kernels); candidates above just
   apply the existing idioms.
