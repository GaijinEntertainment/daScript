# dasLLAMA Architecture - encoder tower, audio, and vision file charters

Companion to `ARCHITECTURE.md`; a section is cited by its anchor. This document carries the encoder-tower, audio and
ASR file charters with the audio carrier's licensed family namers (`#tower-audio-charters`), the family GPU hooks
(`#tower-gpu-hook`) and the tower weight lane (`#tower-weight-lane`). The towers' served GEMM widths are
`ARCHITECTURE_MEDIA_TOWER.md#tower-padded-widths`; the vision charters, the vision carrier's licensed namers and the
vision oracles' provenance are `ARCHITECTURE_MEDIA_VISION.md#vision-charters`.

### Encoder towers, audio, and ASR {#tower-audio-charters}

`REVIEW.das`'s `check_require_bans` holds these charters' require bans: `dasllama_tower.das`
requires no carrier (`dasllama_audio`, `dasllama_vision`), no audio or vision family file the
carriers name, and no TTS family file (`dasllama_kitten`, `dasllama_kokoro`, `dasllama_pocket`,
`dasllama_styletts2`); only `dasllama_audio_io.das` requires `audio` and only
`dasllama_vision_io.das` requires `stbimage`. It licenses no names.

- **`dasllama_asr_types.das`** - the ASR floor: the capability/segment/timestamp types every family file and the facade
  share (`AsrCaps`/`AsrTimestamps`/`TranscribeSegment`, plus `asr_ctx_guard`, the token segment builder `asr_segment`,
  the SentencePiece detokenizer `asr_spm_detokenize`, and the FastConformer layer-offsets record `CanaryLayerOffs`
  canary, parakeet and gigaam share). Family modules require this instead of each other - a shape needed by two families
  moves up here, never sideways.
- **`dasllama_tower.das`** - the family-neutral encoder-tower home, composed by every audio AND vision tower: the
  oracle-exact activations (erf / tanh-LUT / quick-LUT gelu, swiglu, geglu), the LayerNorm/RMS row forms, bias and
  residual row adds and the fused bias + ReLU (`bias_relu_rows`), the `mm_blob_b`/`mm_bf16_b`/`mm_plane_b` GEMM wrappers
  and the q8-lane families' generic pair `tower_mm`/`tower_mm_rq` (with the kq lane's GEMM seats - `TowerGemmSeat`,
  `tower_gemm_seat`, `fc_gemm_seat_index`, `tower_requant` - where a family serves a carrier's own quantized planes),
  the GPU flavors' device block (`TowerDev`, `TowerDevBuild`, `tower_dev_put` / `tower_dev_find` / `tower_dev_word`,
  `ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block`), the GGUF conv reader `tower_read_conv_tap_major` (its reorder is `dasllama_repack`'s), the
  load's lane line `announce_lane`, `Clamp`/`read_clamp` (`read_clamp` returns the four scalars the projector file
  stores beside a weight tensor - `<base>.input_min` / `.input_max` / `.output_min` / `.output_max` - or an inactive
  +/-FLT_MAX clamp where the file carries none), `im2col_rgb_patches`, the Transformer-XL rel-position table
  `rel_pos_table` (each FastConformer family keeps only its requant and memo), `rope_neox_2d_rows`,
  `rope_neox_tab_rows`, `avg_pool2d_rows`, `interpolate_grid_bilinear_aa`, `tower_read_conv_pair_folded`, the padded
  stage readers (`tower_read_gemm_q8`, `tower_stage_q8_zero_rows`/`tower_stage_q8_pad_cols` and their f32 twins
  `tower_stage_f32_zero_rows`/`tower_stage_f32_pad_cols` - the load-scope padded stagers for FFN widths that are not
  32-aligned - and `tower_zero_span`), blocked `attention_bidir` and its per-window form `attention_bidir_windows`, the
  FastConformer CPU block `fc_block_cpu`, the FastConformer GEMM records every home's q8 regions and sites read
  (`fc_gemm_records`, `fc_front_records`), and the encode-stage prof rail. `fc_block_cpu` branches per family on its
  `FcCpuForm` flags alone - `biased` (off for parakeet), `tap_major` (off for canary), `conv_ln` (gigaam's LayerNorm
  conv module) and `vec4` (off for canary) - and on its position type, `FcRelPos` or `FcRotary`. The one home: a family
  file that re-implements one of these is a defect, and nothing here names a family type.
