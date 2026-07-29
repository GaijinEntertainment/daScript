options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_git_ui

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
require math
require strings
require ./rich_state.das
require ./rich_net.das
require ./rich_commands.das
require ./herd_sessions.das
require ./rich_inspector_ui.das
require ./rich_files_ui.das

var WORKTREE_ROW : table<int; ToggleState>

var WORKTREE_CONFLICT_JOIN : table<int; EmptyMarkerState>

var WORKTREE_ACTIVE_JOIN : table<int; EmptyMarkerState>

var WORKTREE_ROW_TOOLTIP : table<int; NarrativeState>

var WORKTREE_POPUP : table<int; PopupContextItemState>

var WORKTREE_MENU_NEW_SESSION : table<int; ToggleState>

var WORKTREE_MENU_OPEN_SESSION : table<int; ToggleState>

var WORKTREE_MENU_DELETE : table<int; ToggleState>

var REPOSITORY_FOLDER_JOIN : table<int; EmptyMarkerState>

var GIT_FILE_ROW : table<int; ToggleState>

// Changes-view rows keep their OWN identity tables — sharing GIT_FILE_ROW
// with the PR views would collide two widgets on one identity the frame the
// user switches tabs.
var GIT_STATUS_ROW : table<int; ToggleState>

var GIT_STATUS_ROW_POPUP : table<int; PopupContextItemState>

var GIT_STATUS_STAGE_MENU : table<int; ToggleState>

var GIT_STATUS_UNSTAGE_MENU : table<int; ToggleState>

var GIT_GROUP_CHECK : table<int; ToggleState>

var GIT_GROUP_CHECK_JOIN : table<int; EmptyMarkerState>

var GIT_STATUS_DISCARD_MENU : table<int; ToggleState>

var GIT_STATUS_DELETE_MENU : table<int; ToggleState>

var GIT_DISCARD_LIST_ROW : table<int; NarrativeState>

var GIT_COMMIT_TITLE_TEXT : table<int; NarrativeState>

var GIT_COMMIT_AUTHOR_TEXT : table<int; NarrativeState>

var GIT_TREE_REF_SAME_LINE : table<int; EmptyMarkerState>

var GIT_TREE_REF_ROW : table<int; ClickState>

var GIT_TREE_GRAPH_CELL : table<int; EmptyMarkerState>

var GIT_PR_COMMIT_HOVER : table<int; EmptyMarkerState>

var GIT_HISTORY_COMMIT_ROW : table<int; ClickState>

var GIT_COMMIT_FILE_ROW : table<int; ToggleState>

var REPOSITORY_ERROR : table<int; NarrativeState>

var REPOSITORY_TITLE : table<int; NarrativeState>

var REPOSITORY_EMPTY : table<int; NarrativeState>

var GIT_GROUP_TITLE : table<int; NarrativeState>

var REPOSITORY_SEPARATOR : table<int; EmptyMarkerState>

var WORKTREE_SEPARATOR : table<int; EmptyMarkerState>

var GIT_GROUP_SEPARATOR : table<int; EmptyMarkerState>

var GIT_GROUP_SPACING : table<int; EmptyMarkerState>

var GIT_HOVER_TOOLTIP : NarrativeState

var GIT_CHANGELIST_STATUS_TEXT : NarrativeState

var GIT_ACTIVITY_STATUS_TEXT : NarrativeState

var GIT_CHANGELIST_STATUS_SEPARATOR : EmptyMarkerState

var GIT_ACTIVITY_STATUS_SEPARATOR : EmptyMarkerState

var REPOSITORY_ERROR_POPUP : table<int; PopupContextItemState>

var REPOSITORY_ERROR_COPY : table<int; ToggleState>

var REPOSITORY_ERROR_SEND : table<int; ToggleState>

var REPOSITORY_ERROR_FIX : table<int; ToggleState>

var CHANGELIST_ERROR_POPUP : PopupContextItemState

var CHANGELIST_ERROR_COPY : ToggleState

var CHANGELIST_ERROR_SEND : ToggleState

var CHANGELIST_ERROR_FIX : ToggleState

var ACTIVITY_ERROR_POPUP : PopupContextItemState

var ACTIVITY_ERROR_COPY : ToggleState

var ACTIVITY_ERROR_SEND : ToggleState

var ACTIVITY_ERROR_FIX : ToggleState

// Index of a non-closed herd session claiming this worktree, or -1. A running
// session prefers over an exited one so the marker reflects the live claimant.
def private active_herd_for_worktree(worktree_path : string) : int {
    if (g_client == null) return -1
    let canonical = herd_canonical_worktree(worktree_path)
    var found = -1
    for (index, record in count(), g_client.herd_sessions) {
        if (record.archived) continue
        if (record.state != "closed"
                && herd_canonical_worktree(record.worktree_path) == canonical) {
            if (record.state == "running") return index
            if (found < 0) {
                found = index
            }
        }
    }
    return found
}

// The "is it safe to delete this?" answer, on the row where the question is
// asked (note 58). Every count names the ref it was measured against, because
// a bare number against an unnamed base is what made the answer unreadable.
def private worktree_retire_hint(worktree : WorktreeState const;
                                 active_herd : int) : string {
    let claimed_by = (active_herd >= 0
        ? g_client.herd_sessions[active_herd].name : "")
    let tier = repository_worktree_delete_tier(worktree, claimed_by)
    if (tier == WORKTREE_TIER_MAIN) return "Main checkout — never deleted."
    if (tier == WORKTREE_TIER_IN_USE) {
        return "In use by session '{claimed_by}' — retire the session first."
    }
    let dirty = (worktree.staged + worktree.unstaged + worktree.untracked
        + worktree.conflicted)
    if (tier == WORKTREE_TIER_DIRTY) {
        return ("{dirty} uncommitted files live only in this folder — " +
            "retire offers to commit them to {worktree.branch} first.")
    }
    if (tier == WORKTREE_TIER_WARN) {
        return ("Safe to delete. {worktree.unmerged_commits} commits vs " +
            "{worktree.unmerged_base_ref} exist only on this machine — the " +
            "branch {worktree.branch} keeps them either way.")
    }
    if (!worktree.unmerged_known) {
        return "Safe to delete. Unmerged count unknown: no base ref resolved."
    }
    return ("Safe to delete. Nothing here that " +
        "{worktree.unmerged_base_ref} does not already have.")
}

// Worktree -> session navigation (note 18): raise Sessions and attach the
// claiming session's terminal when it has one.
def private navigate_to_herd(record_index : int) {
    return if (g_client == null || record_index < 0
        || record_index >= length(g_client.herd_sessions))
    let record & = unsafe(g_client.herd_sessions[record_index])
    g_open_sessions_pending = true
    // Attach only when the PTY is still alive in the watcher's session list;
    // a registry entry can outlive its PTY (watcher restart), and the Sessions
    // card offers Relaunch for those.
    for (session in g_client.sessions) {
        if (session.id == record.pty_session_id) {
            g_client->attach_session(record.pty_session_id, false)
            g_open_terminal_pending = true
            return
        }
    }
}

