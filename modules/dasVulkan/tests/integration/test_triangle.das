options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require scene_helpers

[test]
def test_triangle(t : T?) {
    var pixels <- render_triangle()
    let top = px(pixels, 128, 80)
    let bl = px(pixels, 75, 190)
    let br = px(pixels, 180, 190)
    let mid = px(pixels, 128, 150)
    t |> success(top.x > top.y + 40 && top.x > top.z + 40, "top vertex region is red")
    t |> success(bl.z > bl.x + 40 && bl.z > bl.y + 40, "bottom-left vertex region is blue")
    t |> success(br.y > br.x + 40 && br.y > br.z + 40, "bottom-right vertex region is green")
    t |> success(mid.x > 20 && mid.y > 20 && mid.z > 20, "centroid shows the interpolated blend")
    delete pixels
}
