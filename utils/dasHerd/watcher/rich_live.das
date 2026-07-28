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
require ./rich_commands.das
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
        selected_worktree_path = g_selected_worktree_path,
        selection_explicit = g_selection_explicit))
}

[live_command(description = "Restore the rich client's default dock layout.")]
def herder_reset_layout(_input : JsonValue?) : JsonValue? {
    reset_layout()
    return JV((ok = true))
}

struct HerderContentZoomArgs {
    kind : string
    percent : int
}

[live_command(description = "Read or set a content text scale, the target of Ctrl+wheel over that content. Args: kind = terminal | view | diff; optional percent (50-300) to set it. Chrome is not here: one ui_zoom setting drives every title, status bar and label.")]
def herder_content_zoom(input : JsonValue?) : JsonValue? {
    //! Parity rail (note 56): the content scales were mouse-only - adjustable
    //! but neither commandable nor readable, so nothing could verify them.
    let args = from_JV(input, type<HerderContentZoomArgs>)
    if (args.kind != CONTENT_TERMINAL && args.kind != CONTENT_VIEW
            && args.kind != CONTENT_DIFF) {
        return JV((ok = false,
            error = "kind must be terminal, view, or diff"))
    }
    if (args.percent != 0) {
        set_content_zoom(args.kind, args.percent)
    }
    return JV((ok = true, kind = args.kind,
        percent = content_zoom_percent(args.kind),
        ui_zoom_percent = g_zoom_percent,
        terminal = content_zoom_percent(CONTENT_TERMINAL),
        view = content_zoom_percent(CONTENT_VIEW),
        diff = content_zoom_percent(CONTENT_DIFF)))
}

struct HerderCommandArgs {
    id : string
}

struct HerderSetShortcutArgs {
    id : string
    chord : string
}

[live_command(description = "List registered named commands with categories and current/conflicting shortcuts.")]
def herder_commands_state(_input : JsonValue?) : JsonValue? {
    var rows <- [for (definition in g_command_registry.definitions);
        JV((id = definition.id, category = definition.category,
            title = definition.title,
            chord = command_shortcut_text(g_command_registry, definition.id),
            conflict = herd_command_conflict(definition.id)))]
    return JV((ok = true, count = length(g_command_registry.definitions),
        config_path = g_command_registry.config_path,
        status = g_command_registry.status, commands = rows))
}

[live_command(description = "Queue a named command by id — the same dispatch menus and shortcuts use; it executes this frame. Args: id.")]
def herder_command(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<HerderCommandArgs>)
    if (run_herd_command(args.id, CommandSource.live_api)) {
        return JV((ok = true, id = args.id))
    }
    return JV((ok = false, error = "unknown or disabled command {args.id}"))
}

[live_command(description = "Assign or clear a command shortcut (empty chord unbinds); persists immediately. Chord spelling matches herder_commands_state, e.g. Ctrl+Shift+N. Args: id, chord.")]
def herder_set_shortcut(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<HerderSetShortcutArgs>)
    let result = herd_command_set_shortcut(args.id, args.chord)
    if (result.ok) {
        return JV((ok = true, id = args.id,
            chord = herd_command_shortcut(args.id),
            conflict = herd_command_conflict(args.id)))
    }
    return JV((ok = false, error = result.message))
}

[live_command(description = "Inspect input ownership: the @live user-control lock, synth cursor/key state, io cursor, and real wheel/click counters.")]
def herder_input_state(_input : JsonValue?) : JsonValue? {
    let sc = get_synth_cursor()
    let sk = get_synth_keys()
    let mouse = GetMousePos()
    return JV((ok = true,
        user_control = user_control_enabled(),
        synth_cursor_owned = sc._0,
        synth_cursor_x = sc._1,
        synth_cursor_y = sc._2,
        synth_keys_held = sk._0,
        io_mouse_x = mouse.x,
        io_mouse_y = mouse.y,
        io_mouse_valid = IsMousePosValid(null),
        wheel_events_seen = g_wheel_events_seen,
        wheel_last_delta = g_wheel_last_delta,
        click_events_seen = g_click_events_seen,
        attached = g_client != null && g_client.attached,
        controls_lease = g_client != null && g_client.controls))
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
    open_launcher_modal()
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
        launcher_open = launcher_modal_open(),
        profiles = profiles, sessions = records))
    return result
}

struct HerderAttachSessionArgs {
    session_id : string
    herd_id : string
}

