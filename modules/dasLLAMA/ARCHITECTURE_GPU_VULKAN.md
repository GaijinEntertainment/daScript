# dasLLAMA Architecture - the Vulkan resident driver

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2j, 2.2p and 2.2ab, the prefill chain and the byte stores of the Vulkan
resident driver: the prefill window chain, the Q8 requant byte store, and the decode GEMV
family's grid codebook buffer. The cooperative-matrix tiles the chain's GEMMs run on - the cm2
decode spelling, the tile pick and the coopmat mode ladder, the class-pipeline build seat, and
the MoE expert chain on those tiles - are `ARCHITECTURE_GPU_VULKAN_GEMM.md`'s sections 2.2k-2.2m
and 2.2q. What a model has to fit on the card before any of this runs - the residency plan, and
the marks swap that lets one GPU slot serve many models - is
`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`'s sections 2.2n-2.2o. The decode-era mechanisms of the
per-op tier are `ARCHITECTURE_GPU_VULKAN_DECODE.md`'s sections 2.2r-2.2v. The GPU backend role
table these sections build on stays in `ARCHITECTURE_GPU.md` sec.1.5.

### 2.2j The Vulkan resident prefill window chain {#vk-prefill-window-chain}

Companion to `ARCHITECTURE_GPU.md` sec.1.5; the Metal prefill driver's own ladder is
`ARCHITECTURE_GPU_PREFILL.md`.

**A prompt longer than `PF_WINDOW` rows runs as SEQUENTIAL windows over the same activation
buffers.** Every window's rope and attention address the KV mirror at ABSOLUTE positions, so
window w attends everything the earlier windows stored; only the last window runs the final
requant and the classifier.

**The last layer's FFN runs on the window's last 32 rows only.** Nothing downstream of the
final layer reads more than the last row - the classifier requantizes row `wlen - 1`, the KV
mirrors were stored before the FFN, and a later window starts from fresh embeddings - so the
gate, up and down GEMMs, the activation and the residual step of the last layer take a region
starting 32 rows below the window's end (`fill_arena_batch_sched`'s `row0`, `ActArgs.elem0`,
`ArArgs.row0`). Thirty-two, not one, because the s tile's fast path loads a whole 32-row
column unclamped and the resident prefill's activation planes (`pf_xf`, `pf_hf`) carry no read
slack past the window - unlike the MoE chain's gathered image and hidden plane, which
`_GEMM.md` sec.2.2l sizes with 32 rows of slack past their last region. Rows below the
slice keep stale gate, up, hidden and residual values that nothing reads. The sliced GEMMs do
not split k: the split-k reduce sums partial planes from row 0, so a region starting below the
window's end would reduce the wrong rows. The slice takes the f16-fed cm2 route only
(`gu6 && dn6`); the other feeds run the full window. Only the plain residual step (`cls_ar`)
and the f16 activation honor the row base: the fused residual twins feed the NEXT layer's
projections and never run on the last layer, so they index from row 0 by design.

**The k and v GEMMs merge into ONE dispatch when the layer's q, k and v weight planes are all
q8 and the k and v planes sit adjacent in the arena.** The bump allocator places them
back-to-back unless a slab boundary intervenes, so the merged form asks only those two
questions and dispatches at `d = 2 * kvd`: one dispatch instead of two plus a copy, which
doubles the otherwise starved k/v grid and deletes the v copy. Consumers read the merged output
through a `kstride` field - the projection-row stride, `2 * kvd` merged against the split
path's `kvd` - on `RopeKvBArgs` and on `QkRmsArgs`, and through `RopeKvBArgs.voff`, the v
rows' base inside that buffer - `kvd` merged against the split path's `npos * kvd`. It is a
record-cost win, not a GPU one.

**A window chain submits in chunks so recording overlaps execution.** The chain splits on a
1, 2, 4, 8-layer ramp, so the GPU starts on an early small chunk while the CPU is still
recording, and the doubling stops at 8 layers. Chunks go out through `submit_nofence`; the
window's last submit is the `submit_wait` that signals the one fence the caller waits on.
Ordering between chunks is the hazard rail's: a barrier recorded in chunk N+1 covers the
writes of chunk N because submission order on one queue spans submits, and the terminal
fence covers every earlier submit the same way. The command-buffer ring holds
`3 + ceil(depth/8)` buffers, so chunk N records into a free buffer while chunk N-1 executes;
the per-role GPU profile pins the single submit, so a chunk gap never bills to a role.

A `VkHaz` is private to one RECORDING SESSION, not to one command buffer: the chunked chain
carries the same `h` across every buffer in the window, so a barrier it records in chunk N+1
still knows what chunk N wrote. That is also why the batch/hybrid region bits (`VHB_*`) reuse
the same rail under their own namespace - two recorders never share a pending set.

**The attention chain's K/V host readbacks are recorded at the END of the chain, after the
`wo` GEMM, not beside the preps that produce them.** Driver 610.74 on the RTX 5060 Ti drops
the in-command compute-to-transfer barrier about one run in twelve: a copy recorded right
after the producing dispatch, behind a spec-valid global memory barrier, reads a partial
prefix of the output while a second identical copy at the end of the same command buffer
reads it whole. The intervening attention, requant and `wo` work is what closes that window;
the copies carry a `//!` naming this section, and the placement is a driver-defect mitigation,
not a chain-shape preference.

