# dasLLAMA Architecture - the Vulkan tower's vision chains

Companion to `ARCHITECTURE_GPU_TOWER_VULKAN.md`; a section is cited by its anchor. This document
carries the vision chains the Vulkan tower driver serves whole off the image planes - gemma4v, gemma3v,
qwen3v, qwen25v and the gemma4uv embedder - on both weight lanes. The encode chain's shape and the
residency every chain shares are `ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains`, the Metal twin
of every chain `ARCHITECTURE_GPU_TOWER.md#tower-encode-chains`, and the CPU chain is the specification,
dispatch for dispatch.

### The vision chains on Vulkan {#vk-vision-chains}

Every vision chain runs whole off the image planes, one command buffer an encode, and the soft tokens
alone come back (`vt_served_from` over the tail's host buffer; the residual rows stay on the device).
The family's encode uploads nothing but the planes: it skips its CPU im2col while the chain is
registered, and runs it only after a decline (`stem_cols(..., im2col)`).

**The stem.** The patch im2col (`TowerPatchIm2col`, one element an invocation, the pad rows and
columns zero, gemma4v's [0, 1] -> [-1, 1] map as its scale and shift) writes the column plane
`vis_col` at the scratch's row capacity; the patch conv is the f32 tile over the stem rows - gemma3v's
and qwen3v's off the norms plane (the blob whole), gemma4v's and gemma4uv's off the stem slab
`vis_w` (`vt_g4v_slab`, `vt_uv_slab`: the bf16 patch and projection rows widened on the host, the f32
ones copied), which the family uploads again whenever a fresh tower or a rebuilt buffer asks
(`vis_w_fresh`, raised by the rebuild and never read off the bytes the old buffer held). The position
rows follow: gemma4v's and gemma4uv's 2-axis add (`TowerPosAdd2d`, (row + x-table row) + y-table row),
qwen3v's stem assemble (`TowerStemAsm`: the merge-adjacent walk gathering each row's natural-order
conv row, the conv bias and its position row, the resized position plane bound as `vis_aux` where the
grid is not the table's), qwen25v's window-sort gather on the row gather over the index plane
`vis_idx`, gemma3v's learned rows through the bias pass. The stem's rows land in `x`, which the block
loop reads (gemma3v's pre-LN chain through `x_ready`, the others in the same command buffer).

**The blocks on both lanes.** A q8 tower's block GEMMs ride the q8 tile over its repacked plane as the
audio chains' do - the schedule records, the Q8_0 feeds, qwen3v's fused qkv row, gemma4v's clamped
sites through `vt_mm_clamped`. An exact tower's ride the f32 tile over the whole blob uploaded as the
norms plane (`vt_upload_f32`; gemma4v's bf16 block rows widened behind the blob, the block offsets
read at `wbase`), straight off the f32 rows the norms and seams land: no feed, no schedule
(`vt_mm_lane`, the pre-LN chain's `f32w` arm). gemma4v's input bounds apply on the way in: the
clamped sites that read `xb` copy it under their bounds into `xc` (`TowerClampCopy`, so `xb` stays
whole for the next site's own bounds) and the sites that read `att` or `hg` clamp in place
(`vt_mm_clamped32`). qwen25v, which has no q8 lane, rides its baked halfword twin through the f16
GEMM class (a bf16-sourced twin declines `quant_mode`); its gated hidden, silu(g + bg) . (u + bu),
runs on the shared bias-gate stamp (`TowerBiasGate16`) storing the halves the down GEMM reads in one
dispatch, and its window layers attend in f32 on the compact rows (`TowerWinAttn`). The three q8
families register their f32 lane as served (`register_<family>_gpu`'s `f32_lane`), so the lane policy
prefers the file's planes under the Vulkan tower as under Metal's; the audio towers keep the q8 lane.

**The tails.** gemma3v's grid pool (`TowerPool2d`), soft rms and projection behind the pre-LN chain
(`vt_vis_tail_enc` over `VtVisTail`); gemma4v's pool, the sqrt(d) scale with the standardize in one row
pass (`TowerAffineRows`, the scale alone where the file has no standardize rows), the weightless rms
off the norms plane's ones row - sized to the widest rms the chain runs, the tail's d, not the head's
hs - and the projection between its clamps; qwen3v's and qwen25v's mergers over the rows as [nout x 4d]
on the biased f32 tile with the tanh GELU, qwen3v's deepstack tap mergers inside the block loop into
the slices of one projected plane; gemma4uv's layernorms around the patch linear and the 2-axis add, then
the weightless rms and the projection - no pool. The tail rows ride
`tail_out` / `tail_proj` at the tail's widest width (`vt_tail_bufs`, rebuilt with the scratch).

**The buffers.** `vt_vis_bufs` holds the planes at the canvas's bytes, the column plane, the stem
slab, the index and aux planes and the tail rows at the scratch's row capacity; a canvas that grows
past any of them rebuilds them all and bumps `vis_gen`, so the family builds its sets again over the
new buffers (`vis_sets_gen`). One resident a family, released with the model drop's sweep.
