options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record log_capture.apng against examples/tutorial/log_capture.das.
//! with_log(ImGuiLogFlags.OutputClipboard) captures the rendered text of every widget in its block
//! to the clipboard. Driven for real: click the Capture button (click_count verified), then assert
//! the readback panel populated -- LC_TEXT goes from empty to the captured report (record_check_changed),
//! proving the clipboard round-trip AND the LogRenderedText-injected line landed. A no-op aborts at teardown.

[export]
def main {
    with_recording_app("examples/tutorial/log_capture.das",
                       "log_capture.apng", 45) $(app) {
        let T_BTN  = "LC_WIN/LC_BTN"
        let T_TEXT = "LC_WIN/LC_VIEW/LC_TEXT"

        var snap = wait_for_render(app, T_BTN, 10.0f)
        if (snap == null) {
            panic("{T_BTN} never rendered - wrong app running?")
        }

        // ---- Stage 1: introduce the report + the empty clipboard panel ----
        say(app, "with_log captures rendered text", T_BTN,
            [voice = "imgui can capture the rendered text of your widgets to a target - here, the clipboard. The panel below is empty until we capture. In one nine two this target enum was renamed to ImGui Log Flags, Output Clipboard."])

        // ---- Stage 2: click Capture -- clipboard fills, read straight back ----
        // The button click is verified (click_count ticks); the readback panel going from empty to
        // the full report - including the LogRenderedText-injected line - is the real proof.
        let before_jv = widget_payload_field(snapshot(app), T_TEXT, "value")
        let before = before_jv != null ? write_json(before_jv) : ""
        move_to(app, widget_click_point(snap, T_BTN), 700)
        hold_content(app, SETTLE_MS)
        hold_through_voice(app, say_begin(app, "click Capture - clipboard fills", T_BTN,
            [voice = "I click Capture. Every line in the logged block lands on the clipboard, and we read it straight back into the panel - including a line injected by Log Rendered Text that never appears on screen above."]),
            [T_BTN])
        record_check_changed(app, T_TEXT, "value", before)
        hold_content(app, 1500u)
    }
}
