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

// Token-colored session line (note 20): state and pid render in the shared
// token palette instead of one plain string.
def draw_session_label() {
    if (g_client == null || empty(g_client.selected_id)) {
        text(SESSION_LABEL, (text = "No session selected"))
        return
    }
    for (session in g_client.sessions) {
        if (session.id == g_client.selected_id) {
            text_colored(SESSION_STATE_TOKEN,
                (color = token_state_color("{session.state}"),
                    text = "{session.state}"))
            same_line()
            text_colored(SESSION_PID_TOKEN,
                (color = token_pid_color(), text = "pid {session.pid}"))
            same_line()
            text(SESSION_LABEL, (text = session.command_line))
            return
        }
    }
    text(SESSION_LABEL, (text = g_client.selected_id))
}

// The terminal owns its text size (Boris, 2026-07-27): these buttons and
// Ctrl+wheel over the screen move the TERMINAL scale, never the UI scale.
def terminal_zoom_by(delta : int) {
    set_content_zoom(CONTENT_TERMINAL, content_zoom_percent(CONTENT_TERMINAL) + delta)
}

def draw_terminal_footer() {
    if (small_button(TERMINAL_ZOOM_OUT_BUTTON, (text = "-5%"))) terminal_zoom_by(-5)
    same_line()
    if (small_button(TERMINAL_ZOOM_IN_BUTTON, (text = "+5%"))) terminal_zoom_by(5)
    same_line()
    text("Zoom {content_zoom_percent(CONTENT_TERMINAL)}%")
    same_line()
    // Input mode first: scroll is always local, but typing needs the
    // controller lease — say so instead of silently eating keys (note 38).
    if (g_client != null && g_client.attached && !g_client.controls) {
        icon("warning", GetTextLineHeight())
        same_line()
        text_colored(TERMINAL_INPUT_OFF, (color = token_warning_color(),
            text = "VIEW ONLY - input lease held elsewhere"))
        set_item_tooltip(TERMINAL_INPUT_OFF_TIP,
            (text = "Scrollback and selection stay local; typing goes to the " +
                "session and needs the controller lease"))
        same_line()
    }
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
    content_zoom_wheel(CONTENT_TERMINAL)
    draw_session_label()
    separator()
    let terminal_style = ImGuiTerminalStyle(
        external_input = true,
        cell_backgrounds = false,
        view_height = max(GetTextLineHeight(),
            GetContentRegionAvail().y - GetFrameHeightWithSpacing()
                - GetStyle().ItemSpacing.y))
    with_font(g_terminal_font, UI_BASE_FONT_SIZE * content_zoom_scale(CONTENT_TERMINAL)) {
        if (g_focus_terminal_pending) {
            // The window raise lands a frame after open; focusing while the
            // window is still unfocused is a no-op that used to eat the
            // request (note 44). Hold the flag until the raise arrives, then
            // set the terminal's OWN focus model: the widget is an
            // InvisibleButton, and SetKeyboardFocusHere's tabbing request
            // falls through it onto the next tab stop (the zoom button) -
            // proven by snapshot, the -5% button ended up focus=true.
            if (IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)) {
                g_terminal_view.focused = true
                g_focus_terminal_pending = false
            }
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
