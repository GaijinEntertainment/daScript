# dasLLAMA Architecture - the Vulkan prefill window's per-session parts

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries what a window does a
session at a time: the recurrent block, whose state and ring belong to one region, the segmented window that holds several
sessions' chunks, the short window's peel through the token commands, the final residual rows a window lands for an
embedding, and the scratch window that seats an embedding request's texts back to back in one region. The chain these sit in is
`ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain`.

### The recurrent block of the prefill window {#vk-prefill-dn-block}

**A recurrent (deltanet) layer's window block replaces the attention head; the FFN tail is
shared.** Per window the block runs the qkv and z GEMMs into the window planes, the beta and
alpha rows into the layer's smalls, the conv over the selected region's ring image, the sequential
scan over that region's state slot (the slot's bases ride the pushes, the sets bind every region's
slot), and the out GEMM into `pf_xb2`. The smalls are the layer's per-layer f32 plane: the layer's
cold constants (conv taps, out-norm weights, the `a` and `dt` rows), the beta and alpha rows, and a
parity ring pair per region (`ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`). A call enters on the image the region's
parity word names (`pf_dn_begin`; a prompt from position zero resets the word and zero-fills the slot's state and that image,
`pf_dn_zero_region`): its first window reads that image per-channel (`DnConvArgs.hist_mode` 1), the later windows and the tail
use the other, which the word names after the call (`pf_dn_end`) - the parity the owner handoff hands the session. A window is
one deltanet chunk: `pf_window_len` cuts a recurrent model's window at `DN_WINDOW` rows, since the smalls' beta and g rows and the
scan workspace hold that many and a longer window copied its rows past them into the next plane (`dn_window_parts_enc` asserts
every part inside it); a prompt window asked past it runs as windows of that length. The weight planes stay in their file
formats where the loader tags them natively (`ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`
carries the tagging condition), so a Q5_K/Q6_K file rides the k5/k6 tiles.

The block reads two activation feeds, and each one is decided on its own.

