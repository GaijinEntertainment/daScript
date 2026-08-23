# Qwen3-VL plan - the qwen vision legs (Omni-30B + dense Qwen3-VL + Qwen2.5-Omni)

## Direction

The last big image leg: qwen vision, ALL living tower variations (Boris 2026-08-21: "we need
to support all variations of qwen image tower"). The anchor is the `qwen3vlmoe` decoder
dasLLAMA already serves for text and audio (`Qwen3-Omni-30B-A3B-Instruct-Q8_0`, arch in
`dasllama_arch_qwen3moe.das`, audio tower in `dasllama_qwen3a.das`) - finishing its image
half makes Omni-30B the flagship any-to-text carrier: text + image + audio in ONE chat turn
(`add_user_audio_` and the audio template splice already exist). The dense Qwen3-VL models
(deepstack) and Qwen2.5-Omni (window-attention ViT) complete the family.

## The tower matrix (tensor-dump-verified where a file is on disk)

| Tower | Carriers | Shape | New mechanisms | Files |
|---|---|---|---|---|
| `qwen3vl_merger`, no deepstack | Qwen3-Omni-30B-A3B | 27 blk, 1152w, patch 16, LayerNorm, plain GELU FFN, fused qkv, learned pos (48x48, bilinear-resized) | vision-mrope; decoder IMROPE | on disk yes |
| `qwen3vl_merger` + deepstack | Qwen3-VL dense 2B/4B/8B/32B, VL-MoE 30B/235B | same graph + `v.deepstack.{8,16,24}.{norm,fc1,fc2}` mergers | + deepstack: tower emits (1+3)xproj-dim wide rows; decoder adds slice il+1 after layer il<3 (image rows only) | 4B+8B Q8 + F16 mmprojs FETCHED this arc |
| `qwen2.5o` (= qwen25vl) | Qwen2.5-Omni 3B/7B, Qwen2.5-VL | 32 blk, patch 14, RMSNorm, gated-silu FFN, separate q/k/v, NO learned pos, **window attention** (n_wa_pattern 8) | window attn in the tower; decoder MROPE (non-interleaved sections, `qwen2vl` arch) | mmproj on disk yes (`mmproj-omni-3b-f32`); 3B Q8 decoder + official f16 mmproj FETCHED this arc |
| `qwen2vl` legacy | Qwen2-VL | superseded by 2.5 | - | SKIP - no carrier value |

Two genuinely new mechanisms, both rope-shaped:

1. **Decoder IMROPE** (`GGML_ROPE_TYPE_IMROPE`, sections `[24,20,20,0]` from
   `qwen3vlmoe.rope.dimension_sections`, interleaved t,t,y,x in NEOX ordering): image rows
   rope with per-row int positions (t=pos_0, x=pos_0+i%nx, y=pos_0+i/nx, z unused); text rows
   use equal positions in every section, which collapses IMROPE to the standard NEOX rope - 
   this is why today's text/audio serving is already correct, and why the text-only
   bit-match regression is writable BEFORE any new code.
2. **Vision-mrope in the ViT** (`GGML_ROPE_TYPE_VISION`, sections d_head/4 = 18x4,
   n_dims = d_head/2 = 36, theta=10000) - the tower's 2D rope.

**The mask is NOT new.** llama.cpp's causal mask is position-based and every image row
shares t=pos_0, so the image span is bidirectional-within-span, causal outside - exactly the
uniform-span mask the fused-image-span arc shipped (PR 3815: `Session.attn_uniform_lo/end`,
`AttnArgs.ulo/uend`, CPU per-row select + Metal span eval). Qwen never sets
`mtmd_decode_use_non_causal`; the equal-t positions ARE its non-causality. The span rail is
reused as-is; only the rope angles inside it change.

**Omni has NO deepstack.** Its mmproj carries `clip.vision.is_deepstack_layers` flags
(8/16/24) but ZERO `v.deepstack.*` tensors - converter boilerplate, the same
metadata-vs-tensor-list trap the gemma arc documented; `has_deepstack()` is tensor-driven in
clip.cpp, so the oracle graph never takes that branch for this file. Deepstack is real on the
DENSE Qwen3-VL carriers and lands as its own slice: three extra merger MLPs in the tower
(concat along the feature dim -> (1+3)xproj-dim wide soft tokens) plus the decoder add - after
layer il < 3, slice il+1 of the row's wide embedding is added to the hidden state
(`src/models/qwen3vl.cpp:147`); image rows only, text rows carry zero slices, decode
untouched.

Scope: the three towers above, stills only, ONE image per user turn, CPU towers + CPU
image-span prefill first (Metal IMROPE is a scheduled slice, not a decline-forever). Ladder
order: Omni core (no new tower mask, decoder already serving) -> dense deepstack (same graph
+ mergers + the decoder add + `qwen3vl` dense arch registration) -> qwen2.5o (new tower shape:
window attention, RMS, gated FFN; decoder MROPE non-interleaved on the `qwen2vl` arch - this
also completes `omni-3b`, whose audio half already serves). Oracle = llama.cpp mtmd
(`llama-mtmd-cli` / the patched `llama-mtmd-debug` with the vision-oracle rig from the gemma
arc - fnv1a64 pixel dumps, per-token embedding lines, all reusable unchanged).

## Recon facts (2026-08-21, tensor dumps + source reads - slice A re-verifies via oracle)

- **mmproj** (`mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf`, 860 tensors, both towers):
  vision = 27 blocks x (ln1/ln2 + fused `attn_qkv` w+b + `attn_out` w+b + `ffn_up`/`ffn_down`
  w+b - NO gate, GELU), `v.patch_embd.weight` + `.1` [16,16,3,1152] **F32** (temporal pair),
  `v.patch_embd.bias`, `v.position_embd.weight` [1152, 2304] **F32** (48x48 grid),
  `v.post_ln` (+ NO pre_ln), merger `mm.0` [4608->4608] + `mm.2` [4608->2048] BF16, GELU
  between. Audio = the 32-block `qwen3a` conformer already served.
- **Meta**: width 1152, heads 16 (d_head 72), ffn 4304, patch 16, merge 2, image_size 768,
  image_mean = image_std = [0.5,0.5,0.5] (normalize = x*2-1, NOT gemma's identity),
  vision eps 1e-6, projector `qwen3vl_merger`, projection_dim 2048.
- **ViT graph** (`tools/mtmd/models/qwen3vl.cpp`): temporal-merge conv (still image: conv_0(img)
  + conv_1(img) - for stills this is ONE conv with pre-summed weights W_0+W_1, a load-time fold)
  -> spatial-merge REORDER of the token stream before the blocks (2x2 merge partners made
  adjacent) -> +patch_bias -> +position embeds (48x48 table bilinearly resized to the actual
  grid, then the same merge reorder) -> 27 pre-LN blocks with vision-mrope on q/k, full
  attention (no windows) -> post_ln -> reshape x4 -> merger MLP -> 2048-wide soft tokens.
- **Decoder positions** (`mtmd.cpp: mtmd_image_tokens_get_decoder_pos` /
  `mtmd_image_tokens_get_n_pos`): image token i of an nxxny MERGED grid gets
  (t=pos_0, x=pos_0+i%nx, y=pos_0+i/nx, z=0); the sequence position then advances by
  **max(nx,ny)**, not the token count - the Session needs a rope-position rail decoupled
  from the KV row index from this point on. Audio chunks are mrope_1d = all sections
  sequential (today's behavior, unchanged).
- **Template markers**: `<|vision_start|>` (151652) / `<|vision_end|>` (151653) around the
  soft-token span, `<|image_pad|>` = 151655 (mtmd.cpp:461); NO suppress-token list (unlike
  gemma); stream shape pinned from the tier-2 log.
- **Preproc** (slice-A oracle-verified 2026-08-22): `calc_size_preserved_ratio` 4-arg - 
  round each side to the nearest multiple of align = patchxmerge = 32 with round-half-AWAY
  (528->544, the discriminating case), beta-scale into the [8, 4096]-token budget (x32^2 px;
  4000x3000 -> 2336x1760) - then the SAME PAD_CEIL letterbox as gemma (content scale =
  min ratio, centered, BLACK pad). The plan's first read of `dyn_size` as "stretch, no
  letterbox" was WRONG: PAD_CEIL is the clip-model.h DEFAULT and qwen never overrides it - 
  verified by the white4000x3000 probes (corners -1, mean (1752-8)/1760 = 0.990909).
  Normalize = (x-0.5)/0.5, and the oracle preproc dumps are POST-normalize f32, so tier-0
  gates compare normalized buffers (gemma's identity normalize hid this distinction). das
  reuses the gemma geometry + letterbox code wholesale; only align, budget, and normalize
  constants differ.

## Downloads (fetched 2026-08-21, sha256-verified against the HF API, .sha pins minted)

The Omni core needs nothing - decoder Q8 (31 GB) + mmproj bf16 on disk and pinned. Fetched
for the other two towers (~21 GB, `fetch_models.das` manifest entries owed in the arc):

- `Qwen/Qwen3-VL-8B-Instruct-GGUF`: `Qwen3VL-8B-Instruct-Q8_0.gguf` (8.7 GB) +
  `mmproj-Qwen3VL-8B-Instruct-F16.gguf` (1.2 GB) - the deepstack record carrier.
- `Qwen/Qwen3-VL-4B-Instruct-GGUF`: `Qwen3VL-4B-Instruct-Q8_0.gguf` (4.3 GB) +
  `mmproj-Qwen3VL-4B-Instruct-F16.gguf` (0.8 GB) - the small dev vehicle.
- `ggml-org/Qwen2.5-Omni-3B-GGUF`: `Qwen2.5-Omni-3B-Q8_0.gguf` (3.6 GB) +
  `mmproj-Qwen2.5-Omni-3B-f16.gguf` (2.6 GB) - the qwen2.5o carrier; the official f16 mmproj
  also gives the locally-converted `mmproj-omni-3b-f32` a re-fetchable provenance twin.

Minted locally at slice A: f32-widened mmproj twins (`mint_f32_mmproj.py` pattern) for honest
tier-1 gates on the bf16 Omni mmproj (the F16 mmprojs widen exactly - twins only if measured
noise demands them).

## Slices (commit ladder, single PR; each lands its REVIEW.md entries with it)

- **A. Oracle rig (DONE 2026-08-22)**: `~/Work/llama.cpp/models/qwen3vl-vision-oracle/` - 
  `mint.sh` + 17 dumps (10 preproc, 7 encode vs the f32-widened twin) + `cli.cats.log`;
  the gemma dasdebug patch worked unchanged. Facts pinned:
  - Geometry oracle-verified: 640->640, 100->96, 650x487->640x480, 528->544 (round-half-AWAY,
    the discriminating case), 4000x3000->2336x1760 (beta-floor clamp); letterbox PAD_CEIL
    black centered (white4000x3000 probes); dumps are POST-normalize (x2-1).
  - Encode shapes: 448->[2048,196], 640x320->[2048,200], 96->[2048,9]; per-token lines healthy.
  - ViT rope (ggml ops.cpp `ggml_mrope_cache_init`, VISION mode): full 72-dim head rotated
    as 36 NEOX pairs (i, i+36); pairs 0-17 use the y angle, 18-35 the x angle, each
    section's frequency ladder RESTARTING (indep_sects), theta = 10000^(-2/36); ViT positions
    per patch = (y, x, y, x) in the merge-reordered walk.
  - Decoder IMROPE tail quirk, port VERBATIM: sections [24,20,20,0] over 64 half-dims give
    the t,h,w interleave on dims 0-59, then 60->t, 61->e, 62->e, 63->t - e reads the 4th
    position (0 on image rows => those dims unrotated; = p on text, so the NEOX collapse
    holds).
  - Cats (640x480): 300 soft tokens, grid nx=20 ny=15, position advance 20. Stream:
    `<|im_start|>user\n` + `<|vision_start|>`(151652) + 300 rows + `<|vision_end|>`(151653)
    + text + `<|im_end|>\n<|im_start|>assistant\n` - media-first, NO system prompt, no
    suppress-token list. Greedy caption correctly describes the two cats; `DASLLAMA_IDS`
    minted.
  - Decoder hparams: 48 blocks, 128 experts top-8, GQA 4, head 128, rms eps 1e-6,
    `n_deepstack_layers = 0` explicit, freq_base 1e6, standard attention (no GDN - the
    "fused Gated Delta Net enabled" line in the log is a vacuous capability print).
  - Deepstack carrier verified (4B mmproj): mergers at layers 5/11/17, 24-block 1024-wide
    ViT, projection 2560, flags AGREE with tensors on dense files.
  Note for regeneration: never edit mint.sh while it is running (bash re-reads by offset - 
  this bit once).
- **B. IMROPE table builder (DONE 2026-08-22)**: das rope is TABLE-based (angle generation
  single-sourced in `dasllama_rope`, application kernels read cos/sin rows), so IMROPE is one
  new builder - `build_rope_tabs_imrope` (per-row int4 positions + sections, the ggml sector
  walk verbatim, same float order as the siblings) - and the TEXT PATH IS UNTOUCHED: the
  planned tsv-level regression dissolved into a stronger, cheaper gate. `test_rope.das`
  gained three model-free tests: the (p,p,p,p) collapse BIT-matches `build_rope_tabs_rows`
  (prediction 1: CORRECT, first attempt), an image row matches a per-element ggml-order
  reference including the tail quirk (61/62 -> e, unrotated at e=0), and an h-only
  perturbation moves exactly the h dims. Negative control run: a poisoned axis walk reds
  both gates.
- **C. Preproc (DONE 2026-08-22)**: `vision_normalize` gained mean/std defaults (0/1 - 
  gemma bit-identical by IEEE; qwen passes 0.5/0.5 in clip's own `(x/255 - mean)/std` order);
  geometry and letterbox needed NO code - only constants (align 32, budget 8-4096).
  `test_vision.das`: qwen geometry table (incl. 528->544 and the 2336x1760 clamp), the
  extreme-ratio budget property, and ten post-normalize fnv1a64 gates - ALL bit-exact on the
  first run (prediction 2's convention surprise was consumed at slice A). Negative control:
  a poisoned std reds all ten.
- **D. Tower (DONE 2026-08-22)**: `dasllama_qwen3v.das` - image-rail load (per-tensor
  bf16/f32 planes; W_0+W_1 conv fold at stage; deepstack refused by TENSOR scan), merge
  reorder, the antialiased pos-embed resize (`interpolate_grid_bilinear_aa` in tower - 
  verified BIT-identical to a NumPy port of ggml's loop on the real table), 27 blocks
  (fused-qkv, vision-mrope via `build_rope_tabs_vision` + full-head NEOX apply, scaled
  bidir attention, GELU-tanh-LUT FFN), 2x2 merger. Tier-1 (`test_qwen3v.das`): all seven
  fixtures green at maxdiff 3.7e-4-2.7e-3 on a poison-calibrated 2e-4 + 1e-2*rms bar (a
  swapped rope ladder lands at 0.71-0.85 - 300x). Two oracle lessons: (1) the qwen mint
  had omitted `-fa off` - clip's flash attention casts K/V to f16 and cost up to 1.5e-2
  on the dumps (mint.sh now carries the flag + why); (2) the residual 1e-3-grade floor is
  the f16 GELU LUT quantizing summation-order drift between das GEMMs and the
  reference's - visible here because solid-color fixtures sit at LOW token rms, the same
  floor gemma3v's higher-rms fixtures absorb inside 4e-3*rms. Prediction 3 scored: the
  <=1e-4 claim was WRONG for a LUT-activation tower (right for gemma4uv's linear embedder,
  the wrong prior here); prediction 2's "surprise in the resize" landed as the -fa
  discovery - oracle-side, as predicted by the gemma pattern.
- **E1. The decoder mrope rail (DONE 2026-08-22)**: `Config.rope_sections` (loader-read from
  `{arch}.rope.dimension_sections`, validated 2xsum == rotary span), Session
  `rope_pos_delta` (+ at EVERY rope-angle site - table and classic, decode and prefill - 
  never at KV indexing) and the transient per-quantum `mrope_pos` int4 map
  (`mrope_span_positions` in dasllama_rope, the mtmd walk verbatim, model-free-tested);
  `eval_embd_span_mrope_` rides the uniform-span rail fused AND spliced (per-slice map
  views); an mrope quantum declines the GPU prefill override loudly and is exempt from the
  CPU-prefill tripwire; the delta flows into the CPU-built rope tables, which the Metal
  DECODE consumes - post-image GPU decode ropes correctly for free. Gates (stories15M +
  stamped sections): fused == splice BIT-exact, delta = max(grid) - rows, map-reaches-math
  (0.14 logits shift vs sequential; 11x2 vs 2x11 differ), existing span gates unregressed.
  NOTE for slice F: batched-decode positions come from scheduler stream state - a post-image
  stream needs its per-stream delta plumbed there.
- **E2. Chat splice (DONE 2026-08-22)**: the vision pair on the SHARED ChatML template
  (`<|vision_start|>`/`<|vision_end|>`, the gemma3-on-gemma_chat precedent - text-only
  vocabs refuse at the image_vocab_ok gate); the qwen3v `VisionEmbedder` union arm +
  `vision_mrope_grid` (merged grid from the tower state; (0,0) on gemma families);
  ChatSession carries `image_grid`, `generate_embd_` gained the grid param routing the span
  through `eval_embd_span_mrope_` (facade twin added); the dual-arm
  `create_chat_(model, tower, embedder)` for one session serving all media. `ask --image`
  works UNCHANGED (union-routed). Tier-2/3 follow the house caption-floor convention
  (freeform token parity is banned - tests/CLAUDE.md): the Omni leg in test_vision_chat.das
  pins the stream shape (markers 151652/151653, media-first), 300 rows, grid int2(20,15),
  the caption, and `rope_pos_delta == 20 - 300` after the turn - GREEN 10/10 with every
  gemma leg unregressed; the Omni's greedy caption correctly describes the cats fixture.
  `test_omni_showcase`: ONE session - an image turn, then a text turn whose answer needs the
  image turn's history across the mrope delta. The three-modality form is BLOCKED on a
  standing gap the test surfaced: audio-in-chat rides the whisper-class `AudioTower` only - 
  the qwen3a conformer has no chat splice (`followup_general.md` #41; prediction 6 scores
  WRONG on its premise - `add_user_audio_` existed but never served this family).
- **F. Server (DONE 2026-08-22)**: `PendingReq`/`Stream` gained `media_grid : int2` ((0,0) =
  sequential; submit contract: positive grid <=> `nx*ny == n_media` && non-causal, enforced by
  named panics); the media quantum routes a grid through the new facade
  `eval_embd_span_mrope` (span = the whole call), so the session delta then covers tail
  chunks and decode. The E1 note's gap - batched-decode positions - closed by SPLITTING the
  workspace positions: `ws.positions` stays the KV index (attention counts, cache rows,
  mirror watermarks - every consumer audited), new `ws.rope_positions = n_past +
  rope_pos_delta` feeds `build_rope_table_rows`, which the Metal batched override's table
  copy consumes for free. Same delta fold in the three self-built rope-row sites: Metal spec
  pre-encode (restores its "bit-identical to forward's row" contract), vulkan resident
  single decode, vulkan resident batch decode (resident PREFILL needs nothing - it serves
  `start_pos == 0` only, where the delta is necessarily 0, and an mrope quantum declines
  every prefill override). Wire: the media worker stamps `MediaEvent.grid` from
  `vision_mrope_grid` after encode; `handle_chat_tail` -> `register_request` ->
  `PendingReq`; `/v1/stats` gained `mrope_streams` (scheduler counter) as the wire proof a
  caption alone can't give. Gates: `test_scheduler_mrope` - the scheduler stream ==
  `generate_embd`'s grid form solo AND batched 2-row against solo references, both
  mutation-controlled (grid routing knocked out -> the solo cell reds; delta dropped from the
  batch gather -> the batched cell reds); `test_scheduler_mrope_validation` - four named
  panics; the server Omni leg (`test_openai_server_vision_mrope`, PARITY_FULL) GREEN end to
  end - grid 20x15 through worker -> scheduler -> caption + `mrope_streams == 1` (the caption
  POST rides the request BUILDER at `timeout = 600`: the 30B's CPU tower encode + CPU mrope
  prefill dwarf the outbound default - slice G removes most of that wait). Two PRE-EXISTING
  reds in the PARITY_FULL-gated gemma leg (stash-controlled off this slice's diff) fixed in
  passing: the 12B's family default THINKS, so the caption sat wholly in `reasoning_content`
  with `content` honestly empty (the cell now pins `enable_thinking: false`), and the
  two-image 400 was renamed "one media clip per request" by the audio arc while the cell
  still grepped the old wording. Suite 4/4 green.
- **G. Metal IMROPE (DONE 2026-08-22)**: the plan's "positions plane + sections into the
  kernels" premise was WRONG - Metal's prefill rope (`enc_rope`) is a pure TABLE consumer
  (per-token cos/sin rows, memcpy'd from `s.rope_cos` each quantum), and E1's
  `prefill_rope_tables` already builds those rows from the mrope map. So G is a capability
  gate, not a kernel change (no MSL touched, no DASLLAMA_VERSION bump): new seat
  `register_prefill_override_mrope_tables` (the split-span seat's twin) - Metal registers it
  and serves the quantum; an unregistered override (vulkan: scalar-position angles) declines
  it to the CPU loop by name; the CPU-prefill tripwire's mrope exemption is REMOVED (a
  CPU-served mrope quantum on a Metal build is the silent-sink class again). Gates:
  prefill-parity arm `span-mrope` (Llama-3.2-1B blob twin, stamped sections, 6x4 grid over
  24 rows) - ONE GPU prefill (counter; the capability knockout reds it via the blob-only
  panic), token-exact vs the all-CPU control, and a same-backend grid-vs-sequential
  prefill-logits witness (measured 6.03 - greedy tokens TIE on the counting fixture, logits
  are the honest discriminator); tripwire suite gains the mrope cell (mutation-controlled:
  restoring the exemption reds it); span/span-fused arms + test_vision_chat 12/12 + the
  vision server suite 4/4 unregressed. NOTE: the server-test harness maps the planar flavor
  by design, so the Omni-on-Metal E2E (and its prefill win) lands with slice J's bench
  cells, which run the metal rail.
- **H. Deepstack (DONE 2026-08-22)**: the `qwen3vl` dense arch was ALREADY registered (the
  Qwen3-ASR precedent) and E1's sections read is arch-generic, so the decoder needed only
  `Config.n_deepstack` (loader: `{arch}.n_deepstack_layers`) + the slice-add rail. Design =
  llama.cpp-congruent WIDE ROWS: the tower emits (1+n_ds)*proj_dim rows (slice 0 = main
  merger, slice k+1 = tap k in encounter order), rows travel OPAQUELY through chat / worker /
  PendingReq / scheduler (zero plumbing there), and the eval seam (`forward_prefill_embd
  ds_wide`, exact-length detection in the three eval_embd entries) splits slice 0 into x_b
  and stashes the tails on `Session.ds_embd` (@scratch, `ds_active` framed like
  attn_uniform); the CPU layer loop adds slice l after layer l < n_ds to EVERY row - text
  rows carry zero tails (assembled by `embed_text_rows_strided`), which is exactly
  llama.cpp's semantics, so no row masking exists anywhere. A ds quantum DECLINES every GPU
  prefill override by name (tripwire-exempt) - the Metal per-layer-add kernel is an arc
  followup, needed before slice J's 8B Metal rows. Tower: taps collected from the TENSOR
  list (<=3), per-tap LayerNorm->fc1->GELU-LUT->fc2 over the x4-merged rows; the projector key
  falls back `clip.vision.projector_type` -> `clip.projector_type` (the dense converter
  scopes it top-level). Pairing: `vision_deepstack`/`vision_row_width` accessors + named
  refusals in create_chat_ and the server's arm_slot_vision. Oracle: `mint_4b.sh` (4 encode
  dumps on the f32-widened 4B mmproj + cli caption); the debug patch's per-token line grew
  q1/q2/q3 QUARTER-OFFSET probes because mean+v0..v3 are BLIND to a zeroed slice (measured:
  a skipped tap passed untouched; with q-probes it reds at 6.9-9.7 vs the 2e-4+4e-2*rms bar
  - the compounded 4-chain GELU-LUT floor measures 1.12e-2 on low-rms gray). Gates, all
  mutation-controlled: test_attn_span deepstack rail (zero-tail wide == narrow BIT-exact;
  tails move logits 5.8; slice-0-vs-2 depth; no stale plane - the add knockout reds it),
  test_qwen3v 4B tier-1 4/4, test_vision_chat_deepstack (small tier: wide rows through the
  chat, caption names the cats, delta 20-300, and the zeroed-slices decoder control moves
  prefill logits 10.4). The 8B record carrier smoked end-to-end via `ask --image` ("Two
  cats.", 321-token prompt). The Config field add shifts every Model dlim's meta layout - 
  the v11 fingerprint auto-refuses and re-mints (observed on the 8B/4B first loads).
- **I. qwen2.5o (DONE 2026-08-22)**: `dasllama_qwen25v.das` - the window-attention ViT (32
  RMS-normed blocks, separate biased q/k/v, gated-silu FFN whose hidden comes from the
  TENSOR dims [3420] - the metadata's feed_forward_length lies [1280]; rope-only positions;
  temporal-pair conv fold; per-channel CLIP normalize via new `vision_normalize3`). The
  window machinery: rows sort by 112 px windows of 2x2-merged groups (the same (gy,gx,sy,sx)
  merge walk as qwen3v composed under the window sort), non-full blocks run
  `attention_bidir_windows` (new tower helper: per-(window, head) units over CONTIGUOUS row
  ranges - no mask tensor), every `n_wa_pattern`-th block attends in full, and the merger
  un-sorts back to group-row-major. Decoder = the ALREADY-registered `qwen2vl` arch +
  `Config.mrope_interleaved` (new flag; qwen3vl/qwen3vlmoe configures set it TRUE - the
  qwen3vlmoe MISS would have silently flipped the Omni-30B to the wrong walk, caught in
  review) + `build_rope_tabs_mrope` (the NON-interleaved twin: contiguous section ranges,
  continuous frequency ladder). Vocab quirk: the Omni vocab spells the span markers
  `<|vision_bos|>`/`<|vision_eos|>` at the SAME ids the VL vocabs call
  `<|vision_start|>`/`<|vision_end|>` - the chat layer resolves by VOCAB with a fallback
  (mtmd hardcodes the VL spelling and silently feeds it as plain text on the Omni). Oracle:
  `mint_25o.sh` (6 preproc + 5 encode dumps + cli caption; geometry = the shared code at
  align 28, verified 518->532 round-half-away, 4000x3000->2044x1540). ORACLE LESSON: every
  stock debug fixture is window-SYMMETRIC (uniform/periodic), so an all-full-attention
  poison PASSED tier-1 untouched - the patch grew the `quad` generator (four exact-value
  quadrants), which reds the same poison at 10.7 (1300x). Gates: tier-1 5/5 first-run green
  (3.9e-4-7.6e-3 on 2e-4 + 1e-2*rms), test_rope's non-interleaved cells (collapse bit-exact,
  contiguous-axis reference, interleaved-vs-contiguous divergence), and
  test_vision_chat_qwen25o (small tier) - the 3B captions the cats ("pink couch with two
  cats... two remote controls"), grid 23x17, delta 23-391. omni-3b now serves text + image
  (audio already served standalone; audio-IN-CHAT stays followup #41).
- **J. Bench + records (PREP + MEASUREMENT DONE 2026-08-22; card decision PENDING)**:
  provenance rows + verified sha pins for all six fetched files landed (fetch_models 6/6 ok);
  the Metal ds-add landed, so the 8B Metal cells measure GPU prefill. MEASURED (Boris:
  numbers, not cards): all four models das-vs-mtmd on the M1 Max, driven per-model via the
  released `lcpp_bench --image` (no catalog); the ref pp repriced on ROWS (mtmd's n_past
  advances by the mrope grid delta, so the patched n_prefill undercounts rows ~8x on mrope
  models). Numbers in the arc summary; headline - das wins EVERY CPU cell (pp +11..16%
  dense, 6x on the 30B MoE; tg ties dense, +45% MoE) and EVERY Metal tg (+12..159%), trails
  every Metal pp (-12..-41%) and every encode except the 30B CPU pair (no Metal tower for
  the qwen ViTs = the known slice-J gap; mtmd's clip CPU encode is also 4-5x faster than the
  das CPU tower, except the 30B where mtmd CPU encode collapses to 59 s vs das 6.3 s).
  THREE bench defects found by the sweep (fixed, 13c5916cc): the image cell never passed the
  mrope grid (priced the sequential walk - captions hid it via greedy-tie saturation, the
  arc's own gate lesson, until the 30B emptied); an empty timed reply still minted numbers
  (now refuses by name); and the qwen3 Instruct/VL checkpoints DERAIL on their own template's
  think-suppress form (30B -> immediate stop, 4B -> re-emits 'assistant'; the form is in the
  GGUF template but the Instruct checkpoints never trained on it) - `--image-think` prices
  the template-default turn, which is also what the mtmd ref prices. OPEN protocol rule for
  Boris: should image cells default to template-default thinking (symmetric with mtmd) with
  think-off reserved for genuinely-thinking checkpoints (gemma-4-12B)? STILL PENDING: the
  official-card decision, the gemma-3-4b CPU image row re-mint (3815's tail).
- **K. Docs (DONE 2026-08-22, except J-dependent perf notes)**: README image bullet carries
  all three qwen towers (+ the Metal note); the three stale table claims fixed
  ("vision out of scope" on Qwen2.5-Omni, "deepstack inactive" on Qwen3-ASR/Omni-30B); the
  not-implemented list now names pixtral/minicpm-v as the remaining vision gap. daslang.io:
  the handmade RST family bullets gained the vision/omni lines (Qwen AND Gemma), the das2rst
  `VisionEmbedder` card names qwen3v + qwen25v, and the generated RST regenerates in the doc
  pipeline (the generated dir is gitignored - the source edits are the deliverable). No new
  env knobs (ENVIRONMENT untouched). Predictions 1-8 all scored in place. PERF_LEDGER
  entries ride slice J's numbers. The sec.04 vision NUMBERS on that page turned out to be the
  real staleness (the 3815 re-mint updated records/ but not the committed site merge) - 
  FIXED + GATED 2026-08-22: `gen_site_records --verify` and `test_site_records.das`
  (model-free suite) red any records commit that skips the site merge, and the live-page
  refresh rides `bbatkin/site-image-records-refresh` off master. Slice J's qwen image
  records therefore update the page automatically-on-pain-of-red.

### Arc followups (owed by this arc, after K)

- **Metal deepstack add - DONE 2026-08-22**: NO new kernel - the ds plane repacks CPU-side
  into slice-major contiguous planes, uploads once, and each layer l < n_ds encodes one more
  `enc_add` at the slice offset after the layer-out residual; capability seat
  `register_prefill_override_ds_adds` (metal registers, others decline by name); the
  CPU-prefill tripwire's ds exemption removed (+ its cell). Gate: prefill arm `span-ds` - 
  ONE GPU prefill + token parity + the add-CONTRIBUTION witness (lg(tails)-lg(zero) per
  backend: the skipped-add mutation reds it at contribution 0 where greedy tokens TIE on
  the counting fixture; clean dd = 10-13% of the 3.6 contribution, bar 0.5x).

- **qwen3a/gemma4a audio-in-chat** (= `followup_general.md` #41, pulled onto this arc): the
  chat `AudioTower` serves whisper-class projectors only - the conformer families have no
  chat splice, which blocks the one-session three-modality Omni showcase. Done = an
  audio-encoder seam the chat layer holds for either type (the `VisionEmbedder` union
  pattern), `add_user_audio_` routed by family, `test_omni_showcase` upgraded to
  image + audio + text on one session.

## Predictions (registered before implementation, per the prediction game)

1. Slice B bit-matches on the first honest attempt - the collapse-to-NEOX identity is exact,
   so any diff is an implementation bug in the section walk, not arithmetic.
2. Tier-0 preproc goes bit-exact only after one convention surprise in the RESIZE (algo or
   rounding - the gemma arc's fmadd/roundi class), not in the geometry.
3. Oracle arithmetic (the lesson the gemma arc demanded): the stock bf16 encode dumps carry
   activation-rounding noise ~3e-2 maxdiff; against the f32-widened twin das tier-1 lands
   <= 1e-4. Both numbers get measured, only the twin gates.
4. The biggest debug sink is the decoder POSITION RAIL (the offset bookkeeping through
   prefill/decode/server), not the tower and not the mask.
   **SCORED (F/G): HALF.** The rail itself landed cleanly (E1's design paid off - F was
   plumbing, not debugging), but the CLASS was right: every follow-on gap was positional
   bookkeeping (the batched-decode delta, the three delta-less GPU rope-row sites, the
   qwen3vlmoe interleave-flag miss) - none was tower or mask. What made it cheap instead of
   a sink was the E1 law "delta at every rope-angle site, never KV" turning each gap into a
   grep, plus mutation-controlled gates.
5. CPU encode of a ~300-token still on the M1 lands 0.5-2 s bf16 (27 blocks, 543 M params - 
   between gemma4uv's 54 ms linear and a whisper-large encode).
   **SCORED: WRONG on the high side of usage.** The tier-1 448^2 encode (196 tok) runs ~3 s
   and the server's off-thread 300-token cats encode measured 42 s on the SINGLE-threaded
   media worker (the worker pins team mode off) - the prediction priced the jobque'd
   multi-core path, which the chat CLI gets (~9 s ttft incl. prefill) but the server worker
   does not. The mechanism (GEMM-bound, 543 M params) was right; the deployment context
   dominated. Slice J's Metal tower work is where this cost goes away.
6. The showcase turn (image+audio+text, one prompt) works on the first session after slice E
   with no scheduler change.
   **SCORED at E2: WRONG on its premise** - `add_user_audio_` existed but never served the
   conformer families (chat AudioTower = whisper-class only); the two-modality showcase
   (image + text recall) landed instead, and audio-in-chat is followup #41.
7. Deepstack (slice H) is the CHEAPEST leg - under a session end to end, because both halves
   are adds to an already-green rail; its only red will be a slice-indexing bug caught by the
   zeroed-slices control, not a math bug.
   **SCORED: CORRECT on cost** (well under a session, first-run tier-1 modulo the projector-key
   spelling), **HALF on the red**: no slice-indexing bug materialized in das - the defect the
   controls caught was in the GATE ITSELF (mean+v0..v3 provably blind to a zeroed slice; the
   q-probe extension was the fix). The zeroed-slices DECODER control passed first try (10.4).
8. qwen2.5o (slice I) costs more than deepstack but less than the Omni core; the window
   attention masks land right on the first attempt (they are index arithmetic), and the
   surprise, if any, is again in the reference's preproc arithmetic (patch-14 geometry).
   **SCORED: CORRECT on cost and on the mask** - tier-1 went 5/5 green on the first run,
   windows included. The surprise was NOT preproc (align-28 geometry passed via the shared
   code) but the ORACLE'S FIXTURES: every stock debug image is window-symmetric, so an
   all-full-attention poison passed tier-1 untouched until the `quad` generator landed - 
   the prediction named the right neighborhood (reference-side arithmetic) and the wrong
   organ (fixtures, not preproc). Honorable mention: the vocab spells the span markers
   `<|vision_bos|>`/`<|vision_eos|>`, which no prediction saw coming.

### Measurement predictions (registered 2026-08-22 BEFORE the J numbers; das vs llama.cpp,
### M1 Max t=8, coco fixture, `lcpp_bench --image` vs patched llama-mtmd-cli, r=3)

Context the predictions price in: the Metal tower serves gemma3v/gemma4v ONLY - every qwen
tower encodes on the CPU even on the das Metal leg (slice J's deferred Metal-tower work),
while mtmd offloads the mmproj to Metal unless told not to.

9.  **Qwen3VL-4B** (dense q8 decoder + ~430 M qwen3v tower): decoder cells land gemma-shaped - 
    Metal tg das LEADS 10-25%, Metal pp within +/-10%, CPU pp das leads 15-40%. img:enc is the
    known loser: das enc (CPU tower both legs) 3-8 s vs mtmd Metal enc <= 0.4 s (>= 10x
    against das); on the CPU pair (mtmd pinned --no-mmproj-offload) das enc is within 2x
    of mtmd's CPU enc, either side.
    **SCORED: HALF** (released `lcpp_bench --image` vs patched llama-mtmd-cli, M1 Max t=8, coco fixture, best-of-3). Metal tg +21% yes, CPU pp +16% yes, enc-vs-Metal 29x yes; Metal pp -16%
    (outside +/-10%, wrong side) no; CPU enc 4.6x against das no - mtmd's clip CPU encode is
    genuinely ~5x faster than the das CPU tower, not within 2x.
10. **Qwen3VL-8B** (same tower class, ~580 M): the 4B shape scaled - Metal tg das leads
    10-25%, enc verdicts identical to P9.
    **SCORED: MOSTLY** (released `lcpp_bench --image` vs patched llama-mtmd-cli, M1 Max t=8, coco fixture, best-of-3). Metal tg +12% yes (bottom of band), enc verdicts identical yes (5.3x CPU,
    15x Metal); unpredicted: Metal pp worsens to -30% (vs the 4B's -16%) - the deepstack
    Metal cell (CPU repack + per-layer enc_adds) is the suspect, unprofiled.
11. **Qwen3-Omni-30B-A3B q8** (MoE, 3B active; 32.5 GB fits the 64 GB box on BOTH legs):
    Metal tg das leads 10-30% (the wave-C MoE arm), Metal pp +/-15%; CPU pp das leads; enc =
    the 543 M tower on CPU, 4-10 s das.
    **SCORED: HALF, sandbagged the lead** (released `lcpp_bench --image` vs patched llama-mtmd-cli, M1 Max t=8, coco fixture, best-of-3). CPU pp das leads yes but by 6x (not "leads"), CPU tg
    +45%, Metal tg +159% (band said 10-30%) - mtmd's Metal MoE decode is far weaker than
    predicted; Metal pp -41% no (outside +/-15%); enc 6.3 s yes, and mtmd's own CPU encode
    collapsing to 59 s was unpredicted.
12. **Qwen2.5-Omni-3B q8** (the biggest tower in the set, ~1.3 B window ViT): das enc 8-20 s
    both legs, mtmd Metal enc beats das by >= 20x; decoder (3B dense) Metal tg das leads
    10-25%, pp +/-10%.
    **SCORED: HALF** (released `lcpp_bench --image` vs patched llama-mtmd-cli, M1 Max t=8, coco fixture, best-of-3). enc 12.1 s yes; mtmd Metal enc 17.5x (near the >=20x claim); Metal tg +45%
    (above band), Metal pp -12% (just outside). Direction right everywhere, magnitudes off.
13. Cross-cut: at least one of the 8 pairs voids on cv > 3% and takes a settle + re-run;
    zero das crashes across all cells (every rail in the sweep is mutation-gated).
    **SCORED: HALF.** The cv void happened (4B ref CPU tg, 5.9% - re-run reproduced the value
    to 0.2%; the cv is inherent to mtmd's 15-token decode window, not box noise). "Zero das
    crashes" held literally but missed the real failure mode: TWO silent-wrong classes (the
    grid never passed; the suppress-form derail) - the sweep's value was finding them, and
    the prediction's frame ("crashes") was looking at the wrong hazard.

### Slice L - the CPU encode gap: qwen towers get the q8 block-GEMM lane (predictions
### registered 2026-08-22 BEFORE implementation; the agreed first post-merge slice; every
### enc figure below = released `lcpp_bench --image`, CPU, --image-think, r=3, t=8, M1 Max)

Design: port gemma3v's q8 serving lane (Q8_0 transcode at read, requant + q8q8 batch at
encode, its own image tag, pin knobs, policy = q8 unless the accelerate float-batch tier is
armed) to dasllama_qwen3v + dasllama_qwen25v. Block GEMMs only; conv/mergers/deepstack stay
on file planes. qwen3v ff=4304 is not 32-aligned -> the gemma3v ff_pad treatment (zero up
rows / down cols), Q8 LANE ONLY so the exact bf16 lane stays byte-identical to today.
tower_read_gemm_q8 hoists into dasllama_tower (g3v+g4v+two new = four copies otherwise).

14. The q8 lane speeds the das CPU encode 3.5-5.5x per qwen tower (gemma4v's measured 4.8x
    CPU-tower factor, same block-GEMM dominance): Omni-30B enc 6.3->1.2-1.8 s, VL-4B
    5.1->1.0-1.5 s, VL-8B ~6->1.1-1.7 s, Omni-3B 12.1->2.2-3.5 s.
15. Post-change das CPU enc vs mtmd's clip CPU enc: das lands within 1.5x either side on the
    three dense towers (from 4.6-5.3x behind), and the 30B lead (mtmd 59 s collapse) widens
    past 30x.
16. Greedy coco captions at the bench prompt change on at most 1 of the 4 models vs the
    exact lane (gemma experience: the q8 tower is caption-stable; near-ties may flip).
17. Tier-1 q8 error vs the f32 oracle lands 0.15-0.35 x token rms on both qwen towers
    (gemma3v measured 0.22-0.25 at 27 blocks; qwen3v is 27 blocks, qwen25v 32 windowed).

#### Slice L findings (2026-08-22, the tier-1 + probe round BEFORE any perf measurement)

The port went in as designed for BOTH towers; the tier-1 gates then split them:

- **qwen3v (Omni-30B + VL dense): SHIPS.** q8 error 0.42 x rms worst (gray448; cb fixtures
  0.1-0.3) - roughly double gemma3v's 0.22-0.25 floor, the qwen ViT's outlier channels.
  Bar 5.2e-1; the zero-layer-13 poison reds it by +0.73 (2.4x discrimination). Deepstack q8
  cells run cb-only at 6.5e-1 (the uniform gray fixture measures 7.9 x rms on the tap-slice
  probes - taps sample mid-network residuals where the outlier noise arrives unwashed); the
  zeroed-slices decoder control (10.4) is the gate that proves the slices still carry.
  Captions: VL-4B deepstack q8 caption full and correct.
- **qwen25v (Omni-3B): DOES NOT SHIP - the lane is reverted, exact serving stays.** The q8q8
  lane measured 2.0 x rms vs the oracle, and the zero-layer poison sat BELOW any bar wide
  enough to admit the clean encode (excess -0.38): the gate cannot distinguish q8 noise from
  a deleted layer. Localization: weights are fine (weight-only Q8 rounding = 0.0067 x rms);
  per-site chains on random x are fine (q8q8 3-7% worst-element, down float-x 2.4%); the
  disease is per-32-block ACTIVATION requant on the Qwen2-VL-lineage ViT's outlier rows
  (+/-100 channels), at every site - the gated-FFN down input (silu(gate)*up) worst (its
  float-activation form, `matmul_q8_batch` w8xf32, recovered only 2.0 -> 1.38 x rms; probed,
  then removed with the lane). An all-float-activation q8 lane wins nothing on CPU: these
  GEMMs are compute-bound and the 4-5x IS the int8xint8 SIMD. Options if the Omni-3B CPU
  encode ever matters: outlier-aware activation quant (SmoothQuant-style folds, needs
  calibration) or the Metal tower (the planned other factor).

Prediction scoring (14-17):
14. **HALF.** The mechanism and the recipe landed, but only for qwen3v; qwen25v excluded
    (unpredicted - the prediction assumed all qwen towers quantize alike; the Qwen2-VL
    activation-outlier lore was known and not priced in). Speedups not yet measured.
15. Not yet measurable (perf round pending); qwen25v arm VOID (no q8 lane).
16. On track - VL-4B q8 caption unchanged-quality; full sweep pending.
17. **WRONG on both sides**: qwen3v landed ABOVE the band (0.42 vs 0.15-0.35) and qwen25v
    landed at 2.0 - the band assumed gemma-like activation statistics; the per-family
    outlier ladder (gemma 0.25 -> qwen3v 0.42 -> qwen25v 2.0) was the real story.

#### Slice L measurement (2026-08-22, released rig, CPU cells, --image-think, r=3, t=8, M1 Max)

| model | das enc J (exact) | das enc now (q8) | speedup | vs mtmd clip CPU (J) |
|---|---|---|---|---|
| Qwen3VL-4B | 5.1 s | **1.084 s** | 4.7x | ~1.11 s - TIE (das -2%) |
| Qwen3VL-8B | ~10 s | **1.895 s** | ~5.3x | ~1.89 s - TIE |
| Qwen3-Omni-30B | 6.3 s | **1.496 s** | 4.2x | 59 s - das 39x ahead |

Captions all correct at template-default thinking (cats + couch + remotes; the 30B its terse
form). Cells stamp `(qwen3v q8)`. Qwen2.5-Omni-3B unmeasured - no q8 lane (slice-L findings).

14. **CORRECT** (for the towers that shipped): 4.2-5.3x inside the 3.5-5.5x band.
15. **CORRECT** (for the shipped towers): the dense pairs land at mtmd-clip parity (predicted
    "within 1.5x either side"), and the 30B lead is 39x (predicted past 30x). qwen25v VOID.
16. **CORRECT so far**: captions unchanged-quality on all three measured models (+ the 4B
    chat-suite caption). 4 of 4 checked, 0 flips.
