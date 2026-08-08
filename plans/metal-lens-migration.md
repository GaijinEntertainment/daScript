# Metal lens migration — arc plan (opened 2026-08-08)

Successor to plans/metal-dedup-audit.md (tier-1 closed at 14/16, −600 LOC). Boris's ruling:
lens migration AFTER dedup. Goal: delete the hand-written enc_ surface by migrating prefill
to `[metal_dispatch]`-generated builders and closing the lens gaps the dedup audit named.
Each phase gets decided in detail when reached.

## Ground truth (surveyed 2026-08-08)

- kernels.das: 129 classes, **41 lensed**, 88 not (mostly templates/instances/abstract bases
  that don't dispatch directly — the true unlensed-with-encoder set is small).
- prefill.das: **32 classes, 0 lensed**; 22 named encoders + 2 inline sites + 1 shared tail
  helper (`kn_moe_mm_family_tail`, a proto-lens fragment). 6 A/B race harnesses are bench-only,
  out of scope.
- Cross-file precedent already proven: `pf_enc_moe_route` calls the lens-generated
  `enc_moe_select` (kernels-side builder) — hand encoders composing lens builders works today.

## What the lens already covers (no extension needed)

- `@off = "param"` caller-passed uint64 offsets, shared names emit once — the dual-view blob
  pattern (`wsh`/`wqb` both `@off = "boff"`, caller passes the same buffer twice) is exactly
  category-c "same arena at two slots". `pf_enc_dn_ba`, `pf_enc_ple_gather`, `pf_enc_rms`,
  `enc_add_bias_rows`, `enc_qk_norm_pf` map onto this directly.
- Struct-typed `@uniform` = kargs by value (the MoeMmArgs shape).
- Multi-kernel classes via `kernel=` per `[metal_dispatch]` instance.
- Dead/dummy binds are measured-free (item 14's `ns`: BATCHED=false stamp never reads it,
  g_zero bound at 11 sites — zero cost).
- The lens header's own doctrine: "Variation points (fmt dispatchers, dummy binds, pso twins)
  stay hand-written as thin wrappers over generated builders." Format-axis (KqFmt 3/5-way,
  tensor-crown mode twins) is BY DESIGN wrapper-level — the twin-vs-base binding-layout
  switches (pf_enc_moe_mm, pf_enc_moe_mm_mx4) are different classes with different builders,
  and the wrapper picks the builder. No lens feature required.

## Encoder census by migration class (from the survey)

- **Drop-in (8-10):** pf_enc_ple_finish, pf_enc_moe_reduce, enc_qk, enc_softmax*, enc_av,
  pf_enc_swiglu_oai, pf_cat2 (inline), enc_attn3's four users via per-class builders.
  (*softmax has the optional sink bind at slot 20, `?: g_one` — call-site defaulting.)
- **@off/dual-view shaped (12):** the category-a/c sites; most flatten onto existing @off
  (struct-field args like `bs.doff` pass as plain uint64 at the call site).
- **Format-axis wrappers (3 true + 6 boolean):** enc_gemm_mm, pf_enc_kq_site_mm,
  pf_enc_kq_gemv, pf_enc_moe_mm(+mx4), bf16 inline, enc_qk_mm/enc_av_mm.
- **Composite (1):** pf_enc_moe_route — 3 dispatches + 2 skip rails + encode-time
  upload_region; stays a wrapper composing 3 builders.

## Design decisions (1-3 RULED 2026-08-08; 4 in discussion)

1. ✅ **Lensed classes live next to where they are used** — prefill classes lens IN PLACE.
   Update prefill's require comment ("no [metal_dispatch] classes remain here") when the
   first one lands.
2. ✅ **Items 13/15 via (b): extend [template_struct_instance] with axis-gated fields**
   (bindings present only when a template-constant axis is on) — cleaner than union+dummy,
   serves future single/batch twins.
3. ✅ **Four per-class builders, delete enc_attn3.** Beyond simplicity: per-class DERIVED
   hazard staging is more precise than enc_attn3's one-shape-fits-all hand staging.
4. ✅ **`@default = name` field annotation — bare identifier, NOT a string** (Boris's call;
   grammar already allows it: ds2_parser.ypp:1179 stores an unquoted NAME value as tString,
   probe-verified 2026-08-08, formatter-safe — token-based, never re-quotes). Resolution:
   sibling-field name first (→ its builder param), else module global. The builder param
   stays required; sites without the buffer pass `null` and the builder body resolves
   `b ?? default` once — bind AND hz staging use the resolved buffer.

## Phasing (decide-when-reached)

