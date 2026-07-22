options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

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
require dashv/dashv_boost
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
require ./inspection_worker.das
require ./image_preview_gl.das
require math
require strings

struct private WatcherEnvelope {
    @rename = "type" _type : string
}

struct private WatcherConnectedMessage {
    @rename = "type" _type : string
    client_id : int
}

struct private WatcherAttachedMessage {
    @rename = "type" _type : string
    session_id : string
    controls : bool
}

struct private WatcherSessionMessage {
    @rename = "type" _type : string
    session_id : string
}

struct private WatcherErrorMessage {
    @rename = "type" _type : string
    error : string
}

struct private WatcherSessionsMessage {
    @rename = "type" _type : string
    data : array<WatcherSessionInfo>
}

struct private WatcherRepositoriesMessage {
    @rename = "type" _type : string
    data : array<RepositorySnapshot>
}

struct private WatcherMailboxMessageEnvelope {
    @rename = "type" _type : string
    session_id : string
    revision : int64
    data : array<WatcherMailboxMessage>
}

struct private WatcherBundlesMessageEnvelope {
    @rename = "type" _type : string
    session_id : string
    revision : int64
    data : array<WatcherReviewBundle>
}

struct private WatcherMailboxPostedMessage {
    @rename = "type" _type : string
    message_id : string
    revision : int64
}

struct private WatcherFileInspectionMessage {
    @rename = "type" _type : string
    request_id : int
    repository_id : string
    worktree_path : string
    file_path : string
    comparison : string
    revision : string
    diff_context : string
    task_session_id : string
    loading : bool
    patch : string
    view_text : string
    view_base64 : string
    view_byte_count : int
    view_available : bool
    status : string
    error : string
}

struct private WatcherGitFileActionMessage {
    @rename = "type" _type : string
    request_id : int
    repository_id : string
    worktree_path : string
    file_path : string
    action : string
    task_session_id : string
    loading : bool
    status : string
    error : string
}

struct private WatcherRawResetMessage {
    @rename = "type" _type : string
    offset : int64
}

struct private HerderInspectorTextArgs {
    side : string = "view"
}

struct private HerderInspectorOpenArgs {
    repository_id : string
    worktree_path : string
    file_path : string
    comparison : string = "working"
    revision : string
    diff_context : string = "compact"
}

struct private HerderInspectorModeArgs {
    mode : string
}

struct private HerderGitWorktreeArgs {
    repository_id : string
    worktree_path : string
}

struct private HerderGitPerspectiveArgs {
    perspective : string
}

struct private HerderGitFileActionArgs {
    repository_id : string
    worktree_path : string
    file_path : string
    action : string
}

struct private HerderGitShaArgs {
    sha : string
}

struct private HerderAttentionOpenArgs {
    message_id : string
    target_index : int
}

enum private GitInspectorMode {
    diff
    view
}

let INSPECTOR_SYNTAX_LABELS : array<string> <- [
    "Auto", "Plain text", "daScript", "C", "C++", "Markdown", "JSON"]
let INSPECTOR_SYNTAX_VALUES : array<string> <- [
    "auto", "plain", "daslang", "c", "cpp", "markdown", "json"]
let INSPECTOR_CONTEXT_LABELS : array<string> <- [
    "Compact (3)", "Expanded (20)", "Full file"]
let INSPECTOR_CONTEXT_VALUES : array<string> <- [
    "compact", "expanded", "full"]
let TREE_FILTER_LABELS : array<string> <- ["Auto", "All"]

struct private GitInspectorState {
    // Request identity changes immediately when the user picks a file. The
    // installed identity changes only when its prepared document is ready.
    request_id : int
    repository_id : string
    worktree_path : string
    file_path : string
    comparison : string
    git_revision : string
    diff_context : string = "compact"
    installed_repository_id : string
    installed_worktree_path : string
    installed_file_path : string
    installed_comparison : string
    installed_git_revision : string
    installed_diff_context : string = "compact"
    installed_diff_context_choice : int
    installed_syntax_choice : int
    prepare_language : string
    pending_select_diff : bool
    task_session_id : string
    loading : bool
    status : string
    error : string
    revision : int
    prepare : InspectorLoadState = InspectorLoadState()
    prepare_elapsed_usec : int64
    mode : GitInspectorMode = GitInspectorMode.diff
    parsed : GitParsedPatch = GitParsedPatch()
    old_document : TextSourceDocument = TextSourceDocument()
    new_document : TextSourceDocument = TextSourceDocument()
    view_document : TextSourceDocument = TextSourceDocument()
    new_line_starts : array<int>
    view_line_starts : array<int>
    focus_document_valid : bool
    focus_start_byte : int = -1
    old_view : TextSourceViewState = TextSourceViewState()
    new_view : TextSourceViewState = TextSourceViewState()
    view : TextSourceViewState = TextSourceViewState()
    markdown_document : RichDocument = RichDocument()
    markdown_view : MarkdownViewState = MarkdownViewState()
    changed_source_ranges : array<TextSourceRange>
    view_source : string
    view_prepared : bool
    markdown_gutter_segments : int
    source_patch : string
    source_view_text : string
    source_view_base64 : string
    source_view_byte_count : int
    source_view_available : bool
    syntax_choice : int
    diff_context_choice : int
    resolved_language : string
    preview_texture : uint
    preview_width : int
    preview_height : int
    preview_byte_count : int
    preview_error : string
    diff_scroll_y : float
    diff_scroll_jump_pending : bool
    diff_split_fraction : float = 0.5f
    diff_current_hunk : int
    diff_left_rect : float4
    diff_right_rect : float4
    diff_scrollbar_rect : float4
}

struct private GitWindowStatus {
    text : string
    busy : bool
    error : bool
}

var private g_terminal = Terminal()
var private g_terminal_view = ImGuiTerminalState()
var private g_terminal_cache = ImGuiTerminalCache()
var private g_terminal_font : ImFont?
var private g_fonts = FontRoleSet()
var private g_font_sources : array<string>
var private g_zoom_percent = 150
var private g_status = "Starting"
var private g_url : string
var private g_token : string
var private g_selected_worktree_path : string
var private g_selected_repository_id : string
var private g_focus_terminal_pending = false
var private g_active_focus_message = ""
var private g_active_focus_target = -1
var private g_active_focus_range = -1
var private g_focus_visible = true
var private g_pending_focus_scroll = false
var private g_focus_view_scroll_pending = false
var private g_focus_view_scroll_y = 0.0f
var private g_last_human_request = ""
var private g_applied_focus_identity = ""
var private g_requested_history_commit : string
var private g_pending_clipboard_sha : string
var private g_last_copied_sha : string
var private g_clipboard_copy_status = ClipboardStatus.no_content
var private g_clipboard_copy_attempts = 0
var private g_clipboard_copy_verified = false
var private g_tree_model = GitTreeModel()
var private g_tree_model_identity : string
var private g_tree_focus_ref : string
var private g_inspector = GitInspectorState()
var private g_prepare_worker = InspectorPrepareWorker()
var private g_jobque_owned = false
var private SESSION_ROW : table<int; ToggleState>
var private ATTENTION_MESSAGE_ROW : table<int; ToggleState>
var private ATTENTION_TARGET_ROW : table<int; ToggleState>
var private BUNDLE_PARTICIPANT_ROW : table<int; ToggleState>
var private WORKTREE_ROW : table<int; ToggleState>
var private GIT_FILE_ROW : table<int; ToggleState>
var private GIT_COMMIT_TITLE_TEXT : table<int; NarrativeState>
var private GIT_COMMIT_AUTHOR_TEXT : table<int; NarrativeState>
var private GIT_TREE_REF_SAME_LINE : table<int; EmptyMarkerState>
var private GIT_TREE_REF_ROW : table<int; ClickState>
var private GIT_TREE_GRAPH_CELL : table<int; EmptyMarkerState>
var private GIT_PR_COMMIT_HOVER : table<int; EmptyMarkerState>
var private GIT_HISTORY_COMMIT_ROW : table<int; ClickState>
var private GIT_COMMIT_FILE_ROW : table<int; ToggleState>
var private REPOSITORY_ERROR : table<int; NarrativeState>
var private REPOSITORY_TITLE : table<int; NarrativeState>
var private REPOSITORY_EMPTY : table<int; NarrativeState>
var private GIT_GROUP_TITLE : table<int; NarrativeState>
var private REPOSITORY_SEPARATOR : table<int; EmptyMarkerState>
var private WORKTREE_SEPARATOR : table<int; EmptyMarkerState>
var private GIT_GROUP_SEPARATOR : table<int; EmptyMarkerState>
var private GIT_GROUP_SPACING : table<int; EmptyMarkerState>
var private GIT_HOVER_TOOLTIP : NarrativeState
var private FOCUS_HINT_TOOLTIP : NarrativeState
var private GIT_CHANGELIST_STATUS_TEXT : NarrativeState
var private GIT_ACTIVITY_STATUS_TEXT : NarrativeState
var private GIT_CHANGELIST_STATUS_SEPARATOR : EmptyMarkerState
var private GIT_ACTIVITY_STATUS_SEPARATOR : EmptyMarkerState
var private INSPECTOR_STATUS_TEXT : NarrativeState
var private INSPECTOR_STATUS_SEPARATOR : EmptyMarkerState
var private REPOSITORY_ERROR_POPUP : table<int; PopupContextItemState>
var private REPOSITORY_ERROR_COPY : table<int; ToggleState>
var private REPOSITORY_ERROR_SEND : table<int; ToggleState>
var private CHANGELIST_ERROR_POPUP : PopupContextItemState
var private CHANGELIST_ERROR_COPY : ToggleState
var private CHANGELIST_ERROR_SEND : ToggleState
var private ACTIVITY_ERROR_POPUP : PopupContextItemState
var private ACTIVITY_ERROR_COPY : ToggleState
var private ACTIVITY_ERROR_SEND : ToggleState
var private INSPECTOR_ERROR_POPUP : PopupContextItemState
var private INSPECTOR_ERROR_COPY : ToggleState
var private INSPECTOR_ERROR_SEND : ToggleState
var private CLIENT_ERROR_POPUP : PopupContextItemState
var private CLIENT_ERROR_COPY : ToggleState
var private CLIENT_ERROR_SEND : ToggleState
var private PREVIEW_ERROR_POPUP : PopupContextItemState
var private PREVIEW_ERROR_COPY : ToggleState
var private PREVIEW_ERROR_SEND : ToggleState

