# dasLLAMA Architecture - the tower attention routes and encode chains

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2w-2.2x - the three routes that serve tower attention on Metal, and the
one-command-buffer encode chain each family the Metal tower driver serves gets - and sections
2.2aq-2.2ar, the Vulkan tower driver's row classes and attention routes, and its encode chains. The
GPU backend role table these sections build on - the tower driver's role row included - stays in
`ARCHITECTURE_GPU.md` sec.1.5.

### 2.2w The tower attention routes {#tower-attn-routes}

A tower head width is padded to `hs_pad = max(64, ceil32(hs))` - 72 and 80 both land on 96 -
because the QK stamps walk multiples of 32 while the guarded AV forms carry no multiple-of-64
requirement. A route that uses the padded width pays a restride to and from the towers' compact
rows. Three routes serve tower attention:

- **The flash route** takes a head size of exactly 72 on a canvas whose row count divides 64.
  It runs one pass - no score slab, no rowstat, no head restrides - reading K and V as
  per-head-contiguous f16 panels (the panel and tail-slack contract sits on the kernel class)
  while Q and the output ride the compact f32 rows. `DASLLAMA_METAL_TOWER_FLASH=0` pins the
  slab trio.
- **The per-window route** serves the qwen2.5 ViT's block-diagonal layers: one threadgroup per
  (window, head), one thread per query row, windows capped at 64 rows. It reads q, k and v
  compact and straight from device in f32 - a window's K and V sit in L1, and f16 staging noise
  would compound over the tower's 28 layers. Layers that attend in full restride onto the
  padded slab and take the shared trio.
- **The slab trio** - the prefill driver's QK, rowstat and AV builders - serves everything else
  and sizes its score slab at `heads x mp x nk64` halfs. Off the flash route that slab is the
  encode's only attention allocation; on it the driver takes a stub.

### 2.2x The tower driver's encode chains {#tower-encode-chains}

Each family the tower driver serves gets one chain that encodes the whole encode into ONE
command buffer and reads back once: the block loop, and - for gemma4a and qwen3a - the mel and
conv front ahead of it. A chain is dispatch for dispatch the same graph as its family's CPU
encoder loop, in the same order and at the same operand shapes, so the CPU loop is the chain's
specification and the CPU-vs-GPU transcript cells are its parity instrument. Every chain is
best-effort: it answers false (or -1) on any shape, knob, quant-mode or device decline, and the
CPU chain serves that encode. Engage is read from counter deltas (`metal_tower_stats`,
`metal_tower_f16_encodes`), never from "the model ran".

### 2.2aq The Vulkan tower's row classes and attention routes {#vk-tower-routes}

Every vision block is a handful of row operations around two tiles the LLM rails already own -
the Q8_0 batch tile (or the f16 GEMM class) and the flash tile. The row operations are
`[vk_dispatch]` classes in `dasllama_vulkan_classes.das`, each the device twin of one CPU tower
helper (`rms_rows`, `clamp_rows`, `requant_rows_q8_sized`, `rope_neox_2d_rows`,
`rope_neox_tab_rows`, `layernorm`, `add_inplace_rows`) or of one closed form (the clamped
GEGLU-quick, the tanh GELU with the CPU LUT's f16 rounding, silu(g + bg) . (u + bu)); the CPU
helper is each class's oracle in `tests/test_vulkan_tower_kernels.das`. The seams fold what the
CPU loop spells as two calls: a post-add writes x += y (+ b) and, in the same workgroup, the next
branch's pre-norm off the updated row.

The flash tile is stamped at head sizes 64 and 128, causal (the LLM stamps) and bidirectional, and
the towers take three routes:

- **The compact route** serves a head size of exactly 64 (gemma4v): q, the f16 K/V shadows and the
  output ride the compact rows straight into the h64 bidirectional tile.
- **The padded route** serves the 72- and 80-wide heads (gemma3v, qwen3v, qwen25v's full
  layers): a restride writes each head into a 128-wide slot with zero pads - the q panel f32, the
  K/V shadows f16 - the h128 bidirectional tile runs at the head's own scale (the zero pads add
  nothing to the dot), and the unpad reads the compact rows back off the padded f32 output. A
  fused [q | k | v] row (qwen3v) is read through the restride's slot stride and offset, so no
  copy splits it.
