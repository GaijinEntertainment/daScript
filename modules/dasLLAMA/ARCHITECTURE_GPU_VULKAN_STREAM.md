# dasLLAMA Architecture - the Vulkan per-op tier's streamed expert layers

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document
carries the prefill of an expert layer whose stacks live on the host - the streamed layers of a
model past the card's weight budget: the GPU/CPU split of its routed experts
(`#streamed-layer-split`), the per-expert chain the GPU head runs, the pinned stacks in the device
layout the host reads through row-major kernels (`#streamed-layer-arena`), the streamed slot as a
ring of experts (`#stream-slot-ring`) and the expert cache whose misses read off the lane
(`#expert-cache`). The tier's decode era is in `ARCHITECTURE_GPU_VULKAN_DECODE.md`, the cm2
tiles and the expert chain they carry in `ARCHITECTURE_GPU_VULKAN_GEMM.md`, the model's residency
on the card in `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`. The form is both discrete homes': the Vulkan tier runs
it over its command buffers and timelines (`dasllama_vulkan_prefill.das`, `dasllama_vulkan_decode.das`), the CUDA
home over a copy stream and events (`dasllama_cuda_stream.das` - every `xf_*` of the Vulkan chain has a `cxf_*`
twin: the ring positions, a bundle's three schedules built on the host and copied on the device stream, the copy's
event waited by the chain and the chain's event waited by the copy that reuses its positions; the pool's DMA fill
flushes as an event the device stream waits), and both keep one expert cache (`dasllama_gpu_stream_cache.das`: the
stacks' pinned planes and slot tables, the resident set, the transient ring, the lane readers and the fetch; a home
supplies its pinned memory through `set_stream_pinned_fns` and sources its DMA off a slot's buffer handle or host
pointer). Where a paragraph below names a Vulkan function alone, the CUDA twin carries the same step.

### A streamed expert layer's prefill is split between the GPU and the CPU {#streamed-layer-split}

**The GPU takes the head of the routed-expert list, the CPU the tail, concurrently, and the two
meet in the middle.** A layer whose expert stacks live on the host (the streamed layers of a model
past the weight budget) used to stream every expert's planes in whole and idle the CPU; the plain
streamed rail still does that when `DASLLAMA_GPU_MOE_SPLIT` is off. On, the router's per-expert
bucket counts are walked in LPT order (largest bucket first) and the two sides claim from the two
ends of that order with no cost model (`moe_prefill_meet`, `dasllama_blocks.das`, the one form
both the hyper-connection driver's streamed rows and the plain MoE's window chain take): the device
claims bundles from the busy end as its ring frees (`XF_AHEAD` bundles in flight, the chain's open
/ push / close hooks), the host claims a batch of the rarest experts from the other end
(`RARE_EXPERTS` experts or `RARE_ROWS` rows, whichever fills first) and runs it through the rare
rail, and the window ends where the cursors meet - the faster side claims more. Where the chain
declines to open (a device with no per-expert chain), the window chain keeps the cost-model split:
the GPU claims experts while its predicted time - the claimed experts' plane bytes over the bus
rate plus its bucket rows times the cm2 chain's row cost - stays under the CPU's predicted time
for the rows left, the three constants `SPLIT_BUS_BPS`, `SPLIT_GPU_ROW_S`, `SPLIT_CPU_ROW_S` the
reference box's measurements (13.5 GB/s, 1.2 us and 10 us), and a head that would cover every
expert falls back to the plain rail.

