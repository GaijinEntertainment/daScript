# dasLLAMA Architecture - the block drafter's kernels every home stamps

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the block drafter's
attention as Metal, Vulkan and CUDA stamp it from `dasllama_gpu_kernels_common.das` (`#dflash-attn-tiles`). The
drafter's round on the CUDA home is `ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-dflash-pass`; its drafts under the three
acceptances and its rounds over a tick's streams are `ARCHITECTURE_GPU_MTP_ACCEPT.md`.

### The block drafter's attention runs in key tiles {#dflash-attn-tiles}

**A drafter layer's attention over its ring is two launches on every home: a tile kernel, a workgroup a (row, head, tile
of 256 keys), and a merge kernel, a workgroup a (row, head).** A lane of a tile scores one key; the tile takes its
softmax over its own max and lands that max, the weights' sum and the weighted V row (`GkDflashAttnTile`), and the merge
rescales each tile's sums from its max to the row's and divides (`GkDflashAttnMerge`) - the same value as one softmax over
every key, in another summation order. The shape is set by the serial walk a lane makes, which is what a launch waits
on: a workgroup that holds a (row, head)'s whole ring walks every key in each of its channel lanes, 2048 steps at a full
ring, so its pass costs with the context until the ring fills. In tiles no lane walks past a tile's keys - a key's dot,
then a stripe of the tile's keys a channel - and the pass's cost stays near its no-context cost at any ring fill. Both
kernels take the tile count from the keys in view (`nctx + rows`), never from a bound scalar, so the two launches and the
partials' layout cannot disagree. The tiles' partial rows are a plane of the drafter's own,
[rows][heads][tiles][2 + head size], sized at the key cap (`gk_dflash_attn_partials`, `GK_DFLASH_ATTN_KEYS`); a joint
pass's blocks take it one after another, each block's merge behind its own tiles. `tests/_discrete_cells.das`'
attention cell holds the pair to the in-test softmax at one tile, at two, and at a head size that leaves lanes idle.