[live_command(description = "Attach the terminal to a session. Args: session_id (PTY id) or herd_id (durable session).")]
def herder_attach_session(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderAttachSessionArgs>)
    var target = args.session_id
    if (empty(target) && !empty(args.herd_id)) {
        for (record in g_client.herd_sessions) {
            if (record.id == args.herd_id) {
                target = record.pty_session_id
                break
            }
        }
    }
    if (empty(target)) {
        return JV((ok = false, error = "pass session_id or herd_id of a session with a PTY"))
    }
    ui_event("live-attach-session {target}")
    g_client->attach_session(target)
    g_open_terminal_pending = true
    return JV((ok = true, session_id = target))
}

[live_command(description = "Close a durable herd session (terminates its PTY if running). Args: herd_id.")]
def herder_close_session(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderAttachSessionArgs>)
    if (empty(args.herd_id)) return JV((ok = false, error = "herd_id is required"))
    // Validate before queuing: the watcher's error reply lands async, so an
    // unknown id (a stray \r from a caller's shell pipeline, say) used to
    // report ok=true while closing nothing.
    var known = false
    for (record in g_client.herd_sessions) {
        if (record.id == args.herd_id) {
            known = true
            break
        }
    }
    if (!known) return JV((ok = false, error = "unknown herd session '{args.herd_id}'"))
    g_client->request_herd_close(args.herd_id)
    return JV((ok = true, herd_id = args.herd_id))
}

struct HerderRetireWorktreeArgs {
    repository_id : string
    worktree_path : string
    force : bool
}

struct HerderRetireSessionArgs {
    herd_id : string
    worktree_paths : array<string>
    force : bool
}

def private worktree_verdict_jv(repository_id, worktree_path : string) : JsonValue? {
    var worktree = WorktreeState()
    if (!find_worktree_state(repository_id, worktree_path, worktree)) {
        return JV((ok = false, error = "unknown worktree '{worktree_path}'"))
    }
    let claimed_by = herd_session_claiming(worktree_path)
    // Every number names the ref it was measured against — an unqualified
    // count is the exact ambiguity note 58 was filed about.
    return JV((ok = true, worktree_path = worktree_path,
        branch = worktree.branch, is_main = worktree.is_main,
        tier = repository_worktree_delete_tier(worktree, claimed_by),
        claimed_by = claimed_by,
        staged = worktree.staged, unstaged = worktree.unstaged,
        untracked = worktree.untracked, conflicted = worktree.conflicted,
        unmerged_known = worktree.unmerged_known,
        unmerged_commits = worktree.unmerged_commits,
        unmerged_base_ref = worktree.unmerged_base_ref,
        upstream_ref = worktree.upstream_ref, ahead = worktree.ahead,
        only_here = repository_worktree_only_here(worktree)))
}

[live_command(description = "Read the retire-a-worktree verdict and every fact behind it: tier (in_use | main | dirty | warn | green), claiming session, working-tree counts, and unmerged commits WITH the base ref they were measured against. Args: repository_id, worktree_path.")]
def herder_worktree_retire_state(input : JsonValue?) : JsonValue? {
    if (g_client == null) return JV((ok = false, error = "no client"))
    let args = from_JV(input, type<HerderRetireWorktreeArgs>)
    if (empty(args.worktree_path)) {
        return JV((ok = false, error = "worktree_path is required"))
    }
    return worktree_verdict_jv(args.repository_id, args.worktree_path)
}

[live_command(description = "Open the retire-a-worktree checklist dialog on a worktree, exactly as the row's context menu does. Args: repository_id, worktree_path.")]
def herder_worktree_retire_open(input : JsonValue?) : JsonValue? {
    if (g_client == null) return JV((ok = false, error = "no client"))
    let args = from_JV(input, type<HerderRetireWorktreeArgs>)
    var worktree = WorktreeState()
    if (!find_worktree_state(args.repository_id, args.worktree_path, worktree)) {
        return JV((ok = false, error = "unknown worktree '{args.worktree_path}'"))
    }
    g_worktree_retire = WorktreeRetirePrompt(active = true,
        repository_id = clone_string(args.repository_id),
        worktree_path = clone_string(args.worktree_path))
    return JV((ok = true, worktree_path = args.worktree_path))
}

[live_command(description = "Retire (delete) a worktree. Blocked while a session holds it or the working tree is dirty; force = true is the explicit 'delete anyway, N files lost' escape. The branch and its commits are never touched. Args: repository_id, worktree_path, force.")]
def herder_worktree_retire(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderRetireWorktreeArgs>)
    var worktree = WorktreeState()
    if (!find_worktree_state(args.repository_id, args.worktree_path, worktree)) {
        return JV((ok = false, error = "unknown worktree '{args.worktree_path}'"))
    }
    let claimed_by = herd_session_claiming(args.worktree_path)
    let tier = repository_worktree_delete_tier(worktree, claimed_by)
    if (tier == WORKTREE_TIER_IN_USE || tier == WORKTREE_TIER_MAIN) {
        return JV((ok = false, tier = tier,
            error = "{args.worktree_path} cannot be retired: {tier}"))
    }
    if (tier == WORKTREE_TIER_DIRTY && !args.force) {
        return JV((ok = false, tier = tier,
            error = "working tree is dirty; pass force to delete anyway"))
    }
    g_client->request_worktree_delete(args.repository_id, args.worktree_path,
        args.force)
    return JV((ok = true, tier = tier, worktree_path = args.worktree_path))
}

