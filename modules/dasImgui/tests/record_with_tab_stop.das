options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record with_tab_stop.apng against examples/tutorial/with_tab_stop.das. with_tab_stop(false)
//! marks the wrapped field as a non-tab-stop, so keyboard Tab cycles A -> B -> C -> D and never lands
//! on it. Driven like a human: click the first field and type, then Tab between fields (a FRAME-PACED
//! discrete tap so it fires exactly once - a wall-clock hold trips ImGui key-repeat and over-tabs) and
//! type again. Self-verifying by the typed VALUES: each reachable field ends holding the word typed
//! into it (so focus must have landed there), and the wrapped field's text is unchanged - proof Tab
//! skipped it. A mis-navigation lands a word in the wrong field and aborts the recording at teardown.

def type_field(app : ImguiApp; text : string) {
    //! Type `text` into whichever field currently holds focus, paced for legibility (short words).
    post_command(app, "imgui_key_type", JV((text = text, total_duration_s = 0.7)))
    wait_for_key_idle(app)
}

def tab_to(app : ImguiApp; target : string) {
    //! Frame-paced Tab tap, then wait until `target` reports focus (the next reachable field).
    key_tap(app, int(ImGuiKey.Tab))
    wait_for_key_idle(app)
    wait_until(app, 120) $(var snap) {
        return snap?["globals"]?[target]?["focus"] ?? false
    }
}

[export]
def main {
    with_recording_app("examples/tutorial/with_tab_stop.das",
                       "with_tab_stop.apng", 45) $(app) {
        let T_A    = "TS_WIN/TS_FIELD_A"
        let T_B    = "TS_WIN/TS_FIELD_B"
        let T_SKIP = "TS_WIN/TS_FIELD_SKIP"
        let T_C    = "TS_WIN/TS_FIELD_C"
        let T_D    = "TS_WIN/TS_FIELD_D"

        var snap = wait_for_widget(app, T_A, 10.0f)
        if (snap == null) {
            panic("{T_A} never registered - wrong app running?")
        }

        // Keyboard-tutorial overlays (body-managed): key_hud shows each Tab keypress,
        // focus_rect outlines the focused field so the viewer SEES focus jump past SKIP.
        post_command(app, "imgui_key_hud", JV((enabled = true, show_modifiers = false)))
        post_command(app, "imgui_focus_rect", JV((enabled = true)))

        // ---- Stage 1: click A and type, then Tab to B and type ----
        move_to(app, widget_click_point(snap, T_A), 600)
        hold_content(app, SETTLE_MS)
        let anchor1 = record_frame_count(app)
        let d1 = say_begin(app, "Tab walks focus field to field", T_A,
            [voice = "Keyboard Tab walks focus from one field to the next. I'll click the first field and type, then press Tab and type again - focus steps straight down the form."])
        hold_content(app, 600u)                      // lead: let the caption + voice start
        click(app, T_A)                              // real click focuses the first field
        wait_until(app, 120) $(var s) {
            return s?["globals"]?[T_A]?["focus"] ?? false
        }
        type_field(app, "one")
        record_check_value(app, T_A, "value", "one", 120)
        tab_to(app, T_B)
        type_field(app, "two")
        record_check_value(app, T_B, "value", "two", 120)
        hold_remainder_content(app, d1, anchor1)

        // ---- Stage 2: Tab skips the wrapped field -> C, type; Tab -> D, type ----
        let anchor2 = record_frame_count(app)
        let d2 = say_begin(app, "Tab skips the with_tab_stop field", [T_SKIP, T_C],
            [voice = "The middle field is wrapped in with_tab_stop, so Tab jumps right over it - straight to the next reachable field, and onward. It never lands on the skipped one."])
        hold_content(app, 700u)
        tab_to(app, T_C)                             // B -> C, skipping SKIP
        type_field(app, "three")
        record_check_value(app, T_C, "value", "three", 120)
        tab_to(app, T_D)
        type_field(app, "four")
        record_check_value(app, T_D, "value", "four", 120)
        // The wrapped field never received a keystroke - its text is unchanged.
        record_check_value(app, T_SKIP, "value", "(unreachable via keyboard)", 120)
        hold_remainder_content(app, d2, anchor2)

        post_command(app, "imgui_key_hud", JV((enabled = false)))
        post_command(app, "imgui_focus_rect", JV((enabled = false)))
    }
}
