options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_sessions_ui

require imgui/imgui_harness
require imgui/imgui_icons
require imgui/imgui_drawlist_builtin
require imgui/imgui_terminal
require imgui/imgui_fonts
require imgui/imgui_docking_builtin
require imgui/imgui_table_builtin
require imgui/imgui_markdown_tree_sitter
require imgui/imgui_text_source_view
require daslib/safe_addr
require daslib/json_boost
require daslib/jobque_boost
require daslib/strings_boost
require live/live_commands
require ./watcher_core.das
require ./repository_core.das
require ./git_graph_model.das
require ./file_inspection.das
require strings
require ./rich_state.das
require ./rich_net.das
require ./rich_focus.das
require ./rich_inspector_ui.das

var SESSION_ROW : table<int; ToggleState>

var ATTENTION_MESSAGE_ROW : table<int; ToggleState>

var ATTENTION_TARGET_ROW : table<int; ToggleState>

var BUNDLE_ROW : table<int; ToggleState>

var BUNDLE_PARTICIPANT_ROW : table<int; ToggleState>

var BUNDLE_FILE_ROW : table<int; ToggleState>

def draw_selected_session_context() {
    if (g_client == null || empty(g_client.selected_id)) return
    for (session in g_client.sessions) {
        if (session.id != g_client.selected_id) continue
        let purpose = !empty(session.purpose) ? to_upper(session.purpose) : to_upper(session.kind)
        text("{purpose}  {session.id}")
        text_disabled("Origin: {session.worktree_path}")
        if (!empty(session.task_context)) {
            text_wrapped((text = session.task_context))
        }
        return
    }
}

def draw_review_bundles() {
    text("Review bundles ({g_client != null ? length(g_client.bundles) : 0})")
    child(REVIEW_BUNDLES_CHILD, (text = "review bundles", size = float2(0.0f, 150.0f),
            child_flags = ImGuiChildFlags.Borders, window_flags = ImGuiWindowFlags.None)) {
        if (g_client == null || empty(g_client.bundles)) {
            text_disabled("The agent has not declared a review bundle yet.")
            return
        }
        var participant_row = 0
        var file_row = 0
        for (bundle_index in range(length(g_client.bundles))) {
            let bundle & = unsafe(g_client.bundles[bundle_index])
            BUNDLE_ROW[bundle_index].value = false
            if (selectable(BUNDLE_ROW[bundle_index],
                    (text = "[{to_upper(bundle.status)}] {bundle.title}##{bundle.id}"))) {
                let _ = activate_bundle_focus(bundle)
            }
            BUNDLE_ROW[bundle_index].value = false
            if (!empty(bundle.summary)) text_disabled((text = bundle.summary))
            for (participant in bundle.participants) {
                let files = length(participant.files)
                BUNDLE_PARTICIPANT_ROW[participant_row].value = (participant.repository_id
                    == g_selected_repository_id && participant.worktree_path
                    == g_selected_worktree_path)
                if (selectable(BUNDLE_PARTICIPANT_ROW[participant_row],
                        (text = "  {participant.role}  {participant.worktree_path}  ({files} files)##{bundle.id}:{participant_row}"))) {
                    if (!activate_bundle_focus(bundle, participant.repository_id,
                            participant.worktree_path)) {
                        select_bundle_participant(participant)
                    }
                }
                participant_row++
                for (file in participant.files) {
                    BUNDLE_FILE_ROW[file_row].value = false
                    if (selectable(BUNDLE_FILE_ROW[file_row],
                            (text = "      {file.role}  {file.path}##{bundle.id}:file:{file_row}"))) {
                        if (!activate_bundle_focus(bundle, participant.repository_id,
                                participant.worktree_path, file.path)) {
                            select_bundle_participant(participant)
                            g_client->inspect_file(participant.repository_id,
                                participant.worktree_path, file.path,
                                "working", "full", "")
                            FILE_INSPECTOR_WIN.open = true
                            FILE_INSPECTOR_WIN.pending_raise = true
                        }
                    }
                    BUNDLE_FILE_ROW[file_row].value = false
                    file_row++
                }
            }
        }
    }
}

def pending_attention_count() : int {
    var count = 0
    if (g_client != null) {
        for (message in g_client.mailbox) {
            if (message.direction == "outbox" && message.status == "new") count++
        }
    }
    return count
}

