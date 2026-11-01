# dasLLAMA Architecture - ASR family mechanisms

Companion to `ARCHITECTURE.md`; a section is cited by its anchor. This document carries how the
ASR facade picks a family from a file, the oracle a family is specified by, the family's tokens,
language and long-form policy, the transducer decode step parakeet and GigaAM share and its device
loop (`#rnnt-device-decode`), and the GigaAM Conformer chain's own mechanisms - its log-mel, its
rotary input and its planes.

### The ASR facade picks a family from the file, in one order {#asr-carrier-sniff}

`load_asr_model` reads a path in a fixed order: a `.dlim` routes on its baked family tag
(`<family>-q8` or the exact lane's tag); a ggml bin routes on its magic, then on its vocabulary
size (whisper at 51864-51866, parakeet's SPM vocabularies at 1k and 8k); a GGUF routes on
`gguf_file_arch` when the file is the whole model (`gigaam`); any other GGUF is a decoder that
needs its mmproj, and the one-argument overload panics `pass its mmproj too`. The two-argument
overload routes on the mmproj - its magic (canary's encoder) or a GGUF mmproj's
`clip.audio.projector_type` - with the GGUF as the decoder. The arch read comes before the panic, so a one-file GGUF family loads through the
one-argument call; `asr_needs_mmproj(path)` answers the same question for a caller choosing which
overload to call (`examples/dasLLAMA/transcribe.das`). The bake tool picks the family in the same
order (`utils/dasllama-convert/convert_sniff.das`'s `sniff_asr_kind`). The lane the load passes is the caller's: the fp32 knobs
(`set_asr_fp32`, `set_asr_tower_fp32`) force the exact lane, else the family's lane policy
answers (`ARCHITECTURE_MEDIA.md#tower-weight-lane`).

### A family is specified by the upstream graph, dumped stage by stage {#asr-oracle}

A family's reference is the upstream package run on the CPU in fp32 activations, in a venv the
rig builds: `benchmarks/asr/requirements-<family>.txt`, a `ReferenceVenv` row in
`performance/setup_asr_rig.das` (`--venvs` makes `benchmarks/asr/.venv-<family>`), and the
interpreter resolved by `<family>_py()` in `performance/profile_common.das` under the
`<FAMILY>_PY` knob. `benchmarks/asr/<family>_oracle.py` writes one `.npy` per stage - mel, front,
every layer, the encoder output, the head's logits - and a `tokens.json` (ids, frames, text) under
`asr_oracle/<family>/<clip>_<variant>/` under the model root (`models_dir()`, beside the TTS
families' `tts_oracle/`), one set per carrier and head (`--variant`). The dumps are a local mint,
re-made by the script on the box that runs the external-reference cells; the clips they run on
are the corpus's (`benchmarks/asr/corpus/`, which the tests read through `asr_clip`).

The oracle rounds the weights the carrier stores at reduced precision through that precision
before the forward - GigaAM's encoder and head GEMMs and depthwise taps through f16, its subsample
convs, norms and biases kept f32, as its converter writes them - because the carrier's top
precision is what the das chain serves. The exact lane is then the upstream graph over the same
weights, and its per-stage bar is fp32 rounding, about 1e-4 relative; the q8 lane holds the
tower's q8 tolerance. An oracle that skips the rounding sits a carrier's quantization away from
both lanes, and the bar measures the file, not the code.

A `.npy` is a header dictionary, its length the u16 at byte 8, then raw little-endian f32 from
byte `10 + length`. The header's `fortran_order` says the layout: numpy writes a transposed view
column-major, and GigaAM's `front.npy` and `layer_NN.npy` are Fortran-order where `mel.npy` and
`encoded.npy` are C-order, so a reader honors the flag or compares a transposed stage.

### Tokens, language and long form are the family's {#asr-tokens-long-form}

A transducer or CTC vocabulary rides the image meta as strings. SentencePiece pieces detokenize
through the one shared `asr_spm_detokenize` (`dasllama_asr_types.das`; parakeet and GigaAM's spm
heads call it): the word-start marker U+2581 becomes a space, the first piece's leading spaces are dropped, and a `<...>`
control piece renders as nothing. A character vocabulary joins its
symbols as they are; an LLM decoder's BPE is the decoder `Model`'s own.

A family declares its languages in `AsrCaps` and validates a language in its own file.
`create_session(m)` defaults `lang` to `""`, the model's default, which the facade alone decides:
the one code of a caps sheet that lists one (GigaAM: `ru`), `auto` for a sheet that lists none (the
model detects the language), else `en`. The CLI, the server and the examples pass `""` through. The board's `asr_catalog()` row carries the
clips' language in `lang`, which `benchmarks/lcpp_bench.das` reads to drive the ASR cells.

A clip past the encoder's whole-clip window is the family's to split. GigaAM encodes up to 25 s
whole and, past it, splits at silero-VAD silences (`dasllama_vad.das`) into spans of at most 25 s,
encodes and decodes each alone, and emits one segment a span with `t0` / `t1` from the span's
bounds in centiseconds. The reference package splits a long clip with a separate segmentation
model, which the oracle does not reproduce, so the long clip's check is word agreement against the corpus's
reference text rather than a token compare.

### GigaAM's log-mel frames start at the span's first sample, with no center padding {#gigaam-log-mel}

`gg_log_mel_span` cuts frames of `n_fft` samples (320 on v3) at the file's hop, starting at the span's first sample with no
center padding, so a span of n samples gives `(n - n_fft) / hop + 1` frames and none when n < n_fft. Each frame takes the
file's window, then the DFT as one GEMM against the `build_dft_twiddles` table, the power spectrum, the file's filterbank
and `ln(clamp(mel, 1e-9, 1e9))`, written frame-major into `st.mel`. This is the graph the oracle's `mel.npy` dumps
(`#asr-oracle`). A centered STFT (whisper's) moves every frame by half a window and gives a different frame count.
The transcribe's span encode (`gigaam_encode_span`) hands the GPU encode hook the raw samples, and both tower drivers
cut the same mel on the device ahead of the conv front, in its command buffer: the window product
(`GkFrameWindow`) is the CPU frame loop's, and the filterbank (`GkMelLog`'s clamp form) sums in float and takes
`ln(clamp(sum, GIGAAM_MEL_FLOOR, GIGAAM_MEL_CEIL))`, so the device mel reads within an ulp of the clip's loudest
value. `st.mel` then stays as it was; a declined hook cuts it on the CPU.

### GigaAM rotates the normed rows before the q and k projections {#gigaam-rotary-input}

GigaAM v3's rotary attention applies the rotation to the block's LayerNorm output, viewed per head (16 heads of 48,
NEOX half-split, base 5000, positions from 0), and only then projects: q and k are the biased linears over the ONE
rotated copy, v the biased linear over the unrotated rows. The model's attention takes the same normed rows as query
and key, so a single rotated buffer serves both GEMMs, and on the q8 lane one requant of it
feeds q and k while v requants the unrotated rows. Rotating q and k after their projections - the decoder-LLM placement
- is a different function of the weights, and its first block already misses the oracle's dump by orders of magnitude.

The scores take `attention_bidir` unmasked over the whole clip at scale 1/sqrt(48): a single clip has no padded keys.

### GigaAM serves its block GEMMs as the carrier's halfwords, q8, or its own quantized planes; its taps and CTC head as halfwords, and its front f32 {#gigaam-planes}

The f16 carrier holds the block GEMMs, the depthwise taps and the head GEMMs in f16 and the two subsample convs, the norms
and the biases in f32; a quantized carrier (the hosted `q4_k`: the attention and FF GEMMs Q4_K, the conv pointwise GEMMs
Q8_0) holds the block GEMMs in ggml's block formats and the rest as the f16 file. The ten block GEMMs a layer sit at a fixed
per-layer stride in layout order (ff1 pair, q, k, v, out, pw1, pw2, ff2 pair), in the plane the lane picks (`gg_gemm_plane`):
the exact lane copies the carrier's own f16 halfwords into `wblob` (`gemm_f16`), the q8 lane quantizes them to Q8_0 into
`qblob`/`qscales`, repacked a region at a time for the load's backend. A carrier whose block GEMMs hold no f16 serves them
f32 in `blob` on the exact lane, and one that mixes f16 with another type among them is refused naming both tensors - one
plane serves a region.

A carrier whose every block GEMM is quantized (`gg_kq_carrier`; a mix of quantized and float GEMMs is refused naming
both) takes the kq lane un-pinned (`kq_lane`; the image tag `gigaam-kq`), where each GEMM serves as the file's own
planes: a K-quant tensor transcoded into the byte planes `kq`/`ks` (`gguf_transcode_kq_at`, the engine's superblock
planes) and repacked for the load's backend where it carries kq kernels (`kq_repacked`), a Q8_0 tensor's blocks moved
exactly into `qblob`/`qscales`. The GEMMs pack compactly, each at its own offsets, so the chain's record keeps its
virtual element offsets (`gemm_off` 0, the fixed stride) and the model carries one seat a GEMM in the `gemm_seats` plane
(`TOWER_SEAT_WORDS`: the format slot, the quant offset, the scale offset); `fc_gemm_seat_index` maps a record offset to
its seat, `tower_gemm_seat` reads it, and `tower_mm` runs a K-quant seat on the backend's batched kq tile over the Q8_K
activation image (`tower_requant` - the per-superblock scales in `xs`, the per-16 sums in `xbs`), the portable disk-order
batch GEMM (`matmul_kq_batch_disk_at`: the weight row walked once a position while it is hot) where the planes stayed in
disk order, the repacked GEMV a position where a backend repacked them but carries no batch tile, and a Q8_0 seat on the q8
batch GEMM over the Q8_0 image. The attention's q, k and v read one image, so the planner holds their formats to one class; a GEMM's format is
its disk type's tag on its row (`kq_tag`), and a type the lane has no seat for - a per-32 member, NVFP4, a demoted row -
is refused naming the tensor, the pinned lanes being the way to dequantize it. The lane's name is its one K-quant member
(`gigaam_exec_fmt`: `k4`), `kq` where the members mix, `q8` where every GEMM is Q8_0 - that carrier serves its blocks
exactly where the q8 lane would requantize them. Pinned (`set_gigaam_q8`, the facade's fp32 rails), a quantized carrier
dequantizes into the pinned lane's planes, as the f16 carrier would. f16 depthwise taps
(`dw_f16`) sit in `wblob` on both lanes, behind the block GEMMs where they are there, every layer's [k][d] at `dw_off`; a
carrier whose taps mix f16 with another type is refused the same way, and one whose taps hold no f16 keeps them f32 in
`blob`. An f16 CTC head (`ctc_f16`) sits in `wblob` on both lanes behind them: its rows padded with zero rows to
`GIGAAM_CTC_ROW_TILE`, the device GEMMs' 64-row column tile, so a device GEMM reads the plane in place; the mint writes the
pad as the plane's zero tail. Everything else - the front's im2col GEMMs, the norms and biases and the transducer - is f32 in
`blob` on both lanes. The conv weights are permuted at mint to tap-major columns (`[out][tap][in]`), so the im2col copies
whole input rows; the f16 taps move as halfwords, none converted.

The CPU GEMM over `wblob` (`mm_f16_b`, `matmul_f16_batch`) widens a four-row group of halfwords into the lane's walk
panel and runs the f32 tile and the f32 `dot` over it on the f32 walk's lattice; the widen is exact, so every output is
the f32 plane's to the bit at half the weight bytes, and the exact lane is the upstream graph over its f16 weights to
fp32 rounding. The CPU CTC head reads its halfwords through the same GEMM. The
CPU depthwise conv over f16 taps (`fc_dw_bias_rows_f16`) widens a layer's [k][d] taps once a call into a scratch the call drops,
then runs the f32 loop, so its rows are the f32 taps' to the bit; the GPU stamps read the halfwords in the kernel
(`GkDwConvRows` TAP_F16: `MetalGaDwH`, `TowerGaDwH`), the bias row staying in the f32 plane. The Metal FastConformer chain binds `wblob` as
its halfword GEMM plane and reads it through the half tensor GEMM alone - the f32 blob holds no block GEMM, so a box
without the half tensor crown, or under `DASLLAMA_METAL_TOWER_F16=0`, declines the chain and the CPU serves; the front's
conv stays on the f32 tile over `blob`. On the q8 lane the chain uploads every block's Q8_0 regions once a model into one
device slab (`tw_fc_q8_attach`, the whisper-class towers' slab form) and runs each GEMM on the prefill q8 ladder
(`pf_enc_q8_mm`); the mel, the front, the CTC head and the transducer decode serve on the device on both lanes. Neither
GPU chain has a GEMM over the kq lane's planes: each declines that lane by name (`kq_lane`, the members in the words) and
the CPU serves its blocks, the mel and the heads keeping their device seats. The Vulkan
FastConformer chain serves both other lanes, the exact lane's GEMMs on the f16 GEMM over the uploaded halfwords
(`ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary`), the f16 taps uploaded as the resident's own halfword plane
(`taps_dev`) on both lanes.

The un-pinned lane follows the device's faster lane (`ARCHITECTURE_MEDIA.md#tower-weight-lane`): the Metal driver
registers the exact lane as its faster one; the Vulkan driver registers the q8 image as its faster one until the two lanes
are timed on a Vulkan box, `DASLLAMA_VK_GIGAAM_EXACT` (`set_vulkan_gigaam_exact_lane`) flipping it. Both lanes serve on
both devices, so the device-lane report (`gigaam_device_gpu`) names the device on either.

The mint streams (`gg_mint`): `stage_gigaam_model` reads the hparams, the mel planes and one fill job a tensor
(`GigaamFillJob`: its plane, offset and count), and the mint writes each weight plane off the mapped carrier one tensor
at a time (`gg_stream_plane`) - a halfword plane straight off the mapping, an f32 or q8 tensor through a scratch the size
of the largest tensor - so the load and the mint hold the mapping and one tensor, never the model. A staging asked to
`hold` its planes reads them whole first, for a control that edits a plane before the mint.

On a Metal build the CTC head serves on the device on both lanes (`register_gigaam_gpu_ctc`): an f16 head's half GEMM
(`pf_enc_hmm`) over its halfwords in `wblob` and the encode's held rows converted to half (else `st.x` uploaded) - a box
without the half tensor crown declines the device and the CPU serves the halfwords - or an f32 head's f32 GEMM over its
rows copied to a panel padded to the 64-row tile; then one pick kernel (`MetalCtcPick`) that adds the bias off `blob` and
writes each frame's argmax (the lowest index of a tie, as the CPU's; an all-NaN row picks 0) and the emitted class's
-log sum exp, two words a frame read back; the host collapses repeats and blanks.

On a Vulkan build the CTC head serves on the device on both lanes too (`vulkan_gigaam_ctc`, the ASR-decoder driver's CTC
seat): the head's planes uploaded once a model as the image holds them - an f16 head's padded halfword rows off `wblob`, an
f32 head's rows off `blob` - and the bias row; the encoder rows the FastConformer chain left on the device for the encode's
generation (`vulkan_tower_fc_rows`), else `st.x` uploaded; an f16 head's half feed (`TowerClampCvt`) and the f16 GEMM over
its rows (the cm2 tile or the KHR one), an f32 head's f32 tile (`F32Gemm`); then the pick, the same body as Metal's
(`GkCtcPick`, stamped `CtcPick`). The seat rides the tower knob and declines by name (`vulkan_ctc_declines`: `knob`, `shape`
- d off the f16 GEMM's 64 lattice -, `device` - an f16 head on a device with no f16 GEMM tile -, `memory`); a decline leaves
the head to the CPU.

### The transducer decode step is one team publish over half-precision weights {#rnnt-decode-step}

`rnnt_greedy` decodes a TDT head (a duration table, its logits behind the tokens') and a plain
RNN-T head (no table: a blank advances one frame, a token stays up to `max_symbols`) off an
`RnntNet` - offsets into the family's f32 plane - and encoder rows the family projected through
the joint's encoder half, a GEMM its own weight lane serves. A step is a few hundred thousand
multiply-adds a lane, so the bytes it reads and the times it meets the team set its time. Both
lanes decode off HALVES `RnntState` lays once a weight plane - the LSTM stack and the joint's
prediction projection (`wh`, gate rows repacked i, f, o, g, the biases summed into `bias`), the
joint net transposed into logit blocks (`joint_wt`: a block of `RNNT_JOINT_BLOCK` logits holds
its input columns back to back, so a lane walks its block forward; a lane's slice of a whole-row
column table is short runs a column apart, which the prefetcher does not follow); `dasllama_repack`
holds both layouts, the state the cache. The halves read
through `dot_f16` and `axpy_f16`: a parakeet token's predictor reads 13 MB where f32 was 26.

`rnnt_step` runs a step as ONE `team_parallel_stages` publish: a layer's gate dots (the input's
and the hidden's dot of a row summed in one pass), its gate math, the next layer's pair, the
joint's prediction projection, then the joint's logits a block a chunk - two stages a layer and
two behind them, so the publish's six-stage limit caps the predictor at `RNNT_MAX_LAYERS` (two).
A blank step publishes the logits stage alone. Each lane applies the joint's ReLU itself: a
column whose `enc + pred` is not positive is skipped, about half of them. A lane meeting the team
costs about what 128K multiply-adds of a stage's work earn it, so `rnnt_greedy` narrows the pool
for the decode to the widest stage's work over that (`rnnt_step_lanes`, `dispatch_lanes_narrow`):
GigaAM's 320-wide predictor takes six lanes, parakeet's 640-wide one the whole pool. Under one
dispatch lane the stages run in order on the caller with no publish. The token probability's log-sum leaves out a logit `RNNT_LSE_CUT` (32) under the row's
maximum: N such logits are at most N x e^-32 of the sum, under 1.1e-10 on the widest row served
(parakeet v3's 8198 logits).

### On a GPU build the whole greedy loop runs on the device, in rounds {#rnnt-device-decode}

`rnnt_decode` asks the device hook (`register_rnnt_gpu`) ahead of the CPU loop; the Metal and Vulkan ASR-decoder drivers fill
it with one form over four kernels both homes stamp (`GkRnntPred` twice, `GkRnntJoint`, `GkRnntPick`). The joint's encoder
projection is the home's exact f32 GEMM over the encode's own device rows - the tower holds a transducer family's last
FastConformer encode's residual rows under a generation each encode bumps (`metal_tower_enc_rows`, `vulkan_tower_fc_rows`),
the family records that generation on its state after the encode (`tower_rows_gen`, `enc_gen`) and hands it to the
seat (`RnntEnc.gen`), so a CPU encode, another session's encode, or a freed array's address never names the held rows -
or over rows uploaded where the CPU encoded; a lane whose projection lives in a q8 plane (parakeet's
q8 lane) projects on the CPU and the device decodes off the result. The weights are the CPU state's halves and summed biases,
laid once a model: the seat keys its copy on the net's plane, offsets and lay flags, the encoder half, the small regions'
values whole and a strided sample of the large ones' (`rnnt_dev_key`), so two sessions of one model share it and a net laid
at a freed plane's address with other values uploads its own. Everything but the buffers and the dispatches is one host form
in `dasllama_rnnt` both drivers call - the admission (`rnnt_dev_shape_ok`), the slab layout, the kargs, the start words, the
rounds loop and the record decode.

The loop's control flow lives in a state buffer the kernels read: a round is each layer's gates (gated on the state's `need`
word), joint.pred, the joint over a window of 8 frames from `t`, and the pick, which walks the window as the CPU loop walks
frames - a blank advances by its duration inside the window, a token lands its record, asks the next round's predictor step and
lays its embedding row as the next input; a spent window leaves the predictor standing, so a run of blanks costs one round, not
a predictor step a frame. A round past the clip's end is four dispatches that exit at once. A command buffer carries the rounds
the frames left take at the rate the clip has shown, the last clip's rate before its first; the host reads the state words a
command buffer and the token records once. A token round emits one of at most `max_symbols` a frame and any other round spends
its window or ends the clip, so a clip takes at most n_frames x (`max_symbols` + 1) rounds; a loop past that budget with the
clip not done declines (`overrun`) and the CPU loop decodes, as it does after a failed command buffer. The pick's argmax keeps the CPU's first index of a tie, and its duration argmax
compares exp(logit - max) as the CPU's float rounds it, the subnormal range a tier of its own, so a duration the CPU flushes to
zero ties at index 0 on both.
