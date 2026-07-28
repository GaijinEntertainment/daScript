options gen2
options rtti
options persistent_heap
options gc
options indenting = 4

module repository_core shared public

require ./watcher_core.das public
require daslib/command_line
require daslib/fio public
require daslib/json_boost public
require daslib/strings_boost
require strings public

let private REPOSITORY_CONFIG_VERSION = 1

// Verdict tiers for retiring a worktree (note 58). in_use and main are hard
// blocks; dirty blocks with escapes; warn is about REDUNDANCY, not risk.
let public WORKTREE_TIER_IN_USE = "in_use"
let public WORKTREE_TIER_MAIN = "main"
let public WORKTREE_TIER_DIRTY = "dirty"
let public WORKTREE_TIER_WARN = "warn"
let public WORKTREE_TIER_GREEN = "green"
let private GIT_TIMEOUT_MS = 15000l
// Keep structured records on one ConPTY row. The current wire format carries
// full hashes, author, subject, and parents; 240 columns wrapped ordinary
// commit metadata into cursor-positioned screen fragments.
let private GIT_CAPTURE_COLUMNS = 400
let private GIT_CAPTURE_ROWS = 512
let private GIT_FIELD_SEPARATOR = "<|dasHerd-field|>"
let private GIT_COMMIT_MARKER = "<|dasHerd-commit|>"
let private UNTRACKED_NUMSTAT_MAX_BYTES = 32 * 1024 * 1024

struct public RepositoryRecord {
    id : string
    target_id : string = "local"
    checkout_path : string
    display_name : string
    base_ref : string
}

struct public GitFileState {
    worktree_path : string
    code : string
    path : string
    old_path : string
    added_lines : int
    deleted_lines : int
    stats_known : bool
    binary : bool
}

struct public GitCommitState {
    worktree_path : string
    hash : string
    short_hash : string
    authored_at : int64
    author : string
    subject : string
    parents : string
}

struct public GitGraphRowState {
    worktree_path : string
    topology : string
    commit_hash : string
}

struct public GitReviewFileState {
    worktree_path : string
    code : string
    path : string
    old_path : string
    added_lines : int
    deleted_lines : int
    stats_known : bool
    binary : bool
}

struct public GitRefState {
    worktree_path : string
    full_name : string
    name : string
    hash : string
    current : bool
    remote : bool
    upstream : string
}

struct public GitCommitFileState {
    worktree_path : string
    commit_hash : string
    code : string
    path : string
    old_path : string
    added_lines : int
    deleted_lines : int
    stats_known : bool
    binary : bool
}

struct public WorktreeState {
    path : string
    head : string
    branch : string
    is_main : bool
    detached : bool
    bare : bool
    locked : bool
    prunable : bool
    ahead : int
    behind : int
    staged : int
    unstaged : int
    untracked : int
    conflicted : int
    // Delete-safety facts (note 58). upstream_ref rides the status branch line
    // for free. unmerged_* measure against the AUTHORITATIVE base, never local
    // master: the same branch reads 45 unmerged against a local master sitting
    // 108 behind, and 0 against origin/master — the convenient base inverts
    // the answer. unmerged_known is false when no base ref could be resolved.
    upstream_ref : string
    unmerged_commits : int
    unmerged_base_ref : string
    unmerged_known : bool
    error : string
    review_base_ref : string
    review_merge_base : string
    review_loading : bool
    review_error : string
    history_ref : string
    history_commit : string
    history_loading : bool
    history_error : string
    refs_loading : bool
    refs_error : string
}

struct public RepositorySnapshot {
    record : RepositoryRecord = RepositoryRecord()
    worktrees : array<WorktreeState>
    files : array<GitFileState>
    review_commits : array<GitCommitState>
    review_files : array<GitReviewFileState>
    history_commits : array<GitCommitState>
    graph_rows : array<GitGraphRowState>
    commit_files : array<GitCommitFileState>
    refs : array<GitRefState>
    refreshing : bool
    error : string
    revision : int64
    last_success_ms : int64
    task_session_id : string
    failure_session_id : string
    failure_message : string
    failure_revision : int64
}

// A user-initiated worktree operation (retire, WIP-commit). These run on the
// SAME task-session rail as Git observation instead of blocking the watcher
// tick: `git worktree remove` deletes a whole tree and `git add -A` walks a
// dirty one, neither of which is the "fast plumbing" the synchronous path was
// written for. The UI stays in its dialog and watches this state.
struct public WorktreeOperation {
    kind : string
    repository_id : string
    worktree_path : string
    // "idle" | "running" | "ok" | "failed" | "cancelled"
    state : string = "idle"
    step : string
    step_index : int
    step_count : int
    error : string
    // What the error MEANS, in words, when we recognise it. An error the user
    // cannot act on is the same as no error at all.
    hint : string
    // Whether trying the same thing again could plausibly work. A retry
    // offered against a permanent failure just wastes the user's time.
    retryable : bool
    // The task session that ran (or is running) the current step, so the UI
    // can open the real terminal and read the whole thing.
    session_id : string
    // Every command and its output, in order — the dialog's collapsed log.
    log : array<string>
    revision : int64
}

struct public RepositoryAddRequest {
    path : string
    display_name : string
    target_id : string = "local"
    // When set, the watcher also attaches the path to this durable session's
    // workspace (agent announce / user add); repository_core itself ignores it.
    herd_session_id : string
}

struct public RepositoryRefreshRequest {
    repository_id : string
}

struct private RepositoryConfigFile {
    version : int = REPOSITORY_CONFIG_VERSION
    repositories : array<RepositoryRecord>
}

class private RepositoryRuntime {
    snapshot : RepositorySnapshot = RepositorySnapshot()
    // Five independent task streams: refresh, review, history, commit files,
    // and refs each own a phase, a hosted session, and a worktree identity.
    // Streams hold worktree PATHS, never indices — an inventory replacement in
    // the refresh stream may reorder snapshot.worktrees mid-flight.
    refresh_phase : int
    refresh_session_id : string
    worktree_index : int
    refresh_pending : bool = true
    rerun_pending : bool
    unmerged_base_ref : string
    unmerged_tried_origin_main : bool
    review_phase : int
    review_session_id : string
    review_pending_path : string
    review_worktree_path : string
    review_base_ref : string
    review_merge_base : string
    review_tried_origin_main : bool
    pending_review_commits : array<GitCommitState>
    pending_review_files : array<GitReviewFileState>
    history_phase : int
    history_session_id : string
    history_pending_path : string
    history_pending_ref : string
    history_worktree_path : string
    history_ref : string
    commit_phase : int
    commit_session_id : string
    commit_pending_path : string
    commit_pending_hash : string
    commit_worktree_path : string
    history_commit : string
    refs_phase : int
    refs_session_id : string
    refs_pending_path : string
    refs_worktree_path : string
    pending_refresh_files : array<GitFileState>
    pending_history_commits : array<GitCommitState>
    pending_graph_rows : array<GitGraphRowState>
    pending_commit_files : array<GitCommitFileState>
    pending_refs : array<GitRefState>
    def operator delete {
        delete snapshot.worktrees
        delete snapshot.files
        delete snapshot.review_commits
        delete snapshot.review_files
        delete snapshot.history_commits
        delete snapshot.graph_rows
        delete snapshot.commit_files
        delete snapshot.refs
        delete pending_review_commits
        delete pending_review_files
        delete pending_refresh_files
        delete pending_history_commits
        delete pending_graph_rows
        delete pending_commit_files
        delete pending_refs
    }
}

var private g_repositories : array<RepositoryRuntime?>
var private g_config_path : string
var private g_revision : int64

// One user operation at a time — a retire is a deliberate single act, and one
// slot is exactly what the one dialog can show.
var private g_operation = WorktreeOperation()
var private g_operation_phase : int
var private g_operation_session_id : string
var private g_operation_force : bool
var private g_operation_message : string

def public repository_default_config_path : string {
    var base : string
    if (has_env_variable("APPDATA")) {
        base = get_env_variable("APPDATA")
    } elif (has_env_variable("XDG_CONFIG_HOME")) {
        base = get_env_variable("XDG_CONFIG_HOME")
    } elif (has_env_variable("HOME")) {
        base = path_join(get_env_variable("HOME"), ".config")
    } else {
        base = "."
    }
    return path_join(path_join(base, "daScript"), "dasHerd.json")
}

def private valid_path(path : string) : bool {
    return !empty(path) && find(path, "\r") < 0 && find(path, "\n") < 0
}

def private normalized_path(path : string) : string {
    return get_full_file_name(path)
}

def public repository_same_worktree_identity(left, right : string) : bool {
    //! One path identity for guard checks: full-name normalized, case folded
    //! only on Windows - a case-sensitive filesystem holds case-differing
    //! paths apart (the same platform gate find_worktree uses).
    let left_identity = (get_platform_name() == "windows"
        ? to_lower(normalized_path(left)) : normalized_path(left))
    let right_identity = (get_platform_name() == "windows"
        ? to_lower(normalized_path(right)) : normalized_path(right))
    return left_identity == right_identity
}

def private repository_id(target_id, path : string) : string {
    let identity_path = get_platform_name() == "windows" ? to_lower(path) : path
    return "{target_id}:{identity_path}"
}

def public repository_id_for_path(path : string) : string {
    return repository_id("local", normalized_path(path))
}

def public repository_valid_relative_file_path(path : string) : bool {
    if (!valid_path(path) || starts_with(path, "/") || starts_with(path, "\\")
            || is_absolute(path) || (get_platform_name() == "windows"
                && length(path) >= 2 && byte_at(path, 1) == ':')) return false
    return (path != ".." && !ends_with(path, "/..") && !ends_with(path, "\\..")
        && find(path, "../") < 0 && find(path, "..\\") < 0)
}

def private changed(var runtime : RepositoryRuntime?) {
    runtime.snapshot.revision++
    g_revision++
}

def private save_config : string {
    var inscope config = RepositoryConfigFile()
    config.repositories |> reserve(length(g_repositories))
    for (runtime in g_repositories) {
        if (runtime != null) config.repositories |> push(runtime.snapshot.record)
    }
    var error : string
    if (!mkdir_rec(dir_name(g_config_path), error)) return "could not create config directory: {error}"
    if (!fwrite(g_config_path, sprint_json(config, true))) return "could not write {g_config_path}"
    return ""
}

def private add_runtime(record : RepositoryRecord) {
    g_repositories |> push(new RepositoryRuntime(
        snapshot = RepositorySnapshot(record = record)))
    g_revision++
}

def private load_config : string {
    if (!fexist(g_config_path)) return ""
    var inscope config = RepositoryConfigFile()
    if (!sscan_json(fread(g_config_path), config)) return "invalid repository config JSON"
    if (config.version != REPOSITORY_CONFIG_VERSION) {
        return "unsupported repository config version {config.version}"
    }
    for (record in config.repositories) {
        if (valid_path(record.checkout_path)) add_runtime(record)
    }
    return ""
}

def public repository_init(config_path : string; bootstrap_checkout : string) : string {
    g_config_path = config_path
    let load_error = load_config()
    if (!empty(load_error)) return load_error
    if (empty(g_repositories) && valid_path(bootstrap_checkout)) {
        let path = normalized_path(bootstrap_checkout)
        add_runtime(RepositoryRecord(id = repository_id("local", path), target_id = "local",
            checkout_path = path, display_name = base_name(path)))
        return save_config()
    }
    return ""
}

def public repository_shutdown() {
    for (runtime in g_repositories) {
        if (runtime != null) {
            unsafe { delete runtime }
        }
    }
    g_repositories |> clear()
    unsafe { delete g_repositories }
    g_config_path = ""
    g_revision = 0l
}

def public repository_revision : int64 {
    return g_revision
}

