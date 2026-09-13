# dasLLAMA Architecture - the Vulkan resident driver

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2j, 2.2p, 2.2ab, 2.2ac, 2.2ad, 2.2ai, 2.2aj and 2.2al: the prefill window
chain, the Q8 requant byte store, the decode GEMV family's grid codebook buffer, the tile probe's
shared descriptor set layout, the recurrent block of the prefill window, the roster of Vulkan
capabilities the tier keys its routes on, how the `[vk_dispatch]` lens derives `readonly` from a
class family's accesses, and the token command's attention key split. The MoE block of that
window, the token command's routed twin and their gemma-4 form are `ARCHITECTURE_GPU_VULKAN_MOE.md`'s
sections 2.2af, 2.2ag and 2.2ak. The cooperative-matrix tiles the chain's GEMMs run on - the cm2
decode spelling, the tile pick and the coopmat mode ladder, the class-pipeline build seat, the MoE
expert chain on those tiles, and the KHR arm's hand-staged kq tile - are `ARCHITECTURE_GPU_VULKAN_GEMM.md`'s
sections 2.2k-2.2m, 2.2q and 2.2ae, and the decode GEMV family's lane split by row length its
section 2.2ah. What a model has to fit on the card before any of this runs - the residency plan,
and the marks swap that lets one GPU slot serve many models - is `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`'s
sections 2.2n-2.2o. The decode-era mechanisms of the per-op tier are `ARCHITECTURE_GPU_VULKAN_DECODE.md`'s
sections 2.2r-2.2v. The GPU backend role table these sections build on stays in `ARCHITECTURE_GPU.md` sec.1.5.

The module gate's six Vulkan checks (`REVIEW.das`) read these files. `check_khr_stage16_abstract`
reads `class template KqCm2BatchT` in `dasllama_vulkan_classes.das` and licenses no names: its
`khr_stage16` is declared abstract. `check_ar_max_dim_triple` reads `AR_MAX_DIM` in
`dasllama_vulkan_common.das`, the `row` slab of `ArBase` in `dasllama_vulkan_classes.das` and the
`c.dim` cap of `attn_dec_shape_ok` in `dasllama_blocks.das`, and licenses no names: the three
numbers agree. `check_cm2_ladder_sets` walks every `class template <Fmt>Cm2T : KqCm2BatchT` in
`dasllama_vulkan_classes.das` twice: for the KHR trio it requires `<Fmt>KhrBatch`, its
`kq_batch_<fmt>_khr_cls` stamp and an arm in each of `khr_cls_ensure`, `khr_cls_set` and
`khr_cls_enc` in `dasllama_vulkan_prefill.das`, licensing `Q8Cm2T` alone - q8 is no `kq_sb`
format, its cm2 tiles carry no KHR arm, and the KHR mode serves q8 through its own tile; for the
e trio it requires `<Fmt>Cm2EBatch`, its `kq_batch_<fmt>_cm2e_cls` stamp (`q8_batch_cm2e_cls` for
q8) and an arm in each of `cm2e_cls_ensure`, `cm2e_cls_set` and `cm2e_cls_enc`, licensing none.
`check_cm2_stamp_tiles` reads every `[vk_dispatch]` stamp of those templates - in
`dasllama_vulkan_classes.das`, the probe's twins in `harness/vk_gemm_probe.das` and the bring-up
fixture `tests/_vkd_toy.das` - and requires its `AT`, `BT`, `ACC` and `ACCW` typedefs to follow
its `BK` and `BN`, an e or s stamp's `BN` to equal `SCHED_M_ROWS` or `SCHED_S_ROWS`, and a
scale-caching stamp's `BLKW` to equal `BK x UNR`; it licenses no names. `check_vk_extension_roster`
walks `dasllama/` for every `"VK_*"` extension name and every `*_supported` probe
`modules/dasVulkan/daslib/vulkan_boost.das` declares, and requires each inside `vk_ext_roster` in
`dasllama_vulkan_common.das`, licensing no names.
`check_no_hand_pipelines` walks `dasllama/`, `harness/` and `tests/` for a
`vkCreateComputePipelines(` call and licenses no names inside them; the two llama.cpp shader ports
under `performance/` (`coopmat_mulmm_reference.das`, `coopmat_mulmm_port.das`) sit outside the
walk as reference measurements of another engine's kernels.

### 2.2j The Vulkan resident prefill window chain {#vk-prefill-window-chain}

**A prompt longer than `PF_WINDOW` rows runs as SEQUENTIAL windows over the same activation
buffers.** Every window's rope and attention address the KV mirror at ABSOLUTE positions, so
window w attends everything the earlier windows stored; only the last window runs the final
requant and the classifier.

