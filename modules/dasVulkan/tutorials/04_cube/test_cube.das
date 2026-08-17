options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 04 pixel-oracle gate (lavapipe in CI, real GPU locally). The synthwave cube has no
// analytic-symmetry shortcut the way Mandelbrot did; instead the oracle asserts a handful of
// structural properties of the render at a fixed (time, camera_t):
//   - frame corners are the dark clear color (the cube does not fill the screen),
//   - a central pixel is on the cube and is much brighter than the clear color (the cube renders),
//   - the lit region carries the synthwave palette: at least one magenta-dominant pixel (sky band
//     of the procedural texture) and at least one cyan-dominant pixel (perspective grid).
// (time=1.0, camera_t=0.18) places the camera just past the front-right of the lemniscate orbit,
// so the +Z and +X faces are both visible -- the same two faces a viewer sees on the recording's
// opening frames.

require dastest/testing_boost public
require cube_tut

let TEST_TIME : float = 1.0
let TEST_CAM  : float = 0.18

def bg(p : int3) : bool {
    // The clear color is (0.02, 0.0, 0.06) sRGB-encoded to (39, 0, 69). Tight on y: the dark indigo
    // on the texture's ground has nonzero green after sRGB encoding, so y < 4 still distinguishes.
    return p.x < 50 && p.y < 4 && p.z < 80
}

[test]
def test_cube_oracle(t : T?) {
    var pixels <- render_synthwave_cube(TEST_TIME, TEST_CAM)

    // four corners are background -- the cube does not fill the frame
    t |> success(bg(px(pixels, 8, 8)),                       "top-left corner is the clear color")
    t |> success(bg(px(pixels, CUBE_W - 8, 8)),              "top-right corner is the clear color")
    t |> success(bg(px(pixels, 8, CUBE_H - 8)),              "bottom-left corner is the clear color")
    t |> success(bg(px(pixels, CUBE_W - 8, CUBE_H - 8)),     "bottom-right corner is the clear color")

    // the cube projects onto the center: a 200x200 box sampled on a 20x20 grid is mostly NOT background
    var cube_hits = 0
    let cx = CUBE_W / 2
    let cy = CUBE_H / 2
    for (j in range(20)) {
        for (i in range(20)) {
            let dy = -100 + j * 10
            let dx = -100 + i * 10
            if (!bg(px(pixels, cx + dx, cy + dy))) {
                cube_hits++
            }
        }
    }
    t |> success(cube_hits >= 380, "central 200x200 box is dominantly cube, not background ({cube_hits}/400)")

    // the synthwave palette shows up: >= 1 magenta-dominant pixel (texture sky band) and >= 1 cyan-dominant
    // one (perspective grid) inside the cube's silhouette. Thresholds are calibrated for LINEAR sampling +
    // key/rim lighting -- the 1-texel cyan grid lines average ~50% into the dark indigo under LINEAR.
    var saw_magenta = false
    var saw_cyan = false
    for (j in range(50)) {
        for (i in range(50)) {
            let y = cy - 100 + j * 4
            let x = cx - 100 + i * 4
            let p = px(pixels, x, y)
            if (p.x > 140 && p.z > 120 && p.y < 120) {
                saw_magenta = true
            }
            if (p.z > 120 && p.y > 100 && p.x < 40) {
                saw_cyan = true
            }
        }
    }
    t |> success(saw_magenta, "found a magenta pixel inside the cube (synthwave sky band sampled)")
    t |> success(saw_cyan,    "found a cyan pixel inside the cube (perspective-grid sampled)")

    delete pixels
}