def private setup_default_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let viewport = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, viewport.Size)
    var left_id : uint = 0u
    var right_id : uint = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.24f, left_id, right_id)
    var upper_id : uint = 0u
    var terminal_id : uint = 0u
    DockBuilderSplitNode(right_id, ImGuiDir.Down, 0.34f, terminal_id, upper_id)
    var git_id : uint = 0u
    var inspector_id : uint = 0u
    DockBuilderSplitNode(upper_id, ImGuiDir.Left, 0.34f, git_id, inspector_id)
    var changelist_id : uint = 0u
    var activity_id : uint = 0u
    DockBuilderSplitNode(git_id, ImGuiDir.Up, 0.35f, changelist_id, activity_id)
    var repositories_id : uint = 0u
    var sessions_id : uint = 0u
    DockBuilderSplitNode(left_id, ImGuiDir.Down, 0.42f,
        sessions_id, repositories_id)
    DockBuilderDockWindow("Sessions & Activity###SessionsActivity", sessions_id)
    DockBuilderDockWindow("Repositories & Worktrees", repositories_id)
    DockBuilderDockWindow("Git Changelist", changelist_id)
    DockBuilderDockWindow("Git Activity", activity_id)
    DockBuilderDockWindow("File Inspector", inspector_id)
    DockBuilderDockWindow("Terminal", terminal_id)
    DockBuilderDockWindow("Settings", activity_id)
    DockBuilderDockWindow("Session Launcher", activity_id)
    DockBuilderFinish(dock_id)
}

def private option_value(prefix : string; fallback : string = "") : string {
    for (argument in get_command_line_arguments()) {
        if (starts_with(argument, prefix)) return slice(argument, length(prefix))
    }
    return fallback
}

def private reset_terminal(columns : int = 100; rows : int = 30) {
    terminal_dispose(g_terminal)
    terminal_viewport_snapshot_dispose(g_terminal_cache.snapshot)
    var inscope created <- terminal_create(columns, rows)
    g_terminal := created
    g_terminal_view = ImGuiTerminalState()
    g_terminal_cache = ImGuiTerminalCache()
}

def private feed_terminal_frame(message : string#; byte_count : int) {
    var inscope bytes : array<uint8>
    bytes |> resize(byte_count)
    if (byte_count > 0) {
        unsafe { memcpy(addr(bytes[0]), reinterpret<void?>(message), byte_count) }
        terminal_feed_bytes(g_terminal, bytes)
    }
}

def private upload_inspector_preview(event : InspectorPrepareEvent#) {
    g_inspector.preview_width = event.preview_width
    g_inspector.preview_height = event.preview_height
    g_inspector.preview_byte_count = event.preview_byte_count
    g_inspector.preview_error = clone_string(event.preview_error)
    if (empty(event.preview_pixels)) return
    inspector_preview_texture_upload(event.preview_pixels,
        event.preview_width, event.preview_height,
        g_inspector.preview_texture)
}

def private release_inspector_content() {
    if (g_inspector.preview_texture != 0u) {
        inspector_preview_texture_release(g_inspector.preview_texture)
    }
    git_parsed_patch_dispose(g_inspector.parsed)
    text_source_view_invalidate(g_inspector.old_view)
    text_source_view_invalidate(g_inspector.new_view)
    text_source_view_invalidate(g_inspector.view)
    text_source_document_dispose(g_inspector.old_document)
    text_source_document_dispose(g_inspector.new_document)
    text_source_document_dispose(g_inspector.view_document)
    delete g_inspector.new_line_starts
    delete g_inspector.view_line_starts
    g_inspector.focus_document_valid = false
    g_inspector.focus_start_byte = -1
    markdown_view_dispose(g_inspector.markdown_view)
    rich_document_dispose(g_inspector.markdown_document)
    delete g_inspector.changed_source_ranges
    g_inspector.view_source = ""
    g_inspector.view_prepared = false
    g_inspector.markdown_gutter_segments = 0
    g_inspector.preview_width = 0
    g_inspector.preview_height = 0
    g_inspector.preview_byte_count = 0
    g_inspector.preview_error = ""
    g_inspector.diff_scroll_y = 0.0f
    g_focus_view_scroll_y = 0.0f
    g_inspector.diff_scroll_jump_pending = false
    g_inspector.diff_current_hunk = 0
    g_inspector.diff_left_rect = float4()
    g_inspector.diff_right_rect = float4()
    g_inspector.diff_scrollbar_rect = float4()
}

def private queue_inspector_prepare(status : string) {
    let choice = clamp(g_inspector.syntax_choice, 0,
        length(INSPECTOR_SYNTAX_VALUES) - 1)
    g_inspector.prepare_language = inspector_language_for_path(
        g_inspector.file_path, INSPECTOR_SYNTAX_VALUES[choice])
    let generation = inspector_load_queue(g_inspector.prepare, status)
    g_inspector.error = ""
    if (!inspector_prepare_worker_queue(g_prepare_worker, generation,
            g_inspector.source_patch, g_inspector.source_view_text,
            g_inspector.prepare_language, g_inspector.source_view_base64,
            g_inspector.source_view_byte_count,
            g_inspector.source_view_available)) {
        let failure = "File inspection preparation worker is unavailable"
        inspector_load_fail(g_inspector.prepare, generation,
            failure)
        g_inspector.error = failure
    }
}

def private install_file_inspection(message : WatcherFileInspectionMessage const) {
    if (message.request_id != g_inspector.request_id) return
    g_inspector.task_session_id = message.task_session_id
    g_inspector.loading = message.loading
    g_inspector.status = message.status
    if (message.loading) return
    g_inspector.error = message.error
    if (!empty(message.error)) {
        inspector_load_fail(g_inspector.prepare,
            g_inspector.prepare.generation, message.error)
        return
    }
    g_inspector.source_patch = clone_string(message.patch)
    g_inspector.source_view_text = clone_string(message.view_text)
    g_inspector.source_view_base64 = clone_string(message.view_base64)
    g_inspector.source_view_byte_count = message.view_byte_count
    g_inspector.source_view_available = message.view_available
    queue_inspector_prepare("Preparing diff...")
}

def private drain_inspector_prepare_events() {
    while (true) {
        var event : InspectorPrepareEvent
        if (!inspector_prepare_worker_take_event(g_prepare_worker, event)) break
        if (event.kind == InspectorPrepareEventKind.started) {
            inspector_load_start(g_inspector.prepare, event.generation,
                "Preparing diff...")
        } elif (event.kind == InspectorPrepareEventKind.failed) {
            if (inspector_load_fail(g_inspector.prepare,
                    event.generation, clone_string(event.error))) {
                g_inspector.error = clone_string(event.error)
                g_inspector.prepare_elapsed_usec = event.elapsed_usec
            }
        } elif (event.kind == InspectorPrepareEventKind.completed) {
            if (!inspector_load_succeed(g_inspector.prepare,
                    event.generation)) continue
            release_inspector_content()
            g_inspector.revision = event.generation
            g_inspector.parsed <- event.parsed
            g_inspector.old_document <- event.old_document
            g_inspector.new_document <- event.new_document
            g_inspector.view_document <- event.view_document
            append_line_starts(g_inspector.new_document.source,
                g_inspector.new_line_starts)
            append_line_starts(g_inspector.view_document.source,
                g_inspector.view_line_starts)
            g_inspector.markdown_document <- event.markdown_document
            g_inspector.markdown_view <- event.markdown_view
            g_inspector.changed_source_ranges <- event.changed_source_ranges
            g_inspector.view_source = clone_string(event.view_source)
            g_inspector.view_prepared = event.view_text_ready
            g_inspector.installed_repository_id = g_inspector.repository_id
            g_inspector.installed_worktree_path = g_inspector.worktree_path
            g_inspector.installed_file_path = g_inspector.file_path
            g_inspector.installed_comparison = g_inspector.comparison
            g_inspector.installed_git_revision = g_inspector.git_revision
            g_inspector.installed_diff_context = g_inspector.diff_context
            g_inspector.installed_diff_context_choice = g_inspector.diff_context_choice
            g_inspector.installed_syntax_choice = g_inspector.syntax_choice
            g_inspector.resolved_language = g_inspector.prepare_language
            DIFF_CONTEXT.value = g_inspector.installed_diff_context_choice
            DIFF_SYNTAX.value = g_inspector.installed_syntax_choice
            if (g_inspector.pending_select_diff) {
                g_inspector.mode = GitInspectorMode.diff
                INSPECTOR_DIFF_TAB.pending_select = true
            }
            g_inspector.pending_select_diff = false
            upload_inspector_preview(
                unsafe(reinterpret<InspectorPrepareEvent const#>(event)))
            g_inspector.error = ""
            g_inspector.prepare_elapsed_usec = event.elapsed_usec
        }
    }
}

def private begin_file_inspection(repository_id, worktree_path, file_path,
                                  comparison, diff_context : string;
                                  git_revision : string = "") {
    let same_identity = g_inspector.repository_id == repository_id && g_inspector.worktree_path == worktree_path && g_inspector.file_path == file_path && g_inspector.comparison == comparison && g_inspector.git_revision == git_revision
    let retained_content = g_inspector.prepare.has_retained_content
    g_inspector.request_id++
    g_inspector.repository_id = repository_id
    g_inspector.worktree_path = worktree_path
    g_inspector.file_path = file_path
    g_inspector.comparison = comparison
    g_inspector.git_revision = git_revision
    g_inspector.diff_context = diff_context
    g_inspector.diff_context_choice = 0
    if (diff_context == "expanded") {
        g_inspector.diff_context_choice = 1
    } elif (diff_context == "full") {
        g_inspector.diff_context_choice = 2
    }
    g_inspector.task_session_id = ""
    g_inspector.loading = true
    g_inspector.status = "Requesting diff..."
    g_inspector.error = ""
    if (!same_identity) {
        g_inspector.syntax_choice = 0
        g_inspector.prepare_language = inspector_language_for_path(file_path)
    }
    g_inspector.pending_select_diff = !same_identity
    if (retained_content) {
        inspector_load_supersede(g_inspector.prepare, "Requesting diff...")
    } else {
        release_inspector_content()
        inspector_load_reset(g_inspector.prepare)
        g_inspector.mode = GitInspectorMode.diff
        INSPECTOR_DIFF_TAB.pending_select = true
    }
}

class private WatcherRichClient : HvWebSocketClient {
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
                if (!attached && !empty(selected_id)) {
                    attach_session(selected_id)
                } elif (empty(selected_id)) {
                    for (session in sessions) {
                        if (session.state == "running") {
                            attach_session(session.id)
                            break
                        }
                    }
                }
            }
        } elif (envelope._type == "launched") {
            var incoming : WatcherSessionMessage
            if (sscan_json(body, incoming)) {
                attach_session(incoming.session_id)
                TERMINAL_WIN.open = true
                TERMINAL_WIN.pending_raise = true
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
                        TERMINAL_WIN.open = true
                        TERMINAL_WIN.pending_raise = true
                        g_focus_terminal_pending = true
                    }
                    if (git_failure_baseline_ready) {
                        error = repository.failure_message
                    }
                }
                git_failure_baseline_ready = true
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
                    TERMINAL_WIN.open = true
                    TERMINAL_WIN.pending_raise = true
                    g_focus_terminal_pending = true
                }
            }
        } elif (envelope._type == "attached") {
            var incoming : WatcherAttachedMessage
            if (sscan_json(body, incoming)) {
                attached_id = incoming.session_id
                attached = true
                controls = incoming.controls
                g_status = incoming.controls ? "Attached with control" : "Attached read-only"
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
            sent_columns = 0
            sent_rows = 0
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

    def attach_session(session_id : string) {
        if (!connected || empty(session_id)) return
        var columns = 100
        var rows = 30
        for (session in sessions) {
            if (session.id == session_id) {
                columns = max(20, session.columns)
                rows = max(5, session.rows)
                break
            }
        }
        selected_id = session_id
        attached = false
        controls = false
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
        var argv <- ["codex"]
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
        send("\{\"op\":\"repository_review\"," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}\}")
    }

    def request_repository_history(repository_id : string;
                                   worktree_path : string;
                                   ref_name : string = "") {
        if (!connected || empty(repository_id) || empty(worktree_path)) return
        last_history_identity = "{repository_id}\n{worktree_path}\n{ref_name}"
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
        send("\{\"op\":\"repository_commit_files\"," +
            "\"repository_id\":{sprint_json(repository_id, false)}," +
            "\"worktree_path\":{sprint_json(worktree_path, false)}," +
            "\"revision\":{sprint_json(commit_hash, false)}\}")
    }

    def request_repository_refs(repository_id : string; worktree_path : string) {
        if (!connected || empty(repository_id) || empty(worktree_path)) return
        last_refs_identity = "{repository_id}\n{worktree_path}"
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
            send("\{\"op\":\"heartbeat\"\}")
            last_heartbeat = ref_time_ticks()
        }
        if (get_time_nsec(last_list) / 1000000l >= 1000l) {
            refresh()
            last_list = ref_time_ticks()
        }
    }
}

