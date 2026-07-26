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
require ./rich_commands.das
require ./rich_tokens.das

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

var HERD_MENU_DELETE : table<int; ToggleState>

var HERD_RELAUNCH_BUTTON : table<int; ClickState>

var SESSION_STATE_JOIN : table<int; EmptyMarkerState>

var g_herd_rename_id : string

var ATTENTION_MESSAGE_ROW : table<int; ToggleState>

var ATTENTION_TARGET_ROW : table<int; ToggleState>

var BUNDLE_ROW : table<int; ToggleState>

var BUNDLE_PARTICIPANT_ROW : table<int; ToggleState>

var BUNDLE_FILE_ROW : table<int; ToggleState>

var HERD_ATT_HEADER : table<int; NarrativeState>

var HERD_BUNDLE_HEADER : table<int; NarrativeState>

var ERROR_HISTORY_ROW : table<int; NarrativeState>

def draw_selected_session_context() {
    if (g_client == null || empty(g_client.selected_id)) return
    for (session in g_client.sessions) {
        if (session.id != g_client.selected_id) continue
        // Names over plumbing ids (note 46): prefer the herd card's name,
        // then the display name; the raw session id lives in the tooltip.
        var title = session.display_name
        for (record in g_client.herd_sessions) {
            if (record.pty_session_id == session.id) {
                title = record.name
                break
            }
        }
        if (empty(title)) {
            title = session.id
        }
        let purpose = !empty(session.purpose) ? session.purpose : session.kind
        text("{title}")
        if (IsItemHovered()) {
            set_tooltip(CONTEXT_ID_TOOLTIP, "{session.id}  |  {purpose}")
        }
        text_disabled("{purpose} | {session.state} | {base_name(session.worktree_path)}")
        if (IsItemHovered()) {
            set_tooltip(CONTEXT_ORIGIN_TOOLTIP, session.worktree_path)
        }
        if (!empty(session.task_context)) {
            text_wrapped((text = session.task_context))
        }
        return
    }
}

// Frame-local row counters shared by the per-card subtrees and the unowned
// fallback sections — widget indices must stay unique across both.
var g_bundle_participant_row : int
var g_bundle_file_row : int
var g_attention_target_row : int

// session_filter "" draws every bundle EXCEPT card-owned ones (the fallback
// section); a session id draws that card's own bundles (note 19 subtree).
def private draw_bundle_rows(session_filter : string) {
    return if (g_client == null)
    for (bundle_index in range(length(g_client.bundles))) {
        let bundle & = unsafe(g_client.bundles[bundle_index])
        if (empty(session_filter)) {
            continue if (herd_owns_pty(bundle.session_id))
        } elif (bundle.session_id != session_filter) {
            continue
        }
        BUNDLE_ROW[bundle_index].value = false
        if (selectable(BUNDLE_ROW[bundle_index],
                (text = "[{to_upper(bundle.status)}] {bundle.title}##{bundle.id}"))) {
            let _ = activate_bundle_focus(bundle)
        }
        BUNDLE_ROW[bundle_index].value = false
        if (!empty(bundle.summary)) text_disabled((text = bundle.summary))
        for (participant in bundle.participants) {
            let files = length(participant.files)
            BUNDLE_PARTICIPANT_ROW[g_bundle_participant_row].value = (participant.repository_id
                == g_selected_repository_id && participant.worktree_path
                == g_selected_worktree_path)
            if (selectable(BUNDLE_PARTICIPANT_ROW[g_bundle_participant_row],
                    (text = "  {participant.role}  {participant.worktree_path}  ({files} files)##{bundle.id}:{g_bundle_participant_row}"))) {
                if (!activate_bundle_focus(bundle, participant.repository_id,
                        participant.worktree_path)) {
                    select_bundle_participant(participant)
                }
            }
            g_bundle_participant_row++
            for (file in participant.files) {
                BUNDLE_FILE_ROW[g_bundle_file_row].value = false
                if (selectable(BUNDLE_FILE_ROW[g_bundle_file_row],
                        (text = "      {file.role}  {file.path}##{bundle.id}:file:{g_bundle_file_row}"))) {
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
                BUNDLE_FILE_ROW[g_bundle_file_row].value = false
                g_bundle_file_row++
            }
        }
    }
}

def private draw_attention_rows(session_filter : string) {
    return if (g_client == null)
    for (message_index in range(length(g_client.mailbox))) {
        let message & = unsafe(g_client.mailbox[message_index])
        if (message.direction != "outbox") continue
        if (empty(session_filter)) {
            continue if (herd_owns_pty(message.session_id))
        } elif (message.session_id != session_filter) {
            continue
        }
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
            ATTENTION_TARGET_ROW[g_attention_target_row].value = (selected_message
                && target_index == g_active_focus_target)
            if (selectable(ATTENTION_TARGET_ROW[g_attention_target_row],
                    (text = "  {repository_hint}{target.file_path}##{message.id}:{target_index}"))) {
                activate_focus_target(message, target_index)
            }
            g_attention_target_row++
        }
    }
}

