options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false
// Raw Save/LoadIniSettingsToDisk carry the per-preset layout profiles;
// the boost surface has no wrapper for them yet.
options _allow_imgui_legacy = true

require imgui/imgui_harness
require imgui/imgui_icons
require imgui/imgui_drawlist_builtin
require imgui/imgui_terminal
require imgui/imgui_fonts
require imgui/imgui_docking_builtin
require imgui/imgui_table_builtin
require imgui/imgui_markdown_tree_sitter
require imgui/imgui_text_source_view
require imgui/imgui_text_find
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
require ./rich_commands.das
require ./rich_net.das
require ./rich_inspector_ui.das
require ./rich_git_ui.das
require ./rich_terminal_ui.das
require ./rich_sessions_ui.das
require ./rich_launcher_ui.das
require ./rich_files_ui.das
require ./rich_live.das

// Session preset: sessions own the left (sessions-first); ALL git surfaces
// live on the right — repositories on top, Activity/Inspector tabbed in the
// middle, Changelist at the bottom (notes 49 and the external-sessions
// layout call: "worktree stuff moves to the right").
def private setup_session_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let viewport = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, viewport.Size)
    var left_id : uint = 0u
    var center_id : uint = 0u
    var right_id : uint = 0u
    var repositories_id : uint = 0u
    var changelist_id : uint = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.18f, left_id, center_id)
    DockBuilderSplitNode(center_id, ImGuiDir.Right, 0.32f, right_id, center_id)
    DockBuilderSplitNode(right_id, ImGuiDir.Up, 0.28f, repositories_id, right_id)
    DockBuilderSplitNode(right_id, ImGuiDir.Down, 0.45f, changelist_id, right_id)
    DockBuilderDockWindow("Sessions & Activity###SessionsActivity", left_id)
    DockBuilderDockWindow("Git Repositories & Worktrees###Repositories", repositories_id)
    DockBuilderDockWindow("Git Activity", right_id)
    DockBuilderDockWindow("File Inspector", right_id)
    DockBuilderDockWindow("Settings", right_id)
    DockBuilderDockWindow("Git Changelist", changelist_id)
    DockBuilderDockWindow("Terminal", center_id)
    DockBuilderFinish(dock_id)
    TERMINAL_WIN.pending_raise = true
}

// Review preset: terminal and inspector side by side; same sessions-left /
// git-right shape as the session preset.
def private setup_review_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let viewport = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, viewport.Size)
    var left_id : uint = 0u
    var work_id : uint = 0u
    var repositories_id : uint = 0u
    var git_column_id : uint = 0u
    var activity_id : uint = 0u
    var changelist_id : uint = 0u
    var terminal_id : uint = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.14f, left_id, work_id)
    DockBuilderSplitNode(work_id, ImGuiDir.Right, 0.24f, git_column_id, work_id)
    DockBuilderSplitNode(git_column_id, ImGuiDir.Up, 0.28f,
        repositories_id, git_column_id)
    DockBuilderSplitNode(git_column_id, ImGuiDir.Down, 0.5f,
        changelist_id, activity_id)
    DockBuilderSplitNode(work_id, ImGuiDir.Left, 0.40f, terminal_id, work_id)
    DockBuilderDockWindow("Sessions & Activity###SessionsActivity", left_id)
    DockBuilderDockWindow("Git Repositories & Worktrees###Repositories", repositories_id)
    DockBuilderDockWindow("Git Activity", activity_id)
    DockBuilderDockWindow("Git Changelist", changelist_id)
    DockBuilderDockWindow("Terminal", terminal_id)
    DockBuilderDockWindow("File Inspector", work_id)
    DockBuilderDockWindow("Settings", work_id)
    DockBuilderFinish(dock_id)
    FILE_INSPECTOR_WIN.pending_raise = true
}

def private setup_layout_preset(dock_id : uint) {
    if (g_layout_preset == 1) {
        setup_review_layout(dock_id)
    } else {
        setup_session_layout(dock_id)
    }
}

