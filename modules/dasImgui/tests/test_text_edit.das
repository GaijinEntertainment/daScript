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
let EDIT_TARGET = "EDIT_DOCK/EDIT_WIN/EDIT_CONTENT/editor"
let EDIT_PROBE = "EDIT_DOCK/EDIT_WIN/EDIT_PROBE"

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
        key_tap(d, int(ImGuiKey.Insert)) // leave overtype before the chord tests
        t |> success(wait_for_bool_value(d, EDIT_TARGET, "overtype", false, 300),
            "overtype toggles back off")

        // Registry chords: Ctrl+Right = word end, Shift+End = extend selection.
        key_chord_tap(d, ["Ctrl"], int(ImGuiKey.RightArrow))
        t |> success(wait_for_key_idle(d, 6.0f), "Ctrl+Right drains")
        t |> success(wait_for_int_value(d, EDIT_TARGET, "cursor_column", 3, 300),
            "Ctrl+Right lands at the first word end (VS Code semantics)")
        key_chord_tap(d, ["Shift"], int(ImGuiKey.End))
        t |> success(wait_for_key_idle(d, 6.0f), "Shift+End drains")
        var selected = wait_until_sec(d, 5.0f) $(var snap) {
            return (widget_payload_field(snap, EDIT_TARGET, "anchor_line") ?? -1) >= 0
        }
        t |> success(selected != null, "Shift+End extends a selection via its own binding")

        // Boris's live catch: VS Code double-binds vertical select on Ctrl+Shift.
        key_chord_tap(d, ["Ctrl", "Shift"], int(ImGuiKey.DownArrow))
        t |> success(wait_for_key_idle(d, 6.0f), "Ctrl+Shift+Down drains")
        t |> success(wait_for_int_value(d, EDIT_TARGET, "cursor_line", 1, 300),
            "Ctrl+Shift+Down extends the selection down a line")
    }
}

[test]
def test_text_edit_find_replace(t : T?) {
    let no_modules : array<string>
    with_imgui_app(EDIT_FEATURE, no_modules, "", "") $(d) {
        var initial = wait_for_render(d, EDIT_TARGET, 15.0f)
        t |> success(initial != null, "editor surface renders")
        if (initial == null) return
        click(d, EDIT_TARGET)
        t |> success(wait_for_mouse_idle(d), "focus click timeline drains")
        t |> success(wait_for_bool_value(d, EDIT_TARGET, "focused", true, 300),
            "editor focused")

        // Deterministic fixture on the trailing empty line.
        key_chord_tap(d, ["Ctrl"], int(ImGuiKey.End))
        t |> success(wait_for_key_idle(d, 6.0f), "Ctrl+End drains")
        let typed = post_command(d, "imgui_key_type",
            JV((text = "zzfind zzfind zzfind")))
        t |> success(typed != null, "fixture text accepted")
        var seeded = wait_until_sec(d, 10.0f) $(var snap) {
            let line = find_widget(snap, EDIT_TARGET)?["payload"]?["lines"]?[5] ?? ""
            return line == "zzfind zzfind zzfind"
        }
        t |> success(seeded != null, "fixture line lands in the buffer")

        // Ctrl+Shift+Left selects the last word; Ctrl+H seeds the query from
        // it, opens the replace row, and focuses the replace input.
        key_chord_tap(d, ["Ctrl", "Shift"], int(ImGuiKey.LeftArrow))
        t |> success(wait_for_key_idle(d, 6.0f), "Ctrl+Shift+Left drains")
        key_chord_tap(d, ["Ctrl"], int(ImGuiKey.H))
        t |> success(wait_for_key_idle(d, 6.0f), "Ctrl+H drains")
        var jumped = wait_until_sec(d, 10.0f) $(var snap) {
            // seeded query rebuilt + jump applied: selection sits on match 1
            return (widget_payload_field(snap, EDIT_TARGET, "anchor_line") ?? -1) >= 0
        }
        t |> success(jumped != null, "seeded find selects the first match")

        // Replace text goes to the focused replace input; Enter = replace one.
        let replacement = post_command(d, "imgui_key_type", JV((text = "zzdone")))
        t |> success(replacement != null, "replacement text accepted")
        t |> success(wait_for_key_idle(d, 6.0f), "replacement typing drains")
        key_tap(d, int(ImGuiKey.Enter))
        t |> success(wait_for_key_idle(d, 6.0f), "Enter drains")
        var replaced_one = wait_until_sec(d, 10.0f) $(var snap) {
            let line = find_widget(snap, EDIT_TARGET)?["payload"]?["lines"]?[5] ?? ""
            return line == "zzdone zzfind zzfind"
        }
        t |> success(replaced_one != null, "Enter replaces exactly the current match")

        // Ctrl+Alt+Enter = replace all remaining matches.
        key_chord_tap(d, ["Ctrl", "Alt"], int(ImGuiKey.Enter))
        t |> success(wait_for_key_idle(d, 6.0f), "Ctrl+Alt+Enter drains")
        var replaced_all = wait_until_sec(d, 10.0f) $(var snap) {
            let line = find_widget(snap, EDIT_TARGET)?["payload"]?["lines"]?[5] ?? ""
            return line == "zzdone zzdone zzdone"
        }
        t |> success(replaced_all != null, "replace-all rewrites the remaining matches")

        // Escape closes the bar; the editor takes chords again; ONE undo
        // reverts the whole replace-all.
        key_tap(d, int(ImGuiKey.Escape))
        t |> success(wait_for_key_idle(d, 6.0f), "Escape drains")
        key_chord_tap(d, ["Ctrl"], int(ImGuiKey.Z))
        t |> success(wait_for_key_idle(d, 6.0f), "Ctrl+Z drains")
        var undone = wait_until_sec(d, 10.0f) $(var snap) {
            let line = find_widget(snap, EDIT_TARGET)?["payload"]?["lines"]?[5] ?? ""
            return line == "zzdone zzfind zzfind"
        }
        t |> success(undone != null, "one undo reverts the whole replace-all")
    }
}
