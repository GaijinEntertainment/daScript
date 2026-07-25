options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_live

require imgui/imgui_harness
require imgui/imgui_icons
require imgui/imgui_drawlist_builtin
require imgui/imgui_terminal
require imgui/imgui_fonts
require imgui/imgui_docking_builtin
require imgui/imgui_table_builtin
require imgui/imgui_markdown_tree_sitter
require imgui/imgui_text_source_view
require imgui/imgui_text_tree_sitter
require clipboard/clipboard
require daslib/safe_addr
require daslib/json
require daslib/json_boost
require daslib/jobque_boost
require daslib/strings_boost
require daslib/utf8_utils
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
require ./rich_git_ui.das
require ./rich_terminal_ui.das
require ./rich_sessions_ui.das
require ./rich_launcher_ui.das
require ./rich_files_ui.das

struct HerderInspectorTextArgs {
    side : string = "view"
}

struct HerderInspectorOpenArgs {
    repository_id : string
    worktree_path : string
    file_path : string
    comparison : string = "working"
    revision : string
    diff_context : string = "compact"
}

struct HerderInspectorModeArgs {
    mode : string
}

struct HerderGitWorktreeArgs {
    repository_id : string
    worktree_path : string
}

struct HerderGitPerspectiveArgs {
    perspective : string
}

struct HerderGitFileActionArgs {
    repository_id : string
    worktree_path : string
    file_path : string
    action : string
}

struct HerderGitShaArgs {
    sha : string
}

struct HerderAttentionOpenArgs {
    message_id : string
    target_index : int
}

def open_dock_window(var state : DockWindowState) {
    state.open = true
    state.pending_raise = true
}

def apply_layout_preset(preset : int) {
    REPOSITORIES_WIN.open = true
    WORKTREE_FILES_WIN.open = true
    SESSIONS_WIN.open = true
    GIT_CHANGELIST_WIN.open = true
    GIT_ACTIVITY_WIN.open = true
    FILE_INSPECTOR_WIN.open = true
    TERMINAL_WIN.open = true
    SETTINGS_WIN.open = false
    g_layout_switch_pending = preset
}

def reset_layout() {
    g_layout_factory_reset = true
    apply_layout_preset(g_layout_preset)
}

[live_command(description = "Inspect watcher connectivity and rich-client repository/session counts.")]
def herder_client_state(_input : JsonValue?) : JsonValue? {
    if (g_client == null) return JV((ok = false, error = "client is unavailable"))
    return JV((ok = g_client.connected,
        connected = g_client.connected,
        error = g_client.error,
        connect_count = g_client.connect_count,
        disconnect_count = g_client.disconnect_count,
        reconnect_attempt_count = g_client.reconnect_attempt_count,
        message_count = g_client.message_count,
        repository_message_count = g_client.repository_message_count,
        heartbeat_send_count = g_client.heartbeat_send_count,
        heartbeat_send_failure_count = g_client.heartbeat_send_failure_count,
        heartbeat_last_result = g_client.heartbeat_last_result,
        heartbeat_receive_count = g_client.heartbeat_receive_count,
        list_send_count = g_client.list_send_count,
        list_send_failure_count = g_client.list_send_failure_count,
        list_last_result = g_client.list_last_result,
        heartbeat_age_ms = get_time_nsec(g_client.last_heartbeat) / 1000000l,
        list_age_ms = get_time_nsec(g_client.last_list) / 1000000l,
        attached = g_client.attached,
        controls = g_client.controls,
        desired_columns = g_client.desired_columns,
        desired_rows = g_client.desired_rows,
        sent_columns = g_client.sent_columns,
        sent_rows = g_client.sent_rows,
        heap_bytes = int64(heap_bytes_allocated()),
        heap_total_bytes = int64(heap_total_allocated()),
        string_heap_bytes = int64(string_heap_bytes_allocated()),
        string_heap_total_bytes = int64(string_heap_total_allocated()),
        session_count = length(g_client.sessions),
        repository_count = length(g_client.repositories),
        selected_repository_id = g_selected_repository_id,
        selected_worktree_path = g_selected_worktree_path))
}

