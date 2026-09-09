# dasLLAMA Architecture - the Vulkan resident driver

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2j, 2.2p, 2.2ab, 2.2ac, 2.2af and 2.2ad - the Vulkan resident driver's prefill
chain, its byte stores, and the tile probe's set layout: the prefill window chain, the Q8
requant byte store, the decode GEMV family's grid codebook buffer, the tile probe's shared
descriptor set layout, the MoE block of the prefill window, and the recurrent block of the
prefill window. The cooperative-matrix
tiles the chain's GEMMs run on - the cm2 decode spelling, the tile pick and the coopmat mode
ladder, the class-pipeline build seat, the MoE expert chain on those tiles, and the KHR arm's
hand-staged kq tile - are `ARCHITECTURE_GPU_VULKAN_GEMM.md`'s sections 2.2k-2.2m, 2.2q and
2.2ae. What a model has to fit on
the card before any of this runs - the residency plan, and the marks swap that lets one GPU
slot serve many models - is `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`'s sections 2.2n-2.2o. The
decode-era mechanisms of the per-op tier are `ARCHITECTURE_GPU_VULKAN_DECODE.md`'s sections
2.2r-2.2v. The GPU backend role table these sections build on stays in `ARCHITECTURE_GPU.md`
sec.1.5.

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
past the window - unlike the MoE chain's gathered image and hidden plane, which sec.2.2l sizes
with 32 rows of slack past their last region. Rows below the slice keep stale gate, up,
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

### 2.2af The MoE block of the prefill window {#vk-prefill-moe-block}

**An MoE layer's window block replaces the dense FFN tail with a routed block on the device;
the attention head and the residual steps are shared.** The whole-model driver admits a MoE
whose expert stacks fit the arena beside its attention quads (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`
sec.2.2n), and the window then never leaves the device between layers: the CPU's routing,
bucketing and combine of the per-op tier (`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2q) become
five device stages over the window's FFN-normed rows.

- **The router GEMM** (`RouterGemm`) is the span's router GEMV batched: a 64 x 32 tile of
  positions by router rows per workgroup, each invocation a 4 x 2 block whose two rows sit 16
  apart, K in 64-wide steps through a float4 stage in shared memory at a row stride of 17
  float4, the next step's rows fetched into registers while the current step computes, and each
  output's four products per float4 added in k order (the scalar loop's sums to the bit). The
  stride and the row split are the bank rule: sixteen lanes reading sixteen rows land on all
  eight 16-byte bank groups, so a warp's two weight loads take two wavefronts each and its four
  activation loads (two distinct rows) one. The scalar stage this replaced - rows 2te and 2te+1
  at a stride of 68 floats - put four of every sixteen lanes on one bank, and the tile ran at
  about 17 FMAs per cycle per SM: 4.3 ms per 30B window against 2.4 now (a 16 x 16 tile of one
  output each, stepping K by 32, was barrier-bound at 243 us per layer for 268 MFLOP; a 32 x
  32 tile of 2 x 2 blocks staging each step before computing it read 142). The 64-wide K step
  is why the MoE seats ask for a 64-multiple row width. The router plane holds every MoE layer's f32 rows,
  and a gated shared expert's gate vector rides as one more row past the experts, so one
  dispatch writes the logits row `[ne | gate]` per position.
- **The per-row select** (`TopKRows`, one workgroup per position) is the decode top-k's core
  over each row: the softmax, k picks largest-first with ties to the lower index, the
  renormalized or scaled weights - the host `moe_select_core`'s arithmetic - written
  position-major as the picked expert and its weight per slot.
