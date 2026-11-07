# dasLLAMA Architecture - the discrete homes' vision chains

Companion to `ARCHITECTURE_GPU_TOWER_VULKAN.md`; a section is cited by its anchor. This document
carries the vision chains the Vulkan and the CUDA tower drivers serve whole off the image planes - gemma4v,
gemma3v, qwen3v, qwen25v and the gemma4uv embedder - on both weight lanes, one dispatch sequence a family. The encode chain's shape and the
residency every chain shares are `ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains`, the Metal twin
of every chain `ARCHITECTURE_GPU_TOWER.md#tower-encode-chains`, and the CPU chain is the specification,
dispatch for dispatch. Each chain's dispatch sequence - stem, blocks, tail - is one body both discrete
homes run (`tower_<family>_chain` in `dasllama_tower_chains`, `ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail`);
the Vulkan seat runs it on `VtRail` inside its one command buffer, the CUDA tower driver
(`dasllama_cuda_tower.das`) on `CutRail`: no descriptor sets and no hazard rail, the chain's kernels queued
on the home's stream and the soft tokens read back behind one synchronize; its q8 lane reads the family's CUDA image (`cu.<family>.wq` / `.ws`,
`ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block`) and its exact lanes the file's planes as stored, the
bf16 rows of the gemma-4 towers through the f32 tile's bf16 arm (`BF16W`), as this driver's chains read
them; the q8 lane always takes the cm2 f16 feed, the home having no other GEMM feed.

### The vision chains on the discrete homes {#vk-vision-chains}

Every vision chain runs whole off the image planes, one command buffer (Vulkan) or one stream's run (CUDA)
an encode, and the soft tokens alone come back (`vt_served_from` over the tail's host buffer, `cut_served`
off the tail's device rows; the residual rows stay on the device). The section names the Vulkan seat's
classes and buffers; the CUDA seat stamps the same templates under the same dispatch names.
The family's encode uploads nothing but the planes: it skips its CPU im2col while the chain is
registered, and runs it only after a decline (`stem_cols(..., im2col)`).

**The stem.** The patch im2col (`TowerPatchIm2col`, one element an invocation, the pad rows and
columns zero, gemma4v's [0, 1] -> [-1, 1] map as its scale and shift) writes the column plane
`vis_col` at the scratch's row capacity; the patch conv is the f32 tile over the stem rows - gemma3v's
and qwen3v's off the norms plane (the blob whole), gemma4v's and gemma4uv's off the plane the file holds them in:
the f32 rows off the norms plane, the bf16 rows off the halfword plane uploaded as stored (`wb_dev`,
`vt_upload_wb`) through the f32 tile's bf16 arm (`F32GemmBf16`, picked by `rail_mm32`'s `bf16`), so no
row is widened on the host and the image carries the file's bytes. The position
rows follow: gemma4v's and gemma4uv's 2-axis add (`TowerPosAdd2d`, (row + x-table row) + y-table row),
qwen3v's stem assemble (`TowerStemAsm`: the merge-adjacent walk gathering each row's natural-order
conv row, the conv bias and its position row, the resized position plane bound as `vis_aux` where the
grid is not the table's), qwen25v's window-sort gather on the row gather over the index plane
`vis_idx`, gemma3v's learned rows through the bias pass. The stem's rows land in `x`, which the block
loop reads in the same command buffer.

**The blocks on both lanes.** A q8 tower's block GEMMs ride the q8 tile over its repacked plane as the
audio chains' do - the schedule records, the feeds, qwen3v's fused qkv row, gemma4v's clamped
sites through `tower_mm_clamped_q8`. On Vulkan gemma3v's blocks take the cm2 f16 feed where the device has
it, while gemma4v's and qwen3v's ride the mul_mm tiles over the Q8_0 feed on every device: the seat
sets the lane on its rail, and the one body carries both arms. An exact tower's ride the f32 tile over the whole blob uploaded as the
norms plane (`vt_upload_f32`; gemma4v's bf16 block rows off the halfword plane through the bf16 arm, the
tower's `blk_bf16` picking the stamp), straight off the f32 rows the norms and seams land: no feed, no
schedule (`rail_lane_mm` on the rail's `RailLane.f32`). gemma4v's input bounds apply on the way in: the
clamped sites that read `xb` copy it under their bounds into `xc` (`TowerClampCopy`, so `xb` stays
whole for the next site's own bounds) and the sites that read `att` or `hg` clamp in place
(`tower_mm_clamped32`). qwen25v, which has no q8 lane, rides its baked halfword twin through the f16
GEMM class (a bf16-sourced twin declines `quant_mode`); its gated hidden, silu(g + bg) . (u + bu),
runs on the shared bias-gate stamp (`TowerBiasGate16`) storing the halves the down GEMM reads in one
dispatch, and its window layers attend in f32 on the compact rows (`TowerWinAttn`). The three q8
families register with the f32 lane NOT served (`register_<family>_gpu`'s `f32_lane`), so the lane
policy keeps the q8 image as the default under the Vulkan tower - the q8 tiles read faster than the
f32 tile on every vision family (`PERF_LEDGER.md`, the vision shared-kernels entry) - and a tower pinned
exact serves on the f32 tile instead of falling to the CPU chain; Metal's chains register the f32 lane
as served, its f32 tiles being the faster route there. The audio towers keep the q8 lane.

**The tails.** gemma3v's grid pool (`TowerPool2d`), soft rms and projection behind the pre-LN chain
(`tower_vis_tail` over `TowerVisTail`); gemma4v's pool, the sqrt(d) scale with the standardize in one row
pass (`TowerAffineRows`, the scale alone where the file has no standardize rows), the weightless rms
off the norms plane's ones row - sized to the widest rms the chain runs, the tail's d, not the head's
hs - and the projection between its clamps; qwen3v's and qwen25v's mergers over the rows as [nout x 4d]
on the biased f32 tile with the tanh GELU, qwen3v's deepstack tap mergers inside the block loop into
the slices of one projected plane; gemma4uv's layernorms around the patch linear and the 2-axis add, then
the weightless rms and the projection - no pool. The tail rows ride
`tail_out` / `tail_proj` at the tail's widest width (`vt_tail_bufs`, rebuilt with the scratch).

**The buffers.** `vt_vis_bufs` holds the planes at the canvas's bytes, the column plane, the
index and aux planes and the tail rows at the scratch's row capacity; a canvas that grows
past any of them rebuilds them all, and the rail's seats name the new buffers at the next encode - a
set is the cached set over its plane tuple, so no set outlives the planes it binds. One resident a
family, released with the model drop's sweep.