**The host's share is the rare rail: one fork a batch, a lane runs an expert's rows whole.** The
batch's (expert, row) pairs are chunked across the lanes in expert order; a lane gathers a run of
one expert's activation rows contiguous into its own scratch and runs the run's gate and up as one
batched call each (`kq_expert_batch_core` / `b32_expert_batch_core`, `dasllama_math_gen.das`: the
batch slot's cell walk with no dispatch, the tile over the whole quads of rows and the rows core
over the plane for each row past them), the act and the requant a row, the down batched, and
parks each row into the window's slot rows. A lane's context holds every global as zero, so the
act's switches, the token block, the planes' pointers and the kernel pointers are read in the main
context and captured, and a nested dispatch from a lane is the job-queue panic. The rail is
DRAM-bound on the first row of each expert (a one-row run on the reference box costs the 2.8 MB
expert read under sixteen lanes beside the link's DMA, rows after it a fraction of that), so what
moves it is bytes an expert and which experts the host takes; `rare_hist` under
`DASLLAMA_GPU_PROF=1` logs the lanes' time per run by row count, the instrument the whole-window
rate is too noisy to replace. `DASLLAMA_RARE_ONEPASS=0` keeps the three region-list GEMV passes
with the act and requant between them, `DASLLAMA_RARE_BATCHED=0` the rows core a row inside the
one-pass structure - the two A/B arms.

**The GPU head runs an expert at a time, each expert a transfer submit and a compute submit
that waits its own copy** (`vk_moe_ffn_xf_begin_experts`). In the regions' order - the LPT
order, the busiest expert first - an expert's three plane ranges cross on the transfer queue into
the stream slot at the expert's offsets and signal the transfer timeline; its chain
(`record_xf_expert_cmd`: the by-row gather, gate and up, the act, down, the by-row combine) is
submitted on the compute queue with that value as its one wait, so the first expert computes
while the rest are still on the bus, and the layer's wall is the longer of the bus and the
compute, not their sum. The first expert's command lands the window's x rows and the combine's
planes and zeroes the accumulator; the last one copies the accumulator out, and the join
(`vk_moe_ffn_batch_xf_join`) waits the compute timeline at the last chain's value. Commands,
transfer commands and the slot pseudo-stacks' meta staging are rings of `XF_RING` entries: an
entry re-records only after its own earlier chain retired, so a layer of many experts keeps
`XF_RING` chains queued ahead of the device. One submit costs about 4 us on the reference box
(`harness/vk_submit_grain_probe.das`: a layer's 1150 MB in 512 pieces, each its own transfer and
compute submit, against one piece) against an expert chain of hundreds, so the grain is free; the
gain over one barrier for the layer is the bus-compute overlap. The chain planes - the gathered
f16 image, the hidden plane, the y planes - are chunk-local and shared by every expert's command,
so each command opens with a full barrier over the ring's earlier commands. Because the chain is
per expert, the async arm's chunk cap binds the largest bucket, not the window: a 3072-row window
at eight picks splits where one chunk of its bucket rows would not fit.

**The by-row gather and combine cost an expert its own rows alone.** The window form of both
(`moe_gather_f16_cls`, `combine_cls`) runs a workgroup per grid entry or per position and skips
what sits outside the chunk's window; an expert at a time, that would be a whole window's
workgroups per expert. The by-row stamps (`moe_gather_f16_row_cls`, `combine_row_cls`) run a
workgroup per window row and read the row's grid entry off the slot map's inverse
(`comb_bkt_dev`, built on the host beside the map): the gather copies the position's row into
the row's chunk-local slot, the combine adds the row's weighted down row onto its position's
accumulated row. The combine's rows must belong to one expert: a position picks an expert at most
once, so no two rows of one dispatch add onto one position's row, and the add needs no atomic.
A head whose region held two slots of one position would race there - the per-expert chain never
builds one, and the chain's test routes one slot a region for that reason.

**The chain's act takes the window's routed form.** The driver sets the layer's form around a streamed window
(`rdec_stream_form_set`: `moe_gpu_ffn_form` with the clamped swiglu's act code and, on a biased model, the row base of the
layer's gate and up bias rows in the resident driver's planes - `bias_resident`, the ordinal of the routed layer - beside
the window's regions' experts through `moe_gpu_ffn_experts`, since a pool region's offsets name a slot and not an
expert), and each bundle's command runs the act under it (`xf_act_enc`, the CUDA home's `cxf_act`): plain into the f16
hidden plane, or the biased twin (`actf16_b_cls`) over the bundle's rows' experts - the row-expert ring beside the grid
list, copied chunk-local into `comb_rex_dev` behind the command's barrier - and the driver's bias planes; the whole-layer
form the same through `ffn_rex_fill`'s words. A plain model sets no form, and the act is the silu or gelu the call names.

