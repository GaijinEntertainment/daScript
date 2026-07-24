options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

require imgui/imgui_harness
require imgui/imgui_icons
require imgui/imgui_drawlist_builtin
require imgui/imgui_terminal
require imgui/imgui_fonts
require imgui/imgui_docking_builtin
require imgui/imgui_table_builtin
require imgui/imgui_markdown_tree_sitter
require imgui/imgui_text_source_view
require dashv/dashv_boost
require daslib/safe_addr
require daslib/json_boost
require daslib/jobque_boost
require daslib/strings_boost
require live/live_commands
require ./watcher_core.das
require ./repository_core.das
require ./git_graph_model.das
require ./file_inspection.das
require ./inspection_worker.das
require math
require strings
require ./rich_state.das
require ./rich_net.das
require ./rich_inspector_ui.das
require ./rich_git_ui.das
require ./rich_terminal_ui.das
require ./rich_sessions_ui.das
require ./rich_live.das

def setup_default_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let viewport = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, viewport.Size)
    var left_id : uint = 0u
    var right_id : uint = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.24f, left_id, right_id)
    var repositories_id : uint = 0u
    var sessions_id : uint = 0u
    DockBuilderSplitNode(left_id, ImGuiDir.Down, 0.42f,
        sessions_id, repositories_id)
    DockBuilderDockWindow("Sessions & Activity###SessionsActivity", sessions_id)
    DockBuilderDockWindow("Repositories & Worktrees", repositories_id)
    DockBuilderDockWindow("Git Changelist", right_id)
    DockBuilderDockWindow("Git Activity", right_id)
    DockBuilderDockWindow("File Inspector", right_id)
    DockBuilderDockWindow("Settings", right_id)
    DockBuilderDockWindow("Terminal", right_id)
    DockBuilderFinish(dock_id)
    TERMINAL_WIN.pending_raise = true
}

var private MENU_ERROR_POPUP : PopupContextItemState
var private MENU_ERROR_COPY : ToggleState
var private MENU_ERROR_SEND : ToggleState

def private menu_status_busy_count() : int {
    var count = 0
    if (g_client != null) {
        for (repository in g_client.repositories) {
            if (repository.record.id != g_selected_repository_id) continue
            if (repository.refreshing) count++
            for (worktree in repository.worktrees) {
                if (worktree.path != g_selected_worktree_path) continue
                if (worktree.review_loading) count++
                if (worktree.history_loading) count++
                if (worktree.refs_loading) count++
            }
        }
    }
    if (g_inspector.loading
            || g_inspector.prepare.request != InspectorRequestState.idle) count++
    return count
}

// The app-chrome notification cluster: connection, background work,
// Attention, and the latest error live here so async state never renders
// inside a work surface (HERDER_UX.md notification model).
def private draw_menu_status_cluster() {
    let line = GetTextLineHeight()
    let style & = GetStyle()
    let full_error = g_client != null ? g_client.error : ""
    var error_text = full_error
    if (length(error_text) > 64) {
        error_text = slice(error_text, 0, 64) + "..."
    }
    let attention = pending_attention_count()
    let busy = menu_status_busy_count()
    let connected = g_client != null && g_client.connected
    let conn_label = connected ? "watcher" : "reconnecting"
    var width = line + style.ItemSpacing.x + CalcTextSize(conn_label).x
    if (busy > 0) {
        width += line + CalcTextSize("{busy}").x + style.ItemSpacing.x * 3.0f
    }
    if (attention > 0) {
        width += line + CalcTextSize("{attention}").x + style.ItemSpacing.x * 3.0f
    }
    if (!empty(error_text)) {
        width += line * 2.0f + CalcTextSize(error_text).x + style.ItemSpacing.x * 4.0f
    }
    SetCursorPosX(max(GetCursorPosX(),
        GetWindowWidth() - width - style.WindowPadding.x))
    if (!empty(error_text)) {
        icon("warning", line)
        same_line()
        text_colored(MENU_STATUS_ERROR,
            (color = float4(1.0f, 0.35f, 0.30f, 1.0f), text = error_text))
        set_item_tooltip(MENU_STATUS_ERROR_TIP, (text = full_error))
        draw_error_context(MENU_ERROR_POPUP, MENU_ERROR_COPY,
            MENU_ERROR_SEND, "menu_error", "Watcher", full_error)
        same_line()
        if (icon_button_draw("##menu-error-dismiss", "close", line)) {
            g_client.error = ""
        }
        same_line()
    }
    if (attention > 0) {
        if (icon_button_draw("##menu-attention", "bell", line)) {
            open_dock_window(SESSIONS_WIN)
        }
        same_line()
        text(MENU_STATUS_ATTENTION, (text = "{attention}"))
        set_item_tooltip(MENU_STATUS_ATTENTION_TIP,
            (text = "Pending agent attention; click the bell to open Sessions"))
        same_line()
    }
    if (busy > 0) {
        icon("clock", line)
        same_line()
        text_disabled(MENU_STATUS_BUSY, (text = "{busy}"))
        set_item_tooltip(MENU_STATUS_BUSY_TIP,
            (text = "Background Git work in flight for this worktree"))
        same_line()
    }
    icon(connected ? "link" : "unlink", line)
    same_line()
    text_disabled(MENU_STATUS_CONNECTION, (text = conn_label))
}

