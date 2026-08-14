# Vision plan — image input for dasLLAMA (v1: gemma4uv on gemma-4-12B)

## Direction

Image→text on the existing engine: the mmproj embedder produces soft tokens that splice into
the EXISTING gemma-4 decoder between `<|image>` / `<image|>`. Same rail the four audio towers
ride: mmproj GGUF load, encode-at-add_user, two-span prompt render, `eval_embd_` prefill. The
decoder is untouched except ONE net-new mechanism: non-causal attention within the image
token span (`mtmd_decode_use_non_causal` is true only for gemma vision — every audio
projector decodes causally, so no such flag exists in the tree yet).

**Slice-A recon corrected the family map** (the plan's original premise had the variants
inverted): gemma-4 has TWO vision projectors. The dense 12B/26B models use **`gemma4uv`**
("unified") — an 11-tensor, 52M-param linear EMBEDDER with no attention, no rope, no clamps:
the decoder itself does the visual processing through the non-causal span, which is why that
flag exists. The E-series uses **`gemma4v`** — a full 16-block ViT tower (the E2B mmproj on
disk carries it, alongside the gemma4a audio conformer the audio arc already runs; it is NOT
MobileNet — that's gemma3n's `gemma3nv`). v1 = gemma4uv on the 12B vehicle; the gemma4v ViT
moves to the v2 ledger with its op list intact.

Oracle = llama.cpp mtmd: `llama-mtmd-cli` end-to-end (temp 0), `llama-mtmd-debug -p preproc /
-p encode --image <file|white|black|gray>` for stage gates, `MTMD_DEBUG_EMBEDDINGS=1` for
projected soft tokens — the exact rig the qwen2-audio arc used and documented.

v1 scope: ONE family (gemma-4 dense via gemma4uv), still images only, ONE image per user
turn, local/base64 intake, CPU embedder + CPU span attention. Everything else → the v2+
ledger at the bottom.

Naming: `dasllama_vision*` — `dasllama_image.das` is the dlim model-image file, name taken.

## Settled decisions (discussion 2026-08-13)

1. **Boundary**: engine takes decoded RGB8 `(w, h, rgb : array<uint8>)`; `dasllama_vision_io`
   requires `stbimage` (`stbi_load` / `stbi_load_from_memory`, both already bound) — mirrors
   `dasllama_audio_io` / `audio`. Deploy note: server bundles gain the stbimage shared module.
2. **Resize**: ONE das implementation adopting mtmd's conventions — corner-aligned ratio
   `(src−1)/(dst−1)`, float lerp per channel, truncating u8 cast, no antialiasing. Vanilla
   enough that "port" and "our standard code" coincide; tier-0 stays bit-exact for free.
   Geometry policy ported exactly (it is the model contract, not llama.cpp parity).
3. **CPU-only v1**: embedder f32 first, quantized planes only if slice-G measurement says
   they pay (IMAGE_VERSION → 9 either way). Non-causal flag honored by the CPU attention arm;
   `attn_gpu_prefill_ready` gets a semantic guard (non-causal ⇒ not ready). Note today's
   `start_pos == 0` head-chunk restriction already keeps media chunks off the GPU attention
   arm — the guard encodes the real constraint so lifting that restriction later can't
   silently break vision. FFN/MoE GPU prefill arms are mask-agnostic and stay as they are.
4. **Server**: `image_url` accepts `data:` URIs only (decode via `stbi_load_from_memory`);
   remote URLs stay log-and-skip. `ask.das` gets `--image <path>`.
5. **Vehicle**: gemma-4-12B-it (Q4_K_M dev / Q8_0 parity, on disk with truth tsvs) + its
   bf16 mmproj fetched + `.sha`-pinned (`mmproj-gemma-4-12B-it-BF16.gguf`, 175MB, from
   ggml-org/gemma-4-12B-it-GGUF). Slice-A correction: this mmproj is `gemma4uv` (see
   Direction) — the decision's premise that 12B pairs with a gemma4v ViT was wrong, but the
   vehicle stands; v1 implements gemma4uv. The sibling check resolved on-disk: the E2B mmproj
   IS the gemma4v ViT (not MobileNet as this decision originally claimed) — it stays parked
   for the v2 gemma4v leg, already paired with the E2B decoder dasLLAMA runs.

