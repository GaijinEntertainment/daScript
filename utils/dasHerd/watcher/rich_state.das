options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_state

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
require daslib/json_boost
require daslib/jobque_boost
require daslib/strings_boost
require live/live_commands
require ./rich_tokens.das public
require ./watcher_core.das
require ./repository_core.das
require ./herd_sessions.das
require ./git_graph_model.das
require ./file_inspection.das
require ./inspection_worker.das
require ./image_preview_gl.das
require math
require strings

struct WatcherEnvelope {
    @rename = "type" _type : string
}

struct WatcherConnectedMessage {
    @rename = "type" _type : string
    client_id : int
}

struct WatcherAttachedMessage {
    @rename = "type" _type : string
    session_id : string
    controls : bool
}

struct WatcherSessionMessage {
    @rename = "type" _type : string
    session_id : string
}

struct WatcherErrorMessage {
    @rename = "type" _type : string
    error : string
}

struct WatcherSessionsMessage {
    @rename = "type" _type : string
    data : array<WatcherSessionInfo>
}

struct WatcherRepositoriesMessage {
    @rename = "type" _type : string
    data : array<RepositorySnapshot>
}

struct WatcherHerdSessionsMessage {
    @rename = "type" _type : string
    revision : int64
    profiles : array<HerdProfile>
    data : array<HerdSession>
}

struct WatcherHerdLaunchedMessage {
    @rename = "type" _type : string
    herd_session_id : string
    session_id : string
}

struct WatcherWorktreeDeleteResult {
    @rename = "type" _type : string
    ok : bool
    reason : string
    detail : string
    repository_id : string
    worktree_path : string
}

struct WatcherHerdDeleteKept {
    path : string
    repository_id : string
    reason : string
    detail : string
}

struct WatcherHerdDeleteResult {
    @rename = "type" _type : string
    herd_session_id : string
    removed : array<string>
    kept : array<WatcherHerdDeleteKept>
}

struct WatcherMailboxMessageEnvelope {
    @rename = "type" _type : string
    session_id : string
    revision : int64
    data : array<WatcherMailboxMessage>
}

struct WatcherBundlesMessageEnvelope {
    @rename = "type" _type : string
    session_id : string
    revision : int64
    data : array<WatcherReviewBundle>
}

struct WatcherMailboxPostedMessage {
    @rename = "type" _type : string
    message_id : string
    revision : int64
}

