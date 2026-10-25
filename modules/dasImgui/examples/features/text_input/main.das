options gen2
options gc
options persistent_heap
options indenting = 4
options _ascii_strings = false // Unicode input is the subject of this example.

require imgui/imgui_harness
require imgui_app
require imgui/imgui_fonts
require imgui/imgui_text_source_edit
require daslib/module_path
require daslib/fio
require daslib/json_boost
require strings

let private SAMPLE = "中文 日本語 한국어 · café é · 😀 🚀 👍🏽 🇯🇵 👩‍💻"
var private g_font : ImFont?
var private g_editor = TextEditState()
var private g_editor_style = TextSourceViewStyle()
var private g_submissions = 0
var private g_submitted : string
var private g_fonts_ready = false
var private g_web = false
var private g_glyphs_ready = false

def private font_directory() : string {
    if (get_running_platform_name() == "emscripten") return "/text-input-fonts"
    let bundled = path_join(dir_name(get_executable_path()), "fonts")
    if (font_file_exists(path_join(bundled, "noto-sans/NotoSans-Regular.ttf"))) return bundled
    return path_join(get_this_module_dir(), "fonts")
}

[export]
def init() {
    harness_init("CJK and emoji input", 1040, 820)
    GetStyle().FontScaleMain = 1.0
    g_web = get_running_platform_name() == "emscripten"
    let fonts = font_directory()
    g_font = load_font_from_file(path_join(fonts, "noto-sans/NotoSans-Regular.ttf"), 20.0)
    g_fonts_ready = g_font != null
    if (g_fonts_ready) {
        g_fonts_ready = merge_font_from_file(path_join(fonts, "twemoji/Twemoji.Mozilla.ttf"), 20.0, false, IMGUI_FREETYPE_LOAD_COLOR)
        g_fonts_ready = merge_font_from_file(path_join(fonts, "noto-cjk/NotoSansCJKsc-Regular.otf"), 20.0, false) && g_fonts_ready
    }
    g_glyphs_ready = g_fonts_ready
    for (cp in fixed_array(0x4e2du, 0x65e5u, 0xd55cu, 0x1f600u, 0x1f680u)) {
        g_glyphs_ready = font_has_glyph(g_font, cp) && g_glyphs_ready
    }
    g_editor_style.fonts = FontRoleSet(regular = g_font, bold = g_font, italic = g_font, bold_italic = g_font, monospace = g_font, size_px = 20.0)
    text_edit_load(g_editor, "")
}

[live_command(description = "Input buffers, editor cursor and rendered widget snapshot")]
def input_state(_args : JsonValue?) : JsonValue? {
    return JV((single = SINGLE.value, secondary = SECONDARY.value, multiline = MULTILINE.value,
        editor = text_edit_text(g_editor), cursor = int2(g_editor.cursor_line, g_editor.cursor_column),
        editor_focused = g_editor.focused, submissions = g_submissions, submitted = g_submitted,
        font_directory = font_directory(), fonts_ready = g_fonts_ready,
        glyphs_ready = g_glyphs_ready, ime = ime_state(), widgets = imgui_snapshot(null)))
}

def private ime_state() {
    return (preedit = imgui_ime_preedit(), caret = imgui_ime_caret(),
        owner = int64(imgui_ime_owner()), text_focus = imgui_ime_text_focus(),
        has_window = imgui_ime_has_window(), cursor_rect = imgui_ime_cursor_rect(),
        overlay = imgui_ime_overlay(), marks = imgui_ime_overlay_marks(),
        overlay_in_field_font = imgui_ime_overlay_font() == g_font)
}

[live_command(description = "Inject an input-method composition; args text, caret, split")]
def ime_preedit(args : JsonValue?) : JsonValue? {
    let text = args?["text"] ?? ""
    imgui_ime_inject_preedit(text, args?["caret"] ?? -1, args?["split"] ?? 0)
    return JV(ime_state())
}

def private load_sample(value : string) {
    SINGLE.pending_value = value; SINGLE.has_pending = true
    SECONDARY.pending_value = ""; SECONDARY.has_pending = true
    MULTILINE.pending_value = value; MULTILINE.has_pending = true
    text_edit_load(g_editor, value)
    g_submissions = 0
    g_submitted = ""
}

def private draw_fields() {
    text_wrapped("Type with your OS input method or emoji picker. Confirm a conversion, then press Enter again to submit the single-line field.")
    if (input_text(SINGLE, (text = "Single line", flags = ImGuiInputTextFlags.EnterReturnsTrue))) {
        g_submissions++
        g_submitted = SINGLE.value
    }
    input_text(SECONDARY, (text = "Second field"))
    text("Submissions: {g_submissions} · Last: {g_submitted}")
    text("Multiple lines")
    input_text_multiline(MULTILINE, (text = "##multiple_lines", size = float2(-1.0, 130.0)))
    text("dasImgui/text editor — selection, grapheme deletion and undo")
    child(EDITOR, (text = "Editor", size = float2(0.0, 170.0), child_flags = ImGuiChildFlags.Borders,
        window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
        text_source_edit("unicode_editor", g_editor, g_editor_style)
    }
    text("Editor cursor: line {g_editor.cursor_line + 1}, UTF-8 byte {g_editor.cursor_column}")
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    SetNextWindowPos(float2(8.0, 8.0), ImGuiCond.Always)
    SetNextWindowSize(GetIO().DisplaySize - float2(16.0, 16.0), ImGuiCond.Always)
    window(TEXT_INPUT, (text = "CJK and emoji input", closable = false,
        flags = ImGuiWindowFlags.NoMove | ImGuiWindowFlags.NoResize)) {
        with_font(g_font, 20.0) {
            text("Bundled fonts: {g_fonts_ready ? "loaded" : "MISSING"}; sample glyphs: {g_glyphs_ready ? "present" : "MISSING"}")
            text(SAMPLE_LINE, (text = SAMPLE))
            if (button(LOAD_SAMPLE, (text = "Load samples"))) load_sample(SAMPLE)
            same_line()
            if (button(CLEAR, (text = "Clear"))) load_sample("")
            separator()
            draw_fields()
            separator()
            text_wrapped("Check candidate-window position, visible preedit, Enter/Escape, focus changes, copy/paste and emoji sequences. Glyph coverage and grapheme editing do not prove shaping support.")
            text_wrapped("Fonts: Noto Sans and Noto Sans CJK (SIL OFL); Twemoji artwork by Twitter and contributors, Mozilla color font (CC BY 4.0 / Apache 2.0). Licenses and sources are bundled in fonts/.")
        }
    }
    harness_end_frame()
    if (g_web) {
        var state = input_state(null)
        var encoded = write_json_compact(state)
        fwrite("/text-input-state.json", encoded)
        unsafe(delete_string(encoded))
        delete_json(state)
    }
}

[export]
def shutdown() {
    text_edit_dispose(g_editor)
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
        harness_maybe_collect_gc()
    }
    shutdown()
}
