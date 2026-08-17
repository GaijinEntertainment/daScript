options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 06 pixel oracle (lavapipe in CI, real GPU locally). A chrome/metal sphere sits on a
// checkerboard floor and reflects a procedural cubemap sky (zenith blue + warm horizon + sun glow +
// dark ground). At (camera_t=0.75, time=0) the camera is elevated at (0, 2.2, -4.5) looking at the
// sphere, with the sun glow in the upper-left sky. The oracle asserts the scene's structure:
//
//   - the frame is not all-clear (something rendered);
//   - a bright sun-glow pixel exists in the sky (the cubemap reached the rasteriser);
//   - the reflective sphere is in the centre (its upper cap reflects the saturated blue zenith, its
//     lower cap reflects the dark ground);
//   - the checkerboard floor shows both light and dark tiles.

require dastest/testing_boost public
require skybox_tut

let TEST_TIME : float = 0.0
let TEST_CAM  : float = 0.75        // camera elevated at (0, 2.2, -4.5), looking at the sphere

[test]
def test_skybox_oracle(t : T?) {
    var pixels <- render_skybox(TEST_TIME, TEST_CAM)

    // 1. not all clear
    var sum_lum = 0
    let stride = 16
    for (j in range(SKY_H / stride)) {
        for (i in range(SKY_W / stride)) {
            let p = px(pixels, i * stride, j * stride)
            sum_lum += p.x + p.y + p.z
        }
    }
    let n = (SKY_W / stride) * (SKY_H / stride)
    let mean_lum = sum_lum / (n * 3)
    t |> success(mean_lum > 50, "mean luminance > 50 (scene rendered, got {mean_lum})")

    // 2. bright sun-glow pixel (near-white) somewhere in the top half of the sky
    var saw_sun = false
    for (j in range(48)) {
        for (i in range(96)) {
            let p = px(pixels, i * (SKY_W / 96), j * (SKY_H / 96))
            if (p.x + p.y + p.z > 640 && p.x > 200 && p.y > 200) {
                saw_sun = true
            }
        }
    }
    t |> success(saw_sun, "found a bright sun-glow pixel in the sky")

    // 3. reflective sphere in the centre: a saturated-blue pixel (upper cap reflects the zenith, more
    //    saturated than the pale sky) AND a near-black pixel (lower cap reflects the dark ground).
    var saw_sphere_blue = false
    var saw_sphere_dark = false
    let sy0 = SKY_H * 3 / 10
    let sx0 = SKY_W * 38 / 100
    for (jj in range(SKY_H * 3 / 10)) {            // scans rows sy0 .. sy0 + 30%H (the sphere band)
        let j = sy0 + jj
        for (ii in range(SKY_W * 24 / 100)) {      // cols sx0 .. sx0 + 24%W (the central column)
            let p = px(pixels, sx0 + ii, j)
            if (p.z > p.x + 35 && p.z > 120) { saw_sphere_blue = true }
            if (p.x + p.y + p.z < 90) { saw_sphere_dark = true }
        }
    }
    t |> success(saw_sphere_blue && saw_sphere_dark,
                 "reflective sphere present (blue zenith + dark ground reflection in centre)")

    // 4. checkerboard floor: the bottom third must show both a light-tile and a dark-tile pixel. The sRGB
    // format gamma-encodes the linear output, brightening both (light ~620-635, dark ~370-400; linear was
    // ~430+ and ~60-240), but the clusters stay ~230 lum apart, so contrast detection is still valid.
    var saw_light_tile = false
    var saw_dark_tile = false
    let fy0 = SKY_H * 7 / 10
    for (jj in range((SKY_H - fy0) / 8)) {
        let j = fy0 + jj * 8
        for (ii in range(SKY_W / 8)) {
            let lum = get_lum(px(pixels, ii * 8, j))
            if (lum > 560) { saw_light_tile = true }
            if (lum > 200 && lum < 460) { saw_dark_tile = true }
        }
    }
    t |> success(saw_light_tile && saw_dark_tile, "checkerboard floor present (light + dark tiles)")

    delete pixels
}

def get_lum(p : int3) : int {
    return p.x + p.y + p.z
}
