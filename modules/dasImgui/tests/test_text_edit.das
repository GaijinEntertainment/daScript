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