def draw_repositories_window() {
    text_disabled("Target: Local")
    separator()
    input_text_growable(ADD_REPOSITORY_PATH, (text = "Repository checkout"))
    same_line()
    if (icon_button(ADD_REPOSITORY, (glyph = "add",
            tip = "Add the repository checkout to the watcher",
            size = GetTextLineHeight())) && g_client != null) {
        g_client->add_repository(ADD_REPOSITORY_PATH.value)
    }
    separator()
    var row = 0
    var repository_row = 0
    if (g_client != null) {
        for (repository in g_client.repositories) {
            let refreshing = repository.refreshing ? "  refreshing..." : ""
            icon("folder", GetTextLineHeight())
            same_line(REPOSITORY_FOLDER_JOIN[repository_row])
            text(REPOSITORY_TITLE[repository_row],
                (text = "{repository.record.display_name}  [Local]{refreshing}"))
            if (!empty(repository.error)) {
                text_colored(REPOSITORY_ERROR[repository_row], (color = token_error_color(),
                    text = repository.error))
                draw_error_context(REPOSITORY_ERROR_POPUP[repository_row],
                    REPOSITORY_ERROR_COPY[repository_row],
                    REPOSITORY_ERROR_SEND[repository_row],
                    "repository_error_{repository_row}", "Repository",
                    repository.error,
                    REPOSITORY_ERROR_FIX[repository_row])
            }
            for (worktree in repository.worktrees) {
                var branch = worktree.branch
                if (worktree.detached) {
                    branch = "detached {slice(worktree.head, 0, min(8, length(worktree.head)))}"
                }
                let prefix = worktree.is_main ? "MAIN  " : "      "
                let dirty = worktree.staged + worktree.unstaged + worktree.untracked + worktree.conflicted
                let dirty_label = dirty > 0 ? "  *{dirty}" : ""
                if (g_client->herd_worktree_conflicted(worktree.path)) {
                    icon("warning", GetTextLineHeight())
                    if (IsItemHovered()) {
                        set_tooltip(WORKTREE_ICON_TOOLTIP,
                            "Two or more sessions attend this worktree")
                    }
                    same_line(WORKTREE_CONFLICT_JOIN[row])
                }
                let active_herd = active_herd_for_worktree(worktree.path)
                if (active_herd >= 0) {
                    let claim_running = (g_client.herd_sessions[active_herd].state
                        == "running")
                    icon(claim_running ? "play" : "clock", GetTextLineHeight())
                    if (IsItemHovered()) {
                        set_tooltip(WORKTREE_ICON_TOOLTIP, (claim_running
                            ? "Session '{g_client.herd_sessions[active_herd].name}' is running here"
                            : "Session '{g_client.herd_sessions[active_herd].name}' is attached here (not running)"))
                    }
                } else {
                    // An empty space is indistinguishable from a feature that
                    // was never built — which is exactly the confusion note 58
                    // was filed about. "No session" gets its own mark.
                    icon_tinted("minus", GetTextLineHeight(), token_muted_color())
                    if (IsItemHovered()) {
                        set_tooltip(WORKTREE_ICON_TOOLTIP,
                            "No session works here")
                    }
                }
                same_line(WORKTREE_ACTIVE_JOIN[row])
                // Transparent label keeps the row id + snapshot text; the
                // overlay draws MAIN / slash-tinted branch / amber dirty in
                // the shared token palette (notes 18/20).
                // The folder rides every row: branch and folder names drift
                // apart, and a row naming only the branch leaves "which
                // folder is that" unanswerable (the live-review confusion).
                let folder_tag = " @ {base_name(worktree.path)}"
                let row_label = "{prefix}{branch}{dirty_label}{folder_tag}##{repository.record.id}:{worktree.path}"
                WORKTREE_ROW[row].value = (repository.record.id == g_selected_repository_id && worktree.path == g_selected_worktree_path)
                var row_clicked = false
                with_style((ImGuiCol.Text, float4(0.0f, 0.0f, 0.0f, 0.0f))) {
                    row_clicked = selectable(WORKTREE_ROW[row], (text = row_label))
                }
                if (IsItemHovered()) {
                    set_tooltip(WORKTREE_ROW_TOOLTIP[row],
                        worktree_retire_hint(worktree, active_herd))
                }
                if (row_clicked) {
                    ui_event("worktree-row-click {worktree.path}")
                    g_selected_repository_id = repository.record.id
                    g_selected_worktree_path = worktree.path
                    g_selection_explicit = true
                    g_client->refresh_repository(repository.record.id)
                    g_client->request_repository_review(
                        repository.record.id, worktree.path)
                    GIT_CHANGELIST_WIN.open = true
                    GIT_CHANGELIST_WIN.pending_raise = true
                    GIT_ACTIVITY_WIN.open = true
                }
                {
                    let row_min = GetItemRectMin()
                    let row_max = GetItemRectMax()
                    let text_y = row_min.y + max(0.0f,
                        (row_max.y - row_min.y - GetTextLineHeight()) * 0.5f)
                    let x0 = row_min.x + GetStyle().FramePadding.x
                    if (worktree.is_main) {
                        with_window_drawlist() $(var dl) {
                            dl |> add_text(float2(x0, text_y),
                                GetColorU32(token_title_color()), "MAIN")
                        }
                    }
                    let branch_x = x0 + CalcTextSize(prefix).x
                    let right_limit = git_visible_row_right(row_max.x)
                    draw_git_path_text(branch, float2(branch_x, text_y),
                        float2(right_limit, row_max.y), false)
                    if (dirty > 0) {
                        with_window_drawlist() $(var dl) {
                            dl |> add_text(
                                float2(branch_x + CalcTextSize(branch).x, text_y),
                                GetColorU32(token_warning_color()), dirty_label)
                        }
                    }
                    with_window_drawlist() $(var dl) {
                        dl |> add_text(
                            float2(branch_x + CalcTextSize(branch).x
                                + CalcTextSize(dirty_label).x, text_y),
                            GetColorU32(token_muted_color()), folder_tag)
                    }
                }
                popup_context_item(WORKTREE_POPUP[row],
                        (str_id = "worktree:{worktree.path}",
                         flags = ImGuiPopupFlags.MouseButtonRight)) {
                    if (menu_item(WORKTREE_MENU_NEW_SESSION[row],
                            (text = "New Session here...", shortcut = "",
                             enabled = g_client != null && g_client.connected))) {
                        // Select this worktree, then the shared command opens
                        // the launcher preselected from the selection.
                        g_selected_repository_id = repository.record.id
                        g_selected_worktree_path = worktree.path
                        g_selection_explicit = true
                        let _ = run_herd_command("session.new")
                    }
                    if (active_herd >= 0) {
                        if (menu_item(WORKTREE_MENU_OPEN_SESSION[row],
                                (text = "Open session '{g_client.herd_sessions[active_herd].name}'",
                                 shortcut = "", enabled = true))) {
                            navigate_to_herd(active_herd)
                        }
                    }
                    if (menu_item(WORKTREE_MENU_DELETE[row],
                            (text = "Retire worktree...", shortcut = "",
                             enabled = g_client != null && g_client.connected
                                && !worktree.is_main))) {
                        // Always the checklist: the verdict is what the user
                        // came for, so it is shown BEFORE the attempt rather
                        // than after a refusal (note 58's whole complaint).
                        g_worktree_retire = WorktreeRetirePrompt(active = true,
                            repository_id = repository.record.id,
                            worktree_path = worktree.path)
                    }
                }
                if (worktree.is_main) separator(WORKTREE_SEPARATOR[row])
                row++
            }
            if (empty(repository.worktrees)) {
                text_disabled(REPOSITORY_EMPTY[repository_row],
                    (text = "Waiting for Git worktree inventory..."))
            }
            separator(REPOSITORY_SEPARATOR[repository_row])
            repository_row++
        }
    }
    if (g_client == null || empty(g_client.repositories)) text_disabled("No repositories registered")
}

def agent_marks_file(repository_id, worktree_path,
                     file_path : string) : bool {
    return (g_client != null
        && g_client->marks_file_for_agent(repository_id, worktree_path, file_path))
}

def focused_file_label(repository_id, worktree_path,
                       file_path : string) : string {
    return (agent_marks_file(repository_id, worktree_path, file_path)
        ? "[AGENT] {file_path}" : file_path)
}

def changelist_rail_busy : bool {
    // The operation rail is one slot, and the PR view's single-flight action
    // takes the same index lock — either one running disables the verbs.
    return (g_client != null
        && (g_client.git_action_loading || g_client.operation.state == "running"))
}

def changelist_selection_actions(repository : RepositorySnapshot const;
                                 worktree : WorktreeState const;
                                 var stage_paths : array<string>&;
                                 var unstage_paths : array<string>&) {
    //! Splits the current selection into what each verb can act on: stage
    //! takes CONFLICTS (that is how a resolution is marked), MODIFIED and
    //! UNTRACKED; unstage takes STAGED. Groups are re-derived from the LIVE
    //! status codes, never cached — a refresh may have reclassified a row.
    delete stage_paths
    delete unstage_paths
    for (file in repository.files) {
        if (file.worktree_path != worktree.path) continue
        // No early break: an "MM" file sits in BOTH the staged and modified
        // groups, and each row's selection feeds its own verb.
        for (group in range(4)) {
            if (!git_file_in_group(file, group)) continue
            if (selection_contains(g_changelist_selection,
                    selection_key(git_group_scope(group), file.path))) {
                if (group == 1) {
                    unstage_paths |> push(file.path)
                } else {
                    stage_paths |> push(file.path)
                }
            }
        }
    }
}

def changelist_selection_discards(repository : RepositorySnapshot const;
                                  worktree : WorktreeState const;
                                  var modified_paths : array<string>&;
                                  var staged_paths : array<string>&;
                                  var untracked_paths : array<string>&;
                                  var conflicted : int&) {
    //! Splits the selection into the three discard classes — each runs a
    //! DIFFERENT git command. Conflicts are counted, never listed: discarding
    //! one side of a conflict is a separate decision, out of scope here.
    delete modified_paths
    delete staged_paths
    delete untracked_paths
    conflicted = 0
    for (file in repository.files) {
        if (file.worktree_path != worktree.path) continue
        for (group in range(4)) {
            if (!git_file_in_group(file, group)
                    || !selection_contains(g_changelist_selection,
                        selection_key(git_group_scope(group), file.path))) continue
            if (group == 0) {
                conflicted++
            } elif (group == 1) {
                staged_paths |> push(file.path)
            } elif (group == 2) {
                modified_paths |> push(file.path)
            } else {
                untracked_paths |> push(file.path)
            }
        }
    }
}

def discard_measure_file(base, rel : string) {
    let st = stat(path_join(base, rel))
    g_git_discard.display_paths |> push(rel)
    g_git_discard.file_count++
    if (st.is_valid && !st.is_dir) {
        g_git_discard.total_bytes += int64(st.size)
    }
}

def discard_measure_walk(base, rel : string) {
    let clean_rel = ends_with(rel, "/") ? slice(rel, 0, length(rel) - 1) : rel
    let full = path_join(base, clean_rel)
    let st = stat(full)
    if (!st.is_valid || !st.is_dir) {
        discard_measure_file(base, clean_rel)
        return
    }
    g_git_discard.walked_dirs++
    var inscope names : array<string>
    dir(full) $(name) {
        if (name != "." && name != "..") {
            names |> push(clone_string(name))
        }
    }
    for (name in names) {
        discard_measure_walk(base, "{clean_rel}/{name}")
    }
}