**The last layer's FFN runs on the window's last 32 rows only.** Nothing downstream of the
final layer reads more than the last row - the classifier requantizes row `wlen - 1`, the KV
mirrors are stored before the FFN, and a later window starts from fresh embeddings - so the
gate, up and down GEMMs, the activation and the residual step of the last layer take a region
starting 32 rows below the window's end (`fill_arena_batch_sched`'s `row0`, `ActArgs.elem0`,
`ArArgs.row0`). Thirty-two, not one, because the s tile - the cm2 tile with 32-row columns
(`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2l) - loads a whole 32-row column unclamped on its
fast path, and the resident prefill's activation planes (`pf_xf`, `pf_hf`) carry no read slack
past the window - unlike the MoE chain's gathered image and hidden plane, which
`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2l sizes with 128 rows of slack past their last region
(`TILE_READ_SLACK`). Rows below the slice keep stale gate, up,
hidden and residual values that nothing reads. The sliced GEMMs do not split k: the split-k
reduce sums partial planes from row 0, so a region starting below the window's end would reduce
the wrong rows. The slice takes the f16-fed cm2 route only (`gu6 && dn6`); the other feeds run
the full window. Only the plain residual step (`cls_ar`) and the f16 activation honor the row
base: the fused residual twins feed the NEXT layer's projections and never run on the last
layer, so they index from row 0 by design.

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
the per-role GPU profile pins the single submit, so a chunk gap never bills to a role. The
profile's stamps are bottom-of-pipe timestamps, and what a driver does at one decides how the
roles read: the Windows driver (616) writes it in passing, so two roles the hazard rail lets
co-run (q with k and v, gate with up) share one span and the second reads near zero; the Linux
driver (580) drains the queue at every stamp, so the same roles serialize, each reads its
standalone time, and the profiled window runs about 5% longer than the served one (the 35B on
the RTX 5080: pp512 3833 unprofiled, 3661 profiled; `harness/vk_gemm_probe.das -- ts:<fmt>`
measures the drain). A Linux profile's per-role figures are therefore standalone figures, and
the served window is their sum less the co-runs the rail permits.

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

**A hybrid's gated attention rides the batch kernels through a per-head q stride** (`qhs = 2
x hs`, twice the head size): the q GEMM writes `[q | gate]` per head, qk-rms and rope read q
head-strided in place, and the mirror attention gates on the sigmoid of the gate half. A
partial-rope model rotates the first `rot` elements of a head, and the kernels read the count
as the `half = rot / 2` argument word. At head size 256 the window takes the h256 cm2 flash
stamps (Br 64, Bc 32, the h128 loop with the head-shaped tiles doubled): the gated twins load
q at the head's q stride and scale the normalized output by the sigmoid of the gate half
before the store; the h128 coopmat twin stays 128-only.

### 2.2p The Q8 requant writers store one quant per byte {#q8-requant-byte-store}

Every requant writer on the class rail - the prefill and decode-tail kernels that write Q8_0 or
Q8_K quants - declares its output plane `array<int8>` and stores one quant per element, over
SPIR-V's 8-bit storage path; the fused decode step `DnStepFused` keeps its packed-word head
requant, the one writer that packs instead. Packing four quants into a `uint`
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
per-lane indexed read of it serially, and a two-row GEMV workgroup pays that serial read on
every 2 x 5120 weights it walks - 84 GB/s for iq2s against k4's 410. The
buffer form puts every grid GEMV in the k-format band (iq2s 388, iq2xs 407, iq2xxs 400,
iq3s 415 GB/s on the reference card, `harness/vk_gemv_probe.das`). The buffer dies in the
model-drop sweep with every other device buffer, and its handle zeroes there, so the next
model's first kq set rebuilds it.

### 2.2ac The tile probe's arms share one descriptor set layout {#khrx-shared-set-layout}

`khrx` is the lever sweep of the KHR kq tile (`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2ae) in
`harness/vk_gemm_probe.das` (`ARCHITECTURE_MEASUREMENT.md` sec.2.5): eight arms timed over one
shape. Two are shipped bodies - the KHR class, the reference each compared arm is checked
against, and the sdot4 kq tile. `ship` is the KHR tile copied with no lever moved, the control
the lever arms read against, and the four lever arms are that copy with one lever - the weight
stage (`dec4`, the four-wide decode callback in place of the word stage), the accumulator
width, the subgroup tiling, the workgroup size - moved back to its simpler form.
Every arm but the sdot4 one binds the same five-buffer descriptor set layout as the shipped
class, so an arm's figure differs from the shipped class's by its body alone and never by a
binding difference. The sdot4 arm reads its own Q8 activation fixture, so it binds that
class's own six-buffer set, and it is timed rather than compared.

