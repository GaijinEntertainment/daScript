options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require strings
require math

//! Driver: record input_text.apng against examples/tutorial/input_text.das.
//! Tours the six text-editing forms the human way and self-verifies each, so the
//! recording doubles as a test:
//!   basic       -> type "Ada Lovelace", value == typed
//!   with_hint   -> hint visible at rest, then type an address
//!   multiline   -> type two real lines (one '\n' = one newline; see the key-type fix)
//!   growable    -> force_set a >256-byte passage (CallbackResize can't be typed on camera)
//!   callback    -> type "cla" + TAB -> CallbackCompletion fills in "classify"
//!   text_filter -> type "info" -> only matching log lines stay rendered
//! All fields start EMPTY (the macro `text =` arg is the LABEL, not seed content), so a
//! real click + typing lands a clean value with no select-all dance.

def widget_bbox(var snap : JsonValue?; ident : string) : float4 {
    var b = snap?["globals"]?[ident]?["bbox"]
    return float4(b?["x"] ?? 0.0f, b?["y"] ?? 0.0f, b?["z"] ?? 0.0f, b?["w"] ?? 0.0f)
}

def widget_center(var snap : JsonValue?; ident : string) : tuple<float; float> {
    let b = widget_bbox(snap, ident)
    return ((b.x + b.z) * 0.5f, (b.y + b.w) * 0.5f)
}

//! Type-under-voice for plain text fields: real click to focus+activate, stream the text,
//! optionally tap a terminal key (Tab fires CallbackCompletion), then assert the committed value.
//! terminal_key = 0 means none; total time is >= dwell so the next voiceover never overlaps.
def type_field_voice(app : ImguiApp; dwell : uint; target : string;
                     text : string; terminal_key : int; expected : string) {
    let anchor = record_frame_count(app)
    let lead = dwell / 3u
    hold_content(app, lead)
    click(app, target)                  // real click focuses + activates the field
    wait_for_mouse_idle(app)            // SYNC: the click coroutine fully played
    wait_until(app, 120) $(var snap) {  // SYNC: the field actually has focus before any key lands
        return snap?["globals"]?[target]?["focus"] ?? false
    }
    post_command(app, "imgui_key_type",
                 JV((text = text, total_duration_s = max(0.6f, float(length(text)) * 0.07f))))
    wait_for_key_idle(app)              // SYNC: every char streamed in
    if (terminal_key != 0) {
        key_tap(app, terminal_key)
    }
    record_check_value(app, target, "value", expected, 120)
    hold_remainder_content(app, dwell, anchor)
}

[export]
def main {
    with_recording_app("examples/tutorial/input_text.das",
                       "input_text.apng", 90) $(app) {
        // This tutorial is mostly keyboard input - pop each synth key as a fading
        // keycap at bottom-center so the viewer sees exactly what is typed.
        post_command(app, "imgui_key_hud", JV((enabled = true, show_modifiers = false)))

        let T_NAME      = "IT_WIN/IT_NAME"
        let T_EMAIL     = "IT_WIN/IT_EMAIL"
        let T_BIO       = "IT_WIN/IT_BIO"
        let T_FREE      = "IT_WIN/IT_FREE"
        let T_CMD       = "IT_WIN/IT_CMD"
        let T_FILTER    = "IT_WIN/IT_FILTER"
        let T_LINE_DBG  = "IT_WIN/IT_LINE[5]"   // "[debug] ..." — dropped once the filter is "info"

        var snap = wait_for_widget(app, T_NAME, 10.0f)
        if (snap == null) {
            panic("{T_NAME} never registered - wrong app running?")
        }

        // ---- Stage 1: basic input_text ----
        move_to(app, widget_center(snap, T_NAME), 700)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "input_text: single line", T_NAME,
            [voice = "input_text is a single-line editor. Click it, type a name, and the value mirror updates every frame."])
        type_into_voice(app, d1, T_NAME, widget_center(snap, T_NAME), ["Ada Lovelace"], "Ada Lovelace")

        // ---- Stage 2: with_hint (hint visible at rest, then fill it) ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, T_EMAIL), 600)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "with_hint: placeholder when empty", T_EMAIL,
            [voice = "While the buffer is empty, the with-hint form shows a grey placeholder. Type an address and the hint gives way to your text."])
        type_into_voice(app, d2, T_EMAIL, widget_center(snap, T_EMAIL),
                        ["ada@analytical.engine"], "ada@analytical.engine")

        // ---- Stage 3: multiline (real newlines) ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, T_BIO), 600)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "multiline: accepts new lines", T_BIO,
            [voice = "The multiline form is a sized box that takes new lines. Type a couple of lines and they sit one above the other."])
        type_field_voice(app, d3, T_BIO, "First line of the note\nSecond line follows", 0,
                         "First line of the note\nSecond line follows")

        // ---- Stage 4: growable (force_set a >256-byte passage) ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, T_FREE), 600)
        hold_content(app, SETTLE_MS)
        // Capture capacity BEFORE the payload so we can prove the buffer actually grew
        // (CallbackResize), not just that the text was set.
        let cap_before = write_json(snap?["globals"]?[T_FREE]?["payload"]?["capacity"])
        let payload = "The Analytical Engine weaves algebraic patterns just as the Jacquard loom weaves flowers and leaves. It has no pretensions to originate anything; it can do whatever we know how to order it to perform - which already pushes this buffer well past its first 256 bytes."
        let d4 = say_begin(app, "growable: auto-resizing buffer", T_FREE,
            [voice = "The growable form starts at 256 bytes but resizes itself when the text overflows. Drop in a long passage and the capacity climbs."])
        let anchor4 = record_frame_count(app)
        hold_content(app, d4 / 3u)
        force_set_verified(app, T_FREE, payload)
        record_check_changed(app, T_FREE, "capacity", cap_before, 120)   // CallbackResize grew the buffer
        hold_remainder_content(app, d4, anchor4)

        // ---- Stage 5: callback (TAB completion) ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, T_CMD), 600)
        hold_content(app, SETTLE_MS)
        let d5 = say_begin(app, "callback: TAB completes the command", T_CMD,
            [voice = "With a completion callback, typing part of a command and pressing tab fills in the rest. Here it completes to classify."])
        type_field_voice(app, d5, T_CMD, "cla", int(ImGuiKey.Tab), "classify")

        // ---- Stage 6: text_filter (type to filter the log) ----
        snap = snapshot(app)
        move_to(app, widget_center(snap, T_FILTER), 600)
        hold_content(app, SETTLE_MS)
        let d6 = say_begin(app, "text_filter: live include filter", T_FILTER,
            [voice = "The text filter parses comma-separated tokens. Type info, and only the matching log lines stay on screen."])
        type_field_voice(app, d6, T_FILTER, "info", 0, "info")
        record_check_rendered(app, T_LINE_DBG, false, 120)   // the debug line is filtered out
    }
}
