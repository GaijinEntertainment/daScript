# dasLLAMA Architecture - the block drafter's kernels the homes stamp

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the block drafter's
attention as Metal, Vulkan and CUDA stamp it from `dasllama_gpu_kernels_common.das`, with the grouped tile the
discrete homes add from `dasllama_gpu_kernels_discrete.das` (`#dflash-attn-tiles`). The
drafter's round on the CUDA home is `ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-dflash-pass`; its drafts under the three
acceptances and its rounds over a tick's streams are `ARCHITECTURE_GPU_MTP_ACCEPT.md`.

### The block drafter's attention runs in key tiles {#dflash-attn-tiles}

**A drafter layer's attention over its ring is two launches on every home: a tile kernel over tiles of 256 keys, and a
merge kernel, a workgroup a (row, head).** A lane of a tile scores one key; the tile takes a head's softmax over its own
max and lands that max, the weights' sum and the weighted V row, and the merge rescales each tile's sums from its max to
the row's and divides (`GkDflashAttnMerge`) - the same value as one softmax over every key, in another summation order.
The serial walk a lane makes is what a launch waits on: a workgroup that holds a (row, head)'s whole ring walks every
key in each of its channel lanes, 2048 steps at a full ring; in tiles no lane walks past a tile's keys - a key's dot,
then a stripe of the tile's keys a channel.

The tile kernel is one of two. `GkDflashAttnTile`, which every home stamps, is a workgroup a (row, head, tile).
`GkDflashAttnTileHeads` (`dasllama_gpu_kernels_discrete`), which the discrete homes stamp beside it, is a workgroup a
(row, the query heads of one K/V head, tile): those heads score the same K rows and weigh the same V rows, so a
workgroup a head reads a tile once a head and a full ring's launch is bound by those reads, where the grouped workgroup
reads a key's K row once for its heads (`gk_dflash_attn_qh`: four heads where the group is a multiple of four, two where
it is even, else one; `gk_dflash_attn_heads_wgs` is the launch's workgroup count). A discrete home takes the grouped
tile where a K/V head serves two or four heads a workgroup and the tile a head where it serves one
(`gk_dflash_attn_grouped`): a workgroup of one head shares no read and pays for the grouped body (`PERF_LEDGER.md`,
the drafter attention tile entry). Metal stamps the tile a head alone.

A row still reads every key in view, so the pass costs with the ring's fill until the ring is full. The tile kernels and
the merge take the tile count from the keys in view (`nctx + rows`), never from a bound scalar, so the two launches and
the partials' layout cannot disagree. The tiles' partial rows are a plane of the drafter's own,
[rows][heads][tiles][2 + head size], sized at the key cap (`gk_dflash_attn_partials`, `GK_DFLASH_ATTN_KEYS`); a joint
pass's blocks take it one after another, each block's merge behind its own tiles. `tests/_dflash_oracle.das`' softmax
(`df_attn_fixture`) is the reference of both: `tests/_discrete_cells.das`' attention cell holds each discrete tile
with the merge to it at one tile, at two, at a head size that leaves lanes idle, at each count of heads a workgroup
scores (four, two, one) and at a K/V head wider than a workgroup, and `tests/test_metal_misc_kernels.das`'
`df_attn_gate` holds Metal's pair to it at the same shapes.