**The two partial outputs reduce into one combine.** The host's rows sit in the window's slot
rows under the weights of the experts it took (`moe_w_cpu`, zero for the device's experts), the
device's in the joined accumulator, and the caller sums the two. Where the split CAN engage -
the knob on, a cm2 device - the tier keeps ONE stream slot instead of two (`stream_slot_count`):
the per-expert copies fill it on demand per layer and never prefetch the next group, so the
second slot would only hold VRAM the resident stacks want. A device the split cannot serve keeps
both slots and the whole-group rail's overlap - compute on one while the transfer queue fills the
other. A device with no transfer family records each expert's copies at the head of its own
compute command instead, the same chain on one queue.

### A streamed stack is read off the image file into pinned memory, and the host reads it there too {#streamed-layer-arena}

**A streamed layer's expert exists once in RAM: the device-layout planes in the pinned memory the
transfer queue DMAs from.** The vulkan lane's `vkblob` section holds the device twin of every
placed expert, the streamed layers' included; a mapped load names the file and that section's
place in it (`vk_image_source_set`), the slice walk records a streamed entry's two file offsets
instead of copying its twin out of the mapping (`moe_gpu_stream_source`), and the stack reads its
planes off the file straight into its pinned memory (`stream_read_plane`: a seek and a 64-bit read),
so the mapping's expert pages are never faulted - on the reference box the 30B's warm load drops
from 8.5 to 7.7 s and its peak working set by the streamed layers' twin, 8 GB (`DASLLAMA_VK_STREAM_FILE_READ`
against `=0`, the prefill rotation rows of `PERF_LEDGER.md`). The CUDA home's own lane
(`ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint`) feeds the same cache, its misses read off that lane by the same
readers; a load with no lane behind it (`DASLLAMA_IMAGE=0`, a live regather) copies its stacks off the gathered planes
whole on either home, the budget's ask said once. A chunk-built image
has no file and copies as before (`DASLLAMA_VK_STREAM_FILE_READ=0` is the same arm on a file); the
mapped-image import (`VK_EXT_external_memory_host`) stays the first choice where a driver takes it,
and the NVIDIA Windows driver does not. Under the file-read arm the tier never tries the import: the
planes land in mirrors of the stack's own, so the mapping's window has nothing to import, and on the
NVIDIA Windows driver a refused file-backed import leaves the next host-cached allocation refused as
well.

