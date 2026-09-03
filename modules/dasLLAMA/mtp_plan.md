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
- **Design (recon 2026-09-01) - the head rides the SHARD mechanism.** Both load paths
  (`load_gguf_impl` dasllama_load.das:1580, `load_gguf_streaming` :1492) already resolve
  `gguf_shard_paths(path)` (dasllama_gguf.das) and merge shard tensor directories through
  `parse_gguf_meta_shards`; `gguf_find_tensor` is first-match, so appending the head file as the
  LAST shard makes its `token_embd`/`output`/`output_norm` copies dead by construction (the
  trunk's win). Three additions: (1) `gguf_shard_paths` also appends `mtp-<trunk>.gguf` when it
  sits beside the trunk (or the explicit override path); (2) shard KVs are shard-0-only, so the
  merge promotes the head shard's `<arch>.nextn_predict_layers` (and block_count) onto the meta
  (a sidecar field on `GGUFMeta`, read where :1652 reads the key); (3) `image_path_for`
  (dasllama_image.das:71, 20 call sites) folds the sidecar's basename+size into the hashed
  identity by the same adjacency discovery, so trunk-only and trunk+head images never share a
  path and `image_peek` shows which one a dlim is. The mint itself needs nothing: the image is
  written from the loaded Model, so the head is inside it - one dlim, as ruled.

#### S1 status (2026-09-01) - IMPLEMENTED, gating in flight

- `DASLLAMA_MTP_HEAD` (EngineEnv `mtp_head`) + `set_mtp_head_path`/`clear_mtp_head_path`
  (dasllama_gguf.das); `mtp_head_sidecar(path)` resolves override > `mtp-<trunk basename>` sibling
  > none, never on a head loaded alone; `gguf_shard_paths` appends it as the LAST shard (both load
  paths and `model_available`'s size tier inherit it); `parse_gguf_meta_shards` promotes the head
  shard's `<arch>.nextn_predict_layers` onto `GGUFMeta.nextn_sidecar` when shard 0 lacks the key;
  `load_gguf_parsed` takes nextn from the sidecar and keeps `layers = block_count - nextn_in` (a
  split trunk's block_count counts only itself); `image_path_for` folds `|mtp:<name>:<size>` into
  the path hash. ENVIRONMENT.md regenerated; ARCHITECTURE_ENGINE.md 1.3 carries the statement.
- Gates: `tests/test_gguf_shards.das` +4 model-free arms (adjacency, override, missing override
  attaches nothing, a head alone never recurses) GREEN; `test_metal_mtp_parity.das` fixture
  `3.8-27b` (Qwen3.8-27B-Q4_K_M trunk + mtp-Qwen3.8-27B-Q8_0 head via the override) - large tier:
  **GREEN** (M5, 2026-09-01) - head attached on both the planar and the image load, ONE 23 GB
  image minted under its own path hash (`...gguf.0x4f944f603502a56b.dlim`), forced-feed maxd
  0.00088 / 0.0011, zero flips, 48/48 rounds drafted, counting exact, no leaked Metal objects.
  Qwen3.8-27B's MTP head serves on this engine.

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

#### S2 design (recon 2026-09-01) - what generalizes, what is new

- **Chain** = k calls of the existing `metal_mtp_draft_forward(d_{i-1}, pos+i-1)`
  (dasllama_metal_decode.das:1100): each drafts at slab row `pos+i-2` from `(embed(d_{i-1}),
  s.mtp_h)`, and `finish_draft_step` leaves `s.mtp_h` = the head's OWN output hidden with
  `mtp_h_pos1 = 0` - exactly the chain input for step i. No new kernel; k cb waits until S5's
  in-graph argmax folds the chain into one cb.
- **Verify** = `acquire_step(..., nrows = k+1)` (:295; every buffer already scales by nrows) +
  `encode_verify_step`/`encode_verify_layer` (:1372/:1176) with the ~60 `2l` row literals
  replaced by `r.nrows`: the route table gets one entry per row (`pos+1..pos+k+1`), the kq sites
  already take `nrows` (`enc_kq_site_b` -> `enc_kq_mvb` serves live rows 2..8 via the B2/B4/B8
  ladder), the q8 sites (`enc_gemv_b`/`enc_gemv_w13sw_b`: fixed B2/B4 via `four`) need the row
  count threaded (B8 form or a 2xB4 tile), attention/rope/dn/moe encoders take `nrows`/`npos`
  already. The warm runs at the same width over the k+1 trunk hiddens (the cat assembly of 4
  `enc_copy_row`s per pair becomes a batched interleave - `MetalPfCat2` from the prefill warm).
  `finish_verify_step` (:1441) lands k+1 KV rows and k+1 logits rows.
- **Recurrent rollback** (option A): `MetalDnScan` stores the post-row-p state into slot p for
  every p < npos-1 (today: only p+2 == npos into the one shadow region, kernels.das:11177);
  `DnMirror.regions = kmax+1` (dn_mirror_regions), `dn_mirror_prepare` returns the slot bases;
  commit with `a` accepted: a == k -> the live region stands (advance to pos+a+1); else `cur` =
  slot a (flip generalized to `dn_mirror_select(uid, slot, pos)`). Cost: k x ~2 MB/layer x 48
  layers of extra writes per round at 27B (~1 ms) - measured against the replay variant (B)
  before S5 decides.
- **Accept walk + commit**: `a` = leading rows with `argmax(row i) == d_{i+1}`; commit
  `d_1..d_a`, bonus = `argmax(row a)`; KV watermark -> `pos+a+1`; `mtp_h` = row a's `bxf`;
  `mtp_h_pos1 = pos+a+1`; counters `mtp_drafted += k`, `mtp_accepted += a`, plus the per-position
  survival counters (`mtp_pos_acc[i]` = rounds where row i accepted).
- **API**: new `mtp_spec_round(t, s, tok, var out : array<int64>&) : int64` (returns a, pushes the
  accepted drafts) behind a new override registry; `mtp_spec_eval` stays as the depth-1 wrapper
  (the scheduler and the CPU rail keep it); `generate_mtp_greedy`, the bench tg-real arm and the
  parity test move to the round. Lever: `set_mtp_depth(k)` / `DASLLAMA_MTP_DEPTH` (default 4 per
  ruling; k=1 must reproduce today's rail bit for bit - the forced-feed test is the proof).
- **Order**: (1) lever + `nrows` plumbing at k=1, bit-identical (test green unchanged);
  (2) dn slot ring + accept walk at k=2..4 (tests at each k incl. forced reject at EACH position:
  `set_metal_mtp_debug("reject:i")`); (3) API + callers; (4) `lcpp_bench --mtp-ab` depth sweep on
  the 0.8B / 27B / 3.8-27B / 35B for the k curve.

#### S2 step 1 DONE (2026-09-01): the general round at k=1 is bit-identical
`metal_mtp_spec_round(t, s, tok, k)` (draft chain into `s.mtp_vbatch[1..k]`, `acquire_step` at
nrows = k+1, `encode_verify_step`/`encode_verify_layer` fully on `r.nrows` - per-row route table,
per-row cls, the warm's cat image assembled per row - `finish_verify_step` landing k+1 KV rows and
k+1 logits rows into `s.mtp_logits_b`, the accept walk, commit of rows 0..a). `metal_mtp_spec_eval`
is the depth-1 wrapper. Levers: `set_mtp_depth`/`get_mtp_depth` (`MTP_MAX_ROWS = 9`). No hot-path
allocations (the batch and the logits image live on the Session). The forced-feed suite reads the
SAME digits as before the rewrite (0.8B maxd 0.0016374588 / 0.00065612793). k > 1 clamps to 1 on
recurrent trunks until step 2 (the pre-verify replay); attention-only trunks would serve k > 1
already (KV rollback is the watermark). The qwen38 memo (`~/.claude/plans/mtp-research/
qwen38_design.md`) rules for step 2: replay rows 0..a from the PRE-verify state with a store-only
scan variant, conv state = a slice of the verify's conv inputs, no per-row checkpoints.

#### S2 step 2 (2026-09-01): depth N serves on recurrent trunks - split-write + replay
- `DnArgs.split`: the deltanet scan and conv-history kernels store their final state/taps into
  the shadow region when set; the live region keeps the PRE-verify state. The verify runs split.
- Replay tape: each recurrent layer's scan inputs (l2-normed conv rows, qkv rows, raw beta/alpha,
  k+1 rows each) are copied into a per-layer slot during the verify; a partial accept a < k
  replays rows 0..a from the live state into the shadow with the SAME kernels (npos = a+1,
  split) - bit-exact with the verify's own boundary state by construction - then the shadow
  becomes live (`dn_mirror_flip(pos+a+1)` on every commit; a == k needs no replay). The July
  mid-store (post-row-0 shadow) is superseded; the `mid` path stays for the prefill/decode callers.
- The chained draft's watermark gate takes the round's base position (`wm_pos`): a draft at
  pos+i reads only slab rows its own chain wrote above the base.
- q8 batched GEMV at 5..8 live rows: `enc_gemv_rows` / `enc_gemv_w13sw_rows` tile two B4 forms
  (x offset by 4 rows of n, y by 4 rows of the site stride - `enc_mv_site`'s shape); the B24T and
  W13Sw templates gained the `xoff` binding. Found by the trace: at nr = 5 the B4 classifier
  left row 4 unread, so every bonus token was id 0.
- Facade: `mtp_spec_round(t, s, tok) : int64` (accepted drafts in `s.mtp_vbatch[1..a]`) over a new
  override registry; `set_mtp_depth(k)`; `set_metal_mtp_debug("trace")` logs drafts/truths/a per round.
- Gates on the 0.8B (recurrent, 18 dn + 7 attn layers): depth 1 forced-feed unchanged to the
  digit (0.0016374588 / 0.00065612793); depth 2 and 4 forced-feed (every round a k+1-row verify +
  a 1-row replay) same digits; counting free-run token-exact vs plain at depth 1, 2 AND 4 (full
  accepts, partial accepts, rejects).
- **Two bugs the large tier found, both fixed:** (1) at nr >= 5 the fixed-B GEMV forms write
  their FULL 4-row (B4 tile) or 8-row (B8) group, and the verify sized its row buffers to exactly
  nr rows - the pad rows overflowed into the neighbouring pooled buffers (27B-MTP k4 maxd 25,
  Qwen3.8 NaN; the 0.8B survived by luck). `acquire_step` now lays a multi-row verify out at the
  4-row-padded stride the batch rail already uses (single-row steps stay at one row - padding
  them over-read the session's one-row inputs and SIGBUSed). (2) The test's GPU-prefilled pairs
  need `metal_prefill_shutdown()` before the leak gate (200 live panels were the prefill's, not a
  leak). The pairs prefill on the GPU now (the planar CPU prefill of a 27B cost minutes per
  session and blew the runner's 1200 s budget once the depth arms doubled the pair count).
- `set_metal_mtp_debug("trace")` (one line per round: drafts, per-row truths, a) found bug (1)
  in one look - every round's last truth was id 0. Callers moved to the round: `generate_mtp_greedy`
  (drains the accepted drafts), `lcpp_bench --mtp-ab --mtp-depth k`.

#### S2 measurements: per-phase timing + SpecBench-4 (2026-09-01)

- **27B round split (`DASLLAMA_MTP_DEBUG=time`, plain step ~44 ms):** k=1 verify 44.9 ms (22.4/row),
  k=3 verify 79.8 ms (19.9/row), k=4 verify 153.4 ms (30.7/row); draft 2.8 / 8.5 / 11.3 ms; walk +
  commit < 0.7 ms; replay < 1.8 ms. The verify amortizes 2 -> 4 rows (per-row cost flat to
  improving) and hits a wall at row 5 (the q8 dn projections' two-tile + the B8 form); the
  recurrent replay is free. Tokens/round 1.77 / 2.61 / 2.79.
- **Qwen3.8-27B + Q8_0 head** chained: k=2 65.6%, k=3 56.0% (1.11x) - the newer head does NOT
  chain better than the 3.6's in our engine; both are ~83% at position 1 and ~55% deeper.
- **SpecBench-4** (`benchmarks/data/specbench4_prompts.txt`: writing 81, summarization 241,
  math_reasoning 401, qa 321; `lcpp_bench --prompts ... --chat-prompts` wraps each in the model's
  chat template, thinking off): 27B k=1 **83.1% / 1.31x** (off 26.7, on 34.9), k=3 59.3% / 1.19x;
  0.8B k=1 69.1% / 1.07x, k=3 44.7% / 0.96x. The corpus does not explain the 27B's deep-position
  starvation (same on both corpora); the 0.8B is corpus-sensitive the other way (chat is harder
  than wikitext for a 0.8B). Caveat: the tg-real row generates 128 tokens past end-of-turn (the
  llama-bench contract), which drags the two short-answer prompts - the S6 harness needs stop-at-EOS
  + per-task accounting.
- **Verdict for the qwen carriers:** depth 1 ships (1.24-1.31x on the 27B); depth > 1 is break-even
  because the nextn heads starve past position 2 - a head property, not corpus or engine. The
  controller (S4) makes that automatic per head. The gemma assistant drafter (trained multi-token)
  is the carrier that can show depth paying - S3 next, per ruling.

### S3 - gemma assistant drafter

- Loader for arch `gemma4-assistant` (embed tied to the trunk's, pre/post projections, 4
  Q-only layers, layer_scalar); the two-layer KV snapshot hook on the Metal decode path (a
  view, not a copy - fence against the in-place row writes of the round); the draft forward
  at the anchor position; verify B=k+1 on the MoE trunk (the qwen35moe B=2 arm from July
  generalizes; gemma4 MoE nst arm - recon); the centroid mask if the tensors exist.
- Gates: invariance tests on gemma-4-26B-A4B Q4_K_M + Q8_0; forced-reject; image + audio
  suites untouched (the flavor pick is metal-mode-gated).
- **Design skeleton (recon 2026-09-01; the mlxfast drafter memo refines it):**
  - *Target facts* (gemma-4-26B-A4B Q4_K_M header): 30 layers, dim 2816, pattern 5 sliding + 1
    global per 6 (global = layers 5, 11, 17, 23, 29), sliding KV 8 heads x 256 / global KV 2 heads
    x 512, 16 q heads, 128 experts top-8 (expert ffn 704) + the dense shared FFN (2112), softcap
    30, rope 1e6 / swa 1e4, `shared_kv_layers = 0`, per-layer `layer_output_scale`, pre/post norms.
    The same-slab `encode_verify_layer` refuses every one of these (dual rope, pre_post_norm,
    layer_out_scale, softcap, hetero heads); the BATCH driver (`metal_batch_decode_forward`) serves
    them all at B rows today (gemma4-12B B=2 token-exact, 26B engage cells).
  - *Verify = the batch driver in a SAME-SLAB mode*: one session repeated k+1 times with
    `ws.positions[i] = pos + i`; two deltas - `mirror_prepare` runs once at the base (rows above it
    are written by this very step; the route table's cnt_i = pos+1+i already orders row i's
    attention after rows < i's stores, exactly the same-slab verify's discipline), and the landing
    keeps the per-row logits (`ws.logits_b` is already B x vocab) plus the per-row post-norm
    hidden rows for the drafter's next input instead of scattering into one session's row.
    Synchronous (finish_pending_step right after). No dn state on gemma (attention-only trunk):
    rollback = the KV watermark alone. No head warm (the drafter is KV-less).
  - *Drafter* = a mini-Model loaded from the `gemma4-assistant` sidecar (the shard walk attaches
    it like the qwen head, but a shard whose `general.architecture` differs from the trunk's is a
    DRAFTER, not a nextn block): 4 layers at dim 1024 (3 sliding, 1 full; q_proj 16 heads x 256 /
    x 512, q_norm, o_proj, dense MLP 8192 gelu, per-layer `layer_output_scale`, post norms), own
    tied `token_embd` 1024 x 262144, `nextn.pre_projection` 5632 -> 1024 over `[embed(last) |
    target hidden]`, `nextn.post_projection` 1024 -> 2816 = the feedback hidden for the chain.
    Its attention has NO K/V of its own: a new kernel attends the drafter's Q over the TARGET's
    K/V mirror rows at two capture layers (last sliding = 28, last global = 29) at a constant RoPE
    anchor with a bidirectional mask over the frozen prefix - a snapshot view into the arena, not a
    copy (the fence: the verify's row writes must not land before the chain has read).
  - *Chain*: k drafter forwards at the anchor, step 0 from the target's hidden, step i from the
    drafter's post_projection output; argmax over the drafter's 262144-row tied table (the
    centroid mask is NOT in any conversion - `masked_embedding` tensors dropped; k-means over the
    embed table is the legal fallback, a later item).
  - *Round* = the qwen round with the draft step and the verify swapped: chain -> same-slab batch
    verify -> accept walk -> commit (watermark + carry hidden = row a's post-norm hidden).
  - **Memo facts that pin the drafter** (`~/.claude/plans/mtp-research/gemma_drafter_design.md`,
    2026-09-01): input = `concat(TARGET token_embd(tok) * sqrt(2816), target hidden)` -> pre_projection;
    the target hidden is POST final norm in llama.cpp and PRE norm in mlxfast - **resolve by
    acceptance, feed both**; K/V from target layer 28 (drafter layers 0-2, sliding, GQA 2) and
    layer 29 (layer 3, full, GQA 8); attention scale 1.0; rope 1e6 (full, with the 64/192 proportional
    freq table) and 1e4 (sliding); ONE constant anchor position for every chain step; masks =
    sliding lower bound only (keys > anchor - 1024; the ring-boundary key at exactly anchor-1024
    must be masked); GELU-tanh gated MLP 8192; per-layer `layer_output_scale` 0.29/0.50/0.53/0.41;
    tail = output_norm -> logits over the drafter's OWN tied token_embd (no softcap) and
    post_projection -> the 2816 feedback hidden. The frozen snapshot is reused unchanged for the
    whole chain (a VIEW of the two mirrors, fenced against the round's own row writes). Centroid
    mask: dead for this checkpoint (`use_ordered_embeddings: false`, no tensors anywhere) - not built.
    mlxfast's gemma arm is currently DISARMED (`speculationEnabled = false`): their rectangular verify
    was not argmax-exact vs serial on the quantized target (7/8 prompts forked at near-ties), so they
    sealed a serial oracle - our forced-feed tolerance policy is the honest instrument here.
  - **Batch-rail defects found by the verify gate (2026-09-01)** - the gemma-4-26B-A4B batch step
    had never had its logits checked (the support matrix's batch cell is ENGAGE-only, tolerances
    cover single decode); GPU batch vs GPU single at B=2 read maxd 48 with 46/48 argmax flips, CPU
    reference 23-30 away (single 2.1). Two independent bugs, both fixed in the batch driver:
    (1) the expert-plane stride tested `fmt == q8` for the block-plane case, so Q5_1 experts (the
    26B's `ffn_down_exps`) took the 256-element super-block stride - all nine MoE GEMV sites now
    route through `kq_sb(fmt)`; (2) the fixed-B mul_mv forms stripe K in 256 (B2) / 128 (B4)
    element chunks with no tail guard (their header says so) and the rail gate never checked the
    shapes - the 26B's dense hidden 2112 overran on the w2 site; each mv site now gates on its own
    K % 256 (`mv_kdim` / `mv_wo` / `mv_w2`) and falls to the tail-exact GEMV form otherwise.
    THIRD bug, found by the gemma round at k=4 (five verify rows): the alignment fallback I added
    for (2) called the B2/B4 GEMV form directly (`enc_gemv_b(nrows > 2)`, a 4-row form) at 5..8
    rows, so rows 4+ were never computed - distinct sessions at B=5 flip exactly 1 row in 5, at B=8
    exactly 4 in 8; in the same-slab verify the garbage K/V of those rows then poisons every later
    row. Every fallback now goes through `enc_gemv_rows` (two 4-row tiles at 5..8). Arms added for
    all of it: `mtp-vff5-<tag>` (5 same-slab rows), `mtp-dff8-<tag>` (B=5 and 8 distinct), the
    gemv oracle at K=2112. Ruling (Boris, 2026-09-01): every bug like these lands with its failing
    arm first; the qwen session hit the same class.
    Ruling (Boris): the kernel's alignment contract is a MACRO EXPANSION, not a lint - `requires =
    "ka.ndim % 256"` on `[metal_dispatch]` generates the check at every dispatch (the mv B2/B4 forms
    declare theirs; the toy fixture proves the trip); REVIEW_GPU.md duty + ARCHITECTURE_GPU.md statement.
    The sweep of the remaining header-sentence contracts (GEMM d % 64, mul_mm mp % 32, tensor GEMM,
    float4 views) is LEDGERED - `followup_general.md` #63, ruled for the END of the arc.
    Bisect controls that pinned it: gemma-4-12B dense batch clean (0.024), Qwen3-30B-A3B batch clean
    (0.14), `DASLLAMA_METAL_BATCH_CONCURRENT=0` bit-identical (not a hazard), `DASLLAMA_METAL_BATCH_MV=0`
    + the plane fix -> 0.96 / 0 flips. The `mtp-dff-<tag>` arm (distinct-session GPU batch vs GPU
    single + a CPU reference row) is the batch rail's parity gate from now on; fixtures l1b, g12,
    q30, g26.
  - **S3 DONE (2026-09-02)**: the drafter serves, GPU == CPU oracle to 6e-5, the fused round is
    token-identical to the per-step path at k = 1, 2, 4; SpecBench-4 depth 1 1.185x / depth 2 1.19x
    on gemma-4-26B-A4B (M5, debug-jit). Build notes follow.
  - **S3 build (2026-09-01)**: `dasllama/dasllama_mtp_gemma.das` = the sidecar loader
    (`GemmaDrafter`: Q8_0 weights as one q8 blob + F32 norms, offsets per layer) + the CPU reference
    step `gemma_draft_step_cpu` (the oracle); `dasllama/dasllama_metal_mtp_gemma.das` = the GPU
    drafter (`attach_gemma_drafter` uploads the blob once; `gemma_draft_step_gpu` = embed(tok)*sqrt(2816)
    ‖ h -> pre_proj -> 4 x [rms, Q gemv, per-head q_norm, Q-only rope via the fused store with every
    pair a Q pair, the DECODE's own attention kernels over the target mirror at layer 28/29 with
    cnt = anchor (keys < anchor: the seed is unfed), scale 1.0, sliding window on the chunked form,
    wo, post-norm, residual, rms, gate/up, geglu, down, post-norm, (residual + branch) * layer_scale]
    -> output_norm -> tied-embed logits -> GPU argmax; post_proj -> next h) and the round
    `gemma_mtp_spec_round` (constant anchor = pos for the whole chain; k drafts; `eval_verify_batch_`
    at k+1 rows; accept walk on mtp_logits_b; commit = row a's logits + row a's post-norm hidden from
    mtp_hrows; watermark/n_past = pos+a+1). It registers the `metal` round override after the decode
    module and delegates drafter-less models to `metal_mtp_spec_round`. The decode carries the
    post-norm hidden in `s.mtp_h` on head-less models via `set_metal_mtp_carry_hidden` (the drafter's
    input; pre-norm is the experiment variant still to add). `gguf_shard_paths` refuses an
    `*-assistant` arch sidecar (a drafter is not a shard). `lcpp_bench --mtp-ab` attaches the sidecar
    for gemma targets.
  - *Verify status (2026-09-01)*: the same-slab batch mode landed (`eval_verify_batch_`,
    `set_batch_same_slab`, the driver's shared row-0 mirror + `land_sameslab_rows`); the batch rail
    declines deltanet hybrids (the qwen twins keep the dedicated same-slab verify), so its gate is
    the head-less gemma-4-26B-A4B fixture (`test_metal_verify_gemma26`, arm `mtp-vff-g26`, bar 24.0
    = the support matrix's Q4_K_M forced-step bar). Post-fix trace: rounds 1-5 at the GPU-vs-GPU floor
    (0.003-0.01, same as the one-row arm's 0.007), then isolated spikes at pos 56/64/72/73 (0.97,
    1.8, 3.6, 11.2) with the argmax never flipping and the deviation decaying after - the routed
    experts amplifying a near-tie into the K/V history, not a same-slab defect. Distinct-session
    batch reads 0.96 over 24 steps; gemma-4-12B dense same-slab 0.128; Llama 0.003.

### S4 - the depth controller

**Ruled and landed (2026-09-02, Boris: "we drop controller for sure"):** the depth is a box knob per
round kind. `get_mtp_depth()` serves an explicit `set_mtp_depth` (the bench's `--mtp-depth`, now
0 = box default) when one was made, else `runtime.mtp_depth_assistant` / `runtime.mtp_depth_nextn`
by the `MtpRoundKind` the gemma drafter's attach/detach selects (defaults 1). The assistant knob is
MINTED: `tune_kernels` races depth 1 vs 2 as serving tg on the SpecBench chat corpus (`lcpp_bench
--mtp-ab --prompts specbench4 --chat-prompts -n 128 -r 3`, the `mtp=on` line) on a gemma-4 vehicle with
its `mtp-` head beside it; depth 2 must beat by 2% (its downside is asymmetric - a 6% loss on the M4
Pro, a tie here - and a tie is not worth the longer round), no vehicle = depth 1 with a
`DASLLAMA_CONFIRM_MTP_GEMMA` provisioning hint. The first cut raced on the bench's synthetic 32-token
prompt and depth 2 lost 10% there (137.5 vs 152.2): incoherent text is not the site shape. The M5
mint on the chat corpus reads depth 2 145.55 vs depth 1 145.29 tok/s - the tie the S3 table showed -
so this box's sidecar carries `mtp_depth_assistant = 1`; the knob earns its 2 on a box whose k-row
verify is cheaper than the M5's 1.2x. The NextN
knob is not raced (depth 1 won on every NextN carrier measured; #72 owns the 27B depth-3 gap).
Tests: `test_box_profile.das` `test_mtp_depth_knobs_apply_per_round_kind`. The EWMA controller design
below stays as the record of what was NOT built and why.

**Measured recommendation that led there (2026-09-02):** a fixed depth per carrier is within noise of
anything a controller could pick. gemma-4-26B-A4B: depth 1 and depth 2 yield the same tokens per
millisecond (1.185x vs 1.19x; 1.746 tok / 11.7 ms vs 2.27 tok / 15.25 ms); depths 3-4 lose on the
five-row rail. Qwen3.6-27B: depth 1 (1.31x) beats every deeper setting. The per-position curve is the
same shape on every task (p2/p1 ~0.7), so a controller has nothing to switch on except the task's
overall acceptance, which does not change the depth ranking. Proposal: drop the EWMA controller,
ship a per-carrier default (gemma 2, Qwen NextN 1), keep `--mtp-depth` as the override.

- Adaptive depth per the mlxfast shape, priced off t(M) (the same numbers the lab measured -
  a scheduler fitted to a SEPARATE cost model drifts from the launched geometry; their open
  PR #1388 unifies the two). Position-aware. Lands behind a lever; fixed-k is the fallback.
- **Design (from the qwen38 memo sec. 6, 2026-09-01):** per round choose d in 0..kmax by the
  greedy marginal rule - extend while `reach = prod_{i<d} p_i > price.marginal[d] * (1 +
  expected) / price.cumulative[d]`, where `p_i` = a per-position conditional acceptance EMA
  (P(draft i accepted | 0..i-1 accepted), seeded 0.85 * 0.98^i, alpha 0.15), and `price` = the
  measured per-depth round cost relative to a plain step (T(d) = V + sum marginal[k] * V) -
  MEASURED on this box at warm-up (a t(M) probe of the verify at M = 2..kmax+1 plus the draft
  step), never a fitted shape (their fitted shape lost its whole win on the crown's kernel table).
  Update: positions < a observe success; position a observes failure only on a real reject (not
  a stop-token stop); a full accept nudges position a toward 0.95 (transferred optimism, capped).
  Confidence gate: `p_0 = min(EMA[0], sigmoid(margin/2))` from the pending token's own top-2 logit
  gap (free from the acceptance packet); d = 0 (a plain step, no draft) when p_0 <= h, with h the
  head-step cost ratio (their 0.18, ours measured). Ceiling = the ruled 4; no floor (their floor
  was a measurement artefact). Stop tokens commit like any token and the round drafts past them.
  Denominator discipline for the board: the control is draft-count 0 on the same session with the
  head resident (our `--mtp-ab` OFF arm), never the depth-1 rail.

### S5 - fat

- One command buffer per round (draft chain + verify + warm; in-graph argmax makes it
  possible), one wait, one readback. Kills the two waits and the two CPU argmaxes.
- Drafter quant twins raced (BF16 measured 9-10% slower than Q8_0 upstream; whole-head 2-bit
  collapses acceptance; Q4 + BF16 islands was qwen38's working point).
- Ledger, not this PR unless cheap: compact draft vocabulary (98336 of 248320 rows on qwen38,
  +5.3%), two-tier coarse/rerank proposal, replay-based rollback (B)/(C), tight-grid dispatch
  for the verify grids.

### S6 - the ruler is ours, the story is the ladder (ruled 2026-09-02)

- No llama.cpp reference CELL. `llama-bench` has no speculative flags, a fork patch would spoil the
  stock instrument, and an MTP row cannot sit under the board's llama-bench convention (random-token
  tg128 says nothing about acceptance). Boris: "ruler is for us (u and me). ladder is to present how
  awesome our approach is" - llama.cpp is the yardstick, not a competitor; both projects borrow
  freely with attribution.
- The ruler = `llama-server /completion` on OUR rendered thinking-off prompt, raw, greedy, 128
  tokens, `ignore_eos`, `cache_prompt` off, `-t 6 -np 1`; gemma `-md <head> --spec-type draft-mtp
  --spec-draft-n-max K -ngld 99`, qwen NextN in-file. Identical continuation text is the control;
  timings carry `draft_n / draft_n_accepted`. Prototype: the session's `lcpp_srv.sh`; the story's
  measurement day re-runs it as a `.das` harness (popen + dasHV client) - written then, not before.
- Prompts: the SpecBench-4 chat subset in `benchmarks/data/specbench4_prompts.txt` (Apache-2.0,
  hemingkx/Spec-Bench, qids 81/241/401/321). The 480-prompt freeze and the per-sequence p95 /
  temp-0 accuracy counters are ledgered, not this arc.

### S7 - close: the stories (ruled 2026-09-02, LAST thing in the arc)

- dasllama.io gains a **stories** page. Voice: "Boris and Claude", plain engineering "we" - the
  project is AI development in the open (performance-critical to an obscene degree, Boris has not
  written a line of its code), and that is stated, not hidden.
- Story 1: the M5 news item is over-detailed - it becomes the first story ("why it was fun"; format
  tunable) and the news shrinks to a few lines plus a link.
- Story 2: "we now support MTP on Mac" news + the story: what we tried (depth, width, controller,
  drafter quant, pre/post-norm hidden), how we measured (the ruler above, identical prompts, same
  denominator, the thinking-mode trap as the lesson), the per-prompt table with acceptance for both
  engines, the depth curve with per-position acceptance, the round economics that cap the gain
  ("how small the gain is" is the honest core), where the prompts come from, the commands verbatim,
  **the date and the sha of the build** - "this is when we tried, this is what it was on that day".
  Not a competition: llama.cpp is the gold standard and is named as such.
- Story 3 (separate, later): the stale-sidecar afternoon - how a measurement lied and how we caught
  it (#71/#73). Not in this arc.
- Numbers for story 2 are records-grade: a measurement day with the release exe on a fresh mint for
  ours, and llama.cpp from a STOCK upstream build at a pinned commit (one that carries the gemma
  assistant head), both the same day on the same box. Dev-rail debug-jit numbers do not publish.
- Also in the close: `REVIEW.md`/`REVIEW_GPU.md`: tune races include the verify widths M=2..9
  explicitly (llama.cpp's GEMV gate swallowed the verify batch: -25.8%); README rows;
  `PERF_LEDGER.md` entries; `followup_*` for the ledgered fat (S5); the `requires=` sweep (#70);
  LAWS.md for the rulings; memory.

## S0 results

### M5 re-baseline of the shipped depth-1 rail (2026-09-01, `lcpp_bench --mtp-ab -n 128 -r 5
--ngl 99`, debug-jit, wikitext2 8 prompts, HEAD c0ca624f9 + plan commits)

| model | off t/s | on t/s | ratio | accept | July M1 |
|---|---|---|---|---|---|
| 0.8B Q8 | 357.9 ± 0.5 | 454.6 ± 1.7 | **1.27x** | 87.9% | 1.26x @ 92.8 |
| 27B-MTP Q4_K_M | 23.5 ± 2.1 (VOID, cv 9%) | 28.1 ± 0.3 | 1.19x nominal, ~1.25-1.3x settled | 81.5% | 1.26x @ 85.3 |
| 35B-A3B Q4_K_M | 103.1 ± 1.5 | 127.0 ± 0.3 | **1.23x** | 81.7% | 1.19x @ 81.7 |

Prediction check: dense 1.2-1.4x HELD; "MoE < 1.15x" MISSED - the MoE cell improved to 1.23x
(the plain MoE step nearly doubled on M5 and the B=2 expert-union verify kept pace).

Two findings that feed S0's remaining items:
- **Sustained-load GPU regime on the 27B.** Per-prompt GPU prefill climbs ~1.0 s (first pass)
  -> ~1.7 s (third pass) then plateaus; the 0.8B stays flat. Not a leak (plateau), not
  thermal (no pmset warning). The bench runs the OFF arm fully before ON, so OFF rides the
  fast first pass - a bias AGAINST spec. S6 duty: per-rep arm interleaving (the July ladder's
  shape) or a burn-in before the timed reps; the 27B off row is void until then.
- **Acceptance dropped 4-5 points on the DENSE cells only** (0.8B 92.8 -> 87.9, 27B 85.3 ->
  81.5) while the MoE cell is identical to July (81.7 = 81.7). Same corpus. The dense verify
  rides the kq mvb/gemv kernels the kq-race arc rewrote; the MoE verify rides enc_moe_gemv.
  RESOLVED by the forced-feed test below: the verify's row 0 matches the plain step to 1e-3, so
  the drop is NOT verify drift. Remaining suspects: the DRAFT side (the head's GPU forward on the
  current kernel crowns vs July's - a GPU-draft-vs-CPU-draft argmax parity probe, S2 recon), and
  the window (128-token tg-real rows vs the July ladder's 256). Acceptance is proposal-side
  quality; it moves speed, never correctness.

### Parity harness + the metal MTP test corpus (2026-09-01)

Two instruments, and the distinction is the whole story:

- **`tests/test_metal_mtp_parity.das`** (new, suite `mtp`) - the CORRECTNESS gate. Forced-feed:
  the plain GPU step and the verify's row 0 consume the IDENTICAL token every round (the spec
  round forced down the reject path via `set_metal_mtp_debug("reject")`), so their logits are
  compared before any trajectory can diverge. Result on Qwen3.5-0.8B-MTP (portable backend, the
  served blob flavor): **maxd 0.0016 / 0.0007, ZERO argmax flips over 48 forced-feed steps** on
  two prose openers. The verify is per-step correct. Plus a plain-vs-plain control (bit-identical,
  asserted 0.0), a counting free-run token-exact arm, and a leak gate. Arms `mtp-ctrl/ff/count`
  x tags `0.8b/27b/35b`; the large tiers run under `DASLLAMA_PARITY_FULL`. Per the tests/REVIEW.md
  contract, freeform text uses the forced-feed logits form, never token equality.
  **All three fixtures green** (M5, 2026-09-01): 0.8B maxd 0.0016/0.0007; 27B-MTP Q4_K_M
  0.00034/0.00023 (368 s, the planar CPU prefill dominates); 35B-A3B 0.0013/0.0005 - zero flips,
  48/48 rounds drafted, counting exact, no leaked Metal objects. The GPU twin is the SERVED image
  flavor (`load_model_cached` under metal mode): the direct owned-load transform declines on the
  27B (an offset misses the GPU bind alignment the image builder pads) - a test-route limitation,
  not a serving one.
- **`test_metal_decode_parity.das` arm `batch-ff`** (new) - the batch rail on REAL text: GPU
  single-step vs GPU batched step on identical fed tokens, B=2 (B2 GEMV form) and B=4 (B4 form),
  32 steps x 4 prose openers on Llama-3.2-1B Q8: **maxd 0.0029 / 0.0037, zero flips.** The batch
  rail is per-step clean on near-tie text too; whole batch test green (208 s).
- **`harness/mtp_parity_probe.das`** - the ACCEPTANCE/QUALITY lens, not a correctness gate. It
  measures free-running TRAJECTORY divergence: two sessions each advance on their own path, so
  its per-position delta compounds the (negligible) per-step kernel difference with the KV/state
  the verify writes. Its earlier headline ("row0 flips 8-22%, spec text differs 98%") was that
  compounding under free-running greedy - which forks at the first near-tie given ANY nonzero
  per-step delta. That is the behaviour the repo's forced-feed policy exists to tolerate, NOT a
  defect; the corrected reading is the forced-feed 0.0016 above.

So the S0 verdict flips from the earlier alarm: **the depth-1 metal verify reproduces plain
decode per-step; there was no output-invariance bug, there was a coverage hole.** The hole is
now closed for the depth-1 rail on 0.8B and (under PARITY_FULL) 27B/35B.

Coverage map (Boris's question - what gates the batched shapes). BEFORE this arc: the batch rail
(B sessions x 1 pos) had metal stream-equality gates vs the CPU control - `test_metal_batch_decode_parity`
arms B=2/3/6, mixed, per-dtype one-step tolerance - but on ONE model (Llama-3.2-1B Q8) and
COUNTING prompts only; the same-slab B=2 VERIFY that MTP rides (1 session x 2 pos) had NO
end-to-end test on any model, any fixture (full matrix in the coverage audit). AFTER: the metal
MTP verify is gated on the three qwen MTP twins. Still owed (ledgered):
- the split-head carriers (`mtp-Qwen3.8-27B`, `mtp-gemma-4-26B-A4B` arch gemma4-assistant) gates,
  once S1/S3 make them loadable;
- CPU batch B=2/4/6/8 (only B=3 + B=1-delegation + shrink are gated today).

## S3 results (gemma-4-26B-A4B-it-Q4_K_M + mtp-...-Q8_0 drafter, M5, SpecBench-4 chat, -n 128 -r 3, debug-jit, 2026-09-02)

| depth | off t/s | on t/s | ratio | accept | per position (accepted_i / rounds) |
|---|---|---|---|---|---|
| 1 | 122.2 | 138.6 -> **145.0 fused** | 1.13x -> **1.185x** | 74.6% | p1 74.6 |
| 2 | 122.2 | 140.3 -> **144.7 fused** | 1.15x -> **1.19x** | 63.3% | p1 74.7 p2 52.0 |
| 3 | 122.2 | 96.4 | 0.79x | 51.3% | p1 69.3 p2 47.5 p3 37.1 |
| 4 | 122.1 | 103.1 (78.1 in the sweep run) | 0.84x | 45.2% | p1 72.0 p2 48.9 p3 35.7 p4 24.2 |

- Depth 2 is the peak; 3 and 4 lose because five and six verify rows cross onto the mp-8 / GEMM
  rails while acceptance halves per position. The two depth-4 runs differ by 25% (78 vs 103) at
  1% sd each - a systematic between-run effect (thermal / the sweep's fourth run), to be re-measured
  before anything is recorded.
- Round economics at depth 1, MEASURED (the round's own clocks): draft chain 1.35 ms/round,
  verify + walk + commit 10.97 ms/round, plain step 8.17 ms. The two-row verify costs 1.34x a plain
  step - the MoE EXPERT UNION (two consecutive rows select up to 16 of 128 experts per layer, the
  batched expert GEMV streams every (row, slot) plane, shared experts twice), the wall llama.cpp and
  the July data named. 1.746 tok/round at 12.3 ms vs 8.17 = 1.16x predicted, 1.13x measured. Depth 2:
  draft 2.78, verify 13.38 (1.64x a step) for 2.27 tok/round = 1.15x. So on gemma-26B the drafter is
  NOT the fat; the verify is, and it is physics unless the batched MoE GEMV dedups the expert union
  (gather the rows that share an expert, stream the plane once) - the S5 item that moves this model.
  vLLM reports 1.60x at B1 for gemma-26 (bf16, their acceptance is higher too).
  **CORRECTED by the batch-rows cost probe** (`harness/batch_rows_probe.das`, distinct sessions, ms
  per step vs the single step): gemma-26B B=2 **1.02x**, B=4 1.26x, B=8 2.13x; gemma-12B (dense)
  B=2 0.91x, B=4 1.29x, B=8 2.23x; Qwen3-30B-A3B B=2 **1.37x**, B=4 1.96x, B=8 3.20x. The expert
  union costs nothing at two rows on gemma's dense-branch MoE (its experts are 704 wide, the dense
  branch and attention dominate) and a third on the qwen MoE (the 35B-A3B carrier will pay it). So the
  gemma verify's 1.34x is NOT expert traffic: it is the round's software cost - a synchronous batch
  step (no one-deep pipeline), the CPU accept walk over 2 x 262144 logits, the landing memcpys, the
  synchronous per-draft command buffer. ~4 ms per round recoverable => depth 1 from 1.13x toward ~1.5x.
  The dedup below stays relevant for the qwen MoE carrier only. Same-slab through the verify entry
  (`--sameslab`): B=2 1.01x, B=4 1.25x, **B=5 1.81x** (the mp-8 two-tile rail: the depth-3/4 cliff).
  So the round at depth 1 = draft 1.35 + same-slab step ~10.3 + walk/commit ~0.7 ms, strictly serial,
  against a pipelined single step of 8.2 ms - the batch step's synchronous shape (no one-deep overlap,
  CPU landing of 2 x 1 MB logits) and the serial phases are the recoverable part.
  **FUSED ROUND landed (2026-09-02)**: the k drafts encode into ONE command buffer committed without a
  wait (draft i embeds bvtok[i], the winner lands in bvtok[i+1] on the device, h chains through
  post_projection on the device), the same-slab verify embeds its rows from that token buffer
  (`set_batch_same_slab_tokens`, the driver's own `enc_embed` per row), a per-row argmax
  (`enc_argmax_rows`) lands the accept walk's truths with the join. Draft chain CPU 1.35 -> 0.03 ms;
  round 12.3 -> 11.7 ms at depth 1. Token-identical to the per-step path at k = 1, 2, 4 (counting arm,
  same acceptance counts). Remaining above the GPU floor (~10.8): the join's CPU landing (nr MB of
  logits + KV writeback) and the batch driver's host prep. Note the probe's "B=2 = 1.02x" is WALL
  time: the single step's ~2 ms of host work hides inside it; GPU-wise a two-row step is ~1.2x a
  one-row step (9.8 vs 8.2 ms), and that 20% is the model's real two-row cost on this GPU. GPU envelope
  (`DASLLAMA_METAL_PIPE_DEBUG=1`) of the two-row batch step: 9.8 ms GPU + 0.4 ms gap - the step is
  GPU-bound, so the round's floor is ~9.8 (verify) + ~1.0 (draft GPU) = 10.8 ms => depth 1 ~1.32x
  once the CPU walk, the landing and the serial gaps are gone; the 2-row weight stream itself
  (9.8 vs the pipelined single's 8.2) is the physics.
  The dedup has a home: the prefill's MoE bucket rail (`pf_enc_moe_count` -> `pf_enc_moe_bucket`
  -> per-expert gathered mul_mm over the bucket rows, ARCHITECTURE_GPU_PREFILL.md#prefill-moe-buckets)
  already builds the per-expert row lists on the GPU; a <=8-row verify wants the same two bucket
  dispatches plus a GATHERED GEMV twin (stream each selected expert's plane once, the bucket's x rows
  through the fixed-B form's indirection) in place of the per-(row, slot) MoE GEMV. Expected: the
  two-row verify from 1.34x toward ~1.1x of a step, i.e. 1.13x -> ~1.4x at today's acceptance.
- **Per-task SpecBench-4 (gemma-26B, depth 2, fused round, -r 2, 2026-09-02)** - acceptance is a
  property of the TASK far more than of depth or quantization:

  | task | off | on | ratio | p1 | p2 |
  |---|---|---|---|---|---|
  | writing (qid 81) | 124.1 | 137.8 | 1.11x | 63.1% | 32.3% |
  | summarization (qid 241) | 117.3 | 140.0 | 1.19x | 69.0% | 50.0% |
  | math / GSM8K (qid 401) | 123.1 | **193.7** | **1.57x** | **97.8%** | 82.6% |
  | qa (qid 321) | 124.1 | 158.9 | 1.28x | 75.0% | 51.8% |

  Math reaches vLLM's 1.60x headline on the same model; creative writing is the floor. A SpecBench
  report is a per-task table, never one number - `lcpp_bench --mtp-ab` now prints one row per prompt
  (off/on tok/s, ratio, accepts) under the arm's summary whenever the corpus has more than one
  prompt (S6 in its honest form; the aggregate tok/s over a mixed corpus is time-weighted and reads
  1.26x where the rows read 1.11 / 1.17 / 1.56 / 1.27). Post-#3924 the bench's tune-provenance gate
  refuses an unproven cell: dev runs pass `DASLLAMA_ALLOW_UNTUNED=1`, record runs mint the sidecar.
- **mlx.fast measured on THIS M5 (Opus agent, 2026-09-02, their engine, their free-run leg, their
  tokenizer + chat template, thinking off, same four SpecBench turns, 128 tokens, B=1):** serial
  **126-130 tok/s** (llama-benchy 0.3.7 against their mlx-server independently reads 129.0; their
  shipped benchctl reads 87 because it teacher-forces one decode step per token over the worker's
  stdio). Their MTP arm does not work on this checkpoint: it is sealed behind two static switches
  (`speculationEnabled = false`, `submissionDraftDepth = 0`); armed, it crashes in
  `applyCachedDrafterRoPE` (a rank-2 vs rank-3 concat; one-line local fix), and once running the
  adaptive controller converges to depth 0 (4% slower than serial) because the envelope seals
  `verificationMode = .serialTarget` (a depth-k round = 1+k full target forwards); forced depth 1
  = 74 tok/s, depth 3 = 53. Their recorded 68.5 (M4 Max) came from a different branch. So on M5:
  their best is serial 128; ours is 122 serial (they are ~5% ahead on plain decode) and **145 with
  MTP** - the drafter is the whole lead. Logs: scratchpad/mlxfast/.
- **llama.cpp measured on this M5 (fork build b4-98c4764b6, `llama-server` `/completion` on the
  SAME pre-rendered thinking-off prompt our bench renders - `<|turn>model` + an empty
  `<|channel>thought<channel|>` - greedy, 128 tokens, fa on; gemma assistant head as
  `-md ... --spec-type draft-mtp`, the Qwen head in-file), 2026-09-02.** Both engines produce the
  identical continuation text on every prompt, so the acceptance columns compare one drafter against
  the other on one token stream. (An earlier llama-cli pass had THINKING ON on their side - its 91%
  writing acceptance was an outline, not the blog post; discarded.)

  | prompt | lcpp off | lcpp n_max=1 (accept) | lcpp n_max=2 (accept) | ours off | ours depth 1 (accept) | ours depth 2 (accept) |
  |---|---|---|---|---|---|---|
  | writing | 102.2 | 127.5 = 1.25x (65.8%) | 123.2 = 1.21x (50.4%) | 124.1 | **139.0** = 1.12x (64.9%) | 130.5 = 1.05x (47.7%) |
  | summarization (700-tok ctx) | 101.8 | **134.8** = 1.32x (78.9%) | 134.7 = 1.32x (63.4%) | 118.0 | 132.4 = 1.12x (71.6%) | 129.0 = 1.10x (59.5%) |
  | math | 103.4 | 148.7 = 1.44x (95.0%) | 169.9 = 1.64x (90.5%) | 123.9 | 157.4 = 1.27x (89.6%) | **183.4** = 1.49x (90.2%) |
  | qa | 103.3 | 133.0 = 1.29x (72.6%) | 137.6 = 1.33x (61.9%) | 124.8 | 147.2 = 1.18x (74.0%) | **149.8** = 1.21x (63.4%) |

  - Acceptance is the SAME drafter: pooled depth 1 theirs 77.1% (216/280) vs ours 74.6%; depth 2
    64.5% vs 63.3%; per position within a few points (math depth 1 is their 117-token EOS stop vs
    our forced 128 - the post-EOS tail accepts poorly). The drafter is not where any gap is.
  - Plain decode: ours +18-21% on the three short prompts, +16% on the 700-token one. MTP ON
    absolute: ours ahead on writing, math, qa at both depths (by 4-9%); behind on summarization by
    2-4%. Their step is flat across context (102.2 -> 101.8) while ours drops 118/124 = -5% at 700
    tokens, and their prefill on that prompt reads 2813 tok/s vs our ~1733 (our pp512 board row is
    3868). ONE gap, two faces: our long-context path (attention decode past ~500 keys, prefill past
    512) - ledgered as followup_general #71.
  - Round cost: their two-row verify round is 1.36x a step (13.4 ms on a 9.8 ms step), ours 1.45x
    (11.8 ms on 8.15 - draft chain 0.05 ms, the rest verify+walk+commit); three rows 1.69x vs 1.87x.
    Their k-row verify is relatively cheaper, our step is absolutely faster; the absolute round is
    ours (11.8 vs 13.4 ms). Their ratios read larger for the baseline reason only.
  - **Qwen3.6-27B-MTP (dense), raw prompt no template:** lcpp off 28.4 / n_max 1 40.5 (1.43x,
    84.1%) / n_max 3 45.6-48.9 (1.61-1.72x, 75-89%) vs ours off 26.7 / k=1 34.9 (1.31x) / k=3
    1.19x. They beat us plain (+6%) and ON (+16-40%), and their depth 3 gains where ours loses:
    their 4-row dense verify is ~1.05-1.1x a step, ours 1.3-1.8x. Ledgered as followup_general #72
    (the dense verify rows AND the 27B plain step).
- **M4 Pro (Anton's mini, 14-core, 64 GB; NOT the 40-core M4 Max mlx.fast measured on), same
  corpus, fused round, 2026-09-02:** off 59.1, depth 1 **67.3 (1.14x)**, depth 2 55.7 (0.94x).
  Per task at depth 1: writing 1.10x, summarization 1.06x, math 1.24x, qa 1.16x. Round clocks: the
  two-row verify costs 25.6 ms against a 16.9 ms step (**1.51x**, vs 1.34x on M5), three rows 40.2 ms
  (2.4x) - on the smaller GPU the second row's expert union is paid in full, so depth 2 loses there.
  Against mlx.fast's recorded M4 Max: their 35.2 -> 68.5 tok/s vs our 59.1 -> 67.3 on half the GPU.
  Rows probe on the M4 Pro (wall, distinct sessions): B=1 25.3 (the unpipelined single step; the
  bench's pipelined step is 16.9), B=2 21.7, B=4 36.9 ms. So the two-row GPU step is ~1.28x a step
  there (1.2x on M5) and the round's verify+commit at 25.6 ms carries ~4 ms of host work (join
  landing, per-group command buffers, the slower CPU) against ~1 ms on M5. The depth default is a
  per-box-class knob (M5 2, M4 Pro 1) - a box-profile field, not a runtime controller.
### #71 context sweep (M5, gemma-4-26B-A4B-it-Q4_K_M, 2026-09-02) - the prefill "gap" was the untuned dev rail

Prediction (logged above): a prefill chunking cliff past 512. Wrong on both counts - the prompt
is ONE pass (`mp = round32(npos)`, no chunking), and the cliff was not in the tree at all.

**Prefill, prompt tokens/s (llama-bench `-p N -n 0 -r 3` vs `lcpp_bench -p N -n 0 -r 3`):**

| M | llama.cpp | ours, dev rail (`DASLLAMA_ALLOW_UNTUNED=1`) | ours, m5 sidecar accepted |
|---|---|---|---|
| 35 | 721 | 506 | 611 (0.85x) |
| 256 | 2809 | 1397 | **3124 (1.11x)** |
| 512 (board row 08-30: 3868 vs their 3476) | - | 1662 | **3835** |
| 700 | 3037 | 1733 | **3965 (1.31x)** |
| 2048 | 3175 | 1858 | **4551 (1.43x)** |

The dev rail halves prefill at every M and the board's 08-30 rig exe measures 3861 on this box
today, so the tree is fine: the tune gate's staleness rule (sidecar older than the rebuilt
binary) dropped the run to "every kernel at fallback", and that fallback also serves NO
`runtime.metal_tensor` crowns - the membership that turns the Metal-4 tensor mul_mm forms on
(runtime knobs never travel with the class defaults, by design). Isolated with two temporary
manifests: `kernels` EMPTIED but `runtime` kept = **3822**; the same minus `metal_tensor` = **1664**.
The K-quant tensor twins are the money: the mint races `kq_mulmm_k4/k5/k6` at base 0.18 ms vs
tensor 0.065-0.075 ms (2.5x per dispatch). A fresh mint against the current binary (5.5 min,
one winner change, -0.7% uniform shift) puts the dev rail at 3830. `-jit -module-cache` vs the monolith: 1662 vs
1662, exonerated. `DASLLAMA_GPU_MOE_SPLIT=0` and `-t 6`: no effect (1662-1668). Decode is
untouched by the rail (GPU-bound; 124/122/118/110 on both).

**Decode at depth, tok/s (llama-bench `tg128 @ dN` vs the per-prompt plain rows of a 4-prompt
corpus at 35 / ~260 / ~810 / ~2450 tokens):**

| context | llama.cpp | ours (tuned) | ours MTP depth 1 (accept) |
|---|---|---|---|
| 35 | 99.8 | 124.0 | 142.7 (69.3%) |
| 256 | 99.5 | 122.0 | 142.5 (76.4%) |
| 700 | 98.4 | 117.8 | 129.3 (69.3%) |
| 2048 | 93.9 | 109.8 | 126.1 (82.9%) |

Ours drops 11.5% from 35 to 2048 keys, theirs 6%; ours stays ahead at every depth (+17% at 2048).
The decode attention is a 64-key chunk per threadgroup plus a combine pass (`ATTN_CHUNK_ROWS`);
the extra 5.5 points at 2048 keys is a chunk-count scaling item, not a cliff.

**What #71 becomes:** (a) the dev-rail trap is a rail fix, not a kernel item - see the ledger
rewrite; (b) pp35 at 0.85x is the small-M prefill regime (one 32-row tile, the GEMM ladder's
floor) - a real but narrow item; (c) decode attention past ~1k keys - a microbench item.

- **The Q4 target is not what caps acceptance**: the Q8_0 target at depth 1 accepts 76.5% (vs 74.6%
  on Q4_K_M; off 109.6, on 126.0 = 1.15x; verify 12.33 ms vs step 9.13 = 1.35x again). 75% at
  position 1 is the head's own agreement with the target on SpecBench chat prompts.
- **Pre-norm vs post-norm target hidden: SETTLED for post-norm** (llama.cpp's reading). Counting
  free-run, GPU drafter: post-norm k1 19/21, k2 44/55, k4 72/103 vs pre-norm 17/23, 40/57, 66/113 -
  post-norm wins at every depth. The `mtp-count-pre` lever stays as the record.
- **Qwen3.6-27B-MTP per-position curve** (same corpus, depth 4): p1 81.1% p2 57.7% p3 42.3% p4 29.2%
  (off 20.5 on 17.3 t/s = 0.84x at depth 4). vLLM (bf16, ~2k context, K=6): 93.5 / 82.9 / 71.5 /
  58.5. Ours is lower at EVERY position including p1 (no chain compounding there), so a large part
  of the gap is the Q4_K_M target + Q4_K_M head and the prompts, not the chain; the drop-off ratios
  (p2/p1 0.71 vs 0.89, p3/p2 0.73 vs 0.86) are also worse, which a worse head compounds - the chain
  is not exonerated but the earlier "0.55 at position 2" was a CONDITIONAL rate set against their
  unconditional one. A clean split needs the Q8_0 27B head (mtp-Qwen3.8-27B-Q8_0 exists for 3.8)
  or llama-speculative-simple on the same GGUF. **Q8_0 head on Qwen3.8-27B (Q4_K_M trunk), depth 4:
  p1 81.3 p2 64.3 p3 34.5 p4 22.4** (off 27.4, on 19.4 = 0.71x at depth 4) - the same p1 as the 3.6's
  Q4 head, so head quantization is not the p1 limiter either; what is left between our ~81% and
  vLLM's 93.5% is the Q4_K_M TARGET, the corpus (SpecBench chat turns vs their ~2k-context samples)
  and the acceptance definition. (The Q4_0 head is not a servable pair: with it in the image the metal
  prefill declines and the bench throws "CPU batched prefill ... on a Metal-capable build"; the Q8_0
  head is the pair.)

## Measurement day (2026-09-02, records grade: release exe on a fresh mint, stock llama.cpp 6fdd0ac)

Both boxes minted, both rig exes rebuilt (`daspkg release`, sha e58e5a3ba), llama.cpp pinned at
6fdd0ac89 (2026-08-27, `setup_lcpp_ref` worktree, stock + clean-cpu, `llama-server` added). Board
rows through `gen_bench_records --only <model> --workload llm`; the MTP rows through
`harness/mtp_ruler.das` (SpecBench-4 chat corpus, thinking off, greedy, 128 tokens, 3 reps, settle
before every arm, our exe first, our greedy continuation as the text control).

**Board rows landed (pp512 / tg128, das vs llama.cpp):**

| box | model | Metal das / lcpp stock | CPU tuned das / lcpp clean | CPU accel das / lcpp stock |
|---|---|---|---|---|
| M5 Max | gemma-4-26B-A4B Q4_K_M | 3870 / 118.1 vs 3456 / 99.7 | 193 / 55.9 vs 123 / 46.6 | 220 / 55.8 vs - |
| M4 Pro | gemma-4-26B-A4B Q4_K_M | 781 / 56.9 vs 732 / 54.2 | 319 / 57.2 vs 187 / 51.0 | 338 / 57.0 vs 180 / 49.8 |
| M4 Pro | Qwen3.8-27B Q4_K_M | 126 / 12.9 vs 126 / 11.5 | 47.1 / 11.6 vs 40.3 / 10.9 | 48.2 / 11.6 vs 40.5 / 11.0 |

(gemma-4-26B-A4B was `official = true` and on `records/m5.json` since 08-30; the M4 rows are new.)

**Ruler, gemma-4-26B-A4B, M5 Max** (`records/mtp_m5_gemma-4-26B-A4B.json`): ours off 122.7,
depth 1 145.9 (75.2%), depth 2 148.1 (62.6%); llama.cpp stock off 95.8-98.4 (llama-bench 99.7),
n_max 1 119.8-145.8, n_max 2 126.0-163.4. Text control: math and summarization continuations are
identical across engines and the acceptance counts match to the draft (math 180/201 on both);
writing and qa diverge (the stock build's greedy path differs from ours there), so those two
acceptance columns are flagged, not compared.

**Ruler, gemma-4-26B-A4B, M4 Pro** (`records/mtp_m4_gemma-4-26B-A4B.json`): ours off 59.3, depth 1
67.5 (74.6%, 1.14x), depth 2 55.7 (0.94x); llama.cpp off 51.2-53.2, n_max 1 62.1-72.4 (pooled 66.9)
- a wash at depth 1 on this GPU. Only the math continuation matches there.

**Ruler, Qwen3.8-27B + Q8_0 head, M5 Max** (`records/mtp_m5_Qwen3.8-27B.json`, exe-first, settle
180): ours off 27.6, depth 1 **33.0 (1.20x, 77.4%)**, depth 2 31.4 (1.14x; p1 83.9 p2 66.3);
llama.cpp stock off 24.6-25.8 (llama-bench 25.9), n_max 1 32.9-35.0 (1.34x), n_max 2 30.0-36.7.
Only the writing continuation matches across engines on this carrier (the Q4_K_M dequant paths
differ more than gemma's), so three of four acceptance columns are flagged. Round split (`DASLLAMA_MTP_DEBUG=time`, 1 rep, n 64): draft 3.0 ms, verify
42-43 ms on a 36 ms plain step (**1.19x**), walk+commit 0.25 ms, replay 0.3-0.4 ms, 1.72-1.80
tokens/round. Their dense two-row verify rides at ~1.05x a step, which is the whole difference at
equal acceptance (followup #72). The CPU point (both engines `-ngl 0`, our NextN depth 1) is queued.

**The M4 Pro Qwen3.8 loss, dug (2026-09-02, Boris: "i don't see the point postponing the fix").**
The round's two verify rows cost 154 ms on a 77 ms step - 2.0x to the millisecond - so the second
row bought nothing. Two findings, both fixed in the arc:
- The batched decode driver's rope-row gate asked `head_size/2` floats per row while the rows are
  built at `rope_dim/2`; every partial-rope carrier (qwen35: head 256, rope 64) declined with
  `rope_rows`, the parity fixtures SKIPPED on a decline instead of failing, and the multi-session
  batch fell to per-row steps on the M5 too (`eval_batch_` B=2 read 2.00x a step). The gate asks the
  rotated width and the batch rope-store now rotates rope_dim (its `RopeStoreBArgs.rot` carried the
  head size). What the opened gate then exposed: the batch rail has NO arm for the hybrid graph
  (deltanet layers, the 2x-wide gated Q) and served head-less Qwen3.8-27B with plain attention over
  every layer (maxd 22.5, 47/48 argmax flips) - the rope_rows decline had been hiding that too. The
  rail now graph-declines every non-std graph; the head-less Qwen3.8 fixture asserts the decline
  reason. The 31 ms "same-slab two-row step" that suggested the round should borrow the batch
  rail's verify was that garbage rail - retracted (ledger #79). The MTP round has its own verify
  path (dn tape + gated Q at nrows) and never hit any of this. The rot fix itself has no carrier on
  disk (the std-attention partial-rope arches are glm4moe-class): kernel-level parity covers the
  rope-store at rot < hs, the driver argument is covered by reading.
- The round's verify rows on K-quant planes take the small-batch twins (`enc_kq_mvb`), which the
  July lab already called ALU-bound. The GEMV lab on both boxes (3B shapes, wGB/s): on the M5 the
  two-row twin costs 0.52-0.89 of two single passes; on the M4 Pro 1.35-1.5 for k4 (k6's still
  wins 0.81-0.84). So the form is a per-box crown: `race_kq_rows` (the production wrappers, twin vs
  two passes, per format on `race_kq_planes` fixtures) crowns `kq_rows_<fmt>`, `enc_kq_site_b` takes
  the passes there, an unraced box keeps the twin. The M4 mint (both chains, before the tile):
  passes win k4 0.51 vs twin 0.83 ms, k6 0.70 vs 0.90, k2 0.40 vs 0.82, every iquant; the twin
  keeps k5 and k3. The M5 mint: the twin wins every K-quant, the passes win iq4xs / iq4nl only.
  llama.cpp's round on the M4 is 1.53x a step; our per-row floor is 2.0x, so parity there needs a
  two-row K4 kernel that amortizes on an ALU-short GPU. Built: the two-row register tile
  (`lab k4_r2c2` -> production `MetalKqMvB2K4R2`, `enc_kq_mvb2_k4r2_c`): a thread owns TWO weight
  rows and both x columns, every x float4 load feeds two rows. Lab, B=2 cost as a fraction of two
  single passes (q3b / w2_3b / cls3b): M4 Pro tile 0.85 / 0.96 / 0.74 vs the ext twin 1.36 / 1.49 /
  1.39; M5 Max tile 0.94 / 1.07 / 0.63 vs ext 0.82 / 0.89 / 0.52. A per-box form: `race_kq_k4_form`
  races the two production twins first and crowns `kq_mvb2_k4_r2` where the tile wins; the k4 rows
  race then meets the box's twin. M4 mint on the tile: form ext 0.844 vs tile 0.482 ms -> crowned;
  rows tile-twin 0.484 vs passes 0.551 -> the twin keeps k4. M4 Qwen3.8 ruler on that rig: ours 12.7
  -> 13.9 at depth 1 (1.09x; was 12.9 -> 11.5, 0.89x) vs llama.cpp 11.5 -> 13.8 - ON at parity,
  plain +10%; round 1.62x a step (was 2.06x) vs their 1.52x. Depth 2 still loses (7.9): the round
  split on the box says verify 119 -> 302 ms for the third row - three rows take the four-column
  form, which the lab puts at 1.27-1.46x of four passes on the M4. Ruled in-arc (Boris: "the golden
  nail"): `enc_kq_mvb_k4_pairs` walks the tile over the column pairs plus one single pass for an odd
  tail wherever the tile is crowned (3 = 2+1, 4 = 2+2, ...); direct parity gate at 3 / 4 / 5 columns
  with a negative control; the M4 proof (rows probe B=1..5 on gemma-12B, the Qwen3.8 round split and
  ruler at depth 2) follows. The CPU round ignores depth 2 there (#81); k5 / k6 have no tile (#80).

**Two harness lessons, both now in the ruler:**
- A heat-soaked box under-reads a memory-bound decode by 13-18% (llama.cpp Qwen3.8 off read 21-22
  straight after our runs vs 25.5 cold; our own depth-2 baseline slid 26.9 -> 23.1 back to back).
  Every arm settles (`--settle`, 180 s on the dense 27B) the way the board driver does.
- A parent process that had just run the engine in-process read the exe's speculative arm 20% low
  (27.5 -> 26.4 on the M5, 12.7 -> 11.0 on the 64 GB M4, plain arm unaffected); the same exe from a
  clean shell read 32.5. The exe measures first, before the parent loads anything.
- Also: `llama-cli` re-wraps a raw prompt as a chat turn even under `--no-jinja`; the NextN split
  head rides the shard walk only under the exact trunk basename, so a `-Q8_0` head is pinned
  through `DASLLAMA_MTP_HEAD`.
- **The CPU point found an engine bug.** The CPU NextN round's deltanet rollback grew its two
  snapshot buffers from empty with a bare resize; Qwen3.8-27B's recurrent state (48 x 32 x 128 x 192
  floats, 151 MB) tripped the 64 MB unreserved-growth guard on the first speculative step. The
  July CPU profile predates the guard, the 0.8B fixture's state is 19 MB, so nothing had tripped it.
  Fixed with its test first (`tests/test_mtp_snapshot.das`, model-free, builds that state on a bare
  session). llama.cpp's CPU MTP on the same box: off 9.4-9.6, n_max 1 8.5-9.5 tok/s - a loss at
  80-94% acceptance, the July finding on their side too.

## Predictions (logged BEFORE each measurement)

- **0.8B depth sweep (k = 1..4, `--mtp-ab --mtp-depth`, M5, logged 2026-09-01 before the run):**
  the head's 248320-row classifier is ~1/3 of a 0.8B step, so a draft costs ~0.35 step against
  ~0.88 expected extra tokens per depth level. Predicted: k=1 1.27x (measured), **k=2 the best
  cell at ~1.4x**, k=3 ~1.3x, k=4 ~1.15x. The 27B (classifier a small fraction of the trunk)
  should favour deeper; the 0.8B is the wrong model to set the default from.
  **RESULT (M5, -r 3, 8 prompts x 128):**

  | k | off t/s | on t/s | ratio | per-draft acceptance |
  |---|---|---|---|---|
  | 1 | 357.9 ± 0.6 | 431.6 ± 0.3 | 1.21x | 87.9% |
  | 2 | 356.3 ± 1.1 | 451.1 ± 1.9 | 1.27x | 80.0% |
  | 3 | 355.4 ± 1.2 | **464.6 ± 2.6** | **1.31x** | 73.8% |
  | 4 | 311.1 ± 32.6 (VOID, cv 10%) | 255.0 ± 4.0 | ~0.72x vs the settled off | 67.6% |

  Prediction MISSED on both ends: the per-draft cost is lower than modelled (k=3 still gains), and
  k=4 falls off a wall the model lacked - at nr = 5 the q8 sites run TWO B4 tiles, so every weight
  streams twice per verify (the G(M) staircase of the qwen38 memo, at our width 5). The 8-row
  single-pass form exists for K-quant (`kq_mvb8_*`) but not for q8 (`MetalGemvB24T` stops at B4):
  a q8 B8 twin is the S5 item that unlocks k=4 on q8 models. The off arm's cv 10% at k=4 is the
  sustained-load regime again (six minutes of continuous GPU load by then) - per-rep interleaving
  stays owed (S6).
- **27B-MTP depth sweep (k = 2..4, logged 2026-09-01 before the run):** calibrating from the
  measured depth-1 round (1.82 tokens/round at ~1.25x settled => a round costs ~1.46 plain steps
  at nr = 2) and ~0.22 step per extra level (a kq verify row + a draft whose head is 1/64 of the
  trunk), with per-draft acceptance decaying ~7 points per level (82 -> 75 -> 68 -> 62):
  E[tokens] 2.44 / 2.86 / 3.12 over costs 1.68 / 1.90 / 2.12 => **k=2 ~1.45x, k=3 ~1.5x, k=4
  ~1.5x, no wall** (the kq sites have the single-pass B8 form; only the small q8 dn projections
  tile). The off arms will carry the sustained-load noise.
  **RESULT (M5, -r 3):** k=2 off 23.5 ± 2.5 (VOID) / on 23.2 => ~1.03x vs the settled 22.6, per-draft
  69.1%; **k=3 off 22.6 / on 27.7 = 1.23x**, 61.3%; k=4 off 23.3 / on 17.4 = **0.74x**, 51.4%.
  Depth-1 measured 1.24x earlier. Prediction MISSED: (a) the round's cost is non-monotonic in rows
  (3 rows dearer than 4, 5 rows a wall) - the per-width kernel cliffs of the qwen38 memo, on our
  kq B2/B4/B8 ladder and the q8 dn projections' two-tile above 4; (b) the head's conditional
  acceptance beyond position 1 falls off a cliff (per-draft average 82 -> 69 -> 61 -> 51 =>
  positions 2..4 near 55/45/35%): the Qwen3.6 nextn head is a depth-1 head; chaining it starves.
  Depth as implemented buys nothing on the 27B. Next: the per-phase round timing (`DASLLAMA_MTP_DEBUG=time`),
  then S5 decides which fat to cut first; and the Qwen3.8 head's decay curve (the qwen38 track
  reached 4.7 accepted/round on it - if its head chains, the carrier decides the default depth).

- t(M): the mv family near-flat to M=4 (weights-bound), knee at 8; whole-step verify at
  k=3 within 1.3x of a plain step on 27B.
- M5 `--mtp-ab` re-baseline of the shipped depth-1 rail: dense 1.2-1.4x, 35B MoE < 1.15x.
- Depth-N on Qwen3.8-27B, k=3 at ~85% acceptance: 1.6-1.9x over plain (upstream M3 Max:
  +42% with a GEMV-bound verify; mlxfast pooled 2.69x after a week of kernel work).
- Gemma drafter first pass: 1.4-1.7x at B=1 (mlxfast measured 1.95x on M4 Max; vLLM 1.60x).

- **#71 context sweep (logged 2026-09-02 before the run).** Prefill: ours has a cliff between 512 and
  700 (3868 -> 1733 measured) that I predict is the prefill driver's chunking past its 512-token
  bucket - the 188-token remainder runs a small-M path at a fraction of the rate; so pp2048 recovers
  to 2600-3000 (four full chunks amortize the tail) rather than falling further, and pp256 sits
  near 3000. llama.cpp: pp35 ~350, pp256 ~2200, pp700 2813, pp2048 ~2900 (flat past 512). Decode at
  depth: ours 124 -> 123 (256) -> 118 (700) -> 105-110 (2048), llama.cpp 102 flat to ~99 at 2048 -
  our attention decode kernel scales with keys where theirs does not.

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

## Ruled 2026-09-01 (was "Open for Boris")

- Default depth cap for the fixed-k lever: **4**.
- The adaptive controller lands in THIS PR (fixed-k stays the fallback lever).
- N-wide: decided after S4, from where we land and what t(M) says a wider verify costs.
