# dasLLAMA Architecture - the Vulkan resident driver

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document
carries the prefill window chain, the decode GEMV family's grid
codebook buffer, the tile probe's shared descriptor set layout, the roster of Vulkan capabilities the tier keys its routes on, and how the
`[vk_dispatch]` lens derives `readonly` from a class family's accesses. The Q8 requant byte store is
`ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store`, and the flash-attention tile the window's attention runs is
`ARCHITECTURE_GPU_SHARED_ATTN.md#flash-attn-tile`. The token command's
attention and the K/V mirror's codecs are in `ARCHITECTURE_GPU_SHARED_ATTN.md`, the attention-side planes the driver
uploads in `ARCHITECTURE_GPU_VULKAN_ATTN.md`; what the window does a session at a time
is in `ARCHITECTURE_GPU_VULKAN_WINDOW.md`. The routed block of that window and of the token command is in
`ARCHITECTURE_GPU_VULKAN_MOE.md`. The tile pick and
the coopmat mode ladder, the short window's split-K (`ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split`), the class-pipeline build seat and the MoE expert chain on the cooperative-matrix tiles
are in `ARCHITECTURE_GPU_VULKAN_GEMM.md`, the f32 rows the KHR arm serves
in `ARCHITECTURE_GPU_VULKAN_KHR.md`; the tile bodies - the cm2 decode spelling, the KHR arm's hand-staged tile - and the decode GEMV
family's lane split by row length (`ARCHITECTURE_GPU_SHARED_GEMM.md#kq-gemv-lanes`) are in `ARCHITECTURE_GPU_SHARED_GEMM.md`. What a
model has to fit on the card before any of this runs is in `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md` (the room
it may fill on a shared card, `ARCHITECTURE_GPU_VULKAN_ROOM.md`), the NextN
draft head the driver homes is `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`, and the
N-row token command a batched step's rows go through is in `ARCHITECTURE_GPU_VULKAN_NROW.md`, and
the hyper-connection chain with its host-served routed experts is `ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`.
The decode-era mechanisms of the per-op tier are in `ARCHITECTURE_GPU_VULKAN_DECODE.md`. The GPU
backend role table these sections build on stays in `ARCHITECTURE_GPU.md#gpu-backends`.

The module gate's seven Vulkan checks (`REVIEW.das`) read these files. `check_khr_stage16_abstract`
reads `class template KqCm2BatchT` in `dasllama_gpu_kernels_discrete.das` and licenses no names: its
`khr_stage16` is declared abstract. `check_ar_max_dim_triple` reads `RDEC_AR_MAX_DIM` in
`dasllama_gpu_tier.das`, the `row` slab of `ArBase` in `dasllama_gpu_kernels_discrete.das` and the
`c.dim` cap of `attn_dec_shape_ok` in `dasllama_blocks.das`, and licenses no names: the three
numbers agree. `check_cm2_ladder_sets` walks every `class template <Fmt>Cm2T : KqCm2BatchT` in
`dasllama_gpu_kernels_discrete.das` twice (the KHR tile, the e column) and requires the engine's one ladder trio
(`cm2_cls_*`) stamped by `kq_tile_stamp(fmt, CM2_TILE_NONE, ...)` over its `tile_<verb>()` placeholders, with no
`<fmt>/<tail>` pair of the `CM2_TILE_NONE` skip list naming a format that has a template on the cm2 base for
the walked tile, and the kernel cells' KHR ladder (`khr_cls_*`) stamped over `tile_<verb>_khr_cls()` - the stamp
walks every `KqFmt` member, so a format with no class fails the compile, and a skipped pair would panic at its
first dispatch instead. The KHR trio needs `<Fmt>KhrBatch` and its `kq_batch_<fmt>_khr_cls` stamp (per-32:
`<fmt>_batch_khr_cls` - `Q8KhrBatch`, `Q51KhrBatch`, `Mx4KhrBatch`, `Iq4nl32KhrBatch`; the expert schedule rides that tile in mm mode); the e
trio reads `KQ_CM2E_ALIASES_M` (`dasllama_kqformat.das`): a format whose s stamp steps k by 64 is on it and ships no
`<Fmt>Cm2EBatch` (its e column is the m stamp byte for byte), a 32-step one is off it and ships `<Fmt>Cm2EBatch` with its `kq_batch_<fmt>_cm2e_cls` (per-32 `<fmt>_batch_cm2e_cls`) stamp; none licensed.
`check_cm2_stamp_tiles` reads every `[vk_dispatch]` stamp of those templates - in
`dasllama_vulkan_classes.das`, the probe's twins in `harness/vk_gemm_probe.das` and the bring-up
fixture `tests/_vkd_toy.das` - and requires its `AT`, `BT`, `ACC` and `ACCW` typedefs to follow
its `BK` and `BN`, an e or s stamp's `BN` to equal `SCHED_M_ROWS` or `SCHED_S_ROWS`, and a
scale-caching stamp's `BLKW` to equal `BK x UNR`; it licenses no names. `check_kq_gemv_grid_literals` holds every grid format's `<Fmt>GemvT` literal `GRID_WORDS` / `GRID_OFF` to the `KQ_GRID_<FMT>` chain the host fill reads (`ARCHITECTURE_GPU_VULKAN.md#kq-gemv-grid-buffer`); it licenses no names. `check_vk_extension_roster`
walks `dasllama/` for every `"VK_*"` extension name and every `*_supported` probe
`modules/dasVulkan/daslib/vulkan_boost.das` declares, and requires each inside `vk_ext_roster` in
`dasllama_vulkan_common.das`, licensing no names.
`check_no_hand_pipelines` walks `dasllama/`, `harness/` and `tests/` for a
`vkCreateComputePipelines(` call and licenses no names inside them; the two llama.cpp shader ports
under `performance/` (`coopmat_mulmm_reference.das`, `coopmat_mulmm_port.das`) sit outside the
walk as reference measurements of another engine's kernels.