- **The window route** serves qwen25v's block-diagonal layers in f32 on the compact rows, as the
  Metal per-window route does (2.2w): one workgroup a (window, head), the window's rows (64 at
  most, `wlo` the row starts) attend each other and nothing else, the scores in workgroup memory.
  No restride and no f16 shadow: the coopmat tile's f16 staging noise compounds over the 28
  window layers of a 32-block tower - a slotted f16 tile reads 0.16 to 1.5 x rms against the
  exact chain at 32 blocks, the f32 window route 0.02 to 0.09, the Metal rung's order.

The Metal driver keeps the compact 72-wide heads on its own flash kernel (2.2w); the Vulkan
driver pays the restride instead, because the flash template's coopmat typedefs size on the head
and 72 is off every fragment lattice.

### 2.2ar The Vulkan tower driver's encode chains {#vk-tower-encode-chains}

`dasllama_vulkan_tower.das` fills the gemma4v, gemma3v, qwen3v and qwen25v hook slots on a build
without das_metal (the Metal driver owns them there). Each chain is 2.2x's shape: one command
buffer per encode walks the blocks dispatch for dispatch as the family's CPU loop does, the CPU
loop is the specification, the stem and the tail stay on the CPU, and the residual stream comes
back into the family's state. Where a chain and its family part on the seat:

- **gemma4v** serves the hook after the CPU stem, whole. **gemma3v**'s hook fires before the CPU
  stem (the Metal driver runs the stem itself), so `gemma3v_encode` finishes the stem on the CPU
  first when the tower is q8 and the driver serves the blocks alone. **qwen3v**'s hook is
  exact-lane and whole-chain (Metal's), so the driver takes the blocks-only q8 seat
  (`register_qwen3v_gpu_blocks`): after each deepstack tap block the residual is copied on the device
  into a stash read back beside x, and the tap mergers run on the CPU off those rows, a tap past
  a truncated tower's blocks skipped as the CPU loop skips it. **qwen25v** has no q8 lane, so its
  blocks-only seat (`register_qwen25v_gpu_blocks`) runs after the CPU stem and before the CPU
  tail over the baked halfword twin through the f16 GEMM class; a bf16-sourced twin declines.

The q8 families ride the q8 image the CPU lane reads: the driver's `serves` answer to the lane
policy is no, so a box with the driver keeps the q8 lane, and the device chain's parity
instrument is the three-way twin cell - the exact CPU chain, the CPU q8 chain and the device
chain on the same canvas, the device's distance from the exact chain held within 1.5x the CPU q8
chain's own. The x64 q8 plane is the active backend's grp interleave, which the tiles do not
read, so the upload gathers each block's GEMM rows row-major through `q8_gather_rows` with f16
scales - the resident MoE driver's precedent - once per tower into device memory, beside the
per-block norm rows with a ones row appended (the weightless per-head norms read it). qwen25v's
upload copies the twin's block GEMM region verbatim - IEEE halves, the GEMM offsets rebased to
the region's start - beside the block rows; the family has no scale plane. The residency key -
over the q8 plane, or over qwen25v's halfword twin - folds the plane's address, size, sampled
words and the served block count: an
address is not an identity, a truncated tower minted from the same bytes lands at the freed
address, and a resident sized for fewer blocks reads past its rows under a deeper chain. The
model drop's sweep tells the driver to forget its handles through `register_vk_drop_hook`
(every buffer and set is model-owned); `vulkan_tower_shutdown` is the tests' release. The
per-encode scratch is sized to the canvas and grown when a taller one arrives; every batch
schedule of an encode sits in one meta buffer sized by the encode (a record and a map a
dispatch), so nothing the command reads moves under it; the batch tile's variant is picked per
output width - d, ff and qwen3v's fused 3d can each pick a different tile.

The declines: `quant_mode` on an exact-lane tower (or the bf16 twin), `shape` off the tile's head
sizes or past the row cap, `knob` (`DASLLAMA_VK_TOWER`), `device` where the tier's want
(`DASLLAMA_GPU`, read before any device init) or a class declines. Engage is
`vulkan_tower_stats` and `vulkan_tower_declines` deltas, and the bench's image cell prints them
around its timed turn.
