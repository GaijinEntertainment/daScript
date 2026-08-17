options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 07 pixel oracle (lavapipe in CI, real GPU locally). 1024 particles are seeded on a
// tilted ring of radius ~1.2 with tangential velocity, then one compute dispatch advances the
// integrator by dt=1/30 before the graphics pass rasterises them as PARTICLE_PX-wide POINT_LIST
// splats via gl_PointSize (colour-by-speed palette). The oracle asserts:
//
//   - the compute->graphics handoff actually moved data through (many particles visible);
//   - the palette covers both ends (a teal-leading and a magenta-leading pixel both exist).

require dastest/testing_boost public
require particles_tut

let TEST_TIME : float = 1.0
let TEST_CAM  : float = 0.07         // camera off-axis enough to see the ring tilt

def bg(p : int3) : bool {
    // clear colour is (0.01, 0.0, 0.03) linear; sRGB-encoded to approx (25, 0, 48).
    return p.x < 35 && p.y < 5 && p.z < 60
}

[test]
def test_particles_oracle(t : T?) {
    var pixels <- render_particle_swarm(TEST_TIME, TEST_CAM)

    // Walk the full frame and count non-bg pixels; with PARTICLE_PX=10 each splat covers ~78
    // pixels (pi * 5^2), so 1024 particles minus overlap/edge-clip land around ~25k pixels.
    var non_bg = 0
    var saw_teal = false
    var saw_magenta = false
    var max_b = 0
    var max_r = 0
    for (y in range(PART_H)) {
        for (x in range(PART_W)) {
            let p = px(pixels, x, y)
            if (bg(p)) {
                continue
            }
            non_bg++
            if (p.z > max_b) {
                max_b = p.z
            }
            if (p.x > max_r) {
                max_r = p.x
            }
            // sRGB target: gamma-encoding compresses the blue/red gap relative to linear. Empirically
            // the teal palette (cool = float3(0.20, 0.70, 1.00)) yields blue > red by 10+ at brightness
            // 60+, so requiring a gap >= 10 confirms a genuinely teal-hued particle was rasterised.
            if (p.z > 60 && p.z > p.x + 10) {
                saw_teal = true
            }
            if (p.x > 130 && p.x > p.z + 20) {
                saw_magenta = true
            }
        }
    }
    t |> success(non_bg > 10000, "found > 10000 non-background pixels (most of {N_PARTICLES} particles rasterised, got {non_bg})")
    t |> success(max_b > 130,  "saw a strong-blue particle pixel (cool end of the palette reached, max_b={max_b})")
    t |> success(max_r > 130,  "saw a strong-red particle pixel (hot end of the palette reached, max_r={max_r})")
    t |> success(saw_teal,     "found a teal-dominant particle (blue clearly above red, max_b={max_b})")

    delete pixels
}
