options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver script: record ``toggles.apng`` against
//! ``examples/tutorial/toggles.das``. Tours three toggle forms — single
//! checkbox, single radio_button (bool-form, same ToggleState as
//! checkbox, only the glyph differs), and grouped radio_button_int.
//! Each stage clicks the real widget UNDER an ear-first voice line so the
//! ripple and the value readout change while Emma talks.

[export]
def main {
    with_recording_app("examples/tutorial/toggles.das",
                       "toggles.apng", 35) $(app) {
        let T_CHECK      = "TG_WIN/T_CHECK"
        let T_RADIO_BOOL = "TG_WIN/T_RADIO_BOOL"
        let T_MODE       = "TG_WIN/T_MODE"   // bbox = the last (Auto) of the three shared-ident sites

        var snap = wait_for_widget(app, T_CHECK, 10.0f)
        if (snap == null) {
            panic("{T_CHECK} never registered - wrong app running?")
        }
        // Move to the exact point each click will fire (glyph for the toggle family) so the
        // cursor travels straight there — no mid-row approach then hop to the glyph. Stage 3
        // approaches its FIRST click target (Manual = the #1 site alias).
        let p_check  = widget_click_point(snap, T_CHECK)
        let p_radio  = widget_click_point(snap, T_RADIO_BOOL)
        let p_manual = widget_click_point(snap, "{T_MODE}#1")

        //! Initial cursor placement.
        move_to(app, p_check, 500)
        hold_content(app, SETTLE_MS)

        // ---- Stage 1: checkbox — single bool, click flips it ----
        hold_through_voice(app, say_begin(app, "checkbox - single bool", T_CHECK,
            [voice = "This is a checkbox. Click it once and it turns on; click again and it turns back off. The value just flips between true and false."]),
            [T_CHECK, T_CHECK])

        // ---- Stage 2: radio_button (bool form) — same state, round glyph ----
        move_to(app, p_radio, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "radio_button - same, round", T_RADIO_BOOL,
            [voice = "A radio button, on its own, behaves exactly like a checkbox. Same on-off value underneath; the only difference is the round glyph instead of a square."]),
            [T_RADIO_BOOL])

        // ---- Stage 3: radio_button_int — grouped, mutually exclusive ----
        // Per-site aliases T_MODE#<v_button> let us click each option by name even though
        // all three share one ident; click Manual then Auto so the dot walks and clears the last.
        move_to(app, p_manual, 800)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "radio_button_int - pick one of many", T_MODE,
            [voice = "Group several radio buttons and they share one value, so picking one clears the rest. Watch: click Manual, then Auto, and the dot walks across as each pick clears the last."]),
            ["{T_MODE}#1", "{T_MODE}#2"])
    }
}
