options gen2
options _comment_hygiene = true
options gc
options persistent_heap

require imgui/imgui_harness
require imgui/imgui_text_source_edit
require imgui/imgui_fonts
require strings

//! E1 editor smoke surface: one text_source_edit buffer, a status line, and a
//! single-line probe of the whole buffer so tests read keystroke effects back.

var private g_edit = TextEditState()
var private g_fonts = FontRoleSet()
var private g_font_sources : array<string>

let private SAMPLE = ("The text editor arc, step E1.\n"
    + "\n"
    + "Type here: buffer, cursor, virtualised render.\n"
    + "\tTabs advance a fixed number of cells.\n"
    + "Unicode: héllo — 你好 🌍\n")

[export]
def init() {
    harness_init("text_edit", 960, 640)
    g_fonts = load_unicode_font_roles(16.0f, g_font_sources)
    text_edit_load(g_edit, SAMPLE)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    let viewport = GetMainViewport()
    SetNextWindowPos(viewport.WorkPos, ImGuiCond.Always)
    SetNextWindowSize(viewport.WorkSize, ImGuiCond.Always)
    window(EDIT_WIN, (text = "text edit", closable = false,
            flags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove)) {
        child(EDIT_CONTENT, (text = "editor content",
              size = float2(0.0f, -(GetFrameHeightWithSpacing() * 2.0f)),
              child_flags = ImGuiChildFlags.None,
              window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
            var style = TextSourceViewStyle(
                fonts = g_fonts,
                write_system_clipboard = !harness_is_headless())
            let _ = text_source_edit("editor", g_edit, style)
        }
        var status = ("Ln {g_edit.cursor_line + 1}, Col {g_edit.cursor_column + 1}"
            + " - {text_edit_line_count(g_edit)} lines")
        if (g_edit.dirty) {
            status += " - modified"
        }
        text_show(EDIT_STATUS, status)
        // Tail of the escaped buffer only: the full source would overflow the
        // window and grow a phantom horizontal scrollbar.
        var probe = replace(text_edit_source(g_edit), "\n", "\\n")
        let probe_length = length(probe)
        if (probe_length > 60) {
            probe = "..." + slice(probe, probe_length - 60, probe_length)
        }
        text_show(EDIT_PROBE, probe)
    }
    harness_end_frame()
}

[export]
def shutdown() {
    text_edit_dispose(g_edit)
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