[live_command(description = "Read the running worktree operation the retire dialog shows: kind, state (idle|running|ok|failed|cancelled), current step, and the full command log.")]
def herder_worktree_operation_state(_input : JsonValue?) : JsonValue? {
    if (g_client == null) return JV((ok = false, error = "no client"))
    var log <- [for (line in g_client.operation.log); JV(line)]
    return JV((ok = true, kind = g_client.operation.kind,
        state = g_client.operation.state,
        worktree_path = g_client.operation.worktree_path,
        step = g_client.operation.step,
        step_index = g_client.operation.step_index,
        step_count = g_client.operation.step_count,
        error = g_client.operation.error,
        hint = g_client.operation.hint,
        retryable = g_client.operation.retryable,
        session_id = g_client.operation.session_id,
        revision = g_client.operation.revision,
        queued = length(g_worktree_retire_queue),
        log <- log))
}

[live_command(description = "Cancel the running worktree operation. Stops the run; whatever already completed stays done and stays in the log.")]
def herder_worktree_operation_cancel(_input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    if (g_client.operation.state != "running") {
        return JV((ok = false, error = "no operation is running"))
    }
    g_client->request_worktree_operation_cancel()
    return JV((ok = true))
}

[live_command(description = "Rescue a dirty worktree mechanically: stage everything and commit it to the branch, which outlives the worktree. Args: repository_id, worktree_path.")]
def herder_worktree_commit_wip(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderRetireWorktreeArgs>)
    var worktree = WorktreeState()
    if (!find_worktree_state(args.repository_id, args.worktree_path, worktree)) {
        return JV((ok = false, error = "unknown worktree '{args.worktree_path}'"))
    }
    if (empty(worktree.branch)) {
        return JV((ok = false,
            error = "detached HEAD: a commit here would not be on any branch"))
    }
    g_client->request_worktree_commit_wip(args.repository_id,
        args.worktree_path, "WIP: work in progress on {worktree.branch}")
    return JV((ok = true, branch = worktree.branch))
}

struct HerderChangelistArgs {
    repository_id : string
    worktree_path : string
    mode : string
    group : int = -1
    path : string
    selected : bool
    action : string
    paths : array<string>
    message : string
}

def private changelist_target(var args : HerderChangelistArgs) {
    // Empty scope means "the worktree the Changelist is showing".
    if (empty(args.repository_id)) {
        args.repository_id = g_selected_repository_id
    }
    if (empty(args.worktree_path)) {
        args.worktree_path = g_selected_worktree_path
    }
}

def private changelist_group_keys(repository : RepositorySnapshot const;
                                  worktree_path : string; group : int) : array<string> {
    return <- [for (file in repository.files);
        selection_key(git_group_scope(group), file.path);
        where file.worktree_path == worktree_path
            && git_file_in_group(file, group)]
}