def public repository_list_json : string {
    var inscope parts : array<string>
    parts |> reserve(length(g_repositories))
    for (runtime in g_repositories) {
        if (runtime != null) parts |> push(sprint_json(runtime.snapshot, false))
    }
    return "[" + join(parts, ",") + "]"
}

def private find_repository(id : string) : int {
    for (index in range(length(g_repositories))) {
        if (g_repositories[index] != null && g_repositories[index].snapshot.record.id == id) return index
    }
    return -1
}

def public repository_add(request : RepositoryAddRequest) : string {
    if (request.target_id != "local" && !empty(request.target_id)) return "only the local target is implemented"
    if (!valid_path(request.path)) return "repository path is required and cannot contain CR/LF"
    let path = normalized_path(request.path)
    let id = repository_id("local", path)
    if (find_repository(id) >= 0) return "repository is already registered"
    add_runtime(RepositoryRecord(id = id, target_id = "local", checkout_path = path,
        display_name = !empty(request.display_name) ? request.display_name : base_name(path)))
    let error = save_config()
    if (!empty(error)) return error
    return ""
}

def public repository_refresh(id : string) : string {
    let index = find_repository(id)
    if (index < 0) return "unknown repository"
    var runtime = g_repositories[index]
    if (runtime.refresh_phase != 0) {
        runtime.rerun_pending = true
    } else {
        runtime.refresh_pending = true
    }
    return ""
}

def public repository_phase_family(phase : int) : string {
    if (phase == 10 || phase < 3) return "refresh"
    if (phase >= 7) return "history"
    return "review"
}

def public repository_build_file_diff_argv(worktree_path : string;
                                           file : GitFileState const;
                                           comparison : string;
                                           var argv : array<string>&;
                                           var expected_exit_code : int64&;
                                           diff_context : string = "compact") : string {
    delete argv
    expected_exit_code = 0l
    let staged = file.code != "??" && byte_at(file.code, 0) != ' '
    if (comparison == "staged" && !staged) return "file has no staged change"
    if (comparison != "staged" && comparison != "working") {
        return "comparison is not implemented"
    }
    if (!repository_valid_relative_file_path(file.path)) {
        return "invalid worktree-relative file path"
    }
    var context_lines = 3
    if (diff_context == "expanded") {
        context_lines = 20
    } elif (diff_context == "full") {
        context_lines = 1000000
    } elif (diff_context != "compact") {
        return "diff context is not implemented"
    }
    var inscope common <- ["git", "--literal-pathspecs", "-C", worktree_path,
        "--no-pager", "-c", "color.ui=false", "diff"]
    argv := common
    if (comparison == "staged") argv |> push("--cached")
    if (comparison == "working" && file.code == "??") {
        argv |> push("--no-index")
        expected_exit_code = 1l
    }
    argv |> push("--no-ext-diff")
    argv |> push("--unified={context_lines}")
    argv |> push("--")
    if (comparison == "working" && file.code == "??") {
        argv |> push(get_platform_name() == "windows" ? "NUL" : "/dev/null")
    }
    argv |> push(file.path)
    return ""
}

def public repository_build_worktree_create_argv(source_worktree_path,
                                                 new_worktree_path,
                                                 branch, base_ref : string;
                                                 var argv : array<string>) : string {
    if (!valid_path(source_worktree_path) || empty(strip(source_worktree_path))) {
        return "source worktree path is required"
    }
    if (!valid_path(new_worktree_path) || empty(strip(new_worktree_path))
            || !is_absolute(new_worktree_path)) {
        return "new worktree path must be absolute"
    }
    if (!valid_path(branch)) return "new branch is required"
    let normalized_branch = strip(branch)
    if (empty(normalized_branch)) return "new branch is required"
    if (!empty(base_ref) && !valid_path(base_ref)) return "base ref cannot contain CR/LF"
    let normalized_base = strip(base_ref)
    let resolved_base = !empty(normalized_base) ? normalized_base : "HEAD"
    argv <- ["git", "-C", source_worktree_path, "worktree", "add", "-b",
        normalized_branch, "--", new_worktree_path, resolved_base]
    return ""
}

def private record_git_failure(var runtime : RepositoryRuntime?;
                               session_id, message : string) {
    runtime.snapshot.failure_session_id = session_id
    runtime.snapshot.failure_message = message
    runtime.snapshot.failure_revision++
}

def public repository_record_task_failure(repository_id, session_id,
                                          message : string) : string {
    let index = find_repository(repository_id)
    if (index < 0 || g_repositories[index] == null) return "unknown repository"
    var runtime = g_repositories[index]
    runtime.snapshot.failure_session_id = session_id
    runtime.snapshot.failure_message = message
    runtime.snapshot.failure_revision++
    changed(runtime)
    return ""
}

def public repository_build_review_file_diff_argv(worktree_path : string;
                                                  merge_base : string;
                                                  file_path : string;
                                                  var argv : array<string>&;
                                                  diff_context : string = "compact") : string {
    delete argv
    if (empty(merge_base)) return "worktree review has no merge base"
    if (!repository_valid_relative_file_path(file_path)) {
        return "invalid worktree-relative file path"
    }
    var context_lines = 3
    if (diff_context == "expanded") {
        context_lines = 20
    } elif (diff_context == "full") {
        context_lines = 1000000
    } elif (diff_context != "compact") {
        return "diff context is not implemented"
    }
    var inscope command <- ["git", "--literal-pathspecs", "-C", worktree_path,
        "--no-pager", "-c", "color.ui=false", "diff", "--cached",
        "--no-ext-diff", "--unified={context_lines}", merge_base, "--", file_path]
    argv := command
    return ""
}

def public repository_build_commit_file_diff_argv(worktree_path : string;
                                                  commit_hash : string;
                                                  file_path : string;
                                                  var argv : array<string>&;
                                                  diff_context : string = "compact") : string {
    delete argv
    if (empty(commit_hash)) return "commit hash is required"
    if (!repository_valid_relative_file_path(file_path)) {
        return "invalid worktree-relative file path"
    }
    var context_lines = 3
    if (diff_context == "expanded") {
        context_lines = 20
    } elif (diff_context == "full") {
        context_lines = 1000000
    } elif (diff_context != "compact") {
        return "diff context is not implemented"
    }
    var inscope command <- ["git", "--literal-pathspecs", "-C", worktree_path,
        "--no-pager", "-c", "color.ui=false", "show", "--format=",
        "--first-parent", "--find-renames", "--no-ext-diff", "--unified={context_lines}",
        commit_hash, "--", file_path]
    argv := command
    return ""
}

def public repository_build_file_action_argv(worktree_path : string;
                                             file_path : string;
                                             action : string;
                                             var argv : array<string>&) : string {
    delete argv
    if (!repository_valid_relative_file_path(file_path)) {
        return "invalid worktree-relative file path"
    }
    if (action == "stage") {
        var inscope command <- ["git", "--literal-pathspecs", "-C", worktree_path,
            "add", "--", file_path]
        argv := command
    } elif (action == "unstage") {
        var inscope command <- ["git", "--literal-pathspecs", "-C", worktree_path,
            "reset", "--quiet", "HEAD", "--", file_path]
        argv := command
    } else {
        return "file action is not implemented"
    }
    return ""
}

def public repository_file_action_argv(repository_id : string;
                                       worktree_path : string;
                                       file_path : string;
                                       action : string;
                                       var argv : array<string>&) : string {
    delete argv
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    let runtime = g_repositories[repository_index]
    if (find_worktree(runtime, worktree_path) < 0) return "unknown worktree"
    var file_index = -1
    for (index in range(length(runtime.snapshot.files))) {
        let file = runtime.snapshot.files[index]
        if (file.worktree_path == worktree_path && file.path == file_path) {
            file_index = index
            break
        }
    }
    if (file_index < 0) return "file is not present in the observed Git status"
    let code = runtime.snapshot.files[file_index].code
    let staged = code != "??" && byte_at(code, 0) != ' '
    let working = code == "??" || byte_at(code, 1) != ' '
    if (action == "stage" && !working) return "file has no working-tree change"
    if (action == "unstage" && !staged) return "file has no staged change"
    return repository_build_file_action_argv(
        worktree_path, file_path, action, argv)
}

def public repository_file_diff_argv(repository_id : string;
                                     worktree_path : string;
                                     file_path : string;
                                     comparison : string;
                                     var argv : array<string>&;
                                     var expected_exit_code : int64&;
                                     diff_context : string = "compact";
                                     revision : string = "") : string {
    delete argv
    expected_exit_code = 0l
    if (comparison == "commit" && empty(revision)) {
        return "commit revision is required"
    }
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    let runtime = g_repositories[repository_index]
    let worktree_index = find_worktree(runtime, worktree_path)
    if (worktree_index < 0) return "unknown worktree"

    if (comparison == "pr") {
        var review_file_found = false
        for (file in runtime.snapshot.review_files) {
            if (file.worktree_path == worktree_path && file.path == file_path) {
                review_file_found = true
                break
            }
        }
        if (!review_file_found) return "file is not present in the worktree PR result"
        return repository_build_review_file_diff_argv(
            worktree_path,
            runtime.snapshot.worktrees[worktree_index].review_merge_base,
            file_path, argv, diff_context)
    }

    if (comparison == "commit") {
        var commit_file_found = false
        for (file in runtime.snapshot.commit_files) {
            if (file.worktree_path == worktree_path
                    && file.commit_hash == revision && file.path == file_path) {
                commit_file_found = true
                break
            }
        }
        if (!commit_file_found) return "file is not present in the selected commit"
        return repository_build_commit_file_diff_argv(
            worktree_path, revision, file_path, argv, diff_context)
    }

    var file_index = -1
    for (index in range(length(runtime.snapshot.files))) {
        let file = runtime.snapshot.files[index]
        if (file.worktree_path == worktree_path && file.path == file_path) {
            file_index = index
            break
        }
    }
    if (file_index < 0) {
        if (comparison != "working") {
            return "file is not present in the observed Git status"
        }
        var absolute_path = ""
        let path_error = repository_working_preview_path(
            worktree_path, file_path, absolute_path)
        if (!empty(path_error)) return path_error
        let clean_file = GitFileState(
            worktree_path = worktree_path, code = "  ", path = file_path)
        return repository_build_file_diff_argv(
            worktree_path, clean_file, comparison, argv, expected_exit_code,
            diff_context)
    }
    let file = runtime.snapshot.files[file_index]
    return repository_build_file_diff_argv(
        worktree_path, file, comparison, argv, expected_exit_code,
        diff_context)
}

def public repository_read_text_file(path : string; max_bytes : int;
                                     var text : string&) : string {
    text = ""
    let file_status = stat(path)
    if (!file_status.is_valid || !file_status.is_reg) {
        return "could not read file text"
    }
    var loaded = false
    var too_large = false
    fopen(path, "rb") $(file) {
        if (file != null) {
            fmap(file) $(var data : array<uint8>#) {
                if (length(data) <= max_bytes) {
                    text = string(data)
                } else {
                    too_large = true
                }
                loaded = true
            }
        }
    }
    if (too_large) return "file exceeds the raw-byte View limit"
    return loaded ? "" : "could not read file text"
}

def public repository_read_file_bytes(path : string; max_bytes : int;
                                      var bytes : array<uint8>;
                                      var byte_count : int&) : string {
    delete bytes
    byte_count = 0
    let file_status = stat(path)
    if (!file_status.is_valid || !file_status.is_reg) {
        return "could not read file bytes"
    }
    var loaded = false
    fopen(path, "rb") $(file) {
        if (file != null) {
            fmap(file) $(var data : array<uint8>#) {
                byte_count = length(data)
                if (byte_count <= max_bytes) {
                    bytes := data
                }
                loaded = true
            }
        }
    }
    return loaded ? "" : "could not read file bytes"
}

