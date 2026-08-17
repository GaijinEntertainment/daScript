options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 05 pixel oracle (lavapipe in CI, real GPU locally). 1000 instanced cubes scattered on a
// Lissajous-style 3D curve scatter across the frame with the camera at distance 14, so the central
// band fills with cube hits while the corners stay clear-colour. The palette cycles three full hue
// rotations across the swarm via cos-triplet colour, so the cube pixels span the whole RGB cube --
// the test asserts a warm-red pixel and a cool-blue pixel both exist somewhere in the central band.

require dastest/testing_boost public
require instancing_tut

let TEST_TIME : float = 1.0
let TEST_CAM  : float = 0.18         // camera off the +X axis, swarm rotates ~65 degrees

def bg(p : int3) : bool {
    // clear colour is (0.02, 0.0, 0.04) linear, sRGB-encoded to (39, 0, 56). A dark rim contribution
    // can lift b a few counts, so be loose on b and strict on r/g.
    return p.x < 55 && p.y < 4 && p.z < 72
}

[test]
def test_instancing_oracle(t : T?) {
    var pixels <- render_instance_swarm(TEST_TIME, TEST_CAM)

    // four corners are background -- the swarm sits in the central volume, not edge-to-edge
    t |> success(bg(px(pixels, 8, 8)),                              "top-left corner is the clear colour")
    t |> success(bg(px(pixels, INSTANCE_W - 8, 8)),                 "top-right corner is the clear colour")
    t |> success(bg(px(pixels, 8, INSTANCE_H - 8)),                 "bottom-left corner is the clear colour")
    t |> success(bg(px(pixels, INSTANCE_W - 8, INSTANCE_H - 8)),    "bottom-right corner is the clear colour")

    // the central 400x400 sampled on a 40x40 grid is mostly cube hits (instance rendering worked)
    var hits = 0
    let cx = INSTANCE_W / 2
    let cy = INSTANCE_H / 2
    for (j in range(40)) {
        for (i in range(40)) {
            let dy = -200 + j * 10
            let dx = -200 + i * 10
            if (!bg(px(pixels, cx + dx, cy + dy))) {
                hits++
            }
        }
    }
    t |> success(hits >= 400, "central 400x400 box is at least a quarter cubes ({hits}/1600)")

    // palette: cycling the hue three times across the swarm puts both warm-red-leading and
    // cool-blue-leading pixels into the visible swarm. Cube-shaded so red-leading needs a strong
    // r, weak g, moderate b; blue-leading needs strong b, moderate g, weak r.
    var saw_red = false
    var saw_blue = false
    for (j in range(60)) {
        for (i in range(60)) {
            let y = cy - 200 + j * 6
            let x = cx - 200 + i * 6
            let p = px(pixels, x, y)
            if (p.x > 130 && p.y < 90 && p.z < 110) {
                saw_red = true
            }
            if (p.z > 130 && p.x < 110 && p.y < 130) {
                saw_blue = true
            }
        }
    }
    t |> success(saw_red,  "found a red-leading instance in the swarm (hue rotation present)")
    t |> success(saw_blue, "found a blue-leading instance in the swarm (hue rotation present)")

    delete pixels
}
