options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require daslib/strings_boost

let EDIT_FEATURE = "modules/dasImgui/examples/features/text_edit.das"
let EDIT_TARGET = "EDIT_WIN/EDIT_CONTENT/editor"
let EDIT_PROBE = "EDIT_WIN/EDIT_PROBE"

[test]
def test_text_edit_keystrokes_reach_buffer(t : T?) {
    let no_modules : array<string>

    with_imgui_app(EDIT_FEATURE, no_modules, "", "") $(d) {
        var initial = wait_for_render(d, EDIT_TARGET, 15.0f)
        t |> success(initial != null, "editor surface renders")
        if (initial == null) return
        t |> equal(widget_payload_field(initial, EDIT_TARGET, "line_count") ?? 0, 6,
            "sample buffer loads six lines")
        t |> success(!(widget_payload_field(initial, EDIT_TARGET, "dirty") ?? true),
            "fresh buffer reports clean")

        click(d, EDIT_TARGET)
        t |> success(wait_for_mouse_idle(d), "focus click timeline drains")
        t |> success(wait_for_bool_value(d, EDIT_TARGET, "focused", true, 300),
            "editor focus effect becomes observable")

        // Deterministic spot before typing: the trailing empty line.
        key_tap(d, int(ImGuiKey.PageDown))
        t |> success(wait_for_key_idle(d, 6.0f), "PageDown drains")
        t |> success(wait_for_int_value(d, EDIT_TARGET, "cursor_line", 5, 300),
            "PageDown clamps the cursor to the last line")
        key_tap(d, int(ImGuiKey.End))
        t |> success(wait_for_key_idle(d, 6.0f), "End drains")

        let typed = post_command(d, "imgui_key_type", JV((text = "abc")))
        t |> success(typed != null, "editor accepts synthesized text")
        t |> success(wait_for_key_idle(d, 6.0f), "typing timeline drains")
        var after_type = wait_until_sec(d, 10.0f) $(var snap) {
            let probe = widget_payload_field(snap, EDIT_PROBE, "value") ?? ""
            return ends_with(probe, "abc")
        }
        t |> success(after_type != null, "typed text lands at the buffer tail")
        if (after_type != null) {
            t |> equal(widget_payload_field(after_type, EDIT_TARGET, "cursor_column") ?? -1,
                3, "cursor sits after the typed text")
            t |> success(widget_payload_field(after_type, EDIT_TARGET, "dirty") ?? false,
                "typing marks the buffer dirty")
        }

        key_tap(d, int(ImGuiKey.Enter))
        t |> success(wait_for_key_idle(d, 6.0f), "Enter drains")
        t |> success(wait_for_int_value(d, EDIT_TARGET, "line_count", 7, 300),
            "Enter splits the buffer into a new line")
        key_tap(d, int(ImGuiKey.Backspace))
        t |> success(wait_for_key_idle(d, 6.0f), "Backspace drains")
        t |> success(wait_for_int_value(d, EDIT_TARGET, "line_count", 6, 300),
            "Backspace at column zero joins the lines back")
        var final_snap = wait_until_sec(d, 5.0f) $(var snap) {
            let probe = widget_payload_field(snap, EDIT_PROBE, "value") ?? ""
            return ends_with(probe, "abc")
        }
        t |> success(final_snap != null, "the join preserves the typed text")
    }
}

[test]
def test_text_edit_numpad_and_overtype(t : T?) {
    let no_modules : array<string>
    with_imgui_app(EDIT_FEATURE, no_modules, "", "") $(d) {
        var initial = wait_for_render(d, EDIT_TARGET, 15.0f)
        t |> success(initial != null, "editor surface renders")
        if (initial == null) return
        click(d, EDIT_TARGET)
        t |> success(wait_for_mouse_idle(d), "focus click timeline drains")
        t |> success(wait_for_bool_value(d, EDIT_TARGET, "focused", true, 300),
            "editor focused")

        // Numpad twins: KP9 = PageUp (no char event = NumLock-off navigation).
        key_tap(d, int(ImGuiKey.Keypad9))
        t |> success(wait_for_key_idle(d, 6.0f), "Keypad9 drains")
        t |> success(wait_for_int_value(d, EDIT_TARGET, "cursor_line", 0, 300),
            "numpad PageUp navigates like the dedicated key")

        // Numpad Home, Insert to overtype: a typed char replaces, not inserts.
        key_tap(d, int(ImGuiKey.Keypad7))
        t |> success(wait_for_key_idle(d, 6.0f), "Keypad7 drains")
        t |> success(wait_for_int_value(d, EDIT_TARGET, "cursor_column", 0, 300),
            "numpad Home navigates like the dedicated key")
        key_tap(d, int(ImGuiKey.Insert))
        t |> success(wait_for_key_idle(d, 6.0f), "Insert drains")
        t |> success(wait_for_bool_value(d, EDIT_TARGET, "overtype", true, 300),
            "Insert toggles overtype mode")
        let line_count_before = widget_payload_field(initial, EDIT_TARGET,
            "line_count") ?? 0
        let typed = post_command(d, "imgui_key_type", JV((text = "Z")))
        t |> success(typed != null, "overtype keystroke accepted")
        t |> success(wait_for_key_idle(d, 6.0f), "overtype keystroke drains")
        var overtyped = wait_until_sec(d, 5.0f) $(var snap) {
            let lines = find_widget(snap, EDIT_TARGET)?["payload"]?["lines"]
            return starts_with(lines?[0] ?? "", "Zhe")
        }
        t |> success(overtyped != null, "overtype replaced the first character")
        if (overtyped != null) {
            t |> equal(widget_payload_field(overtyped, EDIT_TARGET, "line_count") ?? -1,
                line_count_before, "overtype did not grow the buffer")
        }
    }
}
