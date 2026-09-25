# dasLLAMA Architecture - the tower attention routes and encode chains

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2w-2.2y - the three routes that serve tower attention on Metal, the
one-command-buffer encode chain each family the Metal tower driver serves gets, and the StyleTTS2
synthesis chain. The GPU backend role table these sections build on - the tower driver's role row
included - stays in `ARCHITECTURE_GPU.md` sec.1.5.

- `ARCHITECTURE_GPU_TOWER_VULKAN.md` - sec.2.2aq-2.2ar, 2.2at: the Vulkan tower driver's row
  classes and attention routes, and its encode chains, and the Vulkan ASR-decoder driver.

### 2.2w The tower attention routes {#tower-attn-routes}

A tower head width is padded to `hs_pad = max(64, ceil32(hs))` - 72 and 80 both land on 96 -
because the QK stamps walk multiples of 32 while the guarded AV forms carry no multiple-of-64
requirement. A route that uses the padded width pays a restride to and from the towers' compact
rows. Three routes serve tower attention:

- **The flash route** takes a head size of exactly 72 on a canvas whose row count divides 64.
  It runs one pass - no score slab, no rowstat, no head restrides - reading K and V as
  per-head-contiguous f16 panels (the panel and tail-slack contract sits on the kernel class)
  while Q and the output ride the compact f32 rows. `DASLLAMA_METAL_TOWER_FLASH=0` pins the
  slab trio.
- **The per-window route** serves the qwen2.5 ViT's block-diagonal layers: one threadgroup per
  (window, head), one thread per query row, windows capped at 64 rows. It reads q, k and v
  compact and straight from device in f32 - a window's K and V sit in L1, and f16 staging noise
  would compound over the tower's 28 layers. Layers that attend in full restride onto the
  padded slab and take the shared trio.
- **The slab trio** - the prefill driver's QK, rowstat and AV builders - serves everything else
  and sizes its score slab at `heads x mp x nk64` halfs. Off the flash route that slab is the
  encode's only attention allocation; on it the driver takes a stub.

### 2.2x The tower driver's encode chains {#tower-encode-chains}

Each family the tower driver serves gets one chain that encodes the whole encode into ONE
command buffer and reads back once: the block loop, and - for gemma4a and qwen3a - the mel and
conv front ahead of it. A chain is dispatch for dispatch the same graph as its family's CPU
encoder loop, in the same order and at the same operand shapes, so the CPU loop is the chain's
specification and the CPU-vs-GPU transcript cells are its parity instrument. Every chain is
best-effort: it answers false (or -1) on any shape, knob, quant-mode or device decline, and the
CPU chain serves that encode. Engage is read from counter deltas (`metal_tower_stats`,
`metal_tower_f16_encodes`), never from "the model ran".

The FastConformer chain (canary and parakeet share it: one context, one block body over the
canary offsets record, parakeet's offsets mapped onto it with no GEMM biases and its tap-major
depthwise stamp) runs the rel-pos (Transformer-XL) attention one head at a time on the f32 GEMM
builder, because a per-row kernel walking every key is the encode's cost past a minute of audio:
a pack writes the head's (Q+u), (Q+v), K_h, V_h^T and pos_h panels contiguous and zero-padded to
the GEMM lattice (rows to 32, keys and rel positions to 64), two GEMMs give the content scores
(Q+u)*K_h^T and the rel scores (Q+v)*pos_h^T, one row kernel adds the rel score at the CPU form's
shift (npos - 1 - q + k), scales, and takes the softmax with zeros in every pad, a third GEMM
gives P*V_h into the head's slot of the AV rows, and one unpack per layer writes the compact
output. The score slabs are per-head scratch reused head after head - each head's writer is
encoded after the previous head's reader. The head size is held to a multiple of 64 (the AV
GEMM's column lattice) and the head count to the per-head uniform seats.

### 2.2y The tower driver's StyleTTS2 synthesis chain {#tower-tts-chain}

