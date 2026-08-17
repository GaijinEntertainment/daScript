options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 11 pixel oracle (lavapipe in CI, real GPU locally). HDR scene + Karis-style 5-level
// bloom pyramid + ACES tonemap. ~1 in 7 of the 1000 cubes is emissive (HDR colour * 8); after
// tonemapping, the emissive cubes should be near-saturated in their dominant channel AND project a
// soft halo on the surrounding pixels via the bloom additive blend.
//
// We assert three independent things:
//   1) The corners are still background colour (no global lift -- the bloom mip chain doesn't
//      smear bright energy into the dark margins outside ~50 texels).
//   2) The frame contains at least one near-saturated emissive cube (>= 240 on the dominant
//      channel) -- the HDR+ACES pipeline made it through without an early clamp.
//   3) Bloom is visibly producing a halo: of 1024 sampled points across the central 480x480 band,
//      at least 50 (~5%) land in the "bright/halo" luma bucket (80..200). A non-bloom render of
//      the same scene wouldn't produce that count -- emissive cubes alone cover <0.5% of the frame
//      even at 8x boost, so the surplus is the bloom halo dilation.

require dastest/testing_boost public
require hdr_tut
require math

let TEST_TIME : float = 1.0
let TEST_CAM  : float = 0.18

def bg(p : int3) : bool {
    // clear is (0.02, 0.0, 0.04) before tonemap. After ACES + gamma 1/2.2 lands roughly
    // (33, 0, 47) in 8-bit; keep the threshold generous so a stray rim-glow doesn't fail it.
    return p.x < 60 && p.y < 30 && p.z < 70
}

def luma(p : int3) : int => (p.x * 77 + p.y * 150 + p.z * 29) / 256   // BT.601-ish, fast integer

[test]
def test_hdr_oracle(t : T?) {
    var pixels <- render_hdr_scene(TEST_TIME, TEST_CAM)

    // (1) corners are background
    t |> success(bg(px(pixels, 8, 8)),                  "top-left corner is background")
    t |> success(bg(px(pixels, HDR_W - 8, 8)),          "top-right corner is background")
    t |> success(bg(px(pixels, 8, HDR_H - 8)),          "bottom-left corner is background")
    t |> success(bg(px(pixels, HDR_W - 8, HDR_H - 8)),  "bottom-right corner is background")

    // (2) at least one near-saturated emissive cube somewhere in the central band
    var max_l = 0
    var n_saturated = 0
    let cx = HDR_W / 2
    let cy = HDR_H / 2
    for (j in range(160)) {
        for (i in range(160)) {
            let y = cy - 240 + j * 3
            let x = cx - 240 + i * 3
            let p = px(pixels, x, y)
            let l = luma(p)
            if (l > max_l) {
                max_l = l
            }
            if (max(max(p.x, p.y), p.z) >= 240) {
                n_saturated++
            }
        }
    }
    t |> success(max_l >= 200,           "brightest sampled pixel is very bright ({max_l} >= 200, emissive cube + tonemap)")
    t |> success(n_saturated >= 4,       "at least 4 near-saturated samples (emissive cubes survived ACES)")

    // (3) bloom halo histogram: 1024 sample positions across the central 480x480 region, counting
    // "bright" pixels (luma 80..200) -- the halo around emissive cubes. Without bloom that count comes
    // from lit non-emissive cubes alone; bloom's halo dilates it above scene-only lighting.
    var n_bright = 0
    var n_total = 0
    for (j in range(32)) {
        for (i in range(32)) {
            let y = cy - 240 + j * 15
            let x = cx - 240 + i * 15
            let p = px(pixels, x, y)
            let l = luma(p)
            if (l >= 80 && l < 200) {
                n_bright++
            }
            n_total++
        }
    }
    let pct = (n_bright * 100) / max(n_total, 1)
    t |> success(n_bright >= 50,         "at least 50 bright/halo samples ({n_bright}/{n_total} ~= {pct}% in central band)")

    delete pixels
}
