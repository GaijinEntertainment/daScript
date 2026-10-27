# dasLLAMA Architecture - the sliding layers' K/V ring

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries how both GPU homes hold a
sliding-window layer's K/V mirror rows as a ring, and the floor a ringed history continues from. The mirrors themselves are
`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan` (Vulkan) and `ARCHITECTURE_GPU_PREFILL_WINDOW.md#kv-mirror-arena` (Metal).

### A sliding layer's mirror rows are a ring of its window and one command's rows {#kv-sliding-ring}

**A query on a sliding layer reads the `window` positions up to its own, so the mirror keeps no older row of that layer.** A
sliding layer that owns rows holds `kv_ring_rows` of them: its window plus the most rows one command writes before its
attention reads (the slack), rounded up to the attention tiles' 64-row step; position `p`'s row sits at slot `kv_ring_row(p,
ring)` = `p % ring`. A global layer, the NextN head, a ring that would reach the context, and every layer under
`DASLLAMA_GPU_KV_RING=0` (`set_gpu_kv_ring` in process) keep a row a position, `ring` 0. A shared-KV layer reads its donor's ring. A home whose row movers copy a layer's rows end to end declares
`install_moe_gpu_resident_kv_ring(false)` (the CUDA home), and the plan then seats a row a position on every sliding layer
whatever the knob asks, announcing the flat slabs.
The slack is what makes the ring sound: a command writes its rows before it attends, and the earliest query of a command of `n`
rows reads back to `window - 1` rows under its own position, so the ring must hold `window + n - 1` positions at once; the Vulkan
slack is the window chain's 512 rows, past the N-row command's 16 and every verify.

**`kv_ring_row` is the one address of a mirror row.** Every kernel that stores or reads a mirror row - the rope-and-store
stamps (`GkRopeKvStore`, `GkQknRopeKv` through their `ring` field), the window's rope store, the block codecs' stores and f16
shadow pass, the decode attention, the flash tiles and the chunked tiles - maps a position through it from the layer's `ring`
push field, and every host mover reaches the rows through the Vulkan accessors `rd_mir_row` (a position's row in a region) and
`rd_ring_runs` (a span cut to the runs that sit back to back in the slab: its last `ring` positions, split where the slots
wrap); the drafter's borrow of the target's mirror reads through the same accessor. A flash tile's step loads 64 (or 32, or a
16-row fragment) keys that never straddle the wrap, since both the step start and the ring are multiples of the step; the keys a
tile loads under a query's window or past the last position land on slots holding other positions' finite rows, which the mask
drops. The block codecs' f16 shadow stays a row a position: the shadow pass reads each attended position's ring slot and the
tiles read the shadow flat.

**The plan charges the ring, not the context.** `resident_mirror_slabs` seats a ringed layer's `ring` rows in its slab;
`resident_ring_bytes` is the part of the mirror the context does not scale, which `resident_fit_ctx` holds fixed when a plan
shortens its context, and `resident_binding_ctx` bounds the context by the widest flat layer's row alone. On gemma-4-26B-A4B
(25 of 30 layers sliding at window 1024, two kv heads of 512 on the global layers) the strict 32768 x 4-stream f16 plan's mirror
falls from the whole context on every layer to the five global layers' and 1536 rows of each sliding one.

**On Metal a stream's mirror slice lays its layers in layer order, a flat layer taking the slice's row capacity of rows and
a ringed one its ring's rows** (`MirrorLayout`: a layer's base is the capacity times the flat rows ahead of it plus the ring
rows ahead of it),
and `mirror_lbase` / `mirror_row_at` / `mirror_ring` are the accessors every store, attention form, writeback and the gemma
drafter's borrow read; the attention stamps take a ring's base past the per-layer stride as `layr` and map a position through
`kv_ring_row`. Metal's slack starts at 512 rows and doubles when a prefill window asks for more (`mirror_ring_ask`): the
layout key changes, the device is quiesced and every slice relayouts at its next prepare. The host cache stays the truth on
Metal - a ring layer uploads only its last `ring` positions, and a rewind or an adopted slice re-uploads them - so a Metal
history has no floor. The window feeds a ring in pieces cut at the wrap (`mirror_ring_pieces`), and the mirror-fed prefill form
reads a ringed layer's keys there through `AttnArgs.ring`: the device pair's QK tile reads its 64 keys from a 64-row step, one
ring piece, and the AV pass splits its key run at the wrap, while the score slab stays a column a position. The slack is at least
the window's rows, so the ring holds every key a window's queries read once the window's own rows are stored.

### A ringed history continues from its floor {#kv-ring-floor}

**Rows a command wrote past a ring's slack overwrote the window under every earlier position.** A claim's high-water mark
(`RdecRegion.hw`, the most positions its rows ever reached) puts its floor at `hw - slack` (`rdec_ring_floor`): a continuation
from that position or later finds its window whole, one from earlier does not, and no copy of the overwritten rows exists - a
host-cached session's cache took only what the ring held when it hydrated. So the floor travels with the history:
`Session.kv_ring_floor` is set when a prompt claims its region from the start (`rdec_claim`, a one-window prompt past the slack
included) and rises as the driver writes (`rdec_rows_hold`), a call at a position under it panics
(`kv_ring_floor_check`; a call at position zero - a prefill from it or a decode of row zero - rebuilds the history, drops
the claim's high-water mark to the rows it writes and clears the floor), a device-home adopt or a pool export under
it refuses (`gpu_device_kv_adopt_`, `gpu_device_kv_export_`), a pool import under the carrier's refuses, and a prefix-cache page
records its donor's floor (`PrefixEntry.ring_floor`), so an attach is cut to the longest run of pages whose floors it reaches
(`prefix_plan_floor_cut`) and the session it fills carries their floor. A conversation's next turn continues from the turn
before it and keeps its rows; an opening another conversation shares, or a rewind past the slack, prefills again - the trade
llama.cpp makes for its own sliding-window cache.