def public repository_working_preview_path(worktree_path : string;
                                           file_path : string;
                                           var absolute_path : string&) : string {
    absolute_path = ""
    if (!repository_valid_relative_file_path(file_path)) {
        return "invalid worktree-relative file path"
    }
    let candidate = path_join(worktree_path, file_path)
    if (!fexist(candidate)) return "working file does not exist"
    var current = worktree_path
    var inscope components <- split(replace(file_path, "\\", "/"), "/")
    for (component in components) {
        if (empty(component) || component == ".") continue
        current = path_join(current, component)
        var symlink_error = ""
        if (is_symlink(current, symlink_error)) {
            return "working file path contains a symlink"
        }
        if (!empty(symlink_error)) {
            return "could not inspect working file path: {symlink_error}"
        }
    }
    absolute_path = candidate
    return ""
}

def private find_worktree(runtime : RepositoryRuntime?; path : string) : int {
    // Canonical compare: herd records, session origins, and git output can
    // disagree on slash direction and drive-letter case for the same tree
    // ("D:/work" vs "D:/Work" seen live); exact equality silently missed.
    let wanted = (get_platform_name() == "windows"
        ? to_lower(normalized_path(path)) : normalized_path(path))
    for (index in range(length(runtime.snapshot.worktrees))) {
        let candidate = (get_platform_name() == "windows"
            ? to_lower(normalized_path(runtime.snapshot.worktrees[index].path))
            : normalized_path(runtime.snapshot.worktrees[index].path))
        if (candidate == wanted) return index
    }
    return -1
}

def private git_capture(argv : array<string>; var output : string&) : int {
    // Synchronous git for the guarded worktree-delete path only: fast plumbing
    // calls on a user click, never the polling rails. 30s cap so a wedged git
    // (index lock) can't hang the watcher tick forever.
    output = ""
    return unsafe(popen_argv(argv, 30.0, $(file) {
        if (file != null) {
            output = unsafe(fread_to_eof(file))
        }
    }))
}

def public repository_remove_worktree(repository_id, worktree_path : string;
                                      var reason : string&;
                                      var detail : string&;
                                      force : bool = false) : bool {
    //! Guarded removal. reason on failure: "unknown" | "main" | "dirty" |
    //! "error"; for "dirty" the detail carries `git status --porcelain` so the
    //! client can brief a resolver session. force skips the dirty guard — the
    //! explicit "delete anyway, N files lost" escape, never a default.
    reason = ""
    detail = ""
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        reason = "unknown"
        detail = "unknown repository"
        return false
    }
    var runtime = g_repositories[repository_index]
    let checkout = runtime.snapshot.record.checkout_path
    if (repository_same_worktree_identity(worktree_path, checkout)) {
        reason = "main"
        detail = "the main checkout cannot be deleted"
        return false
    }
    let worktree_index = find_worktree(runtime, worktree_path)
    if (worktree_index >= 0 && runtime.snapshot.worktrees[worktree_index].is_main) {
        reason = "main"
        detail = "the main checkout cannot be deleted"
        return false
    }
    if (!force) {
        var status_text : string
        let status_exit = git_capture(["git", "-C", worktree_path,
            "status", "--porcelain"], status_text)
        if (status_exit != 0) {
            reason = "error"
            detail = "git status exited {status_exit}"
            return false
        }
        if (!empty(strip(status_text))) {
            reason = "dirty"
            detail = (length(status_text) > 2000
                ? slice(status_text, 0, 2000) + "\n..." : status_text)
            return false
        }
    }
    var remove_text : string
    let remove_argv <- (force
        ? ["git", "-C", checkout, "worktree", "remove", "--force", worktree_path]
        : ["git", "-C", checkout, "worktree", "remove", worktree_path])
    let remove_exit = git_capture(remove_argv, remove_text)
    if (remove_exit != 0) {
        reason = "error"
        detail = (empty(strip(remove_text))
            ? "git worktree remove exited {remove_exit}" : strip(remove_text))
        return false
    }
    let _ = repository_refresh(repository_id)
    return true
}

def public repository_operation_revision : int64 => g_operation.revision

def public repository_operation_json : string => sprint_json(g_operation, false)

def private operation_touch() {
    g_operation.revision++
}

def private operation_log(line : string) {
    if (empty(strip(line))) return
    g_operation.log |> push(line)
}

// Git's own words are the truth, but they are rarely the ANSWER. Each pattern
// here turns a recognised failure into what the user should do about it, and
// says whether trying again could possibly help.
def public repository_classify_failure(message : string; var hint : string&;
                                       var retryable : bool&) {
    classify_failure(message, hint, retryable)
}

def private classify_failure(message : string; var hint : string&;
                             var retryable : bool&) {
    hint = ""
    retryable = false
    let lower = to_lower(message)
    if (find(lower, "should i try again") >= 0
            || find(lower, "failed to delete") >= 0
            || find(lower, "could not remove") >= 0) {
        // Seen live: git emptied the folder, then could not remove the folder
        // itself, so the worktree stayed registered. That is always a handle.
        hint = ("The folder is locked by another process — something still " +
            "has it open: a shell whose current directory is inside it, an " +
            "editor, or a running build. Git deleted the CONTENTS but could " +
            "not remove the folder, so the worktree is still registered. " +
            "Close whatever holds it first; retrying before that will fail " +
            "the same way.")
        retryable = true
        return
    }
    if (find(lower, "could not be deleted") >= 0) {
        hint = ("Git's side is clean now — the worktree is no longer " +
            "registered. What is left is an empty folder that Windows will " +
            "not delete because a process still holds it open. Close that " +
            "process and remove the folder by hand, or hand this to a " +
            "session.")
        return
    }
    if (find(lower, "validation failed") >= 0) {
        hint = ("Git refuses to remove a worktree whose .git is already gone, " +
            "even with --force. The registration has to be pruned instead.")
        retryable = true
        return
    }
    if (find(lower, "index.lock") >= 0) {
        hint = ("Another git process is holding this repository's index lock. " +
            "Wait for it to finish, then retry.")
        retryable = true
        return
    }
    if (find(lower, "is dirty") >= 0 || find(lower, "contains modified") >= 0
            || find(lower, "untracked") >= 0) {
        hint = ("Git found work in the tree after the check passed — " +
            "something wrote to it in between. Re-open the checklist to see " +
            "the current state.")
        retryable = true
        return
    }
    if (find(lower, "not a working tree") >= 0
            || find(lower, "not a git repository") >= 0) {
        hint = ("The folder is no longer a worktree — its .git is gone, which " +
            "is what a half-finished delete leaves behind. Only the stale " +
            "registration remains; removing it again clears that.")
        retryable = true
        return
    }
    if (find(lower, "exited 1") >= 0 && empty(strip(message))) {
        hint = "Git exited with an error but printed nothing. Open the terminal for the full output."
        retryable = true
    }
}

def private operation_fail(message : string) {
    g_operation.state = "failed"
    g_operation.error = message
    g_operation.step = "failed"
    classify_failure(message, g_operation.hint, g_operation.retryable)
    // A step that failed may have left git sitting at a prompt (the delete
    // retry question does exactly that), and a live git keeps holding the very
    // locks the user is trying to clear. The record and its output survive
    // termination, so the terminal button still works afterwards.
    if (!empty(g_operation_session_id)) {
        let _ = watcher_terminate(g_operation_session_id, "operation failed")
    }
    g_operation_phase = 0
    operation_log("ERROR: {message}")
    operation_touch()
}

def private operation_finish() {
    g_operation.state = "ok"
    g_operation.step = "done"
    g_operation.step_index = g_operation.step_count
    g_operation_phase = 0
    g_operation_session_id = ""
    operation_touch()
    let _ = repository_refresh(g_operation.repository_id)
}

def private operation_launch(argv : array<string>; kind, step : string;
                             step_index : int) {
    g_operation.step = step
    g_operation.step_index = step_index
    operation_log("$ " + join(argv, " "))
    operation_touch()
    let repository_index = find_repository(g_operation.repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        operation_fail("unknown repository")
        return
    }
    var launch_error : string
    g_operation_session_id = launch_git(g_repositories[repository_index], argv,
        kind, g_operation.worktree_path, launch_error)
    // Publish the session id even on failure: the terminal is where the full
    // output lives, and a failed step is exactly when it is wanted.
    g_operation.session_id = g_operation_session_id
    if (empty(g_operation_session_id)) operation_fail(launch_error)
}

def private operation_begin(kind, repository_id, worktree_path : string) : string {
    if (g_operation.state == "running") {
        return "another worktree operation is already running"
    }
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    delete g_operation.log
    g_operation = WorktreeOperation(kind = kind, repository_id = repository_id,
        worktree_path = worktree_path, state = "running", step = "starting",
        step_count = 2, revision = g_operation.revision + 1l)
    g_operation_phase = 1
    return ""
}

def public repository_operation_remove_worktree(repository_id, worktree_path : string;
                                                force : bool) : string {
    //! Starts the guarded removal on the task-session rail. Returns "" when it
    //! STARTED — the outcome arrives through repository_operation_json.
    let repository_index = find_repository(repository_id)
    if (repository_index >= 0 && g_repositories[repository_index] != null) {
        let checkout = g_repositories[repository_index].snapshot.record.checkout_path
        if (repository_same_worktree_identity(worktree_path, checkout)) {
            return "the main checkout cannot be deleted"
        }
        let worktree_index = find_worktree(g_repositories[repository_index],
            worktree_path)
        if (worktree_index >= 0
                && g_repositories[repository_index].snapshot.worktrees[worktree_index].is_main) {
            return "the main checkout cannot be deleted"
        }
    }
    let begin_error = operation_begin("remove", repository_id, worktree_path)
    if (!empty(begin_error)) return begin_error
    g_operation_force = force
    // force skips the dirty check entirely; that IS the escape, and re-asking
    // would only stall the delete the user already accepted the cost of.
    if (force) {
        g_operation.step_count = 1
        g_operation_phase = 2
        operation_launch(["git", "-C",
            g_repositories[find_repository(repository_id)].snapshot.record.checkout_path,
            "worktree", "remove", "--force", worktree_path],
            "git-worktree-remove", "removing worktree (forced)", 1)
    } else {
        operation_launch(["git", "-C", worktree_path, "status", "--porcelain"],
            "git-worktree-precheck", "checking for uncommitted work", 1)
    }
    return ""
}

def public repository_operation_commit_wip(repository_id, worktree_path,
                                           message : string) : string {
    //! Stage-everything-and-commit, on the same rail. Refused on a detached
    //! HEAD, where the commit would be unreachable once the worktree goes.
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    let worktree_index = find_worktree(g_repositories[repository_index],
        worktree_path)
    if (worktree_index < 0) return "unknown worktree"
    if (empty(g_repositories[repository_index].snapshot.worktrees[worktree_index].branch)) {
        return "detached HEAD: a commit here would not be on any branch"
    }
    let begin_error = operation_begin("commit_wip", repository_id, worktree_path)
    if (!empty(begin_error)) return begin_error
    g_operation_message = message
    operation_launch(["git", "-C", worktree_path, "add", "-A"],
        "git-wip-add", "staging every change", 1)
    return ""
}

def public repository_operation_cancel : string {
    //! Cancel is honest about what it can undo: it stops the RUN, and the log
    //! keeps whatever already happened. A completed `git worktree remove` is
    //! not put back.
    if (g_operation.state != "running") return "no operation is running"
    if (!empty(g_operation_session_id)) {
        let _ = watcher_terminate(g_operation_session_id, "cancelled by user")
    }
    g_operation.state = "cancelled"
    g_operation.step = "cancelled"
    g_operation_phase = 0
    g_operation_session_id = ""
    operation_log("cancelled by user")
    operation_touch()
    let _ = repository_refresh(g_operation.repository_id)
    return ""
}

