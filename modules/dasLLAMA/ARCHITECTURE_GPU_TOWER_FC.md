# dasLLAMA Architecture - the FastConformer chain's rotary and LayerNorm-conv form

Companion to `ARCHITECTURE_GPU_TOWER.md`; a section is cited by its anchor. This document carries the
second form of the Metal tower driver's FastConformer chain - the one GigaAM serves through - beside the
rel-pos form canary and parakeet serve (`ARCHITECTURE_GPU_TOWER.md#tower-encode-chains`).

### The rotary position form {#tower-fc-rotary}

The FastConformer chain takes its block's shape from a form record (`FcForm`): the depthwise taps' layout,
the position form and the conv module's norm. Canary and parakeet run the rel form; GigaAM runs the rotary
form. One chain carries both because every other dispatch of a block - the macaron feed-forward halves, the
pointwise convs, the GLU, the residual adds and the output norm - is the same graph at the same shapes.

The rotary form rotates the attention's normed INPUT rows, not q and k: the attention LayerNorm writes its
rows, v projects them unrotated, then one NEOX rope pass (`MetalTwRopeTab`, the vision towers' table rope)
turns those rows in place, read as heads of `hs` columns, and q and k project the rotated rows. The CPU family
rotates a copy and projects q, k, then v (`ARCHITECTURE_MEDIA_ASR.md#gigaam-rotary-input`); the chain rotates in
place, so it projects v first. The reorder changes no number: v reads the unrotated rows either way, and the three
projections are independent GEMMs. A rope on q and k after their projections gives different numbers, because the
projection mixes the heads' columns. The tables are the
CPU family's own, `[npos][hs / 2]` from position 0 at the file's rope base, uploaded once an encode; the form
builds no rel table and runs no rel-shift.

The attention is the slab trio (`ARCHITECTURE_GPU_TOWER.md#tower-attn-routes`) over every head at once. The
head restride pads each head to `hs_pad = max(64, round_up(hs, 32))` - GigaAM's 48-wide heads land on 64 -
with zero lanes, so the dot products are exact and the scale stays `1 / sqrt(hs)`. q pads to the query
panel's rows, k and v to the key panel's 64 lattice; the pad writes zero rows past the live frames, so no
pooled buffer is zeroed on the host. The trio's AV output restrides back into the compact rows the output
projection reads. The form's shape gate takes any even head up to 128 wide and a score slab of at most
`FC_DEV_SLAB_MAX_BYTES` - about four minutes of audio at sixteen heads; GigaAM's clips are split at 25 s.

GigaAM's encode seat (`metal_gigaam_encode`) runs the front in the same command buffer: each stride-2
conv1d is the shared 1-D im2col (`MetalSt2Im2col`, rows-major source, tap-major columns - the order the
loader lays the conv weights in) and one GEMM with its bias-and-ReLU rows, both on the f32 tile over the blob - the
halfword plane holds the block GEMMs alone. Off the span encode (`gigaam_encode_span`) the hook takes the raw
samples and the log-mel runs ahead of the front in that command buffer (`ARCHITECTURE_MEDIA_ASR.md#gigaam-log-mel`):
the frame window (`MetalGaFrames`), the DFT on the f32-exact tile against the twiddles' transposed panel (a
half-staged tile would cost the mel its 1e-5 parity), the power spectrum and the clamped-log filterbank
(`MetalGaMel`) - the panel, the window and the filters one buffer a model. Off `gigaam_encode` the hook takes
the CPU mel. The seat answers -1 with nothing counted on
a shape off the lattice, a front lever off (`set_metal_gigaam_front`), a device without the
form's kernels (`pf_ga_ready`, and `pf_ga_mel_ready` off the samples), its lane's weights off the device
(`fc_weights_ready`: the q8 slab, or the half tensor crown under the halfword plane), a scratch buffer it cannot take
or a failed command buffer; the CPU then cuts the mel and the block seat
serves behind the CPU front and counts its own declines.
Both seats serve the q8 and exact weight lanes (`ARCHITECTURE_MEDIA_ASR.md#gigaam-planes`): the exact lane's GEMMs on
the half tensor GEMM over the carrier's halfwords, the q8 lane's on the prefill q8 ladder over the model's q8 slab. The kq
lane - a quantized carrier's own planes - declines by name (`kq_lane`, `TwFc.kq_lane`, the members in the words): no
chain GEMM reads the engine's K-quant planes, and the encode seat stands down with it. Canary and
parakeet keep declining their q8 lane (`quant_mode`, `TwFc.q8_served` off).

The chain submits on the timed encoder, which every submission already pays for, so its device time costs the served
path nothing: under the asr_prof rail `fc_submit` adds the command buffer's GPU time and its recording time under the
family's `FcForm.prof` prefix (`<prefix>.encode.gpu`, `.encode.record`) beside the family's encode wall. One command
buffer leaves no per-kernel split, so with `set_metal_fc_prof_replay` armed the rotary form replays, after the
readback, the head restrides and the slab trio over the last block's panels, `n_layer` times each in a command buffer
of its own (`.encode.gpu.restride`, `.encode.gpu.attn`); every replayed dispatch rewrites only scratch the readback
already left, and the replay's wall lands inside the encode bucket, which is why it is opt-in.

### The LayerNorm conv-module norm {#tower-fc-conv-ln}

Canary's and parakeet's conv module folds its BatchNorm into the depthwise stamp, with the SiLU after it.
GigaAM's conv module has a LayerNorm over the channels after the depthwise conv, which no fold can carry -
its statistics are per row. Its form runs the depthwise conv as the shared template's biased centered stamp
(`MetalGaDw`, `GkDwConvRows` FORM 3: the bias row first, no epilogue; `MetalGaDwH` over taps the carrier holds in
f16, the context's `btaps` binding the family's halfword plane), then the row LayerNorm in place, then the SiLU rows. The family's offsets are the chain's offsets record with the LayerNorm's rows in the
BatchNorm slots (`gigaam_layer_offsets`), which only this form reads.