- **`dasllama_tower_image.das`** - a GPU-flavored tower image's mint and staged load (`#tower-dev-block`): the flavor pick
  (`tower_dev_flavor`), the CPU refusal of a GPU image (`tower_cpu_refuse`), the flavor tags (`register_tower_dev_tags`),
  the mint that bakes a driver's device block over the CPU form and copies every unconsumed plane beside it
  (`tower_dev_image`, `tower_mint_dev`, `tower_mint_flavor`) and the staged load that takes a flavor
  (`load_staged_image_dev`), the q8 gather both discrete homes bake their blocks with (`tower_gather_q8_regions`,
  `tower_bake_q8`: the regions in the row-major device layout under a driver's piece prefix), the lane picks every
  family's load and staging read (`#tower-weight-lane`: `tower_dev_halfword`, the halfword lane on every GPU
  flavor; `tower_lane_f16`, the exact pick's halfword lane on a lane and a flavor; `tower_load_f16`, a load's - a
  `.dlim` by its baked tag; `tower_staging_f16`, the running staging's) and the lane's tag spelling (`tower_lane_fmt`,
  `tower_lane_tag`), and a source's conversion line (`tower_dev_flavor_announced`, `tower_src_quant_type`,
  `tower_src_quant_name`: a GGUF source with an audio tensor (`a.*`, `mm.a.*`) stored quantized but not as Q8_0 keeps the
  flavor pick, and its load names the disk type, its `KqFmt` member and the lane form it re-encodes into). Generic over a
  family's staging and carrier; nothing here names a family type.
- **`dasllama_tower_flavor.das`** - the `[tower_flavor]` structure macro (`TowerFlavorSeat`): on a family's flavor
  record it declares the record's `backend`, `serves` and `bake` fields, the private global `g_<family>_flavor` and its
  registrant `register_<family>_flavor`, so the record and its registrant are one declaration
  (`ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block`). It names no family type.
- **`dasllama_audio.das`** - the audio encoder tower: the mel front-ends (mtmd and whisper.cpp flavors, and the canary,
  gemma4a, qwen3a and gigaam flavors - each a `MelForm` over the one `mel_spectrum_rows`, whose branches are the form's
  `MelSum` sum order and `MelLog` log law), `AudioTower` with its staging, q8-quantize, and image rails, `EncoderState`,
  and the whisper-class encode + block loop with its GPU hooks. Composes `dasllama_tower.das`.
