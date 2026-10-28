# dasLLAMA Architecture - the Vulkan tier's model residency

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries the residency plan
that sizes a whole model before a byte uploads, the checkpoints a device-home recurrent stream keeps on the device, the
token rows' gather off the table's own plane, the marks swap that lets one GPU slot serve many models, the one device every
thread's context shares, and the hydrate a
call passing to the CPU rails takes first. The room the plan may fill on a shared card - the OS's answer, the
headroom, the guards, the pageable allocation shapes - is `ARCHITECTURE_GPU_VULKAN_ROOM.md`. The driver's other parts are its siblings': the logits landing in
`ARCHITECTURE_GPU_VULKAN_LANDING.md`, the NextN draft head in `ARCHITECTURE_GPU_VULKAN_MTP.md`, the N-row token command
in `ARCHITECTURE_GPU_VULKAN_NROW.md`, the prefill chain in `ARCHITECTURE_GPU_VULKAN.md`, the byte stores and the
gather kernels in `ARCHITECTURE_GPU_SHARED_ROWS.md`, the cooperative-matrix GEMM tiles in `ARCHITECTURE_GPU_VULKAN_GEMM.md`
and `ARCHITECTURE_GPU_SHARED_GEMM.md`, the per-op tier's decode era in
`ARCHITECTURE_GPU_VULKAN_DECODE.md`; the GPU backend role table stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The residency plan sizes a whole model before a byte uploads {#resident-plan}

The resident driver is all-or-nothing, so the plan IS the decision, and it is computed from
`Model` metadata alone. It sizes four numbers against the tier's weight budget: the dense weight
planes, the KV mirror at `seq_cap` (a sliding layer's ring at its window, `ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring`), the driver's own device scratch, and the headroom the auto
arm leaves unfilled (zero when the user pins VRAM). The scratch scales with the prompt window (`rdec_scratch_bytes` over
`pf_window_rows()`: the window's panels a row, a hyper-connection model's four-stream panels the widest), and the window
is the plan's to pick where no knob or pin names one: the wide window first (`PF_ROWS_WIDE`, 4096 rows, one expert sweep
for a 3k prompt), the narrow one (`PF_ROWS_DEFAULT`, 2048) where the plan does not fit with the wide
(`resident_window_pick`; `ARCHITECTURE_GPU_VULKAN_HC.md#hc-stream-window` carries the rates). The weight planes are ONE list in placement order
(`resident_planes`: per plane a row length, row count, format, offset and the role the upload registers it as):
the plan sums its bytes and the upload reserves it and places it in order (`place_plane` panics on a role out of turn), so the two cannot drift; the router rows,
the F32-on-disk beta / alpha rows (f32 on the device; a quantized file's beta and alpha are planes of the list in their tags'
formats) and the optional f32 embedding are the terms beside it. KV is reserved BEFORE weights and never
grows: on a discrete card the two compete directly, and evicting weights to grow KV would mean
re-uploading gigabytes. The mirror's codec is f16 by default (`resident_mirror_dtype`): the
flash-attention arms read f16 rows natively, and an f16 row is half an f32 row; a block codec the caller
asks for (q8_0, tq4: `ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`) is about half or a quarter of that again, and `DASLLAMA_VK_KV32=1`
arms f32 mirrors as an A/B instrument. Only sessions of the armed codec are served. A decline carries a reason, and where the numbers allow one it carries
the remedy that works - a shorter context, because the weights are fixed and the KV is not. The
plan takes that remedy itself once: a mirror that does not fit is re-planned at seven eighths of
the room that is left after the weights (`resident_fit_ctx`: a block codec's shadow shortens with the mirror), as long as that context clears the arming floor - the
built-in 4096 positions, `DASLLAMA_GPU_MIN_CTX` where set, and the caller's own context pin
(`set_gpu_ctx_max`, `DASLLAMA_GPU_CTX_MAX`; `resident_pinned_seq_cap` reads it first and announces
it once per walk, not for a prediction of the plan) where that sits under either, since a caller that
pinned its context named the shape it serves (`resident_arm_floor`); a pin asked whole
(`set_gpu_ctx_strict`, `DASLLAMA_GPU_CTX_STRICT`, the server's `--ctx`) is the floor whatever its size.
Where the asked mirror leaves no room for every plane, a MoE moves routed layers' experts to the
host (`resident_plan_host_layers`, `ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`) and a dense model declines
naming the context it would fit at; a mirror under the asked context says why (`resident_ctx_shortened_say`). The eighth held back is for
fragmentation: a mirror sized to the last byte fails on a heap the previous model fragmented
(`resident_plan_pick`). The pin is what lets a
four-stream bench row home a 12B at 648 positions a region on a 16 GB card, where the binding
cap's 6238 a region asks for more K/V than the weights leave. **A layer's q/k/v, and its gate/up, read one activation image, so a
group of two activation forms declines the driver** (`resident_layer_decline`); no stocked gemma file mixes them - each group shares
one form layer by layer - and a file that mixes gate and up rides the per-op dense FFN rail, which feeds each plane its own image.

**The mirror is a run of slabs, each a buffer pair bound as one SSBO range a side, and the
device's single-binding range caps the context at the widest layer's row before any byte is
counted.** `resident_mirror_slabs` seats the attention layers in file order: a layer's `seq_cap`
rows go in the current slab while they fit under the slab's units a region
(`resident_slab_cap_units`: `maxStorageBufferRange` less the slack, over the regions and the
codec's unit), else a new slab opens, so a slab is a contiguous layer run and a layer is never
split. Every slab takes the widest slab's stride (`RDec.mir_units`), so a region's offset is one
number for every layer and the N-row command's `TokMeta.mirbase` serves every slab's dispatches;
a layer's seat (`RLayer.mir_slab`, `rd_slab`) is what every kernel set, sync, readback and
hydrate binds, and the per-slab arrays - `RDec.k_slabs` / `v_slabs`, the window chain's block-codec
store and shadow sets - are indexed by that seat everywhere but `dasllama_vulkan_common.das`'s
accessors (`REVIEW.das`'s `check_mirror_slab_access`). A shared-KV layer carries its donor's seat
and adds no rows; a recurrent layer sits in no slab; the NextN head seats last. The plan charges
the card what the slabs allocate - both sides of every slab at the widest slab's stride times the
regions (`resident_mirror_bytes`), so the slabs that seat fewer rows carry padding. The partition
always succeeds once one layer's rows fit a slab, which is what the plan checks: `seq_cap` is at
most the range over the widest owning layer's bytes a position (`resident_binding_ctx`,
`resident_widest_kv_row`; a block codec's f16 shadow caps it too, `resident_shadow_ctx`), and that is where a hybrid is
right - a recurrent layer's KV width is zero, so the widest row is an attention layer's; read
from layer 0 instead - recurrent on Qwen3.5 - the width is zero, the cap is skipped, the plan
sizes a mirror the device prepare refuses, and the whole driver declines to the per-op rails
with the dense FFN on the CPU. The prepare keeps its own guard for direct callers.

**Each slab is `regions` consecutive copies of its layers' layout, and a region is one
session's whole history.** A host asks for the count before the load (`set_gpu_resident_regions`, the facade's verb,
one by default; a server's stream count), the plan sizes every region's K/V at `seq_cap`, and
the binding cap divides by the count, since a slab's regions share its one binding. A region
is an offset in mirror units (`rd_mir_base`: the layer's base plus the selected region's stride), folded
on the host into the `layerbase` (`GkRopeKvArgs.mirbase` for the rope-and-store) push constant of every kernel that stores or reads a mirror row
and into the sync, readback and hydrate offsets, so no kernel knows regions exist. The push
constant is baked as the token command records, so the driver records one command per region
(and one unsplit twin) and `vk_rdec_select_region` names the region the next call resolves
against; the window chain encodes per call and reads the same accessor. The resident module owns
who sits where (`RdecRegion`: rows, the owning claim's generation, a last-bind tick): a session
binds its own region while its claim stands, else the least recently used one, which its next
claim takes. Two sessions in two regions step between each other, or in one batched step, with no
K/V crossing the bus - `tests/test_gpu_resident_regions.das` holds each bit for bit to the
session alone. A session past the region count takes another's region and brings its history up
from the host, as every session did on the single mirror. An owner whose region another session
took is served from the host or not at all: where its rows never came down, its next prefill or
decode panics rather than read another session's history. A batched step over a
per-layer-embedding model gives each row its own side input: the table row gathered on device
off the row's id where the driver holds the table and the projection (the N-row command's ids,
`RdecTokenNFn`'s `ids`), else that row of the pre-step `eval_batch` already ran
(`rdec_batch_ple_row`).

**A device-home session's region is its only copy.** `create_device_session` makes a session
with scratch and no host cache (`Session.device_kv`), so nothing is allocated per request and
nothing crosses the bus per token: the overrides bring no history up for it, read no row back
after a batched step and hydrate nothing down. The CPU rails cannot take its call, so a pass
that reaches one panics with the pass's reason (`rdec_device_home_guard`) - the prefill pin
is the one gate it walks past, because the pin guards host-cached sessions whose device-only
rows another session's claim would strand. Its region is pinned while it lives
(`RdecRegion.pinned`): no other session is handed it, and a host-cached outsider that finds
every region pinned passes as `busy`. A finished session parks (`gpu_device_kv_park`): the
pin drops, and the claim it answers keeps naming the rows until another session takes the
region; `gpu_device_kv_adopt` hands the first `npos` of them to a fresh session under a claim
of its own, the next prefill rewriting whatever lay past. `gpu_device_sessions` answers how
many a host may hold - the region count - and `gpu_device_prefill_continues` whether a
session's prefill may start past zero: on every model the driver serves (a hyper-connection model's n-gram conv ring goes up from the host first where the host holds it, `rdec_prefill_hc_run`). A recurrent session's state parks
beside its rows: the region's state slots keep the state at the parked position
(`RdecRegion.dn_pos`), and an adopter takes exactly that many rows, binding the slots as they
stand with nothing crossing the bus - or, past a shorter shared opening, the region's checkpoint row at it.

**The scheduler's device mode is that contract over streams.** `set_device_kv` switches an
idle `Scheduler` (one batched step takes one kind of session): admitted streams are
device-home, their prefill quantum is at least the device's window (`gpu_device_prefill_window`),
a media request is refused at `submit` (a device-home session serves no media span),
and a reaped stream parks its claim under the token list its rows hold - one parked claim a
stream slot - so the next request adopts the longest opening it shares, short of its last
token: a conversation's next turn prefills its new suffix alone, with no copy. A recurrent
model's parked claim is adopted at every token it holds or at the longest checkpoint the request
shares (`#device-home-checkpoints`), since its state stands at those positions alone. `dasllama-server` turns the mode on
per slot before each step (`sync_slot_device_kv`) while the slot is served whole from the device,
has a region per stream and no media tower or self-speculation, and sizes the slot's context
to a region's. `tests/_resident_regions.das` holds the sessions, the pin, the park and the
mode against host-cached twins on the same device, on a qwen2 and on the E-series carrier.

**What the plan may fill on a card it shares with a desktop is `ARCHITECTURE_GPU_VULKAN_ROOM.md#vk-plan-room`:**
the room the OS reports and the headroom where none does, the all-zero-logits and demotion guards that
stand whatever the plan decided, and the two allocation shapes the Windows memory manager pages out from
under a running model - the priority tag and the mapped ReBAR type, both opt-ins - and the per-call buffers
the mapped type never serves (`ARCHITECTURE_GPU_VULKAN_ROOM.md#vk-per-call-buffers`).

An OPTIONAL plane rides only the room left under the budget at THIS context - what remains of
`budget_bytes - headroom_bytes` after weights, KV and scratch; the reserved headroom itself
stays unfilled. It never shrinks any of the three, and it reports zero bytes when it does not
fit - so the same model plans the plane in at a short context and out at a long one. Two planes are
optional: the token table's gather plane (`#vk-token-gather`: an untied quantized table's own plane,
`ResidentPlan.emb_bytes`, or a float table's exact f32 rows under `RDEC_EMB_F32_CAP`,
`ResidentPlan.emb_f32_bytes`), and after it the gemma-4 E-series' per-layer-embedding token table (`ResidentPlan.ple_tbl_bytes`, a
plane [vocab x layers*ple] in the table's own tag - q8, superblock or per-32 - gathered in disk order, no repack touching it). Both place first, into the empty arena, in pieces of 65536 rows the host
gather's 2 GB array admits (`resident_place_table`), which must land back to back in one slab because
a gather reads a table off one block base; the side-row gather is the tag's `EMB` leaf at the row width
layers*ple, scaled by sqrt(ple). With the side table placed the ids prefill serves a
per-layer-embedding model too - the embed and the side rows gathered on device, no host row upload -
and the token command reads the row the host's pre-step embedded, in place (`EMB_ID_HOST`).

### A device-home recurrent stream keeps its state at its checkpoints on the device {#device-home-checkpoints}

**A recurrent state exists at one position, so a region keeps copies of it at the positions a later request can share.** A
region carries `RDEC_DN_SNAPS` (4) checkpoint rows beside its state slots, each in the rollback row's shape - every recurrent
layer's slot state (`nvh x ds x ds` floats) and its ring pair (`2 x cd x (dconv - 1)` floats) - with the region's ring parity
(`RDec.snap_dev`, made at the first save or the first capture). The plan charges them (`rdec_dn_snap_bytes`): on the 35B (30 recurrent layers, 32
value heads of 128, a 8192-channel conv of 3 taps) a row is 2.19 MiB a layer, 65.6 MiB in all, 262 MiB a region. The scheduler's
device mode plans a recurrent stream's stops at the request's openings and its last message's start and end
(`device_checkpoints_plan`, the markers the host prefix cache keeps checkpoints at): a stop inside a window the window chain
captures into a row as it passes it (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block`), a stop at a window's end the
driver saves off the slots (`gpu_device_kv_checkpoint`: an empty row, else the least recently touched); the finished turn's
state is the parked claim's own (`RdecRegion.dn_pos`). An adopter of a parked claim takes the longest of those positions its prompt contains
(`parked_state_at`): the end state stands in the slots, a checkpoint row loads into them (`rdec_dn_snap_load`), and the rows
the claim held past it are rewritten by the adopter's prefill. The checkpoints at or under the adopted position stay the new
claim's - the same tokens put them there - so a third request on the same opening adopts them off the second's claim. Nothing
crosses the bus: a save and a load are device copies. On a hyper-connection model a row carries the region's n-gram conv ring past
the state (`RDec.snap_ring_off`), saved, loaded and peeked with it (`vk_rdec_dn_snap_ring` into `DnSnapshot.ple_hist`), and the pre-step's id window (`Session.ple_prev`), which no row holds, the scheduler seeds from the tokens (`ple_window_seed`) at adoption and when it files a snapshot.

### The token rows gather on the device off the table's own plane {#vk-token-gather}

**Where the table sits on the device in a format a gather reads, neither the window nor a token
command brings a row across the bus: the window gathers its rows off the token ids, the one-row step
off its one token id, the N-row command, the verify and the draft off their rows' ids.** Which gather
a table takes is its disk type's (`ARCHITECTURE_GPU_SHARED_ROWS.md#token-table-gather`).
`resident_arm_emb` hands the plane to the tier (`rdec_set_emb` for a quantized plane in the arena,
`rdec_upload_emb_f32` for the f32 rows) and logs why the host embeds where it does: a prepared image (it does not carry `emb_f32_exact`), a plane past the room or the cap, a trimmed
image, a per-layer-embedding model whose side table stays on the host, a hyper-connection model (its
window reads the pre-step's rows). The window binds its ids plane (`pf_ids`); every token command but the in-order rows
command (the recurrent peel) records the gather in place of the x upload, each row's id read from `tok_ids_host` and, for a row
with none (`EMB_ID_HOST`: a non-text row, the split command's caller), the host's row staged in `tok_xh_host` (`nb` of each).
A step that brings no id writes `EMB_ID_HOST` into the id word itself, the split command's entry (`vk_rdec_token_split`)
included: the word otherwise still holds the pick the chained step's prologue copied in (`rd_chain_prologue`), and the gather
would embed that last landed pick in place of the fed row. The engine leaves a decode step's row unembedded under the decode
gate (`register_embed_dev_gates`), a batched step's rows under the batch gate (`register_embed_batch_gate`: no routed layer's
experts on the host), and embeds them late where the override declines; the split command embeds its own row.
`vk_rdec_nrow_embed_gathered` counts the N-row and verify rows, `vk_rdec_head_embed_gathered` the NextN head's;
`set_gpu_resident_embed` pins `DASLLAMA_VK_GPU_EMBED` for a load.

**A MoE is planned like a dense model with bigger FFN planes.** Its weight planes are the
attention quads, every MoE layer's expert triple (`[ne x nfe x dim]` twice and `[ne x dim x
nfe]`, sliced per expert as the per-op walk gathers them), its shared expert's triple where it
has one - in the file's K-quant format where the loader kept those planes beside the q8
transcode the CPU chain reads (`wshk*_offs`, `shexp_plane`; it keeps them on a load a device reads
them on - an armed GPU tier or a Metal mode - so a CPU-only load carries the transcode alone), the
transcode otherwise - the classifier, and the
router plane - every MoE layer's f32 rows with a gated shared expert's gate row beside them. The scratch adds the window's routed planes: the gathered f16
rows, the gate and up rows, the f16 hidden rows and the routed down rows over `pf_window_rows() x k`
bucket rows plus `TILE_READ_SLACK` rows of tile slack (128 - the m column's unclamped partial
load, `dasllama/dasllama_gpu_tier.das`), and the routing smalls. The dense planes size at the shared
expert's width, or the expert width where no layer has one. The plan is all-or-nothing as ever:
a MoE whose stacks do not fit takes the per-op rails, which stream what the card cannot hold.
The plan is sized BEFORE the per-op reserves (the streamed slot, the decode mirrors), and a
fitting plan forgoes them - they would only shrink its room - so a decline past the plan (a
placement, a class rail) leaves the per-op rails without a streamed slot, said out loud. The
tile family the routed block rides is the f16-fed cm2 tiles (`ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block`), and `DASLLAMA_GPU_RESIDENT=0` keeps the per-op rails for any model, the A/B lever.
The driver is attempted only when asked for (`gpu_resident_requested`): the measured-best set
asks (`DASLLAMA_GPU=1`, or `auto_tier` on the want), and so does a want that spells its rails
out one by one and sets `resident` - the server's serving shape, where `gpu_dn = false` must
still turn one rail off. Rails alone, by env or by want, keep the per-op tier.

### One GPU slot, many models: the marks swap {#gpu-slot-marks}

A multi-model host runs one device tier under several loaded models, and the tier's per-model
state is offset-keyed - two models' marks installed together route one model's dispatches at
the other's planes. `GpuModelMarks` is that state WHOLE: the loader-contract marks plus every
resident-driver per-model global (the activation, the mirror count, the mirror cap, the mirror
codec, the device-embed arm, the NextN head's arm, the per-layer-embedding projection arm, and the armed model's planes key). The save moves the installed state out and leaves the globals
reading as no-model; the restore is its exact inverse. The whole-model drop clears the same set
and deselects the `"vulkan"` overrides, so a dropped model's prefill and decode take the plain
CPU path and a later re-arm passes `resident_upload`'s no-active-override gate. The three carry
the same set, which is why a model's device state never survives into the next. The upload
rail enforces it from its own side: a load that finds marks still installed - a model deleted
without the drop, the shape every test process and single-model tool takes - drops that
model's device state before uploading its own, carrying the load's MoE layer request across
the drop (the one mark the drop's reset would otherwise zero before the rail reads it).
Without that drop the second model's stacks
land beside the first's, and the offset-keyed stack lookup serves whichever model's plane
registered that offset first: the decode attention block asserts on the geometry change, and a
model whose geometry matches decodes the earlier model's weights.

### One device a process, shared by every context {#vk-shared-device}

The tier's per-context state - the command pools, the buffers and their memory, the descriptor pools, the class pipelines,
the fence and the timeline semaphores, the resident driver and its K/V - is daslang module globals, so every context keeps
its own, a worker thread's included. The device is not: the first context to arm the tier makes the instance and the
`VkDevice` under the dasVulkan holder's lock (`vk_shared_device_acquire`, `vk_shared_device_publish` - dasVulkan's
shared-device document carries the holder) and every later one borrows it (`vk_device_borrow`), the
handles never deleted by a context. The loader's device table is loaded once, by the context that made the device, so the
process-wide function table is that device's by construction. The device asks `VK_DEVICE_QUEUES` queues of each family it
uses; a context takes the next one round robin (`vk_shared_queue_take`), contexts past the count share a queue, and every
submit and queue wait goes through the holder's per-queue lock (`vk_queue_submit_locked`, `vk_queue_wait_idle_locked`),
uncontended where the queue is a context's own; `REVIEW.das`'s `check_locked_queue_calls` holds every engine, harness and
test source to it, banning `run_cmd_sync`, `vkQueueSubmit`, `vkQueueWaitIdle` and `vkDeviceWaitIdle`, the calls that skip the
lock. A context's shutdown (`vk_context_release`, a `[finalize]` that runs in the
context that made the state alone) drains its queues, destroys everything it made, takes its bytes off the process count and
releases its reference; the last context out destroys the device and the instance (`vk_shared_device_release`). The
availability and caps probes read the shared physical device where one exists, so a worker makes no probe instance.

The card's room is one budget: every device-local buffer is counted against its context and the process
(`vk_shared_device_bytes`), and the resident plan's cap gives up what every other context holds (`vk_others_dev_bytes` in
`carved_budget`), so a model loaded after another plans against what the first left. The server's embedding worker is such a
context beside the chat slot's: it arms under the slot's want (`add_embedding_model`'s `gpu_want`; zero lets the env decide),
asks one K/V region of `emb_scratch_tokens` - the scratch region every request's texts share
(`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-scratch`) - and clamps its context to it; the engine context's one-owner
bookkeeping (`g_gpu_owner`, the marks swap above) is its own slots' alone.

Within a context the overrides are global too, so they serve only the model they armed: the upload records the model's
planes (`model_planes_key` - the mapped image's base, else the first plane array's), and `moe_gpu_resident_serves` tells the
armed model from another loaded beside it; `embed` makes its sessions in the mirror codec only for the model the driver
serves (`embed_session_dtype`), f32 - the CPU rails - for any other. A mapped planes-trimmed lane the driver did not arm for
is refused at its load by name (`trimmed_lane_unarmed_panic`, from `moe_gpu_upload_lane`), never at its first CPU forward;
the server refuses at its start a slot whose trimmed lane cannot give every stream a region (`trimmed_slot_refusal`), since
the scheduler keeps host-cached streams' prompts off the driver.

### A call that passes to the CPU rails hydrates first {#resident-pass-hydrate}

Every pass of a host-cached session's call to the CPU rails - whatever the reason - brings the
region's device-only rows down to the host cache first (`rdec_pass_hydrated`), so the CPU
computes over the rows the mirror holds: an image turn's media rows attend the head the device
prefilled, and a decode past the region's rows reads them beside the CPU-served ones. A
device-home session has no host cache to fill, so its pass panics with the reason. The pass
still answers false, and the CPU rails serve the call.

A region another host-cached session takes may hold rows its owner's cache lacks (a window prefill, a one-row step): the taker saves them first (`rdec_bind_region` reads the region's `host_behind` and head counts, `rdec_stash_region` the rows), and the
owner, back for a bind or a pass, writes them into its cache before anything reads it (`rdec_unstash`); the owner's own bind drops a save its steps outdated, and a released session's save goes with it.

Every row that moves between a host cache and the mirror - up for a take, down for a hydrate, a
batched row's write-back, the NextN head's rows, a saved claim - moves through `rdec_host_runs`: a flat cache as
one span, a paged cache a physical run at a time (`kv_runs`), the mirror addressed by position
either way, so a paged session is served like a flat one. A scheduler hydrates a paged stream
before the prefix cache reads its pages - its donation, a recurrent checkpoint - since the rows a
device stepped live on the mirror alone until a hydrate brings them down.
