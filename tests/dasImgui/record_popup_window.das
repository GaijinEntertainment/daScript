options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record ``popup_window.apng`` against ``examples/tutorial/popup_window.das``
//! (the page's literalinclude source, so the video matches the code). Five voiced,
//! self-verifying beats covering both trigger scopes of the manual-trigger popup:
//!   1. intro       - popup_window has no auto-trigger; the caller drives open_popup.
//!   2. click button - open_popup fires; the popup body renders (rendered=true).
//!   3. pick Apple   - close_current_popup closes it; PW_STATUS -> "Last pick: Apple".
//!   4. right-click  - the second scope opens on a right-release in the hover region.
//!   5. pick Banana  - closes again; PW_STATUS -> "Last pick: Banana".
//! Popups carry no bbox (they float), so open/close is verified through the popup
//! body item's rendered-this-frame state; the pick is verified through the shared
//! status line. The menu_label click_count is shared across both scopes (same IDENT
//! under different with_id), so it is NOT used as a signal here.

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = snap?["globals"]?[ident]?["bbox"]
    return (((b?["x"] ?? 0.0f) + (b?["z"] ?? 0.0f)) * 0.5f,
            ((b?["y"] ?? 0.0f) + (b?["w"] ?? 0.0f)) * 0.5f)
}

//! Click ``pt`` (``button`` 0=left/1=right) under an already-posted voice line, then assert
//! the popup body item ``probe`` is now rendered (popup opened). Caller posts say_begin first
//! (dwell passed here) so prepare_recording can scan the voice line at the call site.
def open_under_voice(app : ImguiApp; dwell : uint; pt : tuple<float; float>;
                     button : int; probe : string) {
    let anchor = record_frame_count(app)
    let lead = dwell > 1400u ? 650u : dwell / 3u
    hold_content(app, lead)
    var ev : array<JsonValue?>
    ev |> click_at(0, pt, 450, button)
    post_command(app, "imgui_mouse_play", JV((events = ev)))
    unsafe { delete ev; }
    let play = 450u + 450u
    hold_content(app, play)
    record_check_rendered(app, probe, true, 180)
    hold_remainder_content(app, dwell, anchor)
}

//! Left-click ``pt`` (a popup menu item) UNDER a voice line, then assert the popup
//! closed (``probe`` no longer rendered) and the shared status line took ``expected``.
def pick_under_voice(app : ImguiApp; dwell : uint; pt : tuple<float; float>;
                     status_target : string; expected : string; probe : string) {
    let anchor = record_frame_count(app)
    let lead = dwell > 1400u ? 650u : dwell / 3u
    hold_content(app, lead)
    var ev : array<JsonValue?>
    ev |> click_at(0, pt, 450)
    post_command(app, "imgui_mouse_play", JV((events = ev)))
    unsafe { delete ev; }
    let play = 450u + 450u
    hold_content(app, play)
    record_check_value(app, status_target, "value", expected, 180)
    record_check_rendered(app, probe, false, 180)
    hold_remainder_content(app, dwell, anchor)
}

[export]
def main {
    with_recording_app("examples/tutorial/popup_window.das",
                       "popup_window.apng", 50) $(app) {
        let T_BTN      = "PW_WIN/via_button/PW_BTN"
        let T_HOVER    = "PW_WIN/via_region/PW_HOVER"
        let T_STATUS   = "PW_WIN/PW_STATUS"
        let BTN_APPLE  = "PW_WIN/via_button/PW_BTN_POPUP/PW_APPLE"
        let REG_BANANA = "PW_WIN/via_region/PW_REG_POPUP/PW_BANANA"

        var snap = wait_for_render(app, T_BTN, 10.0f)
        if (snap == null) {
            panic("{T_BTN} never rendered - wrong app running?")
        }
        let p_btn   = widget_center(snap, T_BTN)
        let p_hover = widget_center(snap, T_HOVER)

        // Warm up the capture pipeline before the first voice (cold-start frames render
        // slower; absorbing them keeps convert's frame->time map uniform) and park the
        // cursor on the button where beat 2 clicks.
        move_to(app, p_btn, 800)
        hold_content(app, 1800u)

        // ---- Beat 1: intro (no action) ----
        say(app, "popup_window: caller drives open_popup", T_BTN,
            [voice = "popup_window has no auto-trigger. The caller decides when to open it, from any predicate it likes."])

        // ---- Beat 2: click the button -> open_popup fires, the body renders ----
        open_under_voice(app, say_begin(app, "click: open_popup", T_BTN,
            [voice = "Click the button. The handler calls open_popup, and popup_window brackets the body that appears."]),
            p_btn, 0, BTN_APPLE)

        // ---- Beat 3: pick Apple -> close_current_popup closes it, status updates ----
        var s3 = snapshot(app)
        let p_apple = widget_center(s3, BTN_APPLE)
        move_to(app, p_apple, 500)
        hold_content(app, SETTLE_MS)
        pick_under_voice(app, say_begin(app, "pick: close_current_popup", BTN_APPLE,
            [voice = "Pick a fruit. The item calls close_current_popup - the popup closes, and the status line updates."]),
            p_apple, T_STATUS, "Last pick: Apple", BTN_APPLE)

        // ---- Beat 4: right-click the hover region -> the second scope opens ----
        move_to(app, p_hover, 600)
        hold_content(app, SETTLE_MS)
        open_under_voice(app, say_begin(app, "right-click: no preceding item", T_HOVER,
            [voice = "The second scope opens on a right-click anywhere in this region - no preceding item required."]),
            p_hover, 1, REG_BANANA)

        // ---- Beat 5: pick Banana -> closes again, status updates ----
        var s5 = snapshot(app)
        let p_banana = widget_center(s5, REG_BANANA)
        move_to(app, p_banana, 500)
        hold_content(app, SETTLE_MS)
        pick_under_voice(app, say_begin(app, "shared str_id, distinct state", REG_BANANA,
            [voice = "Both scopes share one string id, but with_id keeps each scope's open state distinct."]),
            p_banana, T_STATUS, "Last pick: Banana", REG_BANANA)
    }
}
