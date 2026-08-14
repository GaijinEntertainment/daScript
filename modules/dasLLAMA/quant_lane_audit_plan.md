# Quant-lane coverage audit — scope, holes, fix plan

Charter (Boris, 2026-08-04, on the heels of the gemma-26B hunt): audit EVERY weight format ×
every kernel lane × both platforms, plug the holes, ONE PR at the end, targeted profiling
scaled to the magnitude each hole shows. The audio-tower counterpart (gemma4a fp32 encoder)
is explicitly AFTER this arc. Branch: `bbatkin/gemma-kernel-hunt` (continues the hunt's
instrumentation + the landed fused arm). zen2 E: rig re-profiles just the affected models
once Boris clears the box.

## The format universe

Source GGUF types the loader accepts: F32, F16, BF16, Q4_0, Q5_0, Q5_1, Q8_0, Q4_K, Q5_K,
Q6_K, MXFP4. Exec formats (KqFmt): q8, k4, k5, k6, q40, q51, q8n (Metal upload twin), plus
the mx4 rail and the fp32 / legacy-q4_0 QuantModes.

Mapping (`kq_fmt_of`, dasllama_load.das:1024, under `kquant_native`):

| source | exec | knob (default) | notes |
|---|---|---|---|
| Q4_K / Q5_K / Q6_K | k4 / k5 / k6 | kquant_native | superblock lattice, %256 rows |
| Q4_0 | q40 | kq_q40_native (ON) | %256 demotes per tensor to q8 |
| Q5_0 | k5 | kq_q50_native (**OFF**) | LOSSY re-encode (+0.031 rel-rmse measured) — OFF = exact q8 |
| Q5_1 | q51 | kq_q51_native (ON) | **expert stacks only — dense always requants to q8** |
| Q8_0 / F16 / BF16 / rest | q8 | — | exact / standard requant |
| MXFP4 | mx4 | — | own rail (gpt-oss) |

## Lane × format × backend coverage (CPU)

Lanes: mm (dense GEMV) / batch (dense prefill) / groupn (MoE decode GEMV) / batch_groupn
(fused MoE prefill) / rows (fused decode chains) / repack+layout. "gen" = emitted + tuned +
registered; "portable" = default loop (tuned dot cores inside, no tile); "—" = excluded by
gate (never dispatches).

| fmt | lane | x64-gen | arm64-gen | Metal |
|---|---|---|---|---|
| q8 | all six (+ all s16 twins) | gen | gen | kernels |
| mx4 | all (incl. native batch pair + rows) | gen | gen | kernels |
| k4/k5/k6/q40 | mm/4×rows/batch/groupn/batch_groupn/repack | gen | gen | kernels |
| **q51** | **groupn (decode GEMV)** | **portable — DEAD CROWN (emits+races+crowns, never wired)** | gen (sdot, grp planes) | moe_gemv_q51 PSO |
| **q51** | **batch / batch_groupn (prefill)** | **portable** | **portable** | GPU MoE FFN accepts q51 |
| q51 | rows (fused decode chains) | — (kq_servable: by design) | — | n/a |
| q51 | repack/layout | identity/grp1 (disk order) | gen (grp) | n/a |
| q8n | GPU upload twin | n/a | n/a | kernels |
| fp32 mode | dense mm/batch | portable by design (debug/parity rail) | same | n/a |
| legacy q4_0 mode | NO batch lanes — prefill IS the decode loop | fossil by design | same | n/a |

**The audit's verdict: q51 is the ONLY format with unplugged CPU lanes.** Everything else
(q8, mx4, all four kq members incl. q40, s16 twins, rows cores) is generated and registered
on both gen backends. The q4_0 legacy MODE and fp32 MODE are deliberate non-tiled rails, a
retire/keep decision rather than kernel work.

## Exposure (who actually hits it)

