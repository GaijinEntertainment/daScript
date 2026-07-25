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
require ./herd_sessions.das
require ./git_graph_model.das
require ./file_inspection.das
require strings
require ./rich_state.das
require ./rich_net.das
require ./rich_focus.das
require ./rich_inspector_ui.das

var SESSION_ROW : table<int; ToggleState>

var HERD_CARD_ROW : table<int; ToggleState>

var HERD_ICON_JOIN : table<int; EmptyMarkerState>

var HERD_STATE_JOIN : table<int; EmptyMarkerState>

var HERD_CONFLICT_JOIN : table<int; EmptyMarkerState>

var HERD_MENU_ADD_SATELLITE : table<int; ToggleState>

var HERD_ACTION_JOIN : table<int; EmptyMarkerState>

var HERD_CARD_DETAIL : table<int; NarrativeState>

var HERD_CARD_POPUP : table<int; PopupContextItemState>

var HERD_MENU_RELAUNCH : table<int; ToggleState>

var HERD_MENU_RENAME : table<int; ToggleState>

var HERD_MENU_CLOSE : table<int; ToggleState>

var HERD_RELAUNCH_BUTTON : table<int; ClickState>

var SESSION_STATE_JOIN : table<int; EmptyMarkerState>

var g_herd_rename_id : string

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

def private herd_age_label(updated_wall_s : int64) : string {
    let age = int64(get_clock()) - updated_wall_s
    if (age < 60l) return "now"
    if (age < 3600l) return "{age / 60l}m ago"
    if (age < 86400l) return "{age / 3600l}h ago"
    return "{age / 86400l}d ago"
}

def private attach_herd_card(record : HerdSession) {
    if (g_client == null || empty(record.pty_session_id)) return
    ui_event("herd-card-attach {record.id}")
    g_client->attach_session(record.pty_session_id)
    g_open_terminal_pending = true
    g_focus_terminal_pending = true
}

def draw_herd_cards() {
    if (g_client == null) return
    let line = GetTextLineHeight()
    var visible = 0
    for (index in range(length(g_client.herd_sessions))) {
        let record & = unsafe(g_client.herd_sessions[index])
        if (record.state == "closed") continue
        visible++
        var profile_agent = false
        for (profile in g_client.herd_profiles) {
            if (profile.id == record.profile_id) {
                profile_agent = profile.agent
                break
            }
        }
        icon(profile_agent ? "bolt" : "chevron-right", line)
        same_line(HERD_ICON_JOIN[index])
        var state_glyph = "clock"
        if (record.state == "running") {
            state_glyph = "play"
        } elif (record.state == "exited") {
            state_glyph = (record.launch_count == 0 ? "clock"
                : (record.exit_code == 0l ? "check" : "warning"))
        }
        icon(state_glyph, line)
        same_line(HERD_STATE_JOIN[index])
        if (record.conflicted) {
            icon("warning", line)
            same_line(HERD_CONFLICT_JOIN[index])
        }
        let unread_hint = record.unread_outbox > 0 ? "  [{record.unread_outbox}]" : ""
        HERD_CARD_ROW[index].value = (!empty(record.pty_session_id)
            && record.pty_session_id == g_client.selected_id)
        // AllowOverlap or the row swallows clicks aimed at the relaunch
        // icon button drawn over its right-hand side.
        if (selectable(HERD_CARD_ROW[index],
                (text = "{record.name}{unread_hint}##herd:{record.id}",
                 flags = ImGuiSelectableFlags.AllowOverlap))) {
            attach_herd_card(record)
        }
        if (IsItemHovered()) {
            let conflict_hint = (record.conflicted
                ? "  |  another session attends this worktree" : "")
            set_tooltip(HERD_CARD_TOOLTIP,
                "{record.state}  {record.worktree_path}{conflict_hint}")
        }
        popup_context_item(HERD_CARD_POPUP[index],
                (str_id = "herdcard:{record.id}",
                 flags = ImGuiPopupFlags.MouseButtonRight)) {
            if (menu_item(HERD_MENU_RELAUNCH[index], (text = "Relaunch",
                    shortcut = "", enabled = record.state == "exited"))) {
                g_client->request_herd_relaunch(record.id)
            }
            if (menu_item(HERD_MENU_RENAME[index], (text = "Rename...",
                    shortcut = "", enabled = true))) {
                g_herd_rename_id = record.id
                HERD_RENAME_INPUT.pending_value = record.name
                HERD_RENAME_INPUT.has_pending = true
                open_popup(HERD_RENAME_DIALOG)
            }
            if (menu_item(HERD_MENU_ADD_SATELLITE[index],
                    (text = "Add selected worktree", shortcut = "",
                     enabled = !empty(g_selected_worktree_path)))) {
                g_client->request_herd_add_satellite(record.id,
                    g_selected_repository_id, g_selected_worktree_path)
            }
            if (menu_item(HERD_MENU_CLOSE[index], (text = "Close session",
                    shortcut = "", enabled = true))) {
                open_destructive_confirm("herd_close", record.id,
                    "Close session '{record.name}' and terminate its PTY?")
            }
        }
        if (record.state == "exited") {
            same_line(HERD_ACTION_JOIN[index])
            if (icon_button(HERD_RELAUNCH_BUTTON[index], (glyph = "refresh",
                    tip = "Relaunch this session", size = line))) {
                g_client->request_herd_relaunch(record.id)
            }
        }
        let satellite_hint = (!empty(record.satellites)
            ? "  +{length(record.satellites)} repos" : "")
        text_disabled(HERD_CARD_DETAIL[index],
            (text = "      {record.profile_id} | {record.workflow} | " +
                "{base_name(record.worktree_path)}{satellite_hint} | " +
                "{herd_age_label(record.updated_wall_s)}"))
    }
    if (visible == 0) {
        text_disabled(NO_HERD_LABEL,
            (text = "No sessions yet. Commands > New Session..."))
    }
}

