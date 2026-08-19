# Vision v2 plan — the gemma4v ViT (gemma-4 E-series) for dasLLAMA

## Direction

v1 (`vision_plan.md`, merged) delivered images end to end for gemma-4 dense through the
`gemma4uv` linear embedder. v2 ports the **E-series vision tower `gemma4v`** — a real 16-block
ViT — from the same mmproj whose audio half (`gemma4a`) already ships. Finishing it makes
**gemma-4-E2B the first model dasLLAMA serves as text + image + audio**, and it is the family
leg `PERF_LEDGER.md` named as what reopens the quantized vision lane.

Everything below the embedder is v1's and is reused unchanged: geometry, letterbox, normalize
(bit-identical to the uv path — FNV hashes of the 12B and E2B preproc dumps agree), the
two-span render, `eval_embd_span_`, the non-causal `attn_uniform_end` rail, the scheduler's
media fields, the server route, the dlim rail. The port is one new tower file, a handful of
shared tower helpers, and a vision-kind carrier so two embedders can share the eight seams
that today name `Gemma4uvEmbedder` by concrete type.

## Settled decisions (discussion 2026-08-19)

1. **One vision carrier, the ASR shape**: `enum VisionKind { none uv v }` +
   `struct VisionEmbedder { kind; uv : Gemma4uvEmbedder; v : Gemma4vTower }` + the state twin,
   one-line `if/elif` arms in the verbs, the load sniffing `clip.vision.projector_type`. The
   eight seams that name `Gemma4uvEmbedder` today move to the carrier once; the third tower
   (qwen3vl, v2+ ledger) adds an arm, not a seam. Rejected: per-seam overloads (×2 today, ×3
   next family, two "is it live" sentinels).
2. **Planes follow the file per tensor**: the 112 block GEMMs and `mm.input_projection` are
   bf16 (`PlaneU16`, `mm_bf16_b` — exact widen, f32 activations), `patch_embd` / pos tables /
   norms / clamp scalars f32. This is the reference's own format on BOTH of its backends:
   ggml-metal runs bf16 `mul_mm` natively on Apple6+/Metal3+ (activations rounded to bf16 in
   the threadgroup, f32 accumulate) and aborts without bf16; ggml-cpu rounds activations to
   bf16 too. No q8 lane until slice F measures.
3. **Attention**: f32 K/V on the CPU, `enc_attention` grows a `kq_scale` argument (it hardcodes
   1/√d; the ViT needs 1.0). The reference casts K/V to f16 under flash attention on both
   backends (an unconditional `clip.cpp` cast; ggml-metal has f32 FA kernels it never asks
   for) — a later Metal leg of ours uses f32 K/V.
4. **The oracle is CPU + the f32-widened mmproj twin + `-fa off`** — the only true-f32 arm the
   reference has: Metal's "f32" GEMM stages both operands as `half`, and FA moves the dump by
   ~0.7 %; llama.cpp's own four arms (CPU/Metal × FA on/off) spread 2.2 % on tok0 v0. The
   tier-1 gate is set against that reference on a scale-relative bar, and llama.cpp's own
   cross-backend spread is the context for what "parity" can mean for this tower.

## The file on disk (census, re-verified by running `llama-mtmd-debug` on it)

`mmproj-gemma-4-E2B-it-bf16.gguf` (987 MB, pinned in `performance/profile_common.das`): 1411
tensors — vision 658 / 167 M params / 16 blocks + `v.patch_embd [16×16×3×768] F32` +
`v.position_embd [768×10240×2] F32` + `mm.input_projection [768×1536] BF16`; audio 751 (ported).
Meta: `clip.vision.projector_type = gemma4v`, projection_dim 1536, patch 16, n_embd 768, ffn
3072, 16 blocks, 12 heads, eps 1e-6; **absent** (and consequential): `use_gelu`/`use_silu`
(⇒ `gelu_quick`), `projector.scale_factor`, min/max pixels, rope keys. Per block, 41 tensors:
`ln1 ln2 attn_post_norm ffn_post_norm [768]`, `attn_q_norm attn_k_norm [64]`, seven BF16 GEMMs
(`attn_q/k/v/out [768²]`, `ffn_gate/up [768×3072]`, `ffn_down [3072×768]`) each with four F32
scalars `.input_min/.input_max/.output_min/.output_max` (ClippableLinear). **Absent on E2B:**
`v.std_bias/std_scale`, `pre_ln/post_ln`, `patch_bias`, `out_scale`/`ls1`/`ls2`, every linear
bias, clamp scalars on `patch_embd` and `mm.input_projection`. The decoder (E2B, 1536 wide,
PLE, 35 layers) runs today under the single `gemma4` arch registration, renders the image
markers already, and the media prefill path already carries a PLE arm.