[live_command(description = "Read the Changelist Changes view: every status row (path, code, group 0-3 = conflicts/staged/modified/untracked, selected), the selection set, what stage/unstage would act on, and whether the rail is busy. Args: repository_id, worktree_path (empty = the shown worktree).")]
def herder_changelist_state(input : JsonValue?) : JsonValue? {
    if (g_client == null) return JV((ok = false, error = "no client"))
    var args = from_JV(input, type<HerderChangelistArgs>)
    changelist_target(args)
    for (repository in g_client.repositories) {
        if (repository.record.id != args.repository_id) continue
        for (worktree in repository.worktrees) {
            if (worktree.path != args.worktree_path) continue
            var rows : array<JsonValue?>
            for (group in range(4)) {
                let comparison = git_group_scope(group)
                for (file in repository.files) {
                    if (file.worktree_path != worktree.path
                            || !git_file_in_group(file, group)) continue
                    rows |> push(JV((path = file.path, code = file.code,
                        group = group,
                        selected = selection_contains(g_changelist_selection,
                            selection_key(comparison, file.path)))))
                }
            }
            var stage_paths : array<string>
            var unstage_paths : array<string>
            changelist_selection_actions(repository, worktree, stage_paths,
                unstage_paths)
            var modified_paths : array<string>
            var staged_paths : array<string>
            var untracked_paths : array<string>
            var conflicted = 0
            changelist_selection_discards(repository, worktree,
                modified_paths, staged_paths, untracked_paths, conflicted)
            var stage_list <- [for (p in stage_paths); JV(p)]
            var unstage_list <- [for (p in unstage_paths); JV(p)]
            var discard_modified <- [for (p in modified_paths); JV(p)]
            var discard_staged <- [for (p in staged_paths); JV(p)]
            var discard_untracked <- [for (p in untracked_paths); JV(p)]
            return JV((ok = true, repository_id = args.repository_id,
                worktree_path = args.worktree_path,
                selection_count = selection_count(g_changelist_selection),
                busy = changelist_rail_busy(),
                notice = g_client.changelist_notice,
                stage_eligible <- stage_list,
                unstage_eligible <- unstage_list,
                discard_modified <- discard_modified,
                discard_staged <- discard_staged,
                discard_untracked <- discard_untracked,
                discard_conflicted_held = conflicted,
                discard_prompt_open = (GIT_DISCARD_DIALOG.open
                    || g_git_discard.pending_open),
                discard_prompt_files = g_git_discard.file_count,
                discard_prompt_bytes = g_git_discard.total_bytes,
                discard_prompt_armed = g_git_discard.confirming,
                staged_count = worktree.staged,
                commit_message = changelist_commit_message(),
                commit_in_flight = g_commit_clear_watch != 0l,
                rows <- rows))
        }
    }
    return JV((ok = false, error = "unknown worktree '{args.worktree_path}'"))
}

[live_command(description = "Drive the Changelist selection exactly as the mouse would. Args: repository_id, worktree_path (empty = shown), mode = plain|toggle|range|group|clear, group (0-3), path (file path for plain/toggle/range), selected (for group).")]
def herder_changelist_select(input : JsonValue?) : JsonValue? {
    if (g_client == null) return JV((ok = false, error = "no client"))
    var args = from_JV(input, type<HerderChangelistArgs>)
    changelist_target(args)
    if (args.mode == "clear") {
        selection_clear(g_changelist_selection)
        return JV((ok = true, selection_count = 0))
    }
    for (repository in g_client.repositories) {
        if (repository.record.id != args.repository_id) continue
        if (args.group < 0 || args.group > 3) {
            return JV((ok = false, error = "group must be 0-3"))
        }
        let key = selection_key(git_group_scope(args.group), args.path)
        if (args.mode == "plain") {
            selection_plain_click(g_changelist_selection, args.repository_id,
                args.worktree_path, args.group, key)
        } elif (args.mode == "toggle") {
            selection_toggle_click(g_changelist_selection, args.repository_id,
                args.worktree_path, args.group, key)
        } elif (args.mode == "range") {
            let group_keys <- changelist_group_keys(repository,
                args.worktree_path, args.group)
            selection_range_click(g_changelist_selection, args.repository_id,
                args.worktree_path, args.group, group_keys, key)
        } elif (args.mode == "group") {
            let group_keys <- changelist_group_keys(repository,
                args.worktree_path, args.group)
            selection_set_group(g_changelist_selection, args.repository_id,
                args.worktree_path, args.group, group_keys, args.selected)
        } else {
            return JV((ok = false, error = "unknown mode '{args.mode}'"))
        }
        return JV((ok = true,
            selection_count = selection_count(g_changelist_selection)))
    }
    return JV((ok = false, error = "unknown repository '{args.repository_id}'"))
}