**A recurrent (deltanet) layer's window block replaces the attention head; the FFN tail is
shared.** Per window: the block's feed (f16 rows when qkv, z and out all admit the cm2 tiles, else the
q8 image), the qkv and z GEMMs into the window planes (the planes in their file formats - the loader tags a dense hybrid's deltanet planes natively when this driver will be attempted, so a Q5_K/Q6_K file rides the k5/k6 tiles; the out plane is q8, the step's o row feeds it so), the beta and alpha rows into the layer's smalls
(f32 arm: a 16-position tile GEMM over the `[beta ; alpha]` rows, its grid position tiles by 16-output groups with one output per invocation, so a layer of only `2 x nvh` rows - 64 on the 9B - still fills the card; q8 arm: two q8 GEMMs and copies), the
conv reading the layer's ring image, the sequential scan over the layer's own state slot, the o rows' feed (f16 or requant) and the out GEMM into `pf_xb2`. The
scan is the plain per-token delta rule: a four-subgroup workgroup per (head, column group), a lane keeps
16 state rows of two adjacent columns in registers (two independent chains that interleave), the token's k and q rows are staged once per workgroup in
shared and feed both columns, the tokens loop inside the kernel with two shuffle reductions per column each, the raw o rows land in the tier's workspace for the gated out-norm's one workgroup per position.
The conv history crosses windows position-major in ring image 0; the last window transposes the
tail into the decode step's per-channel layout (`dn_tail_cls`; the handoff is `_DECODE.md`
sec.2.2v's). Every window past the first carries at least the conv taps: when the rows left after
a full window would be fewer than the taps, that window gives them up so the last one holds the
taps, and only a lone first window can be shorter - its history is zero, so the tail writes the
ring's leading rows as zero (`DnTailArgs.zero_rows`). Off the f16 feed a K-quant qkv/z pair reads
the Q8_K activation form, as the attention head's kq planes do, and the q8 beta/alpha arm
re-requantizes the rows Q8_0 behind the z GEMM (one feed, two forms - the decode's rule).
Gated attention rides the batch kernels through a per-head q stride (`qhs = 2 x hs`:
the q GEMM writes `[q | gate]` per head, qk-rms and rope read q head-strided in place, the mirror
attention gates on the sigmoid of the gate half); partial rotary is the `half = rot / 2` word.
At head 256 the window takes the h256 cm2 flash stamps (Br 64, Bc 32, the h128 loop with the head-shaped tiles doubled): the gated twins load Q at the head's q stride and scale the normalized output by the sigmoid of the gate half before the store; the h128 coopmat twin stays 128-only.

**A layer's qkv feed comes out of the previous layer's FUSED add+rms twin when the fuse knob is
on and the feed is not the Q8_K quant form.** The producer is layer l-1's addr_next site, the
consumer is layer l's b+0 slot, and both key on one predicate (`pf_qkv_feed_fused`): where it
holds, addr_next encodes `cls_ar_f16_b` (an f16 feed) or `cls_ar_rq_b` (a Q8_0 feed) straight
out of the row stash and b+0 only stamps; where it does not, the split `cls_ar` writes the
residual row and b+0 converts or requantizes it. The fused twins never write the `xb` plane, so
the last layer always takes the split arm - the final requant reads `xb`. The addr_ffn site
fuses the same way for the gate/up feed. Bit-identity with the split pair is a suite gate.

**The cm2 flash-attention tile lands its output f16 when the `wo` feed is f16.** The tile
template carries an `OUT16` stamp: the f16 instance converts the O accumulator in-kernel and
writes the `wo` feed plane directly, so the per-layer attn-to-f16 convert never encodes; the
f32 instance serves the quant route. The two device converts agree bit for bit; the CPU's
`float16()` rounds ties differently, so the twin's gate compares device against device.

### 2.2p The Q8 requant writers store one quant per byte {#q8-requant-byte-store}

Every requant writer on the class rail - the prefill and decode-tail kernels that write Q8_0 or
Q8_K quants - declares its output plane `array<int8>` and stores one quant per element, over
SPIR-V's 8-bit storage path; the fused decode step `DnStepFused` keeps its packed-word head
requant, the one writer outside this rule. Packing four quants into a `uint`
instead costs a shift-and-or chain per word, and in a Q8_K writer - where four co-active lanes
each hold one byte of the word - two subgroup shuffles per element on top. The stored bytes are
the same under either form: the amax fold, the scale and the rounding decide them, and all
three sit above the store. The path needs the device's 8/16-bit storage feature set, which the
family's device creator enables.

### 2.2ab The decode GEMV family stages its grid codebooks from one device buffer {#kq-gemv-grid-buffer}

The grid formats' codebooks (iq2s, iq2xs, iq2xxs, iq3s, iq3xxs) live in one 17 KB model-owned
device buffer, bound at binding 6 of the kq GEMV family and built on the family's first set
(`kq_grid_dev`); each format's table sits at a fixed word offset (`KQ_GRID_<FMT>`), and a grid
format's GEMV stages its table into workgroup memory from that buffer - a coalesced 256 B read
per 64 lanes. The tables also exist as per-index accessors over a `fixed_array` local
(`iq2s_grid_word` and kin), which the batch and cm2 tiles stage from, because a tile amortizes
one stage over 128 rows x 64 columns. A per-row kernel cannot: the emitter lowers such an
accessor to a constant composite stored into a Function variable, the driver serves the
per-lane indexed read of it serially, and the two-row GEMV workgroup paid that serial read on
every 2 x 5120 weights it walked - iq2s streamed at 84 GB/s where k4 streams at 410. The
buffer form puts every grid GEMV in the k-format band (iq2s 388, iq2xs 407, iq2xxs 400,
iq3s 415 GB/s on the reference card, `harness/vk_gemv_probe.das`). The buffer dies in the
model-drop sweep with every other device buffer, and its handle zeroes there, so the next
model's first kq set rebuilds it.
