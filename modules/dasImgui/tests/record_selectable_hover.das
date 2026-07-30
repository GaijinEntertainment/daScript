options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record selectable_hover.apng against examples/tutorial/selectable_hover.das.
//! selectable_hover is a pure HOVER surface — the row never persists as selected; while the
//! pointer rests on it the row returns hovered=true and the SH_STATUS line echoes "Hovering: <row>".
//! So we drive it the human way (move the cursor onto each row, no click) and self-verify that
//! SH_STATUS follows the hover (record_check_value, accumulate-on-miss). Rows are mouse-cursor
//! names (Arrow / TextInput / ResizeAll / Hand / NotAllowed) — the canonical selectable_hover use.

[export]
def main {
    with_recording_app("examples/tutorial/selectable_hover.das",
                       "selectable_hover.apng", 45) $(app) {
        let T_ROW0   = "SH_WIN/SH_ROW[0]"   // Arrow
        let T_ROW2   = "SH_WIN/SH_ROW[2]"   // ResizeAll
        let T_ROW4   = "SH_WIN/SH_ROW[4]"   // NotAllowed
        let T_STATUS = "SH_WIN/SH_STATUS"

        var snap = wait_for_render(app, T_ROW0, 10.0f)
        if (snap == null) {
            panic("{T_ROW0} never rendered - wrong app running?")
        }

        //! Land the cursor on the first row (flips synth_cursor_owned; the row starts hovering).
        move_to(app, widget_click_point(snap, T_ROW0), 600)
        hold_content(app, SETTLE_MS)

        // ---- Stage 1: hover the first row — status starts following ----
        record_check_value(app, T_STATUS, "value", "Hovering: Arrow", 120)
        say(app, "selectable_hover - pure hover surface", T_ROW0,
            [voice = "A selectable hover row lights up while the pointer rests on it, and it never stays selected. Watch the status line at the bottom follow the highlight."])

        // ---- Stage 2: move down the list — status tracks the row under the cursor ----
        move_to(app, widget_click_point(snap, T_ROW2), 800)
        hold_content(app, SETTLE_MS)
        record_check_value(app, T_STATUS, "value", "Hovering: ResizeAll", 120)
        say(app, "status follows the hover", T_ROW2,
            [voice = "As the pointer moves down the list, each row reports that it is hovered, and the status updates to match the one under the cursor."])

        // ---- Stage 3: last row — no click, nothing persisted ----
        move_to(app, widget_click_point(snap, T_ROW4), 800)
        hold_content(app, SETTLE_MS)
        record_check_value(app, T_STATUS, "value", "Hovering: NotAllowed", 120)
        say(app, "no toggle, nothing remembered", T_ROW4,
            [voice = "There is no click to make and nothing is remembered. The row is purely a hover surface, for a per-frame side effect like setting the mouse cursor."])
    }
}
