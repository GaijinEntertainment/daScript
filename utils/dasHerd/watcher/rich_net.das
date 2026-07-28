options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_net

require imgui/imgui_drawlist_builtin
require terminal/terminal
require imgui/imgui_docking_builtin
require imgui/imgui_table_builtin
require dashv/dashv_boost
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
require math
require strings
require ./rich_state.das

class WatcherRichClient : HvWebSocketClient {
    connected : bool
    attached : bool
    controls : bool
    client_id : int
    selected_id : string
    attached_id : string
    sessions : array<WatcherSessionInfo>
    repositories : array<RepositorySnapshot>
    // The watcher's one running worktree operation, pushed as it progresses.
    operation : WorktreeOperation = WorktreeOperation()
    herd_profiles : array<HerdProfile>
    herd_sessions : array<HerdSession>
    herd_revision : int64 = -1l
    mailbox : array<WatcherMailboxMessage>
    mailbox_revision : int64
    bundles : array<WatcherReviewBundle>
    bundles_revision : int64
    agent_mark_revision : int64 = -1l
    agent_mark_files : table<string; bool>
    error : string
    last_heartbeat : int64
    last_list : int64
    last_reconnect : int64
    control_retry_started : int64
    last_control_retry : int64
    sent_input_bytes : int64
    desired_columns : int
    desired_rows : int
    sent_columns : int
    sent_rows : int
    last_review_identity : string
    last_history_identity : string
    last_refs_identity : string
    git_action_request_id : int
    git_action_loading : bool
    git_action_file_path : string
    git_action_status : string
    // A changelist action the watcher refused to start (rail busy, bad path).
    changelist_notice : string
    git_failure_revisions : table<string; int64>
    git_failure_baseline_ready : bool
    connect_count : int64
    disconnect_count : int64
    reconnect_attempt_count : int64
    message_count : int64
    repository_message_count : int64
    heartbeat_send_count : int64
    heartbeat_send_failure_count : int64
    heartbeat_last_result : int
    heartbeat_receive_count : int64
    list_send_count : int64
    list_send_failure_count : int64
    list_last_result : int
    worktree_files_request_id : int

    def override onOpen {
        connected = true
        connect_count++
        delete git_failure_revisions
        delete agent_mark_files
        agent_mark_revision = -1l
        git_failure_baseline_ready = false
        // A restarted watcher's revision counter starts over; reset so its
        // first herd broadcast always applies instead of racing stale state.
        herd_revision = -1l
        error = ""
        last_reconnect = ref_time_ticks()
        last_heartbeat = ref_time_ticks()
        last_list = ref_time_ticks()
        print("dasHerd client: watcher connected (generation {connect_count})\n")
        send("\{\"op\":\"list\"\}")
    }

    def override onClose {
        let was_connected = connected
        connected = false
        disconnect_count++
        attached = false
        controls = false
        attached_id = ""
        control_retry_started = 0l
        last_control_retry = 0l
        sent_columns = 0
        sent_rows = 0
        last_review_identity = ""
        last_history_identity = ""
        last_refs_identity = ""
        g_worktree_files_identity = ""
        g_worktree_files_loading = false
        if (git_action_loading) {
            git_action_loading = false
            git_action_status = "interrupted"
        }
        if (g_inspector.loading) {
            let disconnect_error = "watcher connection closed during file inspection"
            g_inspector.loading = false
            g_inspector.status = ""
            g_inspector.error = disconnect_error
            let _ = inspector_load_fail(g_inspector.prepare,
                g_inspector.prepare.generation, disconnect_error)
        }
        error = "watcher connection closed"
        last_reconnect = ref_time_ticks()
        if (was_connected) {
            print("dasHerd client: watcher connection closed; reconnecting\n")
        }
    }