[live_command(description = "Run a Changelist action on the watcher's operation rail, as the row menu would. Args: repository_id, worktree_path (empty = shown), action = stage|unstage|discard, paths (empty = derive from the current selection; discard ALWAYS derives, split by class). Discard is unrecoverable — this rail IS the explicit second confirmation. Outcome rides herder_worktree_operation_state.")]
def herder_changelist_action(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    var args = from_JV(input, type<HerderChangelistArgs>)
    changelist_target(args)
    if (args.action == "discard") {
        if (changelist_rail_busy()) {
            return JV((ok = false, error = "the operation rail is busy"))
        }
        for (repository in g_client.repositories) {
            if (repository.record.id != args.repository_id) continue
            for (worktree in repository.worktrees) {
                if (worktree.path != args.worktree_path) continue
                var modified_paths : array<string>
                var staged_paths : array<string>
                var untracked_paths : array<string>
                var conflicted = 0
                changelist_selection_discards(repository, worktree,
                    modified_paths, staged_paths, untracked_paths, conflicted)
                let total = (length(modified_paths) + length(staged_paths)
                    + length(untracked_paths))
                if (total == 0) {
                    return JV((ok = false,
                        error = "nothing discardable is selected"))
                }
                g_client->changelist_discard(args.repository_id,
                    args.worktree_path, modified_paths, staged_paths,
                    untracked_paths)
                return JV((ok = true, action = "discard",
                    modified = length(modified_paths),
                    staged = length(staged_paths),
                    untracked = length(untracked_paths),
                    conflicted_held = conflicted))
            }
        }
        return JV((ok = false,
            error = "unknown worktree '{args.worktree_path}'"))
    }
    if (args.action == "commit") {
        if (empty(strip(args.message))) {
            return JV((ok = false, error = "a commit message is required"))
        }
        if (changelist_rail_busy()) {
            return JV((ok = false, error = "the operation rail is busy"))
        }
        g_client->changelist_commit(args.repository_id, args.worktree_path,
            strip(args.message))
        return JV((ok = true, action = "commit"))
    }
    if (args.action != "stage" && args.action != "unstage") {
        return JV((ok = false, error = "unknown action '{args.action}'"))
    }
    var paths : array<string>
    if (!empty(args.paths)) {
        paths := args.paths
    } else {
        for (repository in g_client.repositories) {
            if (repository.record.id != args.repository_id) continue
            for (worktree in repository.worktrees) {
                if (worktree.path != args.worktree_path) continue
                var stage_paths : array<string>
                var unstage_paths : array<string>
                changelist_selection_actions(repository, worktree,
                    stage_paths, unstage_paths)
                if (args.action == "stage") {
                    paths <- stage_paths
                } else {
                    paths <- unstage_paths
                }
            }
        }
    }
    if (empty(paths)) {
        return JV((ok = false,
            error = "nothing the action could apply to; select rows or pass paths"))
    }
    if (changelist_rail_busy()) {
        return JV((ok = false, error = "the operation rail is busy"))
    }
    g_client->changelist_action(args.repository_id, args.worktree_path,
        args.action, paths)
    var acted <- [for (p in paths); JV(p)]
    return JV((ok = true, action = args.action, paths <- acted))
}

[live_command(description = "Retire a durable session: the record is ARCHIVED, never erased — it leaves the default list, keeps its events/mailbox/bundles/journal, and herder_session_restore brings it back. worktree_paths names the session's worktrees to delete in the same act (empty keeps them all). Args: herd_id, worktree_paths, force.")]
def herder_session_retire(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderRetireSessionArgs>)
    if (empty(args.herd_id)) return JV((ok = false, error = "herd_id is required"))
    for (record in g_client.herd_sessions) {
        if (record.id != args.herd_id) continue
        if (record.state == "running") {
            return JV((ok = false,
                error = "session is running; close it before retiring"))
        }
        g_client->request_herd_archive(args.herd_id, args.worktree_paths,
            args.force)
        return JV((ok = true, herd_id = args.herd_id,
            archived = true, worktrees_offered = length(args.worktree_paths)))
    }
    return JV((ok = false, error = "unknown herd session '{args.herd_id}'"))
}

[live_command(description = "Bring an archived session back to the default list. Args: herd_id.")]
def herder_session_restore(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderRetireSessionArgs>)
    if (empty(args.herd_id)) return JV((ok = false, error = "herd_id is required"))
    for (record in g_client.herd_sessions) {
        if (record.id != args.herd_id) continue
        if (!record.archived) {
            return JV((ok = false, error = "session is not archived"))
        }
        g_client->request_herd_restore(args.herd_id)
        return JV((ok = true, herd_id = args.herd_id))
    }
    return JV((ok = false, error = "unknown herd session '{args.herd_id}'"))
}

struct HerderTerminalInputArgs {
    text : string
    submit : bool
    expect_session : string
}

// A second say inside the prior say's ~200ms Enter window would merge both
// texts into one paste (single global submit slot). Queue it; the frame loop
// feeds the queue only after the pending \r has flushed.
def private queue_or_send_input(text : string; submit : bool) {
    if (g_submit_stamp != 0l || !empty(g_input_queue)) {
        g_input_queue |> push(PendingSay(text = clone_string(text), submit = submit,
            session = clone_string(g_client.selected_id)))
        return
    }
    if (!empty(text)) {
        g_client->send_input(text)
        if (submit) {
            g_submit_stamp = ref_time_ticks()
            g_submit_session = g_client.selected_id
        }
    } elif (submit) {
        g_client->send_input("\r")
    }
}

