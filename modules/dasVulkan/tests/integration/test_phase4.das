options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-4 GPU gate: the real-driver proof of the dasSpirv Phase-4 emitter surface (UBO matrix +
// combined image sampler + push constant). render_textured_quad draws a 2x2 checkerboard on a quad
// transformed by a column-major MVP (scale 0.6, translate x +0.4) and tinted by a push constant
// (1,1,0 -> blue zeroed). The MVP and tint are uploaded by the host; only a correct render can
// confirm the column-major matrix layout, the sampler UV mapping, and the push-constant delivery,
// which spirv-val cannot. Quad screen extent is the half-open x in [102,256), y in [51,205) (right
// and bottom edges exclusive; pixel indices run 0..255); quadrant centers at u/v < 0.5 (top-left)
// and > 0.5 (bottom-right).

require dastest/testing_boost public
require scene_helpers

[test]
def test_phase4_textured_quad(t : T?) {
    var pixels <- render_textured_quad()
    let outside = px(pixels, 70, 128)   // left of the right-shifted quad -> clear color
    let red_q   = px(pixels, 140, 90)   // u<0.5, v<0.5 -> texel (0,0) red
    let green_q = px(pixels, 230, 90)   // u>0.5, v<0.5 -> texel (1,0) green (also: quad reaches right)
    let blue_q  = px(pixels, 140, 170)  // u<0.5, v>0.5 -> texel (0,1) blue, tinted black
    let white_q = px(pixels, 230, 170)  // u>0.5, v>0.5 -> texel (1,1) white, tinted yellow

    // MVP translate (column 3) applied: the quad's left edge moved right, so (70,128) is clear.
    t |> success(outside.z > outside.x && outside.x < 80 && outside.y < 80 && outside.z < 90,
        "outside the right-shifted quad is the clear color (MVP translate applied)")
    // sampler reads four distinct texels at four screen positions (UV mapping correct).
    t |> success(red_q.x > 150 && red_q.y < 80 && red_q.z < 80, "top-left quadrant samples the red texel")
    t |> success(green_q.y > 150 && green_q.x < 80 && green_q.z < 80, "top-right quadrant samples the green texel")
    t |> success(blue_q.x < 60 && blue_q.y < 60 && blue_q.z < 60, "bottom-left (blue) texel tinted to black (blue zeroed)")
    // push constant delivered: the white texel loses its blue channel -> yellow.
    t |> success(white_q.x > 150 && white_q.y > 150 && white_q.z < 80, "bottom-right (white) texel tinted yellow (push-constant tint applied)")
    delete pixels
}