def private advance_operation() {
    if (g_operation_phase == 0 || empty(g_operation_session_id)) return
    let task <- poll_task(g_operation_session_id)
    if (!task.ready) return
    let output = strip(task.output)
    operation_log(output)
    let checkout_index = find_repository(g_operation.repository_id)
    let checkout = (checkout_index >= 0 && g_repositories[checkout_index] != null
        ? g_repositories[checkout_index].snapshot.record.checkout_path : "")
    if (g_operation.kind == "remove") {
        if (g_operation_phase == 1) {
            if (task.exit_code != 0l) {
                // A worktree whose .git is already gone has NO working tree
                // left to protect — which is exactly what the dirty guard
                // exists for. This is the half-removed state a previous failed
                // delete leaves behind, and refusing to clean it up would
                // strand the user forever. Proceed to the forced removal.
                if (find(to_lower(output), "not a git repository") >= 0) {
                    operation_log("working tree is already gone; " +
                        "removing the stale registration")
                    g_operation_phase = 2
                    operation_launch(["git", "-C", checkout, "worktree",
                        "remove", "--force", g_operation.worktree_path],
                        "git-worktree-remove",
                        "removing a worktree whose files are already gone", 2)
                    return
                }
                // Git's own words, not our paraphrase of its exit code.
                operation_fail(empty(output)
                    ? "git status exited {task.exit_code}" : output)
                return
            }
            if (!empty(output)) {
                operation_fail("uncommitted or untracked work is present")
                return
            }
            g_operation_phase = 2
            operation_launch(["git", "-C", checkout, "worktree", "remove",
                g_operation.worktree_path], "git-worktree-remove",
                "removing worktree", 2)
            return
        }
        if (g_operation_phase == 2 && task.exit_code != 0l) {
            // Git refuses to remove a worktree whose .git is already gone,
            // even with --force. `prune` is the command for that state, and
            // it is the only way the app can finish a job a previous failure
            // left half-done. It touches administrative metadata only.
            if (find(to_lower(output), "validation failed") >= 0) {
                operation_log("git cannot remove a worktree with no .git; " +
                    "pruning the stale registration instead")
                g_operation_phase = 3
                g_operation.step_count = 3
                operation_launch(["git", "-C", checkout, "worktree", "prune"],
                    "git-worktree-prune",
                    "pruning the stale registration", 3)
                return
            }
            operation_fail(empty(output)
                ? "git worktree remove exited {task.exit_code}" : output)
            return
        }
        if (task.exit_code != 0l) {
            operation_fail(empty(output)
                ? "git worktree prune exited {task.exit_code}" : output)
            return
        }
        var folder_stat : FStat
        let folder_left = (g_operation_phase == 3
            && stat(g_operation.worktree_path, folder_stat)
            && folder_stat.is_dir)
        if (folder_left) {
            // Registration cleared, folder still there: say exactly that,
            // rather than reporting a success the user can see is not one.
            operation_fail("the stale registration is gone, but the folder " +
                "{g_operation.worktree_path} could not be deleted")
            return
        }
        operation_finish()
        return
    }
    if (g_operation_phase == 1) {
        if (task.exit_code != 0l) {
            operation_fail(empty(output)
                ? "git add exited {task.exit_code}" : output)
            return
        }
        g_operation_phase = 2
        operation_launch(["git", "-C", g_operation.worktree_path, "commit",
            "--no-verify", "-m", g_operation_message], "git-wip-commit",
            "committing to the branch", 2)
        return
    }
    if (task.exit_code != 0l) {
        operation_fail(empty(output) ? "git commit exited {task.exit_code}" : output)
        return
    }
    operation_finish()
}

def public repository_commit_worktree_wip(repository_id, worktree_path,
                                          message : string;
                                          var detail : string&) : bool {
    //! The mechanical rescue before retiring a dirty worktree: stage
    //! everything and commit it to the branch. The branch outlives the
    //! worktree and keeps its name, so nothing needs judgment to be saved.
    //! Refused on a detached HEAD, where the commit would be unreachable the
    //! moment the worktree goes.
    detail = ""
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        detail = "unknown repository"
        return false
    }
    let worktree_index = find_worktree(g_repositories[repository_index],
        worktree_path)
    if (worktree_index < 0) {
        detail = "unknown worktree"
        return false
    }
    if (empty(g_repositories[repository_index].snapshot.worktrees[worktree_index].branch)) {
        detail = "detached HEAD: a commit here would not be on any branch"
        return false
    }
    var add_text : string
    let add_exit = git_capture(["git", "-C", worktree_path, "add", "-A"],
        add_text)
    if (add_exit != 0) {
        detail = (empty(strip(add_text))
            ? "git add exited {add_exit}" : strip(add_text))
        return false
    }
    var commit_text : string
    let commit_exit = git_capture(["git", "-C", worktree_path, "commit",
        "--no-verify", "-m", message], commit_text)
    if (commit_exit != 0) {
        detail = (empty(strip(commit_text))
            ? "git commit exited {commit_exit}" : strip(commit_text))
        return false
    }
    let _ = repository_refresh(repository_id)
    return true
}

def public repository_review(repository_id : string; worktree_path : string) : string {
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    var runtime = g_repositories[repository_index]
    if (find_worktree(runtime, worktree_path) < 0) return "unknown worktree"
    runtime.review_pending_path = worktree_path
    changed(runtime)
    return ""
}

def public repository_history(repository_id : string; worktree_path : string;
                              ref_name : string = "") : string {
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    var runtime = g_repositories[repository_index]
    let worktree_index = find_worktree(runtime, worktree_path)
    if (worktree_index < 0) return "unknown worktree"
    var resolved_ref = ref_name
    if (empty(resolved_ref)) {
        resolved_ref = runtime.snapshot.worktrees[worktree_index].branch
    }
    var allowed = resolved_ref == "__all__" || resolved_ref == runtime.snapshot.worktrees[worktree_index].branch
    for (ref in runtime.snapshot.refs) {
        if (ref.worktree_path == worktree_path && ref.name == resolved_ref) {
            allowed = true
            break
        }
    }
    if (empty(resolved_ref) || !allowed) return "unknown Git ref"
    runtime.history_pending_path = worktree_path
    runtime.history_pending_ref = resolved_ref
    changed(runtime)
    return ""
}

def public repository_commit_files(repository_id : string;
                                   worktree_path : string;
                                   commit_hash : string) : string {
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    var runtime = g_repositories[repository_index]
    if (find_worktree(runtime, worktree_path) < 0) return "unknown worktree"
    var allowed = false
    for (commit in runtime.snapshot.history_commits) {
        if (commit.worktree_path == worktree_path && commit.hash == commit_hash) {
            allowed = true
            break
        }
    }
    if (!allowed) return "commit is not present in the observed history"
    runtime.commit_pending_path = worktree_path
    runtime.commit_pending_hash = commit_hash
    changed(runtime)
    return ""
}

def public repository_refs(repository_id : string; worktree_path : string) : string {
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    var runtime = g_repositories[repository_index]
    if (find_worktree(runtime, worktree_path) < 0) return "unknown worktree"
    runtime.refs_pending_path = worktree_path
    changed(runtime)
    return ""
}

def public repository_working_file_text(repository_id : string;
                                        worktree_path : string;
                                        file_path : string;
                                        max_bytes : int; var text : string&) : string {
    text = ""
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    let runtime = g_repositories[repository_index]
    if (find_worktree(runtime, worktree_path) < 0) return "unknown worktree"
    var absolute_path = ""
    let path_error = repository_working_preview_path(
        worktree_path, file_path, absolute_path)
    if (!empty(path_error)) return path_error
    return repository_read_text_file(absolute_path, max_bytes, text)
}

def public repository_working_file_bytes(repository_id : string;
                                         worktree_path : string;
                                         file_path : string;
                                         max_bytes : int;
                                         var bytes : array<uint8>;
                                         var byte_count : int&) : string {
    delete bytes
    byte_count = 0
    let repository_index = find_repository(repository_id)
    if (repository_index < 0 || g_repositories[repository_index] == null) {
        return "unknown repository"
    }
    let runtime = g_repositories[repository_index]
    if (find_worktree(runtime, worktree_path) < 0) return "unknown worktree"
    var absolute_path = ""
    let path_error = repository_working_preview_path(
        worktree_path, file_path, absolute_path)
    if (!empty(path_error)) return path_error
    let read_error = repository_read_file_bytes(
        absolute_path, max_bytes, bytes, byte_count)
    return read_error
}

def public repository_capture_argv(command_argv : array<string>;
                                   output_path : string;
                                   expected_exit_code : int64;
                                   var argv : array<string>&) : string {
    delete argv
    if (empty(command_argv)) return "capture command is required"
    if (empty(output_path)) return "capture output path is required"
    let helper = path_join(get_das_root(),
        "utils/dasHerd/watcher/git_pipe_capture.das")
    var inscope command <- [get_das_exe(), helper, "--",
        "--output={output_path}", "--expect-exit={expected_exit_code}", "--argv"]
    command |> push_from(command_argv)
    argv <- command
    return ""
}

def public repository_staged_file_view_argv(worktree_path : string;
                                            file_path : string;
                                            output_path : string;
                                            var argv : array<string>&) : string {
    delete argv
    if (!repository_valid_relative_file_path(file_path)) {
        return "invalid worktree-relative file path"
    }
    if (empty(output_path)) return "staged view output path is required"
    let helper = path_join(get_das_root(),
        "utils/dasHerd/watcher/git_blob_capture.das")
    var inscope command <- [get_das_exe(), helper, "--",
        "--worktree={worktree_path}", "--revision=:0",
        "--file={file_path}", "--output={output_path}"]
    argv := command
    return ""
}

def public repository_commit_file_view_argv(worktree_path : string;
                                            commit_hash : string;
                                            file_path : string;
                                            output_path : string;
                                            var argv : array<string>&) : string {
    delete argv
    if (empty(commit_hash)) return "commit hash is required"
    if (!repository_valid_relative_file_path(file_path)) {
        return "invalid worktree-relative file path"
    }
    if (empty(output_path)) return "commit view output path is required"
    let helper = path_join(get_das_root(),
        "utils/dasHerd/watcher/git_blob_capture.das")
    var inscope command <- [get_das_exe(), helper, "--",
        "--worktree={worktree_path}", "--revision={commit_hash}",
        "--file={file_path}", "--output={output_path}"]
    argv := command
    return ""
}

def private launch_git(var runtime : RepositoryRuntime?; argv : array<string>;
                       kind, worktree_path : string;
                       var error_message : string&) : string {
    var inscope request = WatcherLaunchRequest(
        cwd = runtime.snapshot.record.checkout_path,
        display_name = "Git: " + join(argv, " "), kind = kind,
        target_id = runtime.snapshot.record.target_id,
        repository_id = runtime.snapshot.record.id,
        worktree_path = worktree_path,
        columns = GIT_CAPTURE_COLUMNS, rows = GIT_CAPTURE_ROWS, timeout_ms = GIT_TIMEOUT_MS,
        drain_quiet_ms = WATCHER_STRUCTURED_DRAIN_QUIET_MS)
    request.argv := argv
    let _pruned = watcher_prune_completed_sessions(kind,
        runtime.snapshot.record.id, worktree_path)
    let result = watcher_launch(request)
    runtime.snapshot.task_session_id = result.session_id
    if (!result.ok) {
        error_message = result.error
        return ""
    }
    error_message = ""
    return result.session_id
}