[live_command(description = "Restore the rich client's default dock layout.")]
def herder_reset_layout(_input : JsonValue?) : JsonValue? {
    reset_layout()
    return JV((ok = true))
}

struct HerderPerfArgs {
    clear : bool
}

[live_command(description = "Read click-to-content perf samples. Args: clear.")]
def herder_perf_state(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<HerderPerfArgs>)
    var samples <- [for (sample in g_perf_samples);
        JV((surface = sample.surface, identity = sample.identity,
            elapsed_ms = sample.elapsed_ms, detail = sample.detail))]
    var result = JV((ok = true, fps = GetIO().Framerate,
        sample_count = length(g_perf_samples),
        pending_review = g_perf_review.started != 0l,
        pending_history = g_perf_history.started != 0l,
        pending_refs = g_perf_refs.started != 0l,
        pending_commit = g_perf_commit.started != 0l,
        pending_inspect = g_perf_inspect_started != 0l,
        samples = samples))
    if (args.clear) {
        g_perf_samples |> clear()
    }
    return result
}

[live_command(description = "Open the New Session launcher pane seeded from the current selection.")]
def herder_open_launcher(_input : JsonValue?) : JsonValue? {
    prepare_launcher(g_selected_repository_id, g_selected_worktree_path)
    open_dock_window(LAUNCHER_WIN)
    return JV((ok = true, repository_id = g_selected_repository_id,
        worktree_path = g_selected_worktree_path))
}

[live_command(description = "Read durable herd sessions, profiles, and launcher state.")]
def herder_herd_state(_input : JsonValue?) : JsonValue? {
    if (g_client == null) {
        return JV((ok = false, error = "client is not initialized"))
    }
    var profiles <- [for (profile in g_client.herd_profiles);
        JV((id = profile.id, display_name = profile.display_name,
            agent = profile.agent))]
    var records <- [for (record in g_client.herd_sessions);
        JV((id = record.id, name = record.name, profile_id = record.profile_id,
            workflow = record.workflow, state = record.state,
            reason = record.reason, worktree_path = record.worktree_path,
            pty_session_id = record.pty_session_id,
            exit_code = record.exit_code, launch_count = record.launch_count,
            satellite_count = length(record.satellites)))]
    var result = JV((ok = true, revision = g_client.herd_revision,
        launcher_open = LAUNCHER_WIN.open,
        profiles = profiles, sessions = records))
    return result
}

[live_command(description = "Select a worktree for Git review. Args: repository_id, worktree_path.")]
def herder_git_select_worktree(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderGitWorktreeArgs>)
    for (repository in g_client.repositories) {
        if (repository.record.id != args.repository_id) continue
        for (worktree in repository.worktrees) {
            if (worktree.path != args.worktree_path) continue
            g_selected_repository_id = args.repository_id
            g_selected_worktree_path = args.worktree_path
            g_requested_history_commit = ""
            g_client->refresh_repository(args.repository_id)
            g_client->request_repository_review(
                args.repository_id, args.worktree_path)
            g_client->request_repository_history(
                args.repository_id, args.worktree_path, worktree.branch)
            g_client->request_repository_refs(
                args.repository_id, args.worktree_path)
            GIT_CHANGELIST_WIN.open = true
            GIT_CHANGELIST_WIN.pending_raise = true
            GIT_ACTIVITY_WIN.open = true
            return JV((ok = true, repository_id = args.repository_id,
                worktree_path = args.worktree_path))
        }
    }
    return JV((ok = false, error = "unknown repository or worktree"))
}

