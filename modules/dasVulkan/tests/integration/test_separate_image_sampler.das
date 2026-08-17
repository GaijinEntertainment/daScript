options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-7.4 separate image + sampler GPU gate: render a fullscreen quad whose fragment samples a
// standalone texture2D (binding 0) combined with a standalone sampler (binding 1) via sampleTexture.
// The texture is a 2x2 checkerboard, so each screen quadrant samples one texel; we assert the four
// quadrant centers equal the four texel colors (as a set, robust to the NDC y-flip) -- proving the
// split image/sampler descriptors bind and combine (OpSampledImage) correctly on a real driver.

require dastest/testing_boost public
require scene_helpers

[test]
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
