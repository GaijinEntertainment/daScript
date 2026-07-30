options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require daslib/strings_boost

let TERMINAL_FEATURE = "modules/dasImgui/examples/features/embedded_terminal.das"
let TERMINAL_TARGET = "TERMINAL_WIN/shell"
let TERMINAL_ZOOM = "TERMINAL_WIN/TERMINAL_ZOOM"
let TERMINAL_VIEW_MENU = "TERMINAL_WIN/TERMINAL_VIEW_MENU"
let TERMINAL_ZOOM_IN = "TERMINAL_WIN/TERMINAL_VIEW_POPUP/TERMINAL_ZOOM_IN"

[test]
def test_embedded_terminal_focus_and_input(t : T?) {
    let extra_modules <- ["modules/dasTerminal"]
    with_imgui_app(TERMINAL_FEATURE, extra_modules, "", "") $(d) {
        var initial = wait_for_render(d, TERMINAL_TARGET, 15.0f)
        t |> success(initial != null, "terminal surface renders")
        if (initial == null) return

        let entry = find_widget(initial, TERMINAL_TARGET)
        t |> equal(entry?["kind"] ?? "", "terminal", "snapshot kind is terminal")
        t |> success((widget_payload_field(initial, TERMINAL_TARGET, "columns") ?? 0) > 0
                   && (widget_payload_field(initial, TERMINAL_TARGET, "rows") ?? 0) > 0,
            "terminal snapshot reports a measured PTY grid")
        t |> success(widget_payload_field(initial, TERMINAL_TARGET, "cursor_blinking") ?? false,
            "terminal snapshot exposes a blinking cursor")
        t |> success(wait_for_string_value(d, TERMINAL_ZOOM, "value", "150", 300),
            "terminal defaults to accessible 150 percent zoom")

        click(d, TERMINAL_VIEW_MENU)
        t |> success(wait_for_mouse_idle(d), "view menu click timeline drains")
        t |> success(wait_for_visible(d, TERMINAL_ZOOM_IN, 5.0f) != null,
            "bottom View menu exposes zoom actions")
        click(d, TERMINAL_ZOOM_IN)
        t |> success(wait_for_mouse_idle(d), "zoom action click timeline drains")
        t |> success(wait_for_string_value(d, TERMINAL_ZOOM, "value", "155", 300),
            "zoom menu advances in five percent steps")

        click(d, TERMINAL_TARGET)
        t |> success(wait_for_mouse_idle(d), "terminal focus click timeline drains")
        t |> success(wait_for_bool_value(d, TERMINAL_TARGET, "focused", true, 300),
            "terminal focus effect becomes observable")

        let command = "Write-Output ('DASHERD_' + 'RESULT')\n"
        let typed = post_command(d, "imgui_key_type", JV((text = command)))
        t |> success(typed != null, "terminal accepts synthesized text")
        t |> success(wait_for_key_idle(d, 6.0f), "terminal key timeline drains")

        var output = wait_until_sec(d, 10.0f) $(var snap) {
            let screen = widget_payload_field(snap, TERMINAL_TARGET, "screen_text") ?? ""
            return contains(screen, "DASHERD_RESULT")
        }
        t |> success(output != null,
            "PowerShell result reaches the semantic terminal snapshot")
        if (output != null) {
            t |> success((widget_payload_field(output, TERMINAL_TARGET, "input_revision") ?? 0) > 0
                       && (widget_payload_field(output, TERMINAL_TARGET, "content_revision") ?? 0) > 0,
                "input and content revisions advanced")
        }
    }
}