**The host's share reads those same planes through row-major kernels where the load kept no planar
planes.** `rare_arena_serves` is the gate: a trimmed lane holds a streamed expert once, so its host picks
and window rows read the pinned mirror; where the planar planes are loaded - a lane minted untrimmed,
`DASLLAMA_IMAGE=0` - the tuned planar kernels serve them and the mirror
is the device's alone, because the row-major bodies trail the planar kernels: on the reference box (RTX 5060 Ti,
`debug-jit`) the 35B UD-Q4_K_M's host step reads 354 us a layer-token through the rail against 305 on the planar
k4/k5 kernels - tg128 60.06 against 64.52 tok/s on the CUDA home and 51.92 against 56.84 on the Vulkan home at the
strict 32k, 7% and 9% of the decode (the dasCUDA host-experts rows of `PERF_LEDGER.md`; the file's committed Vulkan
board row is `performance/records/rtx5060ti.json`'s, minted before the gate); `set_rare_arena(1)` pins the rail on the planar planes too, the formats witness's arm, and
`DASLLAMA_RARE_ARENA=2` is its environment twin, the row-major kernels' A/B arm.
Every generated CPU kernel
walks the grp<mr> interleave the CPU repack lays rows out in, and the device planes are row-major -
a row's blocks one after another - so the rare rail reads them through a second kernel shape
(`dasllama_gemm_gen.das`, `rm_rows_walk` and its bodies): a vector holds 32 weights of one row, the
lanes are k chunks, each block's lane dot folds under its scale, a row's lanes sum in order. A fold is one
fused multiply-add a block on a loop-carried accumulator, so a row's chain is latency-bound alone; the
block loop runs the body on several accumulator sets abreast (`rm_block_loop`: four in the GEMV; two in
the four-token tile on x64, whose four tokens already run four chains; one in the NEON tile, where a
second set spills its 32 vector registers; the sets summed once after the loop, the blocks past the last
whole group one set at a time); nv4 folds a strip - sixteen a superblock against q40's eight - off its sixteen scales
decoded once as vectors and picked by shuffle a block. One body
a family: q8 (int8 rows, one f16 scale a block), the per-32 and MXFP4 planes (16 nibble bytes a block
as 32 codes in k order, the codebook formats' weights off the LUT under the signed lattice, the 5-bit
codes under the unsigned one with q51's m term on the activation sum), and the superblock formats
(a superblock an iteration, its eight blocks' codes off the device's k / k+16 nibble pairing with the
fifth and high bits deposited from the block's qh bytes and the 2-bit lanes off their byte fields,
each vector's dot widened through the sub-scales a quarter, the min and offset terms as scalars off
the Q8_K sums, one fold a superblock under d times the activation scale). On the 256-bit x64 arm k4 and k5 take
the pair form (`rm_pair_codes`, `rm_sb_min_vec`): one 32-byte load holds two blocks, its low nibbles both blocks'
low strips and its high nibbles their high strips (the k / k+16 pairing), the activation's two blocks re-split the
same way, the two strips' pmaddubsw pair sums added in i16 and widened under the pair's sub-scales by one pmaddwd,
k5's fifth bits off the two blocks' eight qh bytes in one spread, and the min term one vector a superblock - the
eight mins times the eight 32-blocks' activation sums off the Q8_K strip sums - folded under -dmin x xs; the
other widths and NEON keep the block-a-vector walk. The pair form halves a core's time over the 35B's K-quant
planes (k4 80 -> 48 us, k5 100 -> 64 at n=2048 d=768, one thread); the token's host step at sixteen lanes streams a
layer's picks at the box's DRAM ceiling either way, so what the rail still trails the planar path by on a token is its
per-layer structure (the dasCUDA host-experts rows of `PERF_LEDGER.md`). Each format is its own
`[tune]` family (`<fmt>q8_rm_gemv_gen`, `dasllama_math_gen.das`, the shared perms over the maddubs,
vpdpbusd and sdot lattices, a scalar decode of the device layout the reference) with its four-token
tile as the companion (`<fmt>q8_rm_tile4_gen`: every row against four activation rows at a row's
stride, the weights decoded once a block) and a witness companion (`<fmt>q8_rm_witness_gen`), raced
by the generator half on the streamed shape alone
(`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#rm-family-race`), and the rail's device arm (`rare_plane`, `rm_dev_serves`,
`rm_dev_tile4`, `dasllama_blocks.das`) runs a run's rows four a tile and the rest a row, reading the
device block strides off `kq_qsb` / `kq_dev_ssb` (a superblock) or the plane's own block bytes
(per-32, mx4; q8 is 32 + 2) under the activation form the format reads (Q8_K a superblock plane, Q8_0
the rest); a streamed layer's planar expert planes are never touched. At the rail's many-lane,
DRAM-bound regime the row-major shape costs what the interleaved one does (llama.cpp's row-major
kernels read even with ours there, `harness/kernel_ladder.sh` at sixteen lanes), and the tile reads
an expert's planes once a tile where a GEMV a row read them once a row. The five codebook-grid
formats (iq3s, iq3xxs, iq2s, iq2xs, iq2xxs) take the superblock body with the block's weights gathered
off the module's grid table (`rm_grid_block`: the iq3 formats eight words of four magnitudes, the iq2
formats four word pairs of eight) and its signs - the plane's own sign bytes, or the ksigns rows of
its 7-bit codes - applied as one masked negate, the sub-scales 1 + 2 x the two-word row's fields or
the per-16 strips after d.
A decode token's host picks on a streamed layer read the same planes: the token window's host sum
(`moe_routed_rows_sum`) takes the rail where every plane of the layer's triple is device-served
(`rare_dev_layer`) - one region an expert, the hot pool's hits left out as before - and the
planar group GEMM only where a plane is not; `g_rare_dec_rows` counts the pairs it served, the
matrix's fed steps witnessing it. The hot pool fills a streamed layer's slot the same way: the expert's device-layout
bytes copied off the mirror (`rare_expert_slot_copy`, `g_rare_slot_copies` the matrix's pool cell's witness), the gather
the mirror took, where a resident layer's slot gathers off the planar planes. So a streamed layer's planar expert planes
serve no call of the driver at all.
The stream mirrors are host-CACHED memory: a write-combined mapping reads at tens of MB/s a core
whatever reads it, and the DMA source rate is the same either way. The host twin - the tier's own
block decode run on the CPU (`host_twin` / `host_rows`, `dasllama_gpu_host_twin.das`: the discrete templates stamped
with no dispatch home, registered by whichever home arms) - is the
test oracle of the device layout: `tests/test_vulkan_host_twin.das` holds the twin and every
row-major kernel, GEMV and tile, to the planar kernels, device-free, and the model-level matrix is
`tests/test_gpu_resident_moe_formats.das`.