[live_command(description = "Type into the attached session's terminal. Args: text, submit (true appends Enter), optional expect_session (reject if another session is attached). Requires the controller lease.")]
def herder_terminal_input(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    if (!g_client.attached) {
        return JV((ok = false, error = "no session attached; herder_attach_session first"))
    }
    if (!g_client.controls) {
        return JV((ok = false, error = "controller lease not held yet; retry shortly"))
    }
    let args = from_JV(input, type<HerderTerminalInputArgs>)
    if (!empty(args.expect_session) && args.expect_session != g_client.selected_id) {
        return JV((ok = false, error = "attached session is {g_client.selected_id}, " +
            "expected {args.expect_session}"))
    }
    if (empty(args.text) && !args.submit) {
        return JV((ok = false, error = "nothing to send"))
    }
    // The Enter must arrive OUTSIDE the text's paste window or Ink-based TUIs
    // (Claude Code) keep it as part of the paste: text now, \r deferred ~200ms
    // via the frame loop. A lone submit (empty text) sends immediately.
    queue_or_send_input(args.text, args.submit)
    return JV((ok = true, bytes = length(args.text) + (args.submit ? 1 : 0),
        session_id = g_client.selected_id))
}

struct HerderTerminalStateArgs {
    ansi : bool
}

var private g_say_baseline : array<string>
var private g_say_revision = 0

struct HerderSessionKeyArgs {
    key : string
}

def private control_key_bytes(name : string) : string {
    let esc = to_char(27)
    if (name == "enter") return "\r"
    if (name == "escape" || name == "esc") return esc
    if (name == "ctrl-c") return to_char(3)
    if (name == "ctrl-d") return to_char(4)
    if (name == "tab") return "\t"
    if (name == "shift-tab") return "{esc}[Z"
    if (name == "up") return "{esc}[A"
    if (name == "down") return "{esc}[B"
    if (name == "right") return "{esc}[C"
    if (name == "left") return "{esc}[D"
    if (name == "backspace") return to_char(127)
    return ""
}

[live_command(description = "Send a control key to the attached session. Args: key = enter | escape | ctrl-c | ctrl-d | tab | shift-tab | up | down | left | right | backspace. escape interrupts a running agent.")]
def herder_session_key(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    if (!g_client.attached) {
        return JV((ok = false, error = "no session attached"))
    }
    if (!g_client.controls) {
        return JV((ok = false, error = "controller lease not held yet; retry shortly"))
    }
    let args = from_JV(input, type<HerderSessionKeyArgs>)
    let bytes = control_key_bytes(to_lower(strip(args.key)))
    if (empty(bytes)) {
        return JV((ok = false, error = "unknown key '{args.key}'"))
    }
    g_client->send_input(bytes)
    return JV((ok = true, key = to_lower(strip(args.key)),
        session_id = g_client.selected_id))
}

struct HerderSayArgs {
    text : string
    herd_id : string
    session_id : string
}

[live_command(description = "Say something to a session (text then Enter as separate writes) and mark the reply baseline. Args: text; optional herd_id or session_id attaches first — an attach returns attach_pending=true, retry once the controller lease arrives.")]
def herder_session_say(input : JsonValue?) : JsonValue? {
    if (g_client == null || !g_client.connected) {
        return JV((ok = false, error = "watcher is not connected"))
    }
    let args = from_JV(input, type<HerderSayArgs>)
    var target = args.session_id
    if (empty(target) && !empty(args.herd_id)) {
        for (record in g_client.herd_sessions) {
            if (record.id == args.herd_id) {
                target = record.pty_session_id
                break
            }
        }
    }
    if (!empty(target) && target != g_client.selected_id) {
        ui_event("live-attach-session {target}")
        g_client->attach_session(target)
        g_open_terminal_pending = true
        return JV((ok = false, attach_pending = true,
            error = "attached {target}; retry say once the controller lease arrives"))
    }
    if (!g_client.attached) {
        return JV((ok = false, error = "no session attached; pass herd_id or session_id"))
    }
    if (!g_client.controls) {
        return JV((ok = false, error = "controller lease not held yet; retry shortly"))
    }
    if (empty(args.text)) return JV((ok = false, error = "text is required"))
    delete g_say_baseline
    g_say_baseline <- split(g_terminal_view.screen_text, "\n")
    g_say_revision = g_terminal_view.content_revision
    queue_or_send_input(args.text, true)
    return JV((ok = true, session_id = g_client.selected_id,
        baseline_revision = g_say_revision))
}

