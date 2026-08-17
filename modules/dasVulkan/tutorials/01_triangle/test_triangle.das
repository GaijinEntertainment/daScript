options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 01 pixel-oracle gate (lavapipe in CI, real GPU locally). Two frames prove the whole path:
// angle 0 reproduces the classic static-triangle layout (so the integration test's proven sample
// points hold -- vert+frag emission + Location I/O + interpolation), and angle pi proves the
// push-constant angle actually drives the vertex shader on the GPU (the red vertex moves).

require dastest/testing_boost public
require triangle_tut
require math

[test]
def test_triangle_spin(t : T?) {
    // angle 0: identical geometry to the classic hello-triangle -- the proven sample points hold.
    var p0 <- render_spin_triangle(0.0f)
    let top0 = px(p0, 128, 80)
    let bl0 = px(p0, 75, 190)
    let br0 = px(p0, 180, 190)
    let mid0 = px(p0, 128, 150)
    t |> success(top0.x > top0.y + 40 && top0.x > top0.z + 40, "angle 0: top vertex region is red")
    t |> success(bl0.z > bl0.x + 40 && bl0.z > bl0.y + 40, "angle 0: bottom-left region is blue")
    t |> success(br0.y > br0.x + 40 && br0.y > br0.z + 40, "angle 0: bottom-right region is green")
    t |> success(mid0.x > 20 && mid0.y > 20 && mid0.z > 20, "angle 0: centroid shows the interpolated blend")
    delete p0

    // angle pi: a half turn. The red (top) vertex rotates to bottom-center and the top sample is no
    // longer red -- proving the push-constant angle drives the vertex shader on the GPU.
    var p1 <- render_spin_triangle(PI)
    let topP = px(p1, 128, 80)
    let botP = px(p1, 128, 190)
    t |> success(botP.x > botP.y + 40 && botP.x > botP.z + 40, "angle pi: red vertex rotated to bottom-center")
    t |> success(!(topP.x > topP.y + 40 && topP.x > topP.z + 40), "angle pi: top sample no longer red (rotation took effect)")
    delete p1
}
