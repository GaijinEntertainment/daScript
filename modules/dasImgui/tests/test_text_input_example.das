options gen2
options indenting = 4
options _ascii_strings = false // CJK and supplementary-plane input regression.
require dastest/testing_boost public
require imgui/imgui_playwright public
require imgui
require daslib/json_boost
require strings

let private FEATURE = "modules/dasImgui/examples/features/text_input/main.das"
let private SINGLE = "TEXT_INPUT/SINGLE"
let private EDITOR = "TEXT_INPUT/EDITOR/unicode_editor"
let private WINDOW_WIDTH = 1040.0

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
        t |> success(erased != null,
            "Backspace still reaches the platform-input owner and removes a whole grapheme")
        t |> equal(widget_payload_field(erased, SINGLE, "value") ?? "", sample,
            "neighboring field is unchanged")
    }
}

def private ime_field(app : ImguiApp; field : string) : JsonValue? =>
    post_command(app, "input_state", null)?["ime"]?[field]

def private point_in(rect : JsonValue?; bbox : float4) : bool {
    let x = rect?["x"] ?? -1.0
    let y = rect?["y"] ?? -1.0
    return x >= bbox.x && x <= bbox.z && y >= bbox.y && y < bbox.w
}

def private caret_in(app : ImguiApp; bbox : float4) : bool =>
    point_in(ime_field(app, "cursor_rect"), bbox)

def private field_value(app : ImguiApp; ident : string) : string =>
    widget_payload_field(snapshot(app), ident, "value") ?? ""

def private overlay_drawn(app : ImguiApp) : bool {
    let overlay = ime_field(app, "overlay")
    return (overlay?["z"] ?? 0.0) > (overlay?["x"] ?? 0.0)
}

[test]
def composition_draws_at_the_caret_and_a_click_commits_it_into_the_old_field(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        let ready = wait_for_visible(app, SINGLE, 15.0)
        t |> success(ready != null)
        if (ready == null) return
        let single = widget_bbox(snapshot(app), SINGLE)
        click(app, SINGLE)
        t |> success(wait_for_mouse_idle(app))
        post_command(app, "ime_preedit", JV((text = "にほん", caret = 2)))
        t |> success(wait_cond(app, 5.0) <| $() => caret_in(app, single),
            "the candidate rectangle sits on the focused field")
        t |> success(wait_cond(app, 5.0) <| $() => overlay_drawn(app),
            "the preedit overlay is drawn")
        t |> success(point_in(ime_field(app, "overlay"), single),
            "the overlay starts inside the field")
        t |> equal(ime_field(app, "preedit") ?? "", "にほん")
        t |> equal(ime_field(app, "caret") ?? -1, 2)
        t |> equal(widget_payload_field(snapshot(app), SINGLE, "value") ?? "?", "",
            "preedit is not committed into the field")
        let second = widget_bbox(snapshot(app), "TEXT_INPUT/SECONDARY")
        click(app, "TEXT_INPUT/SECONDARY")
        t |> success(wait_for_mouse_idle(app))
        t |> success(wait_cond(app, 5.0) <| $() => caret_in(app, second),
            "the candidate rectangle follows the new owner")
        t |> equal(ime_field(app, "preedit") ?? "?", "", "the click finished the composition")
        t |> equal(ime_field(app, "overlay")?["z"] ?? -1.0, 0.0, "no overlay after the commit")
        var after = snapshot(app)
        t |> equal(widget_payload_field(after, SINGLE, "value") ?? "?", "にほん",
            "the click commits the composition into the field it leaves")
        t |> equal(widget_payload_field(after, "TEXT_INPUT/SECONDARY", "value") ?? "?", "",
            "the new field receives none of it")
    }
}

def private focus_single(t : T?; app : ImguiApp) : bool {
    let ready = wait_for_visible(app, SINGLE, 15.0)
    t |> success(ready != null)
    if (ready == null) return false
    let single = widget_bbox(snapshot(app), SINGLE)
    click(app, SINGLE)
    t |> success(wait_for_mouse_idle(app))
    post_command(app, "ime_preedit", JV((text = "x")))
    let focused = wait_cond(app, 5.0) <| $() => caret_in(app, single)
    post_command(app, "ime_preedit", JV((text = "")))
    t |> success(focused, "the single-line field owns text input")
    return focused
}