### The Vulkan resident prefill window chain {#vk-prefill-window-chain}

**A prompt longer than the prompt window (`pf_window_rows`) runs as SEQUENTIAL windows over the same activation buffers.** Every
window's rope and attention address the KV mirror at ABSOLUTE positions, so window w attends everything the
earlier windows stored; only a window holding a segment's last row - a one-session call's last window - runs the
final requant and the classifier at that row.

**A prefill from a position past zero is the same chain entered later.** The call names its first position
(`pos0`), the windows store at and attend from `pos0 + w0`, and the rows `[0, pos0)` are whatever the session's
mirror region already holds - a chat's earlier turns, a scheduler's earlier chunks (`rdec_prefill_region`: kept
where the session owns them, brought up from the host cache - a paged one a run at a time, `rdec_host_rows` - where it does
not). A recurrent continuation starts from the live state at `dn_pos`: the host binds the region's state slots to the session
as before a token (`rdec_dn_prefill_entry`) and the first window reads the ring the parity names (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block`); one
at another position passes as `rewind`, a hyper-connection model's as `continuation` (its n-gram ring has no hand-off).

**The last layer's FFN runs on a one-segment window's last 32 rows only (a segmented window's every segment
wants its own last row, so it runs the FFN whole).** Nothing downstream of the final layer reads more than the
last row - the classifier requantizes row `wlen - 1`, the KV mirrors are stored before the FFN, and a later
window starts from fresh embeddings - so the gate, up and down GEMMs, the activation and the residual step of
the last layer take a region starting 32 rows below the window's end (`fill_arena_batch_sched`'s `row0`,
`ActArgs.elem0` - `ARCHITECTURE_GPU_SHARED_ROWS.md#act-row-base` - and `ArArgs.row0`). Thirty-two, not one, because the s tile - the cm2 tile with 32-row columns
(`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`) - loads a whole 32-row column unclamped on its
fast path, and the resident prefill's activation planes (`pf_xf`, `pf_hf`) carry no read slack past the window -
unlike the MoE chain's gathered image and hidden plane, which
`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default` sizes with 128 rows of slack past their last region
(`TILE_READ_SLACK`). Rows below the slice keep stale gate, up, hidden and residual values that nothing reads.
The sliced GEMMs do not split k: the split-k reduce sums partial planes from row 0, so a region starting below
the window's end would reduce the wrong rows. The slice takes the f16-fed cm2 route only (`gu6 && dn6`); the
other feeds run the full window, and so does a window that warms the NextN head's slab, whose rows read every
row's post-norm hidden (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm`). Only the plain residual
step (`cls_ar`) and the f16 activation honor the row base: the fused residual twins feed the NEXT layer's
projections and never run on the last layer, so they index from row 0 by design.

**A q/k-normed layer's norm, rope and K/V store run as the token command's one fused pass (`QknRopeKvT`'s window form,
`pf_qkn_fused`)** over a window part's rows at their positions of their region's slab - a shared-KV layer's pass carries its q
heads alone, no k head to store; a block-codec mirror and the lever `set_vulkan_prefill_qkn_fuse(false)` keep the norm pass and
the rope-and-store pass. The two forms reduce the norm
in another order, so they agree within the split bar, not bit for bit.

**The k and v GEMMs merge into ONE dispatch when the layer's q, k and v weight planes are all
q8 and the k and v planes sit adjacent in the arena.** The bump allocator places them
back-to-back unless a slab boundary intervenes, so the merged form asks only those two
questions and dispatches at `d = 2 * kvd`: one dispatch instead of two plus a copy, which
doubles the otherwise starved k/v grid and deletes the v copy. Consumers read the merged output
through a `kstride` field - the projection-row stride, `2 * kvd` merged against the split
path's `kvd` - on `RopeKvBArgs` and on `GkQkRmsArgs`, and through `RopeKvBArgs.voff`, the v
rows' base inside that buffer - `kvd` merged against the split path's `npos * kvd`
(`ARCHITECTURE_GPU_SHARED_ATTN.md#rope-kv-merged-reads`). It is a record-cost win, not a GPU one.

**A window chain submits in chunks so recording overlaps execution.** The chain splits on a
1, 2, 4, 8-layer ramp, so the GPU starts on an early small chunk while the CPU is still
recording, and the doubling stops at the chunk cap (`pf_chunk_cap`): 8 layers on a discrete
GPU, one layer on an integrated one, `DASLLAMA_VK_CHUNK` naming another. The cap is the device's
job watchdog in disguise - a Linux amdgpu resets the device under a job past about 10 s, and an
iGPU's 8-layer chunk of a 512-row window reaches that at a long context, where a discrete card's
chunk stays at milliseconds. Chunks go out through `submit_nofence`; the
window's last submit is the `submit_wait` that signals the one fence the caller waits on.
Ordering between chunks is the hazard rail's: a barrier recorded in chunk N+1 covers the
writes of chunk N because submission order on one queue spans submits, and the terminal
fence covers every earlier submit the same way. The command-buffer ring holds
`3 + ceil(depth/cap)` buffers, so chunk N records into a free buffer while chunk N-1 executes;
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

The flash-attention tile the window's attention runs is `ARCHITECTURE_GPU_SHARED_ATTN.md#flash-attn-tile`.

**A hybrid's gated attention rides the batch kernels through a per-head q stride** (`qhs = 2 x hs`):
the q GEMM writes `[q | gate]` per head, qk-rms and rope read q head-strided in place, the mirror
attention gates on the sigmoid of the gate half (tq4's un-rotation in its place), a partial-rope model rotates a head's first `rot` elements, and at head size 256 the gated twins take the h256 cm2 flash stamps (Br 64, Bc 32), q at the head's q stride, the normalized output gated before the store.

### A per-32 plane's window GEMM is a cm2 column or the per-32 batch tile {#vk-b32-window-gemm}

A dense plane in a per-32 format (q51, q5032, q4032, iq4nl32) stamps its format's `<Fmt>Cm2T` at all three columns, so a
window GEMM over it on the f16 feed takes the column and the k split the shape picks (`pf_split_pick`, the same wave model
as a superblock format's), and the KHR tile in KHR mode; the s column is also the expert schedule's 32-row pieces. mx4 is an
expert-only form with no dense plane, so it stamps the s column alone (`cm2_s_only`) and the pool's chain takes that column
whole. The l and m columns run a per-32 plane at a K-quant plane's rate - a 2048 x 2048 window GEMM at 256 rows on the RTX
5060 Ti reads 97 us on the l column and 112 on m against 214 on the s column, beside the k4 tiles' 92 and 104 - where the
s column alone ran the Q5_1 file's prefill at six tenths of the Q4_K_M file's. Off the f16 feed - an sdot4, f16 or int8 card, or a group the FFN
lever keeps on the Q8 feed - the rows quantize to Q8_0 blocks and the GEMM is the per-32 batch tile (`Blk32BatchT`:
`Q8Batch`'s 32x32 sdot4 tile and stage, each staged half block the GEMV's byte lanes, `blk32_*_quad`, and the block's scale
pair beside it), which the kq batch rail dispatches by format (`kq_batch_cls_enc_for`, under `blk32_rail`). The decode steps
read the same planes through the per-32 GEMVs and their N-column leaves, so every site of such a model serves the plane's
own bytes.

### The decode GEMV family stages its grid codebooks from one device buffer {#kq-gemv-grid-buffer}

The grid formats' codebooks (iq2s, iq2xs, iq2xxs, iq3s, iq3xxs) live in one 17 KB model-owned
device buffer, bound at binding 6 of the kq GEMV family and built on the family's first set
(`kq_grid_dev`); each format's table sits at a fixed word offset (`KQ_GRID_<FMT>`), and a grid format's GEMV template
(`<Fmt>GemvT : KqGemvLeafT`, `override GRID = true`, its `GRID_WORDS` / `GRID_OFF` literals held to that chain by
`check_kq_gemv_grid_literals`) stages its table into workgroup memory from that buffer - a coalesced 256 B read per 64 lanes. The tables also exist as per-index accessors over a `fixed_array` local
(`iq2s_grid_word` and kin), which the batch and cm2 tiles stage from, because a tile amortizes
one stage over 128 rows x 64 columns. A per-row kernel cannot: the emitter lowers such an
accessor to a constant composite stored into a Function variable, the driver serves the
per-lane indexed read of it serially, and a two-row GEMV workgroup pays that serial read on
every 2 x 5120 weights it walks - 84 GB/s for iq2s against k4's 410. The
buffer form puts every grid GEMV in the k-format band (iq2s 388, iq2xs 407, iq2xxs 400,
iq3s 415 GB/s on the reference card, `harness/vk_gemv_probe.das`). The buffer dies in the
model-drop sweep with every other device buffer, and its handle zeroes there, so the next
model's first kq set rebuilds it.

### The tile probe's arms share one descriptor set layout {#khrx-shared-set-layout}

`khrx` is the lever sweep of the KHR kq tile (`ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile`) in
`harness/vk_gemm_probe.das` (`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`): eight arms
timed over one shape. Two are shipped bodies - the KHR class, the reference each compared arm
is checked against, and the sdot4 kq tile. `ship` is the KHR tile copied with no lever moved,
the control the lever arms read against, and the four lever arms are that copy with one lever -
the weight stage (`dec4`, the four-wide decode callback in place of the word stage), the
accumulator width, the subgroup tiling, the workgroup size - moved back to its simpler form.
Every arm but the sdot4 one binds the same five-buffer descriptor set layout as the shipped
class, so an arm's figure differs from the shipped class's by its body alone and never by a
binding difference. The sdot4 arm reads its own Q8 activation fixture, so it binds that
class's own six-buffer set, and it is timed rather than compared.

The `nil` arm is the ceiling arm: its weight stage writes constants, so its rate is the
tile's ceiling with the weight loads removed. It still binds the weight plane and the scale
words the shared layout declares, and its stage reads neither (`KhrPxNil`, whose `wq` field
carries `@role = "alias"`). Those two are the probe's deliberately unread bindings.

### The device-init log names every Vulkan capability the tier keys a route on {#vk-extension-roster}

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
same probe - the kernel file's roster cell holds the arming's fields to the roster's entries. The module
gate keeps the roster complete: every extension name and every such probe the tier calls appears in it.

### The `[vk_dispatch]` lens derives `readonly` from the class family's accesses {#vk-readonly-lens}

Every `@ssbo` member on a binding no `[spirv_kernel]` method of the class writes gains
`readonly`, which the emitter decorates NonWritable, so the driver orders no load of that
binding behind a store. The verdict is the family's: a field is one declaration for every
kernel of the class, so a binding one method writes is written for all of them, and a written
view protects its same-binding aliases. A method body the access classifier refuses counts as
writing every binding, so a body the classifier cannot read loses the decoration rather than
carrying a false one. A declared `@readonly` on a binding a kernel writes is the one shape that
yields a module the validator rejects, and the lens refuses it.
