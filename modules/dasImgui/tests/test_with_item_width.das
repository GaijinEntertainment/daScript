options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke + width gate for `with_item_width`. The 120-px-wide
//! slider inside the wrapper should render with bbox width ≈ 120 px.

let WITH_ITEM_WIDTH_FEATURE = "modules/dasImgui/examples/features/with_item_width.das"

[test]
def test_with_item_width_smoke(t : T?) {
    with_imgui_app(WITH_ITEM_WIDTH_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/NARROW_W", 15.0f)
        t |> success(snap0 != null, "NARROW_W slider registers")
        if (snap0 == null) return

        // Slider bbox is float4 (x0, y0, x1, y1) serialized as JV with x/y/z/w.
        let bb = find_widget(snap0, "MAIN_WIN/NARROW_W")?["bbox"]
        let x0 = bb?["x"] ?? 0.0f
        let x1 = bb?["z"] ?? 0.0f
        let width = x1 - x0
        // ImGui adds ~label + framepadding; the slider's drawn rect lands close
        // to 120 but not exact. Allow [60, 200].
        t |> success(width > 60.0f && width < 200.0f,
                     "NARROW_W width ({width}) within with_item_width(120) range")
    }
}
