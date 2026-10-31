# dasLLAMA Architecture - the NVFP4 member

Companion to `ARCHITECTURE_ENGINE_FORMATS.md`; a section is cited by its anchor. This document carries
the nv4 superblock member: how its planes hold NVFP4's disk bytes, and the per-tensor scale that rides
beside them.

### The nv4 planes are NVFP4's disk bytes in the k4 tiling {#nv4-planes}

An NVFP4 disk block holds 64 weights in 36 bytes: four UE4M3 scales, one a 16-weight sub-block, then 32
nibble bytes in which sub-block s's byte j pairs element j (low nibble) with element j + 8 (high). A
superblock is four disk blocks. The transcode lays each 32-weight block g - sub-blocks 2(g % 2) and
2(g % 2) + 1 of disk block g / 2 - in q40's k4 tiling: quant byte j of block g holds element j low and
element j + 16 high (`NV4_QSB`, 128 bytes a superblock). The sixteen scale bytes copy verbatim, block g's
low half under byte 2g and its high half under 2g + 1 (`NV4_SSB`).

A weight is `MX4_LUT[q]` - the e2m1 codebook doubled, MXFP4's table - times its sub-block's UE4M3 scale at
half (`ue4m3_half_f32`), the halving compensating the doubling. The GGUF reader and the reference dequant
(`dequant_nv4_plane_superblock`) decode this way.

The disk-order dot (`dot_kq`) takes two exact int dots a 32-block, one over the low 16 and one over the
high 16. It folds each by its own sub-block scale into one float a superblock, then multiplies by the
Q8_K scale: q40's per-block fold order over two scales. The codes are signed, so no bsum term enters.

The grp<mr> repack keeps q40's plane shapes: the k4 nibble tiling as a column gather, and the scale row
interleaved per block as [blk 0..7][mr x (low, high) UE4M3 byte pair], the pair sitting where q40 carries
its f16 d. The grp dequant and the device gather (`moe_gpu_gather_stack_kq`) read the pair from there.
The generated tile and GEMV (`nv4q8_tile_gen`, `nv4q8_gemv_gen`) are the iq4nl block body under the mx4
codebook with the two strips' dots kept apart: each block folds its lo and hi integer dots by the pair's
UE4M3 scales (decoded in IR as `ue4m3_half_f32`'s bit form) into one float partial a superblock, and the
token's Q8_K scale multiplies that partial once at the superblock's end - the disk-order dot's fold order,
no superblock d.

The row-major kernels (`nv4q8_rm_gemv_gen`, `nv4q8_rm_tile4_gen`) read the device row the gather lays
out - the k4 nibbles verbatim, the sixteen UE4M3 bytes, then the per-tensor factor as the row's fifth
word - and fold as the generated tile does, each strip's scale multiplied by that word; the one-copy
rare rail serves a streamed nv4 expert stack through them (`rm_dev_serves`).

### A per-tensor weight scale rides beside the nv4 planes {#nv4-site-scale}

An NVFP4 weight is its e2m1 code times its 16-weight sub-block's UE4M3 scale times one f32 the whole tensor shares - a
separate `<tensor>.scale` GGUF tensor three mantissa bits cannot absorb. The planes keep the disk bytes; `detect_site_scales`
reads every `.scale` after the layout into `Model.site_scale`, keyed by format slot and plane offset (`site_scale_key`). A
routed stack's `.scale` holds one value an expert (`n_expert` long, or one for the stack): `note_stack_scales` lands each
under its expert's own plane offset, so an expert-offset epilogue and the gather read it like a dense plane's, and keeps the
vector under the stack's offset (`Model.exps_scale`), which the rare rail's lanes read as a pointer (`RarePlane.scales`:
a planar read of an expert's rows multiplies them, a device-layout read folds the row's word). Both tables ride the image
meta (`serialize_image_meta`), so a mapped `.dlim` serves the scales the gguf load read. A shared expert's `.scale`
is a dense plane's. A `.scale` beside a plane served as any member but nv4 refuses the load by name (`note_site_scale`,
`note_stack_scales`): only the nv4 device rows carry the factor, so any other member's device rail would drop it. A
`.scale` at a site no epilogue scales - a fused gate-up stack, the token table, the classifier, the NextN projection, the
PLE table - also refuses the load by name (`refuse_site_scale`). The CPU epilogues multiply their output rows by
`site_scale(t, fmt, woff)` - `mm_at_kq_pre`, `mm_b_kq_pre`, the region-list routers (`mm_at_kq_groupn`, `mm_b_kq_groupn`:
a region is one expert's plane) and the fused decode chains, which scale each 32-row slice ahead of any bias or activation.
The device rows carry it: `moe_gpu_gather_stack_kq` writes the slice's f32 bits - a slice is one expert's plane - into the
fifth word of each nv4 superblock's 20-byte scale row (the pad past the sixteen UE4M3 bytes; halves cost tg128 5%).
`Nv4GemvT` and `KqBatchNv4` multiply it into each sub-block scale;
`Nv4Cm2T` stages the 128 UE4M3 codes at half under it once a workgroup (`UE4LUT`, off the region's first row) and decodes
through that table and the e2m1 table - iq4nl's shape; the CUDA home's FP4 leaf `MmqFp4T` reads it once a tile off the tile's
first row into its epilogue (a plane is whole 128-row tiles, so a tile never straddles two factors). No kernel takes the factor
as an argument: the plane carries it, so a site the driver never named (a deltanet projection, a merged K/V pair) scales the same.
The Metal tier binds the CPU scale row verbatim - its compact 16-byte form carries no pad word
(`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane`) - so a plane under a `.scale` declines there by site and
member (`kq_load_gpu_unserved` for a dense or table plane, `moe_fmts_unserved` and `shexp_fmts_unserved` for a routed or
shared-expert stack, `ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries`), and the mints with no `.scale` serve.
