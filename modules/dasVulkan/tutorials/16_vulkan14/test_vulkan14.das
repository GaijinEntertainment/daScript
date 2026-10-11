options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 16 pixel-oracle gate. One frame at time 0 proves the three 1.4 rails end to end: the three
// cubes show up where the camera puts them, each in the hue of the texture host image copy uploaded
// (red checker, green stripes, blue dots), the lighting pass that read them back through local read lit
// them (a face toward the light is brighter than one away from it), and the sky between them is the
// lighting pass's gradient, not the cleared black of attachment 2. Soft-skips where the instance or
// the device is below 1.4 - lavapipe before Mesa 25.0.

require dastest/testing_boost public
require vulkan14_tut

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

    // the three cubes sit left, center, right at time 0 (camera on +Z looking at the origin)
    let left = px(p, 190, 265)
    let mid = px(p, 384, 265)
    let right = px(p, 578, 265)
    t |> success(left.x > left.y + 40 && left.x > left.z + 40, "left cube carries the red checker texture")
    t |> success(mid.y > mid.x + 30 && mid.y > mid.z + 30, "center cube carries the green stripes texture")
    t |> success(right.z > right.x + 30 && right.z > right.y - 10, "right cube carries the blue dots texture")

    // lighting read the normal attachment: the top face (toward the light) is brighter than a side face
    let top_face = px(p, 384, 190)
    let side_face = px(p, 384, 300)
    t |> success(top_face.x + top_face.y + top_face.z > side_face.x + side_face.y + side_face.z,
        "the face toward the light is lit brighter than the one facing away")
    delete p
}
