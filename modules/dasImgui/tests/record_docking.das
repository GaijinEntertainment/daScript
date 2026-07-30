options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``docking.apng`` against ``examples/tutorial/docking.das``.
//! Voiced, self-verifying. Two REAL synthetic drags drive ImGui's native docking
//! the way a user would - no post_command fakery:
//!   Beat 2 drags the node splitter to resize a pane (the synth mouse drives
//!          SplitterBehavior) and asserts EXPLORER.size CHANGED.
//!   Beat 3 drags Output's tab onto Source to stack them as tabs (the synth mouse
//!          drives MovingWindow + the dock preview + the drop) and asserts
//!          OUTPUT.dock_id CHANGED.
//! A no-op drag aborts the recording at teardown.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

// A docked window's own bbox is empty (0,0,0,0); its geometry lives in the
// payload as pos + size. Pack as float4(pos.x, pos.y, size.x, size.y).
def win_rect(var snap : JsonValue?; ident : string) : float4 {
    let p = snap?["globals"]?[ident]?["payload"]
    return float4(p?["pos"]?["x"] ?? 0.0f, p?["pos"]?["y"] ?? 0.0f,
                  p?["size"]?["x"] ?? 0.0f, p?["size"]?["y"] ?? 0.0f)
}

[export]
def main {
    with_recording_app("examples/tutorial/docking.das",
                       "docking.apng", 75) $(app) {
        //! Targets are path-qualified - dockspace pushes onto the registry path.
        let T_EXPLORER    = "DOCK_ROOT/EXPLORER"
        let T_SOURCE      = "DOCK_ROOT/SOURCE"
        let T_OUTPUT      = "DOCK_ROOT/OUTPUT"
        let T_REFRESH_BTN = "DOCK_ROOT/EXPLORER/REFRESH_BTN"
        let T_SAVE_BTN    = "DOCK_ROOT/SOURCE/SAVE_BTN"
        let T_CLEAR_BTN   = "DOCK_ROOT/OUTPUT/CLEAR_BTN"

        var snap = wait_for_render(app, T_REFRESH_BTN, 10.0f)
        if (snap == null) {
            panic("{T_REFRESH_BTN} never rendered - wrong app running?")
        }

        // ---- Beat 1: intro - the three docked panels ----
        move_to(app, (512.0f, 360.0f), 600)
        hold_content(app, SETTLE_MS)
        record_check_rendered(app, T_REFRESH_BTN, true)
        record_check_rendered(app, T_SAVE_BTN, true)
        record_check_rendered(app, T_CLEAR_BTN, true)
        say(app, "ImGui docking - three panels", T_REFRESH_BTN,
            [voice = "ImGui docking turns each panel into a window you can rearrange. Here we have an Explorer on the left, a Source view, and an Output log."])

        // ---- Beat 2: drag the node splitter to resize a pane ----
        // The vertical splitter sits in the gap between Explorer's right edge and
        // Source's left edge; grab its centerline and drag it right to widen Explorer.
        snap = snapshot(app)
        let er = win_rect(snap, T_EXPLORER)
        let sr = win_rect(snap, T_SOURCE)
        let split_x = (er.x + er.z + sr.x) * 0.5f
        let split_y = er.y + er.w * 0.5f
        move_to(app, (split_x, split_y), 700)
        hold_content(app, SETTLE_MS)
        let dwell2 = say_begin(app, "Drag the divider to resize", T_REFRESH_BTN,
            [voice = "Drag the divider between two panels to resize them. I will widen the Explorer pane on the left."])
        drag_through_voice(app, dwell2, T_EXPLORER, (split_x + 150.0f, split_y), 0, "size")

        // ---- Beat 3: drag a tab onto another panel to stack them ----
        // Grab Output's tab (top-left of its node) and drag it onto the center of
        // Source's node; ImGui shows the dock preview and stacks them on release.
        snap = snapshot(app)
        let outr = win_rect(snap, T_OUTPUT)
        let srcr = win_rect(snap, T_SOURCE)
        let tab_pt     = (outr.x + 40.0f, outr.y + 14.0f)
        let src_center = (srcr.x + srcr.z * 0.5f, srcr.y + srcr.w * 0.5f)
        move_to(app, tab_pt, 700)
        hold_content(app, SETTLE_MS)
        let dwell3 = say_begin(app, "Drag a tab to re-dock", T_REFRESH_BTN,
            [voice = "And drag a panel's tab onto another panel to stack them together as tabs. The layout is just data, so it persists between runs."])
        drag_through_voice(app, dwell3, T_OUTPUT, src_center, 0, "dock_id")
    }
}