The `nil` arm is the ceiling arm: its weight stage writes constants, so its rate is the
tile's ceiling with the weight loads removed. It still binds the weight plane and the scale
words the shared layout declares, and its stage reads neither (`KhrPxNil`, whose `wq` field
carries `@role = "alias"`). Those two are the probe's deliberately unread bindings.

### 2.2ad The recurrent block of the prefill window {#vk-prefill-dn-block}

**A recurrent (deltanet) layer's window block replaces the attention head; the FFN tail is
shared.** Per window the block runs the qkv and z GEMMs into the window planes, the beta and
alpha rows into the layer's smalls, the conv over the layer's ring image, the sequential scan
over the layer's own state slot, and the out GEMM into `pf_xb2`. The smalls are the layer's
per-layer f32 plane: the layer's cold constants (conv taps, out-norm weights, the `a` and `dt`
rows), the two parity ring images, and the beta and alpha rows. The weight planes stay in their file
formats where the loader tags them natively (`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2v
carries the tagging condition), so a Q5_K/Q6_K file rides the k5/k6 tiles.

The block reads two activation feeds, and each one is decided on its own.

- **The x feed is the layer's input rows.** The qkv and z GEMMs read it. `pf_dnx6` decides it:
  f16 rows when both of those planes admit the coopmat tiles (`ARCHITECTURE_GPU_VULKAN_GEMM.md`
  sec.2.2l), else the quant image.
- **The o feed is the scan's output rows.** The out GEMM reads it. `pf_dno6` decides it: f16
  rows when the out plane admits the coopmat tiles, else a requant.

A K-quant file whose out plane is Q8_0 therefore keeps its qkv and z GEMMs on the coopmat
tiles and takes the requant on the o feed alone.

When the x feed is not f16, a K-quant qkv/z pair reads the Q8_K activation form, as the
attention head's kq planes do, and the q8 beta/alpha arm re-requantizes the rows Q8_0 behind
the z GEMM - one feed, two forms, as the decode step does (`ARCHITECTURE_GPU_VULKAN_DECODE.md`
sec.2.2v).

The beta and alpha rows take one of three arms. A file that carries them as f32 rows keeps them
f16 on the device; on the cm2 route with the f16 x feed they are two small f16 GEMMs - the beta
rows and the alpha rows each through `F16GemmCm2`, eight k chunks into the split-k scratch and
the reduce into the layer's smalls at the beta and alpha bases (the class is the MoE router's,
`ARCHITECTURE_GPU_VULKAN_MOE.md` sec.2.2af). Off that route the f32 arm is a scalar tile GEMM
over the `[beta ; alpha]` rows: one workgroup covers 16 positions by 16 output rows, one output
per invocation, and the grid runs over both group axes, so a layer of only `2 x nvh` rows
(`nvh`, the layer's value-head count) - 64 on the 9B - still fills the card. The q8 arm is two
q8 GEMMs and copies.

The conv is channel-major: a workgroup owns 256 channels over `DN_CONV_PB` positions
(`dn_conv_wgs` sizes the grid), every thread slides one channel's window over them with the taps
in registers and one new row read per position, and holds its outputs in registers for the SiLU
and the per-head L2 norm - the head's sum of squares crosses the head's lanes by shuffles and its
32-lane blocks by one shared row, a head being `ds` consecutive channels with `ds` dividing 256. The
position-major form it replaced (one workgroup per position, the row staged in 32 KB of shared
memory) re-read every input row once per tap and the whole tap table once per position, all from
L2.

The scan is the plain per-token delta rule in upstream's shape: one column of a head's state per
lane cluster, 16 state rows per lane in registers, four 32-lane blocks per workgroup, and every lane
reads its own k and q elements from the conv plane a token AHEAD, into registers, while the
current token computes (a row's loads never wait on the recurrence, whose per-token chain was
latency-bound on them: the 9B's scan 342 -> 322 us a layer, the 0.8B's window a twentieth
shorter) - no shared staging, no barrier between tokens. `dn_scan_wgs` sizes the grid, a workgroup covering `4 x 32 / (ds / 16)` columns
of one head (upstream's one warp per workgroup read the 35B 6% slower here: a thousand
one-warp workgroups each holding the gate arrays in shared memory). The head's per-token gates - the decay `exp(a * softplus(g + dt))` and `sigmoid(beta)` - are
computed once per workgroup into shared memory before the token loop, so the recurrence reads
two shared words a token where it paid five transcendentals and a divide per lane (a third of
the scan's time on the Linux RTX 5080; upstream's graph hands its scan the gates pre-activated).
The tokens loop inside the kernel; each token costs two cluster reductions inside the dependency
chain (a sixth of the scan's time there). The conv, the scan and the requant family's K-quant
fold map their 32-lane blocks and clusters from the invocation id, never from the subgroup id:
every shuffle in them carries an xor mask under 32, which stays inside such a block on any
subgroup width the tier admits (32 or more), so the kernels hold on a 64-lane device as on a
32-lane one; only the KHR tile keys on the width itself (`ARCHITECTURE_GPU_VULKAN_GEMM.md`
sec.2.2ae). The conv and smalls bindings are NonWritable, as every
binding no kernel of a class writes is (sec.2.2aj). The decoration is load-bearing: without it the driver orders each token's k and q
loads behind the previous token's o store (the two buffers may alias), and the same kernel ran
2.35x slower (19004 against 8084 us over the 0.8B's 18 layers on the Linux RTX 5080; the staged
two-column form it replaced 8854). The raw o rows land in the per-op tier's workspace, for the
gated out-norm's one workgroup per position.

The conv history crosses windows position-major in ring image 0; the last window transposes the
tail into the decode step's per-channel layout (`dn_tail_cls`; the handoff is stated in
`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2v). Every window past the first carries at least as
many rows as the conv has taps: when a full window would leave the last window fewer rows than
that, the earlier window takes fewer rows instead, so the last one still holds the taps. Only a
lone first window can be shorter - its history is zero, so the tail writes the ring's leading
rows as zero (`DnTailArgs.zero_rows`).