## The pipeline (exact ops, from `tools/mtmd/models/gemma4v.cpp` + `clip.cpp`; executed-graph
## node sums for 96×96 `cb` are in the oracle rig, `-fa off`)

0. **Preprocess = v1's, verbatim**: align 48 (patch 16 × n_merge 3), min/max pixels
   40/280 × 2304, PAD_CEIL letterbox on black, align-corners bilinear (u8 truncating),
   u8/255, mean 0 / std 1. N soft tokens = (W/48)·(H/48); patch order row-major, `pos_x =
   i % n_cols`, `pos_y = i / n_cols` with `n_cols = W/16`. Hash-identical to the uv preproc.
1. **Stem**: `x = px × 2 − 1` (in-graph; v1's normalize stays x/255) → conv2d patch 16 stride
   16 NO bias = im2col (`j = c·256 + ky·16 + kx`, v1's `uv_im2col` with `p = 16`) × GEMM over
   `v.patch_embd` (F32 on disk) → `[n_patches × 768]` → `+ pos_x table row + pos_y table row`
   (two 10240-entry tables, no pos-norm).
2. **16 pre-norm RMS blocks**, eps 1e-6, RMS = `x / sqrt(mean(x²)+eps)` × w (no mean
   subtraction, no `1+w`), every GEMM clamp-in → GEMM → clamp-out with its own four scalars:
   `h = rms(x)·ln1` → q,k,v GEMMs → per-head RMS q/k norms (`attn_q_norm/attn_k_norm`, 64-wide)
   → **2D NEOX rope**: head dim 64 = first 32 by `pos_x`, second 32 by `pos_y`, θ = 100,
   `theta_j = p · 100^(−j/16)` over NEOX pairs (j, j+16) within each half → **V = rms(V)
   weightless, per head** → attention with **`kq_scale = 1.0`** (not 1/√d), full bidirectional,
   no mask → `attn_out` GEMM → `rms·attn_post_norm` → residual → `rms·ln2` → gate/up GEMMs →
   `gelu_quick(gate) · up` (`x/(1+exp(−1.702x))`) → down GEMM → `rms·ffn_post_norm` →
   residual.
3. **Pooler**: avg-pool k3 s3 over the patch grid (sum/9), row-major over the pooled grid
   `(W/48)×(H/48)`, then `× √768`.
4. **Standardize**: `(x − std_bias) · std_scale` only when both tensors exist — absent on E2B;
   implement the guard, it is a no-op here.
5. **Projection**: weightless RMS (eps 1e-6) → `mm.input_projection` 768→1536, no bias
   (the clamp map has ±FLT_MAX defaults for it — identity; omit).
6. **Splice / decode**: identical to v1 — `<|image>` + N rows + `<image|>`, media-first,
   sequential positions, the non-causal span, **no decoder-side rescale** (gemma4's `×√n_embd`
   applies to the token path only).

The three parity traps, named: `kq_scale = 1.0`; the rope halves (x first, y second) with
θ=100; V's weightless per-head RMS. The fourth is the oracle itself: CLIP's flash-attention
default casts K/V to f16 and moves the dump by ~0.7 % — every encode dump is minted with
`-fa off` on the F32-widened mmproj twin (v1's `mint_f32_mmproj.py`, re-pointed).

## Integration shape (das side)

- `dasllama_gemma4v.das` (new): `Gemma4vTower` — hparams, planes (`PlaneF` f32: patch_embd,
  pos tables, norms, clamp scalars; `PlaneU16` bf16: the 7×16 block GEMMs + `mm.input_projection`
  — the file's own bytes, `mm_bf16_b`), per-block offset table as a POD plane (`G4aLayerPlane`
  pattern), `Gemma4vState` scratch; `stage_gemma4v_tower` / `load_gemma4v_tower` (sniff
  `clip.vision.projector_type == gemma4v`, required-tensor panics, clamp scalars presence-gated
  like `g4a_read_clamp`), `gemma4v_encode(t, s, planes, w, h, out) : npos` with the same
  contract as `gemma4uv_encode` (`[npos × proj_dim]`), a `register_gemma4v_gpu` hook slot.
- Shared tower helpers, hoisted or added (behavior-pinned by the tests of the families that
  already use them): `Clamp` + `mm_clamped` out of `dasllama_gemma4a.das` (private today) into
  the shared tower-helper home; `enc_attention` gains a `kq_scale` argument (today hardcodes
  1/√d); a 2-axis rope table builder (`build_rope_tabs_rows` twice, one per half) + a
  half-split NEOX applier; `avg_pool2d_k3s3_rows`; per-head RMS with a 64-wide weight and the
  weightless form (the ones-plane idiom already exists).
- **The vision-kind carrier** (the structural decision): `enum VisionKind { none uv v }` +
  `struct VisionEmbedder { kind; uv : Gemma4uvEmbedder; v : Gemma4vTower }` + `VisionState`
  twin, one-line `if/elif` arms in the verbs (`load_vision_embedder(path)` sniffs the projector
  type; `encode_image_`; `proj_dim`; finalize; serialize) — the ASR side's deliberate shape
  (`AsrKind` + union, `history/dasLLAMA/INVENTORY.md`). The eight seams move from
  `Gemma4uvEmbedder` to `VisionEmbedder`: `dasllama_chat.das` (`ChatSession.embedder`,
  `create_chat_`, `add_user_image_`), the facade (`create_chat`, `encode_image`),
  `openai_server.das` (slot + vision worker + `set_model_vision`), `ask.das --image-mmproj`,
  `lcpp_bench.das --image`, `dasllama_metal_tower.das` (the uv GPU hook keeps its uv type; the
  carrier dispatches), the tutorial. The `proj_dim == 0` "no vision arm" sentinel becomes
  `kind == none`.
- dlim: one tag `gemma4v`, `IMAGE_VERSION` 9 → 10, staging under the 1 GiB cap (vision planes
  ≈ 340 MB bf16 + f32 small planes); a second dlim off the same mmproj beside the two gemma4a
  ones — the file is two towers, the tags say which.
- Env: the 40/280 token bounds are the same for gemma4v (same `patch_area`) — `VisionEnv`
  stays one global, the doc wording drops "the gemma-4 model default" for "gemma-4 (uv and v)".
- Tests: `test_gemma4v.das` tier-1 (oracle dumps, exact-value fixtures, maxdiff logged green
  and red, `tower_parity_deep` for any GPU rung later), `test_vision_chat_e2b.das` tier-2
  (splice shape with the E2B Q8_0 decoder — under the 6 GiB tier, so it runs without
  `PARITY_FULL`) + tier-3 caption, the image-suite arm `gemma4v` in `test_model_image.das`
  (staged-vs-cold-vs-warm element-exact), unit tests for every new shared helper (model-free,
  in the `model-free` suite list), `gemma4e` family tag on every E2B-loading block.
- Measurement: the E2B image turn joins `lcpp_bench --image` (the profiling app — never a
  separate bench); PERF_LEDGER entry with `img:enc`/`img:pp`/`img:tg`; the q8 lane and the
  Metal tower leg are decided by that number, not assumed.

## Review contract — REVIEW.md entries land with what they govern

`REVIEW_VISION.md` gains: the vision-kind carrier is the only place a vision tower type is
named outside its own file (the ASR union rule, vision twin); a vision tower's clamps are read
from the file's sidecar scalars, never hardcoded; every encode oracle is minted `-fa off` on
the f32 twin. `REVIEW.md` placement: `dasllama_gemma4v.das` joins the vision routing line;
shared tower helpers have one home. `tests/REVIEW.md` already binds the oracle/fixture/eyeball
rules.

## Oracle strategy — rig extended at slice A

`models/gemma4-vision-oracle/mint_e2b.sh` (or `mint.sh` parameterized by model pair): F32 twin
of the E2B mmproj (`mint_f32_mmproj.py` re-pointed; ≈1.9 GB, widens the audio half too,
harmless), tier-0 preproc dumps reused from v1 (hash-identical, re-pin only), tier-1 encode
dumps with `-fa off --no-mmproj-offload`: `cb 96` (4 tok — the block-by-block sum ladder lives
here: `layer_inp_normed-0`, `Qcur_pos-0`, `Vcur_normed-0`, `kqv_out-0` … `layer_out-15`,
`pooled`, `projected`), `gray 336`, `cb 336`, `red/green/blue 336`, `cb 480`, `cb 672×336`
(orientation), tier-2/3 with `MTMD_DEBUG_EMBEDDINGS=1 llama-mtmd-cli` on the cats jpg.

**Minted (slice A, 2026-08-19, llama.cpp 98c4764b6):** `e2b.encode.{cb96,gray336,cb336,red336,
green336,blue336,cb480,cb672x336}.log`, `e2b.cli.cats.log`, and `e2b.cb96.ladder.txt` — the
689-node (name, op, shape, sum) ladder extracted from the cb96 dump. Pins:
- cb 96² → 4 tokens; `projected` tok 0 `v0..v3 = 0.114981 0.533945 0.437806 0.140861`,
  stats mean −0.046105 std 0.749907 sum −283.269928. (The bf16 mmproj on the same CPU
  `-fa off` arm gives `0.115705 0.530347 0.439972 0.137498` — the bf16 activation rounding
  alone moves tok 0 entries by up to 3.4e-3; the four reference arms spread ≤ 6.5e-3 on the
  visible entries, std 0.75 — that is the parity context.)
- ladder rungs (sums): `node_4` conv −79.806953 · `pos_embd` −70.089081 ·
  `layer_inp_normed-0` 2504.872803 · `Qcur_pos-0` 101.341209 · `Kcur_pos-0` 701.165649 ·
  `Vcur_normed-0` 643.355042 · `kqv_out-0` 646.843567 · `attn_out-0` 831.147278 ·
  `ffn_inp-0` 300.435028 · `ffn_geglu_quick-0` 527.586731 · `layer_out-0` −2160.166992 ·
  `layer_out-7` −22755.494141 · `layer_out-15` −37735.632812 · `pooled` −116196.156250 ·
  `projected` −283.269928.
- clamps bind at block 0 already: q GEMM −3086.16 → −1355.62 after clamp, k −4706.15 →
  −1154.08, v 7242.03 → 3905.02, up 561.56 → 568.43; gate and the projection-side clamps
  are identity on this fixture (P3's premise holds before any das code).
- rope is split by ggml as two ROPE nodes of {32,12,36}: `node_27`(pos_x) + `node_29`(pos_y)
  → CONCAT — the half-split is literal in the graph, not an interpretation.
- the projection path is `pooled` → RMS_NORM → CLAMP(identity) → MUL_MAT → CLAMP(identity):
  two no-op clamps on E2B because the scalars are absent, NOT because clamping is skipped.

## Slices (commit ladder, single PR — each lands its review-contract entries with it)

- **A. Oracle rig + pins**: E2B F32 twin, `-fa off` encode dumps at the sizes above, the
  per-node sum ladder for 96², meta/token facts pinned here; predictions below scored
  against nothing yet — they are registered before any das code.
- **B. Shared tower helpers** (each with a model-free unit test that fails without it):
  `Clamp`/`mm_clamped` hoist (gemma4a switches to the shared one — its tests pin behavior),
  `enc_attention(kq_scale)`, 2-axis rope tables + half-split NEOX, `avg_pool2d_k3s3_rows`,
  per-head weighted RMS + weightless V RMS.
- **C. The tower**: `dasllama_gemma4v.das` stage/load/forward; tier-1 against the 96² ladder
  first (stem, pos, block 0 at every named node, then the 16-block sum, pooled, projected),
  then the fixture set; the bf16/f32 plane split per tensor as the file has it.
- **D. The carrier + splice**: `VisionEmbedder` through the eight seams; E2B tier-2 (splice
  shape, markers, span length) and tier-3 (cats caption) with the Q8_0 decoder; `ask.das`
  and the server pick the kind by sniffing the mmproj.
- **E. The dlim rail**: tag, `IMAGE_VERSION` 10, image-suite arm, staging cap check,
  `model_specs` companion row, ENVIRONMENT/README/ARCHITECTURE touch.
- **F. Measure**: `lcpp_bench --image` E2B cell, PERF_LEDGER entry, the q8/Metal decision
  written down with the number.
- **G. (gated on F — F said yes)** the Metal tower leg via the `register_gemma4v_gpu` hook:
  the block loop as one command buffer (stem and tail stay CPU), three new kernels
  (`enc_clamp` reading the blob's clamp record, `enc_rope2d` on the CPU's packed tables,
  `enc_geglu_quick` in the f16-table form), each unit-gated with a negative control; the
  tier-1 GPU gate + counters + the knob-off leg. **G0 (F demanded it)** the q8 CPU lane first —
  the gemma4a/parakeet recipe (Q8_0 GEMM planes + per-row requant), its own tier-1 gate on a
  relative bar; **H.** the bug-fix round + docs.

## Predictions (registered before slice A; score at each slice)

- P1 (tier-1 fidelity): against the f32-twin `-fa off` oracle, the 96² per-token mean/v0..v3
  maxdiff lands in **3e-4 … 3e-3** — the uv gate (2e-4) does NOT hold across 16 layers of
  bf16-weight GEMMs with summation-order drift; the gate is set from the measurement on a
  scale-relative bar, not copied from uv.
  **Scored at slice C:** measured 1.9e-4 (96² cb) … 9.4e-4 (blue 336²) across the eight
  fixtures — the band held except the 96² floor (1.9e-4 sits just under 3e-4; the uv 2e-4 would
  have held there and nowhere else). Bar set at 2e-4 + 4e-3·rms (≈3.2e-3): 3.4× headroom.
- P2 (cost): CPU encode of 624×480 (1170 patches → 130 tokens) on the M1 Max lands in
  **0.9 … 1.8 s** (≈420 GFLOP: 16 blocks of GEMMs ≈ 350 + attention ≈ 67) — the image side
  becomes **≥ 12 %** of the turn that was 0.7 % for uv. This is the number that decides the
  Metal leg; I predict it says yes.
  **Scored at slice F:** `img:enc` 1.93 s (just above the band — the bf16 per-row dot path
  runs ≈92 GMAC/s, not the ≈200 the band assumed), **≈61 % of the turn** (pp 337 tok/s, tg
  42 tok/s on the CPU tier). Yes, loudly: the Metal leg is on, AND the q8 CPU lane the
  out-of-scope list held back "unless slice F demands it" is demanded (PERF_LEDGER entry).
- P3 (clamps bind): at least one block's input or output clamp is ACTIVE on the `cb 336`
  fixture — disabling all clamps moves tier-1 by **> 1e-3** (the bounds ±6…±90 sit where
  activations live). Negative control in das, not in mtmd.
  **Scored at slice C:** 0.73 on tok-0 v0 with every block clamp disarmed (`test_gemma4v_clamps_bind`)
  — three orders past the bar; all 112 block GEMMs carry an active clamp on E2B.
- P4 (first red): the first tier-1 red after the stem passes is in the rope halves or
  `kq_scale`, not in the norms — and the 96² node ladder localizes it to block 0 within one
  run.
  **Scored at slice C: did not happen** — tier-1 went green on the first run, all eight
  fixtures; the ladder was never consulted. The prediction over-weighted the rope/scale risk:
  with the helpers unit-tested against in-test references (slice B) the tower had no untested
  seam left to be wrong at.
- P5 (decoder): once tier-1 passes, the E2B cats caption passes tier-3 on the first try —
  the splice, span and decoder are v1's, the E2B decoder already renders the markers.
- P6 (rail): the gemma4v dlim stages in one pass under the 1 GiB cap at ≈ 0.35 GB and maps in
  0 ms; IMAGE_VERSION 10 invalidates nothing but the two vision tags.
  **Scored at slice E:** 352 MB written in 58 ms, mapped in 0 ms (both held). The version
  half was wrong-headed: `IMAGE_VERSION` is global — a bump invalidates EVERY .dlim on every
  box, not "the two vision tags" — and the rule ("bump on a change to the layout OR to what a
  loader puts in an image") does not fire here: gemma4v images are new files under a new tag,
  the gemma4uv and gemma4a loaders put exactly what they did before (the `Clamp` hoist kept
  the POD layout). **No bump.** The companion provenance needs no new row either: the E2B
  mmproj rides the gemma4a `AsrModelSpec`, the cats fixture the 12B row.

- P7 (the q8 lane, registered before its first run — slice G0): with the 112 block GEMMs as
  Q8_0 (weights Q8_0 at read, activations per-row Q8 per site, the gemma4a recipe), tier-1
  maxdiff vs the f32-twin oracle lands in **5e-3 … 3e-2** (16 layers of q8×q8 noise on
  std-0.75 tokens; the CPU gate's exact-plane lane is untouched), the cats caption floor
  holds, and the M1 CPU encode drops to **≤ 0.5 s** (the GEMMs from 1.8 s to ≈0.25 s; the
  attention core, ≈0.16 s, becomes the biggest non-GEMM bucket).
  **Scored at slice G0:** fidelity MISSED by 2× — maxdiff 3.1e-2 (96² cb) … 6.8e-2 (green
  336²), ≈9 % of token rms on the worst sampled element (bar set at 1.2e-1·rms); the caption
  floor held (the E2B caption is the exact-lane caption to the word, one adjective aside);
  the encode landed at **0.43–0.47 s** (GEMMs 238 ms, attention core 163 ms — the #2 bucket as
  predicted). The +AMX tier on the exact planes reaches the same 0.45 s with full fidelity
  (`--accel`, which the image cell now arms) — on Apple the accelerate tier is the better
  default candidate; the q8 lane is the x64 answer. q8 is the serving default on the CPU tier
  (the gemma4a tower's policy) — the fidelity/speed default is flagged for Boris to overrule.

- P8 (the Metal leg, registered before its first run — slice G): the block loop through the
  tower driver (bf16 mul_mm, f32 norms/clamps, the prefill attention trio at scale 1, three
  new elementwise kernels: clamp-from-record, rope2d, GEGLU-quick in the f16-table form) lands
  the 640×480 encode at **≤ 120 ms** on the M1 Max (the 112 f16-tile GEMMs ≈ 40 ms, the rest
  dispatch/upload/readback), tier-1 on the gemma4uv GPU rung (4e-3·rms) **holds on ≥ 4 of 5
  fixtures** — the f16 staging over 16 blocks may breach it once.
  **Scored at slice G:** 89 ms (21× over the exact CPU lane, 4.8× over q8); tier-1 GPU
  8.9e-4 … 3.7e-3 — the 4e-3·rms rung held on 4 of 5 (green 336² at 3.7e-3 breached it by
  5.8e-4), bar set at 8e-3·rms. Both halves landed. The lane policy follows the device: with
  the Metal tower armed the default load is the exact-plane lane (the driver declines q8), the
  q8 lane is the CPU default; a pin (`set_gemma4v_q8`) overrides either way.

## Out of scope (this arc)

E4B vision (mmproj not on this box — a 992 MB download; dump before claiming), multi-image
turns, the Vulkan span (followup 23's open half), the q8 tower lane unless slice F demands
it, any attention-backend change on the decoder side.
