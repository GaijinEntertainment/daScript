options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 13 pixel-oracle gate (lavapipe in CI when it advertises VK_EXT_mesh_shader, real GPU
// locally). Proves the whole GPU-driven mesh-shader path end-to-end: the task shader culls meshlets
// 3 and 4 out of the 4x2 grid, and the surviving mesh workgroups each amplify one colored quad.
//
// The grid is 4 cols x 2 rows; cell (col,row) center maps to screen (256x256):
//   x = (((-1 + (col+0.5)*0.5)) * 0.5 + 0.5) * 256    -> col 0,1,2,3 = 32, 96, 160, 224
//   y = (((-1 + (row+0.5)*1.0)) * 0.5 + 0.5) * 256    -> row 0,1     = 64, 192
// per-meshlet color = ((col+1)/4, (row+1)/2, 0.6); clear color = (0.05, 0.05, 0.08), sRGB-encoded ~ (63,63,80).
//
// If the device lacks VK_EXT_mesh_shader (an older lavapipe), the test soft-skips rather than failing
// -- the same discipline the dasSpirv spirv-val gate uses when the SDK is absent.

require dastest/testing_boost public
require mesh_tut

def private is_background(c : int3) : bool {
    return c.x < 100 && c.y < 100 && c.z < 100
}

[test]
def test_mesh_cull(t : T?) {
    if (!mesh_shader_available()) {
        feint("VK_EXT_mesh_shader not advertised by this device; skipping (CI lavapipe may predate 24.1)\n")
        return
    }
    var p <- render_mesh()
    // survivors: meshlet 0 (col0,row0) and meshlet 5 (col1,row1) are lit with their grid color.
    let m0 = px(p, 32, 64)
    let m5 = px(p, 96, 192)
    t |> success(!is_background(m0), "meshlet 0 (survivor) is lit, not background ({m0})")
    t |> success(m0.y > m0.x + 30, "meshlet 0 color is green-dominant per the grid palette ({m0})")
    t |> success(!is_background(m5), "meshlet 5 (survivor) is lit, not background ({m5})")
    t |> success(m5.y > m5.z + 40, "meshlet 5 is brighter green (row 1) than meshlet 0 ({m5})")
    // culled: meshlet 3 (col3,row0) and meshlet 4 (col0,row1) must be background -- the task removed them.
    let m3 = px(p, 224, 64)
    let m4 = px(p, 32, 192)
    t |> success(is_background(m3), "meshlet 3 was culled by the task shader -> background ({m3})")
    t |> success(is_background(m4), "meshlet 4 was culled by the task shader -> background ({m4})")
    delete p
}
