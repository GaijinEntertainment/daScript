options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver script: record ``group.apng`` against ``examples/tutorial/group.das``.
//! Three voiced, self-verifying beats:
//!   1. Two side-by-side groups — toggle a checkbox in each (both values flip,
//!      proving each block is live while sharing a row via same_line).
//!   2. Combined-bbox hit-test — hover the cluster OFF the button; the
//!      ``GRP_TIP`` tooltip fires (record_check_rendered), proving IsItemHovered
//!      after EndGroup reports against the whole group bbox.
//!   3. The (label | value | button) row — drag ROW_SPEED; the value moves.
//! Each effect is asserted; a no-op aborts the recording at teardown.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(
        b?["x"] ?? 0.0f,
        b?["y"] ?? 0.0f,
        b?["z"] ?? 0.0f,
        b?["w"] ?? 0.0f
    )
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/group.das",
                       "group.apng", 60) $(app) {
        let T_LWIRE     = "GRP_WIN/LEFT_GRP/L_WIRE"
        let T_VSYNC     = "GRP_WIN/RIGHT_GRP/R_VSYNC"
        let T_HOVER_BTN = "GRP_WIN/HOVER_GRP/HOVER_BTN"
        let T_TIP       = "GRP_WIN/GRP_TIP"
        let T_SPEED     = "GRP_WIN/ROW_GRP/ROW_SPEED"

        var snap = wait_for_widget(app, T_LWIRE, 10.0f)
        if (snap == null) {
            panic("{T_LWIRE} never registered — wrong app running?")
        }

        //! Initial cursor placement.
        move_to(app, widget_center(snap, T_LWIRE), 600)
        hold_content(app, SETTLE_MS)

        // ---- Beat 1: side-by-side groups (toggle one in each) ----
        hold_through_voice(app, say_begin(app, "side by side via same_line", [T_LWIRE, T_VSYNC],
            [voice = "Each group stacks its own widgets, and same line joins the two whole blocks, so they share one row instead of just chaining off the last widget. Both blocks stay fully live."]),
            [T_LWIRE, T_VSYNC])

        // ---- Beat 2: combined-bbox hit-test fires the tooltip ----
        // Hover the cluster's TEXT, well left of the button, so the tooltip can
        // only fire if the whole-group bbox is the hit-target.
        let hb = widget_bbox(snapshot(app), T_HOVER_BTN)
        move_to(app, (hb.x - 70.0f, (hb.y + hb.w) * 0.5f), 900)
        hold_content(app, SETTLE_MS)
        let dwell2 = say_begin(app, "hover anywhere - whole-group hit-test", T_HOVER_BTN,
            [voice = "Now hover the cluster, but not on the button itself. Anywhere across the icon and the label brings up one tooltip, because the group reports hover over its entire bounding box."])
        record_check_rendered(app, T_TIP, true, 120)
        hold_content(app, dwell2)

        // ---- Beat 3: the (label | value | button) row ----
        let speed_c = widget_center(snapshot(app), T_SPEED)
        move_to(app, speed_c, 800)
        hold_content(app, SETTLE_MS)
        drag_through_voice(app, say_begin(app, "label | value | button as one row", T_SPEED,
            [voice = "And the classic row. A label, a value slider, and a button, kept together as a single unit. Drag the slider, and the value updates on the line just below."]),
            T_SPEED, (speed_c._0 + 150.0f, speed_c._1))
    }
}
