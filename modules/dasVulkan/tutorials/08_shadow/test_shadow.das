options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require shadow_tut public
require math

//! Sample the RGB of pixel (x, y) from a SCENE_W*SCENE_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * SCENE_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}

def luminance(rgb : int3) : int {
    return (rgb.x + rgb.y + rgb.z) / 3
}

[test]
def test_shadow_oracle(t : T?) {
    t |> run("shadow_tut: shadow visible on floor + cube lit, sky clear") <| @(t : T?) {
        var pixels <- render_shadow_scene(0.0f, 0.5f)

        // Sky pixel (top-left corner) — clear color (10/10/20 dark blue).
        let sky = px(pixels, 16, 16)
        t |> success(luminance(sky) < 80, "sky is dark clear color")

        // Find a SHADOWED floor point. At time=0 (sun_yaw = 0) the sun is roughly along +x, so the
        // shadow falls toward -x of the cube, and the floor covers a large area near the frame bottom.
        // Sample around where the shadow is expected and require at least one dark hit.
        var any_shadow = false
        var min_floor = 255
        var max_floor = 0
        for (j in range(int(float(SCENE_H) * 0.62), int(float(SCENE_H) * 0.85))) {
            for (i in range(SCENE_W / 4, 3 * SCENE_W / 4)) {
                let p = px(pixels, i, j)
                let l = luminance(p)
                // Skip the cube (very saturated) by demanding ~grey-ish.
                if (abs(p.x - p.y) < 60 && abs(p.y - p.z) < 60) {
                    if (l < min_floor) { min_floor = l }
                    if (l > max_floor) { max_floor = l }
                    if (l < 140) { any_shadow = true }
                }
            }
        }
        t |> success(any_shadow, "found at least one shadowed floor pixel (luminance < 140)")
        t |> success(max_floor > 100, "found at least one lit floor pixel (luminance > 100, max = {max_floor})")
        t |> success(max_floor - min_floor > 40, "floor has shadow contrast: max {max_floor} - min {min_floor} > 40")

        delete pixels
    }
}