- **`dasllama_audio_io.das`** - decode-any-format -> mono f32 PCM at the asked rate (`load_audio_mono`;
  `load_audio_16k_mono` is the ASR rate, a voice clip goes in at a TTS model's own), and the reverse
  leg the TTS facade hands out: f32 samples -> 16-bit PCM bytes / a RIFF WAV file. The only file
  that talks to miniaudio.
- **`dasllama_asr.das`** - the ASR facade: capability declaration, timestamp granularity, the backend-neutral entry
  points. It and `dasllama_audio_embedder.das` are the union carriers that route each facade function to one family; a
  family appears in a carrier at four places only - its union field, its finalize line, its kind value, and its one-line
  arms. `check_family_seams` over `AsrKind` licenses (`ASR_SEAM_EXEMPT`) the files driving a family past the facade's
  verbs: the audio carrier, the GPU family hooks - the Metal and Vulkan tower and ASR-decoder drivers and the CUDA home's
  seats (`dasllama_cuda_tower_fc`, `dasllama_cuda_tower_g4a`, `dasllama_cuda_asr_dec`, `dasllama_cuda_asr_dec_fc`), each
  filling a family's seat with its model and state - and the chain bodies both discrete homes' seats run
  (`dasllama_tower_chains_fc`, `dasllama_tower_chains_g4a`, `dasllama_asr_dec_chains`), which read a family's offsets,
  geometry and quirk constants to write its dispatch sequence once, `asr_bench.das`, the bake tool,
  `harness/pk_parity_probe.das`, `harness/cpu_asr_race.das`, `tests/`. `ASR_KIND_FILES` maps an `AsrKind` that has no family file of its own to the
  family file that serves it: `qwen3omni` to `dasllama_qwen3a.das`.
- **`dasllama_whisper.das`** / **`dasllama_parakeet.das`** / **`dasllama_gigaam.das`** / **`dasllama_canary.das`** /
  **`dasllama_qwen3a.das`** / **`dasllama_gemma4a.das`** - one file per model family, each owning its weights, its
  decode loop, and its quirks; the transducer families (parakeet, gigaam's RNN-T head) delegate the decode loop to
  `dasllama_rnnt.das`. Shared tower pieces go up into `dasllama_tower`, not sideways between families.
  `dasllama_gigaam.das` is GigaAM v3 (Russian, one-file GGUF): the rotary Conformer encoder with its CTC or RNN-T head.
- **`dasllama_rnnt.das`** - the transducer home: the LSTM prediction network, the joint and the
  greedy loop, serving parakeet's TDT (duration logits) and GigaAM's RNN-T (none) alike, the
  device hook its `rnnt_decode` asks first (the ASR-decoder drivers fill it), and the device
  seats' host form both drivers call (`rnnt_dev_*`).
- **`dasllama_vad.das`** - Silero-VAD weights and per-stream state. A chunk is a sequential step
  of about 0.7M multiply-adds, so its matvecs run on the calling lane - a fork costs more than it
  splits - and its conv weights sit [cin][3][cout], a tap's output channels back to back, each
  output summing its in-range taps in the file's order: the probabilities are bit-identical to the
  scalar per-output nest's. It owns the GPU seat `register_vad_gpu`, which both tower drivers fill
  (`#tower-gpu-hook`): the window, STFT and convs read raw samples alone, so every chunk of a call
  runs in one encoder dispatch, and the LSTM recurrence with its decoder walks the chunks in one
  workgroup. The batch VAD (`vad_probs`, `find_speech_timestamps`) asks the seat first; the
  streaming `vad_iter_feed` asks it only under `set_vad_stream_gpu` (`DASLLAMA_VAD_STREAM_GPU`, off),
  because one 32 ms chunk runs faster on the CPU chunk step than one command buffer's round trip.
  The device copy of the weights is `VadModel.slab`, the STFT basis and the LSTM's two planes
  transposed k-major, which `dasllama_layout`'s `vad_build_slab` lays at load.

- **`dasllama_audio_embedder.das`** - the audio carrier: `AudioEmbedder` / `AudioState`, the
  vision carrier's audio twin - one union through every seam (server media worker, facade
  `encode_audio`, tutorials), the family probed from the mmproj's audio tensor (or a `.dlim`'s
  baked tag) at load, one-line arms. It carries two kinds: the gemma-4 Conformer, and the
  whisper-class tower with its projector (`AudioKind.audio_tower`: Qwen2-Audio, Qwen2.5-Omni, Ultravox,
  Voxtral), whose family file `dasllama_audio_tower.das` wraps `dasllama_audio`'s tower for the carrier
  and encodes a clip one 30 s mel chunk at a time into its state's `EncoderState`. The
  projector says how its rows splice: an ultravox span takes no marker (`audio_span_bare`), since
  the stock decoder it pairs with declares none, and every other kind takes the markers the
  decoder's chat template declares. Outside a family's own file, an audio family type is named
  only here, in `dasllama_asr.das`'s union field and one-line arms (the ASR rail's own carrier),
  in the GPU family hooks (`dasllama_metal_tower.das`, `dasllama_metal_asr_dec.das`,
  `dasllama_vulkan_tower.das`, `dasllama_vulkan_asr_dec.das`, `dasllama_cuda_tower_g4a.das` - each fills the family's own
  seat with its tower and state - and `dasllama_tower_chains_g4a.das`, the gemma4a chain body both discrete homes' seats
  run, which reads the family's geometry and quirk constants to write its dispatch sequence once), in
  `benchmarks/asr/asr_bench.das` (the ASR bench pins a family serving knob for its correctness
  rail), in `utils/dasllama-convert/main.das` (the per-family `.dlim` bake tool dispatches on family by
  design), and in files under `tests/` - the set `REVIEW.das`'s `check_family_seams` enforces,
  its licensed namers (`AUDIO_SEAM_EXEMPT`) being `dasllama_asr.das`,
  `dasllama_metal_tower.das`, `dasllama_metal_asr_dec.das`, `dasllama_vulkan_tower.das`,
  `dasllama_vulkan_asr_dec.das`, `dasllama_cuda_tower_g4a.das`, `dasllama_tower_chains_g4a.das`,
  `benchmarks/asr/asr_bench.das` and `utils/dasllama-convert/main.das`.

### Text-to-speech front end

`ARCHITECTURE_TTS.md` carries this section - the TTS family mirrors the ASR one (a types floor,
a shared block home, a facade, one file per model family) and adds the text front end no ASR
family needs; every stage is data-driven from the model store, and nothing under `models/`
carries TTS data.

### Family GPU hooks install from the driver and always decline {#tower-gpu-hook}

A family file owns the hook SLOT for a stage the GPU can serve - a `var private` function pointer
plus a `register_*` entry - and a tower driver fills it at `[init]`: the Metal driver on a Metal
build, the Vulkan driver (`dasllama_vulkan_tower.das`) on a build without das_metal - its vision seats under the Vulkan home alone, the CUDA driver (`dasllama_cuda_tower.das`) filling them under the CUDA home (`ARCHITECTURE_GPU_TOWER.md#tower-reach`) - for the
blocks seats it serves and the front seats it fills (the whisper preprocessor's mel
`register_whisper_mel_gpu`, which Qwen3-ASR's mel and the chat towers' chunked mel both ask;
qwen3a's conv front, gemma4a's
whole chunk, canary's front, parakeet's whole encode - the Metal driver's, asked ahead of the CPU
front with the blocks seat behind a decline - and the whisper-class blocks-with-post-norm seat
`register_tower_blocks_ln_post_gpu`, which the whisper encode asks ahead of its CPU block loop and
post-norm - a decline asks no second seat, its driver's blocks seat declining the same way; both
tower drivers fill it, the post-norm one more row pass in the blocks' command buffer; the
seat serves the family whose post-norm follows the blocks with nothing between, whisper, while the
families that pool before it, and ultravox, take the blocks-with-tail seat
`register_tower_blocks_tail_gpu`, which `audio_encode` asks ahead of the blocks and the CPU
projector tail and which answers what it served - nothing, the blocks, or the blocks and the tail
(`TowerTailServed`); the Metal driver fills it). The direction is forced: the
driver requires the family file for its
types, so the family cannot require the driver back. A box with no driver leaves the slot empty
and the CPU form runs. A seat taken over a filled slot (a test's stub through
`register_styletts2_gpu` or `register_pocket_gpu`, a record of seats whose names and per-seat call
and serve counters are a `TtsGpuSeats` of `dasllama_tts_types.das`) gives the displaced registration back on
`unregister` - one level: the record keeps the registration it displaced, not a stack of them - so
the driver's seats survive the test.

Every hook answers "declined" in its own return - `false` for the block hooks and the whisper mel and qwen3a's
front hooks, `-1` for the hooks that return a row count (gemma4a's whole chunk, canary's front) -
so a decline is a fallback, never an outage, and the CPU form stays the reference. A family calls
its hook on either lane: the vision towers' chains read both lanes on both drivers
(`ARCHITECTURE_GPU_TOWER.md#tower-encode-chains`, `ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains`);
every GPU driver - Metal's included - serves both lanes of every audio family, and a Metal image whose
f16 lane finds no half tensor GEMM declines `device`, which under the driver's required mode panics - a
caller that pins a lane pins one its driver serves. While the stage-diff witness is
armed (`set_audio_encode_ref_dir`), the whisper-class tower skips its conv and block hooks and runs
the CPU forms: the witness diffs the CPU stages' rows against reference dumps, and a stage the
device served leaves nothing to diff.

A hook that serves past its seat - the gemma4a whole-chunk hook running the projector tail - says so
on the state it filled (`Gemma4aState.out_ready`), and the family's CPU tail reads the flag before
it runs; the flag is cleared before every hook call, so a hook that stops at its seat leaves the
CPU tail in force.

A family whose hook takes a whole stage splits its encode at the seam the driver needs: a
`*_stem_cols` / `*_window_frames` half both routes run (im2col or windowing, plus every buffer the
driver writes into), and a `*_stem_finish` half only the CPU route runs. The driver reads the
column buffer (`s.x0`, `st.xw`) and never the residual stream the CPU half would have filled.

Two hooks reach past the encoder. GigaAM's CTC head seat (`register_gigaam_gpu_ctc`, the Metal driver's) runs the
head's GEMM, argmax and log-sum on the device off the encode's held rows; the collapse stays on the host. The tower home's
`register_tower_rows_gen` answers the generation of the driver's last encode, which a family records on its state after
the encode (`enc_gen`), so the transducer seat finds that encode's rows on the device
(`ARCHITECTURE_MEDIA_ASR.md#rnnt-device-decode`).