def open_discard_prompt(repository : RepositorySnapshot const;
                        worktree : WorktreeState const;
                        scope : string = "all") {
    //! scope narrows the prompt to one class of the selection: "tracked"
    //! (revert staged + unstaged edits) or "untracked" (delete files) — the
    //! two menu verbs — while "all" keeps every discardable row.
    delete g_git_discard.modified_paths
    delete g_git_discard.staged_paths
    delete g_git_discard.untracked_paths
    delete g_git_discard.display_paths
    g_git_discard.repository_id = repository.record.id
    g_git_discard.worktree_path = worktree.path
    g_git_discard.branch = worktree.branch
    g_git_discard.conflicted_count = 0
    g_git_discard.file_count = 0
    g_git_discard.total_bytes = 0l
    g_git_discard.walked_dirs = 0
    g_git_discard.confirming = false
    changelist_selection_discards(repository, worktree,
        g_git_discard.modified_paths, g_git_discard.staged_paths,
        g_git_discard.untracked_paths, g_git_discard.conflicted_count)
    if (scope == "tracked") {
        delete g_git_discard.untracked_paths
    } elif (scope == "untracked") {
        delete g_git_discard.modified_paths
        delete g_git_discard.staged_paths
    }
    for (path in g_git_discard.staged_paths) {
        discard_measure_file(worktree.path, path)
    }
    for (path in g_git_discard.modified_paths) {
        discard_measure_file(worktree.path, path)
    }
    // Untracked entries can be whole directories ("dir/" rows) — walk them so
    // the confirmation names every file git clean will actually delete.
    for (path in g_git_discard.untracked_paths) {
        discard_measure_walk(worktree.path, path)
    }
    g_git_discard.pending_open = true
}

def discard_bytes_label(total : int64) : string {
    if (total >= 1048576l) {
        return "{total / 1048576l} MB"
    }
    if (total >= 1024l) {
        return "{total / 1024l} KB"
    }
    return "{total} bytes"
}

def draw_git_discard_popup() {
    if (g_git_discard.pending_open) {
        g_git_discard.pending_open = false
        open_popup(GIT_DISCARD_DIALOG)
    }
    popup_modal(GIT_DISCARD_DIALOG.PUBLIC, (text = "Discard changes",
            closable = true, flags = ImGuiWindowFlags.AlwaysAutoResize)) {
        let modified = length(g_git_discard.modified_paths)
        let staged = length(g_git_discard.staged_paths)
        let untracked = length(g_git_discard.untracked_paths)
        text_disabled(GIT_DISCARD_PATH, (text = g_git_discard.worktree_path))
        if (staged > 0) {
            text(GIT_DISCARD_STAGED_LINE, (text =
                "{staged} staged {staged == 1 ? "file" : "files"} — index AND working tree reset to HEAD"))
        }
        if (modified > 0) {
            text(GIT_DISCARD_MODIFIED_LINE, (text =
                "{modified} edited {modified == 1 ? "file" : "files"} — unstaged edits restored from the index"))
        }
        if (untracked > 0) {
            let walked = (g_git_discard.walked_dirs > 0
                ? " ({g_git_discard.walked_dirs} {g_git_discard.walked_dirs == 1 ? "folder" : "folders"} expanded)"
                : "")
            text(GIT_DISCARD_UNTRACKED_LINE, (text =
                "{untracked} untracked {untracked == 1 ? "entry" : "entries"} deleted from disk{walked}"))
        }
        if (g_git_discard.conflicted_count > 0) {
            text_disabled(GIT_DISCARD_CONFLICT_NOTE, (text =
                "{g_git_discard.conflicted_count} conflicted " +
                "{g_git_discard.conflicted_count == 1 ? "file stays" : "files stay"}: " +
                "discarding one side of a conflict is its own decision — " +
                "resolve or stage the resolution instead."))
        }
        separator(GIT_DISCARD_LIST_SEPARATOR)
        var shown = 0
        for (path in g_git_discard.display_paths) {
            if (shown >= 20) {
                text_disabled(GIT_DISCARD_MORE, (text =
                    "+{length(g_git_discard.display_paths) - 20} more"))
                break
            }
            text_disabled(GIT_DISCARD_LIST_ROW[shown], (text = path))
            shown++
        }
        with_style((ImGuiCol.Text, token_warning_color())) {
            text(GIT_DISCARD_SENTENCE, (text =
                "{g_git_discard.file_count} {g_git_discard.file_count == 1 ? "file" : "files"}, " +
                "{discard_bytes_label(g_git_discard.total_bytes)}. These changes " +
                "exist nowhere else and cannot be recovered."))
        }
        // The non-destructive alternative comes FIRST, because it exists and
        // is one command. A user who wanted the folder clean, not the work
        // gone, should not have to know the difference.
        if (button(GIT_DISCARD_COMMIT_WIP, (text = "Commit WIP to the branch instead",
                enabled = g_client != null && !empty(g_git_discard.branch)))
                && g_client != null) {
            g_client->request_worktree_commit_wip(g_git_discard.repository_id,
                g_git_discard.worktree_path,
                "WIP: work in progress on {g_git_discard.branch}")
            close_current_popup(GIT_DISCARD_DIALOG)
        }
        if (button(GIT_DISCARD_ARM, (text =
                "Discard {g_git_discard.file_count} {g_git_discard.file_count == 1 ? "file" : "files"}...",
                enabled = g_client != null && !changelist_rail_busy()))) {
            g_git_discard.confirming = true
        }
        same_line()
        if (button(GIT_DISCARD_CANCEL, (text = "Cancel"))) {
            g_git_discard.confirming = false
            close_current_popup(GIT_DISCARD_DIALOG)
        }
        if (g_git_discard.confirming) {
            separator(GIT_DISCARD_CONFIRM_SEPARATOR)
            with_style((ImGuiCol.Text, token_warning_color())) {
                text(GIT_DISCARD_CONFIRM_TEXT, (text =
                    "Destroy these changes? There is no undo."))
            }
            if (button(GIT_DISCARD_CONFIRM_GO, (text = "Yes, discard"))
                    && g_client != null) {
                g_git_discard.confirming = false
                g_client->changelist_discard(g_git_discard.repository_id,
                    g_git_discard.worktree_path, g_git_discard.modified_paths,
                    g_git_discard.staged_paths, g_git_discard.untracked_paths)
                selection_clear(g_changelist_selection)
                close_current_popup(GIT_DISCARD_DIALOG)
            }
            same_line()
            if (button(GIT_DISCARD_CONFIRM_NO, (text = "No"))) {
                g_git_discard.confirming = false
            }
        }
    }
}

def draw_changelist_row_menu(repository : RepositorySnapshot const;
                             worktree : WorktreeState const;
                             file_row : int; busy : bool) {
    var stage_paths : array<string>
    var unstage_paths : array<string>
    changelist_selection_actions(repository, worktree, stage_paths,
        unstage_paths)
    let stage_count = length(stage_paths)
    let unstage_count = length(unstage_paths)
    let stage_word = stage_count == 1 ? "file" : "files"
    let unstage_word = unstage_count == 1 ? "file" : "files"
    if (menu_item(GIT_STATUS_STAGE_MENU[file_row],
            (text = "Stage {stage_count} {stage_word}", shortcut = "",
            enabled = !busy && stage_count > 0)) && g_client != null) {
        g_client->changelist_action(repository.record.id, worktree.path,
            "stage", stage_paths)
    }
    if (menu_item(GIT_STATUS_UNSTAGE_MENU[file_row],
            (text = "Unstage {unstage_count} {unstage_word}", shortcut = "",
            enabled = !busy && unstage_count > 0)) && g_client != null) {
        g_client->changelist_action(repository.record.id, worktree.path,
            "unstage", unstage_paths)
    }
    var modified_paths : array<string>
    var staged_paths : array<string>
    var untracked_paths : array<string>
    var conflicted = 0
    changelist_selection_discards(repository, worktree, modified_paths,
        staged_paths, untracked_paths, conflicted)
    let tracked_count = length(modified_paths) + length(staged_paths)
    let untracked_count = length(untracked_paths)
    let tracked_word = tracked_count == 1 ? "file" : "files"
    let untracked_word = untracked_count == 1 ? "file" : "files"
    // The destructive verbs live ONLY here, never on a one-pixel icon, and
    // speak per class: reverting tracked edits and deleting untracked files
    // are different acts. The ellipsis is honest: it opens the confirmation,
    // it does not act.
    if (menu_item(GIT_STATUS_DISCARD_MENU[file_row],
            (text = "Discard changes - {tracked_count} tracked {tracked_word}...",
            shortcut = "", enabled = !busy && tracked_count > 0))) {
        open_discard_prompt(repository, worktree, "tracked")
    }
    if (menu_item(GIT_STATUS_DELETE_MENU[file_row],
            (text = "Delete {untracked_count} untracked {untracked_word}...",
            shortcut = "", enabled = !busy && untracked_count > 0))) {
        open_discard_prompt(repository, worktree, "untracked")
    }
}

