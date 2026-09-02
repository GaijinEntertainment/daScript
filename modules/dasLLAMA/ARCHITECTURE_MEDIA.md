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
- **`dasllama_audio_io.das`** - decode-any-format -> 16 kHz mono f32 PCM, and the reverse leg the TTS
  facade hands out: f32 samples -> 16-bit PCM bytes / a RIFF WAV file. The only file that talks to
  miniaudio.
- **`dasllama_asr.das`** - the ASR facade: capability declaration, timestamp granularity, the
  backend-neutral entry points.
- **`dasllama_whisper.das`** / **`dasllama_parakeet.das`** / **`dasllama_canary.das`** /
  **`dasllama_qwen3a.das`** / **`dasllama_gemma4a.das`** - one file per model family, each owning its
  weights, its decode loop, and its quirks. Shared tower pieces go up into `dasllama_tower`, not
  sideways between families.
- **`dasllama_vad.das`** - Silero-VAD weights and per-stream state.

### 1.7c Text-to-speech front end

The TTS family mirrors the ASR one - a types floor, a shared block home, a facade, one file per
model family - and adds the text front end no ASR family needs. The front end runs in three
stages, each its own file, and every stage is data-driven from the model store: nothing under
`models/` carries TTS data.

- **`dasllama_textnorm.das`** - text normalization: numbers, ordinals, years and decades,
  currency, percentages, clock times, dates, fractions, units, abbreviations, URLs and e-mail
  addresses become the words a reader says; prosodic punctuation and apostrophes survive,
  everything else becomes a space. Twelve defects of the reference normalizer are fixed here
  and pinned by the tests. Pure code, no data file.
- **`dasllama_postag.das`** - the tokenizer and part-of-speech tagger. The tokenizer reproduces
  the reference pipeline's English tokenization over normalized text (the exception table for
  contractions and abbreviations, prefix and suffix punctuation, infix hyphens); the tagger is
  a greedy averaged perceptron over the PTB tagset. Both load from `tts_postag.bin`
  (`harness/train_postag.py`: UD English-EWT gold plus normalizer-shaped prose tagged by the
  reference tagger, `harness/mint_postag_silver.py`). Tags matter downstream only through the
  lexicon's part-of-speech entries and the punctuation phonemes.
- **`dasllama_g2p.das`** - grapheme-to-phoneme into the 45-symbol US inventory: a gold lexicon
  with part-of-speech keyed entries, a silver lexicon, function-word rules that read what
  follows (the pass runs right to left), inflection stemming, capitalization and acronym stress,
  then a fallback chain - CMUdict pre-rendered into the same inventory, then a GRU spelling
  model - so no word is ever dropped. Loads `tts_g2p.bin` (`harness/build_g2p_data.py`),
  searched in place as byte-sorted string tables. The 200-sentence fixture under
  `tests/_tts_fixtures/` (minted by `harness/mint_tts_g2p_fixture.py` from the G2P fidelity
  experiment) is the parity rail for all three stages.
- **`dasllama_tts_types.das`** - the TTS floor: `TtsCaps`, `TtsAudio` (f32 PCM + rate), `TtsNoise`
  (the source noise a synthesis consumed - captured from the oracle, or drawn). Family files
  require this, never each other.
