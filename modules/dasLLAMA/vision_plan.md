# Vision plan — image input for dasLLAMA (v1: gemma4v)

## Direction

Image→text on the existing engine: the gemma4v ViT tower + projector produce soft tokens that
splice into the EXISTING gemma-4 decoder between `<|image>` / `<image|>`. Same rail the four
audio towers ride: mmproj GGUF load, encode-at-add_user, two-span prompt render, `eval_embd_`
prefill. The decoder is untouched except ONE net-new mechanism: non-causal attention within
the image token span (`mtmd_decode_use_non_causal` is true only for gemma vision — every
audio projector decodes causally, so no such flag exists in the tree yet).

Oracle = llama.cpp mtmd: `llama-mtmd-cli` end-to-end (temp 0), `llama-mtmd-debug -p preproc /
-p encode --image <file|white|black|gray>` for stage gates, `MTMD_DEBUG_EMBEDDINGS=1` for
projected soft tokens — the exact rig the qwen2-audio arc used and documented.

v1 scope: ONE family (gemma4v), still images only, ONE image per user turn, local/base64
intake, CPU tower + CPU span attention. Everything else → the v2+ ledger at the bottom.

Naming: `dasllama_vision*` — `dasllama_image.das` is the dlim model-image file, name taken.

## Settled decisions (discussion 2026-08-13)

1. **Boundary**: engine takes decoded RGB8 `(w, h, rgb : array<uint8>)`; `dasllama_vision_io`
   requires `stbimage` (`stbi_load` / `stbi_load_from_memory`, both already bound) — mirrors
   `dasllama_audio_io` / `audio`. Deploy note: server bundles gain the stbimage shared module.
2. **Resize**: ONE das implementation adopting mtmd's conventions — corner-aligned ratio
   `(src−1)/(dst−1)`, float lerp per channel, truncating u8 cast, no antialiasing. Vanilla
   enough that "port" and "our standard code" coincide; tier-0 stays bit-exact for free.
   Geometry policy ported exactly (it is the model contract, not llama.cpp parity).
3. **CPU-only v1**: tower f32 first, q8 GEMM planes as a later commit in the same PR
   (IMAGE_VERSION → 9). Non-causal flag honored by the CPU attention arm;
   `attn_gpu_prefill_ready` gets a semantic guard (non-causal ⇒ not ready). Note today's
   `start_pos == 0` head-chunk restriction already keeps media chunks off the GPU attention
   arm — the guard encodes the real constraint so lifting that restriction later can't
   silently break vision. FFN/MoE GPU prefill arms are mask-agnostic and stay as they are.
4. **Server**: `image_url` accepts `data:` URIs only (decode via `stbi_load_from_memory`);
   remote URLs stay log-and-skip. `ask.das` gets `--image <path>`.
5. **Vehicle**: gemma-4-12B-it (Q4_K_M dev / Q8_0 parity, on disk with truth tsvs) + its
   bf16 mmproj fetched + `.sha`-pinned (the on-disk E2B mmproj is the MobileNet `gemma4uv`
   tower — wrong variant). Check for a smaller gemma4v-tower sibling at fetch time; grab as
   iteration mule if one exists.

## The pipeline (exact ops, from mtmd `models/gemma4v.cpp` + `mtmd-image.cpp` + `clip.cpp`)

0. **Decode**: stb → RGB8. Decode parity is free — mtmd vendors stb_image too.
1. **Geometry** (`calc_size_preserved_ratio`, 4-arg form): round each side to the nearest
   multiple of `align = patch_size × n_merge` (min one align unit); if area > max_pixels,
   floor-align both sides ÷ β where β = √(area/max); if area < min_pixels, ceil-align × β.
   min/max_pixels = 40/280 tokens × align². n_merge = 3 (`KEY_PROJ_SCALE_FACTOR` override),
   patch_size from mmproj meta. Then the decision-2 bilinear.
2. **Normalize**: u8/255 (+ `image_mean`/`image_std` from meta — expect identity for
   gemma4v, VERIFY at slice A); the `×2−1` (`ggml_scale_bias`) is part of the tower graph.
3. **Tower**: conv2d patch embed, NO bias, stride = patch → add learned pos tables
   (`position_embeddings` row-split: x-table then y-table, indexed by patch col/row) →
   pre-norm RMS ViT blocks where every attention applies 2D NEOX rope θ=100 (first half of
   head dims by pos_x, second half by pos_y), **V gets rms_norm every layer**, and
   **kq_scale = 1.0, NOT 1/√d** — the two known parity traps.
