# The MTP depth-N arc — Qwen3.8-27B native head + gemma-4-26B-A4B assistant drafter

Branch `bbatkin/mtp-depth-n` off master c0ca624f9 (PR #3919 merged). One PR, both carriers.
Research (cited lane reports + synthesis): `~/.claude/plans/mtp-research/`. This document is
the arc plan; the research is the evidence.

## Rulings (Boris, 2026-09-01)

- One arc, both models, single PR.
- **Width 1, depth N** first - "it may just be good enough". N-wide is NOT ruled out: a late
  experiment gated on the measured t(M) verify-cost curve, not a deliverable.
- First pass = a close implementation of mlxfast's scheme (they serve both carriers: the
  qwen38 track's native-head chain and the gemma track's assistant drafter).
- **Single `.dlim` per carrier**: the split head GGUF folds into the trunk's image at mint;
  serving never opens a second file.
- Corpus: SpecBench (no standard exists; 480 prompts, 6 tasks, greedy).
- llama.cpp reference: `llama-bench` has no speculative support - "we'd find a way to
  profile" (`llama-speculative-simple` / `llama-server` as the ref driver). Our own non-MTP row
  is the baseline regardless.
- Website presentation deferred to arc end; likely a separate model row per carrier
  (Gemma-X vs Gemma-X-Mtp). llama.cpp not benching it = "their loss".
- An Opus agent surveys the mlxfast gemma trunk delta since our 08-28 clone (kernels AND MTP)
  - findings in the "mlxfast trunk delta" section below.

## Where we stand (facts, verified 2026-09-01)

