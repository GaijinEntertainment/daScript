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
require ./rich_inspector_ui.das
require ./rich_files_ui.das

var WORKTREE_ROW : table<int; ToggleState>

var WORKTREE_CONFLICT_JOIN : table<int; EmptyMarkerState>

var REPOSITORY_FOLDER_JOIN : table<int; EmptyMarkerState>

var GIT_FILE_ROW : table<int; ToggleState>

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

var CHANGELIST_ERROR_POPUP : PopupContextItemState

var CHANGELIST_ERROR_COPY : ToggleState

var CHANGELIST_ERROR_SEND : ToggleState

var ACTIVITY_ERROR_POPUP : PopupContextItemState

var ACTIVITY_ERROR_COPY : ToggleState

var ACTIVITY_ERROR_SEND : ToggleState

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
                text_colored(REPOSITORY_ERROR[repository_row], (color = float4(1.0f, 0.35f, 0.30f, 1.0f),
                    text = repository.error))
                draw_error_context(REPOSITORY_ERROR_POPUP[repository_row],
                    REPOSITORY_ERROR_COPY[repository_row],
                    REPOSITORY_ERROR_SEND[repository_row],
                    "repository_error_{repository_row}", "Repository",
                    repository.error)
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
                    same_line(WORKTREE_CONFLICT_JOIN[row])
                }
                let row_label = "{prefix}{branch}{dirty_label}##{repository.record.id}:{worktree.path}"
                if (selectable(WORKTREE_ROW[row], (text = row_label))) {
                    ui_event("worktree-row-click {worktree.path}")
                    g_selected_repository_id = repository.record.id
                    g_selected_worktree_path = worktree.path
                    g_client->refresh_repository(repository.record.id)
                    g_client->request_repository_review(
                        repository.record.id, worktree.path)
                    GIT_CHANGELIST_WIN.open = true
                    GIT_CHANGELIST_WIN.pending_raise = true
                    GIT_ACTIVITY_WIN.open = true
                }
                WORKTREE_ROW[row].value = (repository.record.id == g_selected_repository_id && worktree.path == g_selected_worktree_path)
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

def draw_git_file_group(repository : RepositorySnapshot const;
                        worktree : WorktreeState const;
                        group : int; var file_row : int&) : int {
    var count = 0
    for (file in repository.files) {
        if (file.worktree_path == worktree.path && git_file_in_group(file, group)) count++
    }
    if (count == 0) return 0
    let title = group == 0 ? "CONFLICTS" : (group == 1 ? "STAGED"
        : (group == 2 ? "MODIFIED" : "UNTRACKED"))
    text(GIT_GROUP_TITLE[group], (text = "{title}  {count}"))
    separator(GIT_GROUP_SEPARATOR[group])
    let comparison = git_group_scope(group)
    for (file in repository.files) {
        if (file.worktree_path != worktree.path || !git_file_in_group(file, group)) continue
        GIT_FILE_ROW[file_row].value = (g_inspector.repository_id == repository.record.id
            && g_inspector.worktree_path == worktree.path
            && g_inspector.file_path == file.path
            && g_inspector.comparison == comparison)
        let old_label = !empty(file.old_path) ? "  (from {file.old_path})" : ""
        let display_path = focused_file_label(
            repository.record.id, worktree.path, file.path)
        if (selectable(GIT_FILE_ROW[file_row],
                (text = "{file.code}  {display_path}{old_label}##{comparison}:{file.path}"))) {
            ui_event("status-file-click {comparison}:{file.path}")
            if (g_client != null) {
                g_client->inspect_file(repository.record.id, worktree.path,
                    file.path, comparison)
                FILE_INSPECTOR_WIN.open = true
                FILE_INSPECTOR_WIN.pending_raise = true
            }
        }
        file_row++
    }
    dummy(GIT_GROUP_SPACING[group], (size = float2(0.0f, GetStyle().ItemSpacing.y)))
    return count
}

def draw_git_path_text(path : string; p0, p1 : float2;
                       subdued : bool) {
    let alpha = subdued ? 0.55f : 1.0f
    let directory_color = GetColorU32(float4(0.43f, 0.64f, 0.82f, alpha))
    let separator_color = GetColorU32(float4(0.78f, 0.55f, 0.30f, alpha))
    let name_color = GetColorU32(float4(0.90f, 0.91f, 0.93f, alpha))
    var inscope components <- split(replace(path, "\\", "/"), "/")
    var position = p0
    with_window_drawlist() $(var dl) {
        dl |> with_drawlist_clip_rect(p0, p1, true) {
            for (index in range(length(components))) {
                let component = components[index]
                let color = index + 1 == length(components) ? name_color : directory_color
                dl |> add_text(position, color, component)
                position.x += CalcTextSize(component).x
                if (index + 1 < length(components)) {
                    dl |> add_text(position, separator_color, "/")
                    position.x += CalcTextSize("/").x
                }
            }
        }
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
                GetColorU32(float4(0.42f, 0.82f, 0.42f, alpha)), added)
            dl |> add_text(float2(position.x + CalcTextSize(added).x
                    + GetStyle().ItemSpacing.x, position.y),
                GetColorU32(float4(0.37f, 0.65f, 0.96f, alpha)), deleted)
        }
    }
}

