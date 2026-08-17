options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 09 pixel-oracle gate (lavapipe in CI, real GPU locally). The scene is a procedural spiky
// ball (a smooth sphere studded with ~150 thin Fibonacci-distributed spikes) — a dense, high-frequency
// silhouette where MSAA's value is dramatic (it was subtle on the old cube). The load-bearing test
// renders the SAME frame with 4x MSAA and with 1x rasterization and proves MSAA actually antialiases:
// the 1x image leaves the thin spikes peppered with aliased partial-coverage speckle that 4x MSAA's
// per-pixel averaging smooths away, so the 1x image has far more faint near-background pixels.

require dastest/testing_boost public
require msaa_tut

let TEST_TIME : float = 1.0
let TEST_CAM  : float = 0.18

// sRGB-encoded clear colour: linear (0.02, 0.0, 0.06) → sRGB ≈ (39, 0, 69) per hw encoder.
// The thresholds below are tuned to accept this colour and reject anything brighter/different.
def bg(p : int3) : bool {
    return p.x < 50 && p.y < 5 && p.z < 80
}

//! Faint near-background pixels: above the clear colour (bg.z ~69) but in the aliasing-speckle band.
//! In sRGB space the speckle from thin-spike partial coverage lands in [74,120]; 1x has far more
//! of these than 4x MSAA (which blends them away). Range tuned after sRGB format change (was [20,55]).
def count_faint(pixels : array<uint8>) : int {
    var n = 0
    for (y in range(MSAA_H)) {
        for (x in range(MSAA_W)) {
            let z = px(pixels, x, y).z
            if (z >= 74 && z <= 120) {
                n ++
            }
        }
    }
    return n
}

[test]
def test_msaa_oracle(t : T?) {
    var inscope ctx <- build_msaa_context()
    var on  <- render_msaa_frame(ctx, TEST_TIME, TEST_CAM, true)
    var off <- render_msaa_frame(ctx, TEST_TIME, TEST_CAM, false)

    // four corners are the clear color -- the ball does not fill the frame. Top corners are sampled
    // BELOW the 16-row mode-indicator strip (24px margin) so the indicator's colour doesn't trip bg().
    t |> success(bg(px(on, 8, 24)),                  "top-left corner is the clear color")
    t |> success(bg(px(on, MSAA_W - 8, 24)),         "top-right corner is the clear color")
    t |> success(bg(px(on, 8, MSAA_H - 8)),          "bottom-left corner is the clear color")
    t |> success(bg(px(on, MSAA_W - 8, MSAA_H - 8)), "bottom-right corner is the clear color")

    // central box is dominantly the ball (its solid core fills the centre)
    var ball_hits = 0
    let cx = MSAA_W / 2
    let cy = MSAA_H / 2
    for (j in range(20)) {
        for (i in range(20)) {
            if (!bg(px(on, cx - 100 + i * 10, cy - 100 + j * 10))) {
                ball_hits ++
            }
        }
    }
    t |> success(ball_hits >= 360, "central 200x200 box is dominantly the ball ({ball_hits}/400)")

    // synthwave palette from the procedural texture wrapped on the core (magenta sky, cyan grid)
    var saw_magenta = false
    var saw_cyan = false
    for (j in range(60)) {
        for (i in range(60)) {
            let p = px(on, cx - 120 + i * 4, cy - 120 + j * 4)
            if (p.x > 140 && p.z > 120 && p.y < 120) {
                saw_magenta = true
            }
            if (p.z > 120 && p.y > 100 && p.x < 40) {
                saw_cyan = true
            }
        }
    }
    t |> success(saw_magenta, "found a magenta pixel on the ball (synthwave sky band sampled)")
    t |> success(saw_cyan,    "found a cyan pixel on the ball (perspective-grid sampled)")

    // THE MSAA assertion: 1x rasterization speckles the thin spikes with aliased partial-coverage
    // pixels; 4x MSAA averages them away. So the 1x image has far more faint near-background pixels
    // (measured ~1.9x). A broken/no-op MSAA path would leave the two equal and fail this.
    let faint_on = count_faint(on)
    let faint_off = count_faint(off)
    t |> success(faint_off > faint_on * 3 / 2,
        "4x MSAA antialiases the spikes: 1x speckle {faint_off} >> 4x {faint_on}")

    delete on
    delete off
}