[live_command(description = "Read what the attached session said since the last herder_session_say: changed flag, the new lines (line diff vs the say-time screen), and the full screen text.")]
def herder_session_reply(_input : JsonValue?) : JsonValue? {
    if (g_client == null) return JV((ok = false, error = "client is unavailable"))
    let revision = g_terminal_view.content_revision
    var inscope current <- split(g_terminal_view.screen_text, "\n")
    var inscope seen : table<string; int>
    for (line in g_say_baseline) {
        seen[line]++
    }
    var new_lines : array<JsonValue?>
    for (line in current) {
        if (empty(strip(line))) continue
        if ((seen?[line] ?? 0) > 0) {
            seen[line]--
            continue
        }
        new_lines |> push(JV(line))
    }
    return JV((ok = g_client.attached,
        session_id = g_client.selected_id,
        changed = revision != g_say_revision,
        revision = revision,
        baseline_revision = g_say_revision,
        new_lines = new_lines,
        screen_text = g_terminal_view.screen_text))
}

[live_command(description = "Read the attached terminal screen text and viewport state. Args: ansi (true adds the checkpoint ANSI with full SGR colors for attribute inspection).")]
def herder_terminal_state(input : JsonValue?) : JsonValue? {
    if (g_client == null) return JV((ok = false, error = "client is unavailable"))
    let args = from_JV(input, type<HerderTerminalStateArgs>)
    if (args.ansi) {
        return JV((ok = g_client.attached,
            attached = g_client.attached,
            session_id = g_client.selected_id,
            ansi = terminal_checkpoint_ansi(g_terminal)))
    }
    return JV((ok = g_client.attached,
        attached = g_client.attached,
        controls = g_client.controls,
        session_id = g_client.selected_id,
        columns = g_terminal_view.columns,
        rows = g_terminal_view.rows,
        cursor_row = g_terminal_view.cursor_row,
        cursor_column = g_terminal_view.cursor_column,
        scroll_offset = g_terminal_view.scroll_offset,
        scrollback_rows = g_terminal_view.scrollback_rows,
        content_revision = g_terminal_view.content_revision,
        zoom_percent = content_zoom_percent(CONTENT_TERMINAL),
        ui_zoom_percent = g_zoom_percent,
        screen_text = g_terminal_view.screen_text))
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
            g_selection_explicit = true
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

[live_command(description = "Switch Git Status perspective. Args: perspective = pr | history | tree | project.")]
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
    } elif (perspective == "project") {
        GIT_PROJECT_TAB.pending_select = true
    } else {
        return JV((ok = false, error = "perspective must be pr, history, tree, or project"))
    }
    GIT_CHANGELIST_WIN.open = true
    GIT_ACTIVITY_WIN.open = true
    GIT_ACTIVITY_WIN.pending_raise = true
    return JV((ok = true, perspective = perspective))
}

