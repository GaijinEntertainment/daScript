# dasLLAMA Architecture - the Vulkan resident driver

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2j-2.2q, the mechanisms of the Vulkan resident driver: the prefill window
chain, how a cm2 tile decodes its quant bytes and how one is picked, the class-pipeline build
seat both shader instruments hang on, the residency plan, the marks swap that lets one GPU slot
serve many models, the Q8 requant byte store, and the MoE expert chain on the cm2 tiles. The
decode-era mechanisms of the per-op tier - the decode attention block, the streamed layer's
split, the whole-token decode span - are `ARCHITECTURE_GPU_VULKAN_DECODE.md`'s sections
2.2r-2.2t. The GPU backend role table these sections build on stays in `ARCHITECTURE_GPU.md`
sec.1.5.

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
starting 32 rows below the window's end (`fill_arena_batch_sched`'s `row0`, `ActArgs.base`,
`ArArgs.row0`). Thirty-two, not one, because the s tile's fast path loads a whole 32-row
column unclamped and the resident planes carry no read slack past the window. Rows below the
slice keep stale gate, up, hidden and residual values that nothing reads. A sliced GEMM never
splits k: the split-k reduce sums dense partial planes from row 0, so a region starting below
the window's end would reduce the wrong rows. The slice takes the f16-fed cm2 route only
(`gu6 && dn6`); the other feeds run the full window.

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

### 2.2k The cm2 decode callbacks read their quant bytes as 16-bit lanes {#cm2-decode-16bit-lanes}

A cm2 tile's decode callback runs inside the driver's block load, and the vendor driver's shader
compiler pattern-matches only one spelling into that path: a 16-bit load (`int16[N]` block
members) followed by `unpack8(w)[i & 1u]` - a byte2 lane select - with sub-fields pulled out by
shift and mask. A 32-bit word with a variable shift runs slower; an `unpack8` of a 32-bit word
indexed by a runtime value (a byte4 dynamic select) drops the whole kernel off the block-load
path, to about a third of the rate. Every cm2 decode - q8 and the thirteen kq superblock formats -
is spelled the 16-bit way, which is why the block structs are `int16` arrays over the same
bytes. The IQ4_XS codebook is the one runtime-indexed read a decode makes: it is staged into a
16-entry `@workgroup` f16 table ahead of the tile loop (the reference exe's shared-memory table-staging form),
never selected out of a register vector per element.

Every kq format's four-wide twin is hand-laid (`decode_v4`, the template's `DECV4` axis): it
keeps the same spelling and shares what four consecutive elements share. A K-quant twin reads
its four quant bytes as two 16-bit lanes and extracts the sub-block's scale pair once; a grid
format's twin looks its grid word up once and takes the four bytes and the four sign bits from
it. The synthesized twin (`DECVEC`, the axis a new format starts on) repeats the whole scalar
body four times - the lane selects, the scale-plane words, the grid lookup and the sign parity
- and on the grid formats it lost to the scalar callback for exactly that reason. The twin
computes each element in the scalar's operation order, so the tile's oracle holds bit for bit
under either callback.

### 2.2l The cm2 tile pick and the coopmat default ladder {#cm2-tile-pick-and-default}

**The l/m tile pick is a wave-efficiency comparison.** For a GEMM of width `d` over `cnt` rows
the l tile (256-row columns) and the m tile (128-row columns) each need some number of
workgroups; each grid runs in whole waves over the device's SM count, and the pick compares
occupied slots over allocated slots, cross-multiplied. The m tile wins only on a strict win; a
tie goes to l, whose bigger tile carries twice the arithmetic intensity. Three rules sit ahead
of the comparison: a region of 64 rows or fewer takes the s tile (32-row columns - the MoE
expert-bucket shape, where a 512-token window routes ~32 rows to each of 128 experts and an m
column would pad three quarters of every tile and take the edge path on all of them), a window
of 128 rows or fewer takes m (the l column would run half empty), and a device that reports no
SM count takes l and never splits k. The pick is PURE in `(d, cnt, sm_count)`, so the class the
pipeline binds and the tile rule the meta fill writes can never disagree; `cnt` is the AVERAGE
rows per active region of the dispatch, so one tile serves every region of a MoE schedule. The
narrow-n end below s is GEMV's. The s tile's fast path loads a partial 32-row column UNCLAMPED
and clamps only the store, so every f16 plane the chain feeds it - the gathered activation
image and the hidden plane - is sized with 32 rows of slack past its last region
(`ffn_cm2_chunk_rows`).