def private launch_inventory(var runtime : RepositoryRuntime?) {
    runtime.refresh_pending = false
    runtime.snapshot.refreshing = true
    runtime.snapshot.error = ""
    runtime.refresh_phase = 1
    changed(runtime)
    var launch_error : string
    runtime.refresh_session_id = launch_git(runtime,
        ["git", "-C", runtime.snapshot.record.checkout_path,
            "worktree", "list", "--porcelain"], "git-inventory",
        runtime.snapshot.record.checkout_path, launch_error)
    if (empty(runtime.refresh_session_id)) fail_refresh(runtime, launch_error)
}

def private trim_cr(value : string) : string {
    if (!empty(value) && ends_with(value, "\r")) return slice(value, 0, length(value) - 1)
    return value
}

def private byte_at(value : string; index : int) : int {
    var result = 0
    peek_data(value) $(data) {
        result = int(data[index])
    }
    return result
}

def private append_worktree(var worktrees : array<WorktreeState>;
                            current : WorktreeState) {
    worktrees |> push(WorktreeState(path = current.path, head = current.head,
        branch = current.branch, is_main = empty(worktrees), detached = current.detached,
        bare = current.bare, locked = current.locked, prunable = current.prunable))
}

def public repository_parse_inventory(text : string; var worktrees : array<WorktreeState>) {
    var current <- WorktreeState()
    for (raw_line in split(text, "\n")) {
        let line = trim_cr(raw_line)
        if (empty(line)) {
            if (!empty(current.path)) {
                append_worktree(worktrees, current)
                current <- WorktreeState()
            }
        } elif (starts_with(line, "worktree ")) {
            current.path = slice(line, 9)
        } elif (starts_with(line, "HEAD ")) {
            current.head = slice(line, 5)
        } elif (starts_with(line, "branch ")) {
            current.branch = slice(line, 7)
            if (starts_with(current.branch, "refs/heads/")) {
                current.branch = slice(current.branch, 11)
            }
        } elif (line == "detached") {
            current.detached = true
        } elif (line == "bare") {
            current.bare = true
        } elif (starts_with(line, "locked")) {
            current.locked = true
        } elif (starts_with(line, "prunable")) {
            current.prunable = true
        }
    }
    if (!empty(current.path)) {
        append_worktree(worktrees, current)
    }
}

def private conflict_code(code : string) : bool {
    return code == "DD" || code == "AU" || code == "UD" || code == "UA" || code == "DU" || code == "AA" || code == "UU"
}

def private rename_or_copy_code(code : string) : bool {
    if (length(code) != 2) return false
    return byte_at(code, 0) == 'R' || byte_at(code, 0) == 'C' || byte_at(code, 1) == 'R' || byte_at(code, 1) == 'C'
}

def private parse_counter(text, marker : string) : int {
    let start = find(text, marker)
    if (start < 0) return 0
    let value_start = start + length(marker)
    var value_end = value_start
    let text_length = length(text)
    while (value_end < text_length && is_number(byte_at(text, value_end))) {
        value_end++
    }
    return to_int(slice(text, value_start, value_end))
}

def private parse_upstream_ref(line : string) : string {
    //! `## branch...origin/branch [ahead 1]` -> `origin/branch`. A branch with
    //! no upstream prints no `...` at all, which is the "nothing else has a
    //! copy" half of the unpushed verdict. `..` is illegal inside a ref name,
    //! so the separator is unambiguous.
    let separator = find(line, "...")
    if (separator < 0) return ""
    var value = slice(line, separator + 3)
    let bracket = find(value, " [")
    if (bracket >= 0) {
        value = slice(value, 0, bracket)
    }
    return strip(value)
}

def public repository_parse_status(text : string; var worktree : WorktreeState;
                                   var files : array<GitFileState>) {
    worktree.ahead = 0
    worktree.behind = 0
    worktree.staged = 0
    worktree.unstaged = 0
    worktree.untracked = 0
    worktree.conflicted = 0
    worktree.upstream_ref = ""
    for (raw_line in split(text, "\n")) {
        let line = trim_cr(raw_line)
        if (starts_with(line, "## ")) {
            worktree.ahead = parse_counter(line, "ahead ")
            worktree.behind = parse_counter(line, "behind ")
            worktree.upstream_ref = parse_upstream_ref(line)
            continue
        }
        if (length(line) < 3) continue
        let code = slice(line, 0, 2)
        var path = slice(line, 3)
        var old_path : string
        let rename_separator = find(path, " -> ")
        if (rename_separator >= 0 && rename_or_copy_code(code)) {
            old_path = slice(path, 0, rename_separator)
            path = slice(path, rename_separator + 4)
        }
        files |> push(GitFileState(worktree_path = worktree.path, code = code,
            path = path, old_path = old_path))
        if (code == "??") worktree.untracked++
        else {
            if (byte_at(line, 0) != ' ') worktree.staged++
            if (byte_at(line, 1) != ' ') worktree.unstaged++
            if (conflict_code(code)) worktree.conflicted++
        }
    }
}

def private launch_status(var runtime : RepositoryRuntime?) : bool {
    if (runtime.worktree_index >= length(runtime.snapshot.worktrees)) return false
    runtime.refresh_phase = 2
    let path = runtime.snapshot.worktrees[runtime.worktree_index].path
    runtime.snapshot.record.checkout_path = runtime.snapshot.worktrees[0].path
    var launch_error : string
    runtime.refresh_session_id = launch_git(runtime,
        ["git", "-C", path, "-c", "core.quotePath=false",
            "status", "--porcelain=1", "--branch", "--untracked-files=all"],
        "git-status", path, launch_error)
    if (empty(runtime.refresh_session_id)) {
        fail_refresh(runtime, launch_error)
        return false
    }
    return true
}

def private launch_working_stats(var runtime : RepositoryRuntime?) {
    let path = runtime.snapshot.worktrees[runtime.worktree_index].path
    runtime.refresh_phase = 10
    var launch_error : string
    runtime.refresh_session_id = launch_git(runtime,
        ["git", "--literal-pathspecs", "-C", path,
            "--no-pager", "diff", "--numstat", "--find-renames", "--"],
        "git-working-numstat", path, launch_error)
    if (empty(runtime.refresh_session_id)) fail_refresh(runtime, launch_error)
}

def public repository_worktree_only_here(worktree : WorktreeState const) : bool {
    //! "These commits exist nowhere else": unmerged from the authoritative
    //! base AND not on any upstream. An unknown count is never "only here" —
    //! an unresolved base must not manufacture a warning.
    if (!worktree.unmerged_known || worktree.unmerged_commits <= 0) return false
    return empty(worktree.upstream_ref) || worktree.ahead > 0
}

def public repository_worktree_delete_tier(worktree : WorktreeState const;
                                           claimed_by : string) : string {
    //! The settled verdict ladder for RETIRING A WORKTREE. Committed work is
    //! never at risk here — `git worktree remove` leaves the branch intact —
    //! so the only blocker below in_use is the working tree itself.
    if (!empty(claimed_by)) return WORKTREE_TIER_IN_USE
    if (worktree.is_main) return WORKTREE_TIER_MAIN
    if (worktree.staged + worktree.unstaged + worktree.conflicted
            + worktree.untracked > 0) return WORKTREE_TIER_DIRTY
    if (repository_worktree_only_here(worktree)) return WORKTREE_TIER_WARN
    return WORKTREE_TIER_GREEN
}

def public repository_default_base_ref(record : RepositoryRecord const) : string {
    //! The AUTHORITATIVE base every unmerged count is measured against. A
    //! configured base_ref wins; otherwise origin/master, with the origin/main
    //! fallback tried once at the call site.
    return empty(record.base_ref) ? "origin/master" : record.base_ref
}

def private launch_unmerged(var runtime : RepositoryRuntime?) {
    let path = runtime.snapshot.worktrees[runtime.worktree_index].path
    runtime.refresh_phase = 11
    runtime.unmerged_base_ref = repository_default_base_ref(runtime.snapshot.record)
    runtime.unmerged_tried_origin_main = false
    var launch_error : string
    runtime.refresh_session_id = launch_git(runtime,
        ["git", "-C", path, "rev-list", "--count",
            "{runtime.unmerged_base_ref}..HEAD"],
        "git-unmerged", path, launch_error)
    if (empty(runtime.refresh_session_id)) fail_refresh(runtime, launch_error)
}

def private relaunch_unmerged_on_main(var runtime : RepositoryRuntime?) : bool {
    //! One retry against origin/main for repositories whose trunk is not
    //! master. Anything still failing leaves unmerged_known false — an
    //! unknown count must never read as zero.
    if (runtime.unmerged_tried_origin_main
        || runtime.unmerged_base_ref != "origin/master") return false
    runtime.unmerged_tried_origin_main = true
    runtime.unmerged_base_ref = "origin/main"
    let path = runtime.snapshot.worktrees[runtime.worktree_index].path
    var launch_error : string
    runtime.refresh_session_id = launch_git(runtime,
        ["git", "-C", path, "rev-list", "--count",
            "{runtime.unmerged_base_ref}..HEAD"],
        "git-unmerged", path, launch_error)
    return !empty(runtime.refresh_session_id)
}

def private launch_review_step(var runtime : RepositoryRuntime?; phase : int;
                               argv : array<string>; kind : string) {
    runtime.review_phase = phase
    var launch_error : string
    runtime.review_session_id = launch_git(runtime, argv, kind,
        runtime.review_worktree_path, launch_error)
    if (empty(runtime.review_session_id)) fail_review(runtime, launch_error)
}

def private launch_review_upstream(var runtime : RepositoryRuntime?) {
    let index = find_worktree(runtime, runtime.review_pending_path)
    if (index < 0) {
        runtime.review_pending_path = ""
        return
    }
    runtime.review_worktree_path = runtime.review_pending_path
    runtime.review_pending_path = ""
    runtime.review_base_ref = ""
    runtime.review_merge_base = ""
    runtime.review_tried_origin_main = false
    runtime.pending_review_commits |> clear()
    runtime.pending_review_files |> clear()
    runtime.snapshot.worktrees[index].review_loading = true
    runtime.snapshot.worktrees[index].review_error = ""
    changed(runtime)
    let path = runtime.review_worktree_path
    let branch = runtime.snapshot.worktrees[index].branch
    let ref = empty(branch) ? "refs/heads/__detached__" : "refs/heads/{branch}"
    launch_review_step(runtime, 3, ["git", "-C", path, "for-each-ref",
        "--format=%(upstream:short)", ref], "git-review-base")
}

def private launch_review_merge_base(var runtime : RepositoryRuntime?) {
    let path = runtime.review_worktree_path
    launch_review_step(runtime, 4, ["git", "-C", path, "merge-base",
        runtime.review_base_ref, "HEAD"], "git-review-merge-base")
}

def private launch_review_commits(var runtime : RepositoryRuntime?) {
    let path = runtime.review_worktree_path
    let revision_range = "{runtime.review_merge_base}..HEAD"
    launch_review_step(runtime, 5, ["git", "-C", path, "--no-pager", "log",
        "--max-count=200", "--format=%H{GIT_FIELD_SEPARATOR}%h{GIT_FIELD_SEPARATOR}%at{GIT_FIELD_SEPARATOR}%an{GIT_FIELD_SEPARATOR}%s{GIT_FIELD_SEPARATOR}%P", revision_range],
        "git-review-commits")
}

def private launch_review_files(var runtime : RepositoryRuntime?) {
    let path = runtime.review_worktree_path
    launch_review_step(runtime, 6, ["git", "--literal-pathspecs", "-C", path,
        "--no-pager", "diff", "--cached", "--numstat", "--find-renames",
        runtime.review_merge_base, "--"], "git-review-files")
}

