options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver script: record ``edit_tab_item.apng`` against
//! ``examples/tutorial/edit_tab_item.das``. Three voiced, self-verifying beats
//! exercising the bidirectional visibility bind with REAL gestures (no
//! imgui_force_set — every step is the gesture the tutorial teaches):
//!   1. External checkbox hides a tab — click ``ET_CHECK_B`` (the checkbox flips,
//!      hold_through_voice), then assert ``ET_TAB_B``'s bound bool went false.
//!   2. The X close button writes the SAME bool — position-click alpha's X on its
//!      tab header; assert the tab's bool went false AND ``ET_CHECK_A``
//!      back-propagated to false (the bidir payoff this widget exists for).
//!   3. Restore — click both checkboxes back on; assert both tabs' bools went true.

def tab_close_point(var snap : JsonValue?; ident : string) : tuple<float; float> {
    // ImGui draws a tab's X close button at the right edge of its header bbox;
    // ~9px in from the right edge lands on it across label widths.
    var b = snap?["globals"]?[ident]?["bbox"]
    let x1 = b?["z"] ?? 0.0f
    let y0 = b?["y"] ?? 0.0f
    let y1 = b?["w"] ?? 0.0f
    return (x1 - 9.0f, (y0 + y1) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/edit_tab_item.das",
                       "edit_tab_item.apng", 50) $(app) {
        let T_CHECK_A = "ET_WIN/ET_CHECK_A"
        let T_CHECK_B = "ET_WIN/ET_CHECK_B"
        let T_TAB_A   = "ET_WIN/ET_BAR/ET_TAB_A"
        let T_TAB_B   = "ET_WIN/ET_BAR/ET_TAB_B"

        var snap = wait_for_widget(app, T_TAB_A, 10.0f)
        if (snap == null) {
            panic("{T_TAB_A} never registered - wrong app running?")
        }

        //! Initial cursor placement onto the beta checkbox.
        move_to(app, widget_click_point(snap, T_CHECK_B), 600)
        hold_content(app, SETTLE_MS)

        // ---- Beat 1: external checkbox hides a tab ----
        hold_through_voice(app, say_begin(app, "checkbox hides a tab", T_CHECK_B,
            [voice = "The caller owns each tab's visibility bool. Click the external checkbox for beta, and the whole tab disappears, header and body both gone."]),
            [T_CHECK_B])
        // A hidden edit_tab_item still registers with a zeroed bbox (no rendered:false flag), so
        // its bound bool `value` — not `rendered` — is the honest visibility signal; T_TAB_B
        // reading false proves the checkbox->tab bind landed.
        record_check_value(app, T_TAB_B, "value", false, 180)

        // ---- Beat 2: the X close button writes the SAME bool (bidir payoff) ----
        // Alpha is still the selected tab, so its X is drawn. Re-snapshot for the
        // current bbox (beta hid, but alpha is leftmost and unmoved).
        let x_pos = tab_close_point(snapshot(app), T_TAB_A)
        move_to(app, x_pos, 800)
        hold_content(app, SETTLE_MS)
        let dwell2 = say_begin(app, "the X writes the same bool", T_TAB_A,
            [voice = "Now close alpha with the X on its own tab header. The X writes the very same bool, so watch the external show A checkbox flip itself off. One source of truth, two surfaces."])
        let anchor2 = record_frame_count(app)
        let lead2 = dwell2 > 1200u ? 500u : dwell2 / 3u
        hold_content(app, lead2)
        var ev : array<JsonValue?>
        ev |> click_at(0, x_pos, 300)
        post_command(app, "imgui_mouse_play", JV((events = ev)))
        unsafe { delete ev; }
        let play2 = 300u + 500u + 200u   // click_at timeline (travel + dwell tail) + settle
        hold_content(app, play2)
        record_check_value(app, T_TAB_A, "value", false, 180)   // the X wrote the bool
        record_check_value(app, T_CHECK_A, "value", false, 180)  // ...and the checkbox sees it (bidir)
        // Glide to the now-empty show A box: traces the bidir link the voice is
        // describing AND gets the cursor off the tab bar (gamma reflows left into
        // alpha's old slot, so a resting cursor would otherwise sit on gamma's X).
        move_to(app, widget_click_point(snapshot(app), T_CHECK_A), 700)
        hold_remainder_content(app, dwell2, anchor2)

        // ---- Beat 3: restore both tabs via the checkboxes ----
        var snap3 = snapshot(app)
        move_to(app, widget_click_point(snap3, T_CHECK_A), 700)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "restore - caller stays in control", [T_CHECK_A, T_CHECK_B],
            [voice = "Tick the boxes back on, and every tab returns. External bool or X button, the caller always stays in control."]),
            [T_CHECK_A, T_CHECK_B])
        record_check_value(app, T_TAB_A, "value", true, 180)
        record_check_value(app, T_TAB_B, "value", true, 180)
    }
}