[live_command(description = "Inspect the selected worktree PR model, including outgoing commits, included files, and not-included files.")]
def herder_git_review_state(_input : JsonValue?) : JsonValue? {
    var commits : array<JsonValue?>
    var included : array<JsonValue?>
    var not_included : array<JsonValue?>
    if (g_client != null) {
        for (repository in g_client.repositories) {
            if (repository.record.id != g_selected_repository_id) continue
            for (worktree in repository.worktrees) {
                if (worktree.path != g_selected_worktree_path) continue
                for (commit in repository.review_commits) {
                    if (commit.worktree_path != worktree.path) continue
                    commits |> push(JV((hash = commit.hash,
                        short_hash = commit.short_hash,
                        authored_at = commit.authored_at,
                        author = commit.author, subject = commit.subject,
                        parents = commit.parents)))
                }
                for (file in repository.review_files) {
                    if (file.worktree_path != worktree.path) continue
                    let status_code = git_status_file_code(
                        repository, worktree.path, file.path)
                    included |> push(JV((code = file.code, path = file.path,
                        old_path = file.old_path,
                        added_lines = file.added_lines,
                        deleted_lines = file.deleted_lines,
                        stats_known = file.stats_known, binary = file.binary,
                        staged = (!empty(status_code) && status_code != "??"
                            && git_status_byte(status_code, 0) != ' '))))
                }
                for (file in repository.files) {
                    if (file.worktree_path != worktree.path
                            || (file.code != "??"
                                && git_status_byte(file.code, 1) == ' ')) continue
                    not_included |> push(JV((code = file.code,
                        path = file.path, old_path = file.old_path,
                        added_lines = file.added_lines,
                        deleted_lines = file.deleted_lines,
                        stats_known = file.stats_known, binary = file.binary)))
                }
                return JV((ok = empty(worktree.review_error),
                    repository_id = repository.record.id,
                    worktree_path = worktree.path,
                    branch = worktree.branch,
                    base_ref = worktree.review_base_ref,
                    merge_base = worktree.review_merge_base,
                    loading = worktree.review_loading,
                    error = worktree.review_error,
                    commits = commits, included = included,
                    not_included = not_included,
                    git_action_loading = g_client.git_action_loading,
                    git_action_file_path = g_client.git_action_file_path,
                    git_action_status = g_client.git_action_status))
            }
        }
    }
    return JV((ok = false, error = "selected worktree is unavailable"))
}

[live_command(description = "Inspect selected worktree History and Tree data, including refs, commits, and selected-commit files.")]
def herder_git_history_state(_input : JsonValue?) : JsonValue? {
    var refs : array<JsonValue?>
    var commits : array<JsonValue?>
    var graph_rows : array<JsonValue?>
    var files : array<JsonValue?>
    if (g_client != null) {
        for (repository in g_client.repositories) {
            if (repository.record.id != g_selected_repository_id) continue
            for (worktree in repository.worktrees) {
                if (worktree.path != g_selected_worktree_path) continue
                for (ref in repository.refs) {
                    if (ref.worktree_path != worktree.path) continue
                    refs |> push(JV((name = ref.name, full_name = ref.full_name,
                        hash = ref.hash, current = ref.current, remote = ref.remote,
                        upstream = ref.upstream)))
                }
                for (commit in repository.history_commits) {
                    if (commit.worktree_path != worktree.path) continue
                    commits |> push(JV((hash = commit.hash,
                        short_hash = commit.short_hash,
                        authored_at = commit.authored_at,
                        author = commit.author, subject = commit.subject,
                        parents = commit.parents)))
                }
                for (row in repository.graph_rows) {
                    if (row.worktree_path != worktree.path) continue
                    graph_rows |> push(JV((topology = row.topology,
                        commit_hash = row.commit_hash)))
                }
                for (file in repository.commit_files) {
                    if (file.worktree_path != worktree.path
                            || file.commit_hash != worktree.history_commit) continue
                    files |> push(JV((commit_hash = file.commit_hash,
                        code = file.code, path = file.path,
                        old_path = file.old_path,
                        added_lines = file.added_lines,
                        deleted_lines = file.deleted_lines,
                        stats_known = file.stats_known, binary = file.binary)))
                }
                return JV((ok = empty(worktree.history_error)
                        && empty(worktree.refs_error),
                    repository_id = repository.record.id,
                    worktree_path = worktree.path,
                    branch = worktree.branch,
                    history_ref = worktree.history_ref,
                    history_commit = worktree.history_commit,
                    requested_history_commit = g_requested_history_commit,
                    history_loading = worktree.history_loading,
                    history_error = worktree.history_error,
                    refs_loading = worktree.refs_loading,
                    refs_error = worktree.refs_error,
                    refs = refs, commits = commits,
                    graph_rows = graph_rows, files = files))
            }
        }
    }
    return JV((ok = false, error = "selected worktree is unavailable"))
}