def draw_git_file_group(repository : RepositorySnapshot const;
                        worktree : WorktreeState const;
                        group : int; var file_row : int&) : int {
    let comparison = git_group_scope(group)
    let group_keys <- [for (file in repository.files);
        selection_key(comparison, file.path);
        where file.worktree_path == worktree.path && git_file_in_group(file, group)]
    let count = length(group_keys)
    if (count == 0) return 0
    let busy = changelist_rail_busy()
    // Tri-state select-all: a click on ALL clears, anything else selects all.
    let group_state = selection_group_state(g_changelist_selection, group_keys)
    GIT_GROUP_CHECK[group].value = group_state == 2
    var check_clicked = false
    if (group_state == 1) {
        with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.MixedValue, true) {
            check_clicked = checkbox(GIT_GROUP_CHECK[group], "##group-check-{group}")
        }
    } else {
        check_clicked = checkbox(GIT_GROUP_CHECK[group], "##group-check-{group}")
    }
    if (check_clicked) {
        selection_set_group(g_changelist_selection, repository.record.id,
            worktree.path, group, group_keys, GIT_GROUP_CHECK[group].value)
    }
    same_line(GIT_GROUP_CHECK_JOIN[group])
    let title = group == 0 ? "CONFLICTS" : (group == 1 ? "STAGED"
        : (group == 2 ? "MODIFIED" : "UNTRACKED"))
    text(GIT_GROUP_TITLE[group], (text = "{title}  {count}"))
    separator(GIT_GROUP_SEPARATOR[group])
    for (file in repository.files) {
        if (file.worktree_path != worktree.path || !git_file_in_group(file, group)) continue
        let key = selection_key(comparison, file.path)
        GIT_STATUS_ROW[file_row].value = selection_contains(
            g_changelist_selection, key)
        let old_label = !empty(file.old_path) ? "  (from {file.old_path})" : ""
        let display_path = focused_file_label(
            repository.record.id, worktree.path, file.path)
        let row_text = "{file.code}  {display_path}{old_label}"
        var selected = false
        with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.AllowOverlap, true) {
            with_style((ImGuiCol.Text, float4(0.0f, 0.0f, 0.0f, 0.0f))) {
                selected = selectable(GIT_STATUS_ROW[file_row],
                    (text = "{row_text}##status:{comparison}:{file.path}"))
            }
        }
        // Right-click on an unselected row selects it first (the file-manager
        // convention), so the menu always acts on what the user is over.
        if (IsItemClicked(ImGuiMouseButton.Right)
                && !selection_contains(g_changelist_selection, key)) {
            selection_plain_click(g_changelist_selection, repository.record.id,
                worktree.path, group, key)
        }
        // The popup must attach HERE: BeginPopupContextItem binds to the LAST
        // submitted item, and decorate_git_file_row submits an overlay icon
        // button — attached after it, right-click only worked on the tiny
        // icon (the live-review "menu is missing" bug).
        popup_context_item(GIT_STATUS_ROW_POPUP[file_row],
                (str_id = "status-menu:{comparison}:{file.path}",
                flags = ImGuiPopupFlags.MouseButtonRight)) {
            draw_changelist_row_menu(repository, worktree, file_row, busy)
        }
        var action_clicked = false
        decorate_git_file_row("status:{comparison}:{file.path}", row_text,
            file.added_lines, file.deleted_lines, file.stats_known,
            file.binary, group != 1,
            busy ? "" : (group == 1 ? "minus" : "add"),
            group == 1 ? "Unstage this file"
                : (group == 0 ? "Stage the resolution" : "Stage this file"),
            action_clicked)
        if (action_clicked && g_client != null) {
            var single <- [file.path]
            g_client->changelist_action(repository.record.id, worktree.path,
                group == 1 ? "unstage" : "stage", single)
        } elif (selected) {
            ui_event("status-file-click {comparison}:{file.path}")
            let io & = unsafe(GetIO())
            if (io.KeyShift) {
                selection_range_click(g_changelist_selection,
                    repository.record.id, worktree.path, group, group_keys, key)
            } elif (io.KeyCtrl) {
                selection_toggle_click(g_changelist_selection,
                    repository.record.id, worktree.path, group, key)
            } else {
                selection_plain_click(g_changelist_selection,
                    repository.record.id, worktree.path, group, key)
                if (g_client != null) {
                    g_client->inspect_file(repository.record.id, worktree.path,
                        file.path, comparison)
                    FILE_INSPECTOR_WIN.open = true
                    FILE_INSPECTOR_WIN.pending_raise = true
                }
            }
        }
        file_row++
    }
    dummy(GIT_GROUP_SPACING[group], (size = float2(0.0f, GetStyle().ItemSpacing.y)))
    return count
}

def draw_git_status_groups(repository : RepositorySnapshot const;
                           worktree : WorktreeState const) {
    //! The Changes view: CONFLICTS / STAGED / MODIFIED / UNTRACKED with
    //! multiselect, per-row stage/unstage, the row context menu, and the
    //! commit box under the groups.
    selection_scope(g_changelist_selection, repository.record.id, worktree.path)
    var file_row = 0
    var total = 0
    for (group in range(4)) {
        total += draw_git_file_group(repository, worktree, group, file_row)
    }
    if (total == 0) {
        text_disabled("Working tree is clean - nothing to stage, discard, or commit")
        text_disabled(GIT_CHANGES_CLEAN_HINT, (text =
            "The branch's outgoing files are under the PR result tab"))
    }
    draw_changelist_commit_box(repository, worktree)
}

def changelist_commit_message : string {
    // The widget global is module-private; the live rail reads through this.
    return COMMIT_MESSAGE_INPUT.value
}

def draw_changelist_commit_box(repository : RepositorySnapshot const;
                               worktree : WorktreeState const) {
    // The box clears only when ITS commit lands ok, watched by operation
    // revision — a stale success from an earlier operation never eats a
    // message the user is still writing.
    if (g_commit_clear_watch != 0l && g_client != null
            && g_client.operation.kind == "changelist_commit"
            && g_client.operation.revision > g_commit_clear_watch) {
        if (g_client.operation.state == "ok") {
            COMMIT_MESSAGE_INPUT.pending_value = ""
            COMMIT_MESSAGE_INPUT.has_pending = true
            g_commit_clear_watch = 0l
        } elif (g_client.operation.state != "running") {
            // Failed or cancelled: the message stays, ready to retry.
            g_commit_clear_watch = 0l
        }
    }
    separator(GIT_COMMIT_SEPARATOR)
    input_text_multiline(COMMIT_MESSAGE_INPUT, (text = "##commit-message",
        size = float2(-1.0f, GetTextLineHeight() * 4.0f)))
    let staged = worktree.staged
    let busy = changelist_rail_busy()
    let message_ready = !empty(strip(COMMIT_MESSAGE_INPUT.value))
    let detached = empty(worktree.branch)
    if (button(GIT_COMMIT_GO, (text =
            "Commit {staged} staged {staged == 1 ? "file" : "files"}",
            enabled = !busy && staged > 0 && message_ready && !detached
                && g_client != null))
            && g_client != null) {
        g_commit_clear_watch = g_client.operation.revision
        g_client->changelist_commit(repository.record.id, worktree.path,
            strip(COMMIT_MESSAGE_INPUT.value))
    }
    // A disabled button explains itself, or it teaches nothing.
    if (detached) {
        same_line()
        text_disabled(GIT_COMMIT_WHY, (text =
            "detached HEAD: a commit here would not be on any branch"))
    } elif (staged == 0) {
        same_line()
        text_disabled(GIT_COMMIT_WHY, (text =
            "nothing is staged; commit takes the STAGED group only"))
    } elif (!message_ready) {
        same_line()
        text_disabled(GIT_COMMIT_WHY, (text = "write a commit message"))
    }
}


def draw_git_stats_text(position : float2;
                        added_lines, deleted_lines : int;
                        stats_known, binary, subdued : bool) {
    let alpha = subdued ? 0.55f : 1.0f
    with_window_drawlist() $(var dl) {
        if (binary || !stats_known) {
            let stats = binary ? "binary" : "+?  -?"
            dl |> add_text(position, GetColorU32(ImGuiCol.TextDisabled), stats)
        } else {
            let added = "+{added_lines}"
            let deleted = "-{deleted_lines}"
            dl |> add_text(position,
                GetColorU32(token_added_color(alpha)), added)
            dl |> add_text(float2(position.x + CalcTextSize(added).x
                    + GetStyle().ItemSpacing.x, position.y),
                GetColorU32(token_deleted_color(alpha)), deleted)
        }
    }
}


def decorate_git_file_row(row_id, path : string;
                          added_lines, deleted_lines : int;
                          stats_known, binary, subdued : bool;
                          action_glyph, action_tip : string;
                          var action_clicked : bool&) {
    let stats = git_line_stats(added_lines, deleted_lines, stats_known, binary)
    let row_min = GetItemRectMin()
    let row_max = GetItemRectMax()
    let row_right = git_visible_row_right(row_max.x)
    // Geometric hover, not IsItemHovered: with AllowOverlap on the row the
    // overlaid action icon steals item hover, which hid the icon and killed
    // its tooltip the moment the mouse reached it.
    let hovered = IsMouseHoveringRect(row_min, row_max, true)
    let icon_size = max(12.0f, GetTextLineHeight() - 3.0f)
    let icon_width = icon_size + GetStyle().FramePadding.x * 2.0f
    let action_x = row_right - icon_width
    let stats_size = CalcTextSize(stats)
    let stats_x = max(row_min.x, action_x - stats_size.x - GetStyle().ItemSpacing.x)
    let text_y = row_min.y + max(0.0f,
        (row_max.y - row_min.y - GetTextLineHeight()) * 0.5f)
    draw_git_path_text(path,
        float2(row_min.x + GetStyle().FramePadding.x, text_y),
        float2(stats_x - GetStyle().ItemSpacing.x, row_max.y), subdued)
    draw_git_stats_text(float2(stats_x, text_y), added_lines, deleted_lines,
        stats_known, binary, subdued)
    action_clicked = false
    if (!empty(action_glyph)) {
        action_clicked = icon_button_draw_at("##{row_id}:action", action_glyph,
            float2(action_x, row_min.y), icon_size, 1.0f, hovered)
        if (hovered && IsItemHovered()) {
            set_tooltip(GIT_HOVER_TOOLTIP, action_tip)
        }
    }
}