def private launch_history(var runtime : RepositoryRuntime?) {
    let index = find_worktree(runtime, runtime.history_pending_path)
    if (index < 0) {
        runtime.history_pending_path = ""
        runtime.history_pending_ref = ""
        return
    }
    runtime.history_worktree_path = runtime.history_pending_path
    runtime.history_ref = runtime.history_pending_ref
    runtime.history_pending_path = ""
    runtime.history_pending_ref = ""
    runtime.pending_history_commits |> clear()
    runtime.pending_graph_rows |> clear()
    runtime.snapshot.worktrees[index].history_loading = true
    runtime.snapshot.worktrees[index].history_error = ""
    runtime.history_phase = 7
    changed(runtime)
    let path = runtime.history_worktree_path
    var inscope argv <- ["git", "-C", path, "--no-pager", "log",
        "--max-count=200"]
    if (runtime.history_ref == "__all__") {
        argv |> push("--graph")
        argv |> push("--topo-order")
        argv |> push("--full-history")
        argv |> push("--format={GIT_COMMIT_MARKER}%H{GIT_FIELD_SEPARATOR}%h{GIT_FIELD_SEPARATOR}%at{GIT_FIELD_SEPARATOR}%an{GIT_FIELD_SEPARATOR}%s{GIT_FIELD_SEPARATOR}%P")
    } else {
        argv |> push("--format=%H{GIT_FIELD_SEPARATOR}%h{GIT_FIELD_SEPARATOR}%at{GIT_FIELD_SEPARATOR}%an{GIT_FIELD_SEPARATOR}%s{GIT_FIELD_SEPARATOR}%P")
    }
    argv |> push(runtime.history_ref == "__all__" ? "--all" : runtime.history_ref)
    var launch_error : string
    runtime.history_session_id = launch_git(runtime, argv, "git-history", path,
        launch_error)
    if (empty(runtime.history_session_id)) fail_history(runtime, launch_error)
}

def private launch_commit_files(var runtime : RepositoryRuntime?) {
    let requested_path = runtime.commit_pending_path
    let requested_hash = runtime.commit_pending_hash
    runtime.commit_pending_path = ""
    runtime.commit_pending_hash = ""
    let index = find_worktree(runtime, requested_path)
    if (index < 0 || empty(requested_hash)) return
    runtime.commit_worktree_path = requested_path
    runtime.history_commit = requested_hash
    runtime.pending_commit_files |> clear()
    runtime.snapshot.worktrees[index].history_loading = true
    runtime.snapshot.worktrees[index].history_error = ""
    runtime.commit_phase = 8
    changed(runtime)
    var launch_error : string
    runtime.commit_session_id = launch_git(runtime,
        ["git", "--literal-pathspecs", "-C", requested_path,
            "--no-pager", "show", "--format=", "--numstat", "--find-renames",
            "--first-parent", requested_hash, "--"], "git-commit-files",
        requested_path, launch_error)
    if (empty(runtime.commit_session_id)) fail_commit_files(runtime, launch_error)
}

def private launch_refs(var runtime : RepositoryRuntime?) {
    let index = find_worktree(runtime, runtime.refs_pending_path)
    if (index < 0) {
        runtime.refs_pending_path = ""
        return
    }
    let path = runtime.refs_pending_path
    runtime.refs_pending_path = ""
    runtime.refs_worktree_path = path
    runtime.pending_refs |> clear()
    runtime.snapshot.worktrees[index].refs_loading = true
    runtime.snapshot.worktrees[index].refs_error = ""
    runtime.refs_phase = 9
    changed(runtime)
    var launch_error : string
    runtime.refs_session_id = launch_git(runtime, ["git", "-C", path, "for-each-ref",
        "--format=%(refname){GIT_FIELD_SEPARATOR}%(refname:short){GIT_FIELD_SEPARATOR}%(objectname){GIT_FIELD_SEPARATOR}%(HEAD){GIT_FIELD_SEPARATOR}%(symref){GIT_FIELD_SEPARATOR}%(upstream:short)",
        "refs/heads", "refs/remotes"], "git-refs", path, launch_error)
    if (empty(runtime.refs_session_id)) fail_refs(runtime, launch_error)
}

def private erase_history_content(var runtime : RepositoryRuntime?; path : string) {
    var index = 0
    while (index < length(runtime.snapshot.history_commits)) {
        if (runtime.snapshot.history_commits[index].worktree_path == path) {
            runtime.snapshot.history_commits |> erase(index)
        } else {
            index++
        }
    }
    index = 0
    while (index < length(runtime.snapshot.commit_files)) {
        if (runtime.snapshot.commit_files[index].worktree_path == path) {
            runtime.snapshot.commit_files |> erase(index)
        } else {
            index++
        }
    }
    index = 0
    while (index < length(runtime.snapshot.graph_rows)) {
        if (runtime.snapshot.graph_rows[index].worktree_path == path) {
            runtime.snapshot.graph_rows |> erase(index)
        } else {
            index++
        }
    }
}

def private erase_refs(var runtime : RepositoryRuntime?; path : string) {
    var index = 0
    while (index < length(runtime.snapshot.refs)) {
        if (runtime.snapshot.refs[index].worktree_path == path) {
            runtime.snapshot.refs |> erase(index)
        } else {
            index++
        }
    }
}

def private observed_worktree_path(worktrees : array<WorktreeState> const;
                                   path : string) : bool {
    for (worktree in worktrees) {
        if (worktree.path == path) return true
    }
    return false
}

def public repository_prune_removed_worktree_data(
                                                  var snapshot : RepositorySnapshot;
                                                  worktrees : array<WorktreeState> const) {
    var index = 0
    while (index < length(snapshot.files)) {
        if (!observed_worktree_path(worktrees,
                snapshot.files[index].worktree_path)) {
            snapshot.files |> erase(index)
        } else {
            index++
        }
    }
    index = 0
    while (index < length(snapshot.review_commits)) {
        if (!observed_worktree_path(worktrees,
                snapshot.review_commits[index].worktree_path)) {
            snapshot.review_commits |> erase(index)
        } else {
            index++
        }
    }
    index = 0
    while (index < length(snapshot.review_files)) {
        if (!observed_worktree_path(worktrees,
                snapshot.review_files[index].worktree_path)) {
            snapshot.review_files |> erase(index)
        } else {
            index++
        }
    }
    index = 0
    while (index < length(snapshot.history_commits)) {
        if (!observed_worktree_path(worktrees,
                snapshot.history_commits[index].worktree_path)) {
            snapshot.history_commits |> erase(index)
        } else {
            index++
        }
    }
    index = 0
    while (index < length(snapshot.graph_rows)) {
        if (!observed_worktree_path(worktrees,
                snapshot.graph_rows[index].worktree_path)) {
            snapshot.graph_rows |> erase(index)
        } else {
            index++
        }
    }
    index = 0
    while (index < length(snapshot.commit_files)) {
        if (!observed_worktree_path(worktrees,
                snapshot.commit_files[index].worktree_path)) {
            snapshot.commit_files |> erase(index)
        } else {
            index++
        }
    }
    index = 0
    while (index < length(snapshot.refs)) {
        if (!observed_worktree_path(worktrees,
                snapshot.refs[index].worktree_path)) {
            snapshot.refs |> erase(index)
        } else {
            index++
        }
    }
}

def private promote_pending_refresh(var runtime : RepositoryRuntime?) {
    if (!runtime.rerun_pending) return
    runtime.rerun_pending = false
    runtime.refresh_pending = true
}

def private finish_history_commits(var runtime : RepositoryRuntime?) {
    runtime.history_phase = 0
    let path = runtime.history_worktree_path
    let index = find_worktree(runtime, path)
    if (index < 0) {
        runtime.pending_history_commits |> clear()
        runtime.pending_graph_rows |> clear()
        return
    }
    erase_history_content(runtime, path)
    runtime.snapshot.history_commits |> push_from(runtime.pending_history_commits)
    runtime.pending_history_commits |> clear()
    runtime.snapshot.graph_rows |> push_from(runtime.pending_graph_rows)
    runtime.pending_graph_rows |> clear()
    runtime.snapshot.worktrees[index].history_ref = runtime.history_ref
    if (!empty(runtime.snapshot.history_commits)) {
        var first_hash = ""
        for (commit in runtime.snapshot.history_commits) {
            if (commit.worktree_path == path) {
                first_hash = commit.hash
                break
            }
        }
        if (!empty(first_hash)) {
            // The commit stream fetches the newest commit's files; history
            // stays loading until that lands so the UI reads one transition.
            runtime.commit_pending_path = path
            runtime.commit_pending_hash = first_hash
            changed(runtime)
            return
        }
    }
    runtime.snapshot.worktrees[index].history_commit = ""
    runtime.snapshot.worktrees[index].history_loading = false
    runtime.snapshot.worktrees[index].history_error = ""
    changed(runtime)
}

def private finish_commit_files(var runtime : RepositoryRuntime?) {
    runtime.commit_phase = 0
    let path = runtime.commit_worktree_path
    let index = find_worktree(runtime, path)
    if (index < 0) {
        runtime.pending_commit_files |> clear()
        return
    }
    var file_index = 0
    while (file_index < length(runtime.snapshot.commit_files)) {
        if (runtime.snapshot.commit_files[file_index].worktree_path == path) {
            runtime.snapshot.commit_files |> erase(file_index)
        } else {
            file_index++
        }
    }
    runtime.snapshot.commit_files |> push_from(runtime.pending_commit_files)
    runtime.pending_commit_files |> clear()
    runtime.snapshot.worktrees[index].history_commit = runtime.history_commit
    runtime.snapshot.worktrees[index].history_loading = false
    runtime.snapshot.worktrees[index].history_error = ""
    changed(runtime)
}

def private finish_refs(var runtime : RepositoryRuntime?) {
    runtime.refs_phase = 0
    let path = runtime.refs_worktree_path
    let index = find_worktree(runtime, path)
    if (index < 0) {
        runtime.pending_refs |> clear()
        return
    }
    erase_refs(runtime, path)
    runtime.snapshot.refs |> push_from(runtime.pending_refs)
    runtime.pending_refs |> clear()
    runtime.snapshot.worktrees[index].refs_loading = false
    runtime.snapshot.worktrees[index].refs_error = ""
    changed(runtime)
}

def private erase_review_content(var runtime : RepositoryRuntime?; path : string) {
    var index = 0
    while (index < length(runtime.snapshot.review_commits)) {
        if (runtime.snapshot.review_commits[index].worktree_path == path) {
            runtime.snapshot.review_commits |> erase(index)
        } else {
            index++
        }
    }
    index = 0
    while (index < length(runtime.snapshot.review_files)) {
        if (runtime.snapshot.review_files[index].worktree_path == path) {
            runtime.snapshot.review_files |> erase(index)
        } else {
            index++
        }
    }
}

def private finish_review(var runtime : RepositoryRuntime?) {
    runtime.review_phase = 0
    let path = runtime.review_worktree_path
    let index = find_worktree(runtime, path)
    if (index < 0) {
        runtime.pending_review_commits |> clear()
        runtime.pending_review_files |> clear()
        return
    }
    erase_review_content(runtime, path)
    runtime.snapshot.review_commits |> push_from(runtime.pending_review_commits)
    runtime.snapshot.review_files |> push_from(runtime.pending_review_files)
    runtime.pending_review_commits |> clear()
    runtime.pending_review_files |> clear()
    runtime.snapshot.worktrees[index].review_base_ref = runtime.review_base_ref
    runtime.snapshot.worktrees[index].review_merge_base = runtime.review_merge_base
    runtime.snapshot.worktrees[index].review_loading = false
    runtime.snapshot.worktrees[index].review_error = ""
    changed(runtime)
}