[live_command(description = "Inspect the semantic Tree filter, focus, nodes, and edges.")]
def herder_git_tree_state(_input : JsonValue?) : JsonValue? {
    var nodes : array<JsonValue?>
    var edges : array<JsonValue?>
    nodes |> reserve(length(g_tree_model.nodes))
    edges |> reserve(length(g_tree_model.edges))
    if (g_client != null) {
        for (repository in g_client.repositories) {
            if (repository.record.id != g_selected_repository_id) continue
            for (worktree in repository.worktrees) {
                if (worktree.path != g_selected_worktree_path) continue
                ensure_tree_model(repository, worktree)
                for (node in g_tree_model.nodes) {
                    nodes |> push(JV((hash = node.hash, row = node.row,
                        lane = node.lane, color_slot = node.color_slot,
                        head = node.head, pushed = node.pushed,
                        focused = node.focused, focus_head = node.focus_head,
                        refs = node.refs_text, primary_ref = node.primary_ref)))
                }
                for (edge in g_tree_model.edges) {
                    edges |> push(JV((child = edge.child, parent = edge.parent,
                        color_slot = edge.color_slot, focused = edge.focused)))
                }
                var filter_name = "auto"
                if (g_tree_model.filter == GitTreeFilter.all) {
                    filter_name = "all"
                }
                return JV((ok = true,
                    filter = filter_name,
                    focus_ref = g_tree_model.focus_ref,
                    base_ref = g_tree_model.base_ref,
                    max_lane = g_tree_model.max_lane,
                    included_ref_count = g_tree_model.included_ref_count,
                    nodes = nodes, edges = edges))
            }
        }
    }
    return JV((ok = false, error = "selected worktree is unavailable"))
}

[live_command(description = "Select a commit and load its changelist. Args: sha.")]
def herder_git_select_commit(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderGitShaArgs>)
    for (repository in g_client.repositories) {
        if (repository.record.id != g_selected_repository_id) continue
        for (worktree in repository.worktrees) {
            if (worktree.path != g_selected_worktree_path) continue
            for (commit in repository.history_commits) {
                if (commit.worktree_path == worktree.path
                        && commit.hash == args.sha) {
                    request_commit_files(repository, worktree, commit.hash)
                    return JV((ok = true, sha = commit.hash))
                }
            }
        }
    }
    return JV((ok = false, error = "commit is not present in the observed history"))
}

[live_command(description = "Copy a Git SHA through the same verified clipboard path used by hover controls. Args: sha.")]
def herder_git_copy_sha(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<HerderGitShaArgs>)
    if (empty(args.sha)) return JV((ok = false, error = "sha is empty"))
    queue_sha_copy(args.sha)
    return JV((ok = g_clipboard_copy_verified,
        sha = args.sha,
        status = int(g_clipboard_copy_status),
        pending = !empty(g_pending_clipboard_sha)))
}

