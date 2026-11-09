options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require _scene_helpers
require _storage_image_shaders
require _texelfetch_shaders
require _vk_texture_grad_shaders

// Phase-7.3 sampler2DArray GPU gate: render a fullscreen quad whose fragment samples a 4-layer 2D array
// texture (layer chosen per screen quadrant, each layer a distinct solid color). Each quadrant center is
// sampled and must equal exactly one of the four layer colors, with all four present -- the real-driver
// proof that the Arrayed image + the layer (.z) coordinate select the right array layer. (Robust to the
// NDC y-flip: we assert the SET of quadrant colors equals the set of layer colors, not a fixed mapping.)


[test(description = "a sampler2DArray fragment selects the right layer, so the four quadrant colors equal the four layer colors", timeout = 1200)]
def test_sampler2darray(t : T?) {
    var pixels <- render_sampler2darray()
    let centers = fixed_array(px(pixels, 64, 64), px(pixels, 192, 64), px(pixels, 64, 192), px(pixels, 192, 192))
    let expected = fixed_array(int3(255, 0, 0), int3(0, 255, 0), int3(0, 0, 255), int3(255, 255, 0))
    for (e in expected) {
        var hits = 0
        for (c in centers) {
            if (c.x == e.x && c.y == e.y && c.z == e.z) {
                hits++
            }
        }
        t |> success(hits == 1, "layer color {e} sampled in exactly one quadrant (got {hits})")
    }
    delete pixels
}

[test(description = "a separate texture2D and sampler combine into the four texel colors, one per quadrant", timeout = 1200)]
def test_separate_image_sampler(t : T?) {
    var pixels <- render_separate_image_sampler()
    let centers = fixed_array(px(pixels, 64, 64), px(pixels, 192, 64), px(pixels, 64, 192), px(pixels, 192, 192))
    let expected = fixed_array(int3(255, 0, 0), int3(0, 255, 0), int3(0, 0, 255), int3(255, 255, 0))
    for (e in expected) {
        var hits = 0
        for (c in centers) {
            if (c.x == e.x && c.y == e.y && c.z == e.z) {
                hits++
            }
        }
        t |> success(hits == 1, "texel color {e} sampled in exactly one quadrant (got {hits})")
    }
    delete pixels
}

[test(description = "a compute shader writes a coordinate gradient into a storage image and every pixel reads back exact", timeout = 1200)]
def test_storage_image_gradient(t : T?) {
    var pixels <- compute_image_rgba8(grad_spv, 64, 64)
    assert_pixels_exact(t, pixels, 64, 64, "storage-image gradient") $(x, y : int) {
        return int4(x, y, 0, 255)
    }
    delete pixels
}

[test(description = "texelFetch copies a 64x64 source texture into a storage image with every texel exact", timeout = 1200)]
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

[test(description = "textureGrad with explicit dx and dy selects the expected mip level in each band", timeout = 1200)]
def test_texture_grad_mips(t : T?) {
    // Constant coordinates have no implicit derivatives. Each band
    // must still select its mip;
    // the top half tests dx alone, the bottom half dy alone.
    let colors = fixed_array(int4(255, 0, 0, 255), int4(0, 255, 0, 255), int4(0, 0, 255,
        255), int4(255))
    var src : array<uint8>
    src |> reserve((64 * 64 + 32 * 32 + 16 * 16 + 8 * 8) * 4)
    for (level in range(4)) {
        let size = 64 >> level
        for (_ in range(size * size)) {
            for (channel in range(4)) { src |> push(uint8(colors[level][channel])) }
        }
    }
    var pixels <- texelfetch_copy_rgba8(grad_copy_spv, src, 64, 64, 4u)
    assert_pixels_exact(t, pixels, 64, 64, "textureGrad mip selection, dx and dy") $(x, _y : int) {
        return colors[x / 16]
    }
    delete pixels
    delete src
}