- **Serving today, depth 1, qwen35 in-file nextn head.** `metal_mtp_spec_eval`
  (dasllama/dasllama_metal_decode.das:1475) = one draft forward (`metal_mtp_draft_forward`
  :1100, own slab row) + a B=2 same-slab verify (`acquire_step` nrows=2 :295,
  `encode_verify_step` :1372) + accept/reject with the dn dual-store ping-pong; the warm rides
  the verify cb (head pass at B=2 over [pre-draft mtp_h, bxf row0] = committed-history head
  KV). CPU twin `mtp_spec_eval` (dasllama/dasllama_common.das:4516). Metal prefill stashes
  mtp_h + warms the draft slab (PR #3530) - the servable flip landed 2026-07-21.
- **July board (M1 Max, wikitext2 8x256, real text):** 0.8B 1.26x@92.8, 4B 1.38x@89.2,
  9B 1.44x@88.7, 27B-MTP 1.26x@85.3, 35B-A3B 1.19x@81.7. Pre-deep-dense engine; M5 re-run owed.
- **v1 fat:** two cb waits per step (`finish_draft_step` + `finish_verify_step` :1441); two
  serial CPU `parallel_argmax` over the 248320-row vocab per eval (:1522, :1548); the full
  logits row pair copied back (:1466-1467).
- **Loader:** `blk.{n_layers}.nextn.*` read at dasllama/dasllama_load.das:2539-2548;
  `nextn_predict_layers` capped at 1 (:1653); an MTP-only sidecar GGUF is REFUSED (:1775) -
  exactly the shape ggml-org now publishes.
- **Fixtures.** ggml-org converts the trunk `--no-mtp` and the head `--mtp` into a SEPARATE
  file, for both carriers:

  | carrier | head file (ggml-org/<trunk repo>) | Q8_0 | tensors |
  |---|---|---|---|
  | Qwen3.8-27B (`qwen35`, dim 5120 x 64L) | `mtp-Qwen3.8-27B-{BF16,Q8_0,Q4_0}.gguf` | 3.2 GB | 18: `blk.64.nextn.*` (eh_proj 10240x5120, enorm, hnorm, shared_head_norm + the trunk-shaped block), `qwen35.nextn_predict_layers=1` |
  | gemma-4-26B-A4B-it (MoE) | `mtp-gemma-4-26B-A4B-it-{BF16,Q8_0,Q4_0}.gguf` | 462 MB | arch `gemma4-assistant`: 4 layers (sliding,sliding,sliding,full), Q-only - NO k_proj/v_proj anywhere; hidden 1024, backbone 2816, tied embed 262144 |

  Local: Qwen3.8-27B-Q4_K_M (0 nextn tensors), gemma-4-26B-A4B-it Q4_K_M + Q8_0; 2.7 TB free.
  **Head files pulled 2026-09-01** (`.sha` beside each, Q8_0 + Q4_0 twins of both). Header facts:
  - qwen head: `block_count=65` (includes the block), 18 tensors = the blk.64 full-attention
    block (attn_q 5120x12288 gated, k/v 4 heads x 256, ffn 17408) + the 4 nextn extras + COPIES
    of the trunk's `token_embd.weight` / `output.weight` / `output_norm.weight` (2 x 1.3 GB) -
    the loader skips the copies and binds the trunk's own tables.
  - gemma head: arch `gemma4-assistant`, 49 tensors, `nextn_predict_layers=4`,
    `embedding_length_out=2816`, `shared_kv_layers=4`, 16 q heads (kv 8 sliding / 2 global per
    the HF config), key_length 512 global / 256 swa, its OWN tied `token_embd` 1024 x 262144
    (268 MB of the 462), `nextn.pre_projection` 5632->1024, `nextn.post_projection` 1024->2816
    (the feedback hidden for the next chain step), `blk.N.layer_output_scale`, no k/v anywhere.
  - **No centroid tensors in ANY conversion** (ggml-org GGUF, mlx-community qat-4bit): the
    official `google/gemma-4-26B-A4B-it-assistant` checkpoint is gated (no HF token on this box)
    and carries `num_centroids=2048`, `centroid_intermediate_top_k=32`. The centroid mask needs
    either the token (pull the centroid tensors as our own sidecar) or self-computed centroids
    (k-means over the drafter's embed table - legal, the target verifies every token). The HF
    tensor names contain `masked_embedding` (llama.cpp's converter filters exactly that
    substring); vLLM auto-enables the mask only for E2B/E4B, so on 26B it is a measured
    question, not a given. Recon in S3.
  The llama.cpp PR #27836 is Qwen3.8-**Flash-Next** (`qwen4exp`, hyper-connections, 512-expert
  MoE) - a different architecture, NOT this arc's carrier.
- **mlxfast.** gemma repo clone `~/Work/mlxfast` (5439915, 08-28; upstream fedabc7 09-01);
  qwen38 repo cloned `~/Work/qwen38-mtp-challenge` @ 0863b06 (track closed 08-23; MIT). Nobody
  does wide: qwen38 = width 1, depth k in 0..8 adaptive, median 4.69 accepted/round, 2.69x
  pooled; gemma = width 1, depth 0-3; llama.cpp = linear chain; vLLM deleted trees.
  Key reads: `Sources/MLXFastModel/Qwen36MTPBlockSession.swift` (114 KB: accept walk,
  schedule, streak gate, probe fraction), `Qwen36MTPHeadAttachment.swift` (head quantization
  acceptance), the `Qwen35CustomQMV` M=2..9 kernels, `mtp-head.manifest`; gemma:
  `Vendor/.../MTP/CBv2MTPDepthController.swift`, `EngineLoopV2+MTP*.swift`, `Gemma4MTP.swift`.

## The scheme (first pass = mlxfast's, width 1 depth N)

One round at position `pos`, committed token `tok`, depth `k`:

1. **Draft chain** - k head forwards, each appending its own draft-slab row at `pos+i` and
   emitting `d_i = argmax` IN-GRAPH (GPU argmax; the greedy spec chain's kernel exists -
   `spec_cls_capable` dasllama_metal_decode.das:95). Native qwen head: step i input is
   `eh_proj([enorm(embed(d_{i-1})) ; hnorm(h_{i-1})])`, `h_0` = the trunk's stashed mtp_h,
   `h_i` = the head's own output hidden (llama.cpp "single head reused autoregressively";
   vLLM re-enters the one nextn module).
2. **Verify** - ONE B=k+1 batch-decode forward of the trunk over rows
   `[tok, d_1..d_k]` at `pos..pos+k`; `truth_i = argmax(row i)` in-graph. Accept walk:
   `a` = leading rows with `truth_i == d_{i+1}`; committed = `d_1..d_a, truth_a` (a+1 tokens).
   The warm rides the same cb at B=k+1 (head pass over the trunk hiddens of rows 0..k)
   so slab rows `pos..pos+a` are committed-history entries after the walk - the depth-1
   mechanism generalized, nothing new.