The whole StyleTTS2 synthesis of the kitten and kokoro families rides the tower driver as seven
seats of the family's hook record (`ARCHITECTURE_MEDIA.md` sec.2.14): PL-BERT, the text encoder,
the duration encoder, the duration head, prosody, the decode seat and the generator seat. Each
seat is one command buffer of 2.2x's shape over a per-part weight slab the driver builds once
and keys on a fold of the part's weight addresses - the address of the form each conv serves, the
q8 quants or the f32 operand - and of the q8 lane's active repack layout, so a reload serving
another lane or another layout keys differently; the addresses stand for the model because a
reload never reuses them before the weights epoch drops every slab, which shutdown does too. The decode seat takes the aligned features, F0 and N up and runs the front convs, the
AdaIN residual blocks, the harmonic source, the generator and the inverse STFT as ONE command
buffer, the samples back; the generator seat behind it - reached only by the CPU chain a
declined decode falls into - runs the generator through conv_post as one command buffer and reads
conv_post's rows back for the CPU's inverse STFT (`styletts2_istft`). The CPU chain is the specification, dispatch for dispatch, and every
seat serves both weight lanes: the q8 lane's stacked quants are read row-major through the
active repack's gather and dequantized into the slab, so the lane policy does not flip for the
tower.

Every conv is the f32 tile GEMM over the slab - each conv dense as [cout padded to 64 x k*cin
padded to 32] with column j = tap*cin + ci and its bias row beside it. A conv whose channel
count is a multiple of 8 runs the GEMM's gathering instance (the `CONV` stamp): the X loader
reads each row's 8-run of k through the conv's geometry, forward or transposed, so no im2col
is materialized; the noise convs (22 channels) take the im2col kernel in 32-aligned row chunks,
each chunk's column buffer under a 64 MB ceiling (`ST2_IM2COL_CHUNK_MAX_BYTES`).
The GEMM template stamps twice more as the f32-EXACT twins (`MetalF32ExactMm`, `MetalSt2ConvExactMm`: float tiles,
twice the threadgroup memory) and the FRONT END - the five seats before the decoder - runs on
it, because its outputs round to integer durations and its F0 drives a phase (the f16-staged
stamp moved one raw duration in a sentence across a half; on the exact stamp the durations
agree token for token and every front-end stage reads within 3e-6 of the CPU's). The ALBERT
attention is its own f32 row kernel, one simdgroup a query row with the lanes over the keys
(the online softmax merged across the lanes). The decoder and generator keep the f16-staged
stamp (the decoder rows read within 7e-3 of the CPU's on kitten-nano, 5e-4 on kokoro; the
seam form of the generator alone within 1.2e-3).

The harmonic source is the CPU's `sine_source` on the device, operation for operation (sec.2.33
of `ARCHITECTURE_TTS.md`): the three source kernels compile without fast math, the resample taps
and mixes follow the reference's law - the torch law's double source coordinate as one fma, the
ONNX law's seven-term trilinear mix - the cumulative phase runs as the torch law's double
accumulator (a two-float sum in the CPU's frame order, every add's exact error carried) or the
ONNX law's plain float sum in that order, and the sine of a phase in the hundred thousands of radians reduces it by 2 pi in four
exact-product pieces. The mirror holds to a few ulps, not bit for bit: Metal contracts a
product-sum into an fma where the CPU rounds twice, and its sine is the fast form. The mixed
signal reads within 4e-7 of the CPU's; a phase off by one f32 ulp would read at the percent level.
A chunk past 2^23 samples declines, where the torch law's tap coordinate parts from the CPU's double form. The BiLSTMs run one direction a threadgroup (the input
gates from the GEMM, the recurrence through threadgroup memory; `MetalSt2Lstm` has no
specialization path - `hidden` travels in `St2LstmArgs` as the gate-block stride into its
threadgroup gate array, capped by `ST2_LSTM_MAX_HIDDEN`); AdaIN and its activation are
one fused row pass after a per-channel stats reduction; the style's fc rides up per site in
one buffer the CPU fills as the chain encodes.

The declines: `knob`, `shape` (a width off the 64 lattice, a head over 128 wide, an LSTM over
the threadgroup's lanes, a source branch whose length disagrees with the upsampled stream, a
residual conv that changes the length, more than eight generator stages), `device` (init or
the slab allocation), `gpu_error`. Engage is each seat's `served` counter beside
`metal_tower_stats` (one encode per seat per chunk); the parity instruments are the per-stage
cells on identical inputs on the f32 lane (`tests/CLAUDE.md`, the kitten file) and the served
synthesis across the knob.