def private session_bundle_count(session_id : string) : int {
    var count = 0
    if (g_client != null && !empty(session_id)) {
        for (bundle in g_client.bundles) {
            if (bundle.session_id == session_id) count++
        }
    }
    return count
}

def private session_attention_count(session_id : string) : int {
    var count = 0
    if (g_client != null && !empty(session_id)) {
        for (message in g_client.mailbox) {
            if (message.direction == "outbox"
                    && message.session_id == session_id) count++
        }
    }
    return count
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
    if (g_client == null) return
    ui_event("herd-card-attach {record.id}")
    // A session pick IS a worktree pick (note 48): aim the git surfaces at
    // the card's tree whether or not a terminal can attach.
    if (!empty(record.repository_id) && !empty(record.worktree_path)
            && (g_selected_repository_id != record.repository_id
                || g_selected_worktree_path != record.worktree_path)) {
        g_selected_repository_id = record.repository_id
        g_selected_worktree_path = record.worktree_path
        g_selection_explicit = true
        g_client->refresh_repository(record.repository_id)
        g_client->request_repository_review(record.repository_id,
            record.worktree_path)
    }
    // Attach only while the PTY still exists on the watcher. A stale id
    // bounces ("unknown session") and the auto-attach yanks the selection
    // straight back (note 43).
    if (empty(record.pty_session_id)) return
    var pty_exists = false
    for (session in g_client.sessions) {
        if (session.id == record.pty_session_id) {
            pty_exists = true
            break
        }
    }
    if (!pty_exists) return
    g_client->attach_session(record.pty_session_id)
    g_open_terminal_pending = true
    g_focus_terminal_pending = true
}

