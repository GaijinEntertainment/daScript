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
a scratch sized once for the widest call seen, in steps of 256 positions, kept across calls.

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

The declines: `knob`, `shape` (a width off the 64 lattice, a head width other than 64 or 128,
more than 512 tokens for the attention stage, an LSTM direction over 256 hidden), `device` (the
tier has no device, or a class failed to build), `memory` (a slab or scratch allocation failed),
`gpu_error`. Engage is `vulkan_tts_stats`' `encodes` beside the family's own seat counters
(`styletts2_gpu_stats`); the parity instruments are the per-stage cells on identical inputs on
the f32 lane (`tests/_tts_parity.das`, driver-neutral over the Metal and Vulkan knobs) and the
kernel cells of `tests/test_vulkan_tts_kernels.das` and `tests/test_vulkan_tts_conv_kernels.das`.
