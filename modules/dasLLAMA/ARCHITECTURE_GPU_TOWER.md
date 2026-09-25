# dasLLAMA Architecture - the tower attention routes and encode chains

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2w-2.2y - the three routes that serve tower attention on Metal, the
one-command-buffer encode chain each family the Metal tower driver serves gets, and the StyleTTS2
synthesis chain - and sections
2.2aq-2.2ar, the Vulkan tower driver's row classes and attention routes, and its encode chains. The
GPU backend role table these sections build on - the tower driver's role row included - stays in
`ARCHITECTURE_GPU.md` sec.1.5.

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

### 2.2aq The Vulkan tower's row classes and attention routes {#vk-tower-routes}

Every vision block is a handful of row operations around two tiles the LLM rails already own -
the Q8_0 batch tile (or the f16 GEMM class) and the flash tile. The row operations are
`[vk_dispatch]` classes in `dasllama_vulkan_classes.das`, each the device twin of one CPU tower
helper (`rms_rows`, `clamp_rows`, `requant_rows_q8_sized`, `rope_neox_2d_rows`,
`rope_neox_tab_rows`, `layernorm`, `add_inplace_rows`) or of one closed form (the clamped
GEGLU-quick, the tanh GELU with the CPU LUT's f16 rounding, silu(g + bg) . (u + bu)); the CPU
helper is each class's oracle in `tests/test_vulkan_tower_kernels.das`. The seams fold what the
CPU loop spells as two calls: a post-add writes x += y (+ b) and, in the same workgroup, the next
branch's pre-norm off the updated row.

The flash tile is stamped at head sizes 64 and 128, causal (the LLM stamps) and bidirectional, and
the towers take three routes:

- **The compact route** serves a head size of exactly 64 (gemma4v): q, the f16 K/V shadows and the
  output ride the compact rows straight into the h64 bidirectional tile.
- **The padded route** serves the 72- and 80-wide heads (gemma3v, qwen3v, qwen25v's full
  layers): a restride writes each head into a 128-wide slot with zero pads - the q panel f32, the
  K/V shadows f16 - the h128 bidirectional tile runs at the head's own scale (the zero pads add
  nothing to the dot), and the unpad reads the compact rows back off the padded f32 output. A
  fused [q | k | v] row (qwen3v) is read through the restride's slot stride and offset, so no
  copy splits it.
- **The window route** serves qwen25v's block-diagonal layers in f32 on the compact rows, as the
  Metal per-window route does (2.2w): one workgroup a (window, head), the window's rows (64 at
  most, `wlo` the row starts) attend each other and nothing else, the scores in workgroup memory.
  No restride and no f16 shadow: the coopmat tile's f16 staging noise compounds over the 28
  window layers of a 32-block tower - a slotted f16 tile reads 0.16 to 1.5 x rms against the
  exact chain at 32 blocks, the f32 window route 0.02 to 0.09, the Metal rung's order.

The Metal driver keeps the compact 72-wide heads on its own flash kernel (2.2w); the Vulkan
driver pays the restride instead, because the flash template's coopmat typedefs size on the head
and 72 is off every fragment lattice. Per family, both drivers:

| family | head | Metal attention (2.2w) | Vulkan attention | Vulkan weights |
|---|---|---|---|---|
| gemma4v | 64 | the slab trio | the compact route, h64 tile | the q8 image, gathered on upload |
| gemma3v | 72 | the flash route (rows a multiple of 64), else the slab trio | the padded route, h128 tile | the q8 image, gathered on upload |
| qwen3v | 72 | the flash route (rows a multiple of 64), else the slab trio | the padded route off the fused row | the q8 image, gathered on upload |
| qwen25v full layers | 80 | the slab trio | the padded route, h128 tile | the baked halfword twin |
| qwen25v window layers | 80 | the per-window route | the window route, f32 | the baked halfword twin |

### 2.2ar The Vulkan tower driver's encode chains {#vk-tower-encode-chains}

`dasllama_vulkan_tower.das` fills the gemma4v, gemma3v, qwen3v and qwen25v hook slots on a build
without das_metal (the Metal driver owns them there). Each chain is 2.2x's shape: one command
buffer per encode walks the blocks dispatch for dispatch as the family's CPU loop does, the CPU
loop is the specification, the stem and the tail stay on the CPU, and the residual stream comes
back into the family's state. Where a chain and its family part on the seat:

- **gemma4v** serves the hook after the CPU stem, whole. **gemma3v**'s hook fires before the CPU
  stem (the Metal driver runs the stem itself), so `gemma3v_encode` finishes the stem on the CPU
  first when the tower is q8 and the driver serves the blocks alone. **qwen3v**'s hook is
  exact-lane and whole-chain (Metal's), so the driver takes the blocks-only q8 seat
  (`register_qwen3v_gpu_blocks`): after each deepstack tap block the residual is copied on the device
  into a stash read back beside x, and the tap mergers run on the CPU off those rows, a tap past
  a truncated tower's blocks skipped as the CPU loop skips it. **qwen25v** has no q8 lane, so its
  blocks-only seat (`register_qwen25v_gpu_blocks`) runs after the CPU stem and before the CPU
  tail over the baked halfword twin through the f16 GEMM class; a bf16-sourced twin declines.

The q8 families ride the q8 image the CPU lane reads: the driver's `serves` answer to the lane
policy is no, so a box with the driver keeps the q8 lane, and the device chain's parity
instrument is the three-way twin cell - the exact CPU chain, the CPU q8 chain and the device
chain on the same canvas, the device's distance from the exact chain held within 1.5x the CPU q8
chain's own. The x64 q8 plane is the active backend's grp interleave, which the tiles do not
read, so the upload gathers each block's GEMM rows row-major through `q8_gather_rows` with f16
scales - the resident MoE driver's precedent - once per tower into device memory, beside the
per-block norm rows with a ones row appended (the weightless per-head norms read it). qwen25v's
upload copies the twin's block GEMM region verbatim - IEEE halves, the GEMM offsets rebased to
the region's start - beside the block rows; the family has no scale plane. The residency key -
over the q8 plane, or over qwen25v's halfword twin - folds every plane the upload reads: the
weight plane's address, size and sampled words, the scale plane's, the norm rows' region, the
served block count and offsets, and the active repack layout. An address is not an identity: a
truncated tower minted from the same bytes lands at the freed address, and a resident sized for
fewer blocks reads past its rows under a deeper chain. The
model drop's sweep tells the driver to forget its handles through `register_vk_drop_hook`
(every buffer and set is model-owned); `vulkan_tower_shutdown` is the tests' release. The
per-encode scratch is sized to the canvas and grown when a taller one arrives; every batch
schedule of an encode sits in one meta buffer sized by the encode (a record and a map a
dispatch), so nothing the command reads moves under it; the batch tile's variant is picked per
output width - d, ff and qwen3v's fused 3d can each pick a different tile.

The declines: `quant_mode` on an exact-lane tower (or the bf16 twin), `shape` off the tile's head
sizes, past the row cap, or with a weight plane or scratch buffer over `vk_max_storage_range()`,
`knob` (`DASLLAMA_VK_TOWER`), `device` where the tier's want (`DASLLAMA_GPU`, read before any
device init) or a class declines, `memory` where an allocation fails (the partial upload or
scratch is released, the CPU chain serves). Each decline is logged once per reason per model.
Engage is
`vulkan_tower_stats` and `vulkan_tower_declines` deltas, and the bench's image cell prints them
around its timed turn.