def draw_main_menu() {
    main_menu_bar(MAIN_MENU) {
        menu(FILE_MENU, (text = "File", enabled = true)) {
            if (menu_label(FILE_EXIT, (text = "Exit", shortcut = "Alt+F4"))) {
                request_exit()
            }
        }
        menu(COMMANDS_MENU, (text = "Commands", enabled = true)) {
            if (menu_label(COMMAND_CODEX_DEBUG,
                    (text = "Debug with Codex...", enabled = g_client != null
                        && g_client.connected && !empty(g_selected_worktree_path)))) {
                prepare_debug_launcher()
                open_popup(SESSION_LAUNCHER_DIALOG)
            }
            if (menu_label(COMMAND_POWERSHELL,
                    (text = "PowerShell", enabled = g_client != null && g_client.connected))) {
                g_client->launch("powershell.exe -NoLogo -NoProfile", g_selected_worktree_path,
                    g_selected_repository_id, g_selected_worktree_path)
            }
        }
        menu(EDIT_MENU, (text = "Edit", enabled = true)) {
            if (menu_label(EDIT_COPY, (text = "Copy", shortcut = "Ctrl+C",
                    enabled = g_terminal_view.selection_active))) copy_terminal_selection()
            if (menu_label(EDIT_PASTE, (text = "Paste", shortcut = "Ctrl+V"))) {
                paste_terminal_clipboard()
            }
            separator(EDIT_SELECTION_SEPARATOR)
            if (menu_label(EDIT_SELECT_ALL, (text = "Select all"))) {
                imgui_terminal_select_all(g_terminal, g_terminal_view)
            }
            if (menu_label(EDIT_CLEAR_SELECTION,
                    (text = "Clear selection", enabled = g_terminal_view.selection_active))) {
                imgui_terminal_clear_selection(g_terminal_view)
            }
        }
        menu(VIEW_MENU, (text = "View", enabled = true)) {
            if (menu_label(VIEW_REPOSITORIES,
                    (text = "Repositories & Worktrees", selected = REPOSITORIES_WIN.open))) {
                open_dock_window(REPOSITORIES_WIN)
            }
            if (menu_label(VIEW_SESSIONS,
                    (text = pending_attention_count() > 0
                        ? "Sessions & Activity  [{pending_attention_count()}]"
                        : "Sessions & Activity", selected = SESSIONS_WIN.open))) {
                open_dock_window(SESSIONS_WIN)
            }
            if (menu_label(VIEW_GIT_CHANGELIST,
                    (text = "Git Changelist", selected = GIT_CHANGELIST_WIN.open))) {
                open_dock_window(GIT_CHANGELIST_WIN)
            }
            if (menu_label(VIEW_GIT_ACTIVITY,
                    (text = "Git Activity", selected = GIT_ACTIVITY_WIN.open))) {
                open_dock_window(GIT_ACTIVITY_WIN)
            }
            if (menu_label(VIEW_FILE_INSPECTOR,
                    (text = "File Inspector", selected = FILE_INSPECTOR_WIN.open))) {
                open_dock_window(FILE_INSPECTOR_WIN)
            }
            if (menu_label(VIEW_TERMINAL,
                    (text = "Terminal", selected = TERMINAL_WIN.open))) {
                open_dock_window(TERMINAL_WIN)
            }
            separator(VIEW_WINDOWS_SEPARATOR)
            menu_item(SHOW_EXITED, (text = "Show exited sessions"))
            g_show_exited = SHOW_EXITED.value
            menu_item(SHOW_TASKS, (text = "Show watcher task sessions"))
            g_show_tasks = SHOW_TASKS.value
            separator(VIEW_ZOOM_SEPARATOR)
            if (menu_label(VIEW_ZOOM_IN, (text = "Zoom in", shortcut = "Ctrl++"))) zoom_by(5)
            if (menu_label(VIEW_ZOOM_OUT, (text = "Zoom out", shortcut = "Ctrl+-"))) zoom_by(-5)
            if (menu_label(VIEW_ZOOM_RESET, (text = "Reset zoom", shortcut = "Ctrl+0"))) {
                g_zoom_percent = 100
            }
            if (menu_label(VIEW_SCROLL_BOTTOM, (text = "Scroll terminal to bottom"))) {
                g_terminal_view.scroll_offset = 0
            }
            separator(VIEW_LAYOUT_SEPARATOR)
            if (menu_label(VIEW_RESET_LAYOUT, (text = "Reset layout"))) reset_layout()
        }
        menu(SETTINGS_MENU, (text = "Settings", enabled = true)) {
            if (menu_label(OPEN_SETTINGS, (text = "Open Settings"))) {
                open_dock_window(SETTINGS_WIN)
            }
        }
        draw_menu_status_cluster()
    }
}

