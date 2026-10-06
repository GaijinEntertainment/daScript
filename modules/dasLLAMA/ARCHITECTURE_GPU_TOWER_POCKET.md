# dasLLAMA Architecture - the Metal tower driver's Pocket TTS seats

Companion to `ARCHITECTURE_GPU_TOWER.md`; a section is cited by its anchor. This document carries
the three seats the Metal tower driver fills for the Pocket TTS family - the codec, the frame loop
and the text prompt. The Vulkan TTS driver's Pocket seats are
`ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-pocket-chain`; the family itself is `ARCHITECTURE_POCKET.md`.

### The tower driver's Pocket codec seat {#tower-pocket-codec}

The Pocket TTS codec decoder rides the tower driver as the first seat of the family's hook record
(`register_pocket_gpu`, `ARCHITECTURE_MEDIA.md#tower-gpu-hook`): the latents of a chunk go up, the
samples come back, one command buffer. The chain is the CPU's `pocket_decode_latents` run as its
first window over the whole chunk - the stream's carries are the first window's zero rows, and the
CPU's windows equal that one shot to float noise (`ARCHITECTURE_POCKET.md#pocket-codec-stream`) - so no carry
crosses a dispatch: the 1x1 latent projection and every dense conv on the conv-gathering GEMM stamp
(a forward conv behind `k - stride` zero rows, a transposed conv's first `t * stride` output rows;
the input row stride `xs` of `St2ConvArgs` lets a conv read the padded width its producer wrote),
the depthwise upsample on the residual pool kernel, the two codec transformer layers on the dense
GEMM stamp with the layer scale, the prefill rows rope over the layer's row stride with the CPU's own
cos and sin tables (no device trigonometry) and a windowed causal attention kernel over device K/V
rows, ELU and the row copies on one row kernel, the sample column picked out of the last conv's
padded rows. Every GEMM is the
f32-exact stamp: the codec reads within 1e-6 of the CPU chain on the f32 lane that way on the M5
Max (1e-2 on the served q8 and K-quant lanes, whose CPU chains quantize their activations), and the f16-staged twins
bought no time on this chain (the row kernels and the attention, not the GEMMs, carry its cost)
at three orders of magnitude of agreement. The slab holds every codec weight, keyed on the
addresses and the lane as the StyleTTS2 slabs are, and drops with them. A chunk past
`PK_CODEC_MAX_FRAMES` latent frames declines on its row budget (the one-shot rows scale with the
chunk) and the CPU's windows serve it; the other declines are `knob`, `shape` (a channel count off
the 8-run lattice, a head over 128 wide, a transformer width off the 32 lattice), `device` and
`gpu_error`. The frame loop is the family's second seat and the text prompt its third
(`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`).

### The tower driver's Pocket frames seat {#tower-pocket-frames}

The Pocket frame loop - the backbone step and the flow head for every frame of a chunk - rides
the tower driver as the family's second seat once the prompt's rows sit in the voice's caches:
the CPU's `pocket_synthesize` loop, dispatch for dispatch, in batches of `pocket_frame_batch()` frames
(eight) per command buffer, the EOS logits read back and the stop rule walked on the host between
batches (`pocket_frames_batched`, the host loop both GPU drivers run), the latents and the
conditioning rows read back once at the end. A voice slot holds the device K/V rows `[cap][d]` per
backbone layer, keyed (`tts_pk_voice_current`, the residency both drivers read) on the caches'
addresses, their fill and capacity and a sample of the rows they hold: the voice's prompt rows are transposed in once at
attach, a chunk's text rows behind them every chunk - uploaded from the host caches, or left by the
prompt seat below - and the frames append after those: the frames' rows never come back to the host,
since the CPU chain forgets a chunk's rows by resetting the fill. The
key samples the rows because an address alone outlives the voice that held it: a later voice's
caches can land at the freed address with the same fill and capacity. The
backbone's q8 linears (a K-quant linear requantized to q8 from its dequantized rows, so the small
form serves) ride the decode GEMV over a 34B-block blob with their bias rows in the slab, the bias
a row add after the GEMV (the row GEMV's fused-bias q8 form as it stood before the lane-map fold,
one lane a block, ran a frame 20% slower than the decode GEMV's split-K walk - `PERF_LEDGER.md`'s
Pocket frame loop entry; the folded row stamp keeps the epilogue kinds only); every
f32 linear rides the row GEMV over the slab's rows - a simdgroup a row, x staged in threadgroup
memory - so the parity lane runs exact. Per layer: the first norm (the layer before's residual
joined in the same dispatch - the tower LayerNorm's ADD stamp), the fused q/k/v projection, the
decode rail's rope-and-store kernel rotating q and k and storing k and v into the voice slot's row
(its f32 stamp, the rope tables and the caches bound at the position's row), the attention row (a
threadgroup per head, the scores staged, one softmax, the value sum in parts), the out projection,
the residual join with the second norm, the two ffn projections around the tanh GELU. The out norm writes the conditioning row straight
into the readback rows, and the head's GEMVs carry their norm, modulation and activations as
prologue and epilogue stamps (the LayerNorm and adaLN modulation in, the SiLU, the gated
residual, the SiLU over the time constant, the tail that adds the noise and denormalizes the
latent). The noise is drawn a batch ahead in the frame order the CPU draws it, so a teacher-forced
run reads the same stream, and the last batch's draws past the frames made are rewound so the
generator ends where the CPU loop's does; a command buffer that fails declines the loop as
`gpu_error`, the generator is put back to where the chunk found it, and the CPU loop reruns the
chunk from the caches and the stream as they were. The other declines are `knob`, `shape` (the
backbone and head widths off the 32 lattice, a head size other than the 64 the attention row is
stamped for, a cache past 2048 rows, a width past the GEMV's 4096-wide stage) and `device`. On the served lanes the seat reads within the CPU chain's own
distance from the reference: the CPU quantizes the activations it feeds a q8 or K-quant plane, the
tower feeds them f32.

The text prompt is the family's third seat, the rows form of the same layers: a chunk's text rows
(the host's embedding gather, `pocket_prompt_embed`) through the frames slab's layers at the
positions after the voice's rows, each f32 linear the exact f32 GEMM and each q8 linear the prefill
ladder's q8-blob GEMM (`pf_enc_q8_mm` over the frames blob's regions, the bias row after it), the
rope from the voice slot's tables at the first row's position (`MetalPkRopeTab`, the position-based
rope both homes stamp from `GkRopeTab`), the keys and values into the voice slot's rows at those
positions and the attention over the slot's rows below them. The last layer ends at its keys and
values: a prompt's residual is read by nothing, so its attention, out projection and FFN are not run -
the CPU chain's `transformer_rows` makes the same cut under `kv_only_last` for the prompt and for the voice state. The rows
come back to the host caches after the command buffer (`kv_cache_append_rows`), so the CPU frame loop
and the frames seat's upload read the same cache either way; a served prompt also leaves
`TtsPkPromptDev` naming the voice, the fill and the row count, which the frames call after it spends
on entry - served or declined, so no decline leaves it for a later chunk - to skip uploading rows the
device already holds; a frames call over another voice, fill or prompt finds the record stale and
uploads every row. The prompt declines as the frames seat does (`knob`,
`shape` - the frames admission plus the GEMM widths on the 64 lattice - `device`, `gpu_error`).
