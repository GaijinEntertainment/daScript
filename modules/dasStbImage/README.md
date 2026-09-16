# dasStbImage

The existing image loading, saving and resizing bindings also include `stb_dxt.h`
v1.12. The upstream implementation is vendored unchanged in `src/stb_dxt.h` and
uses its bundled public-domain / MIT licensing options.

`stb_compress_blocks(pixels, width, height, format, high_quality, output)` accepts
RGBA8 pixels and emits one mip level, with edge replication for partial blocks.
Formats: 0 BC1, 1 BC3, 2 BC4 (R), 3 BC5 (RG). Buffer and dimension validation happens
in the native adapter. The compression algorithm is upstream stb; image policy is
written in daslang.

`require stbimage/texture_blocks` provides complete mip chains, colour-space-aware
resizing, validation, content-addressed disk caching and CPU decode fallbacks.
Colour textures use sRGB mip generation; data textures use linear mip generation.
Cache keys include image content, dimensions, format, colour space and codec-layer
version. Cache files are written through temporary files and renamed on completion.

The glTF consumer defaults to BC1 for opaque image data and BC3 when image alpha
is present. BC4/BC5 are available for explicit callers; normal-map-specific RG
reconstruction is not implied by this low-level API.

Tests: `tests/test_texture_blocks.das` covers mip chains, odd dimensions, cache
reuse/invalidation and alpha preservation.