    def override onMessage(message : string#) {
        message_count++
        var envelope : WatcherEnvelope
        let body = string(message)
        if (!sscan_json(body, envelope)) {
            error = "invalid watcher message"
            return
        }
        if (envelope._type == "connected") {
            var incoming : WatcherConnectedMessage
            if (sscan_json(body, incoming)) {
                client_id = incoming.client_id
            }
        } elif (envelope._type == "heartbeat") {
            heartbeat_receive_count++
        } elif (envelope._type == "sessions") {
            var incoming : WatcherSessionsMessage
            if (sscan_json(body, incoming)) {
                delete sessions
                sessions <- incoming.data
                var selected_found = empty(selected_id)
                if (!empty(selected_id)) {
                    for (session in sessions) {
                        if (session.id == selected_id) {
                            selected_found = true
                            break
                        }
                    }
                }
                if (!selected_found) {
                    selected_id = ""
                    attached_id = ""
                    attached = false
                    controls = false
                }
                // Automatic attaches never target watcher plumbing tasks and
                // never re-aim the Git surfaces: only an explicit user attach
                // follows the session's origin worktree.
                if (!attached && !empty(selected_id)) {
                    attach_session(selected_id, false)
                } elif (empty(selected_id)) {
                    for (session in sessions) {
                        if (session.state == "running"
                                && session_is_interactive(session)) {
                            attach_session(session.id, false)
                            break
                        }
                    }
                }
            }
        } elif (envelope._type == "launched") {
            var incoming : WatcherSessionMessage
            if (sscan_json(body, incoming)) {
                attach_session(incoming.session_id)
                g_open_terminal_pending = true
                g_focus_terminal_pending = true
            }
        } elif (envelope._type == "repositories") {
            repository_message_count++
            var incoming : WatcherRepositoriesMessage
            if (sscan_json(body, incoming)) {
                delete repositories
                repositories <- incoming.data
                var selection_found = false
                var selected_worktree_observed = false
                for (repository in repositories) {
                    if (repository.record.id != g_selected_repository_id) continue
                    if (empty(repository.worktrees)) {
                        selection_found = true
                    }
                    for (worktree in repository.worktrees) {
                        if (worktree.path == g_selected_worktree_path) {
                            selection_found = true
                            selected_worktree_observed = true
                        }
                    }
                }
                if (!selection_found && !empty(repositories)) {
                    ui_event("selection-fixup {repositories[0].record.id}")
                    g_selected_repository_id = repositories[0].record.id
                    if (!empty(repositories[0].worktrees)) {
                        g_selected_worktree_path = repositories[0].worktrees[0].path
                        selected_worktree_observed = true
                    } else {
                        g_selected_worktree_path = repositories[0].record.checkout_path
                    }
                }
                let review_identity = "{g_selected_repository_id}\n{g_selected_worktree_path}"
                if (selected_worktree_observed
                        && !empty(g_selected_repository_id)
                        && !empty(g_selected_worktree_path)
                        && review_identity != last_review_identity) {
                    request_repository_review(
                        g_selected_repository_id, g_selected_worktree_path)
                    request_repository_history(
                        g_selected_repository_id, g_selected_worktree_path)
                    request_repository_refs(
                        g_selected_repository_id, g_selected_worktree_path)
                }
                for (repository in repositories) {
                    let id = repository.record.id
                    let seen = key_exists(git_failure_revisions, id) ? git_failure_revisions[id] : 0l
                    if (repository.failure_revision <= seen) continue
                    git_failure_revisions[id] = repository.failure_revision
                    if (git_failure_baseline_ready
                            && !empty(repository.failure_session_id)
                            && repository.failure_session_id != selected_id
                            && repository.failure_session_id != attached_id) {
                        attach_session(repository.failure_session_id)
                        g_open_terminal_pending = true
                        g_focus_terminal_pending = true
                    }
                    if (git_failure_baseline_ready) {
                        error = repository.failure_message
                    }
                }
                git_failure_baseline_ready = true
                perf_observe_repositories(repositories)
            } else {
                error = "invalid repository snapshot message"
            }
        } elif (envelope._type == "herd_sessions") {
            var incoming = WatcherHerdSessionsMessage()
            if (sscan_json(body, incoming)) {
                delete herd_profiles
                delete herd_sessions
                herd_profiles <- incoming.profiles
                herd_sessions <- incoming.data
                herd_revision = incoming.revision
            } else {
                error = "invalid herd sessions message"
            }
        } elif (envelope._type == "herd_created" || envelope._type == "herd_relaunched") {
            var incoming : WatcherHerdLaunchedMessage
            if (sscan_json(body, incoming)) {
                attach_session(incoming.session_id)
                g_open_terminal_pending = true
                g_focus_terminal_pending = true
            }
        } elif (envelope._type == "worktree_delete_result") {
            var incoming = WatcherWorktreeDeleteResult()
            if (sscan_json(body, incoming) && !incoming.ok) {
                g_worktree_retire = WorktreeRetirePrompt(active = true,
                    repository_id = clone_string(incoming.repository_id),
                    worktree_path = clone_string(incoming.worktree_path),
                    server_reason = clone_string(incoming.reason),
                    server_detail = clone_string(incoming.detail))
            }
        } elif (envelope._type == "herd_archive_result") {
            var incoming = WatcherHerdDeleteResult()
            if (sscan_json(body, incoming) && !empty(incoming.offered)) {
                // The record is already archived. Its worktrees queue up and
                // drain one at a time through the same operation rail, so each
                // gets the same checklist, progress and cancel.
                delete g_worktree_retire_queue
                g_worktree_retire_queue <- [for (offer in incoming.offered);
                    WorktreeRetireQueueEntry(
                        repository_id = clone_string(offer.repository_id),
                        worktree_path = clone_string(offer.path))]
                open_next_queued_retire()
            }
        } elif (envelope._type == "worktree_operation") {
            var incoming = WatcherWorktreeOperationEnvelope()
            if (sscan_json(body, incoming)) {
                delete operation.log
                operation <- incoming.data
            }
        } elif (envelope._type == "changelist_action_result") {
            var incoming = WatcherChangelistActionResult()
            if (sscan_json(body, incoming)) {
                // A refusal to START (rail busy, bad path) — a failure of the
                // running operation itself rides the operation broadcast.
                changelist_notice = (incoming.ok ? ""
                    : clone_string("Could not {incoming.action}: {incoming.detail}"))
            }
        } elif (envelope._type == "worktree_commit_wip_result") {
            var incoming = WatcherWorktreeCommitWipResult()
            if (sscan_json(body, incoming) && !incoming.ok) {
                g_worktree_retire.notice = clone_string(
                    "Could not start the commit: {incoming.detail}")
            }
        } elif (envelope._type == "mailbox") {
            var incoming = WatcherMailboxMessageEnvelope()
            if (sscan_json(body, incoming)) {
                delete mailbox
                mailbox <- incoming.data
                mailbox_revision = incoming.revision
                agent_mark_revision = -1l
            } else {
                error = "invalid mailbox message"
            }
        } elif (envelope._type == "bundles") {
            var incoming = WatcherBundlesMessageEnvelope()
            if (sscan_json(body, incoming)) {
                delete bundles
                bundles <- incoming.data
                bundles_revision = incoming.revision
            } else {
                error = "invalid bundles message"
            }
        } elif (envelope._type == "mailbox_posted") {
            var incoming : WatcherMailboxPostedMessage
            if (sscan_json(body, incoming)) {
                g_last_human_request = incoming.message_id
            }
        } elif (envelope._type == "worktree_files") {
            var incoming : WatcherWorktreeFilesMessage
            if (sscan_json(body, incoming)
                    && incoming.request_id == worktree_files_request_id) {
                if (!incoming.loading) {
                    g_worktree_files_loading = false
                    g_worktree_files_error = incoming.error
                    if (empty(incoming.error)) {
                        delete g_worktree_files
                        peek_data(incoming.listing) $(bytes) {
                            var start = 0
                            for (index in range(length(bytes) + 1)) {
                                let at_end = index == length(bytes)
                                if (at_end || int(bytes[index]) == '\n') {
                                    var line_end = index
                                    if (line_end > start
                                            && int(bytes[line_end - 1]) == '\r') {
                                        line_end--
                                    }
                                    if (line_end > start) {
                                        g_worktree_files |> push(slice(
                                            incoming.listing, start, line_end))
                                    }
                                    start = index + 1
                                }
                            }
                        }
                        sort(g_worktree_files)
                    }
                }
            }
        } elif (envelope._type == "file_inspection") {
            var incoming : WatcherFileInspectionMessage
            if (sscan_json(body, incoming)) {
                install_file_inspection(incoming)
            }
        } elif (envelope._type == "git_file_action") {
            var incoming : WatcherGitFileActionMessage
            if (sscan_json(body, incoming)
                    && incoming.request_id == git_action_request_id) {
                git_action_loading = incoming.loading
                git_action_file_path = incoming.file_path
                git_action_status = incoming.status
                if (!empty(incoming.error)) {
                    error = incoming.error
                }
                if (incoming.status == "failed"
                        && !empty(incoming.task_session_id)) {
                    attach_session(incoming.task_session_id)
                    g_open_terminal_pending = true
                    g_focus_terminal_pending = true
                }
            }
        } elif (envelope._type == "attached") {
            var incoming : WatcherAttachedMessage
            if (sscan_json(body, incoming)) {
                attached_id = incoming.session_id
                attached = true
                controls = incoming.controls
                if (incoming.controls) {
                    control_retry_started = 0l
                    last_control_retry = 0l
                    error = ""
                    g_status = "Attached with control"
                } else {
                    if (control_retry_started == 0l) {
                        control_retry_started = ref_time_ticks()
                    }
                    g_status = "Attached read-only; waiting for control"
                }
                if (controls) {
                    if (desired_columns <= 0 || desired_rows <= 0) {
                        desired_columns = g_terminal.columns
                        desired_rows = g_terminal.rows
                    }
                    flush_resize()
                }
            }
        } elif (envelope._type == "detached") {
            attached = false
            controls = false
            attached_id = ""
            control_retry_started = 0l
            last_control_retry = 0l
            sent_columns = 0
            sent_rows = 0
        } elif (envelope._type == "terminal_checkpoint") {
            var incoming : WatcherTerminalCheckpointMessage
            if (sscan_json(body, incoming)) {
                reset_terminal(max(1, incoming.columns), max(1, incoming.rows))
                terminal_feed(g_terminal, incoming.ansi)
                g_status = "Terminal restored at byte {incoming.offset}"
            } else {
                error = "invalid terminal checkpoint"
            }
        } elif (envelope._type == "raw_reset") {
            var incoming : WatcherRawResetMessage
            if (sscan_json(body, incoming)) {
                reset_terminal(g_terminal.columns, g_terminal.rows)
                g_status = "Raw replay reset at byte {incoming.offset}"
            }
        } elif (envelope._type == "error") {
            var incoming : WatcherErrorMessage
            if (sscan_json(body, incoming)) {
                error = incoming.error
                if (incoming.error == "controller lease expired"
                        || incoming.error == "client does not hold the controller lease"
                        || incoming.error == "controller lease is held by another client") {
                    controls = false
                    sent_columns = 0
                    sent_rows = 0
                    if (attached && !empty(attached_id)) {
                        control_retry_started = ref_time_ticks()
                        last_control_retry = 0l
                    }
                } elif (incoming.error == "unknown operation"
                        && attached && !controls && !empty(attached_id)) {
                    // Compatibility with a watcher that predates
                    // `claim_control`: reattach without raw replay or terminal
                    // reset, then flush the latest measured PTY size when the
                    // attached response grants control.
                    send("\{\"op\":\"attach\",\"session_id\":" +
                        "{sprint_json(attached_id, false)}," +
                        "\"control\":true,\"raw\":false\}")
                }
            }
        }
    }