3. **Acceptance packet** - drafts + truths as one int32 buffer, ONE readback per round (the
   round's single host sync; mlxfast). No logits rows cross to the CPU on the hot path.
4. **Commit** - KV watermark -> `pos+a+1` (rows above are garbage the next round rewrites -
   the shipped rule); `mtp_h` = row a's post-final-norm hidden; `mtp_h_pos1 = pos+a+1`.
5. **Recurrent rollback** (48 of Qwen 27B's 64 layers are gated-delta-net; the state is not
   a KV ring). Accepted state = post-row-a, unknown before the walk. Options:
   (A) per-row checkpoint ring - the dn scan kernel writes each row's post-state to slot i
   (generalizes the shipped shadow-store, one uniform; ~2 MB/layer, ~100 MB per checkpoint at
   27B, k+1 checkpoints of write traffic per round);
   (B) mlxfast's lazy exact prefix replay - keep the post-row0 checkpoint (exists) and replay
   rows 1..a through the recurrence on partial accept;
   (C) checkpoints at rows 0 and k (the two common outcomes), replay for partial.
   First pass = (A): least new code, bit-exact by construction. The profile decides whether
   (B)/(C) follow (ledgered).
6. **Depth policy** - a fixed-k lever first (`--mtp-depth N`), then the adaptive controller:
   EWMA wall cost per depth + per-position conditional acceptance + hysteresis + exploration
   probes (mlxfast `CBv2MTPDepthController`, exercised by the qwen38 track: depth floor 6 under
   ceiling 7, probe fraction 0.15). Priced off the measured t(M), position-aware (acceptance
   -51% at 30K on a shipped head).

**Gemma drafter path (the new mechanism).** The assistant is a separate 4-layer model with
NO K/V of its own: input `concat(embed(last_token), target pre-norm hidden)` ->
`pre_projection` -> 4 layers whose attention takes Q from the drafter and K/V from a FROZEN
snapshot of the TARGET's KV at two layers (last non-shared full, last non-shared sliding) at
a constant RoPE anchor with bidirectional masks -> `post_projection` -> tied lm_head (262144).
Consequences: no second cache, no drafter prefill, no drafter rollback; the chain is pure
compute over a ~250-460 MB model. llama.cpp's `is_mem_shared` mode adds every draft token at
the SAME position. Steal: the centroid lm_head mask (262144 -> 4096 candidates via 2048
centroids) that the C++ reference drops at conversion - recon whether the GGUF carries the
centroid tensors. Verify = B=k+1 on the gemma MoE trunk via the batch rail; rollback = KV
watermark only (no recurrent layers) - recon the sliding-window ring rows.

## Slices

### S0 - recon, fixtures, and the harness FIRST (no drafter work before this is green)

- Fixtures: pull `mtp-Qwen3.8-27B-Q8_0.gguf` + `mtp-gemma-4-26B-A4B-it-Q8_0.gguf` (+ the
  Q4_0 twins for the drafter-quant question) into `~/Work/llama.cpp/models`; `.sha` beside.
- Read the two mlxfast files named above; write our own shape table of the verify leg
  (qwen38's: 257 wide QMV calls/round, N 5120..248320 x K 5120..17408).
- **Sequential-vs-verify parity harness**: same tokens, plain decode vs B=M verify rows;
  per-row argmax equality at temp 0 with a ULP tie tolerance (vllm-metal gate) and a
  serial oracle (mlxfast). Lands in `tests/test_mtp.das` as the standing gate for M in
  {2,3,5,9} + a `harness/` driver that prints per-position drift. Also asserts the head
  drafts from COMMITTED history (the qwen38 shipped tree drafted from an empty per-round
  cache: acceptance 0.26-0.64 instead of 0.90, invisible to correctness tests).
- **t(M) curve**, M in {1,2,4,8,16}: the mv/mvb family at nrows M in the gemv lab
  (`benchmarks/matmul/bench_metal_gemv_kernels.das`) at the 27B shapes, then a whole-step
  verify timing. Decides the depth cap and whether N-wide is ever cheap.
- **M5 re-baseline** of the shipped rail: `lcpp_bench --mtp-ab` on 0.8B / 27B-MTP / 35B-MTP
  at HEAD, predictions logged first (below).

### S1 - split head -> single dlim (qwen carrier)

- `load_model_cached(path)` (dasllama/dasllama_image.das:1495) pairs the trunk with its head
  sidecar: auto-discover `mtp-<trunk>.gguf` beside the trunk, explicit override flag. The head
  GGUF's `blk.{n_layers}.nextn.*` + block tensors read into the SAME offsets the in-file form
  uses (dasllama_load.das:2539 path); `nextn_predict_layers` taken from the head file; the
  :1775 refusal becomes "sidecar without a trunk beside it".
- Identity: the head file's identity (size + content hash) folds into
  `image_identity_of`/`dlim_identity` so a trunk-only dlim and a trunk+head dlim never share
  a path; the minted image carries the head - one file at serve time (ruled).
- Gates: `test_mtp_loader` for the split form; draft argmax parity vs the CPU rail on
  Qwen3.8-27B; the existing `-MTP-` in-file fixtures unchanged.

### S2 - depth N on the native-head rail

- Generalize `nrows=2` -> `k+1` through `acquire_step` / `encode_verify_step` /
  `finish_verify_step` / the warm; the draft chain with in-graph argmax; the acceptance
  packet; the accept walk; dn per-row checkpoint ring (option A); counters `drafts`,
  `draft_tokens`, `accepted_tokens`, per-position acceptance as a survival curve (vLLM's
  four counters, exact formulas).
- Fixed-depth lever; CPU rail stays depth 1 (planar keeps its own path; not a target).
- Gates: `test_mtp` invariance at k in {1..4} including forced-reject at EACH position
  (`DASLLAMA_MTP_DEBUG=reject` generalized to a position); 27B invariance; kernels suite;
  arm2 decode; fam-qwen35 matrix.

### S3 - gemma assistant drafter

- Loader for arch `gemma4-assistant` (embed tied to the trunk's, pre/post projections, 4
  Q-only layers, layer_scalar); the two-layer KV snapshot hook on the Metal decode path (a
  view, not a copy - fence against the in-place row writes of the round); the draft forward
  at the anchor position; verify B=k+1 on the MoE trunk (the qwen35moe B=2 arm from July
  generalizes; gemma4 MoE nst arm - recon); the centroid mask if the tensors exist.
- Gates: invariance tests on gemma-4-26B-A4B Q4_K_M + Q8_0; forced-reject; image + audio
  suites untouched (the flavor pick is metal-mode-gated).

### S4 - the depth controller

- Adaptive depth per the mlxfast shape, priced off t(M) (the same numbers the lab measured -
  a scheduler fitted to a SEPARATE cost model drifts from the launched geometry; their open
  PR #1388 unifies the two). Position-aware. Lands behind a lever; fixed-k is the fallback.

### S5 - fat

- One command buffer per round (draft chain + verify + warm; in-graph argmax makes it
  possible), one wait, one readback. Kills the two waits and the two CPU argmaxes.
- Drafter quant twins raced (BF16 measured 9-10% slower than Q8_0 upstream; whole-head 2-bit
  collapses acceptance; Q4 + BF16 islands was qwen38's working point).
- Ledger, not this PR unless cheap: compact draft vocabulary (98336 of 248320 rows on qwen38,
  +5.3%), two-tier coarse/rerank proposal, replay-based rollback (B)/(C), tight-grid dispatch
  for the verify grids.

### S6 - SpecBench harness + the reference row

- Freeze the Spec-Bench 480-prompt JSONL (Apache-2.0, hemingkx/Spec-Bench) subset + our
  counting/board prompts as tie-proof controls into the corpus dir; greedy only.
- Bench arms: plain vs mtp at k (and adaptive) with the counters above + per-sequence p95
  (the BASS straggler: batch-8 first seq 2.44x vs last 1.08x). Temp-0 task accuracy rides
  the harness - accepted length alone is insufficient (GSM8K 93 -> 62 incident).
- llama.cpp reference cells via `llama-speculative-simple --spec-type draft-mtp` (qwen: no
  `-md`, head rides the second context; gemma: the assistant GGUF) - recon its output format
  and pin the invocation in the ref-mint script; `llama-bench` cannot do it.

### S7 - docs, board, close

- `ARCHITECTURE_ENGINE.md` / `ARCHITECTURE_GPU.md` gain the MTP mechanism sections (today
  only mentions); `REVIEW.md`/`REVIEW_GPU.md`: tune races include the verify widths M=2..9
  explicitly (llama.cpp's GEMV gate swallowed the verify batch: -25.8%); README rows;
  `PERF_LEDGER.md` entries; `followup_*` for the ledgered fat; site rows per the ruling at
  arc end; LAWS.md for the rulings; memory.

## Predictions (logged BEFORE each measurement)

- t(M): the mv family near-flat to M=4 (weights-bound), knee at 8; whole-step verify at
  k=3 within 1.3x of a plain step on 27B.
- M5 `--mtp-ab` re-baseline of the shipped depth-1 rail: dense 1.2-1.4x, 35B MoE < 1.15x.
- Depth-N on Qwen3.8-27B, k=3 at ~85% acceptance: 1.6-1.9x over plain (upstream M3 Max:
  +42% with a GEMV-bound verify; mlxfast pooled 2.69x after a week of kernel work).
- Gemma drafter first pass: 1.4-1.7x at B=1 (mlxfast measured 1.95x on M4 Max; vLLM 1.60x).

## Traps (build against these)

- Multi-position verify != 1-position decode on bf16/quant kernels (~1.3% logit drift) and it
  eats acceptance SILENTLY (31% vs 80% same drafter cross-engine). Harness first.
- Fresh head cache per round (see S0). Assert committed history.
- GEMV gates swallowing the verify batch - race M=2..9 explicitly, per width (cliffs are
  per-M: qwen38 "M=4 QMV IPG 4->2 removes a 2.4x width cliff").
- Whole-head 2-bit collapses acceptance; BF16 drafter slower than Q8_0.
- Long context: acceptance -51% at 30K - the policy consults position.
- Accepted length is not sufficient; temp-0 task accuracy rides.
- MoE spec economics contested (expert union vs vLLM's 1.60x@B1 -> 2.27x@B64 on gemma-26):
  settle by OUR M-curve on the gemma trunk, not by either report.
- Measurement discipline: one process per box, cv > 3% void, never `-r 1`, Parsec off.

## mlxfast gemma trunk delta since 08-28 (Opus survey, 2026-09-01)

69 commits 08-28 -> 09-01 (21 solvers, +15,246/-719 across 38 files; tip moved on to 3bcf827).
Composite delta not derivable from the repo (no leaderboard file); formula
`prefill_gain^0.25 * decode_gain^0.75`, token fidelity a **10% divergence budget, not
equality** (`docs/participant-contract.md:492-497`) - the research memory's "sealed by
bit-exactness" reading was wrong.

**MTP verdict: one commit, a two-line constant flip.** `938df76` (08-30) set
`CBv2MTPRoundDriver.submissionDraftDepth` 0 -> 3 + `"arm": "mtp"` in the manifest. No drafter,
verify, acceptance-packet or depth-controller code changed anywhere in the range; speculation
was OFF for the repo's whole prior history. The arena's MTP lane is open and unexploited - 68 of
69 submissions were kernels. So mlxfast-gemma is a KERNEL intel source; the MTP design reference
stays the (closed) qwen38 track.

Per class: (a) GEMM/matmul 19 - batch-8 quantized GEMV promoted to `simdgroup_float8x8` MMA
with 4-bit codes unpacked straight into fragments (`gemma4_qmv_mma8_affine4_g64_impl`,
`quantized.cpp:2244-2420`; the ONE non-bit-exact tier, <=1 bf16 ulp, 0 argmax flips/400 rows) -
**this is the verify-leg shape at M=k+1**; (b) attention 16 - batch-wide two-pass split-K decode
attention (partition count solved for the BATCH: 128 -> 32 -> 16), GQA head pairing on one
simdgroup, xor-butterfly reductions; (c) fusion 13 - per-layer glue 9 -> 3 dispatches, RoPE into
QKV-norm, the norm emits the next GEMV's activation-sum table from registers; (d) dispatch 12 -
tight grids (211,200 -> 52,800 threadgroups on the dense MLP; LM head 262,144 -> 65,536 live),
early graph submission; (f) residency 6 - KV ring bf16 -> q8 -> q4-g64 authoritative,
`MLX_MAX_OPS_PER_BUFFER` 50 -> 512.

Adoption candidates (external figures, their box; each adopts only through our own race):
1. Tight-grid dispatch - identical kernel text on a hand-sized grid; zero numerical risk.
2. Batch-M quantized GEMV on the matrix units - the verify at M=2..9 is exactly this shape.
3. Delete the MoE route sort - one-dispatch simd rank-scatter for the [8,8] decode table
   (`SwitchLayers.swift:383-415`), 392.6 of 5508 ms on their M4 and latency-bound.
4. Split-K partition for the batch + GQA pairing + butterfly reduce (`RaggedTwoPass...:55-87`).
5. Command-buffer op budget; never materialize `scale*q` at scale 1; causal mask folded into
   the QK^T epilogue with a NEGATIVE zero bias (`x + (-0.0)` is identity for every float).

Traps the ratchet corrected (do not port the earlier version): `bc05fe4` pass-B lane guard
(silent wrong answer below 32 blocks, fixed `8fbf2f3`); two-deep weight prefetch on o_proj
landed and reverted twice - a measured LOSS there, survives only on QKV; two rails shipped then
disarmed at the tip (`..._EXPERT_PREFIX_BOUNDS`, `..._MLP_MMA8_DOWN_LANE_SUMS`); PREFILL-SOFTMAX-
SHAPE submitted and deleted three times (JIT compile inside the scored window); the arena
rewards resubmitting unchanged trees to re-roll noise - several per-commit claims sit at the
A/A floor (-0.09%).

## Open for Boris

- Default depth cap for the fixed-k lever (proposal: measured from t(M), likely 3-4).
- Adaptive controller in THIS PR or fixed-k ships first (proposal: both, controller behind
  a lever).
- N-wide experiment: after S4, only if t(M) says a width-2 verify is near-free.
