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
var private g_find = TextFindState()
var private g_commands = CommandRegistry()
var private g_bindings_editor = CommandBindingsEditorState()
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
    text_edit_register_commands(g_commands)
    g_commands.config_path = command_default_config_path("text_edit_demo")
    text_edit_load(g_edit, SAMPLE)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    command_begin_frame(g_commands)
    let viewport = GetMainViewport()
    SetNextWindowPos(viewport.WorkPos, ImGuiCond.Always)
    SetNextWindowSize(viewport.WorkSize, ImGuiCond.Always)
    window(EDIT_WIN, (text = "text edit", closable = false,
            flags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove)) {
        var bar_rows = 2.0f
        if (g_find.open) {
            bar_rows += g_find.replace_open ? 2.0f : 1.0f
        }
        child(EDIT_CONTENT, (text = "editor content",
              size = float2(0.0f, -(GetFrameHeightWithSpacing() * bar_rows)),
              child_flags = ImGuiChildFlags.None,
              window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
            var style = TextSourceViewStyle(
                fonts = g_fonts,
                write_system_clipboard = !harness_is_headless())
            let _ = text_source_edit("editor", g_edit, style, g_commands)
        }
        if (g_find.open) {
            text_find_bar(0, g_find)
        }
        text_edit_find_sync(g_edit, g_find, g_commands)
        var status = ("Ln {g_edit.cursor_line + 1}, Col {g_edit.cursor_column + 1}"
            + " - {text_edit_line_count(g_edit)} lines")
        if (g_edit.dirty) {
            status += " - modified"
        }
        if (g_edit.overtype) {
            status += " - OVR"
        }
        text_show(EDIT_STATUS, status)
        same_line()
        if (small_button(EDIT_BINDINGS, (text = "Bindings"))) {
            g_bindings_editor.open = true
        }
        // Tail of the escaped buffer only: the full source would overflow the
        // window and grow a phantom horizontal scrollbar.
        var probe = replace(text_edit_source(g_edit), "\n", "\\n")
        let probe_length = length(probe)
        if (probe_length > 60) {
            probe = "..." + slice(probe, probe_length - 60, probe_length)
        }
        text_show(EDIT_PROBE, probe)
    }
    command_bindings_editor(g_commands, g_bindings_editor)
    harness_end_frame()
}

[export]
def shutdown() {
    command_registry_dispose(g_commands)
    text_find_dispose(g_find)
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