[live_command(description = "Inspect the last Git SHA clipboard write without exposing unrelated clipboard contents.")]
def herder_git_clipboard_state(_input : JsonValue?) : JsonValue? {
    var copied_text = ""
    let read_status = clipboard_get_text(copied_text)
    let expected = !empty(g_pending_clipboard_sha) ? g_pending_clipboard_sha : g_last_copied_sha
    return JV((ok = g_clipboard_copy_verified && read_status == ClipboardStatus.ok
            && copied_text == expected,
        last_sha = g_last_copied_sha,
        pending_sha = g_pending_clipboard_sha,
        write_status = int(g_clipboard_copy_status),
        read_status = int(read_status),
        attempts = g_clipboard_copy_attempts,
        matches = read_status == ClipboardStatus.ok && copied_text == expected))
}

[live_command(description = "Switch Git Status perspective. Args: perspective = pr | history | tree.")]
def herder_git_perspective(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<HerderGitPerspectiveArgs>)
    let perspective = to_lower(args.perspective)
    if (perspective == "pr") {
        GIT_PR_TAB.pending_select = true
    } elif (perspective == "history") {
        GIT_HISTORY_TAB.pending_select = true
    } elif (perspective == "tree") {
        GIT_TREE_TAB.pending_select = true
        if (g_client != null && g_client.connected
                && !empty(g_selected_repository_id)
                && !empty(g_selected_worktree_path)) {
            g_client->request_repository_history(g_selected_repository_id,
                g_selected_worktree_path, "__all__")
        }
    } else {
        return JV((ok = false, error = "perspective must be pr, history, or tree"))
    }
    GIT_CHANGELIST_WIN.open = true
    GIT_ACTIVITY_WIN.open = true
    GIT_ACTIVITY_WIN.pending_raise = true
    return JV((ok = true, perspective = perspective))
}

[live_command(description = "Stage or unstage one observed worktree file. Args: repository_id, worktree_path, file_path, action.")]
def herder_git_file_action(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderGitFileActionArgs>)
    if (args.action != "stage" && args.action != "unstage") {
        return JV((ok = false, error = "action must be stage or unstage"))
    }
    if (g_client.git_action_loading) {
        return JV((ok = false, error = "another Git file action is running"))
    }
    g_client->git_file_action(args.repository_id, args.worktree_path,
        args.file_path, args.action)
    return JV((ok = true, request_id = g_client.git_action_request_id,
        action = args.action, file_path = args.file_path))
}