## The pipeline (exact ops, from mtmd `models/gemma4uv.cpp` + `mtmd-image.cpp` + `clip.cpp`;
## all numbers slice-A verified against the live mmproj + oracle dumps)

0. **Decode**: stb → RGB8. Decode parity is free — mtmd vendors stb_image too.
1. **Geometry** (`calc_size_preserved_ratio`, 4-arg form → TARGET canvas size): round each
   side to the nearest multiple of `align` (min one align unit); if area > max_pixels,
   floor-align both sides ÷ β where β = √(area/max); if area < min_pixels, ceil-align × β.
   min/max_pixels = 40/280 tokens × align². For gemma4uv clip.cpp folds the merge into the
   patch: effective `patch_size = 16 × 3 = 48`, `n_merge = 1`, so `align = 48` and
   N = (w/48)×(h/48). Oracle-verified: 640×640→624×624 (169 tok), 640×480→624×480 (130
   tok), 800×480→816×480, 3000×2000→960×624, 100×100→336×336.
2. **Letterbox resize** (`img_tool::resize`, PAD_CEIL default, pad color BLACK — a slice-A
   discovery, the plan originally assumed per-side stretch): content scale =
   min(tw/sw, th/sh) as float; content size = min(ceil(side×scale), target) per side;
   decision-2 bilinear to content size (plain copy when overall size is unchanged);
   composite centered at floor((target−content)/2) on a black canvas. Oracle-verified by
   probe: gray 480×800→480×816 has BLACK rows top+bottom, mean exactly ×800/816; white
   3000×2000→960×624 content is 936×624 with 12 black cols each side (mean 0.975 =
   936/960).
3. **Normalize**: u8/255 exactly — `image_mean = 0`, `image_std = 1` (identity, verified),
   NO ×2−1 anywhere in the uv path (gray 128 → 0.501960814 in the dump).
4. **Embedder** (the whole "tower" — 11 tensors, 52M, no attention/rope/clamps):
   im2col 48×48 stride 48 → rows of 6912 (=48×48×3, interleaved RGB row-major) →
   LayerNorm(**eps 1e-5 HARDCODED** — pytorch default, NOT the meta's 1e-6) with
   `v.patch_norm.1` w+b → linear `v.patch_embd` 6912→3840 **+ bias** →
   LayerNorm(`v.patch_norm.2`) → add x/y pos lookups: `v.position_embd.weight`
   [3840, 1120, 2], table 0 = x indexed `i % n_cols`, table 1 = y indexed `i / n_cols`
   (n_cols = w/48, row-major superpatch order) → LayerNorm(`v.patch_norm.3`) →
   **weightless** rms_norm(eps 1e-6 from meta) → `mm.input_projection` 3840×3840, no bias.
   Two eps values in one graph — parity trap. Timing datum (dasprof, M1 CPU bf16,
   130 tok): ~245 ms, 95% in the 3840×3840 proj.
5. **Splice**: head + `<|image>`(id 255999) + N embeddings + `<image|>`(id 258882) + text
   tail — audio's two-span render, specials assembled by id (the tokenizer parse_special
   gap still stands). N known from geometry before the embedder runs. Oracle stream shape
   (cats, `--jinja`): `<|turn>system\n<|think|>\n<turn|>\n<|turn>user\n` + `<|image>` +
   130 embd + `<image|>` + `describe this image<turn|>\n<|turn>model\n` = 151 tokens.
   Template media slot is `<__media__>`, media-first before the user text. NOTE:
   `tokenizer.ggml.suppress_tokens = [258883, 258882]` — llama.cpp never SAMPLES the end
   markers; check dasLLAMA's sampler honors this before tier-2 (slice E).