def draw_pr_included_files(repository : RepositorySnapshot const;
                           worktree : WorktreeState const;
                           var file_row : int&) : int {
    var count = 0
    for (file in repository.review_files) {
        if (file.worktree_path == worktree.path) count++
    }
    for (file in repository.review_files) {
        if (file.worktree_path != worktree.path) continue
        let status_code = git_status_file_code(
            repository, worktree.path, file.path)
        let staged = (!empty(status_code) && status_code != "??"
            && git_status_byte(status_code, 0) != ' ')
        GIT_FILE_ROW[file_row].value = (g_inspector.repository_id == repository.record.id
            && g_inspector.worktree_path == worktree.path
            && g_inspector.file_path == file.path
            && g_inspector.comparison == "pr")
        let busy = g_client != null && g_client.git_action_loading
        var action_clicked = false
        let stats = git_line_stats(file.added_lines, file.deleted_lines,
            file.stats_known, file.binary)
        var selected = false
        with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.AllowOverlap, true) {
            with_style((ImGuiCol.Text, float4(0.0f, 0.0f, 0.0f, 0.0f))) {
                selected = selectable(GIT_FILE_ROW[file_row],
                    (text = "{file.path}  {stats}##pr:{file.path}"))
            }
        }
        let display_path = focused_file_label(
            repository.record.id, worktree.path, file.path)
        decorate_git_file_row("pr:{file.path}", display_path, file.added_lines,
            file.deleted_lines, file.stats_known, file.binary, false,
            staged && !busy ? "minus" : "",
            "Remove from the staged PR result", action_clicked)
        if (action_clicked && g_client != null) {
            g_client->git_file_action(repository.record.id,
                worktree.path, file.path, "unstage")
        } elif (selected) {
            ui_event("pr-file-click {file.path}")
            if (g_client != null) {
                g_client->inspect_file(repository.record.id, worktree.path,
                    file.path, "pr")
                FILE_INSPECTOR_WIN.open = true
                FILE_INSPECTOR_WIN.pending_raise = true
            }
        }
        file_row++
    }
    return count
}

def draw_pr_not_included_files(repository : RepositorySnapshot const;
                               worktree : WorktreeState const;
                               var file_row : int&) : int {
    var count = 0
    for (file in repository.files) {
        if (file.worktree_path != worktree.path) continue
        if (file.code == "??" || git_status_byte(file.code, 1) != ' ') count++
    }
    for (file in repository.files) {
        if (file.worktree_path != worktree.path
                || (file.code != "??" && git_status_byte(file.code, 1) == ' ')) continue
        GIT_FILE_ROW[file_row].value = (g_inspector.repository_id == repository.record.id
            && g_inspector.worktree_path == worktree.path
            && g_inspector.file_path == file.path
            && g_inspector.comparison == "working")
        let busy = g_client != null && g_client.git_action_loading
        var action_clicked = false
        let stats = git_line_stats(file.added_lines, file.deleted_lines,
            file.stats_known, file.binary)
        var selected = false
        with_item_flag(ImGuiItemFlags.None | ImGuiItemFlagsPrivate.AllowOverlap, true) {
            with_style((ImGuiCol.Text, float4(0.0f, 0.0f, 0.0f, 0.0f))) {
                selected = selectable(GIT_FILE_ROW[file_row],
                    (text = "{file.path}  {stats}##working:{file.path}"))
            }
        }
        let display_path = focused_file_label(
            repository.record.id, worktree.path, file.path)
        decorate_git_file_row("working:{file.path}", display_path, file.added_lines,
            file.deleted_lines, file.stats_known, file.binary, true,
            busy ? "" : "check", "Add to the staged PR result",
            action_clicked)
        if (action_clicked && g_client != null) {
            g_client->git_file_action(repository.record.id,
                worktree.path, file.path, "stage")
        } elif (selected && g_client != null) {
            ui_event("working-file-click {file.path}")
            g_client->inspect_file(repository.record.id, worktree.path,
                file.path, "working")
            FILE_INSPECTOR_WIN.open = true
            FILE_INSPECTOR_WIN.pending_raise = true
        }
        file_row++
    }
    return count
}

def draw_shared_pr_files(repository : RepositorySnapshot const;
                         worktree : WorktreeState const) {
    var file_row = 0
    let not_included = draw_pr_not_included_files(
        repository, worktree, file_row)
    if (not_included > 0) {
        separator(GIT_GROUP_SEPARATOR[11])
    }
    draw_pr_included_files(repository, worktree, file_row)
}

def draw_sha_copy_icon(hash, row_id : string;
                       visible_sha : string = "") {
    let row_min = GetItemRectMin()
    let row_max = GetItemRectMax()
    let row_right = git_visible_row_right(row_max.x)
    let icon_size = max(12.0f, GetTextLineHeight() - 3.0f)
    let icon_width = icon_size + GetStyle().FramePadding.x * 2.0f
    let icon_x = (empty(visible_sha)
        ? row_right - icon_width
        : row_min.x + GetStyle().FramePadding.x
            + CalcTextSize(visible_sha).x + GetStyle().ItemSpacing.x)
    // Geometric hover — AllowOverlap rows lose item hover to the overlay icon.
    let hovered = IsMouseHoveringRect(row_min, row_max, true) && (empty(visible_sha)
        || GetMousePos().x <= icon_x + icon_width)
    if (icon_button_draw_at("##{row_id}:copy-sha", "duplicate",
            float2(icon_x, row_min.y), icon_size, 1.0f,
            hovered)) {
        queue_sha_copy(hash)
    }
    if (hovered && IsItemHovered()) {
        set_tooltip(GIT_HOVER_TOOLTIP, "Copy full SHA")
    }
}

def draw_pr_review(repository : RepositorySnapshot const;
                   worktree : WorktreeState const) {
    let base = (!empty(worktree.review_base_ref)
        ? worktree.review_base_ref : "detecting base...")
    draw_perspective_banner("PR", "{worktree.branch}  ->  {base}")
    var commit_row = 0
    let sha_width = (CalcTextSize("000000000").x
        + GetFrameHeightWithSpacing() + 8.0f)
    let table_flags = (ImGuiTableFlags.Resizable | ImGuiTableFlags.RowBg |
        ImGuiTableFlags.BordersInnerV | ImGuiTableFlags.ScrollY |
        ImGuiTableFlags.SizingStretchProp)
    data_table(GIT_PR_COMMIT_TABLE, (text = "##pr-commits", columns = 3,
            flags = table_flags, outer_size = float2(0.0f, 0.0f),
            inner_width = 0.0f)) {
        table_setup_scroll_freeze(0, 1)
        table_setup_column("SHA", ImGuiTableColumnFlags.WidthFixed, sha_width)
        table_setup_column("Title", ImGuiTableColumnFlags.WidthStretch, 0.72f)
        table_setup_column("Author", ImGuiTableColumnFlags.WidthStretch, 0.28f)
        table_headers_row()
        for (commit in repository.review_commits) {
            if (commit.worktree_path != worktree.path) continue
            table_next_row()
            table_set_column_index(0)
            with_style((ImGuiCol.Text, token_sha_color())) {
                selectable_hover(GIT_PR_COMMIT_HOVER[commit_row],
                    (text = "{commit.short_hash}##pr:{commit.hash}",
                    flags = ImGuiSelectableFlags.SpanAllColumns |
                        ImGuiSelectableFlags.AllowOverlap))
            }
            draw_sha_copy_icon(commit.hash, "pr-commit:{commit.hash}",
                commit.short_hash)
            table_set_column_index(1)
            text_colored(GIT_COMMIT_TITLE_TEXT[commit_row],
                (color = token_title_color(),
                text = commit.subject))
            table_set_column_index(2)
            text_colored(GIT_COMMIT_AUTHOR_TEXT[commit_row],
                (color = token_muted_color(),
                text = commit.author))
            commit_row++
        }
    }
}

def ensure_history(repository : RepositorySnapshot const;
                   worktree : WorktreeState const; ref_name : string) {
    if (g_client == null || empty(ref_name)) return
    let identity = "{repository.record.id}\n{worktree.path}\n{ref_name}"
    if (g_client.last_history_identity != identity) {
        g_client->request_repository_history(
            repository.record.id, worktree.path, ref_name)
    }
}

def request_commit_files(repository : RepositorySnapshot const;
                         worktree : WorktreeState const;
                         commit_hash : string) {
    if (g_client == null || !g_client.connected || empty(commit_hash)) return
    g_requested_history_commit = commit_hash
    g_client->request_repository_commit_files(
        repository.record.id, worktree.path, commit_hash)
}

def draw_commit_files(repository : RepositorySnapshot const;
                      worktree : WorktreeState const) {
    if (!empty(g_requested_history_commit)
            && g_requested_history_commit == worktree.history_commit) {
        g_requested_history_commit = ""
    }
    let selected_commit = !empty(g_requested_history_commit) ? g_requested_history_commit : worktree.history_commit
    if (!empty(g_requested_history_commit)) return
    var row = 0
    for (file in repository.commit_files) {
        if (file.worktree_path != worktree.path
                || file.commit_hash != selected_commit) continue
        GIT_COMMIT_FILE_ROW[row].value = (g_inspector.repository_id == repository.record.id
            && g_inspector.worktree_path == worktree.path
            && g_inspector.file_path == file.path
            && g_inspector.comparison == "commit"
            && g_inspector.git_revision == file.commit_hash)
        let stats = git_line_stats(file.added_lines, file.deleted_lines,
            file.stats_known, file.binary)
        var selected = false
        with_style((ImGuiCol.Text, float4(0.0f, 0.0f, 0.0f, 0.0f))) {
            selected = selectable(GIT_COMMIT_FILE_ROW[row],
                (text = "{file.path}  {stats}##commit:{file.commit_hash}:{file.path}"))
        }
        var ignored_action = false
        decorate_git_file_row("commit:{file.commit_hash}:{file.path}",
            file.path, file.added_lines, file.deleted_lines,
            file.stats_known, file.binary, false, "", "", ignored_action)
        if (selected && g_client != null) {
            ui_event("commit-file-click {file.path}")
            g_client->inspect_file(repository.record.id, worktree.path,
                file.path, "commit", "compact", file.commit_hash)
            FILE_INSPECTOR_WIN.open = true
            FILE_INSPECTOR_WIN.pending_raise = true
        }
        row++
    }
}

