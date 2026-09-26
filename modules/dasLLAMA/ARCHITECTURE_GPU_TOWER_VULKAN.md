# dasLLAMA Architecture - the Vulkan tower and ASR-decoder drivers

Companion to `ARCHITECTURE_GPU_TOWER.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2aq-2.2ar, the Vulkan tower driver's row classes and attention routes, and its
encode chains, and section 2.2at, the Vulkan ASR-decoder driver. The Metal sections they cite by
number - 2.2w, the tower attention routes, and 2.2x, the encode chain's shape - stay in
`ARCHITECTURE_GPU_TOWER.md`.

### 2.2aq The Vulkan tower's row classes and attention routes {#vk-tower-routes}

Every vision block is a handful of row operations around two tiles the LLM rails already own -
the Q8_0 batch tile (or the f16 GEMM class) and the flash tile. The row operations are
`[vk_dispatch]` classes in `dasllama_vulkan_classes.das`, each the device twin of one CPU tower
helper (`rms_rows`, `clamp_rows`, `requant_rows_q8_sized`, `rope_neox_2d_rows`,
`rope_neox_tab_rows`, `layernorm`, `add_inplace_rows`) or of one closed form (the clamped
GEGLU-quick, the tanh GELU with the CPU LUT's f16 rounding); the CPU
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
and 72 is off every fragment lattice. Per family, both drivers:

| family | head | Metal attention (2.2w) | Vulkan attention | Vulkan weights |
|---|---|---|---|---|
| gemma4v | 64 | the slab trio | the compact route, h64 tile | the q8 image, gathered on upload |
| gemma3v | 72 | the flash route (rows a multiple of 64), else the slab trio | the padded route, h128 tile | the q8 image, gathered on upload |
| qwen3v | 72 | the flash route (rows a multiple of 64), else the slab trio | the padded route off the fused row | the q8 image, gathered on upload |
| qwen25v full layers | 80 | the slab trio | the padded route, h128 tile | the baked halfword twin |
| qwen25v window layers | 80 | the per-window route | the window route, f32 | the baked halfword twin |

### 2.2ar The Vulkan tower driver's encode chains {#vk-tower-encode-chains}

`dasllama_vulkan_tower.das` fills the gemma4v, gemma3v, qwen3v and qwen25v hook slots, the three
audio blocks seats - the whisper-class block loop (`register_tower_blocks_gpu`), gemma4a's
(`register_gemma4a_gpu`) and canary's (`register_canary_gpu`) - and the audio front seats
(qwen3a's mel and conv front, gemma4a's whole chunk, canary's front; 2.14 in
`ARCHITECTURE_MEDIA.md`) on a build without das_metal (the Metal driver owns them there). Each
blocks chain is 2.2x's shape: one command buffer per encode walks the blocks dispatch for dispatch
as the family's CPU loop does, the CPU loop is the specification, and the residual stream comes
back into the family's state. A front chain is the same shape over the family's front: it starts
from the CPU-windowed frames (`canary_window_frames`, gemma4a's and qwen3a's windowing halves - the
one host phase every audio chain keeps) and runs the spectrum, the mel, the subsample convs and the
input projection in one command buffer, so the blocks chain reads the residual rows the front wrote
and the tail (the projector, the post-norm) stays on the CPU except where a bullet below says
the chain runs it (gemma4a's projector tail). Where a chain and its family part on the seat:

- **gemma4v** serves the hook after the CPU stem, whole. **gemma3v**'s hook fires before the CPU
  stem (the Metal driver runs the stem itself), so `gemma3v_encode` finishes the stem on the CPU
  first when the tower is q8 and the driver serves the blocks alone. **qwen3v**'s hook is
  exact-lane and whole-chain (Metal's), so the driver takes the blocks-only q8 seat
  (`register_qwen3v_gpu_blocks`): after each deepstack tap block the residual is copied on the device
  into a stash read back beside x, and the tap mergers run on the CPU off those rows, a tap past
  a truncated tower's blocks skipped as the CPU loop skips it. **qwen25v** has no q8 lane, so its
  blocks-only seat (`register_qwen25v_gpu_blocks`) runs after the CPU stem and before the CPU
  tail over the baked halfword twin through the f16 GEMM class; a bf16-sourced twin declines. Its
  gated hidden, silu(g + bg) . (u + bu), runs on the LLM's biased f16 act stamp (`ActF16B`) at a
  zero row map (`rex_dev`: every row the one expert), with the norms plane bound as both bias
  planes, so the halves the down GEMM reads land in one dispatch.
- **The whisper-class towers** (whisper, qwen2audio, voxtral, ultravox, the Omni audio towers,
  Qwen3-ASR through its conv front) share one chain with gemma3v: the pre-LN block loop
  `vt_ln_chain` over one offsets record a block (`LayerOffs`; gemma3v's block offsets mapped onto
  it), at the 64-wide heads every whisper-class tower carries, the GELU arm the tower declares
  (the tanh LUT or erf), the k bias where the family has one. On the cm2 feed the chain's row
  passes store the feed themselves: the layernorm and the residual seams write f16 (the stamps'
  f16 twins), the q / k / v biases ride the head restrides (the biased pad stamps), the unpad
  stores the o GEMM's feed and the fc1 bias + GELU the fc2 feed - six row passes a block; the
  mul_mm arm keeps the f32 rows and a requant before every GEMM. The fc2 GEMM - the chain's one
  K past 2048 - takes the prefill's split-k pick (`cm2_gemm_pick` over the ff-deep K against the
  resident's partial planes, `SplitKReduce` summing them into the fc2 rows) where the wave model
  favours it and the encode carries `VT_SK_MIN_ROWS` = 512 rows or more (the window the model's
  units were measured at; `DASLLAMA_CM2_SPLITK` overrides the chunk count, `0` the pick, and
  `vt_sk_build` logs the split once per model): 60 tiles of the 1280-wide whisper rows fill a 36-SM card whole and leave a quarter of an
  82-SM card idle, while a 128-row window's seven tiles pay the reduce for nothing. The conv stem
  (`register_tower_conv_gpu`)
  serves on the same resident where the GEMMs ride the cm2 feed: conv1 as a k3 im2col to f16 and
  the f16 GEMM over an f16 slab of the file's f32 weights (3 n_mel padded to 64), conv2 as an
  im2col into the chain's f16 feed and the q8 tile over the plane the upload gathers beside the
  block records, the biases, GELU and position rows off a plane of the stem's own; the residual
  rows read back into the state and left in the chain's x buffer, which the block loop reads
  without its upload (`x_ready`). The mel and the projector stay on the CPU, and the mul_mm arm
  keeps the CPU stem. Qwen3-ASR's front takes its own two seats: the mel hook runs the
  frames' power spectrum and the mel-major log rows (the global clamp stays with the caller), and
  the front hook runs the window's chunk convs (im2col to f16 and the f16 GEMM a stage, the
  bias + activation as a row class), the feature shuffle, `conv_out` on the cm2 q8 tile and the
  bias + positions, read back into the state the block loop then serves - so a second window on
  the same residency recomputes every stage's offsets from its own chunk count. A position plane
  adds through the bias class as one bias row as wide as the rows it covers (the class repeats its
  bias row every `d` elements): the whisper stem's row is its whole plane, so each position row
  lands once, and qwen3a's is one chunk's 13 position rows, so every chunk takes them - qwen3a's
  finish is two bias passes, the `conv_out` bias row, then those positions.
- **gemma4a** serves the whole chunk (`vulkan_gemma4a_chunk`: the DFT with the magnitude arm and a
  frame-major store, the mel, both subsample convs as im2col + the f16 GEMM + a LayerNorm-ReLU row
  class, the input projection on the cm2 q8 tile) and hands the residual rows to its blocks chain,
  which ends in the projector tail the CPU `g4a_projector_tail` spells - the out-proj with its bias,
  the weightless rms off the norms plane's ones row, the mm-proj - and reads the projected rows
  back into the state's `out` with `out_ready` raised, so the family's CPU tail stands down on
  both routes (the chunk's and the blocks-only one): the macaron halves as the
  post-add class with a half residual weight, the clamped GEMM sites as the arm's feed (the
  clamp + halfword store on the cm2 feed, the clamp-requant on the mul_mm tiles), the tile and the
  output clamp where the site's calibration record is live (`vt_mm_clamped`, the device twin of
  the CPU's `g4a_mm_clamped`; canary's chain rides it with no clamp record, as its CPU `cn_mm_rq`
  has none), the chunk-12 relative-position attention in its own class with the
  query's per-dim scale and the key scale folded in, the conv module's GLU and causal depthwise
  taps as row classes. The RPE table rides the resident's rel quartet and its projection is a
  13-row batch GEMM on its own set.
- **canary** serves the blocks seat with the whisper-class row classes (layernorm, the bias
  class with a silu mode, the layernorm seam with the half residual weight) plus two of its own:
  the full bidirectional Transformer-XL attention with the u/v biases and an online softmax, and
  the centered depthwise conv with the folded BatchNorm and silu. The front seat runs the spectrum,
  the mel and the per-feature normalization, conv0 (im2col, the f16 GEMM, bias + relu, the length
  mask), the two depthwise + pointwise stages (the pointwise on the cm2 q8 tile), the feature rows
  and the input projection into the residual stream. The sinusoidal rel table is per encode
  (2 npos - 1 rows) and a row class builds it on the device ahead of the blocks (the CPU table stays
  the CPU chain's), so the rel quartet is sized by the scratch and the table's projection is a
  batch GEMM over those rows. The attention does not walk the projected table per row: per head
  the (q + v) and rel-table panels are the restride stamps at one head - unpadded f16 panels, the
  (q + v) one on the biased stamp with the head's slice of `bias_v` as its bias row - the plane R = (q + v) P^T runs on the f16
  coopmat GEMM, and a tiled online-softmax kernel (`TowerCnAttnRT`, a workgroup per 64 query rows
  of one head) scores ((q_i + u) . k_j + R[i][npos - 1 - i + j]) x scale, reading R beside the keys
  and values staged 32 a tile through workgroup memory - the plane's [cap x 2 cap - 1] f32 rows are
  the resident's `vt_cn_rel_plane` scratch,
  grown by the row capacity. The chain carries no row cap of its own (`vt_cn_rows_ok`): the
  scratch, the rel quartet and the plane each bind as one range, and the clip declines only where
  one passes the device's range. The family's hook fires on either lane: the Metal driver declines
  the q8 encoder, this one the f32 encoder.

The GEMM sites of the gemma4a and canary chains ride the cm2 f16 feed where the device has the
cm2 tile family (`vt_cm2_feed`): a site names its tile once (`VtTile` - the cm2 token column, or
the q8 batch variant off cm2), one clamp + halfword store class feeds every input and the rel
table, and the Conformer sets are bound per tile. The f16 feed (`xh_dev`) keeps stale rows past
the encode's live count by design: the feed's rows past the live count reach no live row, because
every restride reads `rows` and the GEMM output rows past npos those stale rows produce are dead.
Each block family lists its GEMM regions once, in record order (`vt_g4a_regions`,
`vt_cn_regions`, `vt_ln_regions`, `vt_q3v_regions`): the upload gathers the regions in that order,
and the schedule walk maps its records in the same order beside a per-record tile or group list,
so a record's index names one region in both walks.
A GEMM record on the l column carries the encode's rows rounded up to 256 (`vt_tile_rows`), and
the scratch's row cap is a multiple of the l column (`vt_cap_rows`, so every plane the scratch
sizes holds a record's rounded rows), so the l stamp's last column is whole and takes its fast path: a partial column runs its clamped edge path at a third of the
rate (whisper's 1500-row chunk read q/k/v/o 95 us against 52 at 1536, fc2 357 against 185, on the
RTX PRO 4500). A record on the s or m column keeps the raw count - those stamps load a partial
column unclamped and clamp the store, and a plane sized to the record (gemma4a's 13-row rel
projection) holds nothing past it. The rounded rows past the live count are the same dead rows.

The whisper-class stem leaves its rows on the device (`x_ready`, a pending readback naming the
encoder state's `x`), and the block chain takes them there; a block hook that declines after the
stem served lands them first (`vt_x_flush`), as does the resident's release, so the CPU block loop
reads what the stem computed and the served loop never copies them out. The whisper encode asks
the blocks-with-post-norm seat (`register_tower_blocks_final_gpu`) before the blocks seat: on it
the chain folds the tower's post-norm into the last block's post-add (the f32 post-add over the
post-norm's rows, which ride the norms plane behind the layers'), reads the normed rows back into
`xb` instead of `x`, and records the device plane holding them (`vulkan_tower_enc_out`), which the
whisper cross-KV chain copies device to device in place of the host upload.
Under `DASLLAMA_GPU_PROF=1` every dispatch of an
audio chain writes a timestamp with a role (`VtProfRole`), and `vt_prof_report` prints the chain's
device time per role beside the host wall after the encode - the ledger the levers are read from.

The rel quartet is the resident's relative-position scratch, the device buffers sized by
`rel_cap_rows` and grown by `vt_rel_bufs`: the table's f32 rows, the table's GEMM feed
refreshed once per encode - Q8_0 quants and scales for the mul_mm tiles, f16 halfwords with the
tile read slack behind the rows for the cm2 tiles - and the projected rows one layer at a time;
its projection sets are cached per tile key (`vt_rel_proj_set`: the batch variant, or the token
column over the f16 feed on cm2), and dropped with the scratch or the quartet, because they bind
the scratch's schedule buffer beside the weights and the quartet. The two attention classes are stamped per head width (64 and 128, the widths
the served carriers carry; the driver declines another) and the residual seam per post-norm
(`TowerPostAdd`, `TowerPostAddPlain` for the conv module's plain add), so no kernel loop
bound or branch reads a push constant. gemma4a and canary upload
the family's whole f32 blob as the norms plane - the per-block norm, bias and conv rows read at
the CPU chain's own offsets, so no per-row gather runs - while the pre-LN families (whisper-class,
gemma3v) upload the blob from their layers' base row on and the chain rebases every norm and bias
offset on that `norms_base`.

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
over the q8 plane, or over qwen25v's halfword twin - folds every plane the upload reads: the
weight plane's address, size and sampled words, the scale plane's, the norm rows' region, the
served block count and offsets, and the active repack layout. An address is not an identity: a
truncated tower minted from the same bytes lands at the freed address, and a resident sized for
fewer blocks reads past its rows under a deeper chain. The
model drop's sweep tells the driver to forget its handles through `register_vk_drop_hook`
(every buffer and set is model-owned); `vulkan_tower_shutdown` is the tests' release. The
per-encode scratch is sized to the canvas and grown when a taller one arrives; every batch
schedule of an encode sits in one meta buffer sized by the encode (a record and a map a
dispatch), so nothing the command reads moves under it; the batch tile's variant is picked per
output width - d, ff and qwen3v's fused 3d can each pick a different tile.

The declines: `quant_mode` on an exact-lane tower (or the bf16 twin), `shape` off the tile's head
sizes, past the row cap (`VT_MAX_ENCODE_ROWS`; the canary chain reads the range instead), or with
a weight plane or scratch buffer over `vk_max_storage_range()`, or where the family's constants
differ from the ones the kernel home stamps (`Q3A_TOK_PER_CHUNK`, qwen3a's 13 positions a chunk;
`G4A_ATTN_PAST` and `G4A_ATTN_CAP`, gemma4a's 12-row window and cap 50),
`knob` (`DASLLAMA_VK_TOWER`), `device` where the tier's want (`DASLLAMA_GPU`, read before any
device init) or a class declines, or for every front where the f16 GEMM feed is off
(`vulkan_tower_front_route`: the `DASLLAMA_COOPMAT=sdot4` mode - the block chains ride the q8
batch tile and the KHR flash tile there and serve), `memory` where an allocation fails (the partial
upload or scratch is released, the CPU chain serves). Each decline is logged once per reason per model.
Engage is
`vulkan_tower_stats` and `vulkan_tower_declines` deltas - the stats count encodes, rows and
blocks, the front dispatches (`convs`) and qwen3a's device mels (`mels`) - and the bench's image
and ASR cells print them around their timed turns.

### 2.2at The Vulkan ASR-decoder driver {#vk-asr-decoder}

`dasllama_vulkan_asr_dec.das` fills the whisper decoder's two hook slots
(`register_whisper_cross_kv_gpu`, `register_whisper_decode_gpu`) on a build without das_metal, the
Metal ASR-decoder driver's twin, q8-native: the f32 decoder rail declines `quant_mode` and the CPU
chain serves it. The decoder attaches once per model, its residency key `vt_key` over the
decoder's planes (the tower's key, so an address alone is never the identity): the tied embedding
and every layer's ten GEMM regions gather row-major with f16 scales through `vt_gather_regions`,
the gather the tower drivers use, at their own plane offsets (one weight buffer, every GEMV site addressing it
by its first block), the layers' f32 rows and the final norm upload as one norms plane the row
classes read at rebased offsets, and the K/V planes and the step scratch are sized to the model -
the cross K/V as f16 [layer][head][ta][hs] (unscaled; the attention carries the whole hs^-0.5),
the self cache as f16 [layer][head][tmax][hs].

The cross-KV chain is one command buffer per window: the encoder rows fed once (copied device to
device off the tower's plane when the tower landed exactly these rows - `vulkan_tower_enc_out`,
counted as a handoff - else uploaded from the host; then the halfword
store on the cm2 feed, the Q8_0 requant on the mul_mm tiles), then per layer the ck and cv GEMMs on
the tile the tower chains ride (`VtTile`, the schedule records written as the tower's) and the
K/V store class (`TowerWdecKv`), which writes a projection's [rows x d] output - a head's hs
columns a row - into head-major planes, run twice off each projection - the f16 resident plane, and the CPU chain's f32
layout (kx pre-scaled and transposed, vx with its bias) on device planes of its own. That layout is
read back into the decoder state only when that state's own CPU reader can need it - the step's
`rows` decline on a window's first batch, and the step called on it while it is not the live
window (`wd_flush_cross_kv`, over a pending mark the cross-KV chain leaves) - so the served loop
never pays the 61 MB copy a turbo window's layouts make, and a state's next window supersedes its
pending one unread. The driver never writes into another state, which may already be gone: another
state's window, and the release, drop the pending readback and mark the superseded state, whose
next decode step panics under the one-live-window rule until it starts a window of its own. The decode step is
one command buffer per batch: the token and position rows summed on the host and uploaded, then
per layer the LN with its Q8_0 feed in one dispatch (`TowerLnRq`, the first layer), the fused
q|k|v GEMV (three regions of one dispatch, the N-column form over a batch's rows), the two f16
appends with the v bias folded, the chunked self attention (the q bias folded at the load; its
combine quantizes the row's head slice in place of storing it, `TowerWdecAttnCombRq`), the o GEMV
and the post-add with the next norm and its feed (`TowerPostAddLnRq`), the same over the cross
memory, the fc1 GEMV, the bias + tanh-LUT GELU with its feed (`TowerBiasActRq`), the fc2 GEMV and
the post-add with the next layer's first norm and its feed (the final norm on the last layer,
whose feed is the logits GEMV's), then the tied-embedding logits GEMV over the last row alone and
the logits readback - the CPU filter and sampler stay the parity anchor. The fused passes land
the same Q8_0 bytes as the row pass and the separate requant (`TowerClampRq`) - the block store is
`Q8BlockStoreT`'s, the one text every requant stamp shares, eight consecutive lanes a block, and the
fused passes are their f32 templates' `OUT_Q8` stamps - and cost a token 66 dispatches where the separate
passes cost 91, the dispatch floor of a decode step being its own launch and barrier. A partial's workgroup
(`TowerWdecAttnPart`) is one chunk of 256 keys, one head and one row - the chunk's scores off the
f16 keys, its own max and exp-sum, its unnormalized weighted values - so a 1500-key cross window
spreads over six workgroups a row. The combine's workgroup (`TowerWdecAttnCombRq`; the f32-storing
`TowerWdecAttnComb` is the kernel cells' reference form) is one head and
one row: it merges the chunks' partials by log-sum-exp into the row's head slice of the [rows x
dim] output. A batch wider than the step's row
cap (`WD_ROW_CAP`) at the window's first batch hands the window to the CPU chain (`rows`);
mid-window the device owns the self-KV, so a wider batch or an evicted window panics. Engage is
`vulkan_wdec_stats` deltas (windows, steps, tokens); the parity instrument is
`test_whisper_vulkan_wdec`.

The decoder's declines (`vulkan_wdec_declines`): `knob` (`DASLLAMA_VK_WDEC=0`, or
`set_vulkan_wdec(false)`), `quant_mode` on the f32 oracle rail (the driver reads the q8 image),
`shape` on a width off the tile's lattice or a head width other than the kernel home's `WDEC_HS`
(64), `device`
where the tier has no device or a class fails to build, `memory` where a device or pinned
allocation fails, and `rows` where the window's first batch is wider than the step's row cap - the
CPU chain serves the window. Each is a counted fallback at the window's claim; mid-window the
device owns the self-KV, and a failure there panics.
