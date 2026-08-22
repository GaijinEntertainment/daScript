# Qwen3-VL plan — the qwen vision legs (Omni-30B + dense Qwen3-VL + Qwen2.5-Omni)

## Direction

The last big image leg: qwen vision, ALL living tower variations (Boris 2026-08-21: "we need
to support all variations of qwen image tower"). The anchor is the `qwen3vlmoe` decoder
dasLLAMA already serves for text and audio (`Qwen3-Omni-30B-A3B-Instruct-Q8_0`, arch in
`dasllama_arch_qwen3moe.das`, audio tower in `dasllama_qwen3a.das`) — finishing its image
half makes Omni-30B the flagship any-to-text carrier: text + image + audio in ONE chat turn
(`add_user_audio_` and the audio template splice already exist). The dense Qwen3-VL models
(deepstack) and Qwen2.5-Omni (window-attention ViT) complete the family.

## The tower matrix (tensor-dump-verified where a file is on disk)

| Tower | Carriers | Shape | New mechanisms | Files |
|---|---|---|---|---|
| `qwen3vl_merger`, no deepstack | Qwen3-Omni-30B-A3B | 27 blk, 1152w, patch 16, LayerNorm, plain GELU FFN, fused qkv, learned pos (48×48, bilinear-resized) | vision-mrope; decoder IMROPE | on disk ✓ |
| `qwen3vl_merger` + deepstack | Qwen3-VL dense 2B/4B/8B/32B, VL-MoE 30B/235B | same graph + `v.deepstack.{8,16,24}.{norm,fc1,fc2}` mergers | + deepstack: tower emits (1+3)×proj-dim wide rows; decoder adds slice il+1 after layer il<3 (image rows only) | 4B+8B Q8 + F16 mmprojs FETCHED this arc |
| `qwen2.5o` (= qwen25vl) | Qwen2.5-Omni 3B/7B, Qwen2.5-VL | 32 blk, patch 14, RMSNorm, gated-silu FFN, separate q/k/v, NO learned pos, **window attention** (n_wa_pattern 8) | window attn in the tower; decoder MROPE (non-interleaved sections, `qwen2vl` arch) | mmproj on disk ✓ (`mmproj-omni-3b-f32`); 3B Q8 decoder + official f16 mmproj FETCHED this arc |
| `qwen2vl` legacy | Qwen2-VL | superseded by 2.5 | — | SKIP — no carrier value |

Two genuinely new mechanisms, both rope-shaped:

1. **Decoder IMROPE** (`GGML_ROPE_TYPE_IMROPE`, sections `[24,20,20,0]` from
   `qwen3vlmoe.rope.dimension_sections`, interleaved t,t,y,x in NEOX ordering): image rows
   rope with per-row int positions (t=pos₀, x=pos₀+i%nx, y=pos₀+i/nx, z unused); text rows
   use equal positions in every section, which collapses IMROPE to the standard NEOX rope —
   this is why today's text/audio serving is already correct, and why the text-only
   bit-match regression is writable BEFORE any new code.
2. **Vision-mrope in the ViT** (`GGML_ROPE_TYPE_VISION`, sections d_head/4 = 18×4,
   n_dims = d_head/2 = 36, θ=10000) — the tower's 2D rope.

**The mask is NOT new.** llama.cpp's causal mask is position-based and every image row
shares t=pos₀, so the image span is bidirectional-within-span, causal outside — exactly the
uniform-span mask the fused-image-span arc shipped (PR 3815: `Session.attn_uniform_lo/end`,
`AttnArgs.ulo/uend`, CPU per-row select + Metal span eval). Qwen never sets
`mtmd_decode_use_non_causal`; the equal-t positions ARE its non-causality. The span rail is
reused as-is; only the rope angles inside it change.

**Omni has NO deepstack.** Its mmproj carries `clip.vision.is_deepstack_layers` flags
(8/16/24) but ZERO `v.deepstack.*` tensors — converter boilerplate, the same
metadata-vs-tensor-list trap the gemma arc documented; `has_deepstack()` is tensor-driven in
clip.cpp, so the oracle graph never takes that branch for this file. Deepstack is real on the
DENSE Qwen3-VL carriers and lands as its own slice: three extra merger MLPs in the tower
(concat along the feature dim → (1+3)×proj-dim wide soft tokens) plus the decoder add — after
layer il < 3, slice il+1 of the row's wide embedding is added to the hidden state
(`src/models/qwen3vl.cpp:147`); image rows only, text rows carry zero slices, decode
untouched.

Scope: the three towers above, stills only, ONE image per user turn, CPU towers + CPU
image-span prefill first (Metal IMROPE is a scheduled slice, not a decline-forever). Ladder
order: Omni core (no new tower mask, decoder already serving) → dense deepstack (same graph
+ mergers + the decoder add + `qwen3vl` dense arch registration) → qwen2.5o (new tower shape:
window attention, RMS, gated FFN; decoder MROPE non-interleaved on the `qwen2vl` arch — this
also completes `omni-3b`, whose audio half already serves). Oracle = llama.cpp mtmd
(`llama-mtmd-cli` / the patched `llama-mtmd-debug` with the vision-oracle rig from the gemma
arc — fnv1a64 pixel dumps, per-token embedding lines, all reusable unchanged).

## Recon facts (2026-08-21, tensor dumps + source reads — slice A re-verifies via oracle)

- **mmproj** (`mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf`, 860 tensors, both towers):
  vision = 27 blocks × (ln1/ln2 + fused `attn_qkv` w+b + `attn_out` w+b + `ffn_up`/`ffn_down`
  w+b — NO gate, GELU), `v.patch_embd.weight` + `.1` [16,16,3,1152] **F32** (temporal pair),
  `v.patch_embd.bias`, `v.position_embd.weight` [1152, 2304] **F32** (48×48 grid),
  `v.post_ln` (+ NO pre_ln), merger `mm.0` [4608→4608] + `mm.2` [4608→2048] BF16, GELU
  between. Audio = the 32-block `qwen3a` conformer already served.
- **Meta**: width 1152, heads 16 (d_head 72), ffn 4304, patch 16, merge 2, image_size 768,
  image_mean = image_std = [0.5,0.5,0.5] (normalize = x·2−1, NOT gemma's identity),
  vision eps 1e-6, projector `qwen3vl_merger`, projection_dim 2048.
- **ViT graph** (`tools/mtmd/models/qwen3vl.cpp`): temporal-merge conv (still image: conv₀(img)
  + conv₁(img) — for stills this is ONE conv with pre-summed weights W₀+W₁, a load-time fold)
  → spatial-merge REORDER of the token stream before the blocks (2×2 merge partners made
  adjacent) → +patch_bias → +position embeds (48×48 table bilinearly resized to the actual
  grid, then the same merge reorder) → 27 pre-LN blocks with vision-mrope on q/k, full
  attention (no windows) → post_ln → reshape ×4 → merger MLP → 2048-wide soft tokens.
- **Decoder positions** (`mtmd.cpp: mtmd_image_tokens_get_decoder_pos` /
  `mtmd_image_tokens_get_n_pos`): image token i of an nx×ny MERGED grid gets
  (t=pos₀, x=pos₀+i%nx, y=pos₀+i/nx, z=0); the sequence position then advances by
  **max(nx,ny)**, not the token count — the Session needs a rope-position rail decoupled
  from the KV row index from this point on. Audio chunks are mrope_1d = all sections
  sequential (today's behavior, unchanged).
- **Template markers**: `<|vision_start|>` (151652) / `<|vision_end|>` (151653) around the
  soft-token span, `<|image_pad|>` = 151655 (mtmd.cpp:461); NO suppress-token list (unlike
  gemma); stream shape pinned from the tier-2 log.
- **Preproc** (slice-A oracle-verified 2026-08-22): `calc_size_preserved_ratio` 4-arg —
  round each side to the nearest multiple of align = patch×merge = 32 with round-half-AWAY
  (528→544, the discriminating case), β-scale into the [8, 4096]-token budget (×32² px;
  4000×3000 → 2336×1760) — then the SAME PAD_CEIL letterbox as gemma (content scale =
  min ratio, centered, BLACK pad). The plan's first read of `dyn_size` as "stretch, no
  letterbox" was WRONG: PAD_CEIL is the clip-model.h DEFAULT and qwen never overrides it —
  verified by the white4000x3000 probes (corners −1, mean (1752−8)/1760 = 0.990909).
  Normalize = (x−0.5)/0.5, and the oracle preproc dumps are POST-normalize f32, so tier-0
  gates compare normalized buffers (gemma's identity normalize hid this distinction). das
  reuses the gemma geometry + letterbox code wholesale; only align, budget, and normalize
  constants differ.

## Downloads (fetched 2026-08-21, sha256-verified against the HF API, .sha pins minted)

The Omni core needs nothing — decoder Q8 (31 GB) + mmproj bf16 on disk and pinned. Fetched
for the other two towers (~21 GB, `fetch_models.das` manifest entries owed in the arc):

- `Qwen/Qwen3-VL-8B-Instruct-GGUF`: `Qwen3VL-8B-Instruct-Q8_0.gguf` (8.7 GB) +
  `mmproj-Qwen3VL-8B-Instruct-F16.gguf` (1.2 GB) — the deepstack record carrier.
- `Qwen/Qwen3-VL-4B-Instruct-GGUF`: `Qwen3VL-4B-Instruct-Q8_0.gguf` (4.3 GB) +
  `mmproj-Qwen3VL-4B-Instruct-F16.gguf` (0.8 GB) — the small dev vehicle.
- `ggml-org/Qwen2.5-Omni-3B-GGUF`: `Qwen2.5-Omni-3B-Q8_0.gguf` (3.6 GB) +
  `mmproj-Qwen2.5-Omni-3B-f16.gguf` (2.6 GB) — the qwen2.5o carrier; the official f16 mmproj
  also gives the locally-converted `mmproj-omni-3b-f32` a re-fetchable provenance twin.

Minted locally at slice A: f32-widened mmproj twins (`mint_f32_mmproj.py` pattern) for honest
tier-1 gates on the bf16 Omni mmproj (the F16 mmprojs widen exactly — twins only if measured
noise demands them).

## Slices (commit ladder, single PR; each lands its REVIEW.md entries with it)

- **A. Oracle rig (DONE 2026-08-22)**: `~/Work/llama.cpp/models/qwen3vl-vision-oracle/` —
  `mint.sh` + 17 dumps (10 preproc, 7 encode vs the f32-widened twin) + `cli.cats.log`;
  the gemma dasdebug patch worked unchanged. Facts pinned:
  - Geometry oracle-verified: 640→640, 100→96, 650×487→640×480, 528→544 (round-half-AWAY,
    the discriminating case), 4000×3000→2336×1760 (β-floor clamp); letterbox PAD_CEIL
    black centered (white4000x3000 probes); dumps are POST-normalize (×2−1).
  - Encode shapes: 448→[2048,196], 640×320→[2048,200], 96→[2048,9]; per-token lines healthy.
  - ViT rope (ggml ops.cpp `ggml_mrope_cache_init`, VISION mode): full 72-dim head rotated
    as 36 NEOX pairs (i, i+36); pairs 0-17 use the y angle, 18-35 the x angle, each
    section's frequency ladder RESTARTING (indep_sects), θ = 10000^(−2/36); ViT positions
    per patch = (y, x, y, x) in the merge-reordered walk.
  - Decoder IMROPE tail quirk, port VERBATIM: sections [24,20,20,0] over 64 half-dims give
    the t,h,w interleave on dims 0-59, then 60→t, 61→e, 62→e, 63→t — e reads the 4th
    position (0 on image rows ⇒ those dims unrotated; = p on text, so the NEOX collapse
    holds).
  - Cats (640×480): 300 soft tokens, grid nx=20 ny=15, position advance 20. Stream:
    `<|im_start|>user\n` + `<|vision_start|>`(151652) + 300 rows + `<|vision_end|>`(151653)
    + text + `<|im_end|>\n<|im_start|>assistant\n` — media-first, NO system prompt, no
    suppress-token list. Greedy caption correctly describes the two cats; `DASLLAMA_IDS`
    minted.
  - Decoder hparams: 48 blocks, 128 experts top-8, GQA 4, head 128, rms eps 1e-6,
    `n_deepstack_layers = 0` explicit, freq_base 1e6, standard attention (no GDN — the
    "fused Gated Delta Net enabled" line in the log is a vacuous capability print).
  - Deepstack carrier verified (4B mmproj): mergers at layers 5/11/17, 24-block 1024-wide
    ViT, projection 2560, flags AGREE with tensors on dense files.
  Note for regeneration: never edit mint.sh while it is running (bash re-reads by offset —
  this bit once).
- **B. IMROPE table builder (DONE 2026-08-22)**: das rope is TABLE-based (angle generation
  single-sourced in `dasllama_rope`, application kernels read cos/sin rows), so IMROPE is one
  new builder — `build_rope_tabs_imrope` (per-row int4 positions + sections, the ggml sector
  walk verbatim, same float order as the siblings) — and the TEXT PATH IS UNTOUCHED: the
  planned tsv-level regression dissolved into a stronger, cheaper gate. `test_rope.das`
  gained three model-free tests: the (p,p,p,p) collapse BIT-matches `build_rope_tabs_rows`
  (prediction 1: CORRECT, first attempt), an image row matches a per-element ggml-order
  reference including the tail quirk (61/62 → e, unrotated at e=0), and an h-only
  perturbation moves exactly the h dims. Negative control run: a poisoned axis walk reds
  both gates.
- **C. Preproc (DONE 2026-08-22)**: `vision_normalize` gained mean/std defaults (0/1 —
  gemma bit-identical by IEEE; qwen passes 0.5/0.5 in clip's own `(x/255 − mean)/std` order);
  geometry and letterbox needed NO code — only constants (align 32, budget 8–4096).
  `test_vision.das`: qwen geometry table (incl. 528→544 and the 2336×1760 clamp), the
  extreme-ratio budget property, and ten post-normalize fnv1a64 gates — ALL bit-exact on the
  first run (prediction 2's convention surprise was consumed at slice A). Negative control:
  a poisoned std reds all ten.
- **D. Tower**: `dasllama_qwen3v.das` — mmproj load (W₀+W₁ fold, per-tensor plane types:
  F32 stays f32, BF16 stays halfwords — the gemma slice-G rule), merge reorder, pos-embed
  bilinear resize, 27 blocks (fused-qkv split, vision-mrope, GELU MLP), merger; tier-1
  parity vs the f32-widened twin.
- **E. Decoder image span**: per-row int positions through the CPU prefill rope arms inside
  the existing uniform-span rail; Session rope-position offset (advance by max(nx,ny), decode
  continues from the offset counter); chat arm (`<|vision_start|>` splice via
  `render_prompt_media`), `add_user_image_` routed by family, `ask --image`; tier-2 greedy
  parity + tier-3 captions; the SHOWCASE test: one turn with image + audio + text.
- **F. Server**: the `PendingReq` media rail (gemma slice F) gains the mrope position walk in
  its one-quantum prefill; page unchanged (sticky image already works).
- **G. Metal IMROPE**: positions plane + sections into the prefill rope kernels (rope_store
  family) and the span eval from 3815; decode-side scalar offset; DASLLAMA_VERSION bump;
  parity arm vs CPU. Until G lands, image turns take CPU prefill by an explicit named
  decline (loud, per followup #28's spirit — not silent).
- **H. Deepstack (dense Qwen3-VL)**: `qwen3vl` dense arch registration (qwen3 dense +
  IMROPE sections + the deepstack hparam); tower gains the three merger MLPs and the wide-row
  output; prefill adds slice il+1 to image rows after layer il < 3; 4B is the dev vehicle,
  8B the record carrier; own oracle dumps + tier-1/2/3 gates (the deepstack add is exactly
  the class a caption test alone would miss — a zeroed-slices negative control pins it).
- **I. qwen2.5o (Qwen2.5-Omni-3B)**: the window-attention ViT (32 blk, patch 14, RMSNorm,
  gated-silu FFN, rope-only positions, full-attn every 8th block per `n_wa_pattern`) +
  decoder MROPE (non-interleaved sections) on the `qwen2vl` arch; completes omni-3b to
  text + image + audio; own oracle dumps + gates.
- **J. Bench + records**: `lcpp_bench --image` rows for all new carriers (img:enc / img:pp /
  img:tg), lcpp pairs via the patched mtmd-cli, `gen_bench_records -w image` cells, board
  refresh; the gemma-3-4b CPU image row re-mint rides this slice (owed from 3815's tails).
- **K. Docs**: README support matrix, ENVIRONMENT regen if knobs appear, PERF_LEDGER entries,
  this plan's findings section, predictions scored.

## Predictions (registered before implementation, per the prediction game)

1. Slice B bit-matches on the first honest attempt — the collapse-to-NEOX identity is exact,
   so any diff is an implementation bug in the section walk, not arithmetic.
2. Tier-0 preproc goes bit-exact only after one convention surprise in the RESIZE (algo or
   rounding — the gemma arc's fmadd/roundi class), not in the geometry.
3. Oracle arithmetic (the lesson the gemma arc demanded): the stock bf16 encode dumps carry
   activation-rounding noise ~3e-2 maxdiff; against the f32-widened twin das tier-1 lands
   ≤ 1e-4. Both numbers get measured, only the twin gates.
4. The biggest debug sink is the decoder POSITION RAIL (the offset bookkeeping through
   prefill/decode/server), not the tower and not the mask.
5. CPU encode of a ~300-token still on the M1 lands 0.5–2 s bf16 (27 blocks, 543 M params —
   between gemma4uv's 54 ms linear and a whisper-large encode).
6. The showcase turn (image+audio+text, one prompt) works on the first session after slice E
   with no scheduler change.
7. Deepstack (slice H) is the CHEAPEST leg — under a session end to end, because both halves
   are adds to an already-green rail; its only red will be a slice-indexing bug caught by the
   zeroed-slices control, not a math bug.
8. qwen2.5o (slice I) costs more than deepstack but less than the Omni core; the window
   attention masks land right on the first attempt (they are index arithmetic), and the
   surprise, if any, is again in the reference's preproc arithmetic (patch-14 geometry).