4. **Pooler**: reshape to patch grid → avg_pool2d k=3 s=3 → flatten → × √n_embd.
5. **Standardize**: `(x − std_bias) ⊙ std_scale` (optional per-channel tensors).
6. **Embedder**: rms_norm(eps) → `mm_input_proj`. **ClippableLinear**: any weight with
   `input_min/max` + `output_min/max` scalars in the GGUF clamps activations before AND
   after its GEMM (4 scalars per weight, name-keyed at load).
7. **Splice**: head + `<|image>` + N embeddings + `<image|>` + text tail — audio's two-span
   render, specials assembled by id (the tokenizer parse_special gap still stands).
   N = (w/align)×(h/align) ∈ [40, 280], known from geometry before the tower runs.
8. **Eval**: image rows via `eval_embd_` WITH the non-causal span flag — span queries attend
   `[0, span_end)` (uniform bound), text stays causal. Decoder positions sequential — no
   mrope, that's the qwen family.

## Integration shape (das side)

- `dasllama_vision.das`: `VisionImage` (w, h, rgb u8), geometry calc, bilinear, normalize.
- `dasllama_gemma4v.das`: `Gemma4vTower` — mmproj load (GGUFMeta reuse), f32 blob (+q8
  planes at slice G), forward. The shared audio block loop assumes 1D positions and no
  per-layer V-norm — decide reuse-vs-own-loop at slice C (gemma4a's conformer went own-loop;
  precedent exists either way).
- `dasllama_vision_io.das`: `require stbimage` — file/bytes → VisionImage; debug PNG dump of
  the post-resize image (`stb_image_write`), env-gated — the human smoke test.
- `dasllama_chat.das`: `create_chat_(model, vtower)` (proj_dim width panic at create, not
  respond), `add_user_image_` (geometry+resize+tower encode NOW → rows accumulator),
  `render_prompt_image` two-span render; gemma-4 template arm gets image_pre/image_post =
  `<|image>` / `<image|>`. ONE image per turn in v1 — second `add_user_image_` panics.
- `dasllama_blocks.das` / `dasllama_attn_prefill.das`: non-causal span flag through
  `forward_prefill_embd` → prefill body; CPU arm uniform bound; the readiness guard.
- `dasllama_env.das`: `DASLLAMA_IMAGE_MIN_TOKENS` / `DASLLAMA_IMAGE_MAX_TOKENS` (40/280
  defaults), the dump knob.
- `dasllama_image.das`: IMAGE_VERSION → 9 when tower planes enter the dlim (slice G).
- Server: `openai_server.das` image_url data-URI arm; `ask.das --image`.
- Tests: `test_vision.das` (tier-0, in-repo, procedural fixtures incl. NON-SQUARE),
  `test_gemma4v.das` (tier-1/2, DASLLAMA_MODELS_DIR-gated skip-as-PASS), tier-3 caption
  checks alongside.

## Review contract — CODEREVIEW.md entries land in the same commit as what they govern

Not end-loaded into the docs slice: each slice that creates a file, an oracle convention, or
an output rule lands its `CODEREVIEW.md` entry in the SAME commit, written to the checklist's
opening contract (diff-checkable, one short paragraph, no numbers, no history) and subject to
the per-PR checklist audit like everything else. Expected entries:

- **Placement** (slice B/C, with the files): a `### Vision` block mirroring `### Audio and
  ASR` — `dasllama_vision.das` (geometry, resize, normalize — the only preproc home),
  `dasllama_gemma4v.das` (the tower; per-family files follow the audio tower pattern),
  `dasllama_vision_io.das` (image decode to RGB8; the only file that talks to stbimage).
- **Oracles** (slice A/B, with the rig): where vision fixtures and oracle dumps live and the
  split — procedural fixtures + tier-0 dumps in-repo, real images + tier-1/2 dumps in the
  models dir, `.sha`-pinned, fetched never generated; every oracle dump records the exact
  `llama-mtmd-debug` / `llama-mtmd-cli` invocation that minted it, so regeneration is a
  command not an archaeology dig; model-gated tests skip-as-PASS explicitly.
