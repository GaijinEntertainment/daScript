options gen2
require dastest/testing_boost public
require scene_helpers
require texture_grad_shaders

[test]
def test_texture_grad_mips(t : T?) {
    // Constant coordinates have no implicit derivatives. Each band must still select its mip;
    // the top half tests dx alone, the bottom half dy alone.
    let colors = fixed_array(int4(255, 0, 0, 255), int4(0, 255, 0, 255), int4(0, 0, 255, 255), int4(255))
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