def draw_herd_cards() {
    if (g_client == null) return
    let line = GetTextLineHeight()
    var visible = 0
    // Alive sessions first; registry order within each state group. Widget
    // state stays keyed by the registry index, so reordering cannot swap
    // per-card identity when a session exits.
    var inscope order : array<int>
    order |> reserve(length(g_client.herd_sessions))
    for (registry_index in range(length(g_client.herd_sessions))) {
        order |> push(registry_index)
    }
    sort(order) $(a, b) {
        let a_rank = g_client.herd_sessions[a].state == "running" ? 0 : 1
        let b_rank = g_client.herd_sessions[b].state == "running" ? 0 : 1
        return a_rank != b_rank ? a_rank < b_rank : a < b
    }
    for (index in order) {
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
        if (IsItemHovered()) {
            set_tooltip(HERD_ICON_TOOLTIP, profile_agent
                ? "Agent session (profile '{record.profile_id}')"
                : "Shell session (profile '{record.profile_id}')")
        }
        same_line(HERD_ICON_JOIN[index])
        var state_glyph = "clock"
        var state_hint = "never launched"
        if (record.state == "running") {
            state_glyph = "play"
            state_hint = "running"
        } elif (record.state == "exited") {
            if (record.launch_count == 0) {
                state_glyph = "clock"
                state_hint = "created, never launched"
            } elif (record.exit_code == 0l) {
                state_glyph = "check"
                state_hint = "exited cleanly"
            } else {
                state_glyph = "warning"
                state_hint = "exited with code {record.exit_code}"
            }
        }
        icon(state_glyph, line)
        if (IsItemHovered()) {
            set_tooltip(HERD_ICON_TOOLTIP, state_hint)
        }
        same_line(HERD_STATE_JOIN[index])
        if (record.conflicted) {
            icon("warning", line)
            if (IsItemHovered()) {
                set_tooltip(HERD_ICON_TOOLTIP,
                    "Another session attends this worktree")
            }
            same_line(HERD_CONFLICT_JOIN[index])
        }
        let unread_hint = record.unread_outbox > 0 ? "  [{record.unread_outbox}]" : ""
        HERD_CARD_ROW[index].value = (!empty(record.pty_session_id)
            && record.pty_session_id == g_client.selected_id)
        // AllowOverlap or the row swallows clicks aimed at the relaunch
        // icon button drawn over its right-hand side. Dead sessions read
        // muted; alive keep the title tint.
        var card_clicked = false
        with_style((ImGuiCol.Text, record.state == "running"
                ? token_title_color() : token_state_color(record.state))) {
            card_clicked = selectable(HERD_CARD_ROW[index],
                (text = "{record.name}{unread_hint}##herd:{record.id}",
                 flags = ImGuiSelectableFlags.AllowOverlap))
        }
        if (card_clicked) {
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
            if (menu_item(HERD_MENU_DELETE[index], (text = "Delete session...",
                    shortcut = "", enabled = record.state != "running"))) {
                open_destructive_confirm("herd_delete", record.id,
                    "Delete session '{record.name}' and remove its worktrees? " +
                    "Worktrees in use or with uncommitted changes are kept.")
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
        // Output age (note 29): time since the terminal actually produced
        // bytes, beside the registry age.
        let output_wall = g_session_output_wall?[record.pty_session_id] ?? 0l
        let output_hint = (output_wall > 0l
            ? " | output {herd_age_label(output_wall)}" : "")
        text_disabled(HERD_CARD_DETAIL[index],
            (text = "      {record.profile_id} | {record.workflow} | " +
                "{base_name(record.worktree_path)}{satellite_hint} | " +
                "{herd_age_label(record.updated_wall_s)}{output_hint}"))
        // The card's own Attention and Review Bundles as subtree children
        // (note 19); zero-count sections never render.
        let attention = session_attention_count(record.pty_session_id)
        if (attention > 0) {
            text(HERD_ATT_HEADER[index], (text = "      Attention ({attention})"))
            draw_attention_rows(record.pty_session_id)
        }
        let bundles = session_bundle_count(record.pty_session_id)
        if (bundles > 0) {
            text(HERD_BUNDLE_HEADER[index],
                (text = "      Review bundles ({bundles})"))
            draw_bundle_rows(record.pty_session_id)
        }
    }
    if (visible == 0) {
        text_disabled(NO_HERD_LABEL,
            (text = "No sessions yet. Commands > New Session..."))
    }
}

var g_confirm_kind : string
var g_confirm_target : string
var g_confirm_text : string
// Second capture slot for kinds whose action needs a repository id beside the
// target (worktree_delete); captured at open time like the target.
var g_confirm_repository : string

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
            } elif (g_confirm_kind == "herd_delete") {
                g_client->request_herd_delete(g_confirm_target)
            } elif (g_confirm_kind == "worktree_delete") {
                g_client->request_worktree_delete(g_confirm_repository,
                    g_confirm_target)
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
    g_bundle_participant_row = 0
    g_bundle_file_row = 0
    g_attention_target_row = 0
    let connection = g_client != null && g_client.connected ? "connected" : "disconnected"
    text("Watcher: {connection}")
    same_line()
    if (icon_button(NEW_SESSION_BUTTON, (glyph = "add",
            tip = "New Session for the selected worktree",
            size = GetTextLineHeight()))) {
        let _ = run_herd_command("session.new")
    }
    same_line(NEW_SESSION_JOIN)
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
                (text = g_show_exited ? "No sessions"
                    : "No running terminals (tasks and exited are hidden)"))
        }
    }
    text_disabled("Exited sessions are retained and hidden by default.")
    // Fallback sections (note 19): only items whose session has no live card,
    // and only when there ARE any — zero-count sections never render.
    var unowned_bundles = 0
    var unowned_attention = 0
    if (g_client != null) {
        for (bundle in g_client.bundles) {
            if (!herd_owns_pty(bundle.session_id)) unowned_bundles++
        }
        for (message in g_client.mailbox) {
            if (message.direction == "outbox"
                    && !herd_owns_pty(message.session_id)) unowned_attention++
        }
    }
    if (unowned_bundles > 0) {
        separator()
        text("Review bundles ({unowned_bundles})")
        child(REVIEW_BUNDLES_CHILD, (text = "review bundles", size = float2(0.0f, 150.0f),
                child_flags = ImGuiChildFlags.Borders, window_flags = ImGuiWindowFlags.None)) {
            draw_bundle_rows("")
        }
    }
    if (unowned_attention > 0) {
        separator()
        text("Attention ({unowned_attention})")
        text_disabled("Agent notes never navigate until you click them.")
        child(ATTENTION_CHILD, (text = "attention", size = float2(0.0f, 0.0f),
                child_flags = ImGuiChildFlags.Borders, window_flags = ImGuiWindowFlags.None)) {
            draw_attention_rows("")
        }
    }
    // Error history (note 28, reshaped per note 46): collapsed by default,
    // scrollable, clearable. The ring keeps dismissed chrome errors.
    if (!empty(g_error_history)) {
        separator()
        collapsing_header(ERRORS_HEADER,
                "Errors ({length(g_error_history)})###SessionErrors",
                false, ImGuiTreeNodeFlags.None) {
            if (small_button(ERRORS_CLEAR, (text = "clear"))) {
                g_error_history |> clear()
                g_error_history_wall |> clear()
            }
            child(ERRORS_CHILD, (text = "errors",
                    size = float2(0.0f, 120.0f),
                    child_flags = ImGuiChildFlags.Borders,
                    window_flags = ImGuiWindowFlags.None)) {
                for (error_index in range(length(g_error_history))) {
                    // Newest last in storage; show newest first.
                    let ring = length(g_error_history) - 1 - error_index
                    text_disabled(ERROR_HISTORY_ROW[error_index],
                        (text = "{herd_age_label(g_error_history_wall[ring])}  {g_error_history[ring]}"))
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