def draw_history_commits(repository : RepositorySnapshot const;
                         worktree : WorktreeState const) {
    var inscope commit_indices : array<int>
    commit_indices |> reserve(length(repository.history_commits))
    for (index in range(length(repository.history_commits))) {
        if (repository.history_commits[index].worktree_path == worktree.path) {
            commit_indices |> push(index)
        }
    }
    // The branch/base delimiter (note 22): rows above it are outgoing (the PR
    // view's review set); the first row outside that set starts base history.
    var inscope outgoing : table<string>
    for (commit in repository.review_commits) {
        if (commit.worktree_path == worktree.path) {
            outgoing |> insert(commit.hash)
        }
    }
    var boundary_row = -1
    if (!empty(outgoing)) {
        for (position, index in count(), commit_indices) {
            if (!key_exists(outgoing, repository.history_commits[index].hash)) {
                boundary_row = position
                break
            }
        }
    }
    if (boundary_row == 0) {
        boundary_row = -1
    }
    let count = length(commit_indices)
    let total = count + (boundary_row >= 0 ? 1 : 0)
    let sha_width = (CalcTextSize("000000000").x
        + GetFrameHeightWithSpacing() + 8.0f)
    let table_flags = (ImGuiTableFlags.Resizable | ImGuiTableFlags.RowBg |
        ImGuiTableFlags.BordersInnerV | ImGuiTableFlags.ScrollY |
        ImGuiTableFlags.SizingStretchProp)
    data_table(GIT_HISTORY_COMMIT_TABLE, (text = "##history-commits", columns = 3,
            flags = table_flags, outer_size = float2(0.0f, 0.0f),
            inner_width = 0.0f)) {
        table_setup_scroll_freeze(0, 1)
        table_setup_column("SHA", ImGuiTableColumnFlags.WidthFixed, sha_width)
        table_setup_column("Title", ImGuiTableColumnFlags.WidthStretch, 0.72f)
        table_setup_column("Author", ImGuiTableColumnFlags.WidthStretch, 0.28f)
        table_headers_row()
        list_clipper(total) $(start, end_) {
            for (row in range(start, end_)) {
                if (boundary_row >= 0 && row == boundary_row) {
                    let base = (!empty(worktree.review_base_ref)
                        ? worktree.review_base_ref : "base")
                    table_next_row()
                    table_set_column_index(0)
                    text_disabled(HISTORY_BASE_MARK, (text = "base"))
                    table_set_column_index(1)
                    text_colored(HISTORY_BASE_TEXT,
                        (color = token_branch_color(),
                            text = "^ outgoing on {worktree.branch}    v {base}"))
                    continue
                }
                let commit_row = ((boundary_row >= 0 && row > boundary_row)
                    ? row - 1 : row)
                let commit = repository.history_commits[commit_indices[commit_row]]
                let selected_commit = !empty(g_requested_history_commit) ? g_requested_history_commit : worktree.history_commit
                table_next_row()
                table_set_column_index(0)
                var selected = false
                with_style((ImGuiCol.Text, token_sha_color())) {
                    selected = selectable_label(GIT_HISTORY_COMMIT_ROW[row],
                        (text = "{commit.short_hash}##history:{commit.hash}",
                        selected = commit.hash == selected_commit,
                        flags = ImGuiSelectableFlags.SpanAllColumns |
                            ImGuiSelectableFlags.AllowOverlap))
                }
                draw_sha_copy_icon(commit.hash, "history:{commit.hash}",
                    commit.short_hash)
                table_set_column_index(1)
                text_colored(GIT_COMMIT_TITLE_TEXT[row],
                    (color = token_title_color(),
                    text = commit.subject))
                table_set_column_index(2)
                text_colored(GIT_COMMIT_AUTHOR_TEXT[row],
                    (color = token_muted_color(),
                    text = commit.author))
                if (selected && g_client != null && commit.hash != selected_commit) {
                    ui_event("history-commit-click {commit.short_hash}")
                    request_commit_files(repository, worktree, commit.hash)
                }
            }
        }
    }
}

// Loud active-perspective line (note 22): only the selected tab's body runs,
// so one shared widget set renders exactly once per frame.
def draw_perspective_banner(name, context : string) {
    text_colored(PERSPECTIVE_BADGE,
        (color = token_warning_color(), text = name))
    same_line(PERSPECTIVE_JOIN)
    text_colored(PERSPECTIVE_CONTEXT,
        (color = token_branch_color(), text = context))
    separator(PERSPECTIVE_SEPARATOR)
}

def draw_history_review(repository : RepositorySnapshot const;
                        worktree : WorktreeState const) {
    draw_perspective_banner("HISTORY",
        "{worktree.branch} - outgoing first, then base")
    ensure_history(repository, worktree, worktree.branch)
    draw_history_commits(repository, worktree)
}

def git_graph_color(slot : int; alpha : float = 1.0f) : uint {
    let palette_slot = slot % 12
    if (palette_slot == 0) return GetColorU32(float4(0.31f, 0.72f, 0.96f, alpha))
    if (palette_slot == 1) return GetColorU32(float4(0.95f, 0.50f, 0.34f, alpha))
    if (palette_slot == 2) return GetColorU32(float4(0.48f, 0.82f, 0.42f, alpha))
    if (palette_slot == 3) return GetColorU32(float4(0.76f, 0.54f, 0.95f, alpha))
    if (palette_slot == 4) return GetColorU32(float4(0.97f, 0.76f, 0.28f, alpha))
    if (palette_slot == 5) return GetColorU32(float4(0.29f, 0.84f, 0.74f, alpha))
    if (palette_slot == 6) return GetColorU32(float4(0.95f, 0.43f, 0.65f, alpha))
    if (palette_slot == 7) return GetColorU32(float4(0.55f, 0.66f, 0.98f, alpha))
    if (palette_slot == 8) return GetColorU32(float4(0.76f, 0.82f, 0.35f, alpha))
    if (palette_slot == 9) return GetColorU32(float4(0.98f, 0.60f, 0.27f, alpha))
    if (palette_slot == 10) return GetColorU32(float4(0.39f, 0.78f, 0.87f, alpha))
    return GetColorU32(float4(0.80f, 0.48f, 0.79f, alpha))
}

def current_tree_filter() : GitTreeFilter {
    if (GIT_TREE_FILTER.value == 1) return GitTreeFilter.all
    return GitTreeFilter.auto_
}

def ensure_tree_model(repository : RepositorySnapshot const;
                      worktree : WorktreeState const) {
    let filter = current_tree_filter()
    let identity = "{repository.record.id}\n{worktree.path}\n{repository.revision}\n{int(filter)}\n{g_tree_focus_ref}"
    if (identity == g_tree_model_identity) return
    git_tree_build_model(repository, worktree, filter,
        g_tree_focus_ref, g_tree_model)
    g_tree_model_identity = identity
}

def draw_git_tree_vectors(anchor_x, row_zero_y, row_height,
                          lane_spacing : float) {
    with_window_drawlist() $(var dl) {
        for (edge in g_tree_model.edges) {
            if (edge.child < 0 || edge.child >= length(g_tree_model.nodes)
                    || edge.parent < 0 || edge.parent >= length(g_tree_model.nodes)) continue
            let child = g_tree_model.nodes[edge.child]
            let parent = g_tree_model.nodes[edge.parent]
            let x0 = anchor_x + float(child.lane) * lane_spacing
            let x1 = anchor_x + float(parent.lane) * lane_spacing
            let y0 = row_zero_y + float(child.row) * row_height
            let y1 = row_zero_y + float(parent.row) * row_height
            let alpha = edge.focused ? 0.98f : 0.23f
            let thickness = edge.focused ? 2.5f : 1.45f
            let color = git_graph_color(edge.color_slot, alpha)
            if (abs(x1 - x0) < 0.5f) {
                dl |> add_line(float2(x0, y0), float2(x1, y1),
                    color, thickness)
            } else {
                let transition_y = min(y1,
                    y0 + clamp((y1 - y0) * 0.45f,
                        row_height, row_height * 1.8f))
                let control = (transition_y - y0) * 0.62f
                dl |> add_bezier_cubic(float2(x0, y0),
                    float2(x0, y0 + control),
                    float2(x1, transition_y - control),
                    float2(x1, transition_y), color, thickness, 0)
                if (transition_y < y1) {
                    dl |> add_line(float2(x1, transition_y),
                        float2(x1, y1), color, thickness)
                }
            }
        }
        for (node in g_tree_model.nodes) {
            let x = anchor_x + float(node.lane) * lane_spacing
            let y = row_zero_y + float(node.row) * row_height
            let alpha = node.focused ? 1.0f : 0.34f
            let color = git_graph_color(node.color_slot, alpha)
            let radius = node.head ? 5.8f : 4.2f
            if (node.head) {
                dl |> add_circle_filled(float2(x, y), radius, color, 20)
            } else {
                dl |> add_circle_filled(float2(x, y), radius,
                    GetColorU32(ImGuiCol.WindowBg), 20)
            }
            dl |> add_circle(float2(x, y), radius, color, 20,
                node.pushed ? 2.8f : 1.35f)
            if (node.focus_head) {
                dl |> add_circle(float2(x, y), radius + 3.0f,
                    git_graph_color(node.color_slot, 0.78f), 20, 1.25f)
            }
        }
    }
}

