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

## Open design decisions (Boris, when reached)

1. **Where do lensed classes live?** prefill.das requires the lens with "census policing — no
   [metal_dispatch] classes remain here (they live in kernels)". Is that an observation or a
   rule? If a rule, migrating prefill = moving ~30 classes into kernels.das (7k lines already).
   Alternative: lift the rule, lens classes in place, keep the census.
2. **Items 13 (Comb/CombB) + 15 (SwigluOai/Pf) — instance-added bindings.** Two options:
   (a) template carries the binding UNION, singles bind dummies (the item-14 dead-ns precedent
   says runtime-free, but CombB adds an rt table + args struct — call-site noise is real);
   (b) extend [template_struct_instance] with axis-gated fields (bindings present only when a
   template-constant axis is on). (b) is cleaner and serves future single/batch twins;
   (a) needs zero tooling.
3. **enc_attn3's pso-as-parameter shape** — lens the four attn classes individually (four
   builders, delete enc_attn3), or teach the lens a pso-param mode. Leaning four builders:
   the mini-lens exists only because the classes weren't lensed.
4. **Optional binds with defaults** (softmax sink `?: g_one`, rope `brot ?: bhs` at two
   slots) — wrapper-level defaulting vs a `@default = "g_one"` field annotation.

## Phasing (decide-when-reached)

- **P0** — drop-ins: lens the cat-d classes, delete their encoders. No lens changes. Gates:
  builder `dump=true` byte-identity vs the hand twin (the lens's own proof rail), kernels arm.
- **P1** — @off/dual-view sites. No lens changes expected.
- **P2** — format-axis: per-class builders + thin wrappers. Gates: kernels arm + one
  PARITY_FULL family cell per touched axis (gptoss/qwen3moe pattern from item 14).
- **P3** — items 13/15 via decision #2; absorbs the dedup leftovers.
- **P4** — pf_enc_moe_route composite + the census ratchet end-state (every dispatch through
  the kn_ rail, `_metal_manual_dispatch` opt-outs burned down).

Perf guard throughout: encoders are CPU-side — no kernel text changes except P3 (byte-diff
gates there). Decode/prefill wall-clock spot-check per phase on the M1 (3 launches,
interleaved) only when a phase touches a hot per-token path.