**The split-k pick counts the dispatch group, not the GEMM.** Long K (2048 and up) on a grid
that would fill under half the SMs splits the reduction across f32 partial planes that
`SplitKReduce` sums (three chunks up to two thirds full, at most eight, each chunk 256-aligned
and a split that would strand an empty tail shed). The grid it measures is the role's own
workgroups PLUS those of the chain neighbours it runs beside - q with k and v, gate with up
(`cm2_tiles`, the same pick each neighbour's own dispatch makes) - because the hazard-mask rail
lets independent roles co-run, while every split role serializes through the one scratch plane
(`VHZ_SK`) its neighbours would also claim. Counted alone, a 512-wide k or v projection over a
512-row window fills 16 of 36 SMs and splits in two; counted beside q it runs whole, and k and v
fill the device together. Split-k is left to the lone role - wo, down, a small model's
classifier - whose grid nothing else pads.

**The f16 feed admits q8 and every kq superblock format** (`kq_sb`) - the set the cm2 decode
callbacks cover (sec.2.2k) - and each (format, tile) pair has ONE stamped class. The
prefill driver reaches them through one dispatcher per stage (`cm2_cls_ensure`, `cm2_cls_set`,
`cm2_cls_enc`), all three keyed on the same `(fmt, ml)` pair, so the pipeline a role ensures,
the set it binds and the kernel it encodes can never be three different classes. The three are
not private: the per-format arm of `harness/vk_gemm_probe.das` drives the same ladders, so a
probe row times the class the driver would serve rather than a copy of it. The decode
GEMV keeps its quant chains: the feed format pick is decoupled from the weight format.

**The served GEMM mode resolves once, at init, through one ladder.** cm2 where the device has
NV_cooperative_matrix2, else mm where it has KHR_cooperative_matrix, else sdot4;
`DASLLAMA_COOPMAT` overrides the ladder by name, and a cm2 request or force on a device without
the extension lands on mm. The same resolver stamps the mode into the `.dlim` flavor
configuration, so the recorded mode and the running mode cannot drift. The four-wide decode
callback is NOT in that configuration: a cm2 tile names both callbacks
(`coopmatLoadTensorDecode`'s tenth argument: the format's own `decode_v4` where the template's
`DECV4` axis is on - every kq superblock format today, sec.2.2k - else the `DECVEC` axis,
which synthesizes the twin from the scalar body and is where a new format starts, its
`cm2:<fmt>` probe row deciding whether a hand-laid twin is owed), the device created with
`DASLLAMA_VK_DECVEC` and the extension decides which
one the driver runs, and neither choice shapes an image byte, so the bake identity ignores it
(the configuration's own rule: a serve-only knob is never a field). `decvec_on` is the run's arm,
announced on the `device ready` line.

**The tile's fast path is what makes the loads unclamped.** It runs when the weight tile is
whole (`m0 + 128 <= d`), the token column is whole or stamped s, and K is a whole number of BK
steps; the layouts are then created clamp-Undefined and the B and output strides are masked to
a multiple of 8 f16 (`stride &= ~7`). The mask is an identity on today's shapes - `n` and `d`
are 32-multiples - and it exists to make the alignment PROVABLE to the driver's address
analysis, which is what keeps the loads on the wide path. The s column gates only the weight
tile: its partial token column loads unclamped and its store clamps. Everything else takes the
edge path with clamped layouts.

**The no-split arm keeps literal loop bounds and a literal store base.** Where `ksplit` is zero
the k loop runs the literal `0 .. n` with the store at the row base rather than the general
`k0`/`k1`/`ybase` form, although those values are exactly `0`, `n` and `0` on that path: the
general spelling cost 27% of prefill throughput (`benchmarks/lcpp_bench.das` pp512, 5060 Ti).
The split arm keeps the general form.

### 2.2m Class-pipeline creation is the Vulkan tier's one shader A/B seat {#vk-class-pipeline-build}

`vkd_class_pipe` is the single place a class kernel's SPIR-V becomes a pipeline, so both shader
instruments hang there and nothing else has to know about them. The four-wide decode fallback
hangs there too: when the device was created without `VK_NV_cooperative_matrix_decode_vector`
(`decvec_on` false) the served words go through `strip_decode_vector` (the capability, the
extension and every load's `DecodeVectorFunc` operand removed, the scalar callback left to
serve), after the override and before the shader module, so a dumped or overridden blob is
always the emitted, unstripped one. The seat is also the in-process A/B: `vkd_pipes_rebuild`
marks every class slot stale, so the next ensure rebuilds it under whatever `decvec_on` says,
which is how the `cm2:<fmt>` probe runs both arms interleaved in one process.

**The dump runs before the override.** `DASLLAMA_VK_SPV_DUMP=<dir>` writes the EMITTED words as
`<dir>/<kernel>.spv`; `DASLLAMA_VK_SPV_OVERRIDE=<dir>` then replaces them with that directory's
file. The order is what makes the pair a round trip: dump a kernel, edit or spirv-opt the file,
serve it back. A dump taken after the override would capture the served words, not the emitted
ones.

**Full subgroups are a whole-run arm, never a per-pipeline one.** `DASLLAMA_VK_FULLSG` plus a
device that reports the feature sets `g_gpu.full_sg_on` once at device init, and every class
pipeline is then built with `REQUIRE_FULL_SUBGROUPS`. A run never mixes pinned and plain
pipelines, so an A/B compares two whole runs. Plain is the default: pinned measured slower on
the mm_a gate shape.

### 2.2n The residency plan sizes a whole model before a byte uploads {#resident-plan}

The resident driver is all-or-nothing, so the plan IS the decision, and it is computed from
`Model` metadata alone. It sizes four numbers against the tier's weight budget: the dense weight
planes, the KV mirror at `seq_cap`, the driver's own device scratch, and the headroom the auto
arm leaves unfilled (zero when the user pins VRAM). KV is reserved BEFORE weights and never
grows: on a discrete card the two compete directly, and evicting weights to grow KV would mean
re-uploading gigabytes. A decline carries a reason, and where the numbers allow one it carries
the remedy that works - a shorter context, because the weights are fixed and the KV is not.

An OPTIONAL plane rides only the room left under the budget at THIS context - what remains of
`budget_bytes - headroom_bytes` after weights, KV and scratch; the reserved headroom itself
stays unfilled. It never shrinks any of the three, and it reports zero bytes when it does not
fit - so the same model plans the plane in at a short context and out at a long one. The raw f32 embed
table is the one optional plane today.

### 2.2o One GPU slot, many models: the marks swap {#gpu-slot-marks}

A multi-model host runs one device tier under several loaded models, and the tier's per-model
state is offset-keyed - two models' marks installed together route one model's dispatches at
the other's planes. `GpuModelMarks` is that state WHOLE: the loader-contract marks plus every
resident-driver per-model global (the activation, the mirror count, the mirror cap, the mirror
codec, and the device-embed arm). The save moves the installed state out and leaves the globals
reading as no-model; the restore is its exact inverse. The whole-model drop clears the same set
and deselects the `"vulkan"` overrides, so a dropped model's prefill and decode take the plain
CPU path and a later re-arm passes `resident_upload`'s no-active-override gate. The three carry
the same set, which is why a model's device state never survives into the next.

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

### 2.2q The MoE expert batch arm rides the cm2 tiles through a device-side f16 gather {#cm2-expert-chain}

The per-op tier's expert FFN batch arm has two forms over the same region schedule. The quant
form takes the CPU's gathered activation image (the engine requantizes the normed rows, then
copies each bucket row's quants into expert order) and encodes the kq batch tiles. The f16 form
takes the window's f32 activation rows themselves - one per position - plus the combine's slot
map, and does the gather on the device: one workgroup per (position, slot) grid entry scatters
its position's row as f16 into the entry's bucket row, the inverse walk of the combine over the
same map. Gate and up then run the cm2 decode-in-load tiles over that f16 image, the act writes
the hidden plane as f16, and down runs the cm2 tiles again - the resident dense chain's
`pf_gemm_enc` feed, with the region records the quant form already fills. The engine asks the
tier per layer (`moe_gpu_ffn_xf_ok`): the answer is yes only in mode 4 on a coopmat2 device, for
a gate/up/down triple whose every format the f16 feed admits (sec.2.2l), with the window inside
the x plane's cap - and on yes it skips its own requant and gather, so the CPU cost of the
layer's FFN is the routing alone. The f16 form is the combined (`npos > 0`) form only: the
combine is what makes the device-side gather pay, since neither the gathered image nor the
bucket rows ever cross PCIe. Streamed groups take the same arm after the slot bind.

**The per-op attention chain runs the same cm2 flash-attention tile the resident chain runs**
(`fa_cm2_h64` / `h128`, sec.2.2j) when the device carries the coopmat2-fa trio, the fa knob is
on, the head size is one the tile family stamps, and the model's attention is not gated - the
tile has no gated epilogue, so gated models keep the flash-style `at_attn` pass. The tile reads
f16 K/V: the chain keeps its f32 roped-k / raw-v planes at absolute positions for the host
readback the CPU cache store consumes, and fills f16 shadows of them with the base-less
`f16cvt` over the whole attended prefix each window; the fa output lands in the same out plane
`at_attn` writes, so the requant and `wo` stages never learn which pass ran.