    def override onMessageFrame(message : string#; byte_count : int; opcode : ws_opcode) {
        if (opcode == ws_opcode.WS_OPCODE_TEXT) {
            onMessage(message)
        } elif (opcode == ws_opcode.WS_OPCODE_BINARY) {
            feed_terminal_frame(message, byte_count)
        }
    }

    def session_is_interactive(session : WatcherSessionInfo const) : bool {
        return !session_is_task(session.kind)
    }

    def attach_session(session_id : string; follow_origin : bool = true) {
        if (!connected || empty(session_id)) return
        var columns = 100
        var rows = 30
        var origin_repository_id, origin_worktree_path : string
        for (session in sessions) {
            if (session.id == session_id) {
                columns = max(20, session.columns)
                rows = max(5, session.rows)
                origin_repository_id = session.repository_id
                origin_worktree_path = session.worktree_path
                break
            }
        }
        // Auto-aim policy (note 9): an explicit worktree selection is sacred,
        // but a seeded/default selection may always aim at the attach origin —
        // even for task auto-attaches that pass follow_origin=false.
        let may_aim = follow_origin || !g_selection_explicit
        if (may_aim && !empty(origin_repository_id) && !empty(origin_worktree_path)) {
            let origin_changed = (g_selected_repository_id
                    != origin_repository_id
                || g_selected_worktree_path != origin_worktree_path)
            g_selected_repository_id = origin_repository_id
            g_selected_worktree_path = origin_worktree_path
            if (origin_changed) {
                request_repository_review(
                    origin_repository_id, origin_worktree_path)
                request_repository_history(
                    origin_repository_id, origin_worktree_path)
                request_repository_refs(
                    origin_repository_id, origin_worktree_path)
            }
        }
        ui_event("attach-session {session_id} origin={origin_worktree_path}")
        selected_id = session_id
        attached = false
        controls = false
        control_retry_started = ref_time_ticks()
        last_control_retry = 0l
        sent_columns = 0
        sent_rows = 0
        error = ""
        reset_terminal(columns, rows)
        send("\{\"op\":\"attach\",\"session_id\":{sprint_json(session_id, false)}," +
            "\"control\":true,\"raw\":true\}")
    }