### 2.2ai The device-init log names every Vulkan capability the tier keys a route on {#vk-extension-roster}

`vk_ext_roster` (`dasllama_vulkan_common.das`) is the one list of the Vulkan extensions, core
feature sets and device limits the tier reads: for each, the probe the tier's arming reads
(`modules/dasVulkan/daslib/vulkan_boost.das`'s `*_supported` probes, `device_extension_available`
by name, or a limit), the env knob that holds the route off, and what the tier does with it and
what serves without it. Device init prints one `ext <name>: <state> - <what rides on it>` line
per entry after the `device ready` line, so a box's log says which route each capability decided
- the reading a box-to-box gap starts from (a Linux driver that lists no
`VK_NV_cooperative_matrix_decode_vector` runs the scalar decode arm, a device with no
`VK_NV_shader_sm_builtins` never splits k). The roster re-queries the device rather than reading
the arming's fields, so the two cannot disagree by construction only where the arming reads the
same probe - the kernel file's roster cell holds the arming's fields to the roster's entries. The
module gate keeps the roster complete: every extension name and every such probe the tier calls
appears in it.

### 2.2aj The `[vk_dispatch]` lens derives `readonly` from the class family's accesses {#vk-readonly-lens}

Every `@ssbo` member on a binding no `[spirv_kernel]` method of the class writes gains
`readonly`, which the emitter decorates NonWritable, so the driver orders no load of that
binding behind a store. The verdict is the family's: a field is one declaration for every
kernel of the class, so a binding one method writes is written for all of them, and a written
view protects its same-binding aliases. A method body the access classifier refuses counts as
writing every binding, so a body the classifier cannot read loses the decoration rather than
carrying a false one. A declared `@readonly` on a binding a kernel writes is the one shape that
yields a module the validator rejects, and the lens refuses it.

### 2.2al The token command's attention splits a head's keys across workgroups {#vk-decode-attn-split}

**The decode attention dispatches a workgroup per (head, key split) and a combine per head.** One
workgroup a head leaves a low-head model's attention on a few SMs (four heads of eighty-two), so the
decode pass (`DaAttnT`) cuts the attended span into `nsplit` 32-aligned pieces (`da_nsplit`: enough
workgroups to cover the SM count twice, at most `DA_NSPLIT_MAX`, one where the count is unknown or the
heads alone cover it), each running the online softmax over its piece into an unnormalized partial
(max, denominator, accumulators; an empty piece's weighs nothing); `DaAttnComb` aligns a head's
partials by their maxes, normalizes, gates and stores the row (unsplit, the pass stores it), and the
store quantizes the row for the `wo` plane (`rqk`: Q8_0 blocks by the 32-lane group's amax, Q8_K
superblocks by the workgroup's on a head of 256 or 512), so no requant dispatch follows. The scores
go a subgroup a key, lanes across the dims (one coalesced K row, the dot a subgroup add); the V pass
keeps a thread a dim. The flash tile (`FaCm2T`) runs one workgroup a (head, 64-row q tile) unsplit: a key split there costs more in partial stores and a combine than the shorter key loop returns on every gemma shape measured (`followup_vulkan.md` item 50). A model that softcaps its attention logits (gemma-2) takes the tile's `CAP` leaves at head size 256: every scaled score through `cap * tanh(s / cap)` before the mask, the 8-row tile serving any other capped shape.
