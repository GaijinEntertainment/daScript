options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-7.2 texelFetch GPU gate: upload a known 64x64 source texture (texel (x,y) = (x,y,0,255)), run a
// daslang [compute_shader] that texelFetch-copies it into a storage image, read the storage image back,
// and assert EVERY pixel equals the source. Exact-analytic proof that texelFetch reads the right texel by
// integer coordinate on a real driver -- spirv-val only confirms the SPIR-V is valid, not that it samples
// the intended texel.

require dastest/testing_boost public
require scene_helpers
require texelfetch_shaders

[test]
def test_texelfetch_copy(t : T?) {
    let w = 64
    let h = 64
    var src : array<uint8>
    src |> resize(w * h * 4)
    for (y in range(h)) {
        for (x in range(w)) {
            let p = (y * w + x) * 4
            src[p] = uint8(x)
            src[p + 1] = uint8(y)
            src[p + 2] = uint8(0)
            src[p + 3] = uint8(255)
        }
    }
    var pixels <- texelfetch_copy_rgba8(fetch_copy_spv, src, w, h)
    assert_pixels_exact(t, pixels, w, h, "texelFetch copy") $(x, y : int) {
        return int4(x, y, 0, 255)
    }
    delete pixels
    delete src
}
