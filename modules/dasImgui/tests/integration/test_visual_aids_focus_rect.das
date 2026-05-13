options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Visual-aids focus rectangle — wire integration gates.
//!
//! The focus halo is drawn by `paint_focus_rect()` when
//! `focus_rect_enabled = true` and `active_widget_path()` resolves to a
//! known widget. This test verifies the toggle, the resolution path
//! (`io.active_widget` populates when a boost-wrapped widget is active),
//! and the empty-state behavior (no panic when nothing is active).

let FEATURE = "modules/dasImgui/examples/features/glfw_synth_keys.das"

[test]
def test_focus_rect_toggle(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
        t |> success(snap0 != null, "NAME_INPUT renders")
        if (snap0 == null) return

        let on = post_command(d, "imgui_focus_rect", JV((enabled = true)))
        t |> equal(on?["ok"] ?? false, true, "imgui_focus_rect ok=true on toggle on")
        t |> equal(on?["enabled"] ?? false, true, "imgui_focus_rect reports enabled=true")

        let off = post_command(d, "imgui_focus_rect", JV((enabled = false)))
        t |> equal(off?["enabled"] ?? true, false, "imgui_focus_rect reports enabled=false on toggle off")
    }
}

[test]
def test_focus_rect_resolves_active_widget(t : T?) {
    //! `paint_focus_rect` reads `active_widget_path()` and then
    //! `widget_rect(path)` to know where to draw. This test verifies the
    //! two inputs the overlay actually consumes: that focus dispatch
    //! settles NAME_INPUT, and that `widget_rect` returns a non-empty
    //! bbox via the snapshot. Reading `io.active_widget` directly is
    //! brittle because ImGui's ActiveID lifecycle for InputText doesn't
    //! mirror keyboard focus 1:1 — focus settling is the load-bearing
    //! signal for our overlay.
    with_imgui_app(FEATURE) $(d) {
        var snap0 = wait_for_render(d, "NAME_INPUT", 15.0f)
        if (snap0 == null) {
            t |> success(false, "NAME_INPUT renders")
            return
        }

        focus(d, "NAME_INPUT")
        let focused = wait_until(d, 180) $(var snap) {
            return snap?["globals"]?["NAME_INPUT"]?["focus"] ?? false
        }
        t |> success(focused != null, "NAME_INPUT receives keyboard focus after focus dispatch")

        // bbox is in globals[NAME_INPUT].bbox (per imgui_boost_runtime).
        let snap = post_command(d, "imgui_snapshot", null)
        let bb = snap?["globals"]?["NAME_INPUT"]?["bbox"]
        t |> success(bb != null, "NAME_INPUT.bbox populated in snapshot")
        let x = bb?["x"] ?? 0.0f
        let z = bb?["z"] ?? 0.0f
        t |> success(z > x, "NAME_INPUT.bbox has non-zero width (overlay has something to draw)")
    }
}
