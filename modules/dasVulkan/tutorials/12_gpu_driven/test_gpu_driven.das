options gen2
options _comment_hygiene = true
options indenting = 4
// build_gpu_driven_resources is one large frame (the full GpuDrivenResources struct + ~40 inscope
// locals). dastest runs each test in its own context sized from the program's `options stack`, whose
// small default overflows on that frame (the `daslang` executable's default context stack is large,
// so the window/recording drivers don't hit it). Bump it -- same precedent as the dasPEG tests.
options stack = 1_048_576

// Tutorial 12 pixel oracle (lavapipe in CI, real GPU locally). The wall is pinned at X = 0 so the
// scene is fully deterministic: the central four columns of the cube field sit behind it, the outer
// ring is off-screen, the rest is clear. We validate the GPU's cull DECISION (read back from the
// cull_reason buffer) AND that the rendering reflects it, both ways:
//   * an instance behind the wall is occlusion-culled (reason 2), shows the occluder -- not its own
//     material -- in the camera view, and is a red ghost in the god view;
//   * an instance clear of the wall is visible (reason 0) and shows its material in the camera view;
//   * an off-screen instance is frustum-culled (reason 1);
//   * the GPU-written survivor count equals the number of visible instances (what the indirect-count
//     draw consumed).

require dastest/testing_boost public
require gpu_driven_tut
require math

def private px_at(pixels : array<uint8>; vp : float4x4; i, panel : int) : int3 {
    let p = project_panel(vp, instance_center(i), panel)
    return px(pixels, p.x, p.y)
}

//! The occluder fragment shader is a flat constant (0.45, 0.45, 0.5) -> (115, 115, 127) in 8-bit, with
//! no lighting, so a pixel showing the wall lands tightly on that triple.
def private is_occluder_gray(c : int3) : bool {
    return abs(c.x - 115) < 14 && abs(c.y - 115) < 14 && abs(c.z - 127) < 16
}

def private luma(c : int3) : int => (c.x * 77 + c.y * 150 + c.z * 29) / 256

[test]
def test_gpu_driven_occlusion(t : T?) {
    if (!draw_indirect_count_available()) {
        feint("drawIndirectCount not advertised by this device; skipping (CI lavapipe may predate it)\n")
        return
    }
    var inscope ctx <- build_gpu_driven_context()
    var pixels <- render_frame(ctx, WALL_PIN_X)
    var reasons <- read_cull_reasons(ctx)
    let dc = read_draw_count(ctx)

    // gy = 5 row of the 12x12 grid: f . . . o o o o . . . f
    let A = 5 * GRID_N + 5      // occluded: central column, behind the wall
    let B = 5 * GRID_N + 2      // visible:  left of the wall, on-screen
    let C = 5 * GRID_N + 0      // frustum:  outer ring, off-screen

    // (1) the GPU cull decision, read straight from the cull_reason SSBO
    t |> success(reasons[A] == 2u, "instance A (behind wall) is occlusion-culled, reason 2 (got {reasons[A]})")
    t |> success(reasons[B] == 0u, "instance B (clear of wall) is visible, reason 0 (got {reasons[B]})")
    t |> success(reasons[C] == 1u, "instance C (off-screen) is frustum-culled, reason 1 (got {reasons[C]})")

    // (2) the GPU-written survivor count == the number of visible instances (consumed by the
    //     indirect-count draw -- the CPU never set it)
    var n_visible = 0
    for (i in range(N_INSTANCES)) {
        if (reasons[i] == 0u) {
            n_visible++
        }
    }
    t |> success(int(dc) == n_visible, "draw_count {dc} == visible instances {n_visible}")
    t |> success(n_visible > 0 && n_visible < N_INSTANCES, "some but not all instances survived ({n_visible}/{N_INSTANCES})")

    // (3) camera view: A is hidden behind the opaque wall -> the pixel is the occluder, NOT A's cube.
    //     This is the whole value prop: the culled cube is invisible from the camera.
    let a_cam = px_at(pixels, camera_view_proj(), A, 0)
    t |> success(is_occluder_gray(a_cam), "camera view at A shows the occluder, not the cube (got {a_cam})")

    // (4) god view: A is revealed as a red occlusion ghost.
    let a_god = px_at(pixels, god_view_proj(), A, 1)
    t |> success(a_god.x > 120 && a_god.y < 80 && a_god.z < 80, "god view at A is a red ghost (got {a_god})")

    // (5) camera view: B really was drawn -> a coloured cube, not the gray wall and not the background.
    let b_cam = px_at(pixels, camera_view_proj(), B, 0)
    t |> success(!is_occluder_gray(b_cam) && luma(b_cam) > 25, "camera view at B shows the cube material (got {b_cam})")

    delete reasons
    delete pixels
}