[test]
def the_overlay_marks_blocks_and_caret_in_the_field_font_inside_the_window(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!focus_single(t, app))
        post_command(app, "ime_preedit", JV((text = "にほん", caret = 1, split = 1)))
        t |> success(wait_cond(app, 5.0) <| $() => overlay_drawn(app),
            "the preedit overlay is drawn")
        let overlay = ime_field(app, "overlay")
        let marks = ime_field(app, "marks")
        t |> equal(marks?["y"] ?? 0.0, 2.0, "one underline per conversion block")
        t |> equal(marks?["z"] ?? 0.0, 2.0, "the focused block's underline is thicker")
        let caret_x = marks?["x"] ?? -1.0
        t |> success(caret_x > (overlay?["x"] ?? 0.0) && caret_x < (overlay?["z"] ?? 0.0),
            "the caret sits inside the composition")
        t |> success(ime_field(app, "overlay_in_field_font") ?? false,
            "the overlay uses the field's font")
        post_command(app, "ime_preedit", JV((text = "abc")))
        t |> success(wait_cond(app, 5.0) <| $() => (ime_field(app, "preedit") ?? "") == "abc")
        t |> equal(ime_field(app, "caret") ?? -1, 3, "no caret means the end of the composition")
        post_command(app, "ime_preedit", JV((text = "abc", caret = 99)))
        t |> equal(ime_field(app, "caret") ?? -1, 3, "the caret is clamped to the composition")
        post_command(app, "ime_preedit", JV((text = "")))
        click(app, "TEXT_INPUT/LOAD_SAMPLE")
        t |> success(wait_for_mouse_idle(app))
        let loaded = wait_cond(app, 5.0) <| $() => !empty(field_value(app, SINGLE))
        t |> success(loaded, "the samples load")
        return if (!focus_single(t, app))
        key_tap(app, int(ImGuiKey.End))
        t |> success(wait_for_key_idle(app, 5.0))
        let long_text = build_string() $(var writer) {
            for (_ in range(60)) {
                writer |> write("あ")
            }
        }
        post_command(app, "ime_preedit", JV((text = long_text)))
        t |> success(wait_cond(app, 5.0) <| $() => overlay_drawn(app),
            "the long composition is drawn")
        let wide = ime_field(app, "overlay")
        t |> success((wide?["z"] ?? 9999.0) <= WINDOW_WIDTH,
            "a composition that would cross the window's right edge is shifted inside it")
        t |> success((wide?["x"] ?? -1.0) >= 0.0, "and never starts left of the window")
        post_command(app, "ime_preedit", JV((text = "")))
    }
}

[test]
def a_keyboard_owner_change_cancels_and_focus_waits_for_keys_to_settle(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!focus_single(t, app))
        let second = widget_bbox(snapshot(app), "TEXT_INPUT/SECONDARY")
        post_command(app, "ime_preedit", JV((text = "にほん")))
        t |> success(wait_cond(app, 5.0) <| $() => overlay_drawn(app))
        key_tap(app, int(ImGuiKey.Tab))
        t |> success(wait_for_key_idle(app, 5.0))
        t |> success(wait_cond(app, 5.0) <| $() => caret_in(app, second),
            "Tab moves text input to the next field")
        t |> equal(ime_field(app, "preedit") ?? "?", "", "the owner change cancels the composition")
        var after = snapshot(app)
        t |> equal(widget_payload_field(after, SINGLE, "value") ?? "?", "",
            "nothing commits into the old field")
        t |> equal(widget_payload_field(after, "TEXT_INPUT/SECONDARY", "value") ?? "?", "",
            "nor into the new one")
        t |> equal(ime_field(app, "text_focus") ?? -1, 1, "the new field holds text-input focus")
        post_command(app, "imgui_key_play", JV((frame_paced = false, events = [
            JV((t_ms = 0, kind = "press", key = int(ImGuiKey.Escape))),
            JV((t_ms = 3000, kind = "release", key = int(ImGuiKey.Escape)))
        ])))
        t |> success(wait_cond(app, 2.5) <| $() => (ime_field(app, "owner") ?? -1) == 0,
            "Escape ends editing while still held")
        t |> equal(ime_field(app, "text_focus") ?? -1, 1,
            "focus is kept while the key that ended editing is held")
        t |> success(wait_cond(app, 5.0) <| $() => (ime_field(app, "text_focus") ?? -1) == 0,
            "focus is released once keys settle")
    }
}

[test]
def turning_real_input_off_cancels_the_composition(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!focus_single(t, app))
        post_command(app, "ime_preedit", JV((text = "にほん")))
        t |> success(wait_cond(app, 5.0) <| $() => overlay_drawn(app))
        post_command(app, "set_user_control", JV((enabled = false)))
        t |> success(wait_cond(app, 5.0) <| $() => (ime_field(app, "preedit") ?? "?") == "",
            "detaching real input cancels the composition")
        t |> equal(widget_payload_field(snapshot(app), SINGLE, "value") ?? "?", "",
            "nothing is committed")
        post_command(app, "set_user_control", JV((enabled = true)))
    }
}
