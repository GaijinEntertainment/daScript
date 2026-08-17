options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 10 pixel-oracle gate. The scene is a brick-floor + concrete-cat statue lit by ONE
// directional sun (PCF-shadowed) + THREE orbiting coloured point lights, composed via deferred. The
// LIT mode (debug_mode=0) is the load-bearing visual; we additionally check the indicator strip
// flips colour with debug_mode (proves the lighting fragment's mode switch + the host's push-
// constant flow), and that the G-buffer ALBEDO mode shows orange bricks where the lit-mode shows
// the floor (proves MRT writes the albedo attachment, not just the final colour).

require dastest/testing_boost public
require deferred_tut
require math

let TEST_TIME : float = 2.0
let TEST_CAM  : float = 0.25

[test]
def test_deferred_oracle(t : T?) {   // nolint:STYLE038 - flat test cell
    var lit_pixels <- render_deferred_cube(TEST_TIME, TEST_CAM, DBG_LIT)

    // ----- top-strip indicator (sub-indicator-strip rows) -----
    // The shader paints rows 0..15 as a flat mode-distinct colour. Lit = warm yellow (~0.95, 0.85, 0.45).
    let cx = DEFERRED_W / 2
    let strip_off = (8 * DEFERRED_W + cx) * 4
    let r0 = int(lit_pixels[strip_off])
    let g0 = int(lit_pixels[strip_off + 1])
    let b0 = int(lit_pixels[strip_off + 2])
    t |> success(r0 > 220 && g0 > 180 && b0 < 210,
        "lit indicator strip is warm yellow (got {r0},{g0},{b0})")

    // ----- background (top-quarter sky) -----
    // Above the cube/floor horizon the lighting shader writes a dark blue->magenta gradient.
    let bg_y = DEFERRED_H / 4
    let bg_off = (bg_y * DEFERRED_W + cx) * 4
    let bg_r = int(lit_pixels[bg_off])
    let bg_g = int(lit_pixels[bg_off + 1])
    let bg_b = int(lit_pixels[bg_off + 2])
    // sky_top = (0.06, 0.08, 0.18) -> roughly (15, 20, 46)
    t |> success(bg_r < 50 && bg_g < 60 && bg_b < 90,
        "top-quarter sky is dark blue (got {bg_r},{bg_g},{bg_b})")

    // ----- centre: the cat should be visible + lit -----
    // A single hardcoded sample is fragile under camera motion + the contact-AO darkening, so scan a
    // box around the centre where the cat sits and require its brightest pixel to be clearly lit.
    var cat_bright = 0
    let cy = DEFERRED_H / 2 + 20
    for (dy in range(-80, 81)) {
        for (dx in range(-70, 71)) {
            let o = ((cy + dy) * DEFERRED_W + (cx + dx)) * 4
            let m = max(int(lit_pixels[o]), max(int(lit_pixels[o + 1]), int(lit_pixels[o + 2])))
            cat_bright = max(cat_bright, m)
        }
    }
    t |> success(cat_bright > 90,
        "cat area has a clearly lit pixel (brightest in centre box = {cat_bright})")

    // ----- floor: scan a row below the cube; expect to see orange-warm bricks somewhere -----
    var floor_warm_hits = 0
    let floor_y = DEFERRED_H - 80
    for (x in range(DEFERRED_W / 8)) {
        let xx = x * 8
        let off = (floor_y * DEFERRED_W + xx) * 4
        let r = int(lit_pixels[off])
        let g = int(lit_pixels[off + 1])
        let b = int(lit_pixels[off + 2])
        // a brick pixel: red > green > blue (warm)
        if (r > 80 && r > g + 5 && g > b) {
            floor_warm_hits++
        }
    }
    t |> success(floor_warm_hits >= 5,
        "floor row at y={floor_y} has {floor_warm_hits} warm brick pixels (>=5)")

    delete lit_pixels

    // ----- albedo mode should also show orange bricks on the floor -----
    var albedo_pixels <- render_deferred_cube(TEST_TIME, TEST_CAM, DBG_ALBEDO)
    var albedo_brick_hits = 0
    for (x in range(DEFERRED_W / 8)) {
        let xx = x * 8
        let off = (floor_y * DEFERRED_W + xx) * 4
        let r = int(albedo_pixels[off])
        let g = int(albedo_pixels[off + 1])
        let b = int(albedo_pixels[off + 2])
        if (r > 80 && r > g + 10 && g > b) {
            albedo_brick_hits++
        }
    }
    t |> success(albedo_brick_hits >= 5,
        "ALBEDO mode floor row at y={floor_y} has {albedo_brick_hits} orange-brick pixels (proves the G-buffer wrote albedo, not just the final-colour)")

    // ALBEDO mode indicator strip should be warm red (0.95, 0.45, 0.45).
    let alb_strip = (8 * DEFERRED_W + cx) * 4
    let ar = int(albedo_pixels[alb_strip])
    let ag = int(albedo_pixels[alb_strip + 1])
    let ab = int(albedo_pixels[alb_strip + 2])
    t |> success(ar > 220 && ag < 210 && ab < 210,
        "ALBEDO indicator strip is warm red (got {ar},{ag},{ab})")
    delete albedo_pixels

    // ----- SSAO mode: indicator strip should be light grey (0.80, 0.80, 0.80) -----
    var ssao_pixels <- render_deferred_cube(TEST_TIME, TEST_CAM, DBG_SSAO)
    let ssao_strip = (8 * DEFERRED_W + cx) * 4
    let sr = int(ssao_pixels[ssao_strip])
    let sg = int(ssao_pixels[ssao_strip + 1])
    let sb = int(ssao_pixels[ssao_strip + 2])
    t |> success(sr > 180 && sg > 180 && sb > 180 && abs(sr - sg) < 15,
        "SSAO indicator strip is grey (got {sr},{sg},{sb})")
    delete ssao_pixels
}