[live_command(description = "Inspect the selected Git file, comparison, render mode, and changed-line telemetry.")]
def herder_file_inspector_state(_input : JsonValue?) : JsonValue? {
    let view_selection = text_source_view_selection(g_inspector.view)
    return JV((
        ok = g_inspector.prepare.has_retained_content,
        repository_id = g_inspector.repository_id,
        worktree_path = g_inspector.worktree_path,
        file_path = g_inspector.file_path,
        comparison = g_inspector.comparison,
        git_revision = g_inspector.git_revision,
        installed_repository_id = g_inspector.installed_repository_id,
        installed_worktree_path = g_inspector.installed_worktree_path,
        installed_file_path = g_inspector.installed_file_path,
        installed_comparison = g_inspector.installed_comparison,
        installed_git_revision = g_inspector.installed_git_revision,
        installed_diff_context = g_inspector.installed_diff_context,
        replacement_busy = (g_inspector.loading
            || g_inspector.prepare.request != InspectorRequestState.idle),
        zoom_percent = g_zoom_percent,
        diff_context = g_inspector.diff_context,
        diff_context_choice = g_inspector.diff_context_choice,
        mode = g_inspector.mode == GitInspectorMode.diff ? "diff" : "view",
        loading = g_inspector.loading,
        prepare_request = inspector_request_state_name(g_inspector.prepare.request),
        prepare_outcome = inspector_content_state_name(
            g_inspector.prepare.latest_outcome),
        prepare_generation = g_inspector.prepare.generation,
        prepare_has_retained_content = g_inspector.prepare.has_retained_content,
        prepare_retained_generation = g_inspector.prepare.retained_generation,
        prepare_status = g_inspector.prepare.status,
        prepare_error = g_inspector.prepare.error,
        prepare_elapsed_usec = g_inspector.prepare_elapsed_usec,
        status = g_inspector.status,
        error = g_inspector.error,
        task_session_id = g_inspector.task_session_id,
        revision = g_inspector.revision,
        diff_row_count = length(g_inspector.parsed.rows),
        diff_hunk_count = length(g_inspector.parsed.hunks),
        diff_current_hunk = g_inspector.diff_current_hunk,
        diff_scroll_y = g_inspector.diff_scroll_y,
        diff_split_fraction = g_inspector.diff_split_fraction,
        diff_left_rect = g_inspector.diff_left_rect,
        diff_right_rect = g_inspector.diff_right_rect,
        diff_scrollbar_rect = g_inspector.diff_scrollbar_rect,
        syntax_choice = g_inspector.syntax_choice,
        installed_syntax_choice = g_inspector.installed_syntax_choice,
        resolved_language = g_inspector.resolved_language,
        binary = g_inspector.parsed.binary,
        preview_texture_ready = g_inspector.preview_texture != 0u,
        preview_width = g_inspector.preview_width,
        preview_height = g_inspector.preview_height,
        preview_byte_count = g_inspector.preview_byte_count,
        preview_error = g_inspector.preview_error,
        old_changed_line_count = length(g_inspector.parsed.old_changed_lines),
        new_changed_line_count = length(g_inspector.parsed.new_changed_lines),
        old_byte_count = length(g_inspector.parsed.old_text),
        new_byte_count = length(g_inspector.parsed.new_text),
        new_document_byte_count = length(g_inspector.new_document.source),
        new_document_utf8_valid = is_utf8_string_valid(
            g_inspector.new_document.source),
        view_byte_count = length(g_inspector.view_source),
        view_prepared = g_inspector.view_prepared,
        old_line_mark_count = length(g_inspector.old_document.line_marks),
        new_line_mark_count = length(g_inspector.new_document.line_marks),
        old_line_number_count = length(g_inspector.old_document.line_numbers),
        new_line_number_count = length(g_inspector.new_document.line_numbers),
        view_line_mark_count = length(g_inspector.view_document.line_marks),
        focus_document_valid = g_inspector.focus_document_valid,
        focus_new_marker_count = 0,
        focus_view_marker_count = 0,
        focus_start_byte = g_inspector.focus_start_byte,
        changed_source_range_count = length(g_inspector.changed_source_ranges),
        old_syntax_span_count = length(g_inspector.old_document.spans),
        new_syntax_span_count = length(g_inspector.new_document.spans),
        view_syntax_span_count = length(g_inspector.view_document.spans),
        old_syntax_cell_count = syntax_cell_count(g_inspector.old_view),
        new_syntax_cell_count = syntax_cell_count(g_inspector.new_view),
        view_syntax_cell_count = syntax_cell_count(g_inspector.view),
        view_virtual_mode = g_inspector.view.virtual_mode,
        view_visible_syntax_runs = g_inspector.view.virtual_visible_syntax_runs,
        view_selection_start = view_selection.start_byte,
        view_selection_end = view_selection.end_byte,
        view_selection_revision = g_inspector.view.selection_revision,
        view_selection_copy_revision = g_inspector.view.selection_copy_revision,
        old_keyword_cell_count = syntax_style_cell_count(
            g_inspector.old_view, TextSourceSyntaxStyle.keyword),
        new_keyword_cell_count = syntax_style_cell_count(
            g_inspector.new_view, TextSourceSyntaxStyle.keyword),
        old_type_cell_count = syntax_style_cell_count(
            g_inspector.old_view, TextSourceSyntaxStyle.type_),
        new_type_cell_count = syntax_style_cell_count(
            g_inspector.new_view, TextSourceSyntaxStyle.type_),
        old_function_cell_count = syntax_style_cell_count(
            g_inspector.old_view, TextSourceSyntaxStyle.function_),
        new_function_cell_count = syntax_style_cell_count(
            g_inspector.new_view, TextSourceSyntaxStyle.function_),
        old_render_revision = g_inspector.old_view.render_revision,
        new_render_revision = g_inspector.new_view.render_revision,
        view_render_revision = g_inspector.view.render_revision,
        markdown_render_revision = g_inspector.markdown_view.render_revision,
        markdown_current_layout_count = markdown_view_current_layout_count(
            g_inspector.markdown_view),
        markdown_changed_layout_count = markdown_view_changed_layout_count(
            g_inspector.markdown_document, g_inspector.markdown_view,
            g_inspector.changed_source_ranges),
        markdown_gutter_segments = g_inspector.markdown_gutter_segments))
}

