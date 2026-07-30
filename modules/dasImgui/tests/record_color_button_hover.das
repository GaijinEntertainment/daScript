options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record color_button_hover.apng against examples/tutorial/color_button_hover.das.
//! color_button_hover is a pure HOVER surface — it returns true only while the pointer rests on the
//! swatch, with no click or toggle state. So we drive it the human way (move the cursor onto each
//! swatch, never click) and self-verify that the status line follows the hover (record_check_value,
//! accumulate-on-miss). The hover-only sibling of the click-trigger color_button (see the color tour).

[export]
def main {
    with_recording_app("examples/tutorial/color_button_hover.das",
                       "color_button_hover.apng", 35) $(app) {
        let T_RED    = "CBH_WIN/CBH_RED"
        let T_GREEN  = "CBH_WIN/CBH_GREEN"
        let T_BLUE   = "CBH_WIN/CBH_BLUE"
        let T_STATUS = "CBH_WIN/CBH_STATUS"

        var snap = wait_for_render(app, T_RED, 10.0f)
        if (snap == null) {
            panic("{T_RED} never rendered - wrong app running?")
        }

        //! Land the cursor on the first swatch (flips synth_cursor_owned; red starts hovering).
        move_to(app, widget_click_point(snap, T_RED), 600)
        hold_content(app, SETTLE_MS)

        // ---- Stage 1: hover red — the status line starts following ----
        record_check_value(app, T_STATUS, "value", "Hovering: red", 120)
        say(app, "color_button_hover - pure hover surface", T_RED,
            [voice = "A color button can also be just a hover surface. It returns true only while the pointer rests on the swatch - there is no click and no toggle. Watch the status line follow the cursor."])

        // ---- Stage 2: hover green — the bool follows the cursor ----
        move_to(app, widget_click_point(snap, T_GREEN), 800)
        hold_content(app, SETTLE_MS)
        record_check_value(app, T_STATUS, "value", "Hovering: green", 120)
        say(app, "the bool follows the cursor", T_GREEN,
            [voice = "Move onto another swatch and the return value follows along. Each frame, only the swatch directly under the pointer reports that it is hovered."])

        // ---- Stage 3: hover blue — nothing clicked, nothing remembered ----
        move_to(app, widget_click_point(snap, T_BLUE), 800)
        hold_content(app, SETTLE_MS)
        record_check_value(app, T_STATUS, "value", "Hovering: blue", 120)
        say(app, "no click, nothing remembered", T_BLUE,
            [voice = "There is nothing to click and nothing is stored. Reach for this when the swatch is just a surface for a per-frame effect, like reacting while the mouse is over it."])
    }
}