struct WatcherFileInspectionMessage {
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

struct WatcherGitFileActionMessage {
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

struct WatcherRawResetMessage {
    @rename = "type" _type : string
    offset : int64
}

struct WatcherWorktreeFilesMessage {
    @rename = "type" _type : string
    request_id : int
    repository_id : string
    worktree_path : string
    loading : bool
    listing : string
    error : string
}

struct WatcherTerminalCheckpointMessage {
    @rename = "type" _type : string
    offset : int64
    columns : int
    rows : int
    ansi : string
}

enum GitInspectorMode {
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

struct GitInspectorState {
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
    // Widget-state sync requests consumed by rich_inspector_ui at draw time.
    // State-level code cannot touch UI-module widget globals directly.
    pending_widget_sync : bool
    pending_select_diff_tab : bool
    pending_select_view_tab : bool
    task_session_id : string
    loading : bool
    status : string
    error : string
    revision : int
    prepare : InspectorLoadState = InspectorLoadState()
    prepare_elapsed_usec : int64
    mode : GitInspectorMode = GitInspectorMode.diff
    // The user's chosen mode, updated only on manual tab switches; new files
    // reopen in it (clean files still open straight in View).
    sticky_mode : GitInspectorMode = GitInspectorMode.diff
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

struct GitWindowStatus {
    text : string
    busy : bool
    error : bool
}

struct AgentLaunchLocation {
    repository_id : string
    worktree_path : string
    display_label : string
}

struct HerderPerfSample {
    surface : string
    identity : string
    elapsed_ms : float
    detail : string
}

struct HerderPerfPending {
    repository_id : string
    worktree_path : string
    extra : string
    started : int64
    seen_loading : bool
}

var g_perf_samples : array<HerderPerfSample>

var g_perf_review = HerderPerfPending()

var g_perf_history = HerderPerfPending()

var g_perf_refs = HerderPerfPending()

var g_perf_commit = HerderPerfPending()

var g_perf_inspect_started : int64

def perf_begin(var pending : HerderPerfPending;
               repository_id, worktree_path, extra : string) {
    pending.repository_id = repository_id
    pending.worktree_path = worktree_path
    pending.extra = extra
    pending.started = ref_time_ticks()
    pending.seen_loading = false
}

def perf_record(surface, identity : string; started : int64;
                detail : string = "") {
    if (started == 0l) return
    g_perf_samples |> push(HerderPerfSample(surface = surface,
        identity = identity,
        elapsed_ms = float(double(get_time_nsec(started)) / 1000000.0lf),
        detail = detail))
    if (length(g_perf_samples) > 64) {
        g_perf_samples |> erase(0, length(g_perf_samples) - 64)
    }
}

// Completion is observed from pushed snapshots: a pending surface must be seen
// loading at least once, then records on the first non-loading push.
def perf_observe_worktree(var pending : HerderPerfPending;
                          surface : string;
                          loading : bool; error_text : string) {
    if (pending.started == 0l) return
    if (loading) {
        pending.seen_loading = true
    } elif (pending.seen_loading) {
        perf_record(surface,
            "{pending.worktree_path}|{pending.extra}",
            pending.started, error_text)
        pending.started = 0l
    }
}

def perf_observe_repositories(repositories : array<RepositorySnapshot> const) {
    for (repository in repositories) {
        for (worktree in repository.worktrees) {
            if (repository.record.id == g_perf_review.repository_id
                    && worktree.path == g_perf_review.worktree_path) {
                perf_observe_worktree(g_perf_review, "review",
                    worktree.review_loading, worktree.review_error)
            }
            if (repository.record.id == g_perf_history.repository_id
                    && worktree.path == g_perf_history.worktree_path) {
                perf_observe_worktree(g_perf_history, "history",
                    worktree.history_loading, worktree.history_error)
            }
            if (repository.record.id == g_perf_refs.repository_id
                    && worktree.path == g_perf_refs.worktree_path) {
                perf_observe_worktree(g_perf_refs, "refs",
                    worktree.refs_loading, worktree.refs_error)
            }
            if (repository.record.id == g_perf_commit.repository_id
                    && worktree.path == g_perf_commit.worktree_path) {
                perf_observe_worktree(g_perf_commit, "commit_files",
                    worktree.history_loading, worktree.history_error)
            }
        }
    }
}

// UI trigger forensics: every selection-changing event records who fired it,
// on which frame, and where the mouse was. Read via herder_ui_events.
var g_ui_events : array<string>
var g_ui_frame = 0
var g_ui_tab_signature = ""

def ui_event(tag : string) {
    let mouse = GetMousePos()
    g_ui_events |> push("frame={g_ui_frame} mouse=({int(mouse.x)},{int(mouse.y)}) " +
        "down={IsMouseDown(ImGuiMouseButton.Left)} {tag}")
    if (length(g_ui_events) > 96) {
        g_ui_events |> erase(0, length(g_ui_events) - 96)
    }
}

var g_terminal = Terminal()

var g_terminal_view = ImGuiTerminalState()

var g_terminal_cache = ImGuiTerminalCache()

var g_terminal_font : ImFont?

var g_fonts = FontRoleSet()

var g_font_sources : array<string>

var g_zoom_percent = 150

var g_status = "Starting"

var g_url : string

var g_token : string

var g_selected_worktree_path : string

var g_selected_repository_id : string

// True once the selection came from a deliberate pick (worktree row click,
// focus-target navigation, select rail) rather than a seed or auto-aim.
// Note-9 policy: attaches may re-aim only while the selection is not explicit.
var g_selection_explicit : bool

// Cross-module window-raise request (same pattern as g_open_terminal_pending):
// surfaces that can't see SESSIONS_WIN set this; rich_client's frame loop acts.
var g_open_sessions_pending : bool

// Output-age tracking (note 29): wall time of the last observed output_bytes
// delta per session. Only a witnessed CHANGE records a time — the first
// sighting after connect proves nothing about freshness.
var g_session_output_seen : table<string; int64>
var g_session_output_wall : table<string; int64>

// Client error ring (note 28): the menu chrome shows only the latest error;
// dismissed ones land here (newest last, capped).
var g_error_history : array<string>
var g_error_history_wall : array<int64>
var g_last_seen_error : string

var g_agent_source_worktree_path : string

var g_agent_source_repository_id : string

var g_agent_worktree_name : string

var g_layout_probe_complete = false

var g_layout_force_default = false

var g_focus_terminal_pending = false
// Cross-module window requests: lower layers (net, sessions) cannot write the
// owning UI module's dock-window widget states directly.
var g_open_terminal_pending = false
var g_open_inspector_pending = false
var g_open_git_windows_pending = false
var g_dock_reset_pending = false
var g_show_exited = false
var g_show_tasks = false
var g_layout_preset = 0
var g_layout_switch_pending = -1
var g_layout_factory_reset = false

// Per-preset layout profiles: each preset keeps the user's tuned dock
// arrangement in its own ini beside the global configuration.
def layout_ini_path(preset : int) : string {
    let dir = dir_name(repository_default_config_path())
    return path_join(dir, preset == 1
        ? "dasHerd.layout.review.ini" : "dasHerd.layout.session.ini")
}

def layout_choice_path() : string {
    return path_join(dir_name(repository_default_config_path()),
        "dasHerd.layout.current")
}

// ===== UI settings (write-through) =====
// Every change saves immediately — a crash must never lose settings.

struct HerdUiSettings {
    version : int = 1
    // Matches the g_zoom_percent shipped default.
    ui_zoom_percent : int = 150
    show_exited : bool = false
    show_tasks : bool = false
    layout_preset : int = 0
    window_zoom : table<string; int>
    // Project tab: sort mode (0 folders-first, 1 name, 2 largest, 3 newest)
    // and the optional size/modified columns.
    project_sort : int = 0
    project_show_size : bool = false
    project_show_date : bool = false
}

var g_ui_settings = HerdUiSettings()

def settings_path() : string {
    return path_join(dir_name(repository_default_config_path()), "dasHerd.ui.json")
}

def settings_save() {
    let _ = fwrite(settings_path(), sprint_json(g_ui_settings, true))
}

def settings_load() {
    if (fexist(settings_path())) {
        var inscope loaded = HerdUiSettings()
        if (sscan_json(fread(settings_path()), loaded) && loaded.version == 1) {
            g_ui_settings <- loaded
        }
    } elif (fexist(layout_choice_path())) {
        // Migrate the pre-settings layout choice file once.
        g_ui_settings.layout_preset = clamp(to_int(strip(fread(layout_choice_path()))), 0, 1)
    }
    g_zoom_percent = clamp(g_ui_settings.ui_zoom_percent, 50, 300)
    g_layout_preset = clamp(g_ui_settings.layout_preset, 0, 1)
    g_show_exited = g_ui_settings.show_exited
    g_show_tasks = g_ui_settings.show_tasks
}
// Deferred terminal submit: Ink-based TUIs (Claude Code) treat an Enter that
// arrives in the same paste window as the text as part of the paste. The say
// rails send the text, stamp this deadline, and the frame loop sends the
// lone \r once ~200ms have passed.
var g_submit_stamp : int64
var g_submit_session : string

// Say requests that arrive while an Enter is still pending: sending their text
// immediately would merge it into the prior paste and garble both commands, so
// they wait here until the frame loop flushes the pending \r.
struct PendingSay {
    text : string
    submit : bool
    session : string
}
var g_input_queue : array<PendingSay>

// A blocked worktree delete (in_use / dirty / main / error). The frame loop
// opens the block modal when active; "dirty" adds the offer to launch a
// resolver session briefed with the blocking state.
struct WorktreeDeleteBlock {
    active : bool
    repository_id : string
    worktree_path : string
    reason : string
    detail : string
}
var g_worktree_delete_block = WorktreeDeleteBlock()

// Deferred launcher open for the resolver offer: open_popup from inside the
// block modal would nest the launcher under the popup being closed, so the
// button preps the launcher fields and the frame loop opens it at root level.
var g_launch_resolver_pending = false

// Real-input forensics: per-frame counters of wheel/click events that reached
// ImGui IO. With user control locked these must stay flat — a moving counter
// is a leak through the lock (note 38).
var g_wheel_events_seen : int
var g_wheel_last_delta : float
var g_click_events_seen : int

// Launch debounce: a click stamps the herd revision; the button stays
// disabled until the watcher's herd broadcast bumps it (or a timeout passes),
// so duplicate/replayed clicks cannot create duplicate sessions.
var g_launch_pending_revision : int64 = -1l
var g_launch_pending_started : int64

// Project tab: full project structure of the selected worktree.
var g_worktree_files : array<string>
var g_worktree_files_identity = ""
var g_worktree_files_loading = false
var g_worktree_files_error = ""
var g_files_expanded : table<string; bool>

def session_is_task(kind : string) : bool {
    return starts_with(kind, "git-") || kind == "worktree-create"
}

var g_active_focus_message = ""

var g_active_focus_target = -1

var g_active_focus_range = -1

var g_focus_visible = true

var g_pending_focus_scroll = false

var g_focus_view_scroll_pending = false

var g_focus_view_scroll_y = 0.0f

var g_last_human_request = ""

var g_applied_focus_identity = ""

var g_requested_history_commit : string

var g_pending_clipboard_sha : string

var g_last_copied_sha : string

var g_clipboard_copy_status = ClipboardStatus.no_content

var g_clipboard_copy_attempts = 0

var g_clipboard_copy_verified = false

var g_tree_model = GitTreeModel()

var g_tree_model_identity : string

var g_tree_focus_ref : string

var g_inspector = GitInspectorState()

var g_prepare_worker = InspectorPrepareWorker()

var g_jobque_owned = false

def option_value(prefix : string; fallback : string = "") : string {
    for (argument in get_command_line_arguments()) {
        if (starts_with(argument, prefix)) return slice(argument, length(prefix))
    }
    return fallback
}

def reset_terminal(columns : int = 100; rows : int = 30) {
    terminal_dispose(g_terminal)
    terminal_viewport_snapshot_dispose(g_terminal_cache.snapshot)
    var inscope created <- terminal_create(columns, rows)
    g_terminal := created
    g_terminal_view = ImGuiTerminalState()
    g_terminal_cache = ImGuiTerminalCache()
}

def feed_terminal_frame(message : string#; byte_count : int) {
    var inscope bytes : array<uint8>
    bytes |> resize(byte_count)
    if (byte_count > 0) {
        unsafe { memcpy(addr(bytes[0]), reinterpret<void?>(message), byte_count) }
        terminal_feed_bytes(g_terminal, bytes)
    }
}

def upload_inspector_preview(event : InspectorPrepareEvent#) {
    g_inspector.preview_width = event.preview_width
    g_inspector.preview_height = event.preview_height
    g_inspector.preview_byte_count = event.preview_byte_count
    g_inspector.preview_error = clone_string(event.preview_error)
    if (empty(event.preview_pixels)) return
    inspector_preview_texture_upload(event.preview_pixels,
        event.preview_width, event.preview_height,
        g_inspector.preview_texture)
}

def release_inspector_content() {
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

def queue_inspector_prepare(status : string) {
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

def install_file_inspection(message : WatcherFileInspectionMessage const) {
    if (message.request_id != g_inspector.request_id) return
    g_inspector.task_session_id = message.task_session_id
    g_inspector.loading = message.loading
    g_inspector.status = message.status
    if (message.loading) return
    g_inspector.error = message.error
    if (!empty(message.error)) {
        inspector_load_fail(g_inspector.prepare,
            g_inspector.prepare.generation, message.error)
        perf_record("inspect_error", message.file_path,
            g_perf_inspect_started, message.error)
        g_perf_inspect_started = 0l
        return
    }
    perf_record("inspect_transport", message.file_path,
        g_perf_inspect_started, "task={message.task_session_id}")
    g_inspector.source_patch = clone_string(message.patch)
    g_inspector.source_view_text = clone_string(message.view_text)
    g_inspector.source_view_base64 = clone_string(message.view_base64)
    g_inspector.source_view_byte_count = message.view_byte_count
    g_inspector.source_view_available = message.view_available
    queue_inspector_prepare("Preparing diff...")
}

def drain_inspector_prepare_events() {
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
            g_inspector.pending_widget_sync = true
            if (g_inspector.pending_select_diff) {
                let clean_view = (empty(g_inspector.parsed.hunks)
                    && !g_inspector.parsed.binary && g_inspector.view_prepared)
                g_inspector.mode = (clean_view
                    ? GitInspectorMode.view : g_inspector.sticky_mode)
                if (g_inspector.mode == GitInspectorMode.view) {
                    g_inspector.pending_select_view_tab = true
                } else {
                    g_inspector.pending_select_diff_tab = true
                }
            }
            g_inspector.pending_select_diff = false
            upload_inspector_preview(
                unsafe(reinterpret<InspectorPrepareEvent const#>(event)))
            g_inspector.error = ""
            g_inspector.prepare_elapsed_usec = event.elapsed_usec
            perf_record("inspect_ready", g_inspector.installed_file_path,
                g_perf_inspect_started,
                "prep_ms={event.elapsed_usec / 1000l}")
            g_perf_inspect_started = 0l
        }
    }
}

def begin_file_inspection(repository_id, worktree_path, file_path,
                          comparison, diff_context : string;
                          git_revision : string = "") {
    let same_identity = g_inspector.repository_id == repository_id && g_inspector.worktree_path == worktree_path && g_inspector.file_path == file_path && g_inspector.comparison == comparison && g_inspector.git_revision == git_revision
    ui_event("inspect {comparison}:{file_path}")
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
        g_inspector.mode = g_inspector.sticky_mode
        g_inspector.pending_widget_sync = true
        if (g_inspector.mode == GitInspectorMode.view) {
            g_inspector.pending_select_view_tab = true
        } else {
            g_inspector.pending_select_diff_tab = true
        }
    }
}

let UI_BASE_FONT_SIZE = 14.0f

def git_visible_row_right(row_right : float) : float {
    let style & = GetStyle()
    let window_right = GetWindowPos().x + GetWindowSize().x - style.WindowPadding.x - style.ScrollbarSize
    return min(row_right, window_right)
}

def draw_git_path_text(path : string; p0, p1 : float2;
                       subdued : bool) {
    let alpha = subdued ? 0.55f : 1.0f
    let directory_color = GetColorU32(token_path_dir_color(alpha))
    let separator_color = GetColorU32(token_path_sep_color(alpha))
    let name_color = GetColorU32(token_path_name_color(alpha))
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

def window_zoom_percent(name : string) : int {
    return clamp(g_ui_settings.window_zoom?[name] ?? 100, 50, 300)
}

def set_window_zoom(name : string; percent : int) {
    let clamped = clamp(percent, 50, 300)
    if (window_zoom_percent(name) == clamped) return
    g_ui_settings.window_zoom[name] = clamped
    settings_save()
}

// Per-window zoom (notes 12/24): wraps a dock window's content in a resized
// font (children inherit the push) and claims Ctrl+wheel for THIS window
// only. The base UI zoom lives in Settings; the two compose.
def window_zoom_scope(name : string; blk : block) {
    var io & = unsafe(GetIO())
    if (io.KeyCtrl && io.MouseWheel != 0.0f
            && IsWindowHovered(ImGuiHoveredFlags.RootAndChildWindows)) {
        set_window_zoom(name, window_zoom_percent(name)
            + (io.MouseWheel > 0.0f ? 10 : -10))
        io.MouseWheel = 0.0f
    }
    let percent = window_zoom_percent(name)
    if (percent == 100) {
        invoke(blk)
    } else {
        with_font(null, UI_BASE_FONT_SIZE * float(percent) * 0.01f) {
            invoke(blk)
        }
    }
}

def set_ui_zoom(percent : int) {
    g_zoom_percent = clamp(percent, 50, 300)
    if (g_ui_settings.ui_zoom_percent != g_zoom_percent) {
        g_ui_settings.ui_zoom_percent = g_zoom_percent
        settings_save()
    }
}

def zoom_by(delta : int) {
    set_ui_zoom(g_zoom_percent + delta)
}

def first_focus_range(target : WatcherFocusTarget const) : int {
    var result = -1
    for (index in range(length(target.ranges))) {
        if (result < 0 || target.ranges[index].start_byte
                < target.ranges[result].start_byte) {
            result = index
        }
    }
    return result
}

def git_status_byte(code : string; index : int) : int {
    if (index < 0 || index >= length(code)) return ' '
    var result = ' '
    peek_data(code) $(data) {
        result = int(data[index])
    }
    return result
}

def git_conflict(code : string) : bool {
    return (code == "DD" || code == "AU" || code == "UD" || code == "UA"
        || code == "DU" || code == "AA" || code == "UU")
}

def git_file_in_group(file : GitFileState const; group : int) : bool {
    if (group == 0) return git_conflict(file.code)
    if (git_conflict(file.code)) return false
    if (group == 1) return file.code != "??" && git_status_byte(file.code, 0) != ' '
    if (group == 2) return file.code != "??" && git_status_byte(file.code, 1) != ' '
    return file.code == "??"
}

def git_group_scope(group : int) : string {
    return group == 1 ? "staged" : "working"
}

def git_status_file_code(repository : RepositorySnapshot const;
                         worktree_path : string;
                         file_path : string) : string {
    for (file in repository.files) {
        if (file.worktree_path == worktree_path && file.path == file_path) {
            return file.code
        }
    }
    return ""
}

def git_line_stats(added_lines, deleted_lines : int;
                   stats_known, binary : bool) : string {
    if (binary) return "binary"
    if (!stats_known) return "+?  -?"
    return "+{added_lines}  -{deleted_lines}"
}

def service_sha_copy() {
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

def queue_sha_copy(hash : string) {
    g_pending_clipboard_sha = hash
    g_clipboard_copy_attempts = 0
    g_clipboard_copy_verified = false
    service_sha_copy()
}

def git_review_commit_count(repository : RepositorySnapshot const;
                            worktree : WorktreeState const) : int {
    var count = 0
    for (commit in repository.review_commits) {
        if (commit.worktree_path == worktree.path) count++
    }
    return count
}

def git_history_commit_count(repository : RepositorySnapshot const;
                             worktree : WorktreeState const) : int {
    var count = 0
    for (commit in repository.history_commits) {
        if (commit.worktree_path == worktree.path) count++
    }
    return count
}

def git_selected_commit_file_count(repository : RepositorySnapshot const;
                                   worktree : WorktreeState const;
                                   commit_hash : string) : int {
    var count = 0
    for (file in repository.commit_files) {
        if (file.worktree_path == worktree.path
                && file.commit_hash == commit_hash) count++
    }
    return count
}

def append_line_starts(source : string;
                       var starts : array<int>&) {
    delete starts
    starts |> push(0)
    peek_data(source) $(bytes) {
        for (index in range(length(bytes))) {
            if (int(bytes[index]) == '\n') starts |> push(index + 1)
        }
    }
}

def diff_byte_to_view_byte(diff_byte : int;
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

def focus_intersects(target : WatcherFocusTarget const;
                     start_byte, end_byte : int) : bool {
    if (target.whole_file || empty(target.ranges)) return true
    for (focus_range in target.ranges) {
        if (focus_range.end_byte > start_byte
                && focus_range.start_byte < end_byte) return true
    }
    return false
}

def focus_caption(target : WatcherFocusTarget const;
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