var g_confirm_kind : string
var g_confirm_target : string
var g_confirm_text : string

// Destructive actions (note 27) route through one confirmation modal. The
// target is captured at open time and re-checked at confirm so a selection
// change while the dialog sits open can't redirect the action.
def open_destructive_confirm(kind, target, description : string) {
    g_confirm_kind = kind
    g_confirm_target = target
    g_confirm_text = description
    open_popup(CONFIRM_DIALOG)
}

def draw_destructive_confirm_popup() {
    popup_modal(CONFIRM_DIALOG.PUBLIC, (text = "Are you sure?",
            closable = true, flags = ImGuiWindowFlags.None)) {
        text(CONFIRM_TEXT, (text = g_confirm_text))
        text_disabled(CONFIRM_HINT, (text = "This cannot be undone."))
        if (button(CONFIRM_OK, (text = "Confirm", enabled = g_client != null))) {
            if (g_confirm_kind == "terminate") {
                if (g_client.selected_id == g_confirm_target) {
                    g_client->terminate_selected()
                }
            } elif (g_confirm_kind == "herd_close") {
                g_client->request_herd_close(g_confirm_target)
            }
            close_current_popup(CONFIRM_DIALOG)
        }
        same_line()
        if (button(CONFIRM_CANCEL, (text = "Cancel"))) {
            close_current_popup(CONFIRM_DIALOG)
        }
    }
}

def draw_herd_rename_popup() {
    popup_modal(HERD_RENAME_DIALOG.PUBLIC, (text = "Rename session",
            closable = true, flags = ImGuiWindowFlags.None)) {
        input_text_growable(HERD_RENAME_INPUT, (text = "Name"))
        if (button(HERD_RENAME_OK, (text = "Rename",
                enabled = g_client != null && !empty(g_herd_rename_id)
                    && !empty(strip(HERD_RENAME_INPUT.value))))) {
            g_client->request_herd_rename(g_herd_rename_id,
                strip(HERD_RENAME_INPUT.value))
            close_current_popup(HERD_RENAME_DIALOG)
        }
        same_line()
        if (button(HERD_RENAME_CANCEL, (text = "Cancel"))) {
            close_current_popup(HERD_RENAME_DIALOG)
        }
    }
}

// A PTY row whose session is a live herd card's terminal is redundant in the
// list — the card is its surface. The plumbing View toggle shows everything.
def private herd_owns_pty(session_id : string) : bool {
    if (g_client == null) return false
    for (record in g_client.herd_sessions) {
        if (record.state != "closed" && record.pty_session_id == session_id) {
            return true
        }
    }
    return false
}

def draw_session_list() {
    let connection = g_client != null && g_client.connected ? "connected" : "disconnected"
    text("Watcher: {connection}")
    same_line()
    if (icon_button(REFRESH_SESSIONS, (glyph = "refresh",
            tip = "Refresh the session list",
            size = GetTextLineHeight())) && g_client != null) {
        g_client->refresh()
    }
    same_line()
    if (icon_button(TERMINATE_SESSION, (glyph = "stop",
            tip = "Terminate the attached session",
            size = GetTextLineHeight())) && g_client != null
            && !empty(g_client.selected_id)) {
        open_destructive_confirm("terminate", g_client.selected_id,
            "Terminate session {g_client.selected_id}?")
    }
    separator()
    draw_herd_cards()
    separator()
    draw_selected_session_context()
    child(SESSIONS_CHILD, (text = "sessions", size = float2(0.0f, 120.0f),
            child_flags = ImGuiChildFlags.Borders, window_flags = ImGuiWindowFlags.None)) {
        var visible_sessions = 0
        if (g_client != null) {
            for (index in range(length(g_client.sessions))) {
                let session = g_client.sessions[index]
                if ((!g_show_tasks && session_is_task(session.kind))
                        || (!g_show_exited && session.state == "exited")
                        || (!g_show_tasks && herd_owns_pty(session.id))) continue
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
                same_line(SESSION_STATE_JOIN[index])
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
    HERD_RENAME_INPUT.capacity = 1024
}

def draw_sessions_dock() {
    let attention_count = pending_attention_count()
    let sessions_title = (attention_count > 0
        ? "Sessions & Activity  [! {attention_count}]###SessionsActivity"
        : "Sessions & Activity###SessionsActivity")
    dock_window(SESSIONS_WIN.PUBLIC, (text = sessions_title, closable = true,
            flags = ImGuiWindowFlags.None)) {
        window_zoom_scope("sessions") {
            draw_session_list()
        }
    }
}