    def launch(command_line, cwd : string; repository_id : string = "";
               worktree_path : string = "") {
        if (!connected) {
            error = "watcher is not connected"
            return
        }
        send("\{\"op\":\"launch\",\"command_line\":{sprint_json(command_line, false)}," +
            "\"cwd\":{sprint_json(cwd, false)},\"columns\":100,\"rows\":30," +
            "\"timeout_ms\":0,\"display_name\":\"PowerShell\",\"kind\":\"interactive-shell\"," +
            "\"target_id\":\"local\",\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}\}")
    }

    def request_herd_create(name, profile_id, workflow, repository_id,
                            worktree_path, task_context : string) {
        if (!connected) {
            error = "watcher is not connected"
            return
        }
        send("\{\"op\":\"herd_create\",\"name\":{sprint_json(name, false)}," +
            "\"profile_id\":{sprint_json(profile_id, false)}," +
            "\"workflow\":{sprint_json(workflow, false)}," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}," +
            "\"task_context\":{sprint_json(task_context, false)}\}")
    }

    def request_herd_relaunch(herd_session_id : string) {
        if (!connected || empty(herd_session_id)) return
        send("\{\"op\":\"herd_relaunch\",\"herd_session_id\":" +
            "{sprint_json(herd_session_id, false)}\}")
    }

    def request_herd_close(herd_session_id : string) {
        if (!connected || empty(herd_session_id)) return
        send("\{\"op\":\"herd_close\",\"herd_session_id\":" +
            "{sprint_json(herd_session_id, false)}\}")
    }

    def request_herd_archive(herd_session_id : string;
                             worktree_paths : array<string> const;
                             force : bool = false) {
        if (!connected || empty(herd_session_id)) return
        var inscope parts : array<string>
        parts |> reserve(length(worktree_paths))
        for (path in worktree_paths) {
            parts |> push(sprint_json(path, false))
        }
        let path_list = join(parts, ",")
        send("\{\"op\":\"herd_archive\",\"herd_session_id\":" +
            "{sprint_json(herd_session_id, false)},\"worktree_paths\":[" +
            "{path_list}],\"force\":{force}\}")
    }

    def request_herd_restore(herd_session_id : string) {
        if (!connected || empty(herd_session_id)) return
        send("\{\"op\":\"herd_restore\",\"herd_session_id\":" +
            "{sprint_json(herd_session_id, false)}\}")
    }

    def request_worktree_delete(repository_id, worktree_path : string;
                                force : bool = false) {
        if (!connected || empty(worktree_path)) return
        send("\{\"op\":\"worktree_delete\",\"repository_id\":" +
            "{sprint_json(repository_id, false)},\"worktree_path\":" +
            "{sprint_json(worktree_path, false)},\"force\":{force}\}")
    }

    def request_worktree_operation_cancel() {
        if (!connected) return
        send("\{\"op\":\"worktree_operation_cancel\"\}")
    }

    def changelist_action(repository_id, worktree_path, action : string;
                          paths : array<string>) {
        //! Multi-file stage/unstage on the watcher's operation rail. Outcome
        //! rides the worktree_operation broadcast; a refusal to start comes
        //! back as changelist_action_result.
        if (!connected || empty(repository_id) || empty(worktree_path)
                || empty(paths)) return
        changelist_notice = ""
        send("\{\"op\":\"changelist_action\",\"repository_id\":" +
            "{sprint_json(repository_id, false)},\"worktree_path\":" +
            "{sprint_json(worktree_path, false)},\"action\":" +
            "{sprint_json(action, false)},\"worktree_paths\":" +
            "{sprint_json(paths, false)}\}")
    }

    def changelist_discard(repository_id, worktree_path : string;
                           modified_paths : array<string>;
                           staged_paths : array<string>;
                           untracked_paths : array<string>) {
        //! The unrecoverable one. The caller (dialog or rail) owns the
        //! confirmation; this only carries the three class lists, because
        //! each needs a different git command.
        if (!connected || empty(repository_id) || empty(worktree_path)
                || (empty(modified_paths) && empty(staged_paths)
                    && empty(untracked_paths))) return
        changelist_notice = ""
        send("\{\"op\":\"changelist_discard\",\"repository_id\":" +
            "{sprint_json(repository_id, false)},\"worktree_path\":" +
            "{sprint_json(worktree_path, false)},\"worktree_paths\":" +
            "{sprint_json(modified_paths, false)},\"staged_paths\":" +
            "{sprint_json(staged_paths, false)},\"untracked_paths\":" +
            "{sprint_json(untracked_paths, false)}\}")
    }

    def request_worktree_commit_wip(repository_id, worktree_path,
                                    message : string) {
        if (!connected || empty(worktree_path) || empty(message)) return
        send("\{\"op\":\"worktree_commit_wip\",\"repository_id\":" +
            "{sprint_json(repository_id, false)},\"worktree_path\":" +
            "{sprint_json(worktree_path, false)},\"text\":" +
            "{sprint_json(message, false)}\}")
    }

    def request_herd_rename(herd_session_id, name : string) {
        if (!connected || empty(herd_session_id)) return
        send("\{\"op\":\"herd_rename\",\"herd_session_id\":" +
            "{sprint_json(herd_session_id, false)},\"name\":" +
            "{sprint_json(name, false)}\}")
    }

    def request_herd_add_satellite(herd_session_id, repository_id,
                                   worktree_path : string) {
        if (!connected || empty(herd_session_id) || empty(worktree_path)) return
        send("\{\"op\":\"herd_add_satellite\",\"herd_session_id\":" +
            "{sprint_json(herd_session_id, false)},\"repository_id\":" +
            "{sprint_json(repository_id, false)},\"worktree_path\":" +
            "{sprint_json(worktree_path, false)}\}")
    }

    def create_worktree_and_launch_agent(source_worktree, repository_id,
                                         new_worktree_path, branch, base_ref,
                                         task_context, profile_id, name,
                                         workflow : string) {
        if (!connected) {
            error = "watcher is not connected"
            return
        }
        send("\{\"op\":\"launch_agent_worktree\",\"repository_id\":" +
            "{sprint_json(repository_id, false)},\"worktree_path\":" +
            "{sprint_json(source_worktree, false)},\"new_worktree_path\":" +
            "{sprint_json(new_worktree_path, false)},\"branch\":" +
            "{sprint_json(branch, false)},\"base_ref\":" +
            "{sprint_json(base_ref, false)},\"task_context\":" +
            "{sprint_json(task_context, false)},\"profile_id\":" +
            "{sprint_json(profile_id, false)},\"name\":" +
            "{sprint_json(name, false)},\"workflow\":" +
            "{sprint_json(workflow, false)}\}")
    }

    def add_repository(path : string) {
        if (!connected || empty(path)) return
        send("\{\"op\":\"repository_add\",\"repository_path\":{sprint_json(path, false)}," +
            "\"target_id\":\"local\"\}")
    }

    def refresh_repository(repository_id : string) {
        if (!connected || empty(repository_id)) return
        send("\{\"op\":\"repository_refresh\",\"repository_id\":" +
            "{sprint_json(repository_id, false)}\}")
    }

    def request_repository_review(repository_id : string; worktree_path : string) {
        if (!connected || empty(repository_id) || empty(worktree_path)) return
        last_review_identity = "{repository_id}\n{worktree_path}"
        perf_begin(g_perf_review, repository_id, worktree_path, "")
        send("\{\"op\":\"repository_review\"," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}\}")
    }

    def request_repository_history(repository_id : string;
                                   worktree_path : string;
                                   ref_name : string = "") {
        if (!connected || empty(repository_id) || empty(worktree_path)) return
        last_history_identity = "{repository_id}\n{worktree_path}\n{ref_name}"
        perf_begin(g_perf_history, repository_id, worktree_path, ref_name)
        send("\{\"op\":\"repository_history\"," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}," +
            "\"revision\":{sprint_json(ref_name, false)}\}")
    }

    def request_repository_commit_files(repository_id : string;
                                        worktree_path : string;
                                        commit_hash : string) {
        if (!connected || empty(repository_id) || empty(worktree_path)
                || empty(commit_hash)) return
        perf_begin(g_perf_commit, repository_id, worktree_path, commit_hash)
        send("\{\"op\":\"repository_commit_files\"," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}," +
            "\"revision\":{sprint_json(commit_hash, false)}\}")
    }

    def request_repository_refs(repository_id : string; worktree_path : string) {
        if (!connected || empty(repository_id) || empty(worktree_path)) return
        last_refs_identity = "{repository_id}\n{worktree_path}"
        perf_begin(g_perf_refs, repository_id, worktree_path, "")
        send("\{\"op\":\"repository_refs\"," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}\}")
    }

    def request_worktree_files(repository_id, worktree_path : string) {
        if (!connected || empty(repository_id) || empty(worktree_path)) return
        worktree_files_request_id++
        g_worktree_files_loading = true
        g_worktree_files_error = ""
        g_worktree_files_identity = "{repository_id}\n{worktree_path}"
        send("\{\"op\":\"worktree_files\"," +
            "\"request_id\":{worktree_files_request_id}," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}\}")
    }

    def post_file_focus(repository_id, worktree_path, file_path : string) {
        if (!connected || empty(selected_id) || empty(file_path)) return
        let payload = ("\{\"op\":\"mailbox_post\",\"session_id\":" +
            "{sprint_json(selected_id, false)},\"direction\":\"inbox\"," +
            "\"sender\":\"human:local\",\"kind\":\"focus_set\"," +
            "\"subject\":\"Look at this file\",\"notify\":true,\"focus\":\{" +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}," +
            "\"comparison\":\"working\"," +
            "\"summary\":\"Human-selected file\",\"targets\":[\{" +
            "\"file_path\":{sprint_json(file_path, false)}," +
            "\"whole_file\":true,\"ranges\":[]\}]\}\}")
        send(payload)
    }

    def git_file_action(repository_id : string; worktree_path : string;
                        file_path : string; action : string) {
        if (!connected || git_action_loading || empty(repository_id)
                || empty(worktree_path) || empty(file_path)) return
        git_action_request_id++
        git_action_loading = true
        git_action_file_path = file_path
        git_action_status = "running"
        send("\{\"op\":\"git_file_action\"," +
            "\"request_id\":{git_action_request_id}," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}," +
            "\"file_path\":{sprint_json(file_path, false)}," +
            "\"action\":{sprint_json(action, false)}\}")
    }

    def inspect_file(repository_id, worktree_path, file_path,
                     comparison : string;
                     diff_context : string = "compact";
                     git_revision : string = "") {
        if (!connected) {
            error = "watcher is not connected"
            g_inspector.loading = false
            g_inspector.error = error
            return
        }
        if (empty(repository_id) || empty(worktree_path)
                || empty(file_path)) return
        begin_file_inspection(repository_id, worktree_path, file_path,
            comparison, diff_context, git_revision)
        g_perf_inspect_started = ref_time_ticks()
        send("\{\"op\":\"file_inspect\",\"request_id\":{g_inspector.request_id}," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}," +
            "\"file_path\":{sprint_json(file_path, false)}," +
            "\"comparison\":{sprint_json(comparison, false)}," +
            "\"revision\":{sprint_json(git_revision, false)}," +
            "\"diff_context\":{sprint_json(diff_context, false)}\}")
    }

    def post_selection_request(start_byte, end_byte : int) {
        if (!connected || empty(selected_id) || start_byte < 0
                || end_byte <= start_byte
                || empty(g_inspector.installed_file_path)) return
        let payload = ("\{\"op\":\"mailbox_post\",\"session_id\":" +
            "{sprint_json(selected_id, false)},\"direction\":\"inbox\"," +
            "\"sender\":\"human:local\",\"kind\":\"focus_set\"," +
            "\"subject\":\"Look at that\",\"notify\":true,\"focus\":\{" +
            "\"repository_id\":{sprint_json(g_inspector.installed_repository_id, false)}," +
            "\"worktree_path\":{sprint_json(g_inspector.installed_worktree_path, false)}," +
            "\"comparison\":{sprint_json(g_inspector.installed_comparison, false)}," +
            "\"revision\":{sprint_json(g_inspector.installed_git_revision, false)}," +
            "\"summary\":\"Human-selected code\",\"targets\":[\{" +
            "\"file_path\":{sprint_json(g_inspector.installed_file_path, false)}," +
            "\"whole_file\":false,\"ranges\":[\{\"start_byte\":{start_byte}," +
            "\"end_byte\":{end_byte},\"caption\":\"Look at that\"\}]\}]\}\}")
        send(payload)
    }

    def post_error_request(context, message : string) {
        if (!connected || empty(selected_id) || empty(message)) return
        let subject = "{context}: {message}"
        var focus_json = ("\"focus\":\{\"summary\":" +
            "{sprint_json(subject, false)},\"targets\":[]\}")
        if (!empty(g_inspector.installed_repository_id)
                && !empty(g_inspector.installed_worktree_path)
                && !empty(g_inspector.installed_file_path)) {
            focus_json = ("\"focus\":\{" +
                "\"repository_id\":{sprint_json(g_inspector.installed_repository_id, false)}," +
                "\"worktree_path\":{sprint_json(g_inspector.installed_worktree_path, false)}," +
                "\"comparison\":{sprint_json(g_inspector.installed_comparison, false)}," +
                "\"revision\":{sprint_json(g_inspector.installed_git_revision, false)}," +
                "\"summary\":{sprint_json(subject, false)},\"targets\":[\{" +
                "\"file_path\":{sprint_json(g_inspector.installed_file_path, false)}," +
                "\"whole_file\":true,\"ranges\":[]\}]\}")
        }
        send(("\{\"op\":\"mailbox_post\",\"session_id\":" +
            "{sprint_json(selected_id, false)},\"direction\":\"inbox\"," +
            "\"sender\":\"human:local\",\"kind\":\"error\"," +
            "\"subject\":{sprint_json(subject, false)},\"notify\":true," +
            "{focus_json}\}"))
    }

    def acknowledge_mailbox(message_id : string) {
        if (!connected || empty(selected_id) || empty(message_id)) return
        send("\{\"op\":\"mailbox_state\",\"session_id\":" +
            "{sprint_json(selected_id, false)},\"message_id\":" +
            "{sprint_json(message_id, false)},\"status\":\"acknowledged\"\}")
    }

    def agent_mark_key(repository_id, worktree_path, file_path : string) : string {
        return ("{length(repository_id)}:{repository_id}" +
            "{length(worktree_path)}:{worktree_path}{file_path}")
    }

    def refresh_agent_marks() {
        if (agent_mark_revision == mailbox_revision) return
        delete agent_mark_files
        for (message in mailbox) {
            if (message.direction != "outbox") continue
            for (target in message.focus.targets) {
                agent_mark_files[agent_mark_key(
                    watcher_focus_repository(message.focus, target),
                    watcher_focus_worktree(message.focus, target),
                    target.file_path)] = true
            }
        }
        agent_mark_revision = mailbox_revision
    }

    def marks_file_for_agent(repository_id, worktree_path,
                             file_path : string) : bool {
        refresh_agent_marks()
        return key_exists(agent_mark_files,
            agent_mark_key(repository_id, worktree_path, file_path))
    }

    // Advisory-conflict view of one worktree: attended by 2+ non-closed
    // durable sessions (as primary or satellite).
    def herd_worktree_conflicted(path : string) : bool {
        let canonical = herd_canonical_worktree(path)
        var count = 0
        for (record in herd_sessions) {
            if (record.state == "closed") continue
            if (herd_canonical_worktree(record.worktree_path) == canonical) {
                count++
                continue
            }
            for (entry in record.satellites) {
                if (herd_canonical_worktree(entry.worktree_path) == canonical) {
                    count++
                    break
                }
            }
        }
        return count > 1
    }

    def send_input(text : string) {
        if (!controls || empty(text)) return
        send("\{\"op\":\"input\",\"text\":{sprint_json(text, false)}\}")
        sent_input_bytes += long_length(text)
    }

    def request_resize(columns, rows : int) {
        desired_columns = columns
        desired_rows = rows
        flush_resize()
    }

    def flush_resize() {
        if (!controls || desired_columns <= 0 || desired_rows <= 0 ||
            (desired_columns == sent_columns && desired_rows == sent_rows)) return
        send("\{\"op\":\"resize\",\"columns\":{desired_columns},\"rows\":{desired_rows}\}")
        sent_columns = desired_columns
        sent_rows = desired_rows
    }

    def terminate_selected() {
        if (!connected || empty(selected_id)) return
        send("\{\"op\":\"terminate\",\"session_id\":{sprint_json(selected_id, false)}\}")
    }

    def refresh() {
        if (connected) send("\{\"op\":\"list\"\}")
    }

    def pump() {
        process_event_que()
        if (!connected) {
            if (get_time_nsec(last_reconnect) / 1000000l >= 1000l) {
                cleanup()
                reconnect_attempt_count++
                let result = init(g_url)
                last_reconnect = ref_time_ticks()
                if (result != 0) {
                    error = "watcher reconnect failed ({result})"
                }
            }
            return
        }
        if (get_time_nsec(last_heartbeat) / 1000000l >= 1000l) {
            heartbeat_last_result = send_result("\{\"op\":\"heartbeat\"\}")
            heartbeat_send_count++
            if (heartbeat_last_result < 0) heartbeat_send_failure_count++
            last_heartbeat = ref_time_ticks()
            // libhv accepts the write here but can lose a second write issued
            // from the same client tick. Serialize maintenance frames.
            return
        }
        if (get_time_nsec(last_list) / 1000000l >= 1000l) {
            list_last_result = send_result("\{\"op\":\"list\"\}")
            list_send_count++
            if (list_last_result < 0) list_send_failure_count++
            last_list = ref_time_ticks()
            return
        }
        if (attached && !controls && !empty(attached_id)
                && control_retry_started != 0l
                && get_time_nsec(control_retry_started) / 1000000l < 15000l
                && (last_control_retry == 0l
                    || get_time_nsec(last_control_retry) / 1000000l >= 500l)) {
            send("\{\"op\":\"claim_control\"\}")
            last_control_retry = ref_time_ticks()
        }
    }
}