6. **Eval**: image rows via `eval_embd_` WITH the non-causal span flag — span queries attend
   `[0, span_end)` (uniform bound), text stays causal. Decoder positions sequential — no
   mrope, that's the qwen family. The DECODER does all visual reasoning — there is no ViT;
   this is why the non-causal span is load-bearing for gemma4uv.

## Integration shape (das side)

- `dasllama_vision.das`: `VisionImage` (w, h, rgb u8), geometry calc, letterbox
  (bilinear + center-pad-black), normalize.
- `dasllama_gemma4uv.das`: `Gemma4uvEmbedder` — mmproj load (GGUFMeta reuse), f32 planes,
  forward (im2col → LN → GEMM+bias → LN → pos adds → LN → rms → proj). No block loop at
  all — the reuse-vs-own-loop question from the original plan dissolved with the
  architecture correction; there are no attention blocks to loop over.
- `dasllama_vision_io.das`: `require stbimage` — file/bytes → VisionImage; debug PNG dump of
  the post-resize image (`stb_image_write`), env-gated — the human smoke test.
- `dasllama_chat.das`: `create_chat_(model, vembedder)` (proj_dim width panic at create, not
  respond), `add_user_image_` (geometry+letterbox+embedder encode NOW → rows accumulator),
  `render_prompt_image` two-span render; gemma-4 template arm gets image_pre/image_post =
  `<|image>` / `<image|>`. ONE image per turn in v1 — second `add_user_image_` panics.
- `dasllama_blocks.das` / `dasllama_attn_prefill.das`: non-causal span flag through
  `forward_prefill_embd` → prefill body; CPU arm uniform bound; the readiness guard.
- `dasllama_env.das`: `DASLLAMA_IMAGE_MIN_TOKENS` / `DASLLAMA_IMAGE_MAX_TOKENS` (40/280
  defaults), the dump knob.
- `dasllama_image.das`: IMAGE_VERSION → 9 when embedder planes enter the dlim (slice G).
- Server: `openai_server.das` image_url data-URI arm; `ask.das --image`.
- Tests: `test_vision.das` (tier-0, in-repo, procedural fixtures incl. NON-SQUARE),
  `test_gemma4uv.das` (tier-1/2, DASLLAMA_MODELS_DIR-gated skip-as-PASS), tier-3 caption
  checks alongside.

## Review contract — CODEREVIEW.md entries land in the same commit as what they govern

Not end-loaded into the docs slice: each slice that creates a file, an oracle convention, or
an output rule lands its `CODEREVIEW.md` entry in the SAME commit, written to the checklist's
opening contract (diff-checkable, one short paragraph, no numbers, no history) and subject to
the per-PR checklist audit like everything else. Expected entries:

- **Placement** (slice B/C, with the files): a `### Vision` block mirroring `### Audio and
  ASR` — `dasllama_vision.das` (geometry, letterbox, normalize — the only preproc home),
  `dasllama_gemma4uv.das` (the embedder; per-family files follow the audio tower pattern),
  `dasllama_vision_io.das` (image decode to RGB8; the only file that talks to stbimage).
- **Oracles** (slice A/B, with the rig): where vision fixtures and oracle dumps live and the
  split — procedural fixtures + tier-0 dumps in-repo, real images + tier-1/2 dumps in the
  models dir, `.sha`-pinned, fetched never generated; every oracle dump records the exact
  `llama-mtmd-debug` / `llama-mtmd-cli` invocation that minted it, so regeneration is a
  command not an archaeology dig; model-gated tests skip-as-PASS explicitly.
- **Text output / previewing** (slice C/E, with the tests): the vision extension of the
  existing "logs decoded text for both sides" rule — a caption test logs the full decoded
  caption, not just the contains-verdict; an embedding-parity red names the fixture and logs
  maxdiff/cosine; every image a test feeds the embedder is previewable via the preproc dump
  knob, so a red never requires adding instrumentation before a human can see what the
  model saw.