var private MENU_ERROR_POPUP : PopupContextItemState
var private MENU_ERROR_COPY : ToggleState
var private MENU_ERROR_SEND : ToggleState
var private MENU_ERROR_FIX : ToggleState

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
    let synth_locked = !user_control_enabled()
    var width = line + style.ItemSpacing.x + CalcTextSize(conn_label).x
    if (synth_locked) {
        width += line + CalcTextSize("SYNTH LOCK").x + style.ItemSpacing.x * 3.0f
    }
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
    if (synth_locked) {
        // The @live user-control lock is app-global and otherwise invisible:
        // real mouse/keyboard are detached while an agent drives synth input.
        icon("warning", line)
        same_line()
        text_colored(MENU_STATUS_SYNTH_LOCK,
            (color = token_warning_color(), text = "SYNTH LOCK"))
        set_item_tooltip(MENU_STATUS_SYNTH_LOCK_TIP,
            (text = "Real input is detached (set_user_control off) - an agent " +
                "owns the UI; live command set_user_control enabled=true releases it"))
        same_line()
    }
    if (!empty(error_text)) {
        icon("warning", line)
        same_line()
        text_colored(MENU_STATUS_ERROR,
            (color = token_error_color(), text = error_text))
        set_item_tooltip(MENU_STATUS_ERROR_TIP, (text = full_error))
        draw_error_context(MENU_ERROR_POPUP, MENU_ERROR_COPY,
            MENU_ERROR_SEND, "menu_error", "Watcher", full_error,
            MENU_ERROR_FIX)
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

def private command_new_session() {
    return if (g_client == null || !g_client.connected || empty(g_selected_worktree_path))
    prepare_launcher(g_selected_repository_id, g_selected_worktree_path)
    open_launcher_modal()
}

// Checklist rows are drawn through a shared helper, so every widget inside it
// renders once PER ROW in a single frame. A bare same_line() would take its
// identity from its one source location and collide with itself; both states
// are indexed by row instead.
var WORKTREE_RETIRE_ROW_JOIN : table<int; EmptyMarkerState>

var WORKTREE_RETIRE_ROW_TEXT : table<int; NarrativeState>

let private RETIRE_ROW_SESSION = 0
let private RETIRE_ROW_TREE = 1
let private RETIRE_ROW_COMMITS = 2

def private checklist_row(row : int; passing : bool; caption : string) {
    //! One fact, its pass/fail glyph, and what it means. Never a bare colour:
    //! the sentence has to carry the meaning on its own.
    icon_tinted(passing ? "check" : "warning", GetTextLineHeight(),
        passing ? token_added_color() : token_warning_color())
    same_line(WORKTREE_RETIRE_ROW_JOIN[row])
    text(WORKTREE_RETIRE_ROW_TEXT[row], (text = caption))
}

def private worktree_change_summary(worktree : WorktreeState const) : string {
    var inscope parts : array<string>
    if (worktree.staged > 0) parts |> push("{worktree.staged} staged")
    if (worktree.unstaged > 0) parts |> push("{worktree.unstaged} unstaged")
    if (worktree.untracked > 0) parts |> push("{worktree.untracked} untracked")
    if (worktree.conflicted > 0) parts |> push("{worktree.conflicted} conflicted")
    return join(parts, ", ")
}

var WORKTREE_RETIRE_LOG_ROW : table<int; NarrativeState>

// The failure log opens itself once per failure, then leaves the header alone
// so a user who collapses it is not fought every frame.
var g_retire_log_forced = false

def private operation_failure_brief(operation : WorktreeOperation const) : string {
    //! Everything the agent needs and nothing it has to ask for: the command
    //! that failed, git's own words, and the whole transcript.
    return build_string() $(var writer) {
        writer |> write("The operation FAILED at step {operation.step_index} ")
        writer |> write("of {operation.step_count}.\n\nGit reported:\n")
        writer |> write("{operation.error}\n")
        if (!empty(operation.hint)) {
            writer |> write("\nWhat that usually means:\n{operation.hint}\n")
        }
        writer |> write("\nFull command log:\n")
        for (line in operation.log) {
            writer |> write("{line}\n")
        }
    }
}

let private SPINNER_FRAMES = "|/-\\"

def private draw_retire_operation(operation : WorktreeOperation const) {
    //! The running/finished half of the dialog: what step, how far, and the
    //! exact commands with their output behind a collapsed header. A user who
    //! is told "working..." with no detail has to guess; the log is the detail.
    separator(WORKTREE_RETIRE_OP_SEPARATOR)
    if (operation.state == "running") {
        // A frame-driven glyph is the honest progress indicator here: git
        // reports no percentage, so a bar would be inventing one.
        let phase = (g_ui_frame / 6) % length(SPINNER_FRAMES)
        text(WORKTREE_RETIRE_SPINNER,
            (text = slice(SPINNER_FRAMES, phase, phase + 1)))
        same_line(WORKTREE_RETIRE_SPINNER_JOIN)
        text(WORKTREE_RETIRE_OP_STEP, (text =
            "{operation.step} ({operation.step_index}/{operation.step_count})"))
    } elif (operation.state == "ok") {
        with_style((ImGuiCol.Text, token_added_color())) {
            text(WORKTREE_RETIRE_OP_STEP, (text = "Done."))
        }
    } elif (operation.state == "cancelled") {
        with_style((ImGuiCol.Text, token_warning_color())) {
            text(WORKTREE_RETIRE_OP_STEP, (text =
                "Cancelled. Whatever already ran is in the log."))
        }
    } else {
        // Git's own words, in full and in the error colour — never summarised
        // away, because they are the only first-hand account of the failure.
        with_style((ImGuiCol.Text, token_error_color())) {
            text_wrapped(WORKTREE_RETIRE_OP_STEP,
                (text = "Failed: {operation.error}"))
        }
        if (!empty(operation.hint)) {
            text_wrapped(WORKTREE_RETIRE_OP_HINT, (text = operation.hint))
        }
    }
    // Collapsed while things are going well; a failure opens it, because that
    // is the moment the log is the whole point.
    if (operation.state == "failed" && !g_retire_log_forced) {
        g_retire_log_forced = true
        WORKTREE_RETIRE_LOG.pending_open = true
    }
    if (operation.state == "running") {
        g_retire_log_forced = false
    }
    collapsing_header(WORKTREE_RETIRE_LOG, (text =
            "Operation log ({length(operation.log)} lines)",
            closable = false, flags = ImGuiTreeNodeFlags.None)) {
        for (row, line in count(), operation.log) {
            text_disabled(WORKTREE_RETIRE_LOG_ROW[row], (text = line))
        }
    }
}

// The one retire-a-worktree checklist (note 58), for both entry doors. The
// facts are re-read live every frame instead of captured at open time, so it
// can never describe a state the repository has already left — a WIP commit
// made from inside this dialog flips its own verdict.
def private draw_worktree_retire_popup() {
    if (g_worktree_retire.active) {
        g_worktree_retire.active = false
        // A tick left over from the last worktree must never pre-arm a
        // destructive delete for this one.
        WORKTREE_RETIRE_FORCE.value = false
        open_popup(WORKTREE_RETIRE_DIALOG)
    }
    popup_modal(WORKTREE_RETIRE_DIALOG.PUBLIC, (text = "Retire worktree",
            closable = true, flags = ImGuiWindowFlags.AlwaysAutoResize)) {
        let path = g_worktree_retire.worktree_path
        let repository_id = g_worktree_retire.repository_id
        text(WORKTREE_RETIRE_PATH, (text = path))
        // Once an operation owns this worktree the checklist is history — and
        // after a successful delete it is actively WRONG, because the facts it
        // reads describe a folder that no longer exists.
        let operation_mine = (g_client != null
            && g_client.operation.worktree_path == path
            && g_client.operation.state != "idle"
            && !empty(g_client.operation.state))
        if (operation_mine) {
            let operation & = unsafe(g_client.operation)
            draw_retire_operation(operation)
            if (operation.state == "running") {
                if (button(WORKTREE_RETIRE_CANCEL_OP, (text = "Cancel"))) {
                    g_client->request_worktree_operation_cancel()
                }
            } else {
                if (operation.state == "failed") {
                    // The full output lives in a real PTY; the log above is a
                    // digest of it. This is the way to the whole thing.
                    if (button(WORKTREE_RETIRE_TERMINAL,
                            (text = "Open in terminal",
                             enabled = !empty(operation.session_id)))) {
                        g_client->attach_session(operation.session_id)
                        g_open_terminal_pending = true
                        g_focus_terminal_pending = true
                    }
                    same_line()
                    // When all else fails: hand the whole failure to an agent,
                    // in the worktree it happened in.
                    if (button(WORKTREE_RETIRE_FIX_SESSION,
                            (text = "Start a session to fix this..."))) {
                        start_error_session("Retiring the worktree {path}",
                            operation_failure_brief(operation), path)
                        close_current_popup(WORKTREE_RETIRE_DIALOG)
                    }
                    same_line()
                    // Only offered when trying again could actually differ —
                    // a retry against a permanent failure wastes the user's
                    // time and teaches them to distrust the button.
                    if (button(WORKTREE_RETIRE_RETRY, (text = "Try again",
                            enabled = operation.retryable))) {
                        g_worktree_retire.active = true
                        g_client->request_worktree_delete(repository_id, path,
                            WORKTREE_RETIRE_FORCE.value)
                    }
                    if (!operation.retryable) {
                        set_item_tooltip(WORKTREE_RETIRE_RETRY_TIP, (text =
                            "This failure will not change on its own — " +
                            "retrying would fail the same way."))
                    }
                    same_line()
                }
                if (button(WORKTREE_RETIRE_DONE, (text = "Close"))) {
                    close_current_popup(WORKTREE_RETIRE_DIALOG)
                    open_next_queued_retire()
                }
            }
            return
        }
        var worktree = WorktreeState()
        let observed = find_worktree_state(repository_id, path, worktree)
        let claimed_by = herd_session_claiming(path,
            g_worktree_retire.herd_session_id)
        let tier = (observed
            ? repository_worktree_delete_tier(worktree, claimed_by)
            : WORKTREE_TIER_DIRTY)
        let changes = worktree_change_summary(worktree)

        if (!observed) {
            text_disabled(WORKTREE_RETIRE_UNKNOWN, (text =
                "This worktree is not in the Git inventory yet — refresh the " +
                "repository before retiring it."))
        }
        checklist_row(RETIRE_ROW_SESSION, empty(claimed_by),
            (empty(claimed_by)
                ? "No session is working here."
                : "Session '{claimed_by}' is working here — retire it first."))
        checklist_row(RETIRE_ROW_TREE, empty(changes),
            (empty(changes)
                ? "Working tree is clean."
                : "{changes} — these files exist only in this folder."))
        // The unpushed line is about REDUNDANCY, not risk: removing a worktree
        // cannot lose a commit, the branch keeps every one of them.
        if (!worktree.unmerged_known) {
            checklist_row(RETIRE_ROW_COMMITS, true,
                "Commits: no base ref to compare against — count unknown.")
        } elif (worktree.unmerged_commits == 0) {
            checklist_row(RETIRE_ROW_COMMITS, true,
                "Nothing here that {worktree.unmerged_base_ref} does not have.")
        } elif (repository_worktree_only_here(worktree)) {
            checklist_row(RETIRE_ROW_COMMITS, false,
                "{worktree.unmerged_commits} commits vs {worktree.unmerged_base_ref} " +
                "exist only on this machine; nothing else has a copy.")
        } else {
            checklist_row(RETIRE_ROW_COMMITS, true,
                "{worktree.unmerged_commits} commits vs {worktree.unmerged_base_ref}, " +
                "all pushed to {worktree.upstream_ref}.")
        }
        if (!empty(worktree.branch)) {
            text_disabled(WORKTREE_RETIRE_BRANCH_NOTE, (text =
                "Branch {worktree.branch} keeps its commits — removing this " +
                "folder does not delete the branch."))
        }
        if (!empty(g_worktree_retire.server_reason)) {
            with_style((ImGuiCol.Text, token_error_color())) {
                text(WORKTREE_RETIRE_SERVER, (text =
                    "Refused: {g_worktree_retire.server_detail}"))
            }
        }
        if (!empty(g_worktree_retire.notice)) {
            text_disabled(WORKTREE_RETIRE_NOTICE,
                (text = g_worktree_retire.notice))
        }

        if (tier == WORKTREE_TIER_IN_USE) {
            if (button(WORKTREE_RETIRE_GOTO_SESSION,
                    (text = "Go to session '{claimed_by}'"))) {
                g_open_sessions_pending = true
                close_current_popup(WORKTREE_RETIRE_DIALOG)
            }
        } elif (tier == WORKTREE_TIER_DIRTY) {
            // The mechanical rescue Boris named as the button: the branch
            // outlives the worktree and keeps its name, so committing needs no
            // judgment about what belongs where.
            if (button(WORKTREE_RETIRE_COMMIT, (text = "Commit to the branch",
                    enabled = g_client != null && observed
                        && !empty(worktree.branch)))) {
                g_client->request_worktree_commit_wip(repository_id, path,
                    "WIP: work in progress on {worktree.branch}")
                g_worktree_retire.notice = "Committing..."
            }
            same_line()
            if (button(WORKTREE_RETIRE_LAUNCH,
                    (text = "Start a session to sort it out..."))) {
                g_selected_repository_id = repository_id
                g_selected_worktree_path = path
                g_selection_explicit = true
                prepare_resolver_launcher(repository_id, path,
                    "The user wants to RETIRE the worktree {path}, but it has " +
                    "uncommitted changes:\n{changes}\n" +
                    "Review the changes and resolve them: commit (and push) " +
                    "what should be kept, discard what should not. Do not " +
                    "delete the worktree yourself. When the tree is clean, " +
                    "report what you did and finish.")
                g_launch_resolver_pending = true
                close_current_popup(WORKTREE_RETIRE_DIALOG)
            }
            // Untracked-as-blocker needs a way through, or junk files send the
            // user off to clean by hand for nothing.
            checkbox(WORKTREE_RETIRE_FORCE, (text =
                "Delete anyway — {changes} lost"))
            g_worktree_retire.force_acknowledged = WORKTREE_RETIRE_FORCE.value
            if (button(WORKTREE_RETIRE_FORCE_GO, (text = "Delete worktree...",
                    enabled = g_client != null
                        && g_worktree_retire.force_acknowledged))) {
                g_worktree_retire.confirming = true
            }
        } else {
            if (button(WORKTREE_RETIRE_GO, (text = "Delete worktree...",
                    enabled = g_client != null && observed))) {
                g_worktree_retire.confirming = true
            }
        }
        same_line()
        if (button(WORKTREE_RETIRE_CLOSE, (text = "Cancel"))) {
            close_current_popup(WORKTREE_RETIRE_DIALOG)
        }
        // The confirmation is a second, explicit act, and it names what is
        // about to happen rather than asking "are you sure" about nothing.
        if (g_worktree_retire.confirming) {
            separator(WORKTREE_RETIRE_CONFIRM_SEPARATOR)
            let lost = (tier == WORKTREE_TIER_DIRTY
                ? " {changes} will be lost." : "")
            with_style((ImGuiCol.Text, token_warning_color())) {
                text(WORKTREE_RETIRE_CONFIRM_TEXT, (text =
                    "Delete the folder {path}?{lost} " +
                    "The branch is not touched."))
            }
            if (button(WORKTREE_RETIRE_CONFIRM_GO, (text = "Yes, delete it"))) {
                g_worktree_retire.confirming = false
                g_worktree_retire.notice = ""
                g_client->request_worktree_delete(repository_id, path,
                    tier == WORKTREE_TIER_DIRTY)
            }
            same_line()
            if (button(WORKTREE_RETIRE_CONFIRM_NO, (text = "No"))) {
                g_worktree_retire.confirming = false
            }
        }
    }
}

def private command_powershell() {
    return if (g_client == null || !g_client.connected)
    g_client->launch("powershell.exe -NoLogo -NoProfile", g_selected_worktree_path,
        g_selected_repository_id, g_selected_worktree_path)
}

// ImGuiKey_0..9 sit immediately before ImGuiKey_A; the "0".."9" enum entries
// are not spellable as identifiers.
def private digit_key(digit : int) : ImGuiKey {
    return unsafe(reinterpret<ImGuiKey>(int(ImGuiKey.A) - 10 + digit))
}

// One registration per user-triggerable action (note 23): menus, chords, the
// Shortcuts editor, and the herder_command rail all dispatch through these ids.
def private register_commands() {
    register_herd_command("session.new", "Session", "New Session...",
        command_binding("session.new", ImGuiKey.N, primary = true, shift = true),
        @@command_new_session)
    register_herd_command("session.powershell", "Session", "PowerShell here",
        command_binding("session.powershell", ImGuiKey.P, primary = true, shift = true),
        @@command_powershell)
    register_herd_command("view.repositories", "View", "Git Repositories & Worktrees",
        CommandBinding(), @@() { open_dock_window(REPOSITORIES_WIN); })
    register_herd_command("view.sessions", "View", "Sessions & Activity",
        command_binding("view.sessions", ImGuiKey.S, primary = true, shift = true),
        @@() { open_dock_window(SESSIONS_WIN); })
    register_herd_command("view.git_changelist", "View", "Git Changelist",
        CommandBinding(), @@() { open_dock_window(GIT_CHANGELIST_WIN); })
    register_herd_command("view.git_activity", "View", "Git Activity",
        CommandBinding(), @@() { open_dock_window(GIT_ACTIVITY_WIN); })
    register_herd_command("view.file_inspector", "View", "File Inspector",
        CommandBinding(), @@() { open_dock_window(FILE_INSPECTOR_WIN); })
    register_herd_command("view.terminal", "View", "Terminal",
        command_binding("view.terminal", ImGuiKey.T, primary = true, shift = true),
        @@() { open_dock_window(TERMINAL_WIN); })
    register_herd_command("view.launcher", "View", "New Session dialog",
        CommandBinding(), @@() { open_launcher_modal(); })
    register_herd_command("view.settings", "View", "Settings",
        command_binding("view.settings", ImGuiKey.Comma, primary = true),
        @@() { open_dock_window(SETTINGS_WIN); })
    register_herd_command("view.scroll_terminal_bottom", "View",
        "Scroll terminal to bottom",
        command_binding("view.scroll_terminal_bottom", ImGuiKey.End, primary = true),
        @@() { g_terminal_view.scroll_offset = 0; })
    register_herd_command("zoom.in", "Zoom", "Zoom in",
        command_binding("zoom.in", ImGuiKey.Equal, primary = true),
        @@() { zoom_by(5); })
    register_herd_command("zoom.out", "Zoom", "Zoom out",
        command_binding("zoom.out", ImGuiKey.Minus, primary = true),
        @@() { zoom_by(-5); })
    register_herd_command("zoom.reset", "Zoom", "Reset zoom",
        command_binding("zoom.reset", digit_key(0), primary = true),
        @@() { set_ui_zoom(100); })
    register_herd_command("layout.session", "Layout", "Session layout",
        command_binding("layout.session", digit_key(1), primary = true, shift = true),
        @@() { apply_layout_preset(0); })
    register_herd_command("layout.review", "Layout", "Review layout",
        command_binding("layout.review", digit_key(2), primary = true, shift = true),
        @@() { apply_layout_preset(1); })
    register_herd_command("layout.reset", "Layout", "Reset layout",
        CommandBinding(), @@() { reset_layout(); })
    register_herd_command("terminal.copy", "Terminal", "Copy selection",
        CommandBinding(), @@() { copy_terminal_selection(); })
    register_herd_command("terminal.paste", "Terminal", "Paste clipboard",
        CommandBinding(), @@() { paste_terminal_clipboard(); })
    register_herd_command("terminal.select_all", "Terminal", "Select all",
        CommandBinding(), @@() { imgui_terminal_select_all(g_terminal, g_terminal_view); })
    register_herd_command("terminal.clear_selection", "Terminal", "Clear selection",
        CommandBinding(), @@() { imgui_terminal_clear_selection(g_terminal_view); })
}

def draw_main_menu() {
    main_menu_bar(MAIN_MENU) {
        menu(FILE_MENU, (text = "File", enabled = true)) {
            if (menu_label(FILE_EXIT, (text = "Exit", shortcut = "Alt+F4"))) {
                request_exit()
            }
        }
        menu(COMMANDS_MENU, (text = "Commands", enabled = true)) {
            if (menu_label(COMMAND_NEW_SESSION,
                    (text = "New Session...", shortcut = herd_command_shortcut("session.new"),
                        enabled = g_client != null
                        && g_client.connected && !empty(g_selected_worktree_path)))) {
                let _ = run_herd_command("session.new")
            }
            if (menu_label(COMMAND_POWERSHELL,
                    (text = "PowerShell", shortcut = herd_command_shortcut("session.powershell"),
                        enabled = g_client != null && g_client.connected))) {
                let _ = run_herd_command("session.powershell")
            }
        }
        menu(EDIT_MENU, (text = "Edit", enabled = true)) {
            if (menu_label(EDIT_COPY, (text = "Copy", shortcut = herd_command_shortcut("terminal.copy"),
                    enabled = g_terminal_view.selection_active))) {
                let _ = run_herd_command("terminal.copy")
            }
            if (menu_label(EDIT_PASTE, (text = "Paste",
                    shortcut = herd_command_shortcut("terminal.paste")))) {
                let _ = run_herd_command("terminal.paste")
            }
            separator(EDIT_SELECTION_SEPARATOR)
            if (menu_label(EDIT_SELECT_ALL, (text = "Select all",
                    shortcut = herd_command_shortcut("terminal.select_all")))) {
                let _ = run_herd_command("terminal.select_all")
            }
            if (menu_label(EDIT_CLEAR_SELECTION,
                    (text = "Clear selection", shortcut = herd_command_shortcut("terminal.clear_selection"),
                        enabled = g_terminal_view.selection_active))) {
                let _ = run_herd_command("terminal.clear_selection")
            }
        }
        menu(VIEW_MENU, (text = "View", enabled = true)) {
            if (menu_label(VIEW_REPOSITORIES,
                    (text = "Git Repositories & Worktrees", selected = REPOSITORIES_WIN.open,
                        shortcut = herd_command_shortcut("view.repositories")))) {
                let _ = run_herd_command("view.repositories")
            }
            if (menu_label(VIEW_SESSIONS,
                    (text = pending_attention_count() > 0
                        ? "Sessions & Activity  [{pending_attention_count()}]"
                        : "Sessions & Activity", selected = SESSIONS_WIN.open,
                        shortcut = herd_command_shortcut("view.sessions")))) {
                let _ = run_herd_command("view.sessions")
            }
            if (menu_label(VIEW_GIT_CHANGELIST,
                    (text = "Git Changelist", selected = GIT_CHANGELIST_WIN.open,
                        shortcut = herd_command_shortcut("view.git_changelist")))) {
                let _ = run_herd_command("view.git_changelist")
            }
            if (menu_label(VIEW_GIT_ACTIVITY,
                    (text = "Git Activity", selected = GIT_ACTIVITY_WIN.open,
                        shortcut = herd_command_shortcut("view.git_activity")))) {
                let _ = run_herd_command("view.git_activity")
            }
            if (menu_label(VIEW_FILE_INSPECTOR,
                    (text = "File Inspector", selected = FILE_INSPECTOR_WIN.open,
                        shortcut = herd_command_shortcut("view.file_inspector")))) {
                let _ = run_herd_command("view.file_inspector")
            }
            if (menu_label(VIEW_TERMINAL,
                    (text = "Terminal", selected = TERMINAL_WIN.open,
                        shortcut = herd_command_shortcut("view.terminal")))) {
                let _ = run_herd_command("view.terminal")
            }
            if (menu_label(VIEW_LAUNCHER,
                    (text = "New Session", selected = launcher_modal_open(),
                        shortcut = herd_command_shortcut("view.launcher")))) {
                let _ = run_herd_command("view.launcher")
            }
            separator(VIEW_WINDOWS_SEPARATOR)
            // g_show_* are authoritative (the Settings tab writes them too):
            // seed the widget, then adopt only a user click on the menu item.
            SHOW_EXITED.value = g_show_exited
            menu_item(SHOW_EXITED, (text = "Show exited sessions"))
            if (SHOW_EXITED.value != g_show_exited) {
                g_show_exited = SHOW_EXITED.value
                g_ui_settings.show_exited = g_show_exited
                settings_save()
            }
            SHOW_TASKS.value = g_show_tasks
            menu_item(SHOW_TASKS, (text = "Show watcher task sessions"))
            if (SHOW_TASKS.value != g_show_tasks) {
                g_show_tasks = SHOW_TASKS.value
                g_ui_settings.show_tasks = g_show_tasks
                settings_save()
            }
            separator(VIEW_ZOOM_SEPARATOR)
            if (menu_label(VIEW_ZOOM_IN, (text = "Zoom in",
                    shortcut = herd_command_shortcut("zoom.in")))) {
                let _ = run_herd_command("zoom.in")
            }
            if (menu_label(VIEW_ZOOM_OUT, (text = "Zoom out",
                    shortcut = herd_command_shortcut("zoom.out")))) {
                let _ = run_herd_command("zoom.out")
            }
            if (menu_label(VIEW_ZOOM_RESET, (text = "Reset zoom",
                    shortcut = herd_command_shortcut("zoom.reset")))) {
                let _ = run_herd_command("zoom.reset")
            }
            if (menu_label(VIEW_SCROLL_BOTTOM, (text = "Scroll terminal to bottom",
                    shortcut = herd_command_shortcut("view.scroll_terminal_bottom")))) {
                let _ = run_herd_command("view.scroll_terminal_bottom")
            }
            separator(VIEW_LAYOUT_SEPARATOR)
            if (menu_label(VIEW_SESSION_LAYOUT, (text = "Session layout",
                    selected = g_layout_preset == 0,
                    shortcut = herd_command_shortcut("layout.session")))) {
                let _ = run_herd_command("layout.session")
            }
            if (menu_label(VIEW_REVIEW_LAYOUT, (text = "Review layout",
                    selected = g_layout_preset == 1,
                    shortcut = herd_command_shortcut("layout.review")))) {
                let _ = run_herd_command("layout.review")
            }
            if (menu_label(VIEW_RESET_LAYOUT, (text = "Reset layout",
                    shortcut = herd_command_shortcut("layout.reset")))) {
                let _ = run_herd_command("layout.reset")
            }
        }
        menu(SETTINGS_MENU, (text = "Settings", enabled = true)) {
            if (menu_label(OPEN_SETTINGS, (text = "Open Settings",
                    shortcut = herd_command_shortcut("view.settings")))) {
                let _ = run_herd_command("view.settings")
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
    launcher_ui_init()
    files_ui_init()
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
    settings_load()
    register_commands()
    herd_commands_ready()
    SHOW_EXITED.value = g_show_exited
    SHOW_TASKS.value = g_show_tasks
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
    if (g_client != null) {
        // Error ring (note 28): capture every transition to a new error text.
        if (g_client.error != g_last_seen_error) {
            g_last_seen_error = g_client.error
            if (!empty(g_client.error)) {
                g_error_history |> push(g_client.error)
                g_error_history_wall |> push(int64(get_clock()))
                while (length(g_error_history) > 20) {
                    g_error_history |> erase(0)
                    g_error_history_wall |> erase(0)
                }
            }
        }
        // Output age (note 29): stamp only witnessed output_bytes deltas.
        for (session in g_client.sessions) {
            let seen = g_session_output_seen?[session.id] ?? -1l
            if (seen != session.output_bytes) {
                if (seen >= 0l) {
                    g_session_output_wall[session.id] = int64(get_clock())
                }
                g_session_output_seen[session.id] = session.output_bytes
            }
        }
        // Sessions the watcher pruned leave orphaned entries behind forever
        // otherwise (the tables only ever gain keys above).
        if (length(g_session_output_seen) > length(g_client.sessions)) {
            var inscope live_ids : table<string>
            for (session in g_client.sessions) {
                live_ids |> insert(session.id)
            }
            var inscope stale <- [for (id in keys(g_session_output_seen));
                clone_string(id); where !key_exists(live_ids, id)]
            for (id in stale) {
                g_session_output_seen |> erase(id)
                g_session_output_wall |> erase(id)
            }
        }
    }
    if (g_submit_stamp != 0l
            && get_time_nsec(g_submit_stamp) / 1000000l >= 200l) {
        if (g_client != null && g_client.controls
                && g_client.selected_id == g_submit_session) {
            g_client->send_input("\r")
        }
        g_submit_stamp = 0l
        g_submit_session = ""
    }
    // Feed queued says only once the pending Enter has flushed; an item for a
    // session we no longer control is dropped (same rule as the \r above).
    while (g_submit_stamp == 0l && !empty(g_input_queue)) {
        let item = g_input_queue[0]
        g_input_queue |> erase(0)
        if (g_client == null || !g_client.controls
                || g_client.selected_id != item.session) continue
        if (!empty(item.text)) {
            g_client->send_input(item.text)
        }
        if (item.submit) {
            // Even a lone queued submit re-arms the paste window — its \r must
            // stay clear of whatever text drained just before it.
            g_submit_stamp = ref_time_ticks()
            g_submit_session = item.session
        }
    }
    if (!harness_begin_frame()) return
    service_sha_copy()
    drain_inspector_prepare_events()
    if (g_layout_switch_pending >= 0) {
        // Between the native frame gate and NewFrame: settings may only be
        // loaded while no ImGui frame is active. Switching away snapshots
        // the current preset's arrangement; switching in restores the
        // target's own tuned profile, or falls back to its factory builder.
        let target = g_layout_switch_pending
        g_layout_switch_pending = -1
        SaveIniSettingsToDisk(layout_ini_path(g_layout_preset))
        if (g_layout_factory_reset) {
            g_layout_factory_reset = false
            let _removed = remove(layout_ini_path(target))
        }
        g_layout_preset = target
        g_ui_settings.layout_preset = target
        settings_save()
        if (fexist(layout_ini_path(target))) {
            LoadIniSettingsFromDisk(layout_ini_path(target))
            DOCK_ROOT.has_initial_layout = true
            g_layout_probe_complete = true
            g_layout_force_default = false
        } else {
            g_layout_probe_complete = false
            g_layout_force_default = true
            g_dock_reset_pending = true
        }
    }
    harness_new_frame()
    g_ui_frame++
    {
        let io & = unsafe(GetIO())
        if (io.MouseWheel != 0.0f || io.MouseWheelH != 0.0f) {
            g_wheel_events_seen++
            g_wheel_last_delta = io.MouseWheel
        }
        if (IsMouseClicked(ImGuiMouseButton.Left)
                || IsMouseClicked(ImGuiMouseButton.Right)) {
            g_click_events_seen++
        }
    }
    herd_commands_poll()
    let tab_signature = ("git={g_git_perspective} cl={g_changelist_view} " +
        "diff={INSPECTOR_DIFF_TAB.selected} " +
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
    if (g_open_sessions_pending) {
        g_open_sessions_pending = false
        SESSIONS_WIN.open = true
        SESSIONS_WIN.pending_raise = true
    }
    if (g_dock_reset_pending) {
        g_dock_reset_pending = false
        DOCK_ROOT.pending_reset = true
    }
    if (g_launch_resolver_pending) {
        g_launch_resolver_pending = false
        open_launcher_modal()
    }
    draw_main_menu()
    dockspace(DOCK_ROOT, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        if (!DOCK_ROOT.has_initial_layout && DOCK_ROOT.dock_id != 0u) {
            if (g_layout_force_default) {
                setup_layout_preset(DOCK_ROOT.dock_id)
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
                    setup_layout_preset(DOCK_ROOT.dock_id)
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
    draw_herd_rename_popup()
    draw_herd_retire_popup()
    draw_destructive_confirm_popup()
    draw_worktree_retire_popup()
    draw_launcher_modal()
    herd_commands_drain()
    harness_end_frame()
}

[export]
def shutdown() {
    herd_commands_dispose()
    let prepare_stopped = inspector_prepare_worker_shutdown(g_prepare_worker)
    if (g_client != null) {
        g_client->close()
        g_client->cleanup()
        delete g_client.sessions
        delete g_client.repositories
        delete g_client.herd_profiles
        delete g_client.herd_sessions
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
    text_find_dispose(g_inspector.find)
    delete g_inspector.find_windows
    terminal_viewport_snapshot_dispose(g_terminal_cache.snapshot)
    terminal_dispose(g_terminal)
    delete g_font_sources
    SaveIniSettingsToDisk(layout_ini_path(g_layout_preset))
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
