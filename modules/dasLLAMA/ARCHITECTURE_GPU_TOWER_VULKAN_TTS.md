# dasLLAMA Architecture - the Vulkan tower's TTS seats

Companion to `ARCHITECTURE_GPU_TOWER_VULKAN.md`; a section is cited by its anchor. This document
carries the StyleTTS2 synthesis seats the Vulkan TTS driver serves. The Metal twin of every seat
is `ARCHITECTURE_GPU_TOWER.md#tower-tts-chain`, and the CPU chain is the specification, dispatch
for dispatch. The GPU backend role table these sections build on stays in
`ARCHITECTURE_GPU.md#gpu-backends`.

### The StyleTTS2 seats on Vulkan {#vk-tts-chain}

The kitten and kokoro families' synthesis rides the Vulkan tower driver's knob
(`DASLLAMA_VK_TOWER`) through the family's hook record (`ARCHITECTURE_MEDIA.md#tower-gpu-hook`),
one seat per stage, registered on a build without das_metal. A seat the driver does not fill
stays on the CPU chain, and the parity rail skips its stage loudly, so a driver ported seat by
seat is honest about the rest.

Every seat computes in f32. The part's weights live in a slab the shared host writer lays out
(`dasllama_tts_slab.das`): every conv or linear dense as [cout padded to 64 x k*cin padded to 32]
with column j = tap*cin + ci and its bias row beside it, every norm as a scale and a shift row,
an LSTM direction as its input linear and its dense recurrence; the q8 lane's quants and a
K-quant linear are dequantized into the slab through the active repack, so the served lane's
numbers land. The slab uploads once to one device buffer and stays resident under a key that
folds the part's weight addresses and the q8 lane's repack layout; the model-drop sweep releases
it, and a reload or another lane keys differently and rebuilds. The seats' activation rows are
a scratch per seat, one device buffer a slot sized in floats and grown in 256 KB steps, kept
across calls and rebuilt only when a call needs a slot wider than it holds.

The front end - the PL-BERT encoder, the text encoder, the duration encoder, the duration head
and prosody - runs on the f32-exact tile GEMM (`F32GemmT`: 64 positions by 64 weight rows a
workgroup on plain f32 workgroup tiles, every product one mad in k order, the edges masked, the
bias added at the store) so the durations round as the CPU's and the F0 drives the same phase.
The albert seat is the embeddings gathered from three slab planes (`TtsRowGather`), the
embedding norm, the map-in, then the one ALBERT layer n_layers times: q, k and v projections,
the bidirectional f32 attention row (`TtsAttn`, one workgroup per query row and head, softmax
the CPU way), the dense projection, the residual add with the attention norm, the ffn, the
exact tanh GELU (`TtsGeluTanh`), the ffn output, the residual add with the full norm - the
residual seams on the tower's post-add layernorm stamp (`TowerPostAddLn`: the branch linear
runs the unbiased tile and the seam adds its bias row, then the norm of the sum, as the CPU's
post-LN block orders them), the norms on the tower's layernorm stamp reading the slab's rows.
The rows come back to the host through the scratch's own readback buffer; one seat call is one
submit.

The text seat runs on rows [t][c] where the CPU chain runs channel-major: the embedding rows
gathered (`TtsRowGather` with the position and type rows switched off), then per conv the
im2col over the rows form (`TtsIm2col`, column j = tap*cin + ci, the slab's weight-row order)
and the biased tile over the slot's rows, the layernorm over the row (the CPU's channel
layernorm per position) on the tower's layernorm stamp, and the leaky ReLU (`TtsLeaky`), the
conv stack ping-ponging two row sets as the CPU chain does; then the BiLSTM as two directions,
each its input gates on the biased tile ([t][4H], gate order i, f, g, o) and one workgroup
walking the recurrence (`TtsLstmDir`, the backward direction writing the second half of the
shared [t][2H] rows). The rows read back and transpose on the host into the channel-major
[c][t] the CPU stage answers. Every seat's scratch is one float-count list, a slot a row set,
the seat's own slot table naming them, plus two more row sets - the aux rows a call uploads
(the style vector, then fc(style) per norm: gamma then beta, gamma + 1 where a layernorm reads
it as its weight) and the AdaIN column stats the device writes.

