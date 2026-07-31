options gen2
options _comment_hygiene = true
options gc
options persistent_heap

require imgui/imgui_harness
require imgui/imgui_text_source_edit
require imgui/imgui_text_edit_syntax
require imgui/imgui_text_edit_complete
require imgui/imgui_text_edit_provider_das
require imgui/imgui_fonts
require strings

//! E1 editor smoke surface: one text_source_edit buffer, a status line, and a
//! single-line probe of the whole buffer so tests read keystroke effects back.

var private g_edit = TextEditState()
var private g_find = TextFindState()
var private g_complete = TextEditCompletion()
var private g_das_provider = TextEditDasProvider()
var private g_commands = CommandRegistry()
var private g_bindings_editor = CommandBindingsEditorState()
var private g_fonts = FontRoleSet()
var private g_font_sources : array<string>

// keep line 0 starting "The" and six lines total — the smoke suite counts on both
let private SAMPLE = ("The text editor arc, now with syntax. // E4\n"
    + "\n"
    + "def greet(name : string) : int \{\n"
    + "\treturn print(\"héllo — 你好 \{name\}\") // 42 keywords, strings\n"
    + "\}\n")

[export]
def init() {
    harness_init("text_edit", 960, 640)
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable
    g_fonts = load_unicode_font_roles(16.0f, g_font_sources)
    text_edit_register_commands(g_commands)
    g_commands.config_path = command_default_config_path("text_edit_demo")
    text_edit_load(g_edit, SAMPLE)
    text_edit_set_language(g_edit, "das")
}

def private setup_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let viewport = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, viewport.Size)
    var issues_id = 0u
    var editor_id = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Down, 0.25f, issues_id, editor_id)
    DockBuilderDockWindow("Editor###EDIT_WIN", editor_id)
    DockBuilderDockWindow("Issues###ISSUES_WIN", issues_id)
    DockBuilderFinish(dock_id)
}

[export]
def update() { // nolint:STYLE037
    if (!harness_begin_frame()) return
    harness_new_frame()
    command_begin_frame(g_commands)
    dockspace(EDIT_DOCK, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        if (!EDIT_DOCK.has_initial_layout && EDIT_DOCK.dock_id != 0u) {
            setup_layout(EDIT_DOCK.dock_id)
            EDIT_DOCK.has_initial_layout = true
        }
        draw_editor_window()
        dock_window(ISSUES_WIN, (text = "Issues###ISSUES_WIN", closable = false,
                flags = ImGuiWindowFlags.NoCollapse)) {
            text_edit_issues_panel(g_edit)
        }
    }
    command_bindings_editor(g_commands, g_bindings_editor)
    harness_end_frame()
}

def private draw_editor_window() {
    dock_window(EDIT_WIN, (text = "Editor###EDIT_WIN", closable = false,
            flags = ImGuiWindowFlags.NoCollapse)) {
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
        let _ = text_edit_syntax_sync(g_edit)
        text_edit_complete_sync(g_edit, g_complete)
        text_edit_das_provider_sync(g_edit, g_complete, g_das_provider)
        var status = ("Ln {g_edit.cursor_line + 1}, Col {g_edit.cursor_column + 1}"
            + " - {text_edit_line_count(g_edit)} lines")
        if (g_edit.dirty) {
            status += " - modified"
        }
        if (g_edit.overtype) {
            status += " - OVR"
        }
        if (g_edit.compile_status == TextEditCompileStatus.compiling) {
            status += " - compiling..."
        } elif (g_edit.compile_status == TextEditCompileStatus.ok) {
            status += " - compiles"
        } elif (g_edit.compile_status == TextEditCompileStatus.failed) {
            status += " - {length(g_edit.diagnostics)} error(s)"
        }
        text_show(EDIT_STATUS, status)
        same_line()
        if (small_button(EDIT_BINDINGS, (text = "Bindings"))) {
            g_bindings_editor.open = true
        }
        // buffer tail only - the full source would overflow into a scrollbar
        var probe = replace(text_edit_source(g_edit), "\n", "\\n")
        let probe_length = length(probe)
        if (probe_length > 60) {
            probe = "..." + slice(probe, probe_length - 60, probe_length)
        }
        text_show(EDIT_PROBE, probe)
    }
}

[export]
def shutdown() {
    command_registry_dispose(g_commands)
    text_edit_das_provider_stop(g_das_provider)
    text_edit_complete_dispose(g_complete)
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
