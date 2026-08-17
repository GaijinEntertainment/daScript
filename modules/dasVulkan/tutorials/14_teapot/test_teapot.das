options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 14 pixel-oracle gate (lavapipe in CI when it advertises VK_EXT_mesh_shader, real GPU
// locally). Renders the grounded GPU-tessellated Newell teapot offscreen and asserts the whole scene:
// the ceramic teapot silhouette (mesh-shader task cull -> per-patch Bezier tessellation -> shading),
// the env sky in the background, the ground floor below, and the teapot's cast shadow on the floor.
// Includes a lid-knob check that guards the collapsed-control-net-patch regression (the knob's apex
// rows are single points, which used to NaN the cull and drop the knob entirely).
//
// If the device lacks VK_EXT_mesh_shader (an older lavapipe), the test soft-skips rather than failing.

require dastest/testing_boost public
require teapot_tut
require math

//! Warm ceramic: red dominant and clearly lit — the teapot body/spout/handle/knob.
def private is_ceramic_lit(c : int3) : bool {
    return c.x > 120 && c.x > c.y && c.y > c.z
}

//! Env sky: cool blue, blue channel dominant and bright. Distinct from the warm ceramic + gray floor.
def private is_sky(c : int3) : bool {
    return c.z > 150 && c.z > c.x + 20
}

//! Ground floor: near-neutral gray (the checker), mid brightness — holds whether the tile is light,
//! dark, or shadowed; distinct from the blue sky and the warm ceramic.
def private is_floor(c : int3) : bool {
    return abs(c.x - c.z) < 25 && c.x > 30 && c.x < 230
}

//! Average (r+g+b) over a box. Cancels the checker pattern so a shadowed region reads as a clean
//! brightness drop independent of which tiles the box covers.
def private box_luma3(pixels : array<uint8>; x0, x1, y0, y1 : int) : int {
    var sum = 0
    var n = 0
    for (y in range(y0, y1)) {
        for (x in range(x0, x1)) {
            let c = px(pixels, x, y)
            sum += c.x + c.y + c.z
            n ++
        }
    }
    return n > 0 ? sum / n : 0
}

[test]
def test_teapot_render(t : T?) {
    if (!mesh_shader_available()) {
        feint("VK_EXT_mesh_shader not advertised by this device; skipping (CI lavapipe may predate 24.1)\n")
        return
    }
    var pixels <- render_teapot(0.7)
    // body / upper body / spout / handle are lit with the ceramic material -- proves the patches
    // tessellate + shade and that the backface cull did not punch holes in the visible surface.
    t |> success(is_ceramic_lit(px(pixels, 256, 290)), "teapot body is lit ceramic ({px(pixels, 256, 290)})")
    t |> success(is_ceramic_lit(px(pixels, 256, 255)), "teapot upper body is lit ceramic ({px(pixels, 256, 255)})")
    t |> success(is_ceramic_lit(px(pixels, 360, 235)), "teapot spout is lit ceramic ({px(pixels, 360, 235)})")
    t |> success(is_ceramic_lit(px(pixels, 110, 275)), "teapot handle is lit ceramic ({px(pixels, 110, 275)})")
    // lid knob present (collapsed-patch regression guard): count ceramic pixels in the knob box. The
    // env sky now sits behind the knob, so "not background" no longer isolates it -- count ceramic.
    var knob_ceramic = 0
    for (y in range(185, 212)) {
        for (x in range(238, 274)) {
            if (is_ceramic_lit(px(pixels, x, y))) {
                knob_ceramic ++
            }
        }
    }
    t |> success(knob_ceramic > 120, "lid knob renders (ceramic px = {knob_ceramic}, expect ~230)")
    // grounding: env sky fills the corners + above; the ground floor sits below.
    t |> success(is_sky(px(pixels, 10, 10)), "top-left corner is env sky ({px(pixels, 10, 10)})")
    t |> success(is_sky(px(pixels, 502, 10)), "top-right corner is env sky ({px(pixels, 502, 10)})")
    t |> success(is_sky(px(pixels, 256, 140)), "above the teapot is env sky ({px(pixels, 256, 140)})")
    t |> success(is_floor(px(pixels, 40, 500)), "ground floor below-left ({px(pixels, 40, 500)})")
    t |> success(is_floor(px(pixels, 256, 500)), "ground floor below-center ({px(pixels, 256, 500)})")
    // the teapot casts a shadow: the floor box on the shadow side (left of the teapot) is markedly
    // darker than the symmetric box on the lit side (right). Box averages cancel the checker so the
    // drop is the shadow, not the tiling.
    let shadow_box = box_luma3(pixels, 70, 170, 360, 430)
    let lit_box = box_luma3(pixels, 350, 450, 360, 430)
    t |> success(lit_box > shadow_box + 100, "teapot casts a floor shadow (lit {lit_box} vs shadow {shadow_box})")
    delete pixels
}
