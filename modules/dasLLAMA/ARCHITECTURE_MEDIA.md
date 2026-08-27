# dasLLAMA Architecture - encoder tower, audio, and vision file charters

Companion to `ARCHITECTURE.md`; section numbers are that document's.

### 1.7 Encoder towers, audio, and ASR

- **`dasllama_asr_types.das`** - the ASR floor: the capability/segment/timestamp types every
  family file and the facade share (`AsrCaps`/`AsrTimestamps`/`TranscribeSegment`, plus
  `asr_ctx_guard`). Family modules require this instead of each other - a shape needed by two
  families moves up here, never sideways.
- **`dasllama_tower.das`** - the family-neutral encoder-tower home, composed by every audio AND
  vision tower: the oracle-exact activations (erf / tanh-LUT / quick-LUT gelu, swiglu, geglu),
  the LayerNorm/RMS row forms, bias and residual row adds, the
  `mm_blob_b`/`mm_bf16_b`/`mm_plane_b` GEMM wrappers, `Clamp`/`read_clamp`,
  `im2col_rgb_patches`, `rope_neox_2d_rows`, `rope_neox_tab_rows`, `avg_pool2d_rows`,
  `interpolate_grid_bilinear_aa`, `tower_read_conv_pair_folded`, the padded stage readers
  (`tower_read_gemm_q8`, `tower_stage_q8_zero_rows`/`tower_stage_q8_pad_cols` and their f32
  twins `tower_stage_f32_zero_rows`/`tower_stage_f32_pad_cols` - the load-scope padded
  stagers for FFN widths that are not 32-aligned - and `tower_zero_span`), blocked `attention_bidir` and
  its per-window form `attention_bidir_windows`, and the encode-stage prof rail. The one home:
  a family file that re-implements one of these is a defect, and nothing here names a family
  type.
- **`dasllama_audio.das`** - the audio encoder tower: the mel front-ends (mtmd and whisper.cpp
  flavors), `AudioTower` with its staging, q8-quantize, and image rails, `EncoderState`, and the
  whisper-class encode + block loop with its GPU hooks. Composes `dasllama_tower.das`.
- **`dasllama_audio_io.das`** - decode-any-format -> 16 kHz mono f32 PCM. The only file that talks to
  miniaudio.
- **`dasllama_asr.das`** - the ASR facade: capability declaration, timestamp granularity, the
  backend-neutral entry points.
- **`dasllama_whisper.das`** / **`dasllama_parakeet.das`** / **`dasllama_canary.das`** /
  **`dasllama_qwen3a.das`** / **`dasllama_gemma4a.das`** - one file per model family, each owning its
  weights, its decode loop, and its quirks. Shared tower pieces go up into `dasllama_tower`, not
  sideways between families.
- **`dasllama_vad.das`** - Silero-VAD weights and per-stream state.

### 1.7b Vision

- **`dasllama_vision.das`** - the image preprocessing rail: dynamic-resolution geometry, the
  letterbox resize (aspect-preserving bilinear onto a centered black canvas), u8->f32 normalize,
  and the `DASLLAMA_VISION_DUMP` PPM writer. The only preprocessing home.
- **`dasllama_vision_io.das`** - image decode to RGB8, from a file or a byte blob. The only file
  that talks to stbimage; re-exported by the facade (like `dasllama_audio_io`) so consumers
  decode through one seam - the engine itself takes decoded pixels.
- **`dasllama_gemma4uv.das`** - the gemma4uv embedder (gemma-4 dense): mmproj load and the
  im2col -> LayerNorm -> GEMM -> position-table -> projection forward. One file per vision
  projector family, following the audio tower pattern; shared pieces move up into
  `dasllama_tower.das` (the encoder-tower home), never sideways.
  A shipped mmproj mixes element types per tensor - gemma-4's "BF16" file stores the patch
  embedder as F32 and only the projection as BF16 - which is why a weight plane's element type
  follows its source tensor, per tensor, never a per-file verdict.
