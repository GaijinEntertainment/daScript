options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``child.apng`` against ``examples/tutorial/child.das``.
//! Real-input, self-verifying tour of the three ``child`` variants, each
//! proven by its own telemetry:
//!   A. bordered fixed-size  -> wheel-scroll; assert ChildState.scroll moved
//!   B. AutoResizeY          -> toggle the VSync checkbox inside the child
//!   C. HorizontalScrollbar  -> wheel-scroll sideways; assert scroll moved
//! Cursor + narration anchor at LEAF widgets: a [container] (child) does not
//! capture a bbox on finalize, so the container path resolves to (0,0); the
//! container DOES carry a scroll payload, read directly for the assertions.

//! Left edge of a leaf's bbox, vertically centered. The horizontal-scroll child
//! holds one 2000+px-wide line whose CENTER sits off-screen-right (outside the
//! visible child), so wheel events parked there aren't attributed to the child.
//! Anchoring at the left edge keeps the cursor inside the visible region.
def left_anchor(var snap : JsonValue?; ident : string; dx : float) : tuple<float; float> {
    var b = snap?["globals"]?[ident]?["bbox"]
    let x0 = b?["x"] ?? 0.0f
    let y0 = b?["y"] ?? 0.0f
    let y1 = b?["w"] ?? 0.0f
    return (x0 + dx, (y0 + y1) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/child.das",
                       "child.apng", 70) $(app) {
        let T_A      = "CHILD_WIN/SCROLL_A"                 // container - scroll telemetry
        let T_A_MID  = "CHILD_WIN/SCROLL_A/SCROLL_ROW[5]"   // a mid row - park the cursor over the child
        let T_B_VSYNC = "CHILD_WIN/AUTO_B/B_VSYNC"
        let T_C      = "CHILD_WIN/SCROLL_C"                 // container - scroll telemetry
        let T_C_LEAF = "CHILD_WIN/SCROLL_C/LONG_LINE"

        var snap = wait_for_widget(app, T_A_MID, 10.0f)
        if (snap == null) {
            panic("{T_A_MID} never registered - wrong app running?")
        }
        let p_a_mid   = widget_click_point(snap, T_A_MID)
        let p_b_vsync = widget_click_point(snap, T_B_VSYNC)
        let p_c       = left_anchor(snap, T_C_LEAF, 160.0f)   // wide line: park at its LEFT edge, inside the visible child

        //! Initial cursor placement - flips synth_cursor_owned.
        move_to(app, (40.0f, 30.0f), 600)
        hold_content(app, SETTLE_MS)

        // ---- Stage A: bordered, fixed-size child -> wheel scrolls it ----
        move_to(app, p_a_mid, 900)
        hold_content(app, SETTLE_MS)
        let anchorA = record_frame_count(app)               // anchor at say_begin so the dwell ends WITH the voice
        let dA = say_begin(app, "child - bordered, scrollable sub-window", T_A_MID,
            [voice = "A child is a sub window living inside another window. This one has a border and a fixed size, so its rows scroll. I roll the wheel, and the scroll position climbs."])
        let leadA = dA > 1200u ? 600u : dA / 3u
        hold_content(app, leadA)
        wait_for_mouse_idle(app)                            // cursor settled over the child before the wheel
        var snap_a = snapshot(app)
        let a0 = widget_payload_field(snap_a, T_A, "scroll")
        let a0s = a0 != null ? write_json(a0) : ""
        scroll_child_down(app)
        record_check_changed(app, T_A, "scroll", a0s, 200)  // the wheel moved ChildState.scroll
        hold_remainder_content(app, dA, anchorA)            // hold the caption for the rest of the voice

        // ---- Stage B: AutoResizeY child -> a live checkbox inside it ----
        move_to(app, p_b_vsync, 900)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "child - AutoResizeY tracks content", T_B_VSYNC,
            [voice = "This child uses auto resize. Its height grows to fit whatever it holds, with no scrollbar. It still carries live widgets, like this checkbox. I toggle it."]),
            [T_B_VSYNC])

        // ---- Stage C: HorizontalScrollbar child -> wheel scrolls sideways ----
        move_to(app, p_c, 900)
        hold_content(app, SETTLE_MS)
        let anchorC = record_frame_count(app)
        let dC = say_begin(app, "child - horizontal scroll for wide content", T_C_LEAF,
            [voice = "The last child turns on a horizontal scrollbar for one very wide line. I roll the wheel sideways, and the horizontal scroll position follows."])
        let leadC = dC > 1200u ? 600u : dC / 3u
        hold_content(app, leadC)
        wait_for_mouse_idle(app)
        var snap_c = snapshot(app)
        let c0 = widget_payload_field(snap_c, T_C, "scroll")
        let c0s = c0 != null ? write_json(c0) : ""
        scroll_child_right(app)
        record_check_changed(app, T_C, "scroll", c0s, 200)
        hold_remainder_content(app, dC, anchorC)
    }
}

//! Three paced vertical wheel notches - one event barely moves; three read as a scroll.
def scroll_child_down(app : ImguiApp) {
    for (_i in range(3)) {
        post_command(app, "imgui_mouse_scroll", JV((x = 0.0f, y = -3.0f)))
        hold_content(app, 350u)
    }
}

def scroll_child_right(app : ImguiApp) {
    for (_i in range(3)) {
        post_command(app, "imgui_mouse_scroll", JV((x = -5.0f, y = 0.0f)))
        hold_content(app, 350u)
    }
}