[export]
def init() {
    harness_init("dasHerd", 1280, 800)
    harness_maximize_window()
    inspector_ui_init()
    git_ui_init()
    sessions_ui_init()
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable
    g_fonts = load_unicode_font_roles(14.0f, g_font_sources)
    g_terminal_font = g_fonts.monospace
    reset_terminal()
    create_job_que()
    g_jobque_owned = true
    inspector_prepare_worker_start(g_prepare_worker)
    g_selected_worktree_path = get_das_root()
    SETTINGS_WIN.open = false
    let port = clamp(to_int(option_value("--port=", "9191")), 1024, 65535)
    g_token = option_value("--token=")
    g_url = "ws://127.0.0.1:{port}/ws?token={g_token}"
    g_client = new WatcherRichClient()
    if (empty(g_token)) {
        g_client.error = "Pass the watcher token with --token=<token>"
        return
    }
    let result = g_client->init(g_url)
    if (result != 0) {
        g_client.error = "watcher connection failed ({result})"
    }
}

[export]
def update() {
    // The native frame gate returns false while the window is minimized.
    // Keep the transport alive outside that gate so heartbeats, controller
    // renewal, and queued PTY output continue until the window is restored.
    if (g_client != null) g_client->pump()
    if (!harness_begin_frame()) return
    service_sha_copy()
    drain_inspector_prepare_events()
    harness_new_frame()
    g_ui_frame++
    let tab_signature = ("pr={GIT_PR_TAB.selected} hist={GIT_HISTORY_TAB.selected} " +
        "tree={GIT_TREE_TAB.selected} diff={INSPECTOR_DIFF_TAB.selected} " +
        "view={INSPECTOR_VIEW_TAB.selected} wt={g_selected_worktree_path}")
    if (tab_signature != g_ui_tab_signature) {
        ui_event("state-change {tab_signature}")
        g_ui_tab_signature = tab_signature
    }
    GetStyle().FontScaleMain = float(g_zoom_percent) / 100.0f
    if (g_open_terminal_pending) {
        g_open_terminal_pending = false
        TERMINAL_WIN.open = true
        TERMINAL_WIN.pending_raise = true
    }
    if (g_dock_reset_pending) {
        g_dock_reset_pending = false
        DOCK_ROOT.pending_reset = true
    }
    draw_main_menu()
    dockspace(DOCK_ROOT, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        if (!DOCK_ROOT.has_initial_layout && DOCK_ROOT.dock_id != 0u) {
            if (g_layout_force_default) {
                setup_default_layout(DOCK_ROOT.dock_id)
                DOCK_ROOT.has_initial_layout = true
                g_layout_probe_complete = true
                g_layout_force_default = false
            } elif (g_layout_probe_complete) {
                let restored_layout = (REPOSITORIES_WIN.docked ||
                    SESSIONS_WIN.docked || GIT_ACTIVITY_WIN.docked ||
                    GIT_CHANGELIST_WIN.docked || FILE_INSPECTOR_WIN.docked ||
                    TERMINAL_WIN.docked)
                if (restored_layout) {
                    DOCK_ROOT.has_initial_layout = true
                } else {
                    setup_default_layout(DOCK_ROOT.dock_id)
                    DOCK_ROOT.has_initial_layout = true
                }
            } else {
                g_layout_probe_complete = true
            }
        }
        draw_repositories_dock()
        draw_sessions_dock()
        draw_git_activity_dock()
        draw_git_changelist_dock()
        draw_file_inspector_dock()
        draw_terminal_dock()
        draw_settings_dock()
    }
    draw_session_launcher_popup()
    harness_end_frame()
}

[export]
def shutdown() {
    let prepare_stopped = inspector_prepare_worker_shutdown(g_prepare_worker)
    if (g_client != null) {
        g_client->close()
        g_client->cleanup()
        delete g_client.sessions
        delete g_client.repositories
        delete g_client.mailbox
        delete g_client.bundles
        delete g_client.git_failure_revisions
        delete g_client.agent_mark_files
        unsafe { delete g_client }
        g_client = null
    }
    release_inspector_content()
    delete g_tree_model.nodes
    delete g_tree_model.edges
    text_source_view_dispose(g_inspector.old_view)
    text_source_view_dispose(g_inspector.new_view)
    text_source_view_dispose(g_inspector.view)
    terminal_viewport_snapshot_dispose(g_terminal_cache.snapshot)
    terminal_dispose(g_terminal)
    delete g_font_sources
    harness_shutdown()
    if (g_jobque_owned && prepare_stopped) {
        destroy_job_que()
        g_jobque_owned = false
    }
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        let frame_started = ref_time_ticks()
        update()
        harness_maybe_collect_gc()
        let frame_usec = int64(get_time_usec(frame_started))
        if (frame_usec < 16000l) {
            sleep(uint((16000l - frame_usec) / 1000l))
        }
    }
    shutdown()
}