- **`dasllama_gemma4v.das`** - the gemma4v ViT tower (gemma-4 E-series): mmproj load (bf16 GEMM
  planes as the file has them, the clamp sidecars as a blob table) and the 16-block pre-norm RMS
  forward - clamped GEMMs, per-head q/k RMS, two-axis NEOX rope, weightless V RMS, unscaled
  bidirectional attention, GEGLU-quick - then the 3x3 pool, RMS and projection. Composes the
  `dasllama_tower.das` tower pieces; owns only its layout and the block loop.
- **`dasllama_gemma3v.das`** - the gemma3 SigLIP tower (size-invariant across the gemma-3 line;
  4B and 12B are the tested pairs): mmproj load (the file's f16 planes widen exactly to f32;
  the ffn pair serves at the layout's padded 4352 width, so every q8-lane GEMM quantizes) and the 27-block pre-norm
  LayerNorm forward - biased GEMMs, learned position add, scaled bidirectional attention,
  GELU-tanh - then post-LN, the 4x4 pool, weighted RMS and projection. The canvas is FIXED at
  896^2 (the learned table covers exactly that grid), and the family's image_mean/std (0.5) is
  PREPROCESSING, not graph - `encode_image_` scales, `gemma3v_encode` takes planes raw like the
  mtmd fixtures do. Composes `dasllama_tower.das`; owns only its layout and the block loop.
  Two sanctioned exceptions to the family-quirk placement rule live outside this file: the
  image span markers sit on the SHARED `gemma_chat` template in `dasllama_common.das` (template
  detection cannot tell gemma3 from gemma2 - both spell `<start_of_turn>` - and the chat
  layer's `image_vocab_ok` gate refuses a text-only vocab); and the carrier deliberately serves
  ONE f32 plane (F32/F16/BF16 widen exactly; anything else refuses by name at stage) instead of
  gemma4v's per-tensor plane split - the Metal leg runs the f32 mulmm, so there is no bf16
  plane to preserve.
- **`dasllama_qwen3v.das`** - the qwen3vl vision tower (Omni AND dense Qwen3-VL,
  `qwen3vl_merger`): mmproj load (per-tensor bf16/f32 planes; the temporal patch-conv PAIR
  folds to one f32 GEMM at stage - a still evaluates both convs on one frame, so conv_0+conv_1
  = (W_0+W_1)*img) and the pre-LN forward - fused-qkv biased GEMMs, vision-mrope (h/w ladder
  tables from `build_rope_tabs_vision`, full-head NEOX apply), scaled bidirectional
  attention, GELU-tanh - after a spatial-merge REORDER of the patch stream with the 48x48
  learned position table resized to the grid by an antialiased bilinear resample; then post-LN and
  the 2x2 merger MLP (the x4 reshape is free - merge partners are already adjacent). Dense
  carriers add DEEPSTACK taps (collected from the TENSOR list, never the metadata flags):
  after each tap block a LayerNorm->fc1->GELU->fc2 merger over the x4-merged rows emits one more
  proj_dim slice, and the output rows widen to (1+n_deepstack)*proj_dim - slice 0 = the main
  merger, the decoder adds slice l+1 after layer l. The family token budget [8, 4096] is
  mtmd's, not the gemma-scoped DASLLAMA_VISION_* knobs; image_mean/std (0.5) is
  PREPROCESSING like gemma3v. CPU serving default: the block GEMMs serve as Q8_0 planes
  (read-time transcode, the gemma3v recipe); the exact lane stages EVERYTHING f32-in-blob at
  ff_pad both dims - blocks, mergers, and taps - because the Metal tower (`register_qwen3v_gpu`,
  blocks + rope + taps + tail on-device, tap/tail proj outputs land in `ds_stash`/`ds_slice`)
  and the float-batch tier both read f32 planes, and the same lane is the parity rail (the
  f32 sites are this charter's sanction, not a fallback). Pin knobs
  `set_qwen3v_q8`/`reset_qwen3v_q8`, two image tags; the lane policy prefers f32 when the
  GPU tower or accel serves. Composes `dasllama_tower.das`; owns
  only its layout, the reorder walk, and the block loop. SANCTIONED over the 1 GiB staged-mint line (this family
  and qwen25v): the Omni (2.1 GB) and Qwen2.5-Omni (2.6 GB) mmprojs stage source+image at
  once with no cap - the same shape their audio halves already stage - until
  `followup_general.md` 24's streaming mint covers towers.
- **`dasllama_qwen25v.das`** - the qwen2.5 window-attention ViT (Qwen2.5-Omni/VL, projector
  `qwen2.5o`): 32 RMS-normed blocks with separate biased q/k/v GEMMs and a gated-silu FFN
  whose hidden width comes from the TENSOR dims (the metadata's feed_forward_length lies);
  rope-only positions, per-channel CLIP normalize (`vision_normalize3`). Rows sort by 112 px
  windows of 2x2-merged groups, non-full blocks attend per contiguous window range
  (`attention_bidir_windows` in the tower - no mask tensor exists), every `n_wa_pattern`-th
  block attends in full, and the merger un-sorts back to group-row-major. Its decoder is the
  `qwen2vl` arch whose plain MROPE reads the rope sections as contiguous ranges
  (`Config.mrope_interleaved` false -> `build_rope_tabs_mrope`; the qwen3vl family sets the
  flag and keeps the interleaved walk). This tower serves the file's planes ONLY - no q8
  lane: its residual rows carry the Qwen2-VL-lineage outlier channels that per-32-block
  activation requant cannot represent (a q8q8 lane measures 2.0 x rms vs the oracle, where a
  DELETED layer measures less - the gate cannot discriminate; the weights themselves
  quantize fine at 0.007 x rms, and a float-activation q8 GEMM wins nothing on these
  compute-bound shapes since the CPU speedup IS the int8xint8 dot).
- **`dasllama_vision_embedder.das`** - the vision carrier: `VisionEmbedder` / `VisionState`, the
  `AsrModel` shape for vision - one union through every seam, the family sniffed from the mmproj
  (`clip.vision.projector_type`, or a `.dlim`'s baked tag) at load, one-line arms. Outside a
  family's own file, a family type is named only here, in `dasllama_metal_tower.das`'s family
  hooks, and in files under `tests/` - the set `REVIEW.das`'s seam check enforces.
- **`dasllama_audio_embedder.das`** - the audio carrier: `AudioEmbedder` / `AudioState`, the
  vision carrier's audio twin - one union through every seam (server media worker, facade
  `encode_audio`, tutorials), the family probed from the mmproj's audio tensor (or a `.dlim`'s
  baked tag) at load, one-line arms. Outside a family's own file, an audio family type is named
  only here, in `dasllama_asr.das`'s union field and one-line arms (the ASR rail's own carrier),
  in the metal family hooks (`dasllama_metal_tower.das`, `dasllama_metal_asr_dec.das`), in
  `benchmarks/asr/asr_bench.das` (the ASR bench pins a family serving knob for its correctness
  rail), in `utils/dasllama-convert/main.das` (the per-family `.dlim` bake tool dispatches on family by
  design), and in files under `tests/` - the set `REVIEW.das`'s seam check enforces.

Vision oracle provenance (the convention `REVIEW.md`'s fixture rule points at): real image
fixtures and mmproj files live in the models dir with `.sha` pins, fetched never generated
(their `performance/fetch_models.das` entries are the checkable pins); the mtmd reference dumps
live beside them in `gemma4-vision-oracle/`, `gemma3-vision-oracle/` and
`qwen3vl-vision-oracle/`, whose `mint.sh` (gemma4uv, qwen3v Omni), `mint_4b.sh` (qwen3v
deepstack), `mint_25o.sh` (qwen25v), `mint_e2b.sh` / `mint_e4b.sh`
(gemma4v) and `mint_gemma3.sh` (gemma3v) record the
exact `llama-mtmd-debug` / `llama-mtmd-cli` invocation that minted each
dump, so regeneration is a command, not archaeology. An encode oracle dump is minted on the
CPU, `-fa off`, from the f32-widened mmproj twin - the only true-f32 reference arm (the
reference's Metal "f32" GEMM stages half operands, its flash-attention path casts K/V to f16,
and the shipped bf16 mmproj rounds activations to bf16; its own four arms spread
<= 6.5e-3 on the gemma4v tokens).

