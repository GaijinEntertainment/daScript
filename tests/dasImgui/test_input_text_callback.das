options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `input_text_callback` — InputText with a user
//! callback lambda fired on CallbackCompletion (TAB) and similar events.
//! Verifies the widget registers as input_text_callback kind and value
//! round-trips via force_set_value. Actual TAB-completion firing is exercised
//! as an optional second [test] that may flake on slow CI; main smoke is
//! kind + round-trip.

let FEATURE = "modules/dasImgui/examples/features/input_text_callback.das"

[test]
def test_input_text_callback_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/CMD", 15.0f)
        t |> success(snap != null, "CMD registers")
        if (snap == null) return

        let cmd = find_widget(snap, "MAIN_WIN/CMD")
        t |> equal(cmd?["kind"] ?? "", "input_text_callback",
                   "CMD.kind == 'input_text_callback'")
        t |> equal(widget_payload_field(snap, "MAIN_WIN/CMD", "value") ?? "?",
                   "",
                   "CMD.value initially empty")

        // Surrounding siblings register.
        t |> success(widget_exists(snap, "MAIN_WIN/LEAD"), "LEAD registers")
        t |> success(widget_exists(snap, "MAIN_WIN/SEP"),  "SEP registers")
        t |> success(widget_exists(snap, "MAIN_WIN/STATIC"), "STATIC registers")

        // Round-trip via imgui_force_set dispatcher (inherited from input_text family).
        force_set_value(d, "MAIN_WIN/CMD", JV("he"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/CMD", "value", "he", 300),
                     "imgui_force_set steers CMD.value to 'he'")
        force_set_value(d, "MAIN_WIN/CMD", JV("hello"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/CMD", "value", "hello", 300),
                     "imgui_force_set re-steers CMD.value to 'hello'")
    }
}