[live_command(description = "Open a changed Git file immediately. Args: repository_id, worktree_path, file_path, comparison, optional revision for commit.")]
def herder_file_inspector_open(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderInspectorOpenArgs>)
    if (empty(args.repository_id) || empty(args.worktree_path)
            || empty(args.file_path)) {
        return JV((ok = false,
            error = "repository_id, worktree_path, and file_path are required"))
    }
    let comparison = empty(args.comparison) ? "working" : args.comparison
    g_client->inspect_file(args.repository_id, args.worktree_path,
        args.file_path, comparison, args.diff_context, args.revision)
    FILE_INSPECTOR_WIN.open = true
    FILE_INSPECTOR_WIN.pending_raise = true
    return JV((ok = true, request_id = g_inspector.request_id,
        file_path = args.file_path, comparison = comparison,
        revision = args.revision))
}

[live_command(description = "Read exact inspector text. Args: side = before | after | view.")]
def herder_file_inspector_text(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<HerderInspectorTextArgs>)
    let side = to_lower(args.side)
    if (side == "before" || side == "old") {
        return JV((ok = true, side = "before",
            text = g_inspector.parsed.old_text,
            line_mark_count = length(g_inspector.old_document.line_marks),
            syntax_span_count = length(g_inspector.old_document.spans)))
    }
    if (side == "after" || side == "new" || side == "view") {
        let is_view = side == "view"
        let source = is_view ? g_inspector.view_source : g_inspector.parsed.new_text
        return JV((ok = true, side = side == "view" ? "view" : "after",
            text = source,
            line_mark_count = is_view
                ? length(g_inspector.view_document.line_marks)
                : length(g_inspector.new_document.line_marks),
            syntax_span_count = is_view
                ? length(g_inspector.view_document.spans)
                : length(g_inspector.new_document.spans)))
    }
    return JV((ok = false, error = "side must be before, after, or view"))
}

[live_command(description = "Switch the Git file inspector between Diff and View without mouse input.")]
def herder_file_inspector_mode(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<HerderInspectorModeArgs>)
    let mode = to_lower(args.mode)
    if (mode == "diff") {
        INSPECTOR_DIFF_TAB.pending_select = true
    } elif (mode == "view") {
        INSPECTOR_VIEW_TAB.pending_select = true
    } else {
        return JV((ok = false, error = "mode must be diff or view"))
    }
    return JV((ok = true, mode = mode))
}

[live_command(description = "Inspect persistent, non-modal agent Attention and active Focus Set state.")]
def herder_attention_state(_input : JsonValue?) : JsonValue? {
    var messages : array<JsonValue?>
    if (g_client != null) {
        messages |> reserve(length(g_client.mailbox))
        for (message in g_client.mailbox) {
            if (message.direction != "outbox") continue
            var targets <- [for (target in message.focus.targets);
                JV((repository_id = watcher_focus_repository(message.focus, target),
                    worktree_path = watcher_focus_worktree(message.focus, target),
                    comparison = watcher_focus_comparison(message.focus, target),
                    file_path = target.file_path,
                    whole_file = target.whole_file,
                    range_count = length(target.ranges)))]
            messages |> push(JV((id = message.id, sender = message.sender,
                subject = message.subject, status = message.status,
                targets = targets)))
        }
    }
    return JV((ok = g_client != null,
        session_id = g_client != null ? g_client.selected_id : "",
        mailbox_revision = g_client != null ? g_client.mailbox_revision : 0l,
        active_message_id = g_active_focus_message,
        active_target_index = g_active_focus_target,
        active_range_index = g_active_focus_range,
        focus_visible = g_focus_visible,
        last_human_request_id = g_last_human_request,
        messages = messages))
}