- **P0** — drop-ins: lens the cat-d classes, delete their encoders. ✅ DONE 2026-08-08.
  Batch 1: `@default` shipped (lens `mk_buf_ref` ternary — `??` on pointers DEREFS, use
  `!= null ?:`; `default` keyword whitelisted into annotation-argument-name in BOTH grammars,
  one line each) + MetalRope (`@off = voff`, `rot @default = head_size`) and MetalAttnSoftmax
  (`sink @default = g_one`) lensed, hand encoders deleted. Batch 2: the six attn classes
  (QK/AV direct as enc_qk/enc_av; the mm four as `_c`-suffixed cores — enc_qk_mm_c/_t_c,
  enc_av_mm_c/_t_c — with enc_qk_mm/enc_av_mm kept as hand twin-picking wrappers), enc_attn3
  DELETED; + MetalPleFinish, MetalMoeReduce, MetalSwigluOaiPf, MetalPfCat2 (the inline site
  became a pf_enc_cat2 call). The T twins needed no @role — the classifier's ptr-local +
  tmm2d_tg_store arms derive them. Grid equivalences used: mp/nk64 are 64-padded so ceil-div
  == the hand exact div; swiglu's total/256 == (total/4+63)/64 by the nested ceil-div
  identity. Gates: kernels suite 7/7; prefill-base llama token-exact (mm path); fam-phi3
  matrix (hs=96 ⇒ the non-mm trio); fam-gptoss PARITY_FULL maxd bit-identical
  (0.25608706/0.23721886 — swiglu_oai + moe_reduce + mm attn); gemma4e E4B coverage row
  census `metal_ple_finish = 2`; cat2 + ple_finish `dump=true` statement-identity vs the
  deleted hand forms. Known deltas vs hand twins: derived staging adds the hz_read the hand
  encoders under-staged on in-place buffers (softmax att, ple_finish y, swiglu g).
  ⚠ OPEN (pre-existing, surfaced here): (1) the MTP GPU-warm prefill (pf_enc_cat2's only
  runtime path) has NO in-suite gate — test_mtp is planar-only, so cat2's proof is the dump
  identity; (2) enc_qk_mm_t_c/enc_av_mm_t_c compile + derive on the M1 but Metal-4 tensor
  cannot execute there — M4-leg spot check when that rig round happens.
- **P1** — @off/dual-view sites. ✅ DONE 2026-08-08 (no lens changes, as expected). Six hand
  encoders deleted + one reduced to a thin wrapper: pf_enc_rms + enc_rms_b + enc_qk_norm_pf
  (MetalRmsNorm grew `@off = "xoff"` + two more [metal_dispatch] instances — one annotation
  LIST, comma-separated: stacked `[...]` blocks are a syntax error; MetalQkNorm grew
  `@off = "xoff"`, so prefill now calls decode's enc_qk_norm builder directly — the pf twin
  was the same grid/tg + an offset), enc_add_bias_rows + pf_enc_ple_gather (lensed in place
  in prefill.das; add_bias got exact `@span = "total*4"` staging, ple_gather the dual-view
  `@off = "eoff"` pair), pf_enc_dn_ba + the MetalMoeRouterB pair (classes lensed in
  kernels.das where they live; their PSOs moved into the family registry as g_pso_dn_ba /
  g_pso_moe_router_b — the g_pso_qknorm precedent; MetalQ8GemvT's y span widened
  `"d*4"` → `"d*np*4"` so the batched instance stages exactly, enc_gemv_c grew np and its
  one wrapper caller passes 1l). enc_moe_router_b now serves BOTH the pf_enc_moe_route
  router leg (real rb/hasb) and pf_enc_slab_gemv_b, which stays as the thin dummy-bind
  wrapper (g_one/g_zero) per the lens doctrine. ~32 decode call sites gained an explicit
  `0ul` offset. Derived-staging deltas (all strictly-more-correct): enc_qk_norm +hz_read(x),
  add_bias +hz_read(x) exact-span, ple_gather +hz_read(tok); dn_ba/router_b y staging
  tightened whole-buffer → exact span. Gates: kernels 7/7; prefill parity base/qkv/cont/dim
  token-exact; fam-qwen3 (qk-norm) + fam-qwen35 (deltanet dn_ba/slab paths) CPU==GPU;
  fam-gptoss PARITY_FULL maxd bit-identical (0.25608706/0.23721886 — the route leg through
  the generated builder); gemma4e E4B coverage census metal_ple_gather_q8 = 2; decode base
  parity green; all 8 new/changed builders dump-proven against their hand twins.
- **P2** — format-axis: per-class builders + thin wrappers. Gates: kernels arm + one
  PARITY_FULL family cell per touched axis (gptoss/qwen3moe pattern from item 14).
- **P3** — items 13/15 via decision #2; absorbs the dedup leftovers.
- **P4** — pf_enc_moe_route composite + the census ratchet end-state (every dispatch through
  the kn_ rail, `_metal_manual_dispatch` opt-outs burned down).

Perf guard throughout: encoders are CPU-side — no kernel text changes except P3 (byte-diff
gates there). Decode/prefill wall-clock spot-check per phase on the M1 (3 launches,
interleaved) only when a phase touches a hot per-token path.
