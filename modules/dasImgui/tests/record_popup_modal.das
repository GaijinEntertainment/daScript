options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record popup_modal.apng against examples/tutorial/popup_modal.das.
//! Every modal is opened and dismissed by a REAL button click and self-verified, so the
//! recording doubles as a test. A button that opens (or closes) a modal is checked by the
//! modal body's rendered state via click_render_voice -- the durable reveal/dismiss signal
//! (a closing button is gone next frame, so its own click-count can't be the proof):
//!   CONFIRM  (Yes/No, non-closable) -> click "Delete" to open, click "No" to dismiss; the
//!                                      answer flows back into STATUS_LINE
//!   SETTINGS (closable, real body)  -> click "Open settings" to open, toggle a checkbox,
//!                                      drag the FPS slider, click "Apply" to dismiss

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

[export]
def main {
    with_recording_app("examples/tutorial/popup_modal.das",
                       "popup_modal.apng", 90) $(app) {
        let T_OPEN_CONFIRM  = "PM_WIN/OPEN_CONFIRM"
        let T_OPEN_SETTINGS = "PM_WIN/OPEN_SETTINGS"
        let T_STATUS        = "PM_WIN/STATUS_LINE"
        let T_CONFIRM_YES   = "PM_WIN/CONFIRM_MODAL/CONFIRM_YES"
        let T_CONFIRM_NO    = "PM_WIN/CONFIRM_MODAL/CONFIRM_NO"
        let T_S_AUTOSAVE    = "PM_WIN/SETTINGS_MODAL/S_AUTOSAVE"
        let T_S_MAX_FPS     = "PM_WIN/SETTINGS_MODAL/S_MAX_FPS"
        let T_S_APPLY       = "PM_WIN/SETTINGS_MODAL/S_APPLY"

        var snap = wait_for_render(app, T_OPEN_CONFIRM, 10.0f)
        if (snap == null) {
            panic("{T_OPEN_CONFIRM} never registered - wrong app running?")
        }

        // ---- Stage 1: click "Delete file..." -> the confirm modal opens ----
        move_to(app, widget_center(snap, T_OPEN_CONFIRM), 600)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "a modal blocks until you answer", T_OPEN_CONFIRM,
            [voice = "A modal is a blocking dialog. Click delete, and it dims the page behind it and waits, your only way forward is to answer."])
        click_render_voice(app, d1, T_OPEN_CONFIRM, T_CONFIRM_YES, true)

        // ---- Stage 2: click "No" -> the modal dismisses, answer flows back ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, T_CONFIRM_NO), 500)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "pick an answer and it closes", T_CONFIRM_NO,
            [voice = "Pick an answer and the dialog closes, and your choice flows right back into the program. We click no this time."])
        click_render_voice(app, d2, T_CONFIRM_NO, T_CONFIRM_NO, false)
        record_check_value(app, T_STATUS, "value", "Last confirm answer: No", 120)

        // ---- Stage 3: click "Open settings" -> the closable settings modal opens ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, T_OPEN_SETTINGS), 700)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "a second modal with real controls", T_OPEN_SETTINGS,
            [voice = "Modals can hold a whole form. This second one is closable, with an X in the corner, and real controls inside."])
        click_render_voice(app, d3, T_OPEN_SETTINGS, T_S_APPLY, true)

        // ---- Stage 4: toggle a checkbox inside the modal ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, T_S_AUTOSAVE), 500)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "controls work inside the modal", T_S_AUTOSAVE,
            [voice = "Controls inside a modal behave exactly like anywhere else. Flip the auto-save checkbox and it toggles."])
        hold_through_voice(app, d4, [T_S_AUTOSAVE])

        // ---- Stage 5: drag the FPS slider inside the modal ----
        snap = snapshot(app)
        let fb = widget_bbox(snap, T_S_MAX_FPS)
        let fcy = (fb.y + fb.w) * 0.5f
        move_to(app, (fb.x + 14.0f, fcy), 600)
        hold_content(app, SETTLE_MS)
        let d5 = say_begin(app, "even a slider, right here", T_S_MAX_FPS,
            [voice = "Even a slider works the same. Drag the frame-rate cap and the value follows your cursor."])
        drag_through_voice(app, d5, T_S_MAX_FPS, (fb.x + 0.72f * (fb.z - fb.x), fcy))

        // ---- Stage 6: click "Apply" -> the settings modal dismisses ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, T_S_APPLY), 600)
        hold_content(app, SETTLE_MS)
        let d6 = say_begin(app, "Apply closes it", T_S_APPLY,
            [voice = "And apply dismisses the whole dialog, handing control back to the page underneath."])
        click_render_voice(app, d6, T_S_APPLY, T_S_APPLY, false)
    }
}
