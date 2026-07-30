options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for `set_keyboard_focus_here` + `set_item_default_focus`.
//! Clicking REFOCUS_EMAIL routes keyboard focus to EMAIL_INPUT; the snapshot
//! payload's `focus` field flips true on the target input. Mirror with
//! REFOCUS_NAME -> NAME_INPUT. `set_item_default_focus` first-frame behaviour
//! is implementation-detail (headless harness may not preserve it cleanly
//! across test boots), so we don't gate on it here.

let FEATURE = "modules/dasImgui/examples/features/focus_control.das"

[test]
def test_focus_control_smoke(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/EMAIL_INPUT", 15.0f)
        t |> success(snap != null, "EMAIL_INPUT registers")
        if (snap == null) return

        for (ident in ["LEAD", "REFOCUS_NAME", "NAME_INPUT", "REFOCUS_EMAIL",
                       "EMAIL_INPUT", "STATUS"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "MAIN_WIN/{ident} registers")
        }

        // Click REFOCUS_EMAIL -> set_keyboard_focus_here(0) before EMAIL_INPUT
        // -> EMAIL_INPUT.focus becomes true next frame.
        click(d, "MAIN_WIN/REFOCUS_EMAIL")
        let email_focused = wait_until(d, 300) $(var s) {
            return s?["globals"]?["MAIN_WIN/EMAIL_INPUT"]?["focus"] ?? false
        }
        t |> success(email_focused != null, "EMAIL_INPUT.focus == true after REFOCUS_EMAIL click")

        // Now flip back: click REFOCUS_NAME -> NAME_INPUT focus.
        click(d, "MAIN_WIN/REFOCUS_NAME")
        let name_focused = wait_until(d, 300) $(var s) {
            return s?["globals"]?["MAIN_WIN/NAME_INPUT"]?["focus"] ?? false
        }
        t |> success(name_focused != null, "NAME_INPUT.focus == true after REFOCUS_NAME click")
    }
}
