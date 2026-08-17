options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 15 pixel oracle: the GPU hardware-RT frame must match the CPU reference tracer
// pixel-for-pixel. Both halves run the SAME daslang scene tables and shading functions
// (raytracing_tut_shaders) -- the GPU via dasSpirv-lowered RT shaders and VK_KHR_ray_tracing,
// the CPU in the interpreter. Only silhouette pixels may differ (hardware watertight traversal
// vs the CPU intersector rounding the boundary differently), so the gate is: every pixel within
// a small per-channel tolerance, up to a <1% edge budget. The CPU frame also proves the scene
// exercises every regime: sky, lit ground, SHADOWED ground (the shadow ray + second miss
// shader), and the box faces.

require dastest/testing_boost public
require raytracing_tut

let TEST_DIM = 128
let TOL = 3          // per-channel UNORM8 tolerance for matching pixels
let EDGE_BUDGET = 164   // ~1% of 128*128: silhouette pixels allowed to disagree beyond TOL

[test]
def test_raytracing(t : T?) {
    if (!rt_available()) {
        feint("VK_KHR_ray_tracing_pipeline not supported by this device; skipping (CI lavapipe has no RT)\n")
        return
    }
    var gpu <- render_raytracing(0.0, TEST_DIM, TEST_DIM)
    var cpu <- render_raytracing_cpu(0.0, TEST_DIM, TEST_DIM)
    t |> success(length(gpu) == TEST_DIM * TEST_DIM * 4, "GPU frame is {TEST_DIM}x{TEST_DIM} RGBA8")
    t |> success(length(cpu) == length(gpu), "CPU reference frame has the same size")

    // the CPU reference must contain all four regimes -- proves the scene really exercises the
    // sky miss, plain lambert, the shadow ray, and the box hit group
    var sky_px = 0
    var lit_ground_px = 0
    var shadow_ground_px = 0
    var box_px = 0
    let lit_ground = shade_lambert(float3(0.0, 1.0, 0.0), rt_ground_albedo(), 1.0)
    let dark_ground = shade_lambert(float3(0.0, 1.0, 0.0), rt_ground_albedo(), 0.0)
    for (py in range(TEST_DIM)) {
        for (px in range(TEST_DIM)) {
            let o = (py * TEST_DIM + px) * 4
            let r = int(cpu[o])
            let g = int(cpu[o + 1])
            let b = int(cpu[o + 2])
            if (b > r && b > g) {
                sky_px++   // both sky gradient stops are blue-dominant
            } elif (abs(r - int(255.0 * lit_ground.x)) <= 2 && abs(g - int(255.0 * lit_ground.y)) <= 2) {
                lit_ground_px++
            } elif (abs(r - int(255.0 * dark_ground.x)) <= 2 && abs(g - int(255.0 * dark_ground.y)) <= 2) {
                shadow_ground_px++
            } elif (r > g && r > b) {
                box_px++   // the box albedo is red-dominant at any lambert level
            }
        }
    }
    t |> success(sky_px > 100, "CPU reference shows sky ({sky_px} px)")
    t |> success(lit_ground_px > 100, "CPU reference shows lit ground ({lit_ground_px} px)")
    t |> success(shadow_ground_px > 100, "CPU reference shows the cast shadow ({shadow_ground_px} px)")
    t |> success(box_px > 100, "CPU reference shows the box ({box_px} px)")

    // GPU == CPU up to the silhouette budget
    var over_tol = 0
    var first_bad = int2(-1, -1)
    for (py in range(TEST_DIM)) {
        for (px in range(TEST_DIM)) {
            let o = (py * TEST_DIM + px) * 4
            var worst = 0
            for (c in range(3)) {
                let d = abs(int(gpu[o + c]) - int(cpu[o + c]))
                if (d > worst) {
                    worst = d
                }
            }
            if (worst > TOL) {
                over_tol++
                if (first_bad.x < 0) {
                    first_bad = int2(px, py)
                }
            }
        }
    }
    t |> success(over_tol <= EDGE_BUDGET,
        "GPU matches the CPU reference: {over_tol} of {TEST_DIM * TEST_DIM} pixels beyond +-{TOL} (budget {EDGE_BUDGET}, first at {first_bad})")
    delete gpu
    delete cpu
}