The predictor's three seats share one slab (`tts_pred_write`). The duration encoder: the BERT
projection on the biased tile, the style appended as a broadcast residual of stride 0
(`TtsConcat`), then per LSTM the BiLSTM, the AdaLayerNorm as the layernorm stamp reading the
aux rows, and the style appended again; the rows come back [t][c + style]. The durations: the
BiLSTM, the projection, then the sigmoid sum a token over speed (`TtsSigSum`), one float a
token back. Prosody: the encoder rows transposed on the host into the rows form, the shared
BiLSTM, then each branch's residual blocks pinging between two row sets - the AdaIN as the
column stats (`TtsColStats`, one workgroup a channel) and the fold with the leaky ReLU
(`TtsAdainLeaky`, the CPU `adain_affine`'s scale and shift), the upsampling block's depthwise
transposed pool (`TtsPoolDw`) and its nearest shortcut as the row gather over a r / 2 map, the
convs on the im2col and the biased tile, the learned shortcut the same, the join as the scaled
add (`TtsAddScale` at 1 / sqrt 2) - and the branch's one-channel projection; F0 and energy
read back a float a frame.

The decoder's two seats share one slab (`tts_dec_write`) and one scratch. The decode seat: the
F0, energy and text front convs on the im2col and the biased tile, the encode block over the
concat rows (`TtsConcat` laying the text rows, the residual rows and the two curves as columns),
each decode block re-concatenated where its width asks, the stream pinging between two row sets
and landing in the first; then the source - the noise rows drawn on the device (`TtsSrcNoise`,
a counter-keyed normal, not the CPU's PCG32 stream) unless the call carries a captured stream,
the phase increments and the phase per frame on the resample law's stamps (`TtsSrcLowTorch` /
`TtsSrcLowOnnx`, `TtsSrcCumsumTorch` / `TtsSrcCumsumOnnx` - the torch law's double
accumulator as a compensated two-float sum, the kernel marked `precise` so the device compiler
keeps the correction terms it would otherwise fold to zero), the harmonics' sines at the interpolated phases
through the source linear and its tanh (`TtsSrcSinesTorch` / `TtsSrcSinesOnnx`, the sine of a
phase past a hundred thousand radians reduced by 2 pi in exact pieces), and its spectrum rows
on the pad law's stamp (`TtsStftReflect` / `TtsStftEdge`: the magnitude and the phase); then
the generator - per stage the leaky ReLU, the source rows through the stage's noise conv and
Snake residual block (`TtsAdainSnake` on the norm slot's alpha row), the transposed upsample
conv on the im2col's transposed read, the last stage's reflected row (`TtsReflect1`), the
kernels' blocks averaged into the stage's out (`TtsAxpy`), then the final leaky and conv_post -
and the inverse STFT (`TtsIstft`, the log magnitudes and phases through the transposed conv
weights, the window envelope divided out where the model asks). The waveform reads back. The
generator seat runs the stages and conv_post alone and reads conv_post's rows back for the
host inverse STFT, as the Metal twin does.

The Pocket family's two seats ride the same driver and knob, registered through
`register_pocket_gpu`. Every Pocket linear is f32 rows in the slab - a q8 or K-quant file
dequantized through the active repack at slab time; the driver carries no q8 blob route, so the
served lanes read the weights at f32 where the CPU chain reads quants. The codec seat is one
submit over the whole run (at most 512 frames; a longer chunk declines by shape and the CPU's
windows serve): the latent projection and every codec conv on the im2col in the CPU's
first-window form (a forward conv with the causal pad k - stride, a transposed one over t x
stride rows with no pad) and the biased tile, the depthwise transposed upsample (`TtsPoolDw`),
the transformer as the CPU's layer loop - the layernorm into a copy, the qkv projection, the
NORM rope on the q and k spans against the slab's tables (`TtsPkRope`), the k and v columns
copied to one row set the layers reuse (`TtsPkRows`), the causal attention over the context
window (`TtsPkAttn`, a workgroup a head and query row, the head size the body's literal 64),
the output projection, the layer scale (`TtsPkRowScale`), the residual seam on the post-add
layernorm stamp against the slab's zero row, the ffn with the tanh GELU - then dec_in, per
ratio the ELU (`TtsPkRowsElu`), the transposed upsample and the ELU-conv-ELU-conv residual
block, the last ELU, dec_out and the sample column copied out. The frames seat: the voice's
K/V rows live on the device per backbone layer as [cap][d] rows under a key over the host
caches, with the rope tables for every position they can hold; the chunk's text rows come up
before the loop; each frame is the CPU's `frame_step` and `head_step` at t = 1 - the input
linear, per layer the seam, the qkv row, the rope at the frame's position, the k and v rows
stored at that position, the attention over the cache, the projections and scales, then the
output norm's row copied to the frame's conditioning row, the EOS logit, and the flow head as
the row GEMV family (`TtsPkGemv`: four rows a workgroup, x staged, the dot in lane order; the
bare dot, the layernorm-modulate-SiLU prologue, the gated residual, the add-SiLU over the slab
vector, and the tail that adds the noise row and denormalizes the latent). The frames run in
batches of eight a submit (`set_vulkan_pocket_frame_batch`), the EOS rule walked on the host
between batches from the logits read back, the generator rewound past the frames made, as the
Metal twin does.

The declines: `knob`, `shape` (a width off the 64 lattice, a head width other than 64 or 128,
more than 512 tokens for the attention stage, an LSTM direction over 256 hidden), `device` (the
tier has no device, or a class failed to build), `memory` (a slab or scratch allocation failed),
`gpu_error`. Engage is `vulkan_tts_stats`' `encodes` beside the family's own seat counters
(`styletts2_gpu_stats`); the parity instruments are the per-stage cells on identical inputs on
the f32 lane (`tests/_tts_parity.das`, driver-neutral over the Metal and Vulkan knobs) and the
kernel cells of `tests/test_vulkan_tts_kernels.das` and `tests/test_vulkan_tts_conv_kernels.das`.
