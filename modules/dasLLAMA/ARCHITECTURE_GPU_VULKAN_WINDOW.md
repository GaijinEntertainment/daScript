# dasLLAMA Architecture - the Vulkan prefill window's per-session parts

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries what a window does a
session at a time: the recurrent block, whose state and ring belong to one region, the segmented window that holds several
sessions' chunks, and the short window's peel through the token commands. The chain these sit in is
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
use the other, which the word names after the call (`pf_dn_end`) - the parity the owner handoff hands the session. The weight planes stay in their file
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
32-lane one; only the KHR tile keys on the width itself (`ARCHITECTURE_GPU_VULKAN_KHR.md#khr-mm-kq-tile`). The conv and smalls bindings are NonWritable, as every
binding no kernel of a class writes is (`ARCHITECTURE_GPU_VULKAN.md#vk-readonly-lens`). The decoration is load-bearing: without it the driver orders each token's k and q
loads behind the previous token's o store (the two buffers may alias), and the same kernel ran
2.35x slower (19004 against 8084 us over the 0.8B's 18 layers on the Linux RTX 5080; the staged
two-column form it replaced 8854). The raw o rows land in the per-op tier's workspace, for the
gated out-norm's one workgroup per position.

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
row), so its store and shadow run once over the whole window. A window past `PF_WINDOW` rows loops as the
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
call falls to the engine, which prefills the segments one at a time through the one-session override. The
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
