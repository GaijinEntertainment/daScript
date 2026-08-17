options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-7.3 sampler2DArray GPU gate: render a fullscreen quad whose fragment samples a 4-layer 2D array
// texture (layer chosen per screen quadrant, each layer a distinct solid color). Each quadrant center is
// sampled and must equal exactly one of the four layer colors, with all four present -- the real-driver
// proof that the Arrayed image + the layer (.z) coordinate select the right array layer. (Robust to the
// NDC y-flip: we assert the SET of quadrant colors equals the set of layer colors, not a fixed mapping.)

require dastest/testing_boost public
require scene_helpers

[test]
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
