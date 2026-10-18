# dasLLAMA Architecture - the whole-model driver's hot expert pool

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries the
hot expert pool a routed layer keeps on the device beside the split token command and the window
chain of `ARCHITECTURE_GPU_VULKAN_HC.md`: how the plan sizes it, how the host places and swaps its
experts, how a decode step's and a window's picks split between its slots and the host, the fill that
lands a step's misses by DMA, and the prompt window that serves the pool's hits and leaves it warm.
The streamed expert stacks the pool draws on, and the slot the window chain streams through, are
`ARCHITECTURE_GPU_VULKAN_STREAM.md`'s.

### The hot expert pool {#hc-hot-pool}

**A routed layer keeps its hot experts in device slots, and a step's picks split: the device runs the
hits while the host sums the misses.** The carrier has a working set - a layer's picks fall on a few
of its 512 experts far more often than on the rest - and the device decodes an expert's planes six to
ten times faster than the host does (`iq3s4` at the gate shape: 7 us a cold plane on the reference
card, the host's chain 61 us an expert), so the slots the card has room for pay. A pool is `slots`
slots a routed layer (`RdHotLayer`: a gate, an up and a down plane pair, each `slots` experts in the
plane's device layout, a slot's stride the expert's weight blocks), and the same count on every layer
with host experts - every routed layer of a hyper-connection model, the first `host_layers` of a plain
MoE the card does not hold whole at a context asked whole or that an ask names; a layer whose experts the
card holds needs none. A host-layers ask (`set_gpu_host_layers`, `DASLLAMA_GPU_HOST_LAYERS`) moves the first
that many routed layers' expert stacks to the host whatever the card holds, capped at the routed layer count
(`resident_plan_pick`): the form, its split token command and its cut window chain run on any card, not only
on one whose room forces them. Under a strict pin (`set_gpu_ctx_strict`, the server's `--ctx`), where the
mirror at the asked context leaves no room for every expert stack, the plan (`resident_plan_host_layers`)
moves the fewest routed layers' expert stacks - the first in layer order - for the rest to fit
(`resident_host_layers_fit`), and never fewer than the ask; the pool over those layers takes what is left.
With neither, a plain MoE the card does not hold whole stays with the per-op rails, which decode faster at every context (Qwen3-30B-A3B
Q4_K_M on the reference card, `debug-jit`: tg128 64.0 against this form's 48.4, tg64 at 8192 positions 64.5
against 46.8) and lose only the long prompt (pp8192 93.4 against this form's flat 431-534; `PERF_LEDGER.md`'s
host-layers entry carries the rows).
The count is the resident plan's (`ResidentPlan.hot_slots`, `resident_hot_slots`), sized with the
mirror because the two share the card's room. Unasked, the pool takes every slot that fits beside
the mirror at the context asked, up to the layer's experts (the plan's room keeps the card's headroom,
so a full pool does not page a desktop beside it); where none fit, it takes up to 32 out of the mirror's context,
which keeps a floor of 32768 positions - a slot the device serves is worth more than context past the
floor, and a plan that fills the card to its cap beside a desktop pages (the carrier's decode reads
2.9 tok/s so, 30 with the pool and a mirror of 88 thousand positions). `DASLLAMA_GPU_HEAT` asks a
count, served from the same floor's room, and 0 asks none. A context asked whole (`set_gpu_ctx_strict`,
the server's `--ctx`) is the floor itself: the pool counts its slots out of what the asked mirror leaves
(`resident_hot_floor_ctx`, scaled by eight sevenths for the re-plan's seven eighths), down to none, and
the driver declines only where the dense planes and the asked mirror alone pass the card. The pick shortens the mirror's context to
what the pool leaves and holds the pool at the count the first plan settled on, so the slack of the
shortened context stays slack. Under four slots no pool arms. The chain a window's hits ride
(`RDEC_HOT_CHAIN_BYTES`, 320 MB of scratch) is taken off the room before the slots are counted and
charged to the plan only with a pool (`resident_hot_fit`): a plan with no pool - none asked, or none
that fits - keeps the mirror's context and the room a model needs to fit. The tier's own carve - the
streamed slot and the decode block's mirrors, which the tier allocates outside the plan's total - comes
off the OS's room as well as the budget (`moe_gpu_weight_carve_`, `plan_room`), so the pool's count is
what the card holds beside them.

**The host owns the policy.** It holds, a layer, the slot of every expert and the expert of every slot
(`HotLayer`) and a pick count an expert that halves every 256 routed rows. After a window's host step
on a layer the rows' picks are counted and the free slots fill, hottest expert first
(`rdec_hot_window`); a full pool swaps at most eight experts a window and one a decode step, and only
an expert whose count passes twice the coldest slot's plus one - a swap costs a gather and an upload
of 2.6 MiB, more than the host's sum of the same expert. An upload is the loader's gather of the
expert's rows (`layout_gather_dispatch`, the bytes the arena would hold) written into the slot
(`vk_rdec_hot_upload`); it runs between a segment's wait and the layer's hit chain, so no command reads
a slot while it is written.

**A decode step's hits run in a command of their own.** After a segment lands the FFN-mixed row and
the router logits, the host selects, counts, admits, and splits the picks (`rdec_hot_step`): the hits'
slots and weights go to the device (`vk_rdec_hot_submit` writes the region records of the three GEMVs
and the weights into host-visible buffers the sets bind, zero weights past the hit count), the misses
pack to the front of the session's picks and the host sums them (`moe_routed_sum_k`). The hit chain is
the routed block's own - the row's feed requant in the gate plane's form, the gate and up GEMVs over
`nhit` regions, the act with the down feed's requant, the down GEMV into the routed rows - recorded
once a layer a hit count (`RdHotLayer.cmds`, k + 1 forms, on first use) and submitted with no fence, so
it runs while the host sums. The next segment opens by copying the host's sum into the routed sum's row
(`hacc_dev`) and adding the hit rows under their weights (`MoeCombine` with its slot window at 1 .. k
over slot rows 1 .. k, so the sum accumulates onto the row instead of starting it; a zero weight skips
its row, which is stale). The hit chain's rows are a compute write no fence covered, so that segment's
hazard tracker starts with them pending and its first access takes a barrier
(`rd_encode_routed_sum`). A step with no hit submits nothing and its zero weights leave the combine
adding nothing.

**A window's hits run through the tier's expert chain, beside the host.** The pool's planes are
stacks of the tier (`make_stack_shell` at `HOT_BASE` plus the layer's stride, a plane a stack, a slot
an expert's elements), so the batch arm's f16 chain (`vk_moe_ffn_batch_xf_begin`,
`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain`) finds them by offset like any resident stack. At
a routed layer's host step the host selects the window's rows, counts and fills or swaps the pool
first - so the window that warmed a slot reads it - and splits the picks (`rdec_hot_regions`): the
experts in the pool are the device's, their buckets its regions over the pool's planes (biggest
first), their slot rows' weights in the device's plane and zero in the host's; the chain takes the
window's rows, gathers the bucket rows as f16, runs the gate, up and down tiles over the regions,
combines under the device's weights and lands the rows on the host, all in one submit the host does
not wait for. The host sums the rest (`moe_routed_rows_sum` under `split`: the pool's experts take
no chain, a zero weight's parked row is unread), joins the chain and adds its rows
(`rdec_hot_rows_join`). A window under 32 rows, a window past the chain's one chunk, and a layer
whose formats the chain's f16 feed does not admit stay the host's whole. A per-32 expert plane
(q51, mx4, iq4nl32) takes the chain's s column alone (`batch_tile_edges`): its one cm2 stamp is the
32-row tile.

**The pool's policy is picked offline, on a trace of the routed picks.** Under
`DASLLAMA_MOE_TRACE=<file>` every host expert step records its picks (`rdec_trace_picks`): a record
is a layer, its row count, then k expert ids a row and k weights a row; a record of layer -1 with no
rows marks a prompt that starts at position 0. The file (`rdec_trace_write`) opens with the magic
`MOET` and three int32s - the routed layers, the expert count and k, set where the driver arms - and
holds the records to its end: the first flush writes the header and every later flush appends, the
last at the model drop. `harness/hot_pool_sim.das` replays a trace through a policy at a slot count;
the live run's per-layer line under `DASLLAMA_GPU_PROF` (`rdec_hot_say`: a layer's hit share and
swaps since the pool armed, where the misses sit) is its cross-check.

### A decode step's misses land in the pool by DMA {#pool-dma-fill}

**Under `DASLLAMA_GPU_HOT_DMA` every miss of a decode step is copied off its pinned stream stack into
the pool's coldest unpicked slot on the transfer queue, so the next token finds it, and the host spends
nothing on the fill.** The host's fill gathers an expert's three planes into scratch and uploads them
through staging under a budget of a few experts a step and a heat gate of twice the victim's count; the
DMA fill (`rdec_hot_fill_dma`, `vk_rdec_hot_dma`) records the three copies of each miss into a batch a
layer on a ring of four transfer commands (`HOT_DMA_RING`), submits it after the step's last copy, and
the layer's next hit chain waits the batch's landing through the pending transfer value
(`RdHotLayer.dma_value`, `rd_hot_go`). The victim is the coldest slot whose expert the step does not
pick, free slots first, taken where the miss's count reaches the slot expert's; a slot the step reads
is never a landing. A copy the tier cannot make - a stream stack the expert cache does not hold whole,
a slot and a streamed expert that differ in bytes - takes the host's gather and upload instead, said
once a layer. A prompt window's fill stays the host's place under the knob: a window streams its stacks
on the same transfer queue, and a DMA fill beside them cost the prompt a fifth. The pool's summary
line counts the experts the DMA placed. On Qwen3.6-35B with every expert resident the decode harness
reads 19.8 ms a token against 23.6, the pool serving 97.5% of the picks against 78.7%; on Flash-Next
56.5 against 62.3 (`lcpp_bench` tg128 under `DASLLAMA_GPU_HOT_DMA=1` against unset, the prefill
rotation rows of `PERF_LEDGER.md`). The knob is on by default; `=0` keeps the host fill, the before-and-after on real prompts.

### A prompt's window serves the pool's hits and leaves it warm {#pool-window-prefix}

**A streamed window reorders its regions to a pool prefix - the layer's hits, then the busiest misses
each with a victim slot the stream's copy lands in for keeps - and streams the rest through the slot as
ever, in one chain window that carries both stack sets.** Before the prefix a prompt never read the
pool: every expert of a window streamed, and the decode behind a prompt filled its pool from cold.
`rdec_stream_pool_prefix` takes the prefix under `DASLLAMA_GPU_HOT_DMA` where the pool is on the layer,
the meet is on, and every region's expert is readable (`rare_window_ready` - the meet's rotation under
the host's claim would move a prefix otherwise): the hits are regions at their slots with nothing to
copy; a miss takes the coldest slot whose expert the window does not pick, free first, where the miss's
count reaches the slot expert's (`rdec_pool_victim` reads the window's bucket counts, not every pick
a slot); and the two lists merge by their rows, a hit lighter than the busiest streamed miss riding the
rest - the device takes the list's head, a bundle of light regions costs it what a busy one does, and
hits first starved it while the host claimed the busy misses from the rare end (a 64-row window read
245 device rows against the host's 267 so, 390 against 120 by rows). The tier's pool window hook names
the prefix's length and its layer (`moe_gpu_xf_pool_window`): the chain opens with the pool's stacks
for the prefix and the slot's for the rest (`XfMeet.p1/p3/p2`, `npool`), a bundle never straddles the
seam, a prefix region's copy lands in its slot (`xf_expert_copy_pool`) and a stream region's on the
slot, and the window's compute waits the layer's last DMA fill. After the join a copied region the
device took keeps its slot (`rdec_pool_promote`); the hits count as the pool's. The prompt leaves the
pool holding its hottest experts, so the decode behind it hits at the warm rate from its first token
(on Qwen3.6-35B the decode's pool hits read 90.7% against 79.0% with the knob off, its host experts
2.5 ms a token against 6.0 - `lcpp_bench`'s tg128 behind its pp3000, the prefill rotation rows of
`PERF_LEDGER.md`); a 64-row window on layer 12 serves 414 of 512 rows off 29 hits with
nothing copied. On a long prompt every expert of a layer is picked, so no slot the window reads is a
victim and the prefix promotes into free slots alone; the decode step's DMA fill moves the pool from there.

### Under the decode meet a window claims over the pool {#pool-meet-window}

**Under `DASLLAMA_GPU_DEC_MEET`, a routed layer's window host step - where the layer's stack streams
and its pool is on - runs the meet over the pool (`rdec_pool_meet_rows`).** The window's regions are
its hits in their slots first, then its misses, each with a victim slot that the device's DMA fills
as the meet claims the region, then the misses no slot could take, which are the host's alone. The
device's limit is the hits and half the misses; the host claims the other half from the rare end at
the same time. Under a device limit the host claims off the list's tail and never rotates a region
up from under the limit: the window's copy and victim lists are frozen by region position, and a
rotation would move the regions under them (`moe_prefill_meet`, the `dl < nreg` arm - a pinned
`DASLLAMA_MOE_HOST_FIRST` had the host claim a pool region first, and the device then copied the
wrong expert into the victim slot). After the join, a miss the device took keeps the slot its DMA landed in
(`rdec_pool_promote`) and the slot's old expert goes out. A window the chain declines falls to the
streamed window (`rdec_stream_rows`), then to the host's sums. The knob is off by default: a decode
token's picks through the meet as a one-row window lost to the DMA fill on every carrier measured.
