options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for the ImGui-bypass synthetic keyboard driver —
//! exercises `imgui_focus` + `imgui_key_type` (and the underlying
//! `imgui_key_*` event-queue layer in `widgets/imgui_live_core.das`).
//! Distinct from `test_glfw_synth_keys.das` which drives the GLFW backend
//! layer; this one bypasses GLFW entirely and feeds chars straight into
//! ImGui's input queue.

let FEATURE = "modules/dasImgui/examples/features/imgui_synth_keys.das"

[test]
def test_imgui_synth_keys_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/NAME_INPUT", 15.0f)
        t |> success(snap != null, "NAME_INPUT registers")
        if (snap == null) return

        let input = find_widget(snap, "MAIN_WIN/NAME_INPUT")
        t |> equal(input?["kind"] ?? "", "input_text",
                   "NAME_INPUT.kind == 'input_text'")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/NAME_INPUT", "value") ?? "?",
                   "",
                   "NAME_INPUT.value initially empty")

        // Focus then type via the ImGui-bypass path. Linux + macOS dispatch focus a frame later
        // than Windows, so settle on the payload's `focus` field before firing chars — otherwise
        // they arrive ahead of activation.
        focus(d, "MAIN_WIN/NAME_INPUT")
        let focused = wait_until(d, 120) $(var s) {
            return s?["globals"]?["MAIN_WIN/NAME_INPUT"]?["focus"] ?? false
        }
        t |> success(focused != null, "NAME_INPUT receives keyboard focus")
        post_command(d, "imgui_key_type", JV((text = "Hello", per_char_ms = 30)))
        t |> success(wait_for_key_idle(d, 4.0f), "key timeline drains")
        t |> success(wait_for_string_value(d, "MAIN_WIN/NAME_INPUT", "value", "Hello", 300),
                     "NAME_INPUT.value == 'Hello' after imgui_key_type")
    }
}