- **The x feed is the layer's input rows.** The qkv and z GEMMs read it. `pf_dnx6` decides it:
  f16 rows when both of those planes admit the coopmat tiles (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`), else the quant image.
- **The o feed is the scan's output rows.** The out GEMM reads it. `pf_dno6` decides it: f16
  rows when the out plane admits the coopmat tiles, else a requant.

A K-quant file whose out plane is Q8_0 therefore keeps its qkv and z GEMMs on the coopmat
tiles and takes the requant on the o feed alone.

When the x feed is not f16, a K-quant qkv/z pair reads the Q8_K activation form, as the
attention head's kq planes do, and a beta or alpha plane of the other activation class re-requantizes
the rows into its own form behind the z GEMM - one feed, two forms, as the decode step does (`ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`).

The beta and alpha rows take one of two arms. A file that carries them as f32 rows keeps the f32 rows on the
device and reads them through a scalar tile GEMM over the `[beta ; alpha]` rows (`DnBaGemm`): one workgroup covers 16 positions by 16 output rows, one output
per invocation, and the grid runs over both group axes, so a layer of only `2 x nvh` rows (`nvh`,
the layer's value-head count) - 64 on the 9B - still fills the card. The plane arm is two GEMMs, each in its tag's format, and
copies: a `[dim x nvh]` plane is one 32-row tile of the quant-fed batch class (`BATCH_TILE`, the y store guarded past `nvh`), never the
cm2 family's 128-row tiles, so the plane reads the quant feed whatever the x feed is.

The conv is channel-major: a workgroup owns 256 channels over `DN_CONV_PB` positions
(`dn_conv_wgs` sizes the grid), every thread slides one channel's window over them with the taps
in registers and one new row read per position, and holds its outputs in registers for the SiLU
and the per-head L2 norm - the head's sum of squares crosses the head's lanes by shuffles and its
32-lane blocks by one shared row, a head being `ds` consecutive channels with `ds` dividing 256. The
position-major form it replaced (one workgroup per position, the row staged in 32 KB of shared memory)
re-read every input row once per tap and the whole tap table once per position, all from L2.

The scan is the plain per-token delta rule: one column of a head's state per
lane cluster, 16 state rows per lane in registers, four 32-lane blocks per workgroup, and every lane
reads its own k and q elements from the conv plane a token AHEAD, into registers, while the
current token computes (a row's loads never wait on the recurrence, whose per-token chain was
latency-bound on them: the 9B's scan 342 -> 322 us a layer, the 0.8B's window a twentieth
shorter) - no shared staging, no barrier between tokens. `dn_scan_wgs` sizes the grid, a workgroup covering `4 x 32 / (ds / 16)` columns
of one head (a workgroup of one 32-lane block reads the 35B 6% slower: a thousand such
workgroups each hold the gate arrays in shared memory). The head's per-token gates - the decay `exp(a * softplus(g + dt))` and `sigmoid(beta)` - are
computed once per workgroup into shared memory before the token loop, so the recurrence reads
two shared words a token where it paid five transcendentals and a divide per lane (a third of
the scan's time on the Linux RTX 5080).
The tokens loop inside the kernel; each token costs two cluster reductions inside the dependency
chain (a sixth of the scan's time there). The conv, the scan and the requant family's K-quant
fold map their 32-lane blocks and clusters from the invocation id, never from the subgroup id:
every shuffle in them carries an xor mask under 32, which stays inside such a block on any
subgroup width the tier admits (32 or more), so the kernels hold on a 64-lane device as on a
32-lane one; only the KHR tile keys on the width itself (`ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile`). The conv and smalls bindings are NonWritable, as every
binding no kernel of a class writes is (`ARCHITECTURE_GPU_VULKAN.md#vk-readonly-lens`). The decoration is load-bearing: without it the driver orders each token's k and q
loads behind the previous token's o store (the two buffers may alias), and the same kernel ran
2.35x slower (19004 against 8084 us over the 0.8B's 18 layers on the Linux RTX 5080; the staged
two-column form it replaced 8854). The raw o rows land in the per-op tier's workspace, for the
gated out-norm's one workgroup per position.

A checkpoint stop inside a window is a capture. The tier plans one checkpoint row a stop in the segment's region
(`rdec_caps_plan`: an empty row, else the least recently touched, marked pending) and hands the call its captures
(`rdec_prefill_caps`, `RdecPfCap`: the segment, the row the state stands after, the row). Each window uploads the records of its
parts (`pf_window_caps` into `pf_cap_dev`: two words a capture, the row inside the part and the checkpoint row's float base), and
the scan's pushes name a part's records (`DnScanArgs.cap_n`, `cap_rec`, `cap_state_off`): the walk writes its register state after
that row into the checkpoint row (`snap_dev`, the layer's `snap_off`) and walks on over the live slot untouched; `dn_cap_tail_cls`
writes the taps projected rows before the row - the entry image under the taps - into the row's ring image 0, whose parity the
call marks 0 (`pf_caps_done`). The hyper-connection chain copies each capture's n-gram ring rows into the row ahead of the ring's
own update (`pf_hc_side`). After the call the tier lands them (`rdec_caps_land`, each segment its own records of the call's
list, in order): a device-home session keeps each row at its
stop's position, as `gpu_device_kv_checkpoint` would have saved it; a host-cached session peeks the row into its snapshot
(`rdec_peek_snapshot`) and frees it. The token-command peel keeps no state at an interior row, so a call with a capture takes
the window chain. The checkpoint rows exist from the first save or the first capture (`rd_snap_alloc`); until then each
recurrent layer's scan and cap-tail sets bind the smalls in their place, unread, since a window with no capture never reads
that binding, and `vk_rdec_prefill_caps` rebinds the layers' sets onto the rows when it makes them.

The conv history crosses windows position-major in the image the call did not enter on; the last window transposes the
tail into the decode step's per-channel layout (`dn_tail_cls`; the handoff is stated in
`ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`). Every window past the first carries at least as
many rows as the conv has taps: when a full window would leave the last window fewer rows than
that, the earlier window takes fewer rows instead, so the last one still holds the taps. Only a
lone first window can be shorter - the tail reads the ring's leading rows off the entry image (`DnTailArgs.old_rows`), zero
for a prompt from position zero, the session's history for a continuation; the two images never alias.

### The segmented window {#vk-prefill-segments}

The window chain holds several sessions' chunks at once: the window's rows are the segments' back to back, a
segment one session's `npos` rows from its own position into its own mirror region (`RdecPfSeg` on the tier's
SPI: `row0`, `npos`, `pos0`, `region`; the engine's `eval_segments` reaches it through the `vulkan` segments
override, `vulkan_resident_prefill_segments`, which `rdec_prefill_segs` hands to `vk_rdec_prefill_segs`). The
row-independent stages - the prologue norm, every GEMM, the feeds, the activation, the residual steps - run
once over the window as the one-session chain runs them. What a session owns runs a segment at a time inside
the layer: the rope-and-store writes the segment's K/V rows at its positions of its region
(`RopeKvBArgs.pos0`, `layerbase`, the window row base `row0`), and the attention binds the segment's query
rows and its output rows at `row0` of the window's planes (`FaArgs.row0`, `DaAttnBArgs.row0`: the flash tiles'
tensor layouts end at the segment's last row, so a padded load past it reads zero rows; the chunked pair reads
its q tile from `row0` and keeps its causal arithmetic segment-local) and its keys at its region's base
(`kvbase`), so a row never sees another session's keys. A recurrent layer's conv, scan and ring tail run a segment at a time
too, each over its own region's state slot and ring (`DnConvArgs.row0`, `DnScanArgs.row0`; the window's beta and alpha rows
and the gated out-norm stay window-wide), the region's ring image advancing a window part at a time
(`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block`). Under a block-codec mirror the window holds one
segment: the codec stages the window's rows window-local (the rope-and-store's `pos0` is the segment's window
row), so its store and shadow run once over the whole window. A window past the prompt window's rows (`pf_window_rows`) loops as the
one-session chain does, the parts of the segments inside each window the window's own (`pf_window_segs`).
Every segment whose last row a window holds takes the final requant and the classifier at that row, its logits
landing at its slot of the host buffer (`pf_logits_host`, `regions` slots of `vocab`); a segmented window runs
the last layer's FFN over every row, since every segment's last row wants it. The one-session call is one
segment at row 0, so both forms are one code path.