[live_command(description = "Inspect the Git Activity Project tab file-tree state.")]
def herder_project_state(_input : JsonValue?) : JsonValue? {
    var folders, files, dirty : int
    project_model_counts(folders, files, dirty)
    return JV((ok = empty(g_worktree_files_error),
        selected = GIT_PROJECT_TAB.selected,
        loading = g_worktree_files_loading,
        error = g_worktree_files_error,
        file_count = length(g_worktree_files),
        identity = g_worktree_files_identity,
        expanded_count = length(g_files_expanded),
        model_folders = folders,
        model_files = files,
        model_dirty = dirty,
        sort = g_ui_settings.project_sort,
        show_size = g_ui_settings.project_show_size,
        show_date = g_ui_settings.project_show_date,
        filter = project_filter_text(),
        filter_matches = project_filter_match_count()))
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
        zoom_percent = content_zoom_percent(
            g_inspector.mode == GitInspectorMode.view ? CONTENT_VIEW : CONTENT_DIFF),
        ui_zoom_percent = g_zoom_percent,
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

var SHORTCUT_CATEGORY : table<int; NarrativeState>

var SHORTCUT_TITLE : table<int; NarrativeState>

var SHORTCUT_TITLE_JOIN : table<int; EmptyMarkerState>

var SHORTCUT_EDIT : table<int; ClickState>

var SHORTCUT_CLEAR_JOIN : table<int; EmptyMarkerState>

var SHORTCUT_CLEAR : table<int; ClickState>

var SHORTCUT_CONFLICT_JOIN : table<int; EmptyMarkerState>

var SHORTCUT_CONFLICT : table<int; NarrativeState>

def private draw_shortcuts_tab() {
    if (!empty(g_command_registry.capture_command_id)) {
        text_colored(SHORTCUT_CAPTURE_TEXT,
            (color = token_warning_color(),
                text = "Press the new shortcut (Escape cancels)"))
    } elif (!empty(g_command_registry.status)) {
        text_disabled(SHORTCUT_STATUS_TEXT, (text = g_command_registry.status))
    } else {
        text_disabled(SHORTCUT_HINT_TEXT,
            (text = "Click a binding to change it; changes save immediately"))
    }
    if (small_button(SHORTCUT_RESET_ALL, (text = "Restore defaults"))) {
        command_reset_bindings(g_command_registry)
    }
    separator(SHORTCUT_HEADER_SEPARATOR)
    var category = ""
    for (index, definition in count(), g_command_registry.definitions) {
        if (definition.category != category) {
            category = definition.category
            text(SHORTCUT_CATEGORY[index], (text = category))
        }
        text_disabled(SHORTCUT_TITLE[index], (text = "    {definition.title}"))
        same_line(SHORTCUT_TITLE_JOIN[index])
        let current = command_shortcut_text(g_command_registry, definition.id)
        let armed = g_command_registry.capture_command_id == definition.id
        var chord_text : string = "press keys..."
        if (!armed) {
            chord_text = empty(current) ? "unbound" : current
        }
        if (small_button(SHORTCUT_EDIT[index], (text = "{chord_text}##sc{index}"))) {
            g_command_registry.capture_command_id = definition.id
            g_command_registry.status = ""
        }
        if (!empty(current) && !armed) {
            same_line(SHORTCUT_CLEAR_JOIN[index])
            if (small_button(SHORTCUT_CLEAR[index], (text = "clear##rs{index}"))) {
                command_clear_binding(g_command_registry, definition.id, definition.scope)
            }
            let conflict = herd_command_conflict(definition.id)
            if (!empty(conflict)) {
                same_line(SHORTCUT_CONFLICT_JOIN[index])
                text_colored(SHORTCUT_CONFLICT[index],
                    (color = token_error_color(),
                        text = "conflicts with {conflict}"))
            }
        }
    }
}

// Settings mirrors of the content scales. Declared explicitly because the
// slider state is passed into a shared helper rather than named at the call
// site, so the widget macro cannot emit these globals itself.
var SETTINGS_TERMINAL_ZOOM : SliderStateInt

var SETTINGS_VIEW_ZOOM : SliderStateInt

var SETTINGS_DIFF_ZOOM : SliderStateInt

var SETTINGS_ZOOM_HINT : NarrativeState

def draw_content_zoom_slider(var state : SliderStateInt; kind, caption : string) {
    state.bounds = (50, 300)
    state.value = content_zoom_percent(kind)
    SetNextItemWidth(240.0f)
    if (slider_int(state, (text = caption))) {
        set_content_zoom(kind, state.value)
    }
}

def draw_settings_window() {
    tab_bar(SETTINGS_TABS, (text = "Settings sections", flags = ImGuiTabBarFlags.None)) {
        tab_item(SETTINGS_GENERAL_TAB, (text = "General", closable = false,
                flags = ImGuiTabItemFlags.None)) {
            SETTINGS_UI_ZOOM.bounds = (50, 300)
            SETTINGS_UI_ZOOM.value = g_zoom_percent
            SetNextItemWidth(240.0f)
            if (slider_int(SETTINGS_UI_ZOOM, (text = "UI zoom %"))) {
                set_ui_zoom(SETTINGS_UI_ZOOM.value)
            }
            text_disabled(SETTINGS_ZOOM_HINT,
                (text = "UI zoom sizes every title, status bar and label. " +
                    "Each content below has its own size, also set by " +
                    "Ctrl+wheel over it."))
            draw_content_zoom_slider(SETTINGS_TERMINAL_ZOOM, CONTENT_TERMINAL,
                "Terminal text %")
            draw_content_zoom_slider(SETTINGS_VIEW_ZOOM, CONTENT_VIEW,
                "View text %")
            draw_content_zoom_slider(SETTINGS_DIFF_ZOOM, CONTENT_DIFF,
                "Diff text %")
            SETTINGS_SHOW_EXITED.value = g_show_exited
            if (checkbox(SETTINGS_SHOW_EXITED, (text = "Show exited sessions"))) {
                g_show_exited = SETTINGS_SHOW_EXITED.value
                g_ui_settings.show_exited = g_show_exited
                settings_save()
            }
            SETTINGS_SHOW_TASKS.value = g_show_tasks
            if (checkbox(SETTINGS_SHOW_TASKS, (text = "Show watcher task sessions"))) {
                g_show_tasks = SETTINGS_SHOW_TASKS.value
                g_ui_settings.show_tasks = g_show_tasks
                settings_save()
            }
            separator(SETTINGS_GENERAL_SEPARATOR)
            text_disabled(SETTINGS_PATH_TEXT,
                (text = "Saved immediately to {settings_path()}"))
        }
        tab_item(SETTINGS_SHORTCUTS_TAB, (text = "Shortcuts", closable = false,
                flags = ImGuiTabItemFlags.None)) {
            draw_shortcuts_tab()
        }
    }
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
