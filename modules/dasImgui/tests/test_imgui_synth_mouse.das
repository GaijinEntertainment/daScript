options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the ImGui-bypass synthetic mouse driver — exercises
//! `imgui_mouse_pos` + `imgui_mouse_click` (raw event-queue layer in
//! `widgets/imgui_live_core.das`). Distinct from `test_glfw_synth_mouse.das`
//! (GLFW layer) and from the higher-level `click()` helper (widget-resolution
//! layer); this one feeds raw screen-space mouse events to ImGui directly.

let FEATURE = "modules/dasImgui/examples/features/imgui_synth_mouse.das"

[test]
def test_imgui_synth_mouse_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "SYNTH_WIN/SYNTH_BTN", 15.0f)
        t |> success(snap != null, "SYNTH_BTN registers")
        if (snap == null) return

        let btn = find_widget(snap, "SYNTH_WIN/SYNTH_BTN")
        t |> equal(btn?["kind"] ?? "", "button",
                   "SYNTH_BTN.kind == 'button'")
        t |> equal(widget_payload_field(snap, "SYNTH_WIN/SYNTH_BTN", "click_count") ?? -1,
                   0,
                   "SYNTH_BTN.click_count == 0 initially")

        // Read bbox to compute mouse target — robust against geometry drift.
        let bbox = btn?["bbox"]
        let bx = ((bbox?["x"] ?? 0.0f) + (bbox?["z"] ?? 0.0f)) * 0.5f
        let by = ((bbox?["y"] ?? 0.0f) + (bbox?["w"] ?? 0.0f)) * 0.5f

        // Bypass mouse to button centre then press+release LMB.
        post_command(d, "imgui_mouse_pos", JV((x = bx, y = by)))
        post_command(d, "imgui_mouse_click", JV((button = 0, action = "press")))
        post_command(d, "imgui_mouse_click", JV((button = 0, action = "release")))
        t |> success(wait_for_mouse_idle(d, 4.0f), "mouse timeline drains")
        t |> success(wait_for_int_value(d, "SYNTH_WIN/SYNTH_BTN", "click_count", 1, 300),
                     "SYNTH_BTN.click_count == 1 after bypass click")
    }
}