- **`dasllama_tts_blocks.das`** - the StyleTTS2-lineage block home, the TTS twin of
  `dasllama_tower.das`, in two layouts. The channel-major [C][T] forms are the reference: Conv1d
  (dense as im2col + the batched GEMM, depthwise direct, forward and transposed), the dense
  layer, LayerNorm over rows and over channels, InstanceNorm and AdaIN, AdaLayerNorm, the
  bidirectional LSTM (gates i,f,g,o, both bias halves pre-summed), LeakyReLU / Snake / sigmoid /
  tanh, nearest and ONNX-half-pixel linear resampling, the duration-to-frame expansion,
  half-to-even rounding, PCG32 with a polar normal, the harmonic-plus-noise sine source, and the
  STFT pieces (edge pad, magnitude and phase, polar to rectangular, reflection pad). The
  token-major [T][C] "rows" forms are what bert, the prosody branches, the decoder and the
  generator run (the F0 and energy curves and their projections stay channel-major - one
  channel is a column either way): a dense stride-1 conv is
  k tap-GEMMs on the tiled `gemm_f32_jo` over the shifted input rows (no im2col; the served
  width `cout_s` on the 16-column tile) - on the q8 lane one Q8·Q8 batch GEMM per chunk of
  output rows over tap-stacked int8 rows, the k shifted input rows side by side (K = k*cin,
  the weight baked [cout][k*cin], zero scales where a shift leaves the input), so nothing
  accumulates across taps - a transposed conv one GEMM plus a gather overlap-add, the AdaIN
  residual block's upsampling pool a per-row depthwise transposed conv and its shortcut a
  row copy (the block's width sits on the 16-column tile; AdaIN takes a width off the lane
  one channel at a time, which is how the decoder's concat rows run),
  the dense layer the same tiled GEMM off `wt`, Snake and AdaIN four channels a lane; every rows
  kernel splits its rows across lanes on tile-aligned block edges, and `tests/test_tts_blocks.das`
  holds each one to its channel-major twin at the dot-envelope bar. A weight is an ONNX-layout
  array plus the served layout `conv1d_prepare` / `linear_prepare` mint for the consumer the
  reader names (`served_rows`, `rows_only`), the unread one dropped; beside every weight array
  sits its `TtsSpan` into the model's blob, and `weights_walk` is the one walk that moves weights
  into a staging blob or binds them as borrowed views over a served plane (`release_weight` is
  the one teardown). The sine source keeps the reference's operation order exactly: its phase
  reaches 1e5 radians, where one float32 ulp is a hundredth of a radian. One home: the block
  home holds the operators, and it names no family type.
- **`dasllama_styletts2.das`** - the StyleTTS2-lineage model both families share: the weight
  map of the converted GGUF (conv geometry rides as `styletts2.conv.<weight>` metadata, so the
  assembly hardcodes the wiring and reads the shapes; the STFT convention - replicate or reflect
  padding, basis-folded or envelope-normalized inverse, the magnitude epsilon, which
  resampler's arithmetic the sine source mirrors - rides as `styletts2.*` metadata too), the
  voices as `voice.<name>` [rows x style] tensors, and the assembly: PL-BERT (one ALBERT layer
  applied twelve times) -> text encoder -> duration encoder -> durations -> alignment ->
  prosody (F0, energy) -> decoder -> iSTFTNet generator, with a stopwatch per stage
  (`TtsTimings`) and `StyleTts2Trace` collecting the stage tensors the parity rail compares.
  The served carrier rides the `.dlim` image rail (`ARCHITECTURE_IMAGE.md`): `stage_styletts2`
  reads the GGUF into `St2Staging` - the served layouts minted, every weight moved into one
  staging `blob` with its span recorded - and `load_styletts2` maps the sibling image under
  the lane's tag or bakes it from that staging (`DASLLAMA_IMAGE=0` mints in memory,
  `mint_styletts2` is the suites' off-rail control). Two lanes, each its own image: `tts-q8`,
  the served default - the rows GEMMs' weights as Q8_0 quants in an int8 plane repacked for
  the box's backend, the tag config-bound - and `tts-f32`, the file's planes under a
  config-free tag: the reference lane the parity rail and the block test hold against, which
  carries no optimization duty (the rig held q8 at f32 quality, and f32 costs memory for
  nothing). The `set_tts_q8` pin selects a lane; `reset_tts_q8` returns to the default. The
  meta blob carries the scalars, the
  spans and the voice roster through the leaf structs' own `serialize` overloads, and the
  loader binds every weight array as a borrowed view over the mapped `blob` plane after the
  parse (post-load runs before the planes bind, so binding cannot happen there). The carrier
  therefore takes an explicit `finalize` - views forgotten, owned buffers deleted, the backing
  released once - and so does every struct that holds it (`TtsModel`), the vision embedder's
  reason. The generator carries the sec.2.14 hook slot (`register_styletts2_generator_gpu`,
  `styletts2_generator_gpu_stats`): a driver takes the rows-form input, the style and the source
  spectrum and answers with the waveform or declines; the SineGen phase chain and the harmonic
  STFT stay on the CPU in both routes (the phase law), the trace rail keeps the CPU chain, and
  engage is read from the counters. No driver fills the slot yet. The assembly names no family;
  a family's quirk lives in its family file.
- **`dasllama_kitten.das`** - the KittenTTS family (nano and mini): the reference driver's symbol
  table, re-spacing rule and style-row rule (the chunk's character count), its speed priors and
  voice aliases (`kitten.*` metadata), its 5000-sample tail trim, and the rewrite of the front
  end's inventory into the espeak-style IPA these models consume.
- **`dasllama_kokoro.das`** - the Kokoro family (Kokoro-82M): the reference pipeline's
  vocabulary (`kokoro.symbol_*` metadata - the front end's own inventory, no rewrite), its token
  wrapping and style-row rule (the phoneme string's character count less one). Fifty-four voice
  packs of 510 rows.
- **`dasllama_tts.das`** - the TTS facade: `load_tts_model` (the shared model plus the family
  picked by `general.architecture`; `tts_g2p.bin` and `tts_postag.bin` read from the GGUF's
  directory), `caps`, `synthesize_stream` (text -> normalize -> the reference sentence chunker,
  400 characters a chunk, abbreviations and decimals never split -> per chunk: phonemize -> the
  family's symbols and style row -> PCM, timed, delivered to the caller's block as it lands) and
  `synthesize` (the same, concatenated, timings summed). Requires no `audio` module.

Every local container on the TTS path is `var inscope`: the persistent heap frees nothing at
scope exit, and one synthesis allocates its activation buffers per call - the generator's alone
run to hundreds of megabytes per sentence, so a bare local is a per-sentence leak that ends in
the OS killing a long run.

The product surfaces sit outside the module: `utils/dasllama-server/txt2wav.das` (text or a
file -> a WAV, the timings line on stderr) and the server's `/v1/audio/speech` route (a
dedicated TTS worker thread, one synthesis at a time, the audio served from a temp file the
route reaps a minute after the wire closes - dasHV writes string bodies only). The rig that
scores the whole chain is `harness/tts_rig.py` over `harness/tts_synth.das`: the 200-sentence
fixture through one model and voice, WER by parakeet, UTMOS by the metrics venv, one table.

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
  compute-bound shapes since the CPU speedup IS the int8xint8 dot). It stages every GEMM
  f32-in-blob at the served padded widths (sec.2.13) whatever the file's element type, and
  bakes the halfword twin (`ARCHITECTURE_IMAGE.md` sec.2.1i) when the block and merger GEMMs
  share one halfword type; the Metal tower serves stem, blocks and merger tail off `s.x0`
  through `register_qwen25v_gpu`.
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


### 2.13 Towers serve padded GEMM widths {#tower-padded-widths}

A tower's served GEMM widths round UP to 64 - the Metal mm tile - while the tensor-shape verify
keeps the file's own widths. The gated FFN pair pads with zero weight ROWS and the down GEMM with
zero tail COLUMNS; a patch conv pads its K through the im2col row stride, whose tail lanes are the
resize's zeros. Every pad lane contributes a zero product, so a padded GEMM's output is
bit-identical to the unpadded one on both the CPU and the Metal route, and one served layout feeds
both. `q25v_patch_pad` / `gemma3v_patch_pad` and the `ff_pad` fields are the served widths;
`im2col_rgb_patches` takes the padded stride as `row_stride`.

The qwen25v tower stages every GEMM f32-in-blob at those widths whatever the file's element type -
blocks and merger alike - because the Metal tower reads f32 planes or the baked halfword twin.

### 2.14 Family GPU hooks install from the driver and always decline {#tower-gpu-hook}

A family file owns the hook SLOT for a stage the GPU can serve - a `var private` function pointer
plus a `register_*` entry - and the Metal tower driver fills it at `[init]`. The direction is
forced: the driver requires the family file for its types, so the family cannot require the driver
back. A box with no driver leaves the slot empty and the CPU form runs.

Every hook answers "declined" in its own return - `false` for the block and front hooks, `-1` for
the whole-chunk mel hook - so a decline is a fallback, never an outage, and the CPU form stays the
reference.

A family whose hook takes a whole stage splits its encode at the seam the driver needs: a
`*_stem_cols` / `*_window_frames` half both routes run (im2col or windowing, plus every buffer the
driver writes into), and a `*_stem_finish` half only the CPU route runs. The driver reads the
column buffer (`s.x0`, `st.xw`) and never the residual stream the CPU half would have filled.

### 2.15 The tower weight lane is a policy, not a default {#tower-weight-lane}

A tower serves its GEMMs on one of two lanes: q8 planes (the CPU serving format) or the file's
exact f32 planes. Un-pinned, the lane follows the fastest GEMM path on the box - a serving Metal
driver reads the f32 blob and declines q8, so `*_gpu_serves` answering true flips the default to
exact, and every other box takes q8. Each family exposes the same trio: `set_*_q8` pins a lane,
`reset_*_q8` returns to the policy, `*_serves_q8` reports the lane the next load would take. The
lane picks the image tag, so the two lanes are separate images that coexist.

Pins exist for the arms that must not follow the box: the parity legs, the CPU board rows, and the
facade's fp32 rail.

### 2.16 An ASR decoder that is a plain Model session rides the box decode policy {#asr-decoder-session}

The gemma4a, canary and qwen3a routes drive their decoders as ordinary `Model` sessions - embed
rows, then eval - so they load with no planar pin and take whatever decode form the box has armed,
the Metal blob included. The canary serving artifact is Q8_0 for exactly this reason: its tied
classifier serves the q8 plane, while the f16 parity carrier still mints planar on its own because
the blob drivers decline a tied-fp32 classifier at mint. Whisper and parakeet stay planar - their
decoders are hand-written CPU loops that read planes directly.