## Oracle strategy — rig MINTED at slice A

Reference dumps live in `~/Work/llama.cpp/models/gemma4-vision-oracle/` — `mint.sh` there
regenerates everything (each .log keeps its exact invocation on line 1 and the WHOLE tool
output). The rig needed a LOCAL llama.cpp patch (committed in that tree, dasprof-style):
`--keep <ny>` = non-square fixture height in llama-mtmd-debug; a real pixel dump in
`mtmd_debug_preprocess_image` (fnv1a64 over the f32le buffer + per-channel mean + 5 probes;
full per-pixel via `MTMD_DEBUG_PIXELS=1`, regenerate-on-demand only); per-token
mean/v0..v3 lines in the `MTMD_DEBUG_EMBEDDINGS` dump (global stats are permutation-
invariant — a transposed patch grid is only visible per-token).

- **Tier 0, preproc** (minted: gray/white/black/cb 640, cb 800×480, gray 480×800, cb 100
  upscale, white 3000×2000 downscale): das letterbox output must match the dump's fnv1a64
  bit-exactly (same conventions by construction); probes + means localize a red. Distilled
  in-repo fixtures + expected hashes ride `tests/`; runs in CI with no models.
- **Tier 1, embedder** (minted: `-p encode` on gray/rainbow 480, rainbow 672×336, cb/red/
  green/blue 336 — pre-resized [0,1] floats, preproc skipped): f32 maxdiff/cosine gates on
  the per-token dump lines. Models-gated. `--no-mmproj-offload` everywhere — CPU vs CPU.
- **Tier 2, pipeline** (minted: `cli.cats.log`): `llama-mtmd-cli --temp 0 --jinja` (the
  template REQUIRES --jinja — aborts without it) on the cats fixture
  (`coco-val2017-000000039769.jpg`, sha-pinned, 640×480→130 tokens); greedy ids under the
  `DASLLAMA_IDS:` marker; prompt stream shape pinned in The-pipeline §5.
- **Tier 3, semantic**: greedy caption contains-checks on real photos — the cats + a few
  CC0 shots (text/OCR, chart, portrait, one high-res) fetched at slice E with `.sha` pins,
  fetch-never-generate. Note the 12B-it thinks first — captions sit after the reasoning;
  contains-checks must scan the full output.
- **Negative controls**: gemma-4 text-only truth tsvs unchanged; audio chat unregressed
  (shared seams: `eval_embd_`, chat splice, prefill body).
- Audio-arc traps confirmed live here: mtmd logs are LOG_INF — invisible without
  `--verbose` (bit at slice A again); the image debug path is clean (no SIGBUS analog).

## Slices (commit ladder, single PR — each lands its Review-contract entries with it)

- **A. Oracle rig** (DONE 2026-08-14): mmproj fetched + pinned; sibling check resolved the
  inverted family map (this plan corrected in place); local llama.cpp debug patch; 16
  reference dumps minted via `mint.sh`; meta facts + token ids + stream shape pinned above;
  oracle rules into CODEREVIEW.md.
- **B. Vision rail**: VisionImage + geometry + letterbox (bilinear + center-pad-black) +
  normalize + tier-0 tests; placement entries into CODEREVIEW.md.
- **C. Embedder**: mmproj load + f32 forward (the 11-tensor gemma4uv graph) → tier-1
  parity; embedder placement + output rules into CODEREVIEW.md.
- **D. Non-causal**: flag + CPU arm + guard; toy-config unit test proving span output
  differs from causal and matches a reference computed in-test (branch-test rule).
- **E. Splice**: chat arm + eval → tier-2 parity; `ask.das --image`; tier-3 captions.
- **F. Server**: data-URI arm + server test.
- **G. Embedder planes**: measure first — the whole embedder is 2 GEMMs (~245 ms bf16 CPU
  at 130 tok, dasprof datum); q8 planes only if the ledger says they pay, else keep f32 and
  record why. dlim caching of the planes + IMAGE_VERSION 9 either way; tier-1/2 recheck at
  the chosen dtype's tolerances.