- **The bucket schedule** (`MoeSched`, one workgroup) writes what the host fill writes for the
  per-op chain: the experts' slot counts are an atomic tally over the window (the 256 threads
  stride the picks, one workgroup atomic per slot), exclusive scans place each bucket's rows,
  its region indices (experts with rows, in expert order) and its tile workgroups, and the
  three expert planes' schedules land as 4-word records plus per-workgroup maps. A
  bucket is cut into at most two pieces by the tile ladder: a bucket within the s column (32
  rows) is one s piece; a bigger bucket takes whole m columns (128 rows) with the last one
  partial, unless the remainder past the whole columns fits the s column, which then takes it
  (`sched_ladder_m_rows`). The s pieces' records sit at `[0, ne)`, the m pieces' at `[ne, 2 ne)`,
  and each dispatch's map at its own offset past the records (`PF_MOE_MAP_OFF`, 2048 words for up
  to 256 experts twice), the two tile counts scanned as one packed word. Every dispatch is sized
  for its worst case - one s tile per expert; every expert's whole columns plus a partial one -
  and the map's tail past the real workgroup count carries the sentinel (`SCHED_NONE`): a tile
  workgroup that reads it sees a zero-row region and returns before its first barrier. A real
  window's router is skewed - on the Qwen3-30B-A3B at 512 tokens, 69 of 128 experts route, nine
  hold over 128 rows (the largest 467) - so the ladder runs about 90 column tiles where the s
  column alone ran 175 (`DASLLAMA_GPU_PROF=1` prints the last MoE layer's buckets and both
  counts): the expert planes at 593 / 650 us against 842 / 915 on the s column alone
  (`harness/vk_gemm_probe.das -- moesk:iq2xxs`, that window's profile). The slot-to-bucket-row
  map hands every slot the next row of its expert's bucket through an atomic cursor, so the
  rows within a bucket land in an order the schedule does not fix - which nothing downstream
  reads: the tiles compute rows apart and the combine reads each slot's row through the map, so
  the sums are the CPU walk's to the bit (the kernel cell checks the map as a permutation of
  each bucket's rows). The per-expert slot walks this replaced - thread e scanning every slot
  twice, staged through workgroup memory in 256-slot chunks - cost 163 us per layer on the 30B.
- **The gather, the expert tiles and the act** are the per-op chain's: the f16 gather scatters
  each position's row into its bucket rows, gate and up run the cm2 tiles over the gathered
  image - each plane's m dispatch then its s dispatch, the two writing disjoint rows of one
  plane under separate hazard bits (`VHZ_GATE_M`, `VHZ_UP_M`, `VHZ_MDN_M`) so they co-run and the
  reader's barrier covers both - the act writes the f16 hidden rows, and down runs the tiles
  again. The window planes carry 128 rows of read slack past the last bucket row (the s and m
  tiles load a partial column unclamped, `ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2l) and hold a
  whole window of `PF_WINDOW x k` bucket rows, so the block never chunks.
- **The combine rides the residual step.** The add+rms that follows the block (`ClsArComb`,
  its f16 twin `ClsArCombF16B` where the next layer's head takes the f16 feed) adds the shared
  expert's down rows at the sigmoid of the gate logit - the dense tail ran the shared triple
  first, as the layer's dense triple at the shared width - and the k weighted expert rows
  through the slot map, straight into the residual, then norms the row for the next layer. A
  layer without a shared expert takes the same step with the add partner off. The step sums
  the FFN row first - the gated shared row, then the slots in order - and adds it to the
  residual, the order the separate combine dispatch and the plain add it replaced took, so the
  resident-vs-CPU bars of the MoE files keep their calibration: the fold's natural order - the
  residual first - moves the rounding enough to flip a router near-tie downstream, and one step
  of the 35B two-window cell reads 1.50 logits off the CPU chain against a 1.39 bar where the
  chain's order reads 0.39. The slot loop loads eight rows together, then four, then one at a
  time - the token command's one-row form is latency, and the groups are its shape
  (`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2ag); a slot-major pass through the row stash
  instead read 2.4 ms more on the 30B window, the shared-memory read-modify-write per slot
  costing what the register sum does not. The token command's tail folds the same way; the
  residual step read those rows anyway, and the fold took one dispatch per layer out of both
  chains. There is no Q8 requant leaf: a third form would bind a ninth buffer, past the eight
  the hazard rail carries, so where the next layer's head takes the Q8 feed the f32 leaf writes
  the normed row and a separate requant follows it.

The router reads the f32 normed rows, so an MoE layer takes the split add+rms arm at the FFN
site (the fused twins never store `xb`), and the last-layer FFN slice of sec.2.2j does not apply
to the routed block. The arm's requant of the normed rows feeds the dense triple alone - the
gather takes the f32 rows - so a layer with no shared expert skips it. The tile family is the f16-fed cm2 tiles, so the plan admits a MoE only in
cm2 mode on a coopmat2 device with every expert format the f16 feed admits (`rdec_moe_ok`).

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

The beta and alpha rows take one of two arms. The f32 arm is a tile GEMM over the
`[beta ; alpha]` rows: one workgroup covers 16 positions by 16 output rows, one output per
invocation, and the grid runs over both group axes, so a layer of only `2 x nvh` rows (`nvh`,
the layer's value-head count) - 64 on the 9B - still fills the card. The q8 arm is two q8 GEMMs
and copies.

The scan is the plain per-token delta rule. One four-subgroup workgroup runs per (head, column
group). A lane keeps 16 state rows of two adjacent columns in registers, so it runs two
independent chains that interleave. The token's k and q rows are staged once per workgroup in
shared memory and feed both columns. The tokens loop inside the kernel; each token costs two
shuffle reductions per column. The raw o rows land in the per-op tier's workspace, for the
gated out-norm's one workgroup per position.

The conv history crosses windows position-major in ring image 0; the last window transposes the
tail into the decode step's per-channel layout (`dn_tail_cls`; the handoff is stated in
`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2v). Every window past the first carries at least as
many rows as the conv has taps: when a full window would leave the last window fewer rows than
that, the earlier window takes fewer rows instead, so the last one still holds the taps. Only a
lone first window can be shorter - its history is zero, so the tail writes the ring's leading
rows as zero (`DnTailArgs.zero_rows`).
