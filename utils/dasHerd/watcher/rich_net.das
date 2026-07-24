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

    def override onOpen {
        connected = true
        connect_count++
        delete git_failure_revisions
        delete agent_mark_files
        agent_mark_revision = -1l
        git_failure_baseline_ready = false
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
        return (!starts_with(session.kind, "git-")
            && session.kind != "worktree-create")
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
        if (follow_origin && !empty(origin_repository_id) && !empty(origin_worktree_path)) {
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

    def launch_codex_debug(cwd, repository_id, task_context : string) {
        if (!connected) {
            error = "watcher is not connected"
            return
        }
        let argv <- ["codex"]
        send("\{\"op\":\"launch\",\"argv\":{sprint_json(argv, false)}," +
            "\"cwd\":{sprint_json(cwd, false)},\"columns\":120,\"rows\":36," +
            "\"timeout_ms\":0,\"display_name\":\"Codex debug\"," +
            "\"kind\":\"agent-codex\",\"purpose\":\"debug\"," +
            "\"task_context\":{sprint_json(task_context, false)}," +
            "\"target_id\":\"local\",\"repository_id\":" +
            "{sprint_json(repository_id, false)},\"worktree_path\":" +
            "{sprint_json(cwd, false)}\}")
    }

    def create_worktree_and_launch_codex(source_worktree, repository_id,
                                         new_worktree_path, branch, base_ref,
                                         task_context : string) {
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
            "{sprint_json(task_context, false)}\}")
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

    def send_input(text : string) {
        if (!controls || empty(text)) return
        send("\{\"op\":\"input\",\"text\":{sprint_json(text, false)}\}")
        sent_input_bytes += int64(length(text))
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