var g_client : WatcherRichClient?

def open_next_queued_retire() {
    //! Pops the next worktree a session retire handed over and opens the same
    //! checklist on it. Sequencing is the client's job because the watcher
    //! runs one operation at a time.
    if (empty(g_worktree_retire_queue)) return
    let next = g_worktree_retire_queue[0]
    g_worktree_retire_queue |> erase(0)
    g_worktree_retire = WorktreeRetirePrompt(active = true,
        repository_id = clone_string(next.repository_id),
        worktree_path = clone_string(next.worktree_path))
}

def herd_session_claiming(worktree_path : string;
                          exclude_herd_id : string = "") : string {
    //! Name of the live session attending this worktree, or "" when it is
    //! free — the client-side twin of the watcher's herd_worktree_user, so the
    //! checklist reads the same verdict the watcher will enforce. Closed and
    //! archived sessions are retired and hold nothing.
    if (g_client == null) return ""
    let canonical = herd_canonical_worktree(worktree_path)
    for (record in g_client.herd_sessions) {
        if (record.id == exclude_herd_id || record.state == "closed"
            || record.archived) continue
        if (herd_canonical_worktree(record.worktree_path) == canonical) {
            return record.name
        }
        for (entry in record.satellites) {
            if (herd_canonical_worktree(entry.worktree_path) == canonical) {
                return record.name
            }
        }
    }
    return ""
}

def find_worktree_state(repository_id, worktree_path : string;
                        var found : WorktreeState) : bool {
    //! The live facts for one worktree. False when the snapshot has not seen
    //! it (yet, or any more) — the caller must then say "unknown", never
    //! assume the zeroes mean "clean".
    if (g_client == null) return false
    for (repository in g_client.repositories) {
        if (!empty(repository_id) && repository.record.id != repository_id) continue
        for (worktree in repository.worktrees) {
            if (worktree.path != worktree_path) continue
            found := worktree
            return true
        }
    }
    return false
}