var private g_client : WatcherRichClient?

def private zoom_by(delta : int) {
    g_zoom_percent = clamp(g_zoom_percent + delta, 50, 300)
}

def private selected_session_label() : string {
    if (g_client == null || empty(g_client.selected_id)) return "No session selected"
    for (session in g_client.sessions) {
        if (session.id == g_client.selected_id) {
            return "{session.state}  pid {session.pid}  {session.command_line}"
        }
    }
    return g_client.selected_id
}

def private draw_selected_session_context() {
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

def private select_bundle_participant(participant : WatcherBundleParticipant const) {
    if (g_client == null) return
    g_selected_repository_id = participant.repository_id
    g_selected_worktree_path = participant.worktree_path
    g_client->refresh_repository(participant.repository_id)
    g_client->request_repository_review(participant.repository_id,
        participant.worktree_path)
    GIT_CHANGELIST_WIN.open = true
    GIT_CHANGELIST_WIN.pending_raise = true
    GIT_ACTIVITY_WIN.open = true
}

def private draw_review_bundles() {
    text("Review bundles ({g_client != null ? length(g_client.bundles) : 0})")
    child(REVIEW_BUNDLES_CHILD, (text = "review bundles", size = float2(0.0f, 150.0f),
            child_flags = ImGuiChildFlags.Borders, window_flags = ImGuiWindowFlags.None)) {
        if (g_client == null || empty(g_client.bundles)) {
            text_disabled("The agent has not declared a review bundle yet.")
            return
        }
        var row = 0
        for (bundle in g_client.bundles) {
            text("[{to_upper(bundle.status)}] {bundle.title}")
            if (!empty(bundle.summary)) text_disabled((text = bundle.summary))
            for (participant in bundle.participants) {
                let files = length(participant.files)
                BUNDLE_PARTICIPANT_ROW[row].value = (participant.repository_id
                    == g_selected_repository_id && participant.worktree_path
                    == g_selected_worktree_path)
                if (selectable(BUNDLE_PARTICIPANT_ROW[row],
                        (text = "  {participant.role}  {participant.worktree_path}  ({files} files)##{bundle.id}:{row}"))) {
                    select_bundle_participant(participant)
                }
                row++
            }
        }
    }
}

def private pending_attention_count() : int {
    var count = 0
    if (g_client != null) {
        for (message in g_client.mailbox) {
            if (message.direction == "outbox" && message.status == "new") count++
        }
    }
    return count
}

def private first_focus_range(target : WatcherFocusTarget const) : int {
    var result = -1
    for (index in range(length(target.ranges))) {
        if (result < 0 || target.ranges[index].start_byte
                < target.ranges[result].start_byte) {
            result = index
        }
    }
    return result
}

def private activate_focus_target(message : WatcherMailboxMessage const;
                                  target_index : int) {
    if (g_client == null || target_index < 0
            || target_index >= length(message.focus.targets)) return
    let target & = unsafe(message.focus.targets[target_index])
    let repository_id = watcher_focus_repository(message.focus, target)
    let worktree_path = watcher_focus_worktree(message.focus, target)
    let comparison = watcher_focus_comparison(message.focus, target)
    let revision = watcher_focus_revision(message.focus, target)
    g_active_focus_message = message.id
    g_active_focus_target = target_index
    g_active_focus_range = first_focus_range(target)
    g_focus_visible = true
    g_pending_focus_scroll = true
    g_applied_focus_identity = ""
    let selection_changed = (g_selected_repository_id
            != repository_id
        || g_selected_worktree_path != worktree_path)
    g_selected_repository_id = repository_id
    g_selected_worktree_path = worktree_path
    if (message.status == "new") g_client->acknowledge_mailbox(message.id)
    if (selection_changed) {
        g_client->refresh_repository(repository_id)
        g_client->request_repository_review(repository_id, worktree_path)
        g_client->request_repository_history(repository_id, worktree_path)
        g_client->request_repository_refs(repository_id, worktree_path)
    }
    g_client->inspect_file(repository_id, worktree_path, target.file_path,
        comparison, "full", revision)
    FILE_INSPECTOR_WIN.open = true
    FILE_INSPECTOR_WIN.pending_raise = true
}

def private draw_error_context(var popup_state : PopupContextItemState;
                               var copy_state : ToggleState;
                               var send_state : ToggleState;
                               popup_id, context, message : string) {
    popup_context_item(popup_state,
            (str_id = popup_id, flags = ImGuiPopupFlags.MouseButtonRight)) {
        if (menu_item(copy_state, (text = "Copy error"))) {
            let copied = clipboard_set_text(message)
            g_status = (copied == ClipboardStatus.ok
                ? "Error copied" : "Clipboard unavailable")
        }
        if (menu_item(send_state, (text = "Send to session agent",
                enabled = g_client != null && g_client.connected
                    && !empty(g_client.selected_id)))) {
            g_client->post_error_request(context, message)
            g_status = "Error sent to session agent"
        }
    }
}

def private draw_session_list() {
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
                if (!SHOW_EXITED.value && session.state == "exited") continue
                visible_sessions++
                let row_text = "[{session.state}] {session.command_line}##{session.id}"
                if (selectable(SESSION_ROW[index], (text = row_text))) {
                    g_client->attach_session(session.id)
                    TERMINAL_WIN.open = true
                    TERMINAL_WIN.pending_raise = true
                    g_focus_terminal_pending = true
                }
                SESSION_ROW[index].value = session.id == g_client.selected_id
            }
        }
        if (visible_sessions == 0) {
            text_disabled(NO_SESSION_LABEL,
                (text = SHOW_EXITED.value ? "No sessions" : "No active sessions"))
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

def private draw_terminal_footer() {
    if (small_button(TERMINAL_ZOOM_OUT_BUTTON, (text = "-5%"))) zoom_by(-5)
    same_line()
    if (small_button(TERMINAL_ZOOM_IN_BUTTON, (text = "+5%"))) zoom_by(5)
    same_line()
    text("Zoom {g_zoom_percent}%")
    same_line()
    text_disabled("{g_terminal_view.scrollback_rows} history lines - {g_status}")
    if (g_client != null) {
        same_line()
        text_disabled("input sent {g_client.sent_input_bytes} bytes")
    }
}

def private copy_terminal_selection() {
    g_status = (imgui_terminal_copy_selection(g_terminal, g_terminal_view)
        ? "Selection copied" : "Nothing selected")
}

def private paste_terminal_clipboard() {
    var pasted = ""
    if (clipboard_get_text(pasted) == ClipboardStatus.ok && g_client != null) {
        g_client->send_input(terminal_encode_paste(g_terminal, pasted))
        g_status = "Clipboard pasted"
    } else {
        g_status = "Clipboard unavailable"
    }
}

def private open_dock_window(var state : DockWindowState) {
    state.open = true
    state.pending_raise = true
}

def private reset_layout() {
    REPOSITORIES_WIN.open = true
    SESSIONS_WIN.open = true
    GIT_CHANGELIST_WIN.open = true
    GIT_ACTIVITY_WIN.open = true
    FILE_INSPECTOR_WIN.open = true
    TERMINAL_WIN.open = true
    SESSION_LAUNCHER_WIN.open = false
    SETTINGS_WIN.open = false
    DOCK_ROOT.pending_reset = true
}

def private prepare_debug_launcher() {
    let suffix = "debug-{ref_time_ticks()}"
    AGENT_BRANCH.pending_value = "codex/{suffix}"
    AGENT_BRANCH.has_pending = true
    AGENT_BASE_REF.pending_value = "HEAD"
    AGENT_BASE_REF.has_pending = true
    var checkout = g_selected_worktree_path
    if (g_client != null) {
        for (repository in g_client.repositories) {
            if (repository.record.id == g_selected_repository_id) {
                checkout = repository.record.checkout_path
                break
            }
        }
    }
    AGENT_WORKTREE_PATH.pending_value = path_join(
        path_join(checkout, ".codex/worktrees"), suffix)
    AGENT_WORKTREE_PATH.has_pending = true
    CREATE_DEDICATED_WORKTREE.value = true
}

def private draw_main_menu() {
    main_menu_bar(MAIN_MENU) {
        menu(FILE_MENU, (text = "File", enabled = true)) {
            if (menu_label(FILE_EXIT, (text = "Exit", shortcut = "Alt+F4"))) {
                request_exit()
            }
        }
        menu(COMMANDS_MENU, (text = "Commands", enabled = true)) {
            if (menu_label(COMMAND_CODEX_DEBUG,
                    (text = "Debug with Codex...", enabled = g_client != null
                        && g_client.connected && !empty(g_selected_worktree_path)))) {
                prepare_debug_launcher()
                open_dock_window(SESSION_LAUNCHER_WIN)
            }
            if (menu_label(COMMAND_POWERSHELL,
                    (text = "PowerShell", enabled = g_client != null && g_client.connected))) {
                g_client->launch("powershell.exe -NoLogo -NoProfile", g_selected_worktree_path,
                    g_selected_repository_id, g_selected_worktree_path)
            }
        }
        menu(EDIT_MENU, (text = "Edit", enabled = true)) {
            if (menu_label(EDIT_COPY, (text = "Copy", shortcut = "Ctrl+C",
                    enabled = g_terminal_view.selection_active))) copy_terminal_selection()
            if (menu_label(EDIT_PASTE, (text = "Paste", shortcut = "Ctrl+V"))) {
                paste_terminal_clipboard()
            }
            separator(EDIT_SELECTION_SEPARATOR)
            if (menu_label(EDIT_SELECT_ALL, (text = "Select all"))) {
                imgui_terminal_select_all(g_terminal, g_terminal_view)
            }
            if (menu_label(EDIT_CLEAR_SELECTION,
                    (text = "Clear selection", enabled = g_terminal_view.selection_active))) {
                imgui_terminal_clear_selection(g_terminal_view)
            }
        }
        menu(VIEW_MENU, (text = "View", enabled = true)) {
            if (menu_label(VIEW_REPOSITORIES,
                    (text = "Repositories & Worktrees", selected = REPOSITORIES_WIN.open))) {
                open_dock_window(REPOSITORIES_WIN)
            }
            if (menu_label(VIEW_SESSIONS,
                    (text = pending_attention_count() > 0
                        ? "Sessions & Activity  [{pending_attention_count()}]"
                        : "Sessions & Activity", selected = SESSIONS_WIN.open))) {
                open_dock_window(SESSIONS_WIN)
            }
            if (menu_label(VIEW_GIT_CHANGELIST,
                    (text = "Git Changelist", selected = GIT_CHANGELIST_WIN.open))) {
                open_dock_window(GIT_CHANGELIST_WIN)
            }
            if (menu_label(VIEW_GIT_ACTIVITY,
                    (text = "Git Activity", selected = GIT_ACTIVITY_WIN.open))) {
                open_dock_window(GIT_ACTIVITY_WIN)
            }
            if (menu_label(VIEW_FILE_INSPECTOR,
                    (text = "File Inspector", selected = FILE_INSPECTOR_WIN.open))) {
                open_dock_window(FILE_INSPECTOR_WIN)
            }
            if (menu_label(VIEW_TERMINAL,
                    (text = "Terminal", selected = TERMINAL_WIN.open))) {
                open_dock_window(TERMINAL_WIN)
            }
            if (menu_label(VIEW_SESSION_LAUNCHER,
                    (text = "Session Launcher", selected = SESSION_LAUNCHER_WIN.open))) {
                open_dock_window(SESSION_LAUNCHER_WIN)
            }
            separator(VIEW_WINDOWS_SEPARATOR)
            menu_item(SHOW_EXITED, (text = "Show exited sessions"))
            separator(VIEW_ZOOM_SEPARATOR)
            if (menu_label(VIEW_ZOOM_IN, (text = "Zoom in", shortcut = "Ctrl++"))) zoom_by(5)
            if (menu_label(VIEW_ZOOM_OUT, (text = "Zoom out", shortcut = "Ctrl+-"))) zoom_by(-5)
            if (menu_label(VIEW_ZOOM_RESET, (text = "Reset zoom", shortcut = "Ctrl+0"))) {
                g_zoom_percent = 100
            }
            if (menu_label(VIEW_SCROLL_BOTTOM, (text = "Scroll terminal to bottom"))) {
                g_terminal_view.scroll_offset = 0
            }
            separator(VIEW_LAYOUT_SEPARATOR)
            if (menu_label(VIEW_RESET_LAYOUT, (text = "Reset layout"))) reset_layout()
        }
        menu(SETTINGS_MENU, (text = "Settings", enabled = true)) {
            if (menu_label(OPEN_SETTINGS, (text = "Open Settings"))) {
                open_dock_window(SETTINGS_WIN)
            }
        }
    }
}

def private draw_repositories_window() {
    text_disabled("Target: Local")
    separator()
    input_text_growable(ADD_REPOSITORY_PATH, (text = "Repository checkout"))
    same_line()
    if (button(ADD_REPOSITORY, (text = "Add")) && g_client != null) {
        g_client->add_repository(ADD_REPOSITORY_PATH.value)
    }
    separator()
    var row = 0
    var repository_row = 0
    if (g_client != null) {
        for (repository in g_client.repositories) {
            let refreshing = repository.refreshing ? "  refreshing..." : ""
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
                let row_label = "{prefix}{branch}{dirty_label}##{repository.record.id}:{worktree.path}"
                if (selectable(WORKTREE_ROW[row], (text = row_label))) {
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

def private git_status_byte(code : string; index : int) : int {
    if (index < 0 || index >= length(code)) return ' '
    var result = ' '
    peek_data(code) $(data) {
        result = int(data[index])
    }
    return result
}

def private git_conflict(code : string) : bool {
    return (code == "DD" || code == "AU" || code == "UD" || code == "UA"
        || code == "DU" || code == "AA" || code == "UU")
}

def private git_file_in_group(file : GitFileState const; group : int) : bool {
    if (group == 0) return git_conflict(file.code)
    if (git_conflict(file.code)) return false
    if (group == 1) return file.code != "??" && git_status_byte(file.code, 0) != ' '
    if (group == 2) return file.code != "??" && git_status_byte(file.code, 1) != ' '
    return file.code == "??"
}

def private git_group_scope(group : int) : string {
    return group == 1 ? "staged" : "working"
}

def private agent_marks_file(repository_id, worktree_path,
                             file_path : string) : bool {
    return (g_client != null
        && g_client->marks_file_for_agent(repository_id, worktree_path, file_path))
}

def private focused_file_label(repository_id, worktree_path,
                               file_path : string) : string {
    return (agent_marks_file(repository_id, worktree_path, file_path)
        ? "[AGENT] {file_path}" : file_path)
}

def private draw_git_file_group(repository : RepositorySnapshot const;
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

def private git_status_file_code(repository : RepositorySnapshot const;
                                 worktree_path : string;
                                 file_path : string) : string {
    for (file in repository.files) {
        if (file.worktree_path == worktree_path && file.path == file_path) {
            return file.code
        }
    }
    return ""
}

def private git_line_stats(added_lines, deleted_lines : int;
                           stats_known, binary : bool) : string {
    if (binary) return "binary"
    if (!stats_known) return "+?  -?"
    return "+{added_lines}  -{deleted_lines}"
}

def private service_sha_copy() {
    if (empty(g_pending_clipboard_sha)) return
    if (g_clipboard_copy_attempts >= 8) {
        g_pending_clipboard_sha = ""
        return
    }
    g_clipboard_copy_attempts++
    g_clipboard_copy_status = clipboard_set_text(g_pending_clipboard_sha)
    if (g_clipboard_copy_status != ClipboardStatus.ok) return
    var copied_text = ""
    g_clipboard_copy_status = clipboard_get_text(copied_text)
    if (g_clipboard_copy_status == ClipboardStatus.ok
            && copied_text == g_pending_clipboard_sha) {
        g_last_copied_sha = g_pending_clipboard_sha
        g_pending_clipboard_sha = ""
        g_clipboard_copy_verified = true
    }
}

def private queue_sha_copy(hash : string) {
    g_pending_clipboard_sha = hash
    g_clipboard_copy_attempts = 0
    g_clipboard_copy_verified = false
    service_sha_copy()
}

def private draw_git_path_text(path : string; p0, p1 : float2;
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

def private draw_git_stats_text(position : float2;
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

def private git_visible_row_right(row_right : float) : float {
    let style & = GetStyle()
    let window_right = GetWindowPos().x + GetWindowSize().x - style.WindowPadding.x - style.ScrollbarSize
    return min(row_right, window_right)
}

def private decorate_git_file_row(row_id, path : string;
                                  added_lines, deleted_lines : int;
                                  stats_known, binary, subdued : bool;
                                  action_glyph, action_tip : string;
                                  var action_clicked : bool&) {
    let stats = git_line_stats(added_lines, deleted_lines, stats_known, binary)
    let row_min = GetItemRectMin()
    let row_max = GetItemRectMax()
    let row_right = git_visible_row_right(row_max.x)
    let hovered = IsItemHovered()
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

def private draw_pr_included_files(repository : RepositorySnapshot const;
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

def private draw_pr_not_included_files(repository : RepositorySnapshot const;
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
            g_client->inspect_file(repository.record.id, worktree.path,
                file.path, "working")
            FILE_INSPECTOR_WIN.open = true
            FILE_INSPECTOR_WIN.pending_raise = true
        }
        file_row++
    }
    return count
}

def private draw_shared_pr_files(repository : RepositorySnapshot const;
                                 worktree : WorktreeState const) {
    var file_row = 0
    let not_included = draw_pr_not_included_files(
        repository, worktree, file_row)
    if (not_included > 0) {
        separator(GIT_GROUP_SEPARATOR[11])
    }
    draw_pr_included_files(repository, worktree, file_row)
}

def private draw_sha_copy_icon(hash, row_id : string;
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
    let hovered = IsItemHovered() && (empty(visible_sha)
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

def private draw_pr_review(repository : RepositorySnapshot const;
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

def private ensure_history(repository : RepositorySnapshot const;
                           worktree : WorktreeState const; ref_name : string) {
    if (g_client == null || empty(ref_name)) return
    let identity = "{repository.record.id}\n{worktree.path}\n{ref_name}"
    if (g_client.last_history_identity != identity) {
        g_client->request_repository_history(
            repository.record.id, worktree.path, ref_name)
    }
}

def private request_commit_files(repository : RepositorySnapshot const;
                                 worktree : WorktreeState const;
                                 commit_hash : string) {
    if (g_client == null || !g_client.connected || empty(commit_hash)) return
    g_requested_history_commit = commit_hash
    g_client->request_repository_commit_files(
        repository.record.id, worktree.path, commit_hash)
}

def private draw_commit_files(repository : RepositorySnapshot const;
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
            g_client->inspect_file(repository.record.id, worktree.path,
                file.path, "commit", "compact", file.commit_hash)
            FILE_INSPECTOR_WIN.open = true
            FILE_INSPECTOR_WIN.pending_raise = true
        }
        row++
    }
}

def private draw_history_commits(repository : RepositorySnapshot const;
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
                    request_commit_files(repository, worktree, commit.hash)
                }
            }
        }
    }
}

def private draw_history_review(repository : RepositorySnapshot const;
                                worktree : WorktreeState const) {
    ensure_history(repository, worktree, worktree.branch)
    draw_history_commits(repository, worktree)
}

def private git_graph_color(slot : int; alpha : float = 1.0f) : uint {
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

def private current_tree_filter() : GitTreeFilter {
    if (GIT_TREE_FILTER.value == 1) return GitTreeFilter.all
    return GitTreeFilter.auto_
}

def private ensure_tree_model(repository : RepositorySnapshot const;
                              worktree : WorktreeState const) {
    let filter = current_tree_filter()
    let identity = "{repository.record.id}\n{worktree.path}\n{repository.revision}\n{int(filter)}\n{g_tree_focus_ref}"
    if (identity == g_tree_model_identity) return
    git_tree_build_model(repository, worktree, filter,
        g_tree_focus_ref, g_tree_model)
    g_tree_model_identity = identity
}

def private draw_git_tree_vectors(anchor_x, row_zero_y, row_height,
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

def private draw_tree_commits(repository : RepositorySnapshot const;
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

def private draw_tree_review(repository : RepositorySnapshot const;
                             worktree : WorktreeState const) {
    if (g_client != null) {
        let identity = "{repository.record.id}\n{worktree.path}"
        if (g_client.last_refs_identity != identity) {
            g_client->request_repository_refs(repository.record.id, worktree.path)
        }
    }
    ensure_history(repository, worktree, "__all__")
    SetNextItemWidth(130.0f)
    combo(GIT_TREE_FILTER, (text = "Branches", items <- TREE_FILTER_LABELS))
    same_line()
    ensure_tree_model(repository, worktree)
    text_disabled(GIT_TREE_FOCUS_TEXT,
        (text = "Focus: {g_tree_model.focus_ref}   refs {g_tree_model.included_ref_count}"))
    draw_tree_commits(repository, worktree)
}

def private git_review_commit_count(repository : RepositorySnapshot const;
                                    worktree : WorktreeState const) : int {
    var count = 0
    for (commit in repository.review_commits) {
        if (commit.worktree_path == worktree.path) count++
    }
    return count
}

def private git_history_commit_count(repository : RepositorySnapshot const;
                                     worktree : WorktreeState const) : int {
    var count = 0
    for (commit in repository.history_commits) {
        if (commit.worktree_path == worktree.path) count++
    }
    return count
}

def private git_selected_commit_file_count(repository : RepositorySnapshot const;
                                           worktree : WorktreeState const;
                                           commit_hash : string) : int {
    var count = 0
    for (file in repository.commit_files) {
        if (file.worktree_path == worktree.path
                && file.commit_hash == commit_hash) count++
    }
    return count
}

def private git_activity_status(repository : RepositorySnapshot const;
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

def private git_changelist_status(repository : RepositorySnapshot const;
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

def private git_status_color(status : GitWindowStatus const) : float4 {
    return (status.error
        ? float4(1.0f, 0.35f, 0.30f, 1.0f)
        : (status.busy
            ? float4(0.56f, 0.59f, 0.64f, 1.0f)
            : float4(0.69f, 0.73f, 0.80f, 1.0f)))
}

def private draw_git_changelist_window() {
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
                    if (small_button(REFRESH_GIT, (text = "Refresh",
                        enabled = !repository.refreshing))) {
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
    text_colored(GIT_CHANGELIST_STATUS_TEXT,
        (color = git_status_color(status), text = status.text))
    if (status.error) {
        draw_error_context(CHANGELIST_ERROR_POPUP, CHANGELIST_ERROR_COPY,
            CHANGELIST_ERROR_SEND, "changelist_error", "Git Changelist",
            status.text)
    }
}

def private draw_git_activity_window() {
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
                        tab_item(GIT_PR_TAB, (text = "PR", closable = false,
                            flags = ImGuiTabItemFlags.None)) {
                            draw_pr_review(repository, worktree)
                        }
                        tab_item(GIT_HISTORY_TAB, (text = "History", closable = false,
                            flags = ImGuiTabItemFlags.None)) {
                            draw_history_review(repository, worktree)
                        }
                        tab_item(GIT_TREE_TAB, (text = "Tree", closable = false,
                            flags = ImGuiTabItemFlags.None)) {
                            draw_tree_review(repository, worktree)
                        }
                    }
                    status = git_activity_status(repository, worktree)
                }
            }
        }
        if (!found) text_disabled("Select a worktree to inspect Git activity")
    }
    separator(GIT_ACTIVITY_STATUS_SEPARATOR)
    text_colored(GIT_ACTIVITY_STATUS_TEXT,
        (color = git_status_color(status), text = status.text))
    if (status.error) {
        draw_error_context(ACTIVITY_ERROR_POPUP, ACTIVITY_ERROR_COPY,
            ACTIVITY_ERROR_SEND, "activity_error", "Git Activity",
            status.text)
    }
}

def private source_view_style(diff_background : bool = false) : TextSourceViewStyle {
    return TextSourceViewStyle(
        fonts = g_fonts,
        zoom = float(g_zoom_percent) / 100.0f,
        wrap = false,
        line_mark_background_alpha = diff_background ? 0.11f : 0.0f,
        line_mark_width = 2.0f,
        write_system_clipboard = !harness_is_headless())
}

def private syntax_cell_count(state : TextSourceViewState const) : int {
    var count = 0
    for (cell in state.cells) {
        if (cell.style_id > 1) count++
    }
    return count
}

def private syntax_style_cell_count(state : TextSourceViewState const;
                                    style : TextSourceSyntaxStyle) : int {
    var count = 0
    let style_id = 1 + int(style)
    for (cell in state.cells) {
        if (cell.style_id == style_id) count++
    }
    return count
}

def private inspector_request_state_name(state : InspectorRequestState) : string {
    if (state == InspectorRequestState.queued) return "queued"
    if (state == InspectorRequestState.running) return "running"
    return "idle"
}

def private inspector_content_state_name(state : InspectorContentState) : string {
    if (state == InspectorContentState.ready) return "ready"
    if (state == InspectorContentState.failed) return "failed"
    return "absent"
}

def private draw_binary_preview() {
    text("Binary file ({g_inspector.preview_byte_count} bytes)")
    if (g_inspector.preview_texture == 0u) {
        text_disabled(INSPECTOR_BINARY_UNAVAILABLE,
            (text = empty(g_inspector.preview_error)
                ? "Preview unavailable"
                : "Preview unavailable: {g_inspector.preview_error}"))
        if (!empty(g_inspector.preview_error)) {
            draw_error_context(PREVIEW_ERROR_POPUP, PREVIEW_ERROR_COPY,
                PREVIEW_ERROR_SEND, "preview_error", "File Preview",
                g_inspector.preview_error)
        }
        return
    }
    text_disabled(INSPECTOR_BINARY_DIMENSIONS,
        (text = "Decoded by the bundled stb_image: " +
            "{g_inspector.preview_width} x {g_inspector.preview_height}"))
    let available = GetContentRegionAvail()
    let scale = min(1.0f, min(
        available.x / max(1.0f, float(g_inspector.preview_width)),
        available.y / max(1.0f, float(g_inspector.preview_height))))
    unsafe {
        image(INSPECTOR_BINARY_PREVIEW,
            (user_texture_id = inspector_preview_texture_ref(
                g_inspector.preview_texture),
             size = float2(max(1.0f, float(g_inspector.preview_width) * scale),
                           max(1.0f, float(g_inspector.preview_height) * scale)),
             uv0 = float2(0.0f, 0.0f), uv1 = float2(1.0f, 1.0f),
             tint_col = float4(1.0f, 1.0f, 1.0f, 1.0f),
             border_col = float4(0.35f, 0.35f, 0.35f, 1.0f)))
    }
}

def private draw_side_by_side_diff() {
    if (g_inspector.parsed.binary) {
        draw_binary_preview()
        return
    }
    let hunk_count = length(g_inspector.parsed.hunks)
    g_inspector.diff_current_hunk = hunk_count > 0 ? clamp(g_inspector.diff_current_hunk, 0, hunk_count - 1) : 0
    let replacement_busy = (g_inspector.loading
        || g_inspector.prepare.request != InspectorRequestState.idle)
    with_disabled(replacement_busy) {
        SetNextItemWidth(150.0f)
        combo(DIFF_CONTEXT,
            (text = "Context", items <- INSPECTOR_CONTEXT_LABELS))
        same_line()
        SetNextItemWidth(150.0f)
        combo(DIFF_SYNTAX, (text = "Syntax", items <- INSPECTOR_SYNTAX_LABELS))
    }
    let selected_context = clamp(DIFF_CONTEXT.value, 0,
        length(INSPECTOR_CONTEXT_VALUES) - 1)
    if (!replacement_busy
            && selected_context != g_inspector.installed_diff_context_choice
            && g_client != null && g_client.connected) {
        g_client->inspect_file(g_inspector.installed_repository_id,
            g_inspector.installed_worktree_path, g_inspector.installed_file_path,
            g_inspector.installed_comparison, INSPECTOR_CONTEXT_VALUES[selected_context],
            g_inspector.installed_git_revision)
    }
    let selected_syntax = clamp(DIFF_SYNTAX.value, 0,
        length(INSPECTOR_SYNTAX_VALUES) - 1)
    if (!replacement_busy
            && selected_syntax != g_inspector.installed_syntax_choice) {
        g_inspector.repository_id = g_inspector.installed_repository_id
        g_inspector.worktree_path = g_inspector.installed_worktree_path
        g_inspector.file_path = g_inspector.installed_file_path
        g_inspector.comparison = g_inspector.installed_comparison
        g_inspector.git_revision = g_inspector.installed_git_revision
        g_inspector.diff_context = g_inspector.installed_diff_context
        g_inspector.diff_context_choice = g_inspector.installed_diff_context_choice
        g_inspector.syntax_choice = selected_syntax
        g_inspector.pending_select_diff = false
        if (!empty(g_inspector.source_patch)) {
            queue_inspector_prepare("Applying syntax...")
        }
    }
    same_line()
    if (small_button(DIFF_PREVIOUS_CHANGE, (text = "Previous change",
            enabled = g_inspector.diff_current_hunk > 0))) {
        g_inspector.diff_current_hunk = max(0,
            g_inspector.diff_current_hunk - 1)
        if (g_inspector.diff_current_hunk < hunk_count) {
            let row = g_inspector.parsed.hunks[g_inspector.diff_current_hunk].row_start
            g_inspector.diff_scroll_y = float(row) * max(
                GetTextLineHeight(), g_inspector.new_view.key.line_height)
            g_inspector.diff_scroll_jump_pending = true
        }
    }
    same_line()
    if (small_button(DIFF_NEXT_CHANGE, (text = "Next change",
            enabled = g_inspector.diff_current_hunk + 1 < hunk_count))) {
        g_inspector.diff_current_hunk = min(hunk_count - 1,
            g_inspector.diff_current_hunk + 1)
        if (g_inspector.diff_current_hunk >= 0
                && g_inspector.diff_current_hunk < hunk_count) {
            let row = g_inspector.parsed.hunks[g_inspector.diff_current_hunk].row_start
            g_inspector.diff_scroll_y = float(row) * max(
                GetTextLineHeight(), g_inspector.new_view.key.line_height)
            g_inspector.diff_scroll_jump_pending = true
        }
    }
    same_line()
    text_disabled(DIFF_CHANGE_POSITION, (text = hunk_count > 0
        ? "Change {g_inspector.diff_current_hunk + 1} of {hunk_count}"
        : "No text changes"))

    let available = GetContentRegionAvail()
    let splitter_width = 8.0f
    let usable_width = max(2.0f, available.x - splitter_width)
    var left_width = usable_width * clamp(
        g_inspector.diff_split_fraction, 0.12f, 0.88f)
    var right_width = usable_width - left_width
    let origin = GetCursorScreenPos()
    let split_min = min(120.0f, usable_width * 0.4f)
    let split_box = float4(origin.x + left_width, origin.y,
        origin.x + left_width + splitter_width, origin.y + available.y)
    let splitting = SplitterBehavior(split_box, GetID("git_diff_splitter"),
        ImGuiAxis.X, left_width, right_width, split_min, split_min)
    g_inspector.diff_split_fraction = left_width / max(1.0f, left_width + right_width)

    child(DIFF_LEFT_PANE, (text = "before", size = float2(left_width, available.y),
            child_flags = ImGuiChildFlags.Borders,
            window_flags = ImGuiWindowFlags.NoScrollbar)) {
        let window_pos = GetWindowPos()
        let window_size = GetWindowSize()
        g_inspector.diff_left_rect = float4(window_pos.x, window_pos.y,
            window_pos.x + window_size.x, window_pos.y + window_size.y)
        let hovered = IsWindowHovered(ImGuiHoveredFlags.AllowWhenBlockedByActiveItem)
        if (g_inspector.diff_scroll_jump_pending || !hovered) {
            SetScrollY(g_inspector.diff_scroll_y)
        } else {
            g_inspector.diff_scroll_y = GetScrollY()
        }
        text_disabled("BEFORE")
        separator()
        var style = source_view_style(true)
        let _ = text_source_view("git diff before", g_inspector.old_document,
            g_inspector.old_view, style)
    }
    same_line((spacing = 0.0f))
    dummy(DIFF_SPLITTER_SPACE, (size = float2(splitter_width, available.y)))
    let splitter_hot = IsItemHovered() || splitting
    if (splitter_hot) SetMouseCursor(ImGuiMouseCursor.ResizeEW)
    let splitter_color = GetColorU32(splitter_hot
        ? ImGuiCol.SeparatorActive : ImGuiCol.Separator)
    with_window_drawlist() $(var dl) {
        dl |> add_line(
            float2(origin.x + left_width + splitter_width * 0.5f, origin.y),
            float2(origin.x + left_width + splitter_width * 0.5f,
                origin.y + available.y), splitter_color, 1.0f)
    }
    same_line((spacing = 0.0f))
    child(DIFF_RIGHT_PANE, (text = "after", size = float2(right_width, available.y),
            child_flags = ImGuiChildFlags.Borders,
            window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
        let window_pos = GetWindowPos()
        let window_size = GetWindowSize()
        let scrollbar_width = GetStyle().ScrollbarSize
        g_inspector.diff_right_rect = float4(window_pos.x, window_pos.y,
            window_pos.x + window_size.x, window_pos.y + window_size.y)
        g_inspector.diff_scrollbar_rect = float4(
            window_pos.x + window_size.x - scrollbar_width, window_pos.y,
            window_pos.x + window_size.x, window_pos.y + window_size.y)
        let hovered = IsWindowHovered(ImGuiHoveredFlags.AllowWhenBlockedByActiveItem)
        if (g_inspector.diff_scroll_jump_pending || !hovered) {
            SetScrollY(g_inspector.diff_scroll_y)
        } else {
            g_inspector.diff_scroll_y = GetScrollY()
        }
        text_disabled("AFTER")
        separator()
        var style = source_view_style(true)
        let result <- text_source_view("git diff after", g_inspector.new_document,
            g_inspector.new_view, style)
        draw_active_focus_overlay(g_inspector.new_view,
            g_inspector.new_document, true)
        if (result.selection_context_requested) {
            open_popup(DIFF_SELECTION_POPUP)
        }
        popup(DIFF_SELECTION_POPUP,
                (text = "DiffSelection", flags = ImGuiWindowFlags.None)) {
            let selection = text_source_view_selection(g_inspector.new_view)
            if (menu_item(DIFF_LOOK_AT_THAT,
                    (text = "Look at that", enabled = g_client != null
                        && g_client.connected && !empty(g_client.selected_id)
                        && selection.end_byte > selection.start_byte))) {
                let canonical_start = diff_byte_to_view_byte(
                    selection.start_byte, false)
                let canonical_end = diff_byte_to_view_byte(
                    selection.end_byte, true)
                g_client->post_selection_request(canonical_start, canonical_end)
                close_current_popup(DIFF_SELECTION_POPUP)
            }
        }
    }
    g_inspector.diff_scroll_jump_pending = false
}

def private draw_file_view() {
    if (!g_inspector.view_prepared) {
        draw_binary_preview()
        return
    }
    child(FILE_VIEW_PANE, (text = "file view", size = float2(0.0f, 0.0f),
            child_flags = ImGuiChildFlags.None,
            window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
        if (g_inspector.resolved_language == "markdown"
                && !g_inspector.focus_document_valid) {
            var style = MarkdownViewStyle(
                fonts = g_fonts,
                typography = MarkdownTypography(
                    inline_code_scale = 0.84f, code_block_scale = 0.90f),
                zoom = float(g_zoom_percent) / 100.0f,
                write_system_clipboard = !harness_is_headless())
            let _ <- markdown_view_with_change_gutter("git markdown view",
                g_inspector.markdown_document, g_inspector.markdown_view,
                style, g_inspector.revision, g_inspector.changed_source_ranges,
                g_inspector.markdown_gutter_segments,
                max(1.0f, 2.0f * float(g_zoom_percent) / 100.0f))
        } else {
            if (g_focus_view_scroll_pending) {
                SetScrollY(max(0.0f, g_focus_view_scroll_y))
                g_focus_view_scroll_pending = false
            }
            var style = source_view_style(false)
            let result <- text_source_view("git file view",
                g_inspector.view_document, g_inspector.view, style)
            draw_active_focus_overlay(g_inspector.view,
                g_inspector.view_document, false)
            if (result.selection_context_requested) {
                open_popup(INSPECTOR_SELECTION_POPUP)
            }
            popup(INSPECTOR_SELECTION_POPUP,
                    (text = "InspectorSelection", flags = ImGuiWindowFlags.None)) {
                let selection = text_source_view_selection(g_inspector.view)
                if (menu_item(INSPECTOR_LOOK_AT_THAT,
                        (text = "Look at that", enabled = g_client != null
                            && g_client.connected && !empty(g_client.selected_id)
                            && selection.end_byte > selection.start_byte))) {
                    g_client->post_selection_request(
                        selection.start_byte, selection.end_byte)
                    close_current_popup(INSPECTOR_SELECTION_POPUP)
                }
            }
        }
    }
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

def private file_inspector_status() : GitWindowStatus {
    if (g_client == null || !g_client.connected) {
        return GitWindowStatus(text = "WATCHER DISCONNECTED - reconnecting...", error = true)
    }
    let retained = g_inspector.prepare.has_retained_content
    let showing = retained ? " - showing {g_inspector.installed_file_path}" : ""
    if (g_inspector.loading) {
        let phase = empty(g_inspector.status) ? "Reading Git diff..." : g_inspector.status
        return GitWindowStatus(
            text = "LOADING {g_inspector.file_path} - {phase}{showing}", busy = true)
    }
    if (g_inspector.prepare.request != InspectorRequestState.idle) {
        let phase = (empty(g_inspector.prepare.status)
            ? "Preparing view..." : g_inspector.prepare.status)
        return GitWindowStatus(
            text = "PREPARING {g_inspector.file_path} - {phase}{showing}", busy = true)
    }
    if (!empty(g_inspector.error)) {
        let task = (empty(g_inspector.task_session_id)
            ? "" : "   TASK {g_inspector.task_session_id}")
        return GitWindowStatus(
            text = "FAILED {g_inspector.file_path} - {g_inspector.error}{showing}{task}", error = true)
    }
    if (retained) {
        return GitWindowStatus(text = "{to_upper(g_inspector.installed_comparison)}  {g_inspector.installed_file_path}   HUNKS {length(g_inspector.parsed.hunks)}   PREP {g_inspector.prepare_elapsed_usec / 1000l} ms")
    }
    return GitWindowStatus(text = "Select a changed file")
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

def private append_line_starts(source : string;
                               var starts : array<int>&) {
    delete starts
    starts |> push(0)
    peek_data(source) $(bytes) {
        for (index in range(length(bytes))) {
            if (int(bytes[index]) == '\n') starts |> push(index + 1)
        }
    }
}

def private diff_byte_to_view_byte(diff_byte : int;
                                   end_bias : bool) : int {
    let clamped_byte = clamp(diff_byte, 0,
        length(g_inspector.new_document.source))
    var display_line = 0
    for (index in range(length(g_inspector.new_line_starts))) {
        if (g_inspector.new_line_starts[index] > clamped_byte) break
        display_line = index
    }
    if (display_line < 0
            || display_line >= length(g_inspector.new_document.line_numbers)) return -1
    var source_line = g_inspector.new_document.line_numbers[display_line]
    if (source_line <= 0 && end_bias) {
        var scan = display_line - 1
        while (scan >= 0) {
            source_line = g_inspector.new_document.line_numbers[scan]
            if (source_line > 0) break
            scan--
        }
    }
    if (source_line <= 0
            || source_line > length(g_inspector.view_line_starts)) return -1
    let column = max(0,
        clamped_byte - g_inspector.new_line_starts[display_line])
    let line_start = g_inspector.view_line_starts[source_line - 1]
    let line_end = (source_line < length(g_inspector.view_line_starts)
        ? max(line_start, g_inspector.view_line_starts[source_line] - 1)
        : length(g_inspector.view_document.source))
    return min(line_end, line_start + column)
}

def private focus_intersects(target : WatcherFocusTarget const;
                             start_byte, end_byte : int) : bool {
    if (target.whole_file || empty(target.ranges)) return true
    for (focus_range in target.ranges) {
        if (focus_range.end_byte > start_byte
                && focus_range.start_byte < end_byte) return true
    }
    return false
}

def private focus_caption(target : WatcherFocusTarget const;
                          start_byte, end_byte : int) : string {
    for (focus_range in target.ranges) {
        if (!empty(focus_range.caption)
                && focus_range.end_byte > start_byte
                && focus_range.start_byte < end_byte) {
            return focus_range.caption
        }
    }
    return ""
}

def private mouse_inside(p0, p1 : float2) : bool {
    let mouse = GetMousePos()
    return (mouse.x >= p0.x && mouse.x <= p1.x
        && mouse.y >= p0.y && mouse.y <= p1.y)
}

def private draw_focus_target_overlay(target : WatcherFocusTarget const;
                                      state : TextSourceViewState const;
                                      document : TextSourceDocument const;
                                      diff_mapping : bool) {
    let dim_color = GetColorU32(float4(0.015f, 0.018f, 0.024f, 0.48f))
    let focus_fill = GetColorU32(float4(1.0f, 0.72f, 0.18f, 0.13f))
    let focus_outline = GetColorU32(float4(1.0f, 0.76f, 0.24f, 0.92f))
    let clip_min = GetWindowPos()
    let clip_max = clip_min + GetWindowSize()
    var hover_caption = ""
    with_window_drawlist() $(var dl) {
        dl |> with_drawlist_clip_rect(clip_min, clip_max, true) {
            if (state.virtual_mode && !empty(state.virtual_line_starts)) {
                let line_height = (state.screen_size.y
                    / float(length(state.virtual_line_starts)))
                for (line_index in range(length(state.virtual_line_starts))) {
                    let display_start = state.virtual_line_starts[line_index]
                    let display_end = (line_index + 1 < length(state.virtual_line_starts)
                        ? max(display_start,
                            state.virtual_line_starts[line_index + 1] - 1)
                        : length(document.source))
                    var source_start = display_start
                    var source_end = display_end
                    var mapped = !diff_mapping
                    if (diff_mapping && line_index < length(document.line_numbers)) {
                        let source_line = document.line_numbers[line_index]
                        if (source_line > 0
                                && source_line <= length(g_inspector.view_line_starts)) {
                            source_start = g_inspector.view_line_starts[source_line - 1]
                            source_end = (source_line < length(g_inspector.view_line_starts)
                                ? max(source_start,
                                    g_inspector.view_line_starts[source_line] - 1)
                                : length(g_inspector.view_document.source))
                            mapped = true
                        }
                    }
                    let y0 = state.screen_origin.y + float(line_index) * line_height
                    let p0 = float2(state.screen_origin.x, y0)
                    let p1 = float2(state.screen_origin.x + state.screen_size.x,
                        y0 + line_height)
                    if (p1.y < clip_min.y || p0.y > clip_max.y) continue
                    let focused = mapped && focus_intersects(target,
                        source_start, max(source_start + 1, source_end))
                    if (!focused) {
                        dl |> add_rect_filled(p0, p1, dim_color)
                    } else {
                        dl |> add_rect_filled(p0, p1, focus_fill)
                        let marker_right = (state.screen_origin.x
                            - max(2.0f, GetStyle().ItemSpacing.x * 0.35f))
                        dl |> add_rect_filled(
                            float2(marker_right - 3.0f, p0.y),
                            float2(marker_right, p1.y), focus_outline)
                        let caption = focus_caption(target,
                            source_start, max(source_start + 1, source_end))
                        if (empty(hover_caption) && !empty(caption)
                                && mouse_inside(p0, p1)) {
                            hover_caption = caption
                        }
                    }
                }
                return
            }
            var last_focused_line = -1
            for (cell in state.cells) {
                var source_start = cell.source.start_byte
                var source_end = cell.source.end_byte
                var mapped = !diff_mapping
                if (diff_mapping && cell.line_index >= 0
                        && cell.line_index < length(document.line_numbers)
                        && cell.line_index < length(g_inspector.new_line_starts)) {
                    let source_line = document.line_numbers[cell.line_index]
                    if (source_line > 0
                            && source_line <= length(g_inspector.view_line_starts)) {
                        let canonical_line_start = g_inspector.view_line_starts[source_line - 1]
                        let canonical_line_end = (source_line
                                < length(g_inspector.view_line_starts)
                            ? max(canonical_line_start,
                                g_inspector.view_line_starts[source_line] - 1)
                            : length(g_inspector.view_document.source))
                        source_start = min(canonical_line_end,
                            canonical_line_start + max(0,
                                cell.source.start_byte
                                    - g_inspector.new_line_starts[cell.line_index]))
                        source_end = min(canonical_line_end,
                            canonical_line_start + max(0,
                                cell.source.end_byte
                                    - g_inspector.new_line_starts[cell.line_index]))
                        mapped = true
                    }
                }
                let p0 = state.screen_origin + float2(cell.rect.x, cell.rect.y)
                let p1 = state.screen_origin + float2(cell.rect.z, cell.rect.w)
                let focused = mapped && focus_intersects(target,
                    source_start, max(source_start + 1, source_end))
                if (!focused) {
                    dl |> add_rect_filled(p0, p1, dim_color)
                } else {
                    dl |> add_rect_filled(p0, p1, focus_fill)
                    if (cell.line_index != last_focused_line) {
                        let marker_right = (state.screen_origin.x
                            - max(2.0f, GetStyle().ItemSpacing.x * 0.35f))
                        dl |> add_rect_filled(
                            float2(marker_right - 3.0f, p0.y),
                            float2(marker_right, p1.y), focus_outline)
                        last_focused_line = cell.line_index
                    }
                    let caption = focus_caption(target,
                        source_start, max(source_start + 1, source_end))
                    let hover_p0 = float2(state.screen_origin.x, p0.y)
                    let hover_p1 = float2(state.screen_origin.x
                        + state.screen_size.x, p1.y)
                    if (empty(hover_caption) && !empty(caption)
                            && mouse_inside(hover_p0, hover_p1)) {
                        hover_caption = caption
                    }
                }
            }
        }
    }
    if (!empty(hover_caption)) {
        set_tooltip(FOCUS_HINT_TOOLTIP, hover_caption)
    }
}

def private draw_active_focus_overlay(state : TextSourceViewState const;
                                      document : TextSourceDocument const;
                                      diff_mapping : bool) {
    if (!g_inspector.focus_document_valid || g_client == null) return
    for (message in g_client.mailbox) {
        if (message.id != g_active_focus_message
                || g_active_focus_target < 0
                || g_active_focus_target >= length(message.focus.targets)) continue
        let target & = unsafe(message.focus.targets[g_active_focus_target])
        if (watcher_focus_repository(message.focus, target)
                    == g_inspector.installed_repository_id
                && watcher_focus_worktree(message.focus, target)
                    == g_inspector.installed_worktree_path
                && target.file_path == g_inspector.installed_file_path) {
            draw_focus_target_overlay(target, state, document, diff_mapping)
        }
        return
    }
}

def private apply_active_focus() {
    if (g_client == null) return
    let identity = ("{g_active_focus_message}:{g_active_focus_target}:" +
        "{g_active_focus_range}:" +
        "{g_focus_visible}:{g_inspector.revision}:{g_inspector.installed_file_path}")
    if (identity == g_applied_focus_identity && !g_pending_focus_scroll) return
    g_inspector.focus_document_valid = false
    g_inspector.focus_start_byte = -1
    var matched_target = false
    if (g_focus_visible && !empty(g_active_focus_message)) {
        for (message in g_client.mailbox) {
            if (message.id != g_active_focus_message
                    || g_active_focus_target < 0
                    || g_active_focus_target >= length(message.focus.targets)) continue
            let target & = unsafe(message.focus.targets[g_active_focus_target])
            if (watcher_focus_repository(message.focus, target)
                        != g_inspector.installed_repository_id
                    || watcher_focus_worktree(message.focus, target)
                        != g_inspector.installed_worktree_path
                    || target.file_path != g_inspector.installed_file_path) continue
            matched_target = true
            g_inspector.focus_document_valid = true
            if (!empty(target.ranges)) {
                if (g_active_focus_range < 0
                        || g_active_focus_range >= length(target.ranges)) {
                    g_active_focus_range = first_focus_range(target)
                }
                g_inspector.focus_start_byte = target.ranges[
                    g_active_focus_range].start_byte
            } else {
                g_inspector.focus_start_byte = 0
            }
            if (g_pending_focus_scroll) {
                var line_index = 0
                peek_data(g_inspector.view_document.source) $(bytes) {
                    for (index in range(clamp(g_inspector.focus_start_byte,
                            0, length(bytes)))) {
                        if (int(bytes[index]) == '\n') {
                            line_index++
                        }
                    }
                }
                var diff_row = line_index
                for (row_index in range(length(g_inspector.parsed.rows))) {
                    if (g_inspector.parsed.rows[row_index].new_line
                            == line_index + 1) {
                        diff_row = row_index
                        break
                    }
                }
                let diff_line_height = max(GetTextLineHeight(),
                    g_inspector.new_view.key.line_height)
                let view_line_height = max(GetTextLineHeight(),
                    g_inspector.view.key.line_height)
                g_inspector.diff_scroll_y = (float(max(0, diff_row - 1))
                    * diff_line_height)
                g_focus_view_scroll_y = (float(max(0, line_index - 1))
                    * view_line_height)
                g_inspector.diff_scroll_jump_pending = true
                g_focus_view_scroll_pending = true
            }
            break
        }
    }
    if (!g_pending_focus_scroll || matched_target) {
        g_pending_focus_scroll = false
        g_applied_focus_identity = identity
    } else {
        // Replacement inspection keeps the old document alive. Do not consume
        // the one-shot navigation until the requested Focus target installs.
        g_applied_focus_identity = ""
    }
}

def private navigate_active_focus(delta : int) {
    if (g_client == null || empty(g_active_focus_message)) return
    for (message in g_client.mailbox) {
        if (message.id != g_active_focus_message || empty(message.focus.targets)) continue
        let target_index = (g_active_focus_target + delta
            + length(message.focus.targets)) % length(message.focus.targets)
        activate_focus_target(message, target_index)
        return
    }
}

def private active_focus_range_count() : int {
    if (g_client == null || empty(g_active_focus_message)) return 0
    for (message in g_client.mailbox) {
        if (message.id == g_active_focus_message
                && g_active_focus_target >= 0
                && g_active_focus_target < length(message.focus.targets)) {
            return length(message.focus.targets[g_active_focus_target].ranges)
        }
    }
    return 0
}

def private navigate_active_focus_range(delta : int) {
    if (g_client == null || empty(g_active_focus_message) || delta == 0) return
    for (message in g_client.mailbox) {
        if (message.id != g_active_focus_message
                || g_active_focus_target < 0
                || g_active_focus_target >= length(message.focus.targets)) continue
        let target & = unsafe(message.focus.targets[g_active_focus_target])
        if (empty(target.ranges)) return
        if (g_active_focus_range < 0
                || g_active_focus_range >= length(target.ranges)) {
            g_active_focus_range = first_focus_range(target)
        }
        let current_start = target.ranges[g_active_focus_range].start_byte
        var candidate = -1
        for (index in range(length(target.ranges))) {
            let start_byte = target.ranges[index].start_byte
            if (delta > 0 && start_byte > current_start
                    && (candidate < 0 || start_byte
                        < target.ranges[candidate].start_byte)) {
                candidate = index
            } elif (delta < 0 && start_byte < current_start
                    && (candidate < 0 || start_byte
                        > target.ranges[candidate].start_byte)) {
                candidate = index
            }
        }
        if (candidate < 0) {
            candidate = first_focus_range(target)
            if (delta < 0) {
                for (index in range(length(target.ranges))) {
                    if (target.ranges[index].start_byte
                            > target.ranges[candidate].start_byte) {
                        candidate = index
                    }
                }
            }
        }
        g_active_focus_range = candidate
        g_pending_focus_scroll = true
        g_applied_focus_identity = ""
        return
    }
}

def private draw_file_inspector_content() {
    let io & = unsafe(GetIO())
    if (IsWindowHovered(ImGuiHoveredFlags.RootAndChildWindows) && io.KeyCtrl) {
        if (io.MouseWheel > 0.0f) zoom_by(5)
        elif (io.MouseWheel < 0.0f) zoom_by(-5)
    }
    if (!g_inspector.prepare.has_retained_content) {
        if (empty(g_inspector.file_path)) {
            text_disabled("Select a changed file")
        }
        return
    }
    apply_active_focus()
    text(INSPECTOR_PATH, (text = g_inspector.installed_file_path))
    same_line()
    text_disabled(INSPECTOR_SCOPE,
        (text = to_upper(g_inspector.installed_comparison)))
    if (!empty(g_active_focus_message)) {
        same_line()
        if (arrow_button(FOCUS_PREVIOUS,
                (text = "##previous-focused-file", dir = ImGuiDir.Left))) {
            navigate_active_focus(-1)
        }
        set_item_tooltip(FOCUS_PREVIOUS_TIP,
            (text = "Previous focused file"))
        same_line()
        if (arrow_button(FOCUS_NEXT,
                (text = "##next-focused-file", dir = ImGuiDir.Right))) {
            navigate_active_focus(1)
        }
        set_item_tooltip(FOCUS_NEXT_TIP, (text = "Next focused file"))
        if (active_focus_range_count() > 1) {
            same_line()
            if (arrow_button(FOCUS_RANGE_PREVIOUS,
                    (text = "##previous-highlight", dir = ImGuiDir.Up))) {
                navigate_active_focus_range(-1)
            }
            set_item_tooltip(FOCUS_RANGE_PREVIOUS_TIP,
                (text = "Previous highlight in file"))
            same_line()
            if (arrow_button(FOCUS_RANGE_NEXT,
                    (text = "##next-highlight", dir = ImGuiDir.Down))) {
                navigate_active_focus_range(1)
            }
            set_item_tooltip(FOCUS_RANGE_NEXT_TIP,
                (text = "Next highlight in file"))
        }
        same_line()
        if (icon_button(FOCUS_TOGGLE,
                (glyph = g_focus_visible ? "visible" : "hidden",
                 tip = g_focus_visible ? "Hide focus" : "Show focus",
                 size = 16.0f))) {
            g_focus_visible = !g_focus_visible
            g_applied_focus_identity = ""
        }
    }
    tab_bar(INSPECTOR_MODE_TABS,
            (text = "InspectorModes", flags = ImGuiTabBarFlags.None)) {
        tab_item(INSPECTOR_DIFF_TAB, (text = "Diff", closable = false,
                flags = ImGuiTabItemFlags.None)) {
            g_inspector.mode = GitInspectorMode.diff
            draw_side_by_side_diff()
        }
        tab_item(INSPECTOR_VIEW_TAB, (text = "View", closable = false,
                flags = ImGuiTabItemFlags.None)) {
            g_inspector.mode = GitInspectorMode.view
            draw_file_view()
        }
    }
}

def private draw_file_inspector_window() {
    child(FILE_INSPECTOR_CONTENT, (text = "inspector content",
            size = float2(0.0f, -GetFrameHeightWithSpacing()),
            child_flags = ImGuiChildFlags.None,
            window_flags = ImGuiWindowFlags.None)) {
        draw_file_inspector_content()
    }
    let status = file_inspector_status()
    separator(INSPECTOR_STATUS_SEPARATOR)
    text_colored(INSPECTOR_STATUS_TEXT,
        (color = git_status_color(status), text = status.text))
    if (status.error) {
        draw_error_context(INSPECTOR_ERROR_POPUP, INSPECTOR_ERROR_COPY,
            INSPECTOR_ERROR_SEND, "inspector_error", "File Inspector",
            status.text)
    }
}

def private draw_settings_window() {
    text("Global user configuration")
    text_disabled("Repository registration and layout settings will be stored here.")
}

def private draw_session_launcher_window() {
    text("Codex debug session")
    text_disabled("The selected worktree becomes the immutable session origin.")
    separator()
    text("Repository: {g_selected_repository_id}")
    text("Selected worktree: {g_selected_worktree_path}")
    checkbox(CREATE_DEDICATED_WORKTREE, (text = "Create a dedicated worktree"))
    if (CREATE_DEDICATED_WORKTREE.value) {
        input_text_growable(AGENT_BRANCH, (text = "New branch"))
        input_text_growable(AGENT_BASE_REF, (text = "Base ref"))
        input_text_growable(AGENT_WORKTREE_PATH, (text = "New worktree path"))
    }
    text("Debug task")
    input_text_multiline(AGENT_TASK_CONTEXT,
        (text = "##debug-task", size = float2(-FLT_MIN, GetTextLineHeight() * 8.0f),
         flags = ImGuiInputTextFlags.None))
    if (button(LAUNCH_CODEX_DEBUG,
            (text = "Launch Codex", enabled = g_client != null
                && g_client.connected && !empty(g_selected_repository_id)
                && !empty(g_selected_worktree_path)
                && (!CREATE_DEDICATED_WORKTREE.value
                    || (!empty(AGENT_BRANCH.value)
                        && !empty(AGENT_WORKTREE_PATH.value)))
                && !empty(AGENT_TASK_CONTEXT.value)))) {
        if (CREATE_DEDICATED_WORKTREE.value) {
            g_client->create_worktree_and_launch_codex(g_selected_worktree_path,
                g_selected_repository_id, AGENT_WORKTREE_PATH.value,
                AGENT_BRANCH.value, AGENT_BASE_REF.value,
                AGENT_TASK_CONTEXT.value)
        } else {
            g_client->launch_codex_debug(g_selected_worktree_path,
                g_selected_repository_id, AGENT_TASK_CONTEXT.value)
        }
        SESSION_LAUNCHER_WIN.open = false
    }
}

def private draw_terminal_window() {
    let io & = unsafe(GetIO())
    if (IsWindowHovered(ImGuiHoveredFlags.RootAndChildWindows) && io.KeyCtrl) {
        if (io.MouseWheel > 0.0f) zoom_by(5)
        elif (io.MouseWheel < 0.0f) zoom_by(-5)
    }
    text(SESSION_LABEL, (text = selected_session_label()))
    if (g_client != null && !empty(g_client.error)) {
        text_colored(CLIENT_ERROR,
            (color = float4(1.0f, 0.35f, 0.30f, 1.0f), text = g_client.error))
        draw_error_context(CLIENT_ERROR_POPUP, CLIENT_ERROR_COPY,
            CLIENT_ERROR_SEND, "client_error", "Watcher", g_client.error)
    }
    separator()
    let terminal_style = ImGuiTerminalStyle(
        external_input = true,
        view_height = max(GetTextLineHeight(),
            GetContentRegionAvail().y - GetFrameHeightWithSpacing()
                - GetStyle().ItemSpacing.y))
    with_font(g_terminal_font) {
        if (g_focus_terminal_pending) {
            set_keyboard_focus_here()
            g_focus_terminal_pending = false
        }
        let result <- imgui_terminal("watcher", g_terminal, g_terminal_view,
            g_terminal_cache, terminal_style)
        if (g_client != null) {
            if (!empty(result.input_text)) g_client->send_input(result.input_text)
            if (result.resized) g_client->request_resize(result.columns, result.rows)
        }
    }
    separator()
    draw_terminal_footer()
}

[export]
def init() {
    harness_init("dasHerd", 1280, 800)
    harness_maximize_window()
    DIFF_SYNTAX.value = 0
    DIFF_CONTEXT.value = 0
    GIT_TREE_FILTER.value = 0
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
    AGENT_TASK_CONTEXT.capacity = 16 * 1024
    AGENT_BRANCH.capacity = 1024
    AGENT_BASE_REF.capacity = 1024
    AGENT_WORKTREE_PATH.capacity = 16 * 1024
    CREATE_DEDICATED_WORKTREE.value = true
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
    if (!harness_begin_frame()) return
    if (g_client != null) g_client->pump()
    service_sha_copy()
    drain_inspector_prepare_events()
    harness_new_frame()
    GetStyle().FontScaleMain = float(g_zoom_percent) / 100.0f
    draw_main_menu()
    dockspace(DOCK_ROOT, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        if (!DOCK_ROOT.has_initial_layout && DOCK_ROOT.dock_id != 0u) {
            setup_default_layout(DOCK_ROOT.dock_id)
            DOCK_ROOT.has_initial_layout = true
        }
        dock_window(REPOSITORIES_WIN, (text = "Repositories & Worktrees", closable = true,
                flags = ImGuiWindowFlags.None)) {
            draw_repositories_window()
        }
        let attention_count = pending_attention_count()
        let sessions_title = (attention_count > 0
            ? "Sessions & Activity  [! {attention_count}]###SessionsActivity"
            : "Sessions & Activity###SessionsActivity")
        dock_window(SESSIONS_WIN, (text = sessions_title, closable = true,
                flags = ImGuiWindowFlags.None)) {
            draw_session_list()
        }
        dock_window(GIT_ACTIVITY_WIN, (text = "Git Activity", closable = true,
                flags = ImGuiWindowFlags.None)) {
            draw_git_activity_window()
        }
        dock_window(GIT_CHANGELIST_WIN, (text = "Git Changelist", closable = true,
                flags = ImGuiWindowFlags.None)) {
            draw_git_changelist_window()
        }
        dock_window(FILE_INSPECTOR_WIN, (text = "File Inspector", closable = true,
                flags = ImGuiWindowFlags.None)) {
            draw_file_inspector_window()
        }
        dock_window(TERMINAL_WIN, (text = "Terminal", closable = true,
                flags = ImGuiWindowFlags.None)) {
            draw_terminal_window()
        }
        dock_window(SETTINGS_WIN, (text = "Settings", closable = true,
                flags = ImGuiWindowFlags.None)) {
            draw_settings_window()
        }
        dock_window(SESSION_LAUNCHER_WIN, (text = "Session Launcher", closable = true,
                flags = ImGuiWindowFlags.None)) {
            draw_session_launcher_window()
        }
    }
    harness_end_frame()
}

[export]
def shutdown() {
    let prepare_stopped = inspector_prepare_worker_shutdown(g_prepare_worker)
    if (g_client != null) {
        g_client->close()
        g_client->cleanup()
        delete g_client.sessions
        delete g_client.repositories
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
    terminal_viewport_snapshot_dispose(g_terminal_cache.snapshot)
    terminal_dispose(g_terminal)
    delete g_font_sources
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
        update()
        harness_maybe_collect_gc()
    }
    shutdown()
}