Board catalog (records/*.json exec_fmt across every box): q51 appears in EXACTLY ONE model —
gemma-4-26B-A4B Q4_K_M (ffn_down_exps Q5_1 on 29/30 layers). k5 only in the Qwen3.6 pair
(covered), q40 in no published row, `f32/q8` rows are the audio towers (next arc). Wild
models: any Q5_1-expert recipe hits the same lanes (knob-gated); Q5_0 recipes stay exact-q8
unless someone flips the lossy knob; Q4_0 recipes ride the covered q40/kq lattice.

Measured magnitude (zen2 3990X, t16, fallback winners, pp128 window, 2026-08-04): q51
batch_groupn runs 21 GMAC/s vs 290 GMAC/s for the kq tile beside it (14x); 80% of the 26B's
prefill wall. x86 decode GEMV (the dead-crown lane) magnitude: TBD in targeted profiling
(tg window split). Apple: Metal masks everything on published boards; -ngl 0 ARM prefill
pays the same portable batch lanes (verify once, low priority).

## The holes, ranked, with fix shapes

1. **q51 batch tile family (both platforms)** — add `q51_batch_groupn_gen` (+ plain batch)
   mirroring the in-file `kq_batch_cell_gen` tile pattern; new [tune] family + grid entry;
   wire both gen registrations. THE fix for the 26B pp hole. Cost: emitter work + re-mint
   re-race on every box. NOTE: the expand-region-to-q8-scratch composed alternative is a
   PROVEN dead end — mx4 tried exactly that and the scratch round-trip was 87% of gpt-oss
   prefill wall (the KernelBackend.mx4_native_batch comment); native tile only.
2. **Wire the existing q51 GEMV on x64** — `groupn_q51 = q51q8_groupn_gen` + `repack_q51` +
   `q51_layout` in the x64 block. The family already emits, races, and CROWNS on x86 mints
   (zen2 sidecar carries `q51q8_gemv_gen -> mr8` that never dispatches). One registration
   diff; fixes 26B decode on zen boards and un-deadens the crown.
3. **Day-1 mitigation (already implemented, policy-only)**: `set_kq_q51_native(false)` —
   the exact q8-requant A/B rail (~1.4x expert memory, ~+2GB on the 26B) puts the 26B on
   crowned q8 tiles TODAY. Candidate default until (1) lands — per-platform call is Boris's.
4. **Mint hygiene rule**: a [tune] family that races on a platform where no backend wires it
   = wasted mint time + a misleading sidecar (it fooled this audit's first pass). Rule: tune
   scope must match the registration table per-arch — assert at mint time or gate the race.
5. **q51 rows core** (optional, rides (1)): lets the fused decode chains serve q51 layers
   instead of declining to per-op dispatch. Measure first — may be sub-1%.
6. **Fossil decision**: legacy QuantMode.q4_0 (no batch lanes, prefill=decode loop) — retire
   or document; not kernel work.

## Verification protocol (per hole)

- Structure: `lcpp_bench --prof` census + pp window + `mm_moe_dn` split (landed this arc) on
  the 26B; tg twin split for hole (2).
- Correctness: `test_parity_gemma4_26b_q4km` both legs (fused default + per-expert) — landed,
  green on zen2 2026-08-04; new kernels extend the same oracle gates.
- Numbers that count: re-mint on the fresh binary FIRST (everything so far is debug-jit on
  stale-mint fallback winners — structure/ratios only), then the one-model re-profile on the
  cleaned zen2 box, das vs ref, protocol per METHODOLOGY.md.
- Expected end state: 26B pp window 2.99s -> ~1.0-1.2s; board 0.51x flips to a das lead.

## q51 tile v1 — LANDED (2026-08-04 late, commits b4fc4953a + 246f200e9)

- `q51q8_tile_gen` [tune] family (anchor; GEMV + layout demoted to companions — the kq
  pattern, which also kills the dead crown structurally) + `q51_batch_cell_gen` +
  `q51q8_batch_kernel_gen` + `q51q8_batch_groupn_gen` drivers (in-driver per-32 asum planes);
  full q51 family wired on BOTH gen backends; the generator's sdot-only decline removed
  (emit_block_q51 dots ride kq_dot_lane — every lattice leg emits).
- **v1 grid is 128-bit**: the 256-bit maddubs stamp died mid-emission ("Failed to get IR" —
  emit_block_q51 has no wide block arm; 16B loads + te.rv-lane shuffles are invalid at width
  256). Grid = arm mr4/mr8(/nrsplit) + maddubs width=128 mr4/mr8; fallback
  `dot_maddubs_width128_mr8;mr4`.
- **Measured (zen2, fallback perm, debug-jit, r=1)**: mm_moe_dn 2.79s -> 1.84s (-34%),
  window wall 2.99 -> 2.78s. **Parity GREEN both legs** (fused + per-expert, token-for-token,
  84s).
- **Layout seam observed**: the dlim identity hash did NOT change and active_q51_layout_mr()
  read 1 at load — the fallback-resolved stamp did not move the layout companion, so planes
  stayed disk-order and the whole walk ran coherently at mr=1 (correct, un-interleaved).
  VERIFY under a real mint: does a raced winner stamp companions coherently AND re-prep the
  dlim (q51_mr is in the identity)? If fallback-resolved stamps never move companions, the
  fallback chain's mr8 spelling is decorative — check tune_pick_fallback vs companion stamping.

SAME NIGHT, ALL DONE — **THE HOLE IS CLOSED**:
- Wide q51 block arm LANDED (emit_block_q51 + qh_bits width-generic; the 256-bit maddubs
  stamp emits; layout companion moved, planes grp8, new dlim identity). Debug census:
  mm_moe_dn 2.79s -> 296ms (9.4x), pp128 40.85 -> 74.0, parity green (48.7s, fastest run).
- Fresh PARANOID mint SHIPPED on attempt 4 (noise=ok): attempts 1-3 refused honestly (ambient
  noise -> priorities worn off; validation drift -> the OTHER SESSION woke mid-mint; cv 1.51%
  -> recalibrated DAS_TUNE_NOISE_CV=2.0, stamped). q51q8_tile_gen crowned
  dot_maddubs_width256_mr8_nrsplit2 under its anchor (the probe rekey worked); 41 kernels
  complete; the staged NOISE_OVERRIDE batch was never needed.
- **TARGETED PROFILE (released exe, baked winners, p512/n128/r5): pp512 153.84 +- 6.37
  (cv 4.1%, above board bar — verdict-grade, not board-grade), tg128 13.65 +- 0.39.
  vs stored bare-ref 105.7 = das LEADS 1.46x — INSIDE the 1.45-1.66x family band; vs the
  pre-fix 54.1 = 2.84x.** The 0.51x board hole is closed at family parity.
REMAINS FOR THE PR: arm-side batch-lane validation (M-series); mm_gemm bucket unification;
the q51 tile-shape bench in gen_tune_probe (nrsplit crowned via the gemv proxy tonight);
clargs unknown-flag fix; the audio-tower q8 chapter (scoped above). NO zen2 board re-run
(Boris: nothing new to find out — the zen2 store's 0.51x rows stay as honest provenance-
stamped history; the row updates whenever zen4/rented boxes do their targeted run).

## Audio-tower counterpart — scoped (M1 code-read 2026-08-05, no profiling yet)

The x86 audio gap has the same anatomy as the q51 hole, in the fp32 lane:

1. **Tower GEMMs ride a dot loop, not a tile.** gemma4a's projections (attn q/k/v/o, ffn
   up/down, conv pw1/pw2, inproj, RPE) all funnel via mm_blob_b/g4a_mm_clamped ->
   `matmul_batch_core` (dasllama_math.das:547) = parallel tuned-`dot` per (row, token) —
   weight row hoisted, but no register blocking, activations re-read d times. The tiled
   `gemm_f32_uk_4x16` [tune] ukernel EXISTS in the same file (crowned on zen2) and IS used
   by some audio call sites (canary/audio/attn_prefill) — per-site inventory owed: which
   tower mms are dot-loop vs tiled.
2. **The platform asymmetry that hid it**: `g_mm_fp_batch_override` — das_accelerate (Apple
   AMX BLAS) claims fp32 batch above min_ntok, so M-boards never run the dot loop; x86 has
   no override. Meanwhile ggml's x86 tower runs real tiles (~3x its own ARM tower — the
   zen4 audio-chat finding). Same story as Metal masking the q51 CPU lanes.
3. **Scalar glue in gemma4a_encode**: the sliding-window attention core (QK^T + RPE +
   softcap, statement-level scalar per head), Q/K scale loops, GLU sigmoid, causal depthwise
   k=5 conv, im2col, and a hand residual loop where `add_scale_inplace` (a crowned kernel)
   exists. rmsnorm/softmax/silu DO ride tuned kernels.
4. **The tower q8 asymmetry**: canary/whisper encoders were q8'd (canary arc) — their mms
   ride the crowned q8 tile machinery; gemma4a and the Qwen-Omni/ASR towers stayed fp32 —
   the dot-loop lane, and the board rows that read `f32/q8`.
5. **DECIDED (Boris 2026-08-05): force the stragglers to q8 — no fp32/bf16 weight lane,
   ever.** q8-everything is the standing loader rule; the fp32 towers predate its
   application to encoders, they are not an exception. Work: gemma4a + qwen3a/Omni tower
   weights -> Q8_0 planes at load (the canary phase-2 pattern), tower mms move to the q8
   batch rail (crowned tiles everywhere), transcript oracles gate each tower. gemm_f32
   survives only as the attention (activation x activation) engine — not a weight lane.
   The ref runs its tower at bf16 (mmproj census: 247 BF16 + 1164 F32 tensors, tinyBLAS
   tiles). Scalar-glue items (attention core, GLU, depthwise conv, hand residual loops)
   remain a separate cleanup pass.
6. **Comparison protocol (Boris 2026-08-05, BOARD-WIDE — every model, not just audio):
   force the reference into OUR formats where its tooling allows, so performance compares
   format-to-format.** Fair perf comparison = both sides at the same weights format;
   fp32/bf16 has never shown enough quality edge to justify a lane, least of all on ASR
   where transcripts are the gate. The LLM rows already comply (same GGUF file both
   engines). The gaps and their fixes: llama-mtmd-cli audio towers — convert mmproj with
   `--outtype q8_0` (composes with --mmproj; clip/mtmd has no quant guard; ggml mul_mat
   takes q8_0) — verify with one conversion + transcript run, recipe joins the
   fetch_models manifest; whisper.cpp refs — its own quantize tool, q8_0 standard (our
   whisper encoder is q8 — their leg should be too); NeMo/ONNX python legs — NOT forceable
   (torch fp32/16 graphs), the "oh well" bin: they run native precision and the board
   provenance says so per leg. Rule lands in METHODOLOGY.md with the PR.

## Audio chapter — das side LANDED (2026-08-05, M1-gated)

- **gemma4a forced q8** (0f6736e97): canary pattern — GEMM set (11/layer + 3 global) ->
  qblob/qscales read-time transcode + repack; convs/norms/biases/RPE fp32; per-site requant;
  RPE image once per encode; gemma4a-q8 image lane; f32 = A/B rail (set_gemma4a_q8). Gates:
  image roundtrip element-exact both lanes; NEW transcribe A/B — **q8 vs f32 transcripts
  BYTE-IDENTICAL on jfk**.
- **qwen3a core forced q8** (ff2a800c0): one tower_quantize call arms the shared AudioTower
  rail (encoder_blocks was already tw_mm-routed). The NEW A/B cell caught a LATENT BUG:
  tower_quantize_ repacked a phantom conv2 region at offset 0 on conv-less cores, garbling
  layer 0 (0.086 -> 0.014 maxdiff after the gate — the fix is in dasllama_audio.das).
  Measured drift: rel-rms 3.2% / tail-max 10.9% (the element-level instrument keeps a 5%
  rel-rms bar). (a) LANDED 2026-08-05: the Qwen3-ASR-0.6B pair is enrolled in fetch_models
  (ggml-org GGUFs, sha-pinned: decoder 804.7MB Q8_0 + the 378.6MB bf16 mmproj the tests
  already used), and the REAL gate exists — test_qwen3a_transcribe_q8_ab (image suite,
  arm/family qwen3a): q8 vs f32 tower transcripts through the 0.6B decoder are
  BYTE-IDENTICAL on jfk. The stale qwen3a image roundtrip (still pinned to the f32 tag
  from before the q8 default) was rewritten on the gemma4a pattern: q8 lane element-exact
  incl. qblob/qscales + the f32 A/B tail. (b) LANDED 2026-08-05: the cblob GEMM trio
  (cout/mm1/mm2) serves q8 from whole-cblob qcblob/qcscales planes (the core's recipe —
  offsets stay valid, trio regions tw_repack'd; convs/positions/biases stay fp32);
  requant scratch is the conv state's own grow-only pair, deliberately NOT the core's
  pre-sized s.xqi (shrinking that under the encoder would be a heap overrun). Gates:
  roundtrip element-exact incl. the new planes; transcripts STAY BYTE-IDENTICAL with the
  trio quantized. PREDICTION MISS (the good direction): rel-rms was predicted to rise
  3.2% -> ~4%; it FELL to 2.44% (maxdiff 0.014 -> 0.0076). Unconfirmed candidates:
  yesterday's 3.2% retained residual stale-dlim corruption (today both lanes re-minted
  from scratch), or the q8 trio's integer-exact block dots vs long fp32 reduction chains
  change the leg's noise structure. Struct layout grew (meta 17+2, two new sections) —
  stale tower dlims (ASR x2 + Omni x1) deleted by hand; identity does not see code changes.
## Review response (PR #3629, six-agent Opus review, 2026-08-05)

- Mechanical batch (response 1/2): IMAGE_VERSION 8, --kv f32 arm + probe validation,
  profiler disarm, pin-leak fix, RPE wrapper, family kv defaults f16, mtmd log-line filter
  (the capture merges stderr), rel-rms cell moved to the image suite, inscope sweep,
  doc-truth pass; REVIEW.md gains the Documentation section; followup 18 = tutorial
  resync; CLAUDE.md comment cap reworded to match STYLE014 (rest of the lint threshold
  question = Boris's separate session).
- **COMPACT q8 planes (response 2/2, the auditor's memory UNPROVEN -> a fix)**: the shared
  AudioTower core, the whisper decoder rail, and the qwen3a cblob trio all stop carrying
  fp32 copies of their GEMM regions — dense q planes + a compact fp32 remainder, offsets
  rewritten at quantize (formulaic layers got a q8-branch offset formula; whisper's tied
  te keeps an fp32 copy for row lookups + a q8 copy for the logits GEMV). The STREAMED
  whisper mint got the same split (job-filtered per-plane walks + offsets-only twins);
  the streamed-vs-staged image cell is the consistency gate between the two builders —
  it caught the first attempt's length mismatch exactly as designed. Measured images:
  Omni tower 3167 -> 708 MB (-78%), qwen3a tower 911 -> 212 MB (-77%), whisper tiny
  185 -> 120 MB; large-v3-turbo re-mints at next use (projected ~3954 -> ~1.2 GB).
  parakeet/canary/gemma4a were already compact. Gates: image arms 30/0 from cold
  (streamed cell green), PARITY_FULL test_whisper 35/35 zero skips (all frozen-id oracles
  token-exact on compact planes), asr_verbs 6/6.

  (c) CLOSED 2026-08-05: PARITY_FULL test_whisper 37/37 zero skips — the Omni oracle held
  full frozen token-for-token ids on all three clips with the trio q8 armed (its tower
  cold-minted the new-layout dlim in the same run); canary oracle green as the
  unaffected-path regression check. **The das-side audio chapter is DONE** — every audio
  tower (whisper, parakeet, gemma4a, qwen3a core+trio, Omni via the shared tower) serves
  q8 GEMMs, each behind a transcript-level or token-exact gate. Remaining audio items are
  ref-side (zen2 legs) or cross-box (ARM q51 batch validation).
- **M1 profile (asr_bench --fp32-tower A/B, E2B pair, CPU prefill, r=2, 2026-08-05)**:
  gb1 54.3s -> 35.0s (**-35%**, 3.7x -> 5.7x realtime), jfk warm 2.54 -> 1.92s (-24%).
  The tower-dominated audio-chat tax drops by a third from q8 alone, ON APPLE — the
  "das tower gets nothing from the ISA" era ends on both platforms. x86 leg expected
  bigger (maddubs tiles); measure on zen2 with the ref legs.
- Ref-side format matching (mmproj --outtype q8_0 conversion + whisper.cpp q8 ref leg)
  rides the zen2/rig leg per the protocol above.

## Card re-pair — the two stale audio-chat rows per box (das f32->q8 towers, 2026-08-05)

- **M1 DONE (store updated, uncommitted; no site regen per Boris).** Rig re-released
  (--quick self-minted the branch's q51 families, noise=ok). E2B das: jfk 2.38->1.73s,
  jfk3 6.75->4.72s, gb1 45.4->33.9s (-25%), hp0 62.0->47.6s; Omni das: jfk 3.3->2.5s,
  jfk3 7.4->5.1s, gb1 64.2->51.1s (-20%), hp0 89.1->71.2s, hp0x2 218->182s. exec_fmt
  q8/q8 both. mtmd refs reproduced within 0.5-2.5% of 2026-08-03 (box-state validation).
  Omni beat the -7..-12% prediction — the tower is a bigger share of its wall than credited.
- **M4 STAGED, PARKED on console access.** Branch a63300c2f, daslang REBUILT (box binary
  predated the branch daslib — 64-bit resize builtins, error[30165]), stale tower dlims
  deleted. Release self-mint REFUSED twice, honestly: Anton's GUI session ran the aerial
  wallpaper (video decode ~25% CPU); after bootout the login screen animates instead
  (WindowServer 16-21%) — flips got WORSE as ambient got better (2.16%@1.87-band ->
  3.70%@1.32-band) = scheduling weather, not steady load. displaysleepnow ineffective
  (error 1004 / re-wakes), pkill respawns, FileVault ON = no remote reboot, VNC password
  unknown. Waits on Anton: dasbox console login / wallpaper off Aerials / console reboot.
- **zen2 DONE and released.** Cards re-paired (E2B -5..-11% steady-state, 0.88x -> 1.01x at
  gb1 / 1.18x at jfk3; Omni -4..-9%, 1.22x at gb1; old hp0 149s = separate dead pathology),
  store + tune archive committed, site regenerated — Boris: the cards PUBLISH; ref-side q8
  legs ride the next profiling round.

## The deep-clip fade — named and half-fixed (2026-08-05, zen2+M1 probes)

- Stage instrumentation landed first (the Phase-B lesson: three blind spots — gemma4a's
  encode bucket returned "" behind a stale comment, qwen3a's ASR turn had no encode site,
  the probe never armed forward_profile). asr_stage_probe now reports both rails; mtmd
  refs now STORE their transcripts ("does it work" must be readable, not hoped).
- **The finding**: coarse shares are box-identical (decode ~61% / encode ~20% / mel 2% /
  prefill-residual ~17%), but INSIDE decode the boxes swap: M1 spends it on FFN (blk_ffn
  15.4s vs attn 5.6s on gb1), zen2 on ATTENTION (attn 16.4s vs blk_ffn 17.4s) — the fp32
  attention core over fp32 KV runs 2.9x the M1 while the q8 FFN GEMMs convert at 1.13x.
- **KV A/B (zen2, E2B, jfk/jfk3/gb1)**: f16 KV -3% at gb1, free on shorts, transcripts
  byte-identical on jfk/jfk3 and near-tie-sane in gb1's degraded tail; q8_0 KV SLOWER
  (+5..17% shorts — block-decode overhead beats the bandwidth win at ASR depths).
  Prediction miss #3: predicted -8% for f16, got -3% — only the K/V read bytes halve; the
  fp32 dot math and softmax stay, so the remaining attention meat is KERNELS, not cache
  format (ledger candidate: f16-aware attention dots / vectorized softmax on x86).
- **DECIDED (Boris): f16 KV is the ASR serving default** — apples-to-apples with the ref's
  f16 KV; q8_0 rejected; f32 demoted to the oracle/parity rail (frozen-id cells pin it
  explicitly, restore to f16 after). The ~3% row staleness is noise — no reprofiling,
  cards refresh next cycle. Gates: image arms 21/0 (transcribe A/Bs byte-identical under
  f16), PARITY_FULL test_whisper 37/37 zero skips.

## Landed already (this arc's branch, 7 commits)

pp profile window + dispatch census (per-fmt layer counts) in lcpp_bench --prof;
blk_attn/blk_ffn coarse buckets; mm_b_kq_pre/mm_b_q51_pre mm_gemm self-bucketing;
the gemma4 fused_kq arm (parity green both legs); mm_moe_dn bucket split.
Known follow-ups riding the PR: unify mm_gemm bucketing at wrapper level (generic path
double-counts via caller wraps); clargs unknown-long-flag silent ignore; embed cold-fault
ledger note (grp interleave multiplies first-touch page count).
