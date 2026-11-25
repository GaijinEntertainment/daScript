# dasLLAMA Architecture - the block drafter's kernels the homes stamp

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the block drafter's
attention as Metal, Vulkan and CUDA stamp it from `dasllama_gpu_kernels_common.das` (`#dflash-attn-tiles`), and the
matrix form Metal runs in the tile pair's place at a head of 128 (`#dflash-metal-matrix`). The drafter's round on the CUDA home is `ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-dflash-pass`; its drafts under the three
acceptances and its rounds over a tick's streams are `ARCHITECTURE_GPU_MTP_ACCEPT.md`.

### The block drafter's attention runs in key tiles {#dflash-attn-tiles}

**A drafter layer's attention over its ring is two launches on every home - on Metal where the matrix form does not
serve (`#dflash-metal-matrix`): a tile kernel over tiles of 256 keys, and a merge kernel, a workgroup a (row, head).** A lane of a tile scores one key; the tile takes a query's softmax over its own
max and lands that max, the weights' sum and the weighted V row, and the merge rescales each tile's sums from its max to
the row's and divides (`GkDflashAttnMerge`) - the same value as one softmax over every key, in another summation order.
The serial walk a lane makes is what a launch waits on: a workgroup that holds a (row, head)'s whole ring walks every
key in each of its channel lanes, 2048 steps at a full ring; in tiles no lane walks past a tile's keys - a key's dot,
then a stripe of the tile's keys a channel.

`GkDflashAttnTile` is a workgroup a (K/V head, chunk of queries, tile). A K/V head's queries are its query heads
at every row of the block - `rows * grp` of them, row t's head g at `t * grp + g` - and they score the same K rows and
weigh the same V rows, so a workgroup stages a chunk of eight (`GK_DFLASH_ATTN_Q`) and reads each key of its tile once
for all eight; a launch is `gk_dflash_attn_tile_wgs` workgroups. A workgroup a (row, head) reads the tile once a query,
and a full ring's launch is bound by those reads: on the CUDA home of an RTX PRO 4500 the chunked launch is ahead of a
launch grouped by a row's heads at every filled ring, at the 27B drafter's geometry (32 heads on 8) and by more at the
E2B drafter's (12 on 4) (`PERF_LEDGER.md`, the entry on the block drafter's attention tile a (K/V head, chunk of eight
queries, key tile) a workgroup). The rings hold K and V as halves (`rk`, `rv`): the store rounds a row once, and the
tile widens what it reads.

A row still reads every key in view, so the pass costs with the ring's fill until the ring is full. The tile kernels and
the merge take the tile count from the keys in view (`nctx + rows`), never from a bound scalar, so the two launches and
the partials' layout cannot disagree. The tiles' partial rows are a plane of the drafter's own,
[rows][heads][tiles][2 + head size], sized at the key cap (`gk_dflash_attn_partials`, `GK_DFLASH_ATTN_KEYS`); a joint
pass's blocks take it one after another, each block's merge behind its own tiles. `tests/_discrete_cells.das`'
attention cell (`df_attn_case`) holds the discrete homes' pair to an in-test softmax over rings of halves at one tile,
at two, at a head size that leaves lanes idle and at a dense layer's shape, and `tests/test_metal_misc_kernels.das`'
`df_attn_gate` holds Metal's.

### Metal's matrix form of the block attention {#dflash-metal-matrix}

**On Metal a drafter whose head is 128 wide and whose window is a multiple of 64 runs the block attention on the
matrix kernel `MetalDflashFlash`, not on the tile pair** (`df_flash_on`, picked in `df_layer_attn`). Every other shape,
and a device where the kernel's pipeline did not build, takes `GkDflashAttnTile` and the merge. A drafter with a dense
layer needs the matrix form: where it does not serve, the Metal arm declines the drafter to the CPU pass
(`df_shape_ok`).

A threadgroup is four simdgroups over a (key piece, K/V head, chunk of eight queries) - the tile's chunk, on 8 x 8
matrix products. A step scores 64 keys: Q K^T into the scores, an online softmax over them, then P V into the chunk's
weighted V rows, which stay in the threadgroup between steps. The ring's steps come first, its halves loaded as they
lie; the block's own f32 rows are one more step of the last piece. A layer's keys split into pieces of four steps or
more, sixteen pieces at most (`df_flash_pieces`, `DF_FLASH_PIECES`). At one piece the kernel divides and writes the
attention rows itself; at more, each piece leaves its max, its sum and its weighted V row in the partials plane and
`GkDflashAttnMerge` joins them, a piece read as a key tile.

**The kernel's loads read past a block's rows and past a ring's last layer, into slack the allocations carry.** A matrix
load takes eight rows at a time and a step takes 64, whatever the block holds:

- **The block planes.** The step over the block's own rows loads 64 rows of K and of V from the block's first row
  (`rbase`), and a block holds fewer. `g.bk` and `g.bv` are allocated a step of rows past their rows
  (`(pr + DF_FLASH_STEP) * kvd` floats, `df_ready`), so the last block of a panel reads inside the buffer.
- **The ring.** A ring step starts at a slot that is a multiple of 64 (`DF_FLASH_STEP`) and loads the 64 slots from
  it with no wrap inside the step. A ring's planes carry the engine's `DFLASH_RING_SLACK` (64) slots past the last
  layer (`dflash_ring_bytes`) for a step that runs past a layer's slots; `df_ring_bytes` refuses to compile where the
  step is longer than the slack.

A key the kernel loads from outside the positions in view, or from a block row past the block's count, is masked
ahead of the softmax and weighs zero in P V, so a finite value in the slack never reaches a row.
`tests/test_metal_misc_kernels.das`' `df_attn_gate` holds the matrix kernel to the in-test softmax at eight pieces with
a ring that wraps, at one piece, and at a dense layer's sixteen.
