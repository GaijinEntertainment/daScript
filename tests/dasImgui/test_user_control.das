options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration regression for set_user_control's real-input ownership toggle and
//! the GLFW-callback detach/reattach path added to imgui_synth_tick (the fix for
//! "real clicks sometimes still select while the canvas is locked" — a per-frame
//! event-queue clear couldn't fully close that race, so we now detach the backend
//! input callbacks AT THE SOURCE).
//!
//! Real-input *blocking* needs a physical mouse, so it's verified manually. This
//! guards the toggle's observable contract: the command round-trips, the per-frame
//! tick runs through BOTH transitions (detach + reattach) without crashing or
//! wedging — in headless CI the source-detach is a no-op (no current GLFW window),
//! which also exercises the helper's null-guard — and the UI stays responsive.

let FEATURE = "modules/dasImgui/examples/features/active_widget.das"

[test]
def test_user_control_toggle(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        t |> success(wait_for_widget(d, "MAIN_WIN/DEMO_BTN", 15.0f) != null,
                     "app rendered (ImGui context up)")

        // Disable: subsequent ticks detach the GLFW input callbacks at the source.
        let r_off = post_command(d, "set_user_control", JV((enabled = false)))
        t |> equal(r_off?["user_control"] ?? true, false, "control disabled")

        // Frames keep advancing cleanly while disabled (the per-frame detach +
        // ClearEventsQueue path runs without crashing or wedging).
        t |> success(wait_for_render(d, "MAIN_WIN/DEMO_BTN", 5.0f) != null,
                     "still rendering while control disabled")

        // Re-enable: subsequent ticks reattach the callbacks.
        let r_on = post_command(d, "set_user_control", JV((enabled = true)))
        t |> equal(r_on?["user_control"] ?? false, true, "control re-enabled")

        // Repeated toggle must remain stable (no double install/restore wedge)...
        post_command(d, "set_user_control", JV((enabled = false)))
        let r_on2 = post_command(d, "set_user_control", JV((enabled = true)))
        t |> equal(r_on2?["user_control"] ?? false, true, "stable after repeated toggles")

        // ...and the UI is still responsive afterward (L3 click round-trips).
        let click_after = click(d, "MAIN_WIN/DEMO_BTN")
        t |> equal(click_after?["ok"] ?? false, true, "UI responsive after toggles")
    }
}