### The tower weight lane is a policy, not a default {#tower-weight-lane}

A tower serves its GEMMs on one of two lanes: q8 planes (the CPU serving format) or the exact lane. The audio towers
take one lane set on every GPU home - Vulkan, CUDA and Metal - for the ASR families and the chat towers
(`load_audio_tower`) alike: q8 is the un-pinned default, a Q8_0 requant of the file's GEMMs at default levers
(`ARCHITECTURE_ENGINE_FORMATS.md#the-load-and-image-rail`); the exact lane is asked - by a pin or by `DASLLAMA_AUDIO_LANE=16` -
never defaulted, and its form on a GPU home is the
f16 lane - the file's GEMMs as halfwords, the image's only GEMM plane, f32 accumulation, its own image tag (`*-f16`,
`whisper-f16-q8`; `tower_lane_tag`, `tower_dev_halfword`). On Metal the f16 lane rides the half tensor GEMM: where the
Metal toolchain did not compile it (it carries no tensor ops), or `DASLLAMA_METAL_TOWER_F16=0`, the exact pick maps the CPU
image and the load says so in one line naming the family and the missing condition (`tw_half_pick`, the homes' shared
`tower_flavor_refused`); the q8 lane serves on that device.
The f32 planes are the exact lane on the CPU; a box with no GPU tower takes q8 for the ASR families, and the chat towers
keep their f32 load there. Un-pinned, no audio family but GigaAM asks a serving home which lane it prefers - the chat
towers ask only whether one exists. GigaAM and the vision towers ask: a GPU tower registered with its exact lane served
(`register_<family>_gpu`'s `f32_lane` - the Metal vision chains, whose f32 tiles are the faster lane) and answering its
serves hook true flips the default to exact (`*_gpu_exact_serves`). A driver whose exact lane needs a device capability
its q8 lane does not registers that answer too (`register_gigaam_gpu_exact`, the Metal driver's half tensor GEMM), so
GigaAM's default on a Mac whose toolchain did not compile it is the q8 lane on the device. A vision tower's exact form is its f32 tile on every home. An exact pin (`set_*_q8(false)`, the facade's `set_asr_fp32`; on whisper `set_asr_tower_fp32`, whose `set_asr_fp32`
pair keeps its f32 decoder on the CPU) lands on the home's form. Whisper carries no lane policy: q8 unless the facade's fp32
rails or `DASLLAMA_AUDIO_LANE` ask, on every home. Each family exposes one trio over one `GemmLane` pin, read through the audio lane seam
(`audio_lane_serves_q8`, `dasllama_tower.das`): `set_*_q8` pins a lane, `reset_*_q8` returns to the knob, else the policy,
`*_serves_q8` reports the next load's lane - the chat towers'
is `set_audio_tower_q8` / `reset_audio_tower_q8` / `audio_tower_serves_q8` (`dasllama_audio`). The lane picks the
image tag, so the lanes are separate images that coexist. A GGUF source of the GGUF tower families (the chat towers,
qwen3a, gemma4a) is read tensor by tensor through `gguf_read_tensor_f32` and re-encoded into the lane's form - Q8_0 on the
q8 lane, halfwords on a GPU home's exact lane, f32 on the CPU's - so a tensor stored in any type the reader decodes serves
on the home the flavor pick names, the CPU image converting it the same way. Where an audio tensor is stored quantized but
not as Q8_0 the load says so in one line (`tower_dev_flavor_announced`): the disk type, the `KqFmt` member `kq_fmt_of` maps
it to - or that no member serves it, where `kq_fmt_of` answers only its Q8_0 fallback - and the lane form it re-encodes into. A carrier holding its GEMMs quantized (GigaAM) has a third lane
the file decides: un-pinned it serves them as its own planes (`ARCHITECTURE_MEDIA_ASR.md#gigaam-planes`) whatever the GPU
policy says, and a pin dequantizes it into the pinned lane.

Pins exist for the arms that must not follow the box: the parity legs, the CPU board rows, and the facade's fp32 rail.

`DASLLAMA_AUDIO_LANE` is the environment's lane ask, read in that one seam for every audio family on every home and by no
vision tower, TTS family or LLM load. Precedence, highest first: a pin (`set_<family>_q8`, `set_asr_tower_q8`,
`set_audio_tower_q8`, the facade's fp32 rails), the knob (`8` the q8 lane, `16` the exact one), the family's policy.
Unset, or at any other value (a warning names it), every family keeps its policy. Whisper has no trio: the facade asks the
seam for its encoder (`whisper_tower_serves_q8`), an fp32 rail standing as its pin. The knob asks a lane and nothing more:
the lane's serving conditions are a pin's - an exact ask on a Metal box whose toolchain compiled no half tensor GEMM maps the CPU image - and a quantized GigaAM carrier keeps its own planes, which a pin alone dequantizes. It says
itself once a process where it moves a family off its policy.

Every family load logs its GEMM lane and the reason at LOG_INFO - the pin, `DASLLAMA_AUDIO_LANE` where it asked the lane
the load took, the serving GPU tower's lane, the accelerate tier, or the q8 default - because the lane changes what the load mints and serves. Where the
caller hands the lane in (parakeet, canary), the line names the caller's pick beside the reason
the policy gives, so a caller that overrides the policy shows as a pick its reason does not match. A load that takes a lane
the set knob did not ask - a named image, a lane the caller hands in, a quantized GigaAM carrier's own planes - says the knob
does not reach it (`audio_lane_unheard`).