- **H. Docs**: README/ARCHITECTURE touch, ENVIRONMENT.md regen, PERF_LEDGER tower timings,
  findings section here.

## Predictions

Originals (registered pre-recon, against the wrong architecture — the honest scoring):
1. Tier-0 bit-exact first honest attempt — STANDS (revised failure modes below).
2. f32 tower maxdiff ~1e-4 — VOID (no flash-attn tower exists; revised R2).
3. q8 tower cosine — VOID (q8 may not even be built; revised R3).
4. Transposed-grid biggest sink — PARTIALLY STANDS (pos tables still indexed col/row; but
   no rope halves to swap).
5. Tower 1–3 s f32 — VOID, and the recon already refutes its basis: the real embedder is
   ~245 ms bf16 CPU at 130 tok.
6. Cats called cats — STANDS.

Revised (registered at slice A, before any das implementation):
- R1. Tier-0 bit-exact on the first honest attempt; if not, the bug is in the LETTERBOX
  seam (content-size ceil, centering floor, pad color) or the u8 truncate — not the
  geometry rounding, which the dumps already table-drive.
- R2. f32 embedder vs oracle: maxdiff < 1e-4, cosine > 0.99999 — two GEMMs of bf16
  weights, no attention; the only float-order freedom is GEMM accumulation order. (Their
  CPU run accumulates bf16×f32; ours will run f32 planes — weight-rounding, not order,
  dominates.)
- R3. Slice G measurement says f32 planes are ALREADY fast enough (< 300 ms at 280 tok on
  M1) and q8 planes get skipped with a ledger entry.
- R4. Biggest debug sink moves DECODER-side: the non-causal span arm + splice bookkeeping
  (slice D/E), not the embedder. The embedder lands tier-1-green within one session.
- R5. Cats get called cats (greedy, contains-check after the thinking preamble).

## Out of scope — v2+ ledger

- **E-series `gemma4v` ViT** (the on-disk E2B mmproj carries it, paired with the E2B
  decoder dasLLAMA already runs — natural next family leg). The op list, from
  `models/gemma4v.cpp`, kept from the original plan: ×2−1 scale_bias in-graph → conv2d
  patch embed no-bias (patch 16, n_merge 3 pooling) → learned x/y pos tables → pre-norm
  RMS ViT blocks (16 × 768-wide, ffn 3072, 12 heads) with 2D NEOX rope θ=100 (first half
  of head dims by pos_x, second by pos_y), **V rms_norm every layer** (weightless — no
  attn_v_norm tensor), q/k weighted norms, **kq_scale = 1.0 NOT 1/√d** — the two parity
  traps; avg_pool k3 s3 → ×√n_embd pooler; optional standardize tensors; rms_norm →
  mm_input_proj with **ClippableLinear** (input/output_min/max scalars on all seven GEMM
  weights per block, clamps before AND after each GEMM). Same markers, geometry, limits,
  and non-causal decode as gemma4uv — the whole v1 rail except the embedder is reused.
- **qwen3vl / Omni**: IMROPE decoder (int3 positions + sections — text-only (p,p,p) MUST
  bit-match today's qwen35 output; write that regression test FIRST), deepstack, dynamic-res
  ViT with window attention. The Omni-30B mmproj is already on disk.
- **gemma-4-12B audio** (`gemma4ua`): the 12B mmproj carries an audio twin of the uv
  embedder (`mm.a.input_projection` 640→3840) — the same unified pattern, mel frames
  embedded straight into the decoder. A cheap leg once vision lands.
- **Metal/Vulkan**: uniform-bound non-causal prefill kernels (then delete the readiness
  guard); Metal embedder is likely pointless (2 GEMMs, ~245 ms CPU).
- Multi-image turns, remote URL fetch, video.
- (Dead claim from the original plan, for the record: nothing in gemma-4 uses MobileNet —
  that's gemma3n's `gemma3nv`.)
