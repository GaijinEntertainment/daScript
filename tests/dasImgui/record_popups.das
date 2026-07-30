options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record popups.apng against examples/tutorial/popups.das.
//! Two right-click context menus, each driven the human way and self-verified:
//!   popup_context_item   -> right-click the button   -> pick Rename  -> PO_RENAME.value == true
//!   popup_context_window -> right-click empty space  -> pick Refresh -> PO_REFRESH.value == true
//! The key discipline (missed on earlier menu tutorials): after the right-click we POLL until
//! the menu item actually renders before travelling onto it - never a fixed sleep guessing the
//! popup is open. The window popup carries NoOpenOverItems, so a right-click ON the button opens
//! the item menu (not the window menu); empty-space right-clicks open the window menu.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

//! A point in the lower, empty half of PO_WIN - guaranteed clear of every item, so the
//! window context menu (NoOpenOverItems) will open there.
def window_empty_point(var snap : JsonValue?) : tuple<float; float> {
    var p = snap?["globals"]?["PO_WIN"]?["payload"]
    let px = p?["pos"]?["x"] ?? 20.0f
    let py = p?["pos"]?["y"] ?? 20.0f
    let sx = p?["size"]?["x"] ?? 720.0f
    let sy = p?["size"]?["y"] ?? 520.0f
    return (px + sx * 0.5f, py + sy * 0.70f)
}

//! Right-click at rclick_pos to open a context menu, WAIT for item_path to render, travel onto
//! it and click, then assert it fired. Paced under an already-posted voice line; total time >=
//! dwell so the next stage's voiceover never overlaps.
def context_pick_voice(app : ImguiApp; dwell : uint; rclick_pos : tuple<float; float>; item_path : string) {
    let anchor = record_frame_count(app)
    let lead = dwell / 3u
    hold_content(app, lead)
    post_command(app, "imgui_mouse_click_at",
                 JV((x = rclick_pos._0, y = rclick_pos._1, button = 1, travel_ms = 350)))
    wait_for_mouse_idle(app)                              // SYNC: the right-click timeline played
    // Poll until the menu item is painted (no fixed sleep). Use widget_rendered, not a raw
    // `rendered ?? false`: painted widgets OMIT the rendered field (present only when
    // registered-but-unpainted), so `?? false` would never see the open menu and time out.
    var opened = wait_until(app, 120) $(var snap) {
        return widget_rendered(snap, item_path)
    }
    if (opened == null) {
        opened = snapshot(app)                           // let record_check_value report the miss
    }
    let ipos = widget_center(opened, item_path)
    post_command(app, "imgui_mouse_click_at",
                 JV((x = ipos._0, y = ipos._1, button = 0, travel_ms = 350)))
    wait_for_mouse_idle(app)                              // SYNC: the item click played
    record_check_value(app, item_path, "value", true, 120)
    hold_remainder_content(app, dwell, anchor)
}

[export]
def main {
    with_recording_app("examples/tutorial/popups.das",
                       "popups.apng", 55) $(app) {
        let T_TARGET  = "PO_WIN/PO_TARGET"
        let T_RENAME  = "PO_WIN/PO_CTX_ITEM/PO_RENAME"
        let T_REFRESH = "PO_WIN/PO_CTX_WIN/PO_REFRESH"

        var snap = wait_for_widget(app, T_TARGET, 10.0f)
        if (snap == null) {
            panic("{T_TARGET} never registered - wrong app running?")
        }

        // ---- Intro ----
        move_to(app, widget_center(snap, T_TARGET), 600)
        hold_content(app, SETTLE_MS)
        say(app, "right-click menus: two flavors", T_TARGET,
            [voice = "Right click menus come in two flavors. One attaches to a single item, the other to the whole window. Let's open each."])

        // ---- Stage 1: popup_context_item - right-click the button ----
        let p_target = widget_center(snap, T_TARGET)
        move_to(app, p_target, 600)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "popup_context_item: right-click the button", T_TARGET,
            [voice = "An item context menu attaches to the item just before it. Right click the button and pick Rename."])
        context_pick_voice(app, d1, p_target, T_RENAME)

        // ---- Stage 2: popup_context_window - right-click empty space ----
        snap = snapshot(app)
        let p_empty = window_empty_point(snap)
        move_to(app, p_empty, 700)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "popup_context_window: right-click empty space", T_TARGET,
            [voice = "A window context menu opens on empty space anywhere in the window. Right click below the button and pick Refresh. A flag stops it opening over an item, so each item keeps its own menu."])
        context_pick_voice(app, d2, p_empty, T_REFRESH)
    }
}
