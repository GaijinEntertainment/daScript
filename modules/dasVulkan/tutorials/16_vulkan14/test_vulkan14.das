options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 16 pixel-oracle gate. One frame at time 0 proves the three 1.4 rails end to end: the three
// cubes show up where the camera puts them, each in the hue of the texture host image copy uploaded
// (red checker, green stripes, blue dots), the lighting pass that read them back through local read lit
// them (the top face under the light is brighter than a side face turned away from it), and the sky at
// the frame's edge is the lighting pass's gradient, not the cleared black of attachment 2. Soft-skips
// where the instance or the device is below 1.4 - lavapipe before Mesa 25.0.

require dastest/testing_boost public
require vulkan14_tut

//! Mean RGB of the 16x16 block at (x0, y0): a block average reads a texture's hue whichever texel the
//! sample would hit, so the oracle does not depend on landing on a stripe or a dot.
def avg_px(pixels : array<uint8>; x0, y0 : int) : int3 {
    let side = 16
    var sum = int3(0, 0, 0)
    for (y in range(y0, y0 + side)) {
        for (x in range(x0, x0 + side)) {
            sum += px(pixels, x, y)
        }
    }
    return sum / (side * side)
}

[test]
def test_vulkan14_scene(t : T?) {
    if (!vulkan14_available()) {
        feint("Vulkan 1.4 with pushDescriptor + hostImageCopy + dynamicRenderingLocalRead not available; skipping (lavapipe reports 1.4 from Mesa 25.0)\n")
        return
    }
    var p <- render_vulkan14_scene(0.0f)
    t |> success(length(p) == V14_W * V14_H * 4, "one RGBA8 frame came back")

    // sky: the lighting pass wrote its gradient where no cube is - lighter at the top than the bottom
    let sky_top = px(p, 20, 10)
    let sky_bottom = px(p, 20, V14_H - 10)
    t |> success(sky_top.z > sky_top.x && sky_top.z > 40, "sky is the lighting pass's blue gradient, not the cleared black")
    t |> success(sky_top.z > sky_bottom.z, "sky is brighter at the top")

    // the three cubes sit left, center, right at time 0 (camera on +Z looking at the origin); each block
    // sits on a front face, and the block mean carries the texture's hue
    let left = avg_px(p, 180, 257)
    let mid = avg_px(p, 384, 257)
    let right = avg_px(p, 570, 257)
    t |> success(left.x > left.y + 30 && left.x > left.z + 30, "left cube carries the red checker texture")
    t |> success(mid.y > mid.x + 30 && mid.y > mid.z + 30, "center cube carries the green stripes texture")
    t |> success(right.z > right.x + 20 && right.z > right.y + 20, "right cube carries the blue dots texture")

    // lighting read the normal attachment: on the right cube, the top face under the light is brighter
    // than the -X face the time-0 light direction leaves in ambient
    let top_face = avg_px(p, 592, 195)
    let away_face = avg_px(p, 508, 245)
    t |> success(top_face.x + top_face.y + top_face.z > away_face.x + away_face.y + away_face.z + 100,
        "the face under the light is lit brighter than the one turned away from it")
    delete p
}