def private fail_review(var runtime : RepositoryRuntime?; message : string) {
    record_git_failure(runtime, runtime.review_session_id, message)
    runtime.review_phase = 0
    let index = find_worktree(runtime, runtime.review_worktree_path)
    if (index >= 0) {
        runtime.snapshot.worktrees[index].review_loading = false
        runtime.snapshot.worktrees[index].review_error = message
    }
    runtime.pending_review_commits |> clear()
    runtime.pending_review_files |> clear()
    changed(runtime)
}

def private fail_history(var runtime : RepositoryRuntime?; message : string) {
    record_git_failure(runtime, runtime.history_session_id, message)
    runtime.history_phase = 0
    let index = find_worktree(runtime, runtime.history_worktree_path)
    if (index >= 0) {
        runtime.snapshot.worktrees[index].history_loading = false
        runtime.snapshot.worktrees[index].history_error = message
    }
    runtime.pending_history_commits |> clear()
    runtime.pending_graph_rows |> clear()
    changed(runtime)
}

def private fail_commit_files(var runtime : RepositoryRuntime?; message : string) {
    record_git_failure(runtime, runtime.commit_session_id, message)
    runtime.commit_phase = 0
    let index = find_worktree(runtime, runtime.commit_worktree_path)
    if (index >= 0) {
        runtime.snapshot.worktrees[index].history_loading = false
        runtime.snapshot.worktrees[index].history_error = message
    }
    runtime.pending_commit_files |> clear()
    changed(runtime)
}

def private fail_refs(var runtime : RepositoryRuntime?; message : string) {
    record_git_failure(runtime, runtime.refs_session_id, message)
    runtime.refs_phase = 0
    let index = find_worktree(runtime, runtime.refs_worktree_path)
    if (index >= 0) {
        runtime.snapshot.worktrees[index].refs_loading = false
        runtime.snapshot.worktrees[index].refs_error = message
    }
    runtime.pending_refs |> clear()
    changed(runtime)
}

def private finish_refresh(var runtime : RepositoryRuntime?) {
    runtime.refresh_phase = 0
    runtime.snapshot.refreshing = false
    runtime.snapshot.last_success_ms = int64(get_clock()) * 1000l
    changed(runtime)
    promote_pending_refresh(runtime)
}

def private fail_refresh(var runtime : RepositoryRuntime?; message : string) {
    record_git_failure(runtime, runtime.refresh_session_id, message)
    runtime.refresh_phase = 0
    runtime.snapshot.refreshing = false
    runtime.snapshot.error = message
    changed(runtime)
    promote_pending_refresh(runtime)
}

def private repository_parse_commit_line(line, worktree_path : string;
                                         var commit : GitCommitState&) : bool {
    let delimiter = find(line, GIT_FIELD_SEPARATOR) >= 0 ? GIT_FIELD_SEPARATOR : "\t"
    let delimiter_length = length(delimiter)
    let hash_end = find(line, delimiter)
    if (hash_end < 0) return false
    let short_end = find(line, delimiter, hash_end + delimiter_length)
    if (short_end < 0) return false
    let time_end = find(line, delimiter, short_end + delimiter_length)
    if (time_end < 0) return false
    let author_end = find(line, delimiter, time_end + delimiter_length)
    if (author_end < 0) return false
    let subject_end = delimiter == GIT_FIELD_SEPARATOR ? rfind(line, delimiter) : -1
    var subject = slice(line, author_end + delimiter_length)
    var parents : string
    if (subject_end >= 0) {
        subject = slice(line, author_end + delimiter_length, subject_end)
        parents = slice(line, subject_end + delimiter_length)
    }
    commit = GitCommitState(
        worktree_path = worktree_path,
        hash = slice(line, 0, hash_end),
        short_hash = slice(line, hash_end + delimiter_length, short_end),
        authored_at = to_int64(slice(line, short_end + delimiter_length, time_end)),
        author = slice(line, time_end + delimiter_length, author_end),
        subject = subject,
        parents = parents)
    return true
}

def public repository_parse_review_commits(text : string; worktree_path : string;
                                           var commits : array<GitCommitState>) {
    for (raw_line in split(text, "\n")) {
        let line = trim_cr(raw_line)
        if (empty(line)) continue
        var commit = GitCommitState()
        if (repository_parse_commit_line(line, worktree_path, commit)) {
            commits |> push(commit)
        }
    }
}

def public repository_parse_graph_history(text : string; worktree_path : string;
                                          var commits : array<GitCommitState>;
                                          var rows : array<GitGraphRowState>) {
    for (raw_line in split(text, "\n")) {
        let line = trim_cr(raw_line)
        if (empty(line)) continue
        let marker = find(line, GIT_COMMIT_MARKER)
        var commit_hash : string
        var topology = line
        if (marker >= 0) {
            topology = slice(line, 0, marker)
            var commit = GitCommitState()
            if (!repository_parse_commit_line(
                    slice(line, marker + length(GIT_COMMIT_MARKER)),
                    worktree_path, commit)) continue
            commit_hash = commit.hash
            commits |> push(commit)
        }
        rows |> push(GitGraphRowState(
            worktree_path = worktree_path,
            topology = topology,
            commit_hash = commit_hash))
    }
}

struct private GitNumstatLine {
    path : string
    old_path : string
    added_lines : int
    deleted_lines : int
    known : bool
    binary : bool
}

def private numstat_token_end(line : string; start : int) : int {
    var result = start
    let line_length = length(line)
    while (result < line_length && !is_white_space(byte_at(line, result))) {
        result++
    }
    return result
}

def private numstat_skip_space(line : string; start : int) : int {
    var result = start
    let line_length = length(line)
    while (result < line_length && is_white_space(byte_at(line, result))) {
        result++
    }
    return result
}

def private numstat_count_token(token : string) : bool {
    if (token == "-") return true
    if (empty(token)) return false
    for (index in range(length(token))) {
        if (!is_number(byte_at(token, index))) return false
    }
    return true
}

def private expanded_numstat_path(path : string) : string {
    let arrow = find(path, " => ")
    var tail_text : string // nolint:LINT003 daScript loses let lifetime after the early-return guard
    var end_pos : int // nolint:LINT003 daScript loses let lifetime after the early-return guard
    if (arrow < 0) {
        return path
    }
    let brace_start = find(path, "\{")
    tail_text = slice(path, arrow + 4)
    end_pos = find(tail_text, "\}")
    if (brace_start >= 0 && end_pos >= 0) {
        return "{slice(path, 0, brace_start)}{slice(tail_text, 0, end_pos)}{slice(tail_text, end_pos + 1)}"
    }
    return slice(path, arrow + 4)
}

def private original_numstat_path(path : string) : string {
    let arrow = find(path, " => ")
    var tail_text : string // nolint:LINT003 daScript loses let lifetime after the early-return guard
    var end_pos : int // nolint:LINT003 daScript loses let lifetime after the early-return guard
    if (arrow < 0) return ""
    let brace_start = find(path, "\{")
    tail_text = slice(path, arrow + 4)
    end_pos = find(tail_text, "\}")
    if (brace_start >= 0 && end_pos >= 0) {
        return "{slice(path, 0, brace_start)}{slice(path, brace_start + 1, arrow)}{slice(tail_text, end_pos + 1)}"
    }
    return slice(path, 0, arrow)
}

def private parse_numstat_line(line : string; var result : GitNumstatLine&) : bool {
    let added_end = numstat_token_end(line, 0)
    let deleted_start = numstat_skip_space(line, added_end)
    let deleted_end = numstat_token_end(line, deleted_start)
    let path_start = numstat_skip_space(line, deleted_end)
    if (added_end == 0 || deleted_start >= deleted_end || path_start >= length(line)) return false
    let added = slice(line, 0, added_end)
    let deleted = slice(line, deleted_start, deleted_end)
    if (!numstat_count_token(added) || !numstat_count_token(deleted)) return false
    let raw_path = slice(line, path_start)
    result.path = expanded_numstat_path(raw_path)
    result.old_path = original_numstat_path(raw_path)
    result.binary = added == "-" || deleted == "-"
    result.known = !result.binary
    result.added_lines = result.known ? to_int(added) : 0
    result.deleted_lines = result.known ? to_int(deleted) : 0
    return true
}

def private count_untracked_lines(worktree_path, relative_path : string;
                                  var added_lines : int&; var binary : bool&) : bool {
    let full_path = path_join(worktree_path, relative_path)
    let file_status = stat(full_path)
    if (!file_status.is_valid || !file_status.is_reg
            || file_status.size > uint64(UNTRACKED_NUMSTAT_MAX_BYTES)) return false
    var loaded = false
    fopen(full_path, "rb") $(file) {
        if (file != null) {
            fmap(file) $(var data : array<uint8>#) {
                added_lines = 0
                binary = false
                for (raw_byte in data) {
                    let byte = int(raw_byte)
                    if (byte == 0) {
                        binary = true
                        break
                    }
                    if (byte == '\n') added_lines++
                }
                if (!binary && !empty(data) && data[length(data) - 1] != uint8('\n')) added_lines++
                loaded = true
            }
        }
    }
    return loaded
}

def public repository_apply_working_numstat(text : string; worktree_path : string;
                                            var files : array<GitFileState>) {
    var inscope file_by_path : table<string; int>
    for (index in range(length(files))) {
        if (files[index].worktree_path == worktree_path) {
            file_by_path[files[index].path] = index
        }
    }
    for (raw_line in split(text, "\n")) {
        var parsed = GitNumstatLine()
        if (!parse_numstat_line(trim_cr(raw_line), parsed)) continue
        let file_index = file_by_path?[parsed.path] ?? -1
        if (file_index < 0) continue
        files[file_index].added_lines = parsed.added_lines
        files[file_index].deleted_lines = parsed.deleted_lines
        files[file_index].stats_known = parsed.known
        files[file_index].binary = parsed.binary
    }
    for (file in files) {
        if (file.worktree_path != worktree_path || file.code != "??") continue
        file.stats_known = count_untracked_lines(worktree_path, file.path,
            file.added_lines, file.binary) && !file.binary
    }
}

def public repository_parse_review_files(text : string; worktree_path : string;
                                         var files : array<GitReviewFileState>) {
    for (raw_line in split(text, "\n")) {
        let line = trim_cr(raw_line)
        if (empty(line)) continue
        var numstat = GitNumstatLine()
        if (parse_numstat_line(line, numstat)) {
            files |> push(GitReviewFileState(
                worktree_path = worktree_path, path = numstat.path,
                old_path = numstat.old_path,
                added_lines = numstat.added_lines,
                deleted_lines = numstat.deleted_lines,
                stats_known = numstat.known, binary = numstat.binary))
            continue
        }
        let delimiter = find(line, GIT_FIELD_SEPARATOR) >= 0 ? GIT_FIELD_SEPARATOR : "\t"
        let fields = split(line, delimiter)
        if (length(fields) < 2) continue
        let code = fields[0]
        var path = fields[1]
        var old_path : string
        if ((starts_with(code, "R") || starts_with(code, "C"))
                && length(fields) >= 3) {
            old_path = path
            path = fields[2]
        }
        files |> push(GitReviewFileState(
            worktree_path = worktree_path, code = code,
            path = path, old_path = old_path))
    }
}

