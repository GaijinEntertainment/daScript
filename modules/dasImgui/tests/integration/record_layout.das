options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``layout.apng`` against ``examples/tutorial/layout.das``.
//! Real-input, self-verifying tour of the three boost layout helpers, each
//! proven by dragging its OWN handle and asserting state.value moved:
//!   1. dock_left -> drag SIDEBAR/HANDLE right; sidebar width (px) rises
//!   2. split_h   -> drag SPLIT_MAIN/HANDLE right; left-pane fraction rises
//!   3. split_v   -> drag SPLIT_VERT/HANDLE down;  top-pane fraction rises
//! Each split/dock surfaces its 8px drag handle as a ``<container>/HANDLE``
//! alias (register_handle_bbox), so the cursor grabs the REAL InvisibleButton
//! and drags it - no imgui_force_set. The handle bboxes reflow after every
//! drag, so each beat re-resolves its handle centre from a fresh snapshot.

[export]
def main {
    with_recording_app("examples/tutorial/layout.das",
                       "layout.apng", 75) $(app) {
        let T_SIDEBAR = "LAYOUT_WIN/SIDEBAR"                 // dock_left  - value is px width
        let T_SPLIT_H = "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN"   // split_h    - value is left fraction
        let T_SPLIT_V = "LAYOUT_WIN/SPLIT_VERT"              // split_v    - value is top fraction
        let H_SIDEBAR = "LAYOUT_WIN/SIDEBAR/HANDLE"
        let H_SPLIT_H = "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/HANDLE"
        let H_SPLIT_V = "LAYOUT_WIN/SPLIT_VERT/HANDLE"

        var snap = wait_for_render(app, H_SIDEBAR, 10.0f)
        if (snap == null) {
            panic("{H_SIDEBAR} never rendered - wrong app, or a build without register_handle_bbox?")
        }

        //! Initial cursor placement - flips synth_cursor_owned.
        move_to(app, (40.0f, 30.0f), 600)
        hold_content(app, SETTLE_MS)

        // ---- Beat 1: dock_left - grab the sidebar's right edge, pull it wider ----
        let c1 = widget_click_point(snap, H_SIDEBAR)
        move_to(app, c1, 900)
        hold_content(app, SETTLE_MS)
        drag_through_voice(app, say_begin(app, "dock_left - a fixed left rail", H_SIDEBAR,
            [voice = "We build an editor layout from three composable helpers. The first, dock left, pins a sidebar to the left edge with a draggable border. I grab that border and pull the rail wider."]),
            T_SIDEBAR, (c1._0 + 70.0f, c1._1))

        // ---- Beat 2: split_h - drag the vertical splitter between the top panes ----
        snap = snapshot(app)
        let c2 = widget_click_point(snap, H_SPLIT_H)
        move_to(app, c2, 900)
        hold_content(app, SETTLE_MS)
        drag_through_voice(app, say_begin(app, "split_h - top splits left and right", H_SPLIT_H,
            [voice = "Inside the main area, split h slices the top into a left pane and a right pane, with a vertical handle between them. I drag the handle, and the two panes resize together."]),
            T_SPLIT_H, (c2._0 + 60.0f, c2._1))

        // ---- Beat 3: split_v - drag the horizontal splitter down ----
        snap = snapshot(app)
        let c3 = widget_click_point(snap, H_SPLIT_V)
        move_to(app, c3, 900)
        hold_content(app, SETTLE_MS)
        drag_through_voice(app, say_begin(app, "split_v - top splits from bottom", H_SPLIT_V,
            [voice = "And split v divides that whole top region from the output pane below, on a horizontal handle. I drag it down, and the editor grows while the output shrinks. Every split stays draggable, and each pane clips its own contents."]),
            T_SPLIT_V, (c3._0, c3._1 + 55.0f))
    }
}
