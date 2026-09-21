# dasGLTF architecture

## 1. Atmospheric transmittance {#atmosphere-transmittance}

The height-density integral uses a primitive continuous at the layer top, where
constant density meets exponential falloff. Rays crossing that boundary therefore
retain continuous transmittance.

## 2. Directional shadow filtering {#directional-shadow-filtering}

Directional shadow gradients are evaluated before divergent bounds checks. Shadow
coordinates reconstruct the geometric receiver plane independently of shading normals.
Filtering interpolates depth-comparison results, rather than raw depths: a 4-by-4
nearest-sample footprint combines the weights of a 3-by-3 bilinear comparison kernel.

Shadow declarations, receiver correction and directional/local filtering live in
`gltf_shadows.das`, re-exported by `gltf_pbr`. The default remains weighted PCF.
`GltfPbrRenderer.shadowFilter = 1` selects seven hardware bilinear comparison
queries: four diagonal edge estimates, the center, and two edge-directed samples.
The result weights center by one half and each directional sample by one quarter,
preserving fully lit and fully shadowed regions.

Comparison samplers use texture units 10 and 11, separately from raw depth units
6 and 9. Each renderer owns a comparison sampler and a one-pixel lit fallback;
these keep sampler types valid when a shadow source is disabled. The renderer owns
sampler bindings on those two units during its draw and clears them afterward.
Local samples clamp within each atlas face before atlas-coordinate conversion.

## 3. Processed vertex portability {#processed-vertex-portability}

UV packing uses scalar float16 conversion so CPU processing does not depend on
native half-vector casts.

## 4. Compressed texture capability {#compressed-texture-capability}

When the reported compressed-format list omits S3TC formats, upload checks S3TC
extension support. Its sRGB variants also require core or extension sRGB support.
Each mip dimension is block-aligned or one or two pixels; other dimensions cause
CPU decoding and uncompressed upload for the entire texture.

## 5. Compact skin influences {#compact-skin-influences}

Processed format version 2 stores four unsigned-byte joint indices and four
normalized unsigned-byte weights in eight bytes. Inputs are normalized and sorted
by descending weight with their indices. Secondary weights are rounded, the first
weight receives the integer remainder to 255, and the quantized pairs are sorted
again. This preserves a sum of 255 and a trailing run of zero weights even at ties.
Unsupported indices above 255 and malformed influences are rejected explicitly.
The processed format assumes little-endian hosts, like its existing packed float and
direction fields.

The GL adapter uses unsigned-byte attributes (normalized for weights only). The
ordinary float upload path also canonicalizes influence order. The shared GL vertex
stage reconstructs the first weight as one minus the other three and skips trailing
zero influences with nested branches. Bone matrices retain their existing precision
and the renderer's existing palette-size limit. Other backends' float skinning math
is unchanged. Old processed files must be regenerated rather than reinterpreted.

## Optional surface motion output

`gltf_motion.das` captures a compact previous rendered pose (node matrices and skin
palettes), independent of mesh/material ownership. `gltf_pbr_render_normals` can
optionally write a third MRT attachment containing unjittered previous-minus-current
UV motion, suitable for RG16F. `motion_kind` selects static/camera reconstruction
(sentinel 4), valid dynamic pose (1), or invalid history (sentinel 2). The caller owns
history lifetime, jitter convention, framebuffer setup and capability checks. Missing
node/skin history and incomplete palettes emit the invalid-history sentinel per node.

The motion vertex variant reuses current-pose skinning and adds previous-pose
skinning with the same byte-weight reconstruction. Ordinary surface/depth/colour
variants have no previous-pose uniforms or binding work. Optional shader creation is
lazy. Retain at least 544 vertex-uniform vectors for the two 64-joint palettes and
remaining transforms. Tests cover rigid/root and skeletal pose independence as well
as the existing packed-skin attribute contracts.
