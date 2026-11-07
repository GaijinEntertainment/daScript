# dasLLAMA Architecture - the Vulkan FastConformer chain's rotary form

Companion to `ARCHITECTURE_GPU_TOWER_VULKAN.md`; a section is cited by its anchor. This document
carries what the Vulkan tower driver's FastConformer chain does differently for the rotary form
GigaAM serves through, beside the rel-pos form canary and parakeet serve
(`ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains`). The form itself - the rope on the
normed input rows ahead of the q and k projections, the LayerNorm conv module - is the Metal
driver's and is stated once: `ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-rotary`,
`ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-conv-ln`.

### The rotary form on the discrete homes' chain {#vk-tower-fc-rotary}

One chain (`tower_fc_blocks`, the body both discrete homes run - `ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains`) serves both
forms over the family descriptor (`TowerFc`), whose `rotary` and `conv_ln` flags play `FcForm`'s part; GigaAM's offsets are
canary's record, which `gigaam_layer_offsets` returns. On the device the rope (`TowerRopeTab`) turns the normed rows in `xb`
after v's GEMM has read their feed, and the feed is rebuilt off the rotated rows for q and k. The session's
tables (`gigaam_rope_tables`) upload once an encode into the scratch's `tabs` plane, cos at element 0
and sin at `npos * hs / 2` - the two bases the rope's arguments carry. The form allocates no rel
quartet and no rel plane, and its GEMM schedule keeps the rel record's slot empty (no rows, no
groups), so a record's index names the same GEMM in both forms.

The attention is the vision towers' padded route (`ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-routes`),
not the Metal slab trio: the 48-wide heads sit off the flash tile's 64 and 128 stamps, so the biased
restrides write each head into a 64-wide slot with zero lanes - q f32, k and v f16 - adding the
projection's bias as they read, the bidirectional h64 flash tile runs at 1/sqrt(48), and the unpad
reads the compact rows back (`tower_attn_padded`). The conv module's depthwise stamp is `TowerGaDw` (`GkDwConvRows`
FORM 3 over tap-major taps), `TowerGaDwH` over taps the carrier holds in f16 - the resident's halfword plane `taps`,
uploaded behind the GEMM planes on both lanes and folded into the resident's key; the layernorm class then runs in place over the conv rows and the act
class applies the SiLU.

The front seat (`register_gigaam_gpu_encode`) runs, off the raw samples,
the device mel - the frame window (`TowerGaFrames`), the power spectrum (`TowerSpecDft`) and the clamped-log
filterbank (`TowerMelLog`, the clamp form of `GkMelLog`, the stamp gemma4a's, canary's and qwen3a's mels
share), the templates the Metal seat stamps, the window riding the twiddle buffer past
the twiddles - then the two stride-2 convs (off the CPU mel where the caller cut one): each an f16 im2col (`TtsIm2col16`), the f16 GEMM over one slab of
both convs' weights as halves (the flavored image's `front16` piece, conv0 then conv2, each row's taps on the GEMM's 64
lattice), and the bias + ReLU rows; on Vulkan the residual rows read back and the block chain uploads them.
The encode seat (gigaam's, and parakeet's the same) asks the block chain's admission - its shape, its range and
its classes (`vt_fc_admits`) - before the front runs, and counts the front and the mel only once the blocks
served: a decline is one decline of the whole encode, which a Vulkan-flavored image then refuses (`tower_cpu_refuse`).
The chain serves GigaAM's three lanes. The q8 lane runs every
block GEMM on the q8 tiles over its schedule, as parakeet's does; the kq lane - the carrier's own planes - is the section
below. The exact lane uploads the block GEMMs' halfword region
of `wblob` as the resident's weight plane (`vt_upload_f16`, the qwen25v chain's form) and runs every block GEMM on the f16
GEMM (`rail_mm16`, the cm2 tile or its KHR twin) over the f16 feed the cm2 route already converts each input into -
one GEMM a site at the region's element offset, no schedule (the body picks the form a site off `TowerFc.f16w`); the
norms, biases and taps stay the f32 rows of `blob`. Every lane of a FastConformer family needs the f16 feed, the
conv front's GEMMs riding it: off it (the sdot4 mode) the flavor pick (`vulkan_gigaam_serves_model`, `vulkan_fc_serves_model`)
refuses, and the load maps the CPU image. A carrier whose block GEMMs hold no f16 declines `quant_mode`.

### The kq lane on the discrete homes' chain {#vk-tower-fc-kq}

A quantized GigaAM carrier serves its block GEMMs as the engine's own planes (`ARCHITECTURE_MEDIA_ASR.md#gigaam-planes`):
each GEMM a seat - its format, its quant and scale offsets - and the chain runs each seat on the tile its format names. The
Vulkan flavor's bake gathers the Q8_0 seats into the q8 planes at their seat offsets, the schedule's block base a seat's
own, and each K-quant member's seats into planes of that member's own in the tier's device layout (`kq_gather_rows_dev`,
the MoE stack gather's core over raw planes, the staging in disk order), one device-block piece pair a member keyed by its
format slot (`tower_fc_kq_piece`), in layout order, so a record's block base is its row base in the member's planes; the attach
uploads each piece as stored, and `kq_seats` keeps the slot, the base and the member pair a record. A GEMM record's site
bind carries the format: on a K-quant seat the body binds the site to the member's planes and format before the tile
(`ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains`), the chain's tile at the same column decoding the member, and the prefill's K-quant
cm2 tile - or its KHR twin - decodes the superblocks on the way in, over the f16 feed the cm2 route already converts each
input into. The lane therefore needs the f16 feed as every lane does: off it (the sdot4 mode) the flavor pick
(`vulkan_gigaam_serves_model`) refuses, and the load maps the CPU image (`ARCHITECTURE_MEDIA_ASR.md#gigaam-planes`). Every superblock format has its cm2 and KHR tiles, so a member whose tile class
fails to compile (`vt_fc_kq_tiles_ok`, asked at the chain's three widths) is a device fault: the chain declines `kq_lane`
naming the carrier's members, and the Vulkan image's encode refuses rather than reach the CPU; a carrier the lane refuses at
load never reaches the chain.
The Metal chain's form is `ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-kq`.