def draw_tree_commits(repository : RepositorySnapshot const;
                      worktree : WorktreeState const) {
    ensure_tree_model(repository, worktree)
    let sha_width = (CalcTextSize("000000000").x
        + GetFrameHeightWithSpacing() + 8.0f)
    let lane_spacing = max(15.0f, 17.0f * float(g_zoom_percent) / 100.0f)
    let topology_width = clamp(float(g_tree_model.max_lane + 1) *
        lane_spacing + 30.0f, 130.0f, 420.0f)
    let table_flags = (ImGuiTableFlags.Resizable | ImGuiTableFlags.RowBg |
        ImGuiTableFlags.BordersInnerV | ImGuiTableFlags.ScrollY |
        ImGuiTableFlags.SizingStretchProp)
    let row_height = GetTextLineHeightWithSpacing()
    var graph_anchor = false
    var graph_anchor_x = 0.0f
    var graph_row_zero_y = 0.0f
    data_table(GIT_TREE_COMMIT_TABLE, (text = "##tree-commits", columns = 4,
            flags = table_flags, outer_size = float2(0.0f, 0.0f),
            inner_width = 0.0f)) {
        table_setup_scroll_freeze(0, 1)
        table_setup_column("SHA", ImGuiTableColumnFlags.WidthFixed, sha_width)
        table_setup_column("Title", ImGuiTableColumnFlags.WidthStretch, 0.70f)
        table_setup_column("Author", ImGuiTableColumnFlags.WidthStretch, 0.30f)
        table_setup_column("Topology", ImGuiTableColumnFlags.WidthFixed,
            topology_width)
        table_headers_row()
        list_clipper(length(g_tree_model.nodes)) $(start, end_) {
            for (row in range(start, end_)) {
                let node = g_tree_model.nodes[row]
                let commit = repository.history_commits[node.commit_index]
                let selected_commit = (!empty(g_requested_history_commit)
                    ? g_requested_history_commit : worktree.history_commit)
                let text_alpha = node.focused ? 1.0f : 0.42f
                table_next_row(ImGuiTableRowFlags.None, row_height)
                table_set_column_index(0)
                var selected = false
                with_style((ImGuiCol.Text,
                        token_sha_color(text_alpha))) {
                    selected = selectable_label(GIT_HISTORY_COMMIT_ROW[row],
                        (text = "{commit.short_hash}##tree:{commit.hash}",
                        selected = commit.hash == selected_commit,
                        flags = ImGuiSelectableFlags.SpanAllColumns |
                            ImGuiSelectableFlags.AllowOverlap))
                }
                draw_sha_copy_icon(commit.hash, "tree:{commit.hash}",
                    commit.short_hash)
                table_set_column_index(1)
                if (!empty(node.refs_text)) {
                    let ref_size = float2(CalcTextSize(node.refs_text).x + 8.0f,
                        GetTextLineHeight())
                    var ref_clicked = false
                    with_style(
                            (ImGuiCol.Text,
                                float4(0.52f, 0.82f, 0.53f, text_alpha)),
                            (ImGuiCol.Header,
                                float4(0.20f, 0.36f, 0.25f, 0.62f)),
                            (ImGuiCol.HeaderHovered,
                                float4(0.25f, 0.48f, 0.31f, 0.72f))) {
                        ref_clicked = selectable_label(GIT_TREE_REF_ROW[row],
                            (text = "{node.refs_text}##tree-ref:{node.hash}",
                            selected = node.primary_ref == g_tree_model.focus_ref,
                            flags = ImGuiSelectableFlags.AllowOverlap,
                            size = ref_size))
                    }
                    if (ref_clicked && !empty(node.primary_ref)) {
                        ui_event("tree-ref-click {node.primary_ref}")
                        g_tree_focus_ref = node.primary_ref
                        g_tree_model_identity = ""
                    }
                    same_line(GIT_TREE_REF_SAME_LINE[row])
                }
                text_colored(GIT_COMMIT_TITLE_TEXT[row],
                    (color = token_title_color(text_alpha),
                    text = commit.subject))
                table_set_column_index(2)
                text_colored(GIT_COMMIT_AUTHOR_TEXT[row],
                    (color = token_muted_color(text_alpha),
                    text = commit.author))
                if (selected && g_client != null
                        && commit.hash != selected_commit) {
                    ui_event("tree-commit-click {commit.short_hash}")
                    request_commit_files(repository, worktree, commit.hash)
                }
                table_set_column_index(3)
                dummy(GIT_TREE_GRAPH_CELL[row],
                    (size = float2(0.0f, GetTextLineHeight())))
                if (!graph_anchor) {
                    let row_min = GetItemRectMin()
                    let row_max = GetItemRectMax()
                    graph_anchor = true
                    graph_anchor_x = (row_min.x + GetStyle().CellPadding.x
                        + lane_spacing * 0.5f)
                    graph_row_zero_y = ((row_min.y + row_max.y) * 0.5f
                        - float(row) * row_height)
                }
            }
        }
        if (graph_anchor) {
            draw_git_tree_vectors(graph_anchor_x, graph_row_zero_y,
                row_height, lane_spacing)
        }
    }
}

def draw_tree_review(repository : RepositorySnapshot const;
                     worktree : WorktreeState const) {
    draw_perspective_banner("TREE", worktree.branch)
    if (g_client != null) {
        let identity = "{repository.record.id}\n{worktree.path}"
        if (g_client.last_refs_identity != identity) {
            g_client->request_repository_refs(repository.record.id, worktree.path)
        }
    }
    ensure_history(repository, worktree, "__all__")
    SetNextItemWidth(130.0f)
    combo(GIT_TREE_FILTER.PUBLIC, (text = "Branches", items <- TREE_FILTER_LABELS))
    same_line()
    ensure_tree_model(repository, worktree)
    text_disabled(GIT_TREE_FOCUS_TEXT,
        (text = "Focus: {g_tree_model.focus_ref}   refs {g_tree_model.included_ref_count}"))
    draw_tree_commits(repository, worktree)
}

def git_activity_status(repository : RepositorySnapshot const;
                        worktree : WorktreeState const) : GitWindowStatus {
    if (g_client == null || !g_client.connected) {
        return GitWindowStatus(text = "WATCHER DISCONNECTED - reconnecting...", error = true)
    }
    if (GIT_TREE_TAB.selected) {
        if (!empty(worktree.refs_error)) {
            return GitWindowStatus(text = worktree.refs_error, error = true)
        }
        if (!empty(worktree.history_error)) {
            return GitWindowStatus(text = worktree.history_error, error = true)
        }
        if (worktree.refs_loading) {
            return GitWindowStatus(text = "Reading branches...", busy = true)
        }
        if (worktree.history_loading) {
            return GitWindowStatus(text = "Reading Git history...", busy = true)
        }
        var mode = "AUTO"
        if (GIT_TREE_FILTER.value == 1) {
            mode = "ALL"
        }
        return GitWindowStatus(text = "TREE {mode}   COMMITS {length(g_tree_model.nodes)}   REFS {g_tree_model.included_ref_count}")
    }
    if (GIT_HISTORY_TAB.selected) {
        if (!empty(worktree.history_error)) {
            return GitWindowStatus(text = worktree.history_error, error = true)
        }
        if (worktree.history_loading) {
            return GitWindowStatus(text = "Reading Git history...", busy = true)
        }
        return GitWindowStatus(text = "COMMITS ON {worktree.history_ref}  {git_history_commit_count(repository, worktree)}")
    }
    if (GIT_PROJECT_TAB.selected) {
        if (!empty(g_worktree_files_error)) {
            return GitWindowStatus(text = g_worktree_files_error, error = true)
        }
        if (g_worktree_files_loading) {
            return GitWindowStatus(text = "Reading file list...", busy = true)
        }
        return GitWindowStatus(text = "PROJECT FILES  {length(g_worktree_files)}")
    }
    if (!empty(worktree.review_error)) {
        return GitWindowStatus(text = worktree.review_error, error = true)
    }
    if (worktree.review_loading) {
        return GitWindowStatus(text = "Reading outgoing commits...", busy = true)
    }
    let base = (!empty(worktree.review_base_ref)
        ? worktree.review_base_ref : "detecting base...")
    // An empty PR table with a dirty worktree is not "no work" — say where
    // the uncommitted changes live instead of showing a silent empty view.
    let dirty = worktree.staged + worktree.unstaged + worktree.untracked
    let dirty_hint = dirty > 0 ? "   {dirty} uncommitted in Changelist" : ""
    return GitWindowStatus(text = "OUTGOING COMMITS  {git_review_commit_count(repository, worktree)}   -> {base}{dirty_hint}")
}

def git_changelist_status(repository : RepositorySnapshot const;
                          worktree : WorktreeState const) : GitWindowStatus {
    if (g_client == null || !g_client.connected) {
        return GitWindowStatus(text = "WATCHER DISCONNECTED - reconnecting...", error = true)
    }
    if (!empty(repository.error)) {
        return GitWindowStatus(text = repository.error, error = true)
    }
    if (g_client != null && g_client.git_action_loading) {
        let action = (empty(g_client.git_action_status)
            ? "Running Git..." : g_client.git_action_status)
        return GitWindowStatus(text = action, busy = true)
    }
    if (g_client != null && !empty(g_client.changelist_notice)) {
        return GitWindowStatus(text = g_client.changelist_notice, error = true)
    }
    if (g_client != null && starts_with(g_client.operation.kind, "changelist_")) {
        if (g_client.operation.state == "running") {
            return GitWindowStatus(text = g_client.operation.step, busy = true)
        }
        if (g_client.operation.state == "failed") {
            // The rail's failure, with git's words; the hint rides the error
            // context menu the status line already offers.
            return GitWindowStatus(text = g_client.operation.error, error = true)
        }
    }
    if (repository.refreshing) {
        return GitWindowStatus(text = "Refreshing repository...", busy = true)
    }
    if (GIT_HISTORY_TAB.selected || GIT_TREE_TAB.selected) {
        if (!empty(worktree.history_error)) {
            return GitWindowStatus(text = worktree.history_error, error = true)
        }
        if (!empty(g_requested_history_commit)) {
            return GitWindowStatus(text = "Loading selected commit files...", busy = true)
        }
        let commit_hash = worktree.history_commit
        if (empty(commit_hash)) {
            return GitWindowStatus(text = "Select a commit to inspect its files")
        }
        let short_hash = slice(commit_hash, 0, min(9, length(commit_hash)))
        return GitWindowStatus(text = "FILES IN {short_hash}  {git_selected_commit_file_count(repository, worktree, commit_hash)}")
    }
    if (!empty(worktree.review_error)) {
        return GitWindowStatus(text = worktree.review_error, error = true)
    }
    if (worktree.review_loading) {
        return GitWindowStatus(text = "Refreshing worktree PR...", busy = true)
    }
    let ahead_behind = ((worktree.ahead != 0 || worktree.behind != 0)
        ? "   ahead {worktree.ahead}   behind {worktree.behind}" : "")
    return GitWindowStatus(text = "staged {worktree.staged}   unstaged {worktree.unstaged}   untracked {worktree.untracked}   conflicted {worktree.conflicted}{ahead_behind}")
}