The host's copies between a streamed stack's pinned planes and the rail's planes split across the job
queue's lanes (`par_memcpy`, in `PAR_COPY_CHUNK` pieces; a copy under two pieces, or with no job queue,
runs as one memcpy): a hop moves 8 to 34 MB, one lane's memcpy runs well under the box's DRAM rate, and
the lanes sit idle while the GPU owns the layer.

### The streamed slot is the chain's ring of experts {#stream-slot-ring}

**Where the per-expert chain serves every routed layer, the streamed slot holds `MOE_GPU_LAND_EXPERTS`
(128) experts of the widest planes, and each streamed region lands at the next ring position; where
it does not, the slot holds a whole layer and each expert lands at its own index.** The whole-layer
arm copies entire stacks into the slot, and the split chain used to land each expert at its index in
that same slot - one layer's planes in device memory, 476 MB on Qwen3.6-35B and 1.7 GB on Flash-Next,
of which a window ever held a few bundles. The driver decides the form once a load
(`resident_stream_need`: the split on, and the f16 feed admitting every routed layer's formats at the
prompt window), tells the tier (`set_moe_gpu_stream_ring`), and carves the slot's bytes accordingly
before the plan - the budget's probe brings the device up first, since the chain's admission asks it
- so what the ring leaves is the hot pool's (`ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`). The carve
is taken only where the model does not fit whole: the plan runs once without it, and a plan with host
layers runs again behind it (`resident_plan_carved`); where the carve leaves the uncarved plan's
mirror no room and the context is not asked whole, more routed layers go to the host at that mirror
(`resident_plan_host_layers`) rather than the driver declining to the per-op rails. Under the
ring the window's copy list names every stream region's expert (the pool window hook, with no pool
prefix), a bundle's regions take the next positions (`xf_assign_land`), its copies land there
(`xf_push_copy`, the region's position as the slot), its lists name the positions (`xf_bundle_lists`), and the copy of a bundle
waits the compute value of the last chain that read any position it lands in (`land_val`, the
transfer submit's wait) - the ring's reuse gate, where the whole-layer form had none to need. A
whole-layer copy asserts under the ring, and a meet that declines takes the same chain's begin form,
routed by the pool window's group. On Flash-Next the slot falls from 1.7 GB to 445 MB and the plan
counts it honestly for the first time: 41 slots a layer against 44 that sat 1.7 GB past the plan's
room; on Qwen3.6-35B the pool grows from 64 to 76 slots a layer, `lcpp_bench`'s frozen-fixture gate
holding 8/8 on both (the prefill rotation rows of `PERF_LEDGER.md` carry the runs).

### The streamed stacks are an expert cache, its misses read off the lane on demand {#expert-cache}

**A streamed layer keeps a quota of its experts resident in its pinned stack; the rest come off the
lane file when a window or a token picks them.** `DASLLAMA_EXPERT_CACHE_MB` is the budget over every
streamed layer's experts; the quota a layer keeps is the budget's share after one layer's complement
is set aside for the ring (`resident_cache_quota`: f = (B - ne x epb) / (total - ne x epb), the same
quota every layer), and at 0, or where the budget holds every expert, every stack is whole and the
cache is the trivial case of one path. A partial stack carries its experts' slot table
(`StreamStack.slot_of` / `expert_of`) and its planes' offsets in the lane (`qoff` / `soff`), and every
reader of an expert resolves its slot: the window chain's per-expert DMA (`xf_expert_copy`), the hot
pool's slice, and the host's rail through a view captured in the main context (`RarePlane.view`,
`MoeGpuStreamView`), since a job lane holds every hook as a null function. The resident set is the
head of the layer's heat order where the lane has a profile, the first ids otherwise, sorted
ascending so the stack keeps the lane's order and fills in runs of consecutive ids
(`stream_resident_set`, `stream_fill_slots`).

**A miss lands in the transient ring, read by a reader thread.** The ring holds `DASLLAMA_EXPERT_RING`
slots a plane kind (gate, up, down; the largest layer's complement unasked), each the kind's widest
expert rounded up a page plus one, since a slot takes its expert's lane span widened to the page
and the expert's bytes start the lane skew in (`stream_page_span`; the lane packs experts at their
natural stride). `DASLLAMA_EXPERT_READERS` threads loop on a request channel, each with its own
buffered handle of the lane and a page-aligned bounce of one slot, and answer on a second channel
(`stream_readers_make`); a request claims a slot round robin and queues the read
(`stream_request`), ready says whether it landed (`stream_ready`), and fetch is the two in turn.
The reads are buffered, not direct: the lane stays mapped for the planes the host reads, and the
kernel serializes unbuffered reads of a mapped file behind its coherency flush - eight readers at one
reader's rate, 0.77 against 1.97 GB/s unmapped on the reference box's NVMe - where buffered reads
through the same cache keep their parallelism (1.68 GB/s mapped), and the pages they leave are
standby (`harness/kq_disk_rail_probe.das` reads the kernels, the rates are the prefill rotation
rows of `PERF_LEDGER.md`). A channel's timed pop waits only while a job is outstanding, so each channel carries one
for the readers' lifetime; the readers quit at the ring's drop and at the context's shutdown
(`stream_readers_quit`, a `[finalize]`), since the job queue's teardown waits on every thread.
A reader's answer to the quit order is its last touch of either channel - it releases its two
references first - so once every reader answered the main context's reference is the last and
`channel_remove` frees the channels (a release alone never frees one: the exit-time dump would list
them at zero references). A reader that never answers keeps its references, and the channels stay with it.
Every slot frees at the window's join, the window sum's end and the decode step's end
(`stream_ring_release`), a read still in flight landing first.

**The meet claims over what has arrived.** The window's experts are queued in the list's order, the
busiest first, before the requant; the device takes bundles off the ready prefix, the host takes the
rare end of the ready prefix, and the regions still on their way rotate under the host's claim - the
list and its three offset tables together, since the chain reads offsets by region index at push
time (`rare_ready_prefix`, `moe_regions_rotate`); the decode step queues its picks before it waits.

**The profile is the picks a (layer, expert) took.** Every window's regions by their rows, every
token's picks by one (`expert_heat_add`, `dasllama_blocks.das`) are summed into `<lane>.heat` at the
model's drop and at the context's shutdown ("HEAT", the layer and expert counts, then int64 counts;
an earlier file's counts added; the layers are the trunk's and the NextN head's, `expert_heat_layer_count`,
since the head's experts route and stream like a trunk layer's), and the next arm reads it and seats each layer's hottest experts
(`expert_heat_load`, `expert_heat_order` -> `moe_gpu_stream_cache_order`). On Qwen3.6-35B at half
its experts resident the profile halves the on-demand reads and doubles the decode rate; on
Flash-Next at half, pp3000 577 and tg128 19.0 against 598 and 21.1 with every expert resident
(`lcpp_bench` at `DASLLAMA_EXPERT_CACHE_MB` half the experts' bytes against unset, the prefill rotation
rows of `PERF_LEDGER.md`). The
`gpu_prof` line `expert_cache:` reports a window's fetches, the wait on them, and the readers' busy
sum against their wall. The matrix's cache cell loads the small carrier through its lane under a
budget of half and feeds it twice, the second feed reading fewer planes under the profile.
