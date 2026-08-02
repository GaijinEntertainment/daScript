options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke + size gate for `dynamic_font_scale`. The three text rows
//! drawn under `with_font(font, 16/24/32)` must report strictly taller bboxes —
//! proof the per-push pixel size reaches PushFont (imgui 1.92 dynamic fonts).

let FEATURE = "modules/dasImgui/examples/features/dynamic_font_scale.das"

[test]
def test_dynamic_font_scale_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_render(d, "FONT_WIN/SIZE_32", 15.0f)
        t |> success(snap != null, "sized text rows render")
        if (snap == null) return

        t |> success(widget_exists(snap, "FONT_WIN/GLOBAL_SCALE"), "global-scale slider registers")

        let bb16 = find_widget(snap, "FONT_WIN/SIZE_16")?["bbox"]
        let bb24 = find_widget(snap, "FONT_WIN/SIZE_24")?["bbox"]
        let bb32 = find_widget(snap, "FONT_WIN/SIZE_32")?["bbox"]
        let h16 = (bb16?["w"] ?? 0.0f) - (bb16?["y"] ?? 0.0f)
        let h24 = (bb24?["w"] ?? 0.0f) - (bb24?["y"] ?? 0.0f)
        let h32 = (bb32?["w"] ?? 0.0f) - (bb32?["y"] ?? 0.0f)
        t |> success(h16 > 0.0f, "16px row has a bbox ({h16})")
        t |> success(h24 > h16, "24px row taller than 16px ({h24} > {h16})")
        t |> success(h32 > h24, "32px row taller than 24px ({h32} > {h24})")
    }
}
