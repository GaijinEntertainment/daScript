options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 03 pixel-oracle gate. The SDF scene is deterministic at a fixed time, so we sample a few
// pixels at known positions and assert palette/structure: sky at the top corners (warm-blue gradient),
// ground at the bottom row (checkered greys with brightness floor from ambient), and a hero blob
// somewhere in the center band that is brighter than the ground average and clearly time-coloured.

require dastest/testing_boost public
require sdf_tut

let TEST_TIME = 1.2f                    // chosen so the scene is mid-orbit and the blob is visible

[test]
def test_sdf(t : T?) {
    var p <- render_sdf(TEST_TIME)

    // ----- corners: top-left and top-right should be sky (always above horizon at this camera)
    let tl = px(p, 4, 4)
    t |> success(tl.x > 60 && tl.y > 100 && tl.z > 140, "top-left is sky (blue-dominant): {tl}")
    let tr = px(p, SDF_DIM - 5, 4)
    t |> success(tr.x > 60 && tr.y > 100 && tr.z > 140, "top-right is sky (blue-dominant): {tr}")

    // ----- bottom row: ground checkerboard. The checkers vary, so just assert it's not black and
    // not saturated -- a real hit shaded with key + ambient lands in the mid-tones.
    var bottom_hits = 0
    for (i in range(12)) {
        let x = i * 64
        let s = px(p, x, SDF_DIM - 8)
        if (s.x > 20 && s.y > 20 && s.z > 20 && s.x < 250 && s.y < 250 && s.z < 250) {
            bottom_hits++
        }
    }
    t |> success(bottom_hits >= 8, "bottom row hits the ground (mid-tone, non-saturated): {bottom_hits}/12")

    // ----- center band: the blob (sphere smin torus) lives near the screen center. Sample a 10x10
    // grid centered at (DIM/2, DIM/2) and count pixels that look like the warm blob palette
    // (red-dominant or yellow-dominant) rather than sky-blue or checker-grey.
    var blob_hits = 0
    let cx = SDF_DIM / 2
    let cy = SDF_DIM / 2
    for (j in range(10)) {
        for (i in range(10)) {
            let s = px(p, cx - 80 + i * 16, cy - 40 + j * 16)
            // blob_color(p, TEST_TIME) = (0.6 + 0.4*cos(0.72), 0.45 + 0.4*cos(1.52), 0.4 + 0.4*cos(2.32))
            // ~= (0.90, 0.47, 0.13) before lift/lighting -- warm/red-dominant; oracle asserts red exceeds blue.
            if (s.x > s.z + 15 && s.x > 80) {
                blob_hits++
            }
        }
    }
    t |> success(blob_hits >= 10, "central band hits the blob (warm-toned hero): {blob_hits}/100")

    delete p
}