- **Text output / previewing** (slice C/E, with the tests): the vision extension of the
  existing "logs decoded text for both sides" rule — a caption test logs the full decoded
  caption, not just the contains-verdict; an embedding-parity red names the fixture and logs
  maxdiff/cosine; every image a test feeds the tower is previewable via the preproc dump
  knob, so a red never requires adding instrumentation before a human can see what the
  model saw.

## Oracle strategy

- **Tier 0, preproc**: `llama-mtmd-debug -p preproc` on procedural (`white`/`black`/`gray`)
  + fixture files → pixel-exact (same conventions by construction). Fixtures + small oracle
  dumps live in-repo; runs in CI with no models.
- **Tier 1, tower**: `-p encode` / `MTMD_DEBUG_EMBEDDINGS=1` → projected soft tokens; f32
  maxdiff/cosine gates. Models-gated.
- **Tier 2, pipeline**: `llama-mtmd-cli --temp 0` → first-step logits + token-for-token
  parity on the cats fixture.
- **Tier 3, semantic**: greedy caption contains-checks on real photos — COCO val2017
  `000000039769.jpg` (the canonical two-cats) + a few CC0 shots (text/OCR, chart, portrait,
  one high-res) — fetched into the models dir with `.sha` pins, fetch-never-generate.
- **Negative controls**: gemma-4 text-only truth tsvs unchanged; audio chat unregressed
  (shared seams: `eval_embd_`, chat splice, prefill body).
- Audio-arc traps that carry over: mtmd library logs are LOG_INF — invisible without
  `--verbose`; the local mtmd-debug SIGBUS patch was audio-path (`is_audio` drop) — confirm
  the image path is clean before trusting dumps.

## Slices (commit ladder, single PR — each lands its Review-contract entries with it)

- **A. Oracle rig**: fetch mmproj (+ sibling check); reference dumps (preproc/encode/cli) on
  all fixtures; pin from meta + mtmd logs: patch_size, n_merge, mean/std, eps, ffn_op,
  n_embd/blocks/heads, `<|image>`/`<image|>` token ids, exact prompt stream shape; oracle
  rules into CODEREVIEW.md.
- **B. Vision rail**: VisionImage + geometry + bilinear + normalize + tier-0 tests;
  placement entries into CODEREVIEW.md.
- **C. Tower**: mmproj load + f32 forward → tier-1 parity; tower placement + output rules
  into CODEREVIEW.md.
- **D. Non-causal**: flag + CPU arm + guard; toy-config unit test proving span output
  differs from causal and matches a reference computed in-test (branch-test rule).
- **E. Splice**: chat arm + eval → tier-2 parity; `ask.das --image`; tier-3 captions.
- **F. Server**: data-URI arm + server test.
- **G. q8 tower**: planes + IMAGE_VERSION 9 + tier-1/2 recheck at q8 tolerances.
- **H. Docs**: README/ARCHITECTURE touch, ENVIRONMENT.md regen, PERF_LEDGER tower timings,
  findings section here.

## Predictions (registered before implementation)

1. Tier-0 goes bit-exact on the first honest attempt; if it doesn't, the bug is the u8
   truncate-vs-round seam or the align rounding (round vs floor vs ceil misread).
2. f32 tower vs oracle: maxdiff ~1e-4, cosine > 0.9999 (their encoder runs flash-attn —
   float-order noise, same as audio's 3.1e-05 mel gate).
3. q8 tower: cosine > 0.999 vs f32 oracle, tier-3 captions unchanged.
4. Biggest debug sink: patch-grid orientation (pos_x/pos_y swap, row- vs col-major patch
   order, rope half assignment) — the transposed-grid bug class. Non-square tier-0/1
   fixtures exist specifically to catch it.
5. Tower runtime, M4 CPU, 280 tokens (~2520 patches): f32 1–3 s, q8 under 1 s. Wide bars —
   no data yet; ledger gets real numbers at slice G.
6. The cats get called cats. Anything else is our bug, not the model's.

## Out of scope — v2+ ledger

- **qwen3vl / Omni**: IMROPE decoder (int3 positions + sections — text-only (p,p,p) MUST
  bit-match today's qwen35 output; write that regression test FIRST), deepstack, dynamic-res
  ViT with window attention. The Omni-30B mmproj is already on disk.
- **Metal/Vulkan**: uniform-bound non-causal prefill kernels (then delete the readiness
  guard), Metal tower.
- E-series `gemma4uv` (MobileNetV5 CNN tower), multi-image turns, remote URL fetch, video.