def network_ready : bool {
    return (g_client != null && g_client.connected && !changelist_rail_busy())
}

def draw_changelist_network_row(repository : RepositorySnapshot const;
                                worktree : WorktreeState const) {
    //! The network four. Counted labels ("Push 3", "Pull 12") instead of
    //! blind verbs, from the ahead/behind the status branch line already
    //! carries; a branch with no upstream turns Push into an explicit
    //! Publish that names the remote it will create.
    let has_upstream = !empty(worktree.upstream_ref)
    let detached = empty(worktree.branch)
    if (button(GIT_NET_FETCH, (text = "Fetch",
            enabled = network_ready())) && g_client != null) {
        g_client->changelist_network(repository.record.id, worktree.path,
            "fetch")
    }
    same_line()
    let pull_label = worktree.behind > 0 ? "Pull {worktree.behind}" : "Pull"
    if (button(GIT_NET_PULL, (text = pull_label,
            enabled = network_ready() && has_upstream))
            && g_client != null) {
        g_client->changelist_network(repository.record.id, worktree.path,
            "pull")
    }
    same_line()
    let push_label = (has_upstream
        ? (worktree.ahead > 0 ? "Push {worktree.ahead}" : "Push")
        : "Publish branch")
    if (button(GIT_NET_PUSH, (text = push_label,
            enabled = network_ready() && !detached
                && (!has_upstream || worktree.ahead > 0)))
            && g_client != null) {
        g_client->changelist_network(repository.record.id, worktree.path,
            "push")
    }
    same_line()
    if (button(GIT_NET_SYNC, (text = "Sync",
            enabled = network_ready() && has_upstream && !detached))
            && g_client != null) {
        g_client->changelist_network(repository.record.id, worktree.path,
            "sync")
    }
    if (!has_upstream && !detached) {
        same_line()
        text_disabled(GIT_NET_NOTE, (text =
            "no upstream - Publish creates origin/{worktree.branch}"))
    } elif (detached) {
        same_line()
        text_disabled(GIT_NET_NOTE, (text = "detached HEAD - nothing to push"))
    }
}

def draw_git_changelist_window() {
    var status = GitWindowStatus(text = "No worktree selected")
    var found = false
    child(GIT_CHANGELIST_CONTENT, (text = "changelist content",
            size = float2(0.0f, -GetFrameHeightWithSpacing()),
            child_flags = ImGuiChildFlags.None,
            window_flags = ImGuiWindowFlags.None)) {
        if (g_client != null) {
            for (repository in g_client.repositories) {
                if (repository.record.id != g_selected_repository_id) continue
                for (worktree in repository.worktrees) {
                    if (worktree.path != g_selected_worktree_path) continue
                    found = true
                    let identity = worktree_identity_label(worktree.branch,
                        worktree.path)
                    let title = worktree.is_main ? "MAIN - {identity}" : identity
                    text(GIT_TITLE, (text = title))
                    same_line()
                    if (icon_button(REFRESH_GIT, (glyph = "refresh",
                            tip = "Refresh the worktree status and PR review",
                            size = GetTextLineHeight()))
                            && !repository.refreshing) {
                        g_client->refresh_repository(repository.record.id)
                        g_client->request_repository_review(
                            repository.record.id, worktree.path)
                        g_client->request_repository_history(
                            repository.record.id, worktree.path, worktree.branch)
                        g_client->request_repository_refs(
                            repository.record.id, worktree.path)
                    }
                    text_disabled(GIT_WORKTREE_PATH, (text = worktree.path))
                    draw_changelist_network_row(repository, worktree)
                    separator()
                    if (GIT_HISTORY_TAB.selected || GIT_TREE_TAB.selected) {
                        draw_commit_files(repository, worktree)
                    } else {
                        tab_bar(GIT_CHANGELIST_TABS,
                            (text = "Changelist views", flags = ImGuiTabBarFlags.None)) {
                            tab_item(CHANGELIST_CHANGES_TAB.PUBLIC,
                                (text = "Changes", closable = false,
                                flags = ImGuiTabItemFlags.None)) {
                                draw_git_status_groups(repository, worktree)
                            }
                            tab_item(CHANGELIST_PR_TAB.PUBLIC,
                                (text = "PR result", closable = false,
                                flags = ImGuiTabItemFlags.None)) {
                                draw_shared_pr_files(repository, worktree)
                            }
                        }
                    }
                    status = git_changelist_status(repository, worktree)
                }
            }
        }
        if (!found) text_disabled("Select a worktree to inspect its changelist")
        draw_git_discard_popup()
    }
    separator(GIT_CHANGELIST_STATUS_SEPARATOR)
    if (status.error || status.busy) {
        icon(status.error ? "warning" : "clock", GetTextLineHeight())
        same_line()
    }
    text_colored(GIT_CHANGELIST_STATUS_TEXT,
        (color = git_status_color(status), text = status.text))
    if (status.error) {
        draw_error_context(CHANGELIST_ERROR_POPUP, CHANGELIST_ERROR_COPY,
            CHANGELIST_ERROR_SEND, "changelist_error", "Git Changelist",
            status.text, CHANGELIST_ERROR_FIX, g_selected_worktree_path)
    }
}

def draw_git_activity_window() {
    var status = GitWindowStatus(text = "No worktree selected")
    var found = false
    child(GIT_ACTIVITY_CONTENT, (text = "activity content",
            size = float2(0.0f, -GetFrameHeightWithSpacing()),
            child_flags = ImGuiChildFlags.None,
            window_flags = ImGuiWindowFlags.None)) {
        if (g_client != null) {
            for (repository in g_client.repositories) {
                if (repository.record.id != g_selected_repository_id) continue
                for (worktree in repository.worktrees) {
                    if (worktree.path != g_selected_worktree_path) continue
                    found = true
                    tab_bar(GIT_PERSPECTIVE_TABS,
                        (text = "Git perspectives", flags = ImGuiTabBarFlags.None)) {
                        tab_item(GIT_PR_TAB.PUBLIC, (text = "PR", closable = false,
                            flags = ImGuiTabItemFlags.None)) {
                            draw_pr_review(repository, worktree)
                        }
                        tab_item(GIT_HISTORY_TAB.PUBLIC, (text = "History", closable = false,
                            flags = ImGuiTabItemFlags.None)) {
                            draw_history_review(repository, worktree)
                        }
                        tab_item(GIT_TREE_TAB.PUBLIC, (text = "Tree", closable = false,
                            flags = ImGuiTabItemFlags.None)) {
                            draw_tree_review(repository, worktree)
                        }
                        tab_item(GIT_PROJECT_TAB.PUBLIC, (text = "Project", closable = false,
                            flags = ImGuiTabItemFlags.None)) {
                            draw_perspective_banner("PROJECT", worktree.path)
                            draw_project_files()
                        }
                    }
                    status = git_activity_status(repository, worktree)
                }
            }
        }
        if (!found) text_disabled("Select a worktree to inspect Git activity")
    }
    separator(GIT_ACTIVITY_STATUS_SEPARATOR)
    if (status.error || status.busy) {
        icon(status.error ? "warning" : "clock", GetTextLineHeight())
        same_line()
    }
    text_colored(GIT_ACTIVITY_STATUS_TEXT,
        (color = git_status_color(status), text = status.text))
    if (status.error) {
        draw_error_context(ACTIVITY_ERROR_POPUP, ACTIVITY_ERROR_COPY,
            ACTIVITY_ERROR_SEND, "activity_error", "Git Activity",
            status.text, ACTIVITY_ERROR_FIX, g_selected_worktree_path)
    }
}

def git_ui_init() {
    GIT_TREE_FILTER.value = 0
    // Commit messages outgrow the 256-byte default; sized before first frame.
    COMMIT_MESSAGE_INPUT.capacity = 4096
}

def draw_repositories_dock() {
    dock_window(REPOSITORIES_WIN.PUBLIC, (text = "Git Repositories & Worktrees###Repositories", closable = true,
            flags = ImGuiWindowFlags.None)) {
        {
            draw_repositories_window()
        }
    }
}

def draw_git_activity_dock() {
    dock_window(GIT_ACTIVITY_WIN.PUBLIC, (text = "Git Activity", closable = true,
            flags = ImGuiWindowFlags.None)) {
        {
            draw_git_activity_window()
        }
    }
}

def draw_git_changelist_dock() {
    dock_window(GIT_CHANGELIST_WIN.PUBLIC, (text = "Git Changelist", closable = true,
            flags = ImGuiWindowFlags.None)) {
        {
            draw_git_changelist_window()
        }
    }
}