[live_command(description = "Inspect selected dasHerd session origin, purpose, and multi-repository Review Bundles.")]
def herder_session_state(_input : JsonValue?) : JsonValue? {
    var session_id, session_state, session_kind, session_purpose : string
    var task_context, repository_id, worktree_path, context_path : string
    var bundles : array<JsonValue?>
    if (g_client != null) {
        for (session in g_client.sessions) {
            if (session.id == g_client.selected_id) {
                session_id = session.id
                session_state = session.state
                session_kind = session.kind
                session_purpose = session.purpose
                task_context = session.task_context
                repository_id = session.repository_id
                worktree_path = session.worktree_path
                context_path = session.context_path
                break
            }
        }
        bundles |> reserve(length(g_client.bundles))
        for (bundle in g_client.bundles) {
            var participants <- [for (participant in bundle.participants);
                JV((repository_id = participant.repository_id,
                    worktree_path = participant.worktree_path,
                    role = participant.role, state = participant.state,
                    file_count = length(participant.files)))]
            bundles |> push(JV((id = bundle.id, title = bundle.title,
                status = bundle.status, revision = bundle.revision,
                participant_count = length(bundle.participants),
                focus_target_count = length(bundle.focus.targets),
                participants = participants)))
        }
    }
    return JV((ok = g_client != null, session_id = session_id,
        session_state = session_state, session_kind = session_kind,
        session_purpose = session_purpose, task_context = task_context,
        repository_id = repository_id, worktree_path = worktree_path,
        context_path = context_path,
        bundles_revision = g_client != null ? g_client.bundles_revision : 0l,
        bundles = bundles))
}

[live_command(description = "Open one agent Focus Set target at the user's chosen time. Args: message_id, target_index.")]
def herder_attention_open(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderAttentionOpenArgs>)
    for (message in g_client.mailbox) {
        if (message.id != args.message_id) continue
        if (args.target_index < 0
                || args.target_index >= length(message.focus.targets)) {
            return JV((ok = false, error = "target_index is out of range"))
        }
        let target & = unsafe(message.focus.targets[args.target_index])
        activate_focus_target(message, args.target_index)
        return JV((ok = true, message_id = message.id,
            target_index = args.target_index, file_path = target.file_path))
    }
    return JV((ok = false, error = "unknown attention message"))
}

def draw_settings_window() {
    text("Global user configuration")
    text_disabled("Repository registration and layout settings will be stored here.")
}

def draw_settings_dock() {
    dock_window(SETTINGS_WIN.PUBLIC, (text = "Settings", closable = true,
            flags = ImGuiWindowFlags.None)) {
        draw_settings_window()
    }
}

[live_command(description = "Dump recent UI selection-trigger events. Args: clear.")]
def herder_ui_events(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<HerderPerfArgs>)
    var events := g_ui_events
    var result = JV((ok = true, frame = g_ui_frame, events = events))
    if (args.clear) {
        g_ui_events |> clear()
    }
    return result
}

struct HerderLayoutArgs {
    preset : string
}

[live_command(description = "Switch the dock layout preset. Args: preset (session|review).")]
def herder_layout(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<HerderLayoutArgs>)
    let preset = args.preset == "review" ? 1 : 0
    apply_layout_preset(preset)
    return JV((ok = true, preset = preset))
}
