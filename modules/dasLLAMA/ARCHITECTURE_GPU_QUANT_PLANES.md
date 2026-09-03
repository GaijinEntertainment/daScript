# dasLLAMA Architecture - the Metal quant plane reads

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2y-2.2z: the split scale plane a Metal blob stores a superblock format's
scale row in, and the iquant GEMV codebook grid read with its per-box f4-slab twin. The GPU
backend role table these sections build on stays in `ARCHITECTURE_GPU.md` sec.1.5.

### 2.2y The Metal kq split scale plane {#metal-kq-split-scale-plane}

Every superblock format but k4, k5, q40 and iq4nl stores its Metal-blob scale row SPLIT into two
regions of one buffer: the 16-byte sub-scale strips of every superblock first, then the packed
per-superblock d tail. A kernel binds that one buffer twice - the strips at `soff = sb0 * 16` and
the tail at `doff = nsb * 16 + sb0 * 2` - so the two reads stride independently and the strip
read stays 16-byte aligned. k2 is the one shape variation: its tail is 4 bytes per superblock
(`nsb * 16 + sb0 * 4`), because it carries d and dmin. `kq_scales_of` builds the pair;
`metal_blob_scale_plane` mints it at bake time, folding each format's 20-byte decoded row into
`[16B strips][2B d]` (k3's row is 18 bytes and is already in that shape). The 2-byte tail is why
a region's bind offset must be a multiple of 512 elements - the `(off/256)*2` d-plane bind is
4-byte aligned only then - which is what `metal_blob_off_ok` and `moe_site_ok` check. iq4nl is
the exception: it reuses q40's 16-byte plane of eight f16 d per superblock, binds once, and
ignores `doff`. The Vulkan tier does not use this form - it binds the decoded 20-byte row as five
uints per superblock.

### 2.2z The iquant GEMV grid read and its f4-slab twin {#metal-iquant-gemv-grid}

Every iquant Metal GEMV - iq3s, iq3xxs, iq2s, iq2xs, iq2xxs - reads its codebook grid DIRECT
off the module's hoisted constant tables and float-expands each word in place. That is the base
form and the one that ships everywhere.

Three of them carry a second form, the f4-slab twin (`MetalKqGemvIq3sF4`,
`MetalKqGemvIq3xxsF4`, `MetalKqGemvIq2xxsF4`): the whole grid staged into threadgroup memory
once per threadgroup, pre-expanded to `float4` magnitudes, signs applied by select, so the
inner loop does no byte extraction. The twin is a PER-BOX CROWN, never a heuristic and never a
default - it wins on M5-class GPUs and REGRESSES on M1/M4-class, which have less threadgroup
bandwidth to trade for the byte work. Its PSO compiles only when the box's tune manifest
carries the crown (`metal_tensor_crowned("kq_gemv_<fmt>_f4")`), and `enc_kq_gemv` picks the
twin exactly when that PSO is non-null, so a box with no crown never compiles it.

The `kq_gemv_iq3s_f4` and `kq_gemv_iq3xxs_f4` crowns are raced (`race_gemv_f4_twin`).
`kq_gemv_iq2xxs_f4` cannot be settled by an isolated race at all and is minted from a serving
A/B instead - `ARCHITECTURE_MEASUREMENT.md` sec.2.21.
