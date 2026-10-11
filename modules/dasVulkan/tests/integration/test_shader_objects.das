options gen2
options _comment_hygiene = true
options indenting = 4

// The shader-objects example renders the same triangle the pipeline path does: two linked shader
// objects bound per stage and the baseline dynamic state set by hand produce the red / green / blue
// corners the integration triangle test samples. Soft-skips where VK_EXT_shader_object is absent.

require dastest/testing_boost public
require ../../examples/shader_objects.das

[test]
def test_shader_objects_triangle(t : T?) {
    if (!shader_objects_available()) {
        feint("VK_EXT_shader_object not advertised by this device; skipping\n")
        return
    }
    var p <- render_shader_object_triangle()
    let top = px(p, 128, 80)
    let bl = px(p, 75, 190)
    let br = px(p, 180, 190)
    let mid = px(p, 128, 150)
    let corner = px(p, 5, 5)
    delete p
    t |> success(top.x > top.y + 40 && top.x > top.z + 40, "top vertex region is red")
    t |> success(bl.z > bl.x + 40 && bl.z > bl.y + 40, "bottom-left region is blue")
    t |> success(br.y > br.x + 40 && br.y > br.z + 40, "bottom-right region is green")
    t |> success(mid.x > 20 && mid.y > 20 && mid.z > 20, "centroid shows the interpolated blend")
    t |> success(corner.x < 40 && corner.z > corner.x, "the corner holds the clear color")
}
