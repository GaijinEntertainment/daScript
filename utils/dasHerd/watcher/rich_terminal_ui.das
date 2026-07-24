options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_terminal_ui

require imgui/imgui_harness
require imgui/imgui_icons
require imgui/imgui_drawlist_builtin
require imgui/imgui_terminal
require imgui/imgui_fonts
require imgui/imgui_docking_builtin
require imgui/imgui_table_builtin
require imgui/imgui_markdown_tree_sitter
require imgui/imgui_text_source_view
require clipboard/clipboard
require daslib/safe_addr
require daslib/json_boost
require daslib/jobque_boost
require live/live_commands
require ./watcher_core.das
require ./repository_core.das
require ./git_graph_model.das
require math
require ./rich_state.das
require ./rich_net.das

def selected_session_label() : string {
    if (g_client == null || empty(g_client.selected_id)) return "No session selected"
    for (session in g_client.sessions) {
        if (session.id == g_client.selected_id) {
            return "{session.state}  pid {session.pid}  {session.command_line}"
        }
    }
    return g_client.selected_id
}

def draw_terminal_footer() {
    if (small_button(TERMINAL_ZOOM_OUT_BUTTON, (text = "-5%"))) zoom_by(-5)
    same_line()
    if (small_button(TERMINAL_ZOOM_IN_BUTTON, (text = "+5%"))) zoom_by(5)
    same_line()
    text("Zoom {g_zoom_percent}%")
    same_line()
    text_disabled("{g_terminal_view.scrollback_rows} history lines - {g_status}")
    if (g_client != null) {
        same_line()
        text_disabled("input sent {g_client.sent_input_bytes} bytes")
    }
}

def copy_terminal_selection() {
    g_status = (imgui_terminal_copy_selection(g_terminal, g_terminal_view)
        ? "Selection copied" : "Nothing selected")
}

def paste_terminal_clipboard() {
    var pasted = ""
    if (clipboard_get_text(pasted) == ClipboardStatus.ok && g_client != null) {
        g_client->send_input(terminal_encode_paste(g_terminal, pasted))
        g_status = "Clipboard pasted"
    } else {
        g_status = "Clipboard unavailable"
    }
}

def draw_terminal_window() {
    let io & = unsafe(GetIO())
    if (IsWindowHovered(ImGuiHoveredFlags.RootAndChildWindows) && io.KeyCtrl) {
        if (io.MouseWheel > 0.0f) zoom_by(5)
        elif (io.MouseWheel < 0.0f) zoom_by(-5)
    }
    text(SESSION_LABEL, (text = selected_session_label()))
    separator()
    let terminal_style = ImGuiTerminalStyle(
        external_input = true,
        cell_backgrounds = false,
        view_height = max(GetTextLineHeight(),
            GetContentRegionAvail().y - GetFrameHeightWithSpacing()
                - GetStyle().ItemSpacing.y))
    with_font(g_terminal_font) {
        if (g_focus_terminal_pending) {
            set_keyboard_focus_here()
            g_focus_terminal_pending = false
        }
        let result <- imgui_terminal("watcher", g_terminal, g_terminal_view,
            g_terminal_cache, terminal_style)
        if (g_client != null) {
            if (!empty(result.input_text)) g_client->send_input(result.input_text)
            if (result.resized) g_client->request_resize(result.columns, result.rows)
        }
    }
    separator()
    draw_terminal_footer()
}

def draw_terminal_dock() {
    dock_window(TERMINAL_WIN.PUBLIC, (text = "Terminal", closable = true,
            flags = ImGuiWindowFlags.None)) {
        draw_terminal_window()
    }
}