def git_visible_row_right(row_right : float) : float {
    let style & = GetStyle()
    let window_right = GetWindowPos().x + GetWindowSize().x - style.WindowPadding.x - style.ScrollbarSize
    return min(row_right, window_right)
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
            with_style((ImGuiCol.Text, float4(0.36f, 0.76f, 0.96f, 1.0f))) {
                selectable_hover(GIT_PR_COMMIT_HOVER[commit_row],
                    (text = "{commit.short_hash}##pr:{commit.hash}",
                    flags = ImGuiSelectableFlags.SpanAllColumns |
                        ImGuiSelectableFlags.AllowOverlap))
            }
            draw_sha_copy_icon(commit.hash, "pr-commit:{commit.hash}",
                commit.short_hash)
            table_set_column_index(1)
            text_colored(GIT_COMMIT_TITLE_TEXT[commit_row],
                (color = float4(0.91f, 0.93f, 0.96f, 1.0f),
                text = commit.subject))
            table_set_column_index(2)
            text_colored(GIT_COMMIT_AUTHOR_TEXT[commit_row],
                (color = float4(0.64f, 0.70f, 0.80f, 1.0f),
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
    let count = length(commit_indices)
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
        list_clipper(count) $(start, end_) {
            for (row in range(start, end_)) {
                let commit = repository.history_commits[commit_indices[row]]
                let selected_commit = !empty(g_requested_history_commit) ? g_requested_history_commit : worktree.history_commit
                table_next_row()
                table_set_column_index(0)
                var selected = false
                with_style((ImGuiCol.Text, float4(0.36f, 0.76f, 0.96f, 1.0f))) {
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
                    (color = float4(0.91f, 0.93f, 0.96f, 1.0f),
                    text = commit.subject))
                table_set_column_index(2)
                text_colored(GIT_COMMIT_AUTHOR_TEXT[row],
                    (color = float4(0.64f, 0.70f, 0.80f, 1.0f),
                    text = commit.author))
                if (selected && g_client != null && commit.hash != selected_commit) {
                    ui_event("history-commit-click {commit.short_hash}")
                    request_commit_files(repository, worktree, commit.hash)
                }
            }
        }
    }
}

def draw_history_review(repository : RepositorySnapshot const;
                        worktree : WorktreeState const) {
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
                        float4(0.36f, 0.76f, 0.96f, text_alpha))) {
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
                    (color = float4(0.91f, 0.93f, 0.96f, text_alpha),
                    text = commit.subject))
                table_set_column_index(2)
                text_colored(GIT_COMMIT_AUTHOR_TEXT[row],
                    (color = float4(0.64f, 0.70f, 0.80f, text_alpha),
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
    return GitWindowStatus(text = "OUTGOING COMMITS  {git_review_commit_count(repository, worktree)}   -> {base}")
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
                    let title = worktree.is_main ? "MAIN - {worktree.branch}" : worktree.branch
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
                    separator()
                    if (GIT_HISTORY_TAB.selected || GIT_TREE_TAB.selected) {
                        draw_commit_files(repository, worktree)
                    } else {
                        draw_shared_pr_files(repository, worktree)
                    }
                    status = git_changelist_status(repository, worktree)
                }
            }
        }
        if (!found) text_disabled("Select a worktree to inspect its changelist")
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
            status.text)
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
            status.text)
    }
}

def git_ui_init() {
    GIT_TREE_FILTER.value = 0
}

def draw_repositories_dock() {
    dock_window(REPOSITORIES_WIN.PUBLIC, (text = "Repositories & Worktrees", closable = true,
            flags = ImGuiWindowFlags.None)) {
        draw_repositories_window()
    }
}

def draw_git_activity_dock() {
    dock_window(GIT_ACTIVITY_WIN.PUBLIC, (text = "Git Activity", closable = true,
            flags = ImGuiWindowFlags.None)) {
        draw_git_activity_window()
    }
}

def draw_git_changelist_dock() {
    dock_window(GIT_CHANGELIST_WIN.PUBLIC, (text = "Git Changelist", closable = true,
            flags = ImGuiWindowFlags.None)) {
        draw_git_changelist_window()
    }
}
