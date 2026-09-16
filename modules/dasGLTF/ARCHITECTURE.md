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

## 3. Processed vertex portability {#processed-vertex-portability}

UV packing uses scalar float16 conversion so CPU processing does not depend on
native half-vector casts.

## 4. Compressed texture capability {#compressed-texture-capability}

When the reported compressed-format list omits S3TC formats, upload checks S3TC
extension support. Its sRGB variants also require core or extension sRGB support.
Each mip dimension is block-aligned or one or two pixels; other dimensions cause
CPU decoding and uncompressed upload for the entire texture.