The segmented form serves where every segment would take the one-session form - the codec armed, no span,
under the cap, every session's embed form alike (the ids the driver gathers, or the host's rows) - and each
session binds a region of its own, the owners first; the admission decides before any bind, so a declined call
binds, steals and counts nothing (the one-session fallback counts the pass it takes). A recurrent segment's state
goes into its region's slots as its region binds (`rdec_dn_prefill_entry`); a routed block routes every row of the window,
whatever segment it holds; a NextN carry lands a row a segment (its slot's row of the carry plane), and a segmented window
warms no head row - each session's head rows past its start are cut, a draft acceptance's loss alone. It declines a
per-layer-embedding model, a hyper-connection chain, routed experts on the host and a block-codec mirror: their side rows,
wide residual, host sums and staged rows are per call on this driver. A declined
call falls to the engine, which prefills the segments one at a time through the one-session override. The CUDA
home takes the same form (`cud_rdec_prefill_segs`): the window's segment parts and the ending-segment lookup
are the tier's (`rdec_window_segs`, `rdec_seg_ending_at` beside `RdecPfSeg`), its per-segment rope-and-store
and attention ride the shared kernels' `row0`, each ending segment's logits land at its slot of the out
block, and its witness is `cud_rdec_prefill_segment_windows`. The
scheduler's prefill tick hands a device-home scheduler its segments like a host-cached one, but for a model
whose device prefill cannot continue past zero, which takes each stream's whole head alone. The witness is
`vk_rdec_prefill_segment_windows`; the driver counts a prefill a segment.

### The short window's peel {#vk-prefill-peel}

A prefill of at most the rows the N-row token command steps at once (`vk_rdec_token_n_rows`; one row is the
one-row command's) skips the window chain: `rdec_prefill_peel` embeds the rows on the host and runs them
through the token command with every row in the session's region at its own position, row i's key count its
position plus one, so a row never reads a row after it; the last row lands the session's logits, the rows
before it land picks nobody reads. The chain costs a short continuation about two decode steps whatever its
row count (the race bed's rows are the `PERF_LEDGER.md` entry's); the token command reads the weights once.
A recurrent model's rows peel through the in-order rows command (`vk_rdec_token_seq`): the N-row command recorded with
every recurrent layer stepping the rows one after another in the region's slot, each row's ring image the one the row
before it wrote (`TokMeta.parity` alternating), the region's parity advanced a row at a time after. A NextN-headed
model's peel warms no head row, so it peels only with speculation off - the head's rows past the window's start cut
(`rdec_head_rows_cut`); under speculation the head warm the chain runs is wanted, and the window takes the chain. The peel fits a
chain with no side rows, wide residual or host experts (`rdec_peel_fits`, logging the property that keeps a model off it
once per armed model); the rest take the
chain, as does every window under the lever `set_vulkan_prefill_peel(false)`, which
`harness/vk_short_window_race.das` races the two forms through. The rows stay on the device as the chain
leaves them; `rdec_prefill_landed` is either form's bookkeeping; the witness is `moe_gpu_resident_peels`, a
peel counting as a served prefill too.

### The window lands its hidden rows for an embedding {#vk-prefill-hidden-rows}

An embedding pools the post-final-norm rows of every prompt position (the mean, the last row, the first row), so its forward
asks the session to keep every row's hidden state (`Session.keep_hidden`), and the window lands them: under the ask
(`vk_rdec_hidden_ask`, through the tier's seat `install_rdec_hidden`; the resident overrides set it from the session and lift
it after the call) every row runs the last layer's FFN - the last-row tail slice (`pf_ffn_slice_row0`) is off - and the
window's final residual plane (`pf_x`) is copied into a host plane in place of the classifier - an embedding reads no
logits, so its window runs none and lands none, and no segment needs a logits slot - then read into the call's `[npos x dim]`
rows; `vk_rdec_hidden_rows` hands a session its rows - the one-session call's from row 0,
a segment's from its `row0` - into its `x_b`, where the embed pool applies the final norm on the host. A window of an
embedding never peels (the token commands land the last row alone), and the hyper-connection chain lands no rows, so a
session keeping its hidden state passes there by name (`RdecPass.hidden`). A one-token text takes a one-row window too
(`embed_forward`): the one-row command lands the logits and no hidden row. The witness is the device cell of
`tests/test_embeddings.das`: the model card's cosines, batch against single, every window prefilled on the device and no
call passed to the CPU rails. The CUDA home installs the same seat (`cud_rdec_hidden_ask` / `cud_rdec_hidden_rows`): its
window runs the last layer's residual step alone under the ask and reads `pf_x` off the device after each window's sync; its
window has no last-row FFN tail slice to turn off. The device cell runs on whichever home the tier armed.

### The scratch window: an embedding request's texts back to back in one region {#vk-prefill-scratch}

An embedding keeps no K/V past its call, so its texts need no region each: a segmented window whose every segment is an
embedding's from position 0 (its session keeps its hidden rows, no checkpoint stop) on a model whose sessions keep nothing
past their rows (`session_keeps_rows_only`: no recurrent state, NextN head row or per-layer-embedding n-gram ring - the state
the embed pool's reuse reads too) seats all its segments in ONE
region (`rdec_segs_scratch_why`, `rdec_segs_scratch_bind`: the least recently used unpinned region, its owner's rows saved
first). A segment's rows start past the rows the segments before it hold (`RdecPfSeg.koff`): its mirror base is its region's
plus `koff` rows (`pf_seg_mir_base`), folded into the rope-and-store's and the attention's `layerbase`, so its keys sit at
positions 0.. of its own run and no segment reads another's; a window part carries its segment's `koff`. The driver seats
segments this way where no layer is a sliding ring and no block codec stores (`vk_rdec_segs_packed`, the seat
`install_rdec_segs_packed`; the CUDA home's `cud_rdec_segs_packed` and `crd_seg_mir_base` are the same rule and the same
offset). The texts' rows must fit the region's context; `embed_batch` cuts a request into windows of at
most that many rows (`embed_window_rows`). After the window no session claims the region (`rdec_scratch_landed`,
`rdec_scratch_release`): an embedding reads its hidden rows and starts its next call at position 0. Its sessions are therefore
device-home - no host K/V - wherever the driver promises the model this window before the call
(`moe_gpu_resident_scratch_serves`; `embed` pools them, one a text of the request, a one-token text and a single text riding
the window too), so a request's host memory is its texts' rows, never a context-sized cache a text. The promise is the
window's own admission - `rdec_segs_model_fits` (no hyper-connection residual, no host experts, a mirror codec the window
serves, and for a per-layer-embedding model the device embed its side rows gather off) and the packed seat - plus one region
no live device-home session holds. A
served model the window is not promised takes host-cached sessions, one region a text, and logs why once per armed model. The server's embedding
worker asks one region of `emb_scratch_tokens` (`EMB_SCRATCH_TOKENS`, 8192, or the embedder's context where wider) - 0.9 GB
of K/V for Qwen3-Embedding-0.6B - and an input past the embedder's
context is refused with a 400; the in-handler route on a chat model shares one unpinned region of the chat model's the same way,
and answers 503 where every region holds a live device-home stream. The witness is `moe_gpu_resident_scratch_windows`.

