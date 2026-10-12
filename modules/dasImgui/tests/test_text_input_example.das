options gen2
options indenting = 4
options _ascii_strings = false // CJK and supplementary-plane input regression.
require dastest/testing_boost public
require imgui/imgui_playwright public
require imgui
require daslib/json_boost

let private FEATURE = "modules/dasImgui/examples/features/text_input/main.das"
let private SINGLE = "TEXT_INPUT/SINGLE"
let private EDITOR = "TEXT_INPUT/EDITOR/unicode_editor"

[test]
def unicode_input_reaches_rendered_buffers_and_custom_editor_keeps_keyboard_commands(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        let ready = wait_for_visible(app, SINGLE, 15.0)
        t |> success(ready != null)
        if (ready == null) return
        let diagnostic = post_command(app, "input_state", null)
        t |> success(diagnostic?["fonts_ready"] ?? false)
        t |> success(diagnostic?["glyphs_ready"] ?? false)
        let sample = "中文 日本語 한국어 😀"
        let typed = type_text(app, SINGLE, sample)
        t |> success(typed?["ok"] ?? false)
        t |> success(wait_for_string_value(app, SINGLE, "value", sample, 300))
        var rendered = snapshot(app)
        let buffer = widget_payload_field(rendered, SINGLE, "buffer")
        for (byte, index in sample, count()) {
            t |> equal(buffer?[index] ?? -1, int(uint8(byte)), "rendered UTF-8 byte {index}")
        }
        t |> equal(buffer?[length(sample)] ?? -1, 0)
        click(app, EDITOR)
        t |> success(wait_for_mouse_idle(app))
        t |> success(wait_for_bool_value(app, EDITOR, "focused", true, 300))
        let inserted = post_command(app, "imgui_key_type", JV((text = "編集 👩‍💻")))
        t |> success(inserted != null)
        let written = wait_until_sec(app, 5.0) $(var snap) {
            return (widget_payload_field(snap, EDITOR, "lines")?[0] ?? "") == "編集 👩‍💻"
        }
        t |> success(written != null)
        key_tap(app, int(ImGuiKey.Backspace))
        t |> success(wait_for_key_idle(app, 5.0))
        var erased = wait_until_sec(app, 5.0) $(var snap) {
            return (widget_payload_field(snap, EDITOR, "lines")?[0] ?? "") == "編集 "
        }
        t |> success(erased != null, "Backspace still reaches the platform-input owner and removes a whole grapheme")
        t |> equal(widget_payload_field(erased, SINGLE, "value") ?? "", sample, "neighboring field is unchanged")
    }
}
