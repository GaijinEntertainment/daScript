options gen2
options _comment_hygiene = true
options gc
options persistent_heap

require imgui/imgui_harness
require imgui/imgui_terminal
require imgui/imgui_fonts
require daslib/safe_addr
require math
require strings

var private g_terminal = Terminal()
var private g_terminal_view = ImGuiTerminalState()
var private g_terminal_cache = ImGuiTerminalCache()
var private g_zoom_percent = 150
var private g_terminal_font : ImFont?
var private g_font_sources : array<string>
var private g_status = "Ready"

def private zoom_by(delta : int) {
    g_zoom_percent = clamp(g_zoom_percent + delta, 50, 300)
}

def terminal_command() : string {
    let prefix = "--terminal-command="
    for (arg in get_command_line_arguments()) {
        if (starts_with(arg, prefix)) return slice(arg, length(prefix))
    }
    return "powershell.exe -NoLogo -NoProfile"
}

[export]
def init() {
    harness_init("embedded_terminal", 960, 640)
    harness_maximize_window()
    g_terminal_font = load_unicode_ui_font(14.0f, g_font_sources)
    var inscope launched <- terminal_launch(terminal_command(), "", 100, 30)
    g_terminal := launched
}

[export]
def update() {
    if (!harness_begin_frame()) return
    // Keep live control responsive even when the child produces output faster
    // than one frame can render. Remaining bytes stay in the PTY pipe.
    for (_ in range(64)) {
        if (terminal_poll(g_terminal) != TerminalPollStatus.data) break
    }
    harness_new_frame()
    GetStyle().FontScaleMain = float(g_zoom_percent) / 100.0f
    let viewport = GetMainViewport()
    SetNextWindowPos(viewport.WorkPos, ImGuiCond.Always)
    SetNextWindowSize(viewport.WorkSize, ImGuiCond.Always)
    window(TERMINAL_WIN, (text = "dasHerd terminal", closable = false,
            flags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove)) {
        let io & = unsafe(GetIO())
        if (IsWindowHovered(ImGuiHoveredFlags.RootAndChildWindows) && io.KeyCtrl) {
            if (io.MouseWheel > 0.0f) {
                zoom_by(5)
            } elif (io.MouseWheel < 0.0f) {
                zoom_by(-5)
            }
        }
        let terminal_style = ImGuiTerminalStyle(
            view_height = max(GetTextLineHeight(),
                GetContentRegionAvail().y - GetFrameHeightWithSpacing()
                    - GetStyle().ItemSpacing.y))
        with_font(g_terminal_font) {
            let _ = imgui_terminal("shell", g_terminal, g_terminal_view,
                g_terminal_cache, terminal_style)
        }
        separator()
        if (small_button(TERMINAL_EDIT_MENU, (text = "Edit"))) {
            open_popup("terminal_edit_menu")
        }
        popup_window(TERMINAL_EDIT_POPUP,
                     (str_id = "terminal_edit_menu",
                      flags = ImGuiWindowFlags.AlwaysAutoResize)) {
            if (menu_label(TERMINAL_COPY,
                    (text = "Copy", shortcut = "Ctrl+C",
                     enabled = g_terminal_view.selection_active))) {
                g_status = (imgui_terminal_copy_selection(g_terminal, g_terminal_view)
                    ? "Selection copied" : "Nothing selected")
            }
            if (menu_label(TERMINAL_PASTE,
                    (text = "Paste", shortcut = "Ctrl+V"))) {
                g_status = (imgui_terminal_paste_clipboard(g_terminal)
                    ? "Clipboard pasted" : "Clipboard unavailable")
            }
            separator()
            if (menu_label(TERMINAL_SELECT_ALL, (text = "Select all"))) {
                imgui_terminal_select_all(g_terminal, g_terminal_view)
            }
            if (menu_label(TERMINAL_CLEAR_SELECTION,
                    (text = "Clear selection",
                     enabled = g_terminal_view.selection_active))) {
                imgui_terminal_clear_selection(g_terminal_view)
            }
        }
        same_line()
        if (small_button(TERMINAL_VIEW_MENU, (text = "View"))) {
            open_popup("terminal_view_menu")
        }
        popup_window(TERMINAL_VIEW_POPUP,
                     (str_id = "terminal_view_menu",
                      flags = ImGuiWindowFlags.AlwaysAutoResize)) {
            if (menu_label(TERMINAL_ZOOM_IN, (text = "Zoom in", shortcut = "Ctrl++"))) {
                zoom_by(5)
            }
            if (menu_label(TERMINAL_ZOOM_OUT, (text = "Zoom out", shortcut = "Ctrl+-"))) {
                zoom_by(-5)
            }
            if (menu_label(TERMINAL_ZOOM_RESET, (text = "Reset zoom", shortcut = "Ctrl+0"))) {
                g_zoom_percent = 100
            }
            separator()
            if (menu_label(TERMINAL_SCROLL_BOTTOM, (text = "Scroll to bottom"))) {
                g_terminal_view.scroll_offset = 0
            }
        }
        same_line()
        text("Zoom")
        same_line()
        SetNextItemWidth(130.0f)
        if (edit_slider_int(safe_addr(g_zoom_percent),
                (id = "TERMINAL_ZOOM_SLIDER", text = "##zoom", v_min = 50,
                 v_max = 300, format = "%d%%"))) {
            g_zoom_percent = clamp(((g_zoom_percent + 2) / 5) * 5, 50, 300)
        }
        same_line()
        text_show(TERMINAL_ZOOM, "{g_zoom_percent}")
        same_line()
        text_disabled("{g_terminal_view.scrollback_rows} history lines - {g_status}")
    }
    harness_end_frame()
}

[export]
def shutdown() {
    if (!terminal_process_exited(g_terminal)) {
        terminal_terminate(g_terminal)
    }
    terminal_dispose(g_terminal)
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