def draw_session_list() {
    let connection = g_client != null && g_client.connected ? "connected" : "disconnected"
    text("Watcher: {connection}")
    if (small_button(REFRESH_SESSIONS, (text = "Refresh")) && g_client != null) {
        g_client->refresh()
    }
    same_line()
    if (small_button(TERMINATE_SESSION, (text = "Terminate")) && g_client != null) {
        g_client->terminate_selected()
    }
    separator()
    draw_selected_session_context()
    child(SESSIONS_CHILD, (text = "sessions", size = float2(0.0f, 120.0f),
            child_flags = ImGuiChildFlags.Borders, window_flags = ImGuiWindowFlags.None)) {
        var visible_sessions = 0
        if (g_client != null) {
            for (index in range(length(g_client.sessions))) {
                let session = g_client.sessions[index]
                if ((!g_show_tasks && session_is_task(session.kind))
                        || (!g_show_exited && session.state == "exited")) continue
                visible_sessions++
                var state_glyph = "clock"
                if (session.state == "running") {
                    state_glyph = "play"
                } elif (session.state == "terminating") {
                    state_glyph = "stop"
                } elif (session.state == "failed") {
                    state_glyph = "warning"
                } elif (session.state == "exited") {
                    state_glyph = session.exit_code == 0l ? "check" : "warning"
                }
                icon(state_glyph, GetTextLineHeight())
                same_line()
                let row_text = "{session.command_line}##{session.id}"
                if (selectable(SESSION_ROW[index], (text = row_text))) {
                    g_client->attach_session(session.id)
                    g_open_terminal_pending = true
                    g_focus_terminal_pending = true
                }
                if (IsItemHovered()) {
                    set_tooltip(SESSION_STATE_TOOLTIP,
                        "{session.state}  pid {session.pid}  {session.kind}")
                }
                SESSION_ROW[index].value = session.id == g_client.selected_id
            }
        }
        if (visible_sessions == 0) {
            text_disabled(NO_SESSION_LABEL,
                (text = g_show_exited ? "No sessions" : "No active sessions"))
        }
    }
    text_disabled("Exited sessions are retained and hidden by default.")
    separator()
    draw_review_bundles()
    separator()
    let attention_count = pending_attention_count()
    text("Attention ({attention_count})")
    text_disabled("Agent notes never navigate until you click them.")
    child(ATTENTION_CHILD, (text = "attention", size = float2(0.0f, 0.0f),
            child_flags = ImGuiChildFlags.Borders, window_flags = ImGuiWindowFlags.None)) {
        if (g_client != null) {
            var target_row = 0
            for (message_index in range(length(g_client.mailbox))) {
                let message & = unsafe(g_client.mailbox[message_index])
                if (message.direction != "outbox") continue
                ATTENTION_MESSAGE_ROW[message_index].value = false
                if (selectable(ATTENTION_MESSAGE_ROW[message_index],
                        (text = "{message.subject}##{message.id}"))
                        && !empty(message.focus.targets)) {
                    activate_focus_target(message, 0)
                }
                ATTENTION_MESSAGE_ROW[message_index].value = false
                let selected_message = message.id == g_active_focus_message
                for (target_index in range(length(message.focus.targets))) {
                    let target & = unsafe(message.focus.targets[target_index])
                    let target_repository = watcher_focus_repository(message.focus, target)
                    let repository_hint = (!empty(target.repository_id)
                        ? "[{target_repository}] " : "")
                    ATTENTION_TARGET_ROW[target_row].value = (selected_message
                        && target_index == g_active_focus_target)
                    if (selectable(ATTENTION_TARGET_ROW[target_row],
                            (text = "  {repository_hint}{target.file_path}##{message.id}:{target_index}"))) {
                        activate_focus_target(message, target_index)
                    }
                    target_row++
                }
            }
        }
    }
}

def sessions_ui_init() {
    pass
}

def draw_sessions_dock() {
    let attention_count = pending_attention_count()
    let sessions_title = (attention_count > 0
        ? "Sessions & Activity  [! {attention_count}]###SessionsActivity"
        : "Sessions & Activity###SessionsActivity")
    dock_window(SESSIONS_WIN.PUBLIC, (text = sessions_title, closable = true,
            flags = ImGuiWindowFlags.None)) {
        draw_session_list()
    }
}