def public repository_parse_commit_files(text : string; worktree_path : string;
                                         commit_hash : string;
                                         var files : array<GitCommitFileState>) {
    for (raw_line in split(text, "\n")) {
        let line = trim_cr(raw_line)
        if (empty(line)) continue
        var numstat = GitNumstatLine()
        if (parse_numstat_line(line, numstat)) {
            files |> push(GitCommitFileState(
                worktree_path = worktree_path, commit_hash = commit_hash,
                path = numstat.path, old_path = numstat.old_path,
                added_lines = numstat.added_lines,
                deleted_lines = numstat.deleted_lines,
                stats_known = numstat.known, binary = numstat.binary))
            continue
        }
        let fields = split(line, "\t")
        if (length(fields) < 2) continue
        let code = fields[0]
        var path = fields[1]
        var old_path : string
        if ((starts_with(code, "R") || starts_with(code, "C"))
                && length(fields) >= 3) {
            old_path = path
            path = fields[2]
        }
        files |> push(GitCommitFileState(
            worktree_path = worktree_path, commit_hash = commit_hash,
            code = code, path = path, old_path = old_path))
    }
}

def public repository_parse_refs(text : string; worktree_path : string;
                                 var refs : array<GitRefState>) {
    for (raw_line in split(text, "\n")) {
        let line = trim_cr(raw_line)
        if (empty(line)) continue
        let delimiter = find(line, GIT_FIELD_SEPARATOR) >= 0 ? GIT_FIELD_SEPARATOR : "\t"
        let fields = split(line, delimiter)
        if (length(fields) < 4
                || (length(fields) >= 5 && !empty(fields[4]))) continue
        let full_name = fields[0]
        refs |> push(GitRefState(
            worktree_path = worktree_path,
            full_name = full_name,
            name = fields[1],
            hash = fields[2],
            current = strip(fields[3]) == "*",
            remote = starts_with(full_name, "refs/remotes/"),
            upstream = length(fields) >= 6 ? fields[5] : ""))
    }
}

struct private GitTaskResult {
    ready : bool
    exit_code : int64
    truncated : bool
    output : string
}

def private poll_task(session_id : string) : GitTaskResult {
    var result = GitTaskResult()
    var info = WatcherSessionInfo()
    if (empty(session_id) || !watcher_get_session_info(session_id, info)
            || !info.drained) {
        return <- result
    }
    result.ready = true
    result.exit_code = info.exit_code
    var truncated = false
    result.output = watcher_machine_output_text(session_id, truncated)
    result.truncated = truncated
    let _compacted = watcher_compact_session(session_id)
    return <- result
}

def private task_overflow_message(session_id : string) : string {
    return "Git structured output exceeded the watcher raw-history limit; inspect session {session_id}"
}

def private advance_worktree(var runtime : RepositoryRuntime?) {
    runtime.worktree_index++
    if (runtime.worktree_index < length(runtime.snapshot.worktrees)) {
        let _next_status_started = launch_status(runtime)
        changed(runtime)
    } else {
        finish_refresh(runtime)
    }
}

def private advance_refresh(var runtime : RepositoryRuntime?) {
    if (runtime.refresh_phase == 0) {
        if (runtime.refresh_pending) launch_inventory(runtime)
        return
    }
    let task <- poll_task(runtime.refresh_session_id)
    if (!task.ready) return
    if (task.truncated) {
        fail_refresh(runtime, task_overflow_message(runtime.refresh_session_id))
        return
    }
    if (task.exit_code != 0l) {
        // A missing base ref is a fact about the repository, not a refresh
        // failure: retry once on origin/main, then carry "unknown" forward.
        if (runtime.refresh_phase == 11) {
            if (relaunch_unmerged_on_main(runtime)) return
            runtime.snapshot.worktrees[runtime.worktree_index].unmerged_known = false
            runtime.snapshot.worktrees[runtime.worktree_index].unmerged_commits = 0
            runtime.snapshot.worktrees[runtime.worktree_index].unmerged_base_ref = ""
            advance_worktree(runtime)
            return
        }
        fail_refresh(runtime, "git exited with {task.exit_code}: {task.output}")
        return
    }
    let output = task.output
    if (runtime.refresh_phase == 1) {
        var worktrees : array<WorktreeState>
        repository_parse_inventory(output, worktrees)
        if (empty(worktrees)) {
            fail_refresh(runtime, "git returned no worktrees")
            return
        }
        let main_path = worktrees[0].path
        let resolved_id = repository_id(runtime.snapshot.record.target_id, main_path)
        let collision_index = find_repository(resolved_id)
        if (collision_index >= 0 && g_repositories[collision_index] != runtime) {
            delete worktrees
            fail_refresh(runtime,
                "main checkout is already registered: {main_path}")
            return
        }
        for (new_worktree in worktrees) {
            for (old_worktree in runtime.snapshot.worktrees) {
                if (old_worktree.path != new_worktree.path) continue
                new_worktree.review_base_ref = old_worktree.review_base_ref
                new_worktree.review_merge_base = old_worktree.review_merge_base
                new_worktree.review_loading = old_worktree.review_loading
                new_worktree.review_error = old_worktree.review_error
                new_worktree.history_ref = old_worktree.history_ref
                new_worktree.history_commit = old_worktree.history_commit
                new_worktree.history_loading = old_worktree.history_loading
                new_worktree.history_error = old_worktree.history_error
                new_worktree.refs_loading = old_worktree.refs_loading
                new_worktree.refs_error = old_worktree.refs_error
                // Carry the delete-safety facts across the inventory swap or
                // the verdict badge blinks to "unknown" on every refresh.
                new_worktree.upstream_ref = old_worktree.upstream_ref
                new_worktree.unmerged_commits = old_worktree.unmerged_commits
                new_worktree.unmerged_base_ref = old_worktree.unmerged_base_ref
                new_worktree.unmerged_known = old_worktree.unmerged_known
                break
            }
        }
        repository_prune_removed_worktree_data(runtime.snapshot, worktrees)
        delete runtime.snapshot.worktrees
        runtime.snapshot.worktrees <- worktrees
        if (runtime.snapshot.record.checkout_path != main_path) {
            let old_default_name = base_name(runtime.snapshot.record.checkout_path)
            runtime.snapshot.record.checkout_path = main_path
            runtime.snapshot.record.id = resolved_id
            if (empty(runtime.snapshot.record.display_name) || runtime.snapshot.record.display_name == old_default_name) {
                runtime.snapshot.record.display_name = base_name(main_path)
            }
            let _save_error = save_config()
        }
        runtime.worktree_index = 0
        let _status_started = launch_status(runtime)
        changed(runtime)
        return
    }
    if (runtime.refresh_phase == 2) {
        runtime.pending_refresh_files |> clear()
        repository_parse_status(output, runtime.snapshot.worktrees[runtime.worktree_index], runtime.pending_refresh_files)
        launch_working_stats(runtime)
        return
    }
    if (runtime.refresh_phase == 11) {
        let index = runtime.worktree_index
        runtime.snapshot.worktrees[index].unmerged_commits = to_int(strip(output))
        runtime.snapshot.worktrees[index].unmerged_base_ref = runtime.unmerged_base_ref
        runtime.snapshot.worktrees[index].unmerged_known = true
        advance_worktree(runtime)
        return
    }
    // Swap this worktree's file rows atomically only after its status and
    // numstat pair completes, so a refresh never blanks the changelist.
    let stats_path = runtime.snapshot.worktrees[runtime.worktree_index].path
    repository_apply_working_numstat(output, stats_path, runtime.pending_refresh_files)
    var file_index = 0
    while (file_index < length(runtime.snapshot.files)) {
        if (runtime.snapshot.files[file_index].worktree_path == stats_path) {
            runtime.snapshot.files |> erase(file_index)
        } else {
            file_index++
        }
    }
    runtime.snapshot.files |> push_from(runtime.pending_refresh_files)
    runtime.pending_refresh_files |> clear()
    launch_unmerged(runtime)
}

def private advance_review(var runtime : RepositoryRuntime?) {
    if (runtime.review_phase == 0) {
        if (!empty(runtime.review_pending_path)) launch_review_upstream(runtime)
        return
    }
    let task <- poll_task(runtime.review_session_id)
    if (!task.ready) return
    if (task.truncated) {
        fail_review(runtime, task_overflow_message(runtime.review_session_id))
        return
    }
    if (task.exit_code != 0l) {
        if (runtime.review_phase == 4 && runtime.review_base_ref == "origin/master"
                && !runtime.review_tried_origin_main) {
            runtime.review_base_ref = "origin/main"
            runtime.review_tried_origin_main = true
            launch_review_merge_base(runtime)
        } else {
            fail_review(runtime, "git exited with {task.exit_code}: {task.output}")
        }
        return
    }
    let output = task.output
    if (runtime.review_phase == 3) {
        runtime.review_base_ref = strip(output)
        if (empty(runtime.review_base_ref)) {
            if (!empty(runtime.snapshot.record.base_ref)) {
                runtime.review_base_ref = runtime.snapshot.record.base_ref
            } else {
                runtime.review_base_ref = "origin/master"
            }
        }
        launch_review_merge_base(runtime)
    } elif (runtime.review_phase == 4) {
        runtime.review_merge_base = strip(output)
        if (empty(runtime.review_merge_base)) {
            fail_review(runtime, "git returned no merge base for {runtime.review_base_ref}")
        } else {
            launch_review_commits(runtime)
        }
    } elif (runtime.review_phase == 5) {
        repository_parse_review_commits(output, runtime.review_worktree_path,
            runtime.pending_review_commits)
        launch_review_files(runtime)
    } else {
        repository_parse_review_files(output, runtime.review_worktree_path,
            runtime.pending_review_files)
        finish_review(runtime)
    }
}

def private advance_history(var runtime : RepositoryRuntime?) {
    if (runtime.history_phase == 0) {
        if (!empty(runtime.history_pending_path)) launch_history(runtime)
        return
    }
    let task <- poll_task(runtime.history_session_id)
    if (!task.ready) return
    if (task.truncated) {
        fail_history(runtime, task_overflow_message(runtime.history_session_id))
        return
    }
    if (task.exit_code != 0l) {
        fail_history(runtime, "git exited with {task.exit_code}: {task.output}")
        return
    }
    if (runtime.history_ref == "__all__") {
        repository_parse_graph_history(task.output, runtime.history_worktree_path,
            runtime.pending_history_commits, runtime.pending_graph_rows)
    } else {
        repository_parse_review_commits(task.output, runtime.history_worktree_path,
            runtime.pending_history_commits)
    }
    finish_history_commits(runtime)
}

def private advance_commit_files(var runtime : RepositoryRuntime?) {
    if (runtime.commit_phase == 0) {
        if (!empty(runtime.commit_pending_path)) launch_commit_files(runtime)
        return
    }
    let task <- poll_task(runtime.commit_session_id)
    if (!task.ready) return
    if (task.truncated) {
        fail_commit_files(runtime, task_overflow_message(runtime.commit_session_id))
        return
    }
    if (task.exit_code != 0l) {
        fail_commit_files(runtime, "git exited with {task.exit_code}: {task.output}")
        return
    }
    repository_parse_commit_files(task.output, runtime.commit_worktree_path,
        runtime.history_commit, runtime.pending_commit_files)
    finish_commit_files(runtime)
}

def private advance_refs(var runtime : RepositoryRuntime?) {
    if (runtime.refs_phase == 0) {
        if (!empty(runtime.refs_pending_path)) launch_refs(runtime)
        return
    }
    let task <- poll_task(runtime.refs_session_id)
    if (!task.ready) return
    if (task.truncated) {
        fail_refs(runtime, task_overflow_message(runtime.refs_session_id))
        return
    }
    if (task.exit_code != 0l) {
        fail_refs(runtime, "git exited with {task.exit_code}: {task.output}")
        return
    }
    repository_parse_refs(task.output, runtime.refs_worktree_path,
        runtime.pending_refs)
    finish_refs(runtime)
}

def public repository_tick() {
    for (runtime in g_repositories) {
        if (runtime == null) continue
        advance_refresh(runtime)
        advance_review(runtime)
        advance_history(runtime)
        advance_commit_files(runtime)
        advance_refs(runtime)
    }
    advance_operation()
}
