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
| 5 | **RopeStore Q8/BQ8 + Tq4/BTq4** | free helper w/ row-base params (sq_fill_scores precedent) | −95..115 | ~73% verbatim per pair; the census's wrong-axis closure |
| 6 | **MetalAddRmsB delete** | row-index no-op edit on AddRms (RmsNorm/enc_rms_b precedent) | −50..55 + a PSO | row≡0 at grid=1 compiler-verified |
| 7 | **KqMulMmK4T/K5T** (prefill tensor twins) | template `BLK/QH` — exactly their merged parents | −48 | ~97% identical |
| 8 | **K4/K5 scale-decode block** | shared helper | −40 | 13 lines byte-identical x5: MoeGemvK4/K5, KqGemvK4/K5/K5C |
| 9 | **MetalPfCopy delete** | call enc_copy_row (lensed, public, already required) | −30 | tg-width 256-vs-64 perf sanity check at 2 sites |
| 10 | **enc_qk/av/qk_mm/av_mm dispatch helper** | shared helper (same AttnArgs, 4x copy-paste) | −25..30 | removes 4-site hand-sync hazard |
| 11 | **Swiglu/Geglu/Add/Sigmul quad** | template `@template_call combine` | −25 | UNFLAGGED by the tool; already share one encoder (enc_ew2) |
| 12 | **Q8Gemm tensor triple (BT/BSkT/64BT)** | `TILE_N` + `IS_SK` axes | −10..18 | land with #4; BT≡64BT already exact clones |
| 13 | **Comb/CombB** | template `BATCHED` | −15..20 | chunk-range prologue is the only real delta |
| 14 | **Router/RouterB** | template `BATCH` width (1 vs 8) | −15..20 | bit-identical-logits invariant is currently a COMMENT |
| 15 | **SwigluOai/SwigluOaiPf** | template + optional guard | −16 | byte-identical clamp math x2 files; gated: can an instance ADD bindings? |
| 16 | **enc_rms_last delete** | default `xoff` param on pf_enc_rms | −13 | 1 call site |

Tier-1 total ≈ −550..650 LOC, most of it verbatim-duplicate risk removal.

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
