# dasLLAMA Architecture - the prepared-image rail, the baked GPU planes

Companion to `ARCHITECTURE_IMAGE.md`; this document carries the planes a mint bakes for a GPU route - the dev-W f16 panel
plane, the tower twin-W plane and a GPU-flavored tower image's device block - each section cited by its anchor.

### The baked dev-W f16 panel plane {#image-devw-plane}

A metal-flavor image minted under the dev-W panel lever (`set_metal_devw_panels`, its lane `metal-devw`; off, the sites
take per-window scratch panels and the image carries none) carries `devwf16`: every dev-W-eligible q8, k4/k5/k6 and split-scale
(iq4xs, iq4nl, q40, k3, iq3s, iq3xxs, k2, iq2s, iq2xs, iq2xxs) weight - a plane job whose f16 panel fits
`DEVW_BAKE_PANEL_MAX`, the prefill knee's mirror, and whose tensor is not an expert stack (`*_exps.weight`: gemma-4
splits its fused stacks per expert at load, and the routed block reads the quantized planes with no dev-W route, so a
baked expert panel is never read) - pre-dequantized at mint into one concatenated f16 plane, with parallel site
tables (`devwf16_fmt/key/off/n`) serialized as ordinary planes. The mint dequant is the format's CPU mirror of the runtime dev-W dequant
kernel. The q8 and K-quant mirrors are hand-written and bit-exact by construction: each f32
product fits the 24-bit significand exactly (the widest, k6's `dsc*q`, needs 24), so only the
final f16 round exists and contraction cannot move it. The split-scale mirror is the dequant
class itself run on the CPU (`devw_dequant_kq_mirror`, registered into the loader by the
Metal prefill at init - a build with no Metal tier bakes none of those formats): its
`stage16` decodes the same 64x64 chunk into the same tile the kernel stages, and the
kernel-gate cell holds the two bit-equal per format. Either way a baked panel is bit-equal
to a runtime one. The first served prefill seeds the resident-panel cache with zero-copy
`(buffer, offset)` views over the mapped plane, so a baked site never dispatches a dequant
and holds no dedicated memory; sites the bake does not cover (q51 - the dev-W route has no
dequant kernel for it - and owned gguf loads) stay on the runtime path, resident under
`DASLLAMA_METAL_DEVW_RESIDENT` or scratch. Which form a site's prefill takes under the lever, and
why the default is a per-window scratch panel rather than a stored one, is `ARCHITECTURE_GPU_PREFILL_GEMM.md#devw-panel-knees`.

The bake set is the minting PROGRAM's, not the box's: the split-scale formats bake only where
the Metal prefill module registered its mirror, so a program on the same box whose module graph
lacks that module mints a thinner image at the SAME identity, and every split-scale site it
skipped serves off the runtime dequant until a program carrying the mirror re-mints. The
asymmetry is accepted rather than designed away: a `.dlim` is a box-local cache, so a thinner
image costs a dequant pass and never a different answer.

The k6 dequant writes its value as `dsc * (q - 32)` at every site - the runtime kernel, its
double-buffered twin, and the mint's CPU mirror. The factored spelling costs nothing, since the
compiler factors the distributed form anyway, and it pins the SIGN of a zero result: at q == 32
with a negative scale the distributed form yields +0 and the factored one -0. One spelling
everywhere makes the equality hold by source rather than by toolchain mood.

### The baked tower twin-W plane {#image-tower-twin-plane}

A vision- or audio-tower image carries `wblob`: the tower's block and merger GEMM weights
re-emitted as halfwords in the same element order as the f32 blob, so a site reads its twin at
the f32 offset doubled. The mint emits the twin only for a GEMM whose source tensor is already
f16 or bf16, so the twin holds the file's own values and introduces no rounding; an empty
`wblob` is therefore the ineligibility flag a driver tests, not a separate capability field. A
GEMM whose weights are COMPUTED at load - the folded patch-conv pair of the qwen3v and qwen25v
stems - has no source tensor, no twin, and stays on the f32 route.

The f16 arm needs a tensor crown (`pf_hmm_ready`) and converts X once per encode into one
shared half panel every site reuses; the bf16 arm reads the twin natively and takes that panel
only where its half-X stamp compiled. `DASLLAMA_METAL_TOWER_F16=0` pins the f32 route, and
`metal_tower_f16_encodes()` is the engage witness.

The bake (`tower_bake_half_twin`) is whole-blob: the file's own halves land at the same element
offsets, and the f32 regions that ride along (norms, biases, position tables) clamp on the f16
form - safe because the halfword route never reads them. An f16 source round-trips exactly and a
bf16 source is the f32 word's top halfword, so the twin holds the file's bits, not a re-rounded
copy. A streamed mint writes the same bytes as the eager bake over the same span, so an image
minted either way serves the same twin. The q8 lane never bakes one - it serves quantized planes
instead. `blk_bf16` names which halfword family the route dispatches.

The crown set is read when the prefill PSOs compile, not when a GEMM dispatches, so
`pf_hmm_ready` answers for whatever crowns were armed at the last `metal_prefill_init`. A
caller that arms a crown after bring-up runs `metal_prefill_shutdown` to force the recompile;
without it the f16 arm silently stays unavailable and every encode takes the f32 route.

### A GPU-flavored tower image carries its driver's device block {#tower-dev-block}

Each GPU backend loads its own tower image and converts nothing on load: a tower family's Metal and Vulkan images hold the
bytes that driver's kernels read, and the CPU never runs either. The image carries them as one device block - ``dev``, the
pieces' bytes, and ``devplan``, `TOWER_DEV_PIECE_WORDS` words a piece (the key of its name, its byte offset in ``dev``, its
bytes) - and a ``dev_backend`` meta field (`TowerDev`) naming the driver. A GPU-flavored mint builds the family's CPU form in
memory, runs the driver's bake over it - the conversions the attach ran before: the q8 re-lay or gather, the K-quant device
forms, the padded panels, the f16 halves - into a `TowerDevBuild` (`tower_dev_put`, each piece at `TOWER_DEV_ALIGN`, the image
page, so a mapped piece wraps as a device buffer with no copy), and writes the planes the driver reads as stored beside the
block, the planes the bake consumed dropped. The attach finds each piece by name (`tower_dev_find`) and binds it: zero-copy on
Metal, a byte-for-byte upload on Vulkan.
