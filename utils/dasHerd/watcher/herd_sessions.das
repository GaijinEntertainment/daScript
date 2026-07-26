options gen2
options rtti
options persistent_heap
options gc
options indenting = 4

module herd_sessions shared public

require ./watcher_core.das
require daslib/strings_boost
require math

// Durable session registry. A herd session is the long-lived entity the user
// names and switches between: workspace (primary worktree + satellites), agent
// profile, and lifecycle state survive both the hosting PTY process and the
// watcher itself. The PTY (WatcherSession) is a transient attribute linked via
// pty_session_id; relaunching an exited herd session starts a fresh PTY with
// the profile's resume command in the same primary worktree.

let public HERD_REGISTRY_VERSION = 1
let public HERD_PROFILES_VERSION = 1

struct public HerdWorkspaceEntry {
    repository_id : string
    worktree_path : string
    added_by : string
}

struct public HerdProfile {
    id : string
    display_name : string
    launch_argv : array<string>
    resume_argv : array<string>
    agent : bool
    // Optional first-prompt lines injected on the INITIAL launch of an agent
    // session (never on resume). Empty by default: prompt injection is a
    // per-profile choice, since tolerance for it is agent- and model-specific.
    intro_lines : array<string>
}

struct public HerdSession {
    id : string
    name : string
    host : string = "local"
    profile_id : string
    workflow : string
    repository_id : string
    worktree_path : string
    satellites : array<HerdWorkspaceEntry>
    task_context : string
    state : string = "exited"
    reason : string
    pty_session_id : string
    exit_code : int64 = -1l
    created_wall_s : int64
    updated_wall_s : int64
    launch_count : int
    // Unread agent->human mailbox messages on the linked PTY; live UI state,
    // refreshed every tick and not meaningful across restarts.
    unread_outbox : int
    // Advisory: another non-closed session attends one of this session's
    // worktrees. Recomputed every tick, never enforced.
    conflicted : bool
    columns : int = 120
    rows : int = 36
}

struct public HerdCreateRequest {
    name : string
    host : string = "local"
    profile_id : string
    workflow : string
    repository_id : string
    worktree_path : string
    task_context : string
    columns : int = 120
    rows : int = 36
}

struct private HerdRegistryFile {
    version : int = HERD_REGISTRY_VERSION
    next_id : int
    sessions : array<HerdSession>
}

struct private HerdProfilesFile {
    version : int = HERD_PROFILES_VERSION
    profiles : array<HerdProfile>
}

var private g_registry_path : string
var private g_profiles : array<HerdProfile>
var private g_herd : array<HerdSession>
var private g_next_herd_id = 0
var private g_herd_revision : int64

def private wall_seconds : int64 => int64(get_clock())

def public herd_canonical_worktree(path : string) : string {
    let full = get_full_file_name(path)
    return get_platform_name() == "windows" ? to_lower(full) : full
}

def public herd_display_worktree(path : string) : string {
    //! One stored spelling — forward slashes, matching git output — so plain
    //! string comparisons against observer worktree paths hold everywhere.
    return replace(path, "\\", "/")
}

def private refresh_conflicts : bool {
    var inscope counts : table<string; int>
    for (record in g_herd) {
        if (record.state == "closed") continue
        counts[herd_canonical_worktree(record.worktree_path)]++
        for (entry in record.satellites) {
            counts[herd_canonical_worktree(entry.worktree_path)]++
        }
    }
    var changed = false
    for (record in g_herd) {
        var flagged = false
        if (record.state != "closed") {
            flagged = counts[herd_canonical_worktree(record.worktree_path)] > 1
            if (!flagged) {
                for (entry in record.satellites) {
                    if (counts[herd_canonical_worktree(entry.worktree_path)] > 1) {
                        flagged = true
                        break
                    }
                }
            }
        }
        if (flagged != record.conflicted) {
            record.conflicted = flagged
            changed = true
        }
    }
    return changed
}

def private default_profiles : array<HerdProfile> {
    var result : array<HerdProfile>
    var shell = HerdProfile(id = "shell", display_name = "Shell")
    if (get_platform_name() == "windows") {
        shell.launch_argv <- ["powershell.exe", "-NoLogo"]
    } else {
        shell.launch_argv <- ["bash", "-l"]
    }
    result |> emplace(shell)
    var claude = HerdProfile(id = "claude", display_name = "Claude Code", agent = true,
        launch_argv <- ["claude"], resume_argv <- ["claude", "--continue"])
    result |> emplace(claude)
    var codex = HerdProfile(id = "codex", display_name = "Codex", agent = true,
        launch_argv <- ["codex"], resume_argv <- ["codex", "resume", "--last"])
    result |> emplace(codex)
    return <- result
}

// The profiles file is user-editable config: written once with the shipped
// defaults so it is discoverable, then authoritative on every load.
def private load_profiles(profiles_path : string) {
    delete g_profiles
    if (fexist(profiles_path)) {
        var inscope file = HerdProfilesFile()
        if (sscan_json(fread(profiles_path), file) && file.version == HERD_PROFILES_VERSION
                && !empty(file.profiles)) {
            g_profiles <- file.profiles
            return
        }
    }
    g_herd_revision++
    g_profiles <- default_profiles()
    var inscope file = HerdProfilesFile()
    file.profiles := g_profiles
    var error : string
    if (mkdir_rec(dir_name(profiles_path), error)) {
        let _ = fwrite(profiles_path, sprint_json(file, true))
    }
}

def private save_registry : string {
    if (empty(g_registry_path)) return "herd registry is not initialized"
    var inscope file = HerdRegistryFile(next_id = g_next_herd_id)
    file.sessions := g_herd
    var error : string
    if (!mkdir_rec(dir_name(g_registry_path), error)) {
        return "could not create herd registry directory: {error}"
    }
    // Durability boundary: never overwrite the registry in place — a kill or
    // full disk mid-write must not truncate it. Write the temp fully, then
    // swap (remove + rename; Windows _wrename cannot replace). A crash leaves
    // either file COMPLETE, and herd_init recovers the temp when only the
    // swap was cut.
    let temp_path = "{g_registry_path}.tmp"
    if (!fwrite(temp_path, sprint_json(file, true))) {
        return "could not write {temp_path}"
    }
    if (fexist(g_registry_path) && !remove(g_registry_path)) {
        return "could not replace {g_registry_path}"
    }
    if (!rename(temp_path, g_registry_path)) {
        return "could not swap {temp_path} into place"
    }
    return ""
}

def public herd_init(registry_path, profiles_path : string) : string {
    g_registry_path = registry_path
    load_profiles(profiles_path)
    delete g_herd
    g_next_herd_id = 0
    g_herd_revision++
    if (!fexist(registry_path)) {
        // A crash between save_registry's remove and rename leaves only the
        // completed temp — adopt it.
        let temp_path = "{registry_path}.tmp"
        if (fexist(temp_path)) {
            let _ = rename(temp_path, registry_path)
        }
    }
    if (!fexist(registry_path)) return ""
    var inscope file = HerdRegistryFile()
    if (!sscan_json(fread(registry_path), file)) return "invalid herd registry JSON"
    if (file.version != HERD_REGISTRY_VERSION) {
        return "unsupported herd registry version {file.version}"
    }
    g_next_herd_id = file.next_id
    // The watcher owns every PTY, so none of them survived the restart.
    var orphaned = 0
    var healed = 0
    for (record in file.sessions) {
        if (record.state == "running") {
            record.state = "exited"
            record.reason = "watcher_restart"
            record.updated_wall_s = wall_seconds()
            orphaned++
        }
        // Heal records saved before path spellings were canonicalized.
        let display_path = herd_display_worktree(record.worktree_path)
        if (display_path != record.worktree_path) {
            record.worktree_path = display_path
            healed++
        }
        for (entry in record.satellites) {
            let entry_path = herd_display_worktree(entry.worktree_path)
            if (entry_path != entry.worktree_path) {
                entry.worktree_path = entry_path
                healed++
            }
        }
    }
    g_herd <- file.sessions
    return (orphaned > 0 || healed > 0) ? save_registry() : ""
}

def public herd_shutdown() {
    delete g_herd
    delete g_profiles
    g_registry_path = ""
    g_next_herd_id = 0
    g_herd_revision = 0l
}

def public herd_revision : int64 => g_herd_revision

def public herd_sessions_json : string {
    var inscope parts : array<string>
    parts |> reserve(length(g_herd))
    for (record in g_herd) {
        parts |> push(sprint_json(record, false))
    }
    return "[" + join(parts, ",") + "]"
}

def public herd_profiles_json : string {
    var inscope parts : array<string>
    parts |> reserve(length(g_profiles))
    for (profile in g_profiles) {
        parts |> push(sprint_json(profile, false))
    }
    return "[" + join(parts, ",") + "]"
}

def public herd_find(herd_id : string) : int {
    for (index in range(length(g_herd))) {
        if (g_herd[index].id == herd_id) return index
    }
    return -1
}

def public herd_find_profile(profile_id : string) : int {
    for (index in range(length(g_profiles))) {
        if (g_profiles[index].id == profile_id) return index
    }
    return -1
}

def public herd_get(herd_id : string; var record : HerdSession) : bool {
    let index = herd_find(herd_id)
    if (index < 0) return false
    record := g_herd[index]
    return true
}

// Creates the durable record only; the caller launches the PTY separately and
// links it. A record is born "exited" so a failed first launch still leaves a
// relaunchable session behind.
def public herd_create(request : HerdCreateRequest; var error : string&) : string {
    error = ""
    if (empty(g_registry_path)) {
        error = "herd registry is not initialized"
        return ""
    }
    if (!empty(request.host) && request.host != "local") {
        error = "only the local host is implemented"
        return ""
    }
    if (herd_find_profile(request.profile_id) < 0) {
        error = "unknown profile '{request.profile_id}'"
        return ""
    }
    let worktree_path = herd_display_worktree(request.worktree_path)
    let worktree_status = stat(worktree_path)
    if (!worktree_status.is_valid || !worktree_status.is_dir) {
        error = "session worktree does not exist: {worktree_path}"
        return ""
    }
    g_next_herd_id++
    let now = wall_seconds()
    var record = HerdSession(id = "h{ref_time_ticks()}_{g_next_herd_id}",
        name = !empty(strip(request.name)) ? request.name : base_name(worktree_path),
        host = watcher_target_id_or_local(request.host),
        profile_id = request.profile_id, workflow = request.workflow,
        repository_id = request.repository_id, worktree_path = worktree_path,
        task_context = request.task_context,
        reason = "never_launched", created_wall_s = now, updated_wall_s = now,
        columns = clamp(request.columns, 20, 400), rows = clamp(request.rows, 5, 512))
    let id = record.id
    g_herd |> emplace(record)
    g_herd_revision++
    error = save_registry()
    return id
}

def public herd_build_launch(herd_id : string; initial : bool;
                             agent_url, agent_token, agent_skill_path : string;
                             var request : WatcherLaunchRequest;
                             var error : string&) : bool {
    error = ""
    let index = herd_find(herd_id)
    if (index < 0) {
        error = "unknown herd session"
        return false
    }
    let record & = unsafe(g_herd[index])
    let profile_index = herd_find_profile(record.profile_id)
    if (profile_index < 0) {
        error = "unknown profile '{record.profile_id}'"
        return false
    }
    let profile & = unsafe(g_profiles[profile_index])
    let worktree_status = stat(record.worktree_path)
    if (!worktree_status.is_valid || !worktree_status.is_dir) {
        error = "session worktree is missing: {record.worktree_path}"
        return false
    }
    request.command_line = ""
    if (initial || empty(profile.resume_argv)) {
        request.argv := profile.launch_argv
    } else {
        request.argv := profile.resume_argv
    }
    request.cwd = record.worktree_path
    request.display_name = record.name
    request.kind = profile.agent ? "agent-{profile.id}" : "shell"
    request.target_id = record.host
    request.repository_id = record.repository_id
    request.worktree_path = record.worktree_path
    request.purpose = !empty(record.workflow) ? record.workflow : "session"
    request.task_context = record.task_context
    request.intro_text = join(profile.intro_lines, "\n")
    request.herd_session_id = record.id
    request.resume = !initial
    request.columns = record.columns
    request.rows = record.rows
    request.timeout_ms = 0l
    request.drain_quiet_ms = 0l
    if (profile.agent) {
        request.agent_url = agent_url
        request.agent_token = agent_token
        request.agent_skill_path = agent_skill_path
    }
    return true
}

def public herd_link_pty(herd_id, pty_session_id : string) : string {
    let index = herd_find(herd_id)
    if (index < 0) return "unknown herd session"
    g_herd[index].pty_session_id = pty_session_id
    g_herd[index].state = "running"
    g_herd[index].reason = ""
    g_herd[index].exit_code = -1l
    g_herd[index].launch_count++
    g_herd[index].updated_wall_s = wall_seconds()
    g_herd_revision++
    return save_registry()
}

def public herd_mark_launch_failed(herd_id, reason : string) : string {
    let index = herd_find(herd_id)
    if (index < 0) return "unknown herd session"
    g_herd[index].state = "exited"
    g_herd[index].reason = !empty(reason) ? reason : "launch_failed"
    g_herd[index].updated_wall_s = wall_seconds()
    g_herd_revision++
    return save_registry()
}

// Observes linked PTYs; the drained gate keeps the transition after the last
// output byte so the terminal replay of an exited session is complete. Unread
// mailbox counts bump only the revision (broadcast), never the registry file.
def public herd_tick() {
    var save_dirty = false
    var revision_dirty = false
    for (record in g_herd) {
        if (!empty(record.pty_session_id)) {
            let unread = watcher_mailbox_unread_outbox(record.pty_session_id)
            if (unread != record.unread_outbox) {
                record.unread_outbox = unread
                revision_dirty = true
            }
        }
        if (record.state != "running") continue
        var info = WatcherSessionInfo()
        if (!watcher_get_session_info(record.pty_session_id, info)) {
            record.state = "exited"
            record.reason = "pty_missing"
            record.updated_wall_s = wall_seconds()
            save_dirty = true
            continue
        }
        if ((info.state == "exited" || info.state == "failed") && info.drained) {
            record.state = "exited"
            record.reason = info.state == "failed" ? info.reason : "process_exited"
            record.exit_code = info.exit_code
            record.updated_wall_s = wall_seconds()
            save_dirty = true
        }
    }
    if (refresh_conflicts()) {
        revision_dirty = true
    }
    if (save_dirty) {
        let _ = save_registry()
    }
    if (save_dirty || revision_dirty) {
        g_herd_revision++
    }
}

def public herd_close(herd_id : string; var pty_to_terminate : string&) : string {
    pty_to_terminate = ""
    let index = herd_find(herd_id)
    if (index < 0) return "unknown herd session"
    if (g_herd[index].state == "closed") return ""
    if (g_herd[index].state == "running") {
        pty_to_terminate = g_herd[index].pty_session_id
    }
    g_herd[index].state = "closed"
    g_herd[index].reason = "closed_by_user"
    g_herd[index].updated_wall_s = wall_seconds()
    g_herd_revision++
    return save_registry()
}

def public herd_rename(herd_id, name : string) : string {
    if (empty(strip(name))) return "session name is required"
    let index = herd_find(herd_id)
    if (index < 0) return "unknown herd session"
    g_herd[index].name = name
    g_herd[index].updated_wall_s = wall_seconds()
    g_herd_revision++
    return save_registry()
}

def public herd_add_satellite(herd_id, repository_id, worktree_path, added_by : string) : string {
    let index = herd_find(herd_id)
    if (index < 0) return "unknown herd session"
    if (empty(worktree_path)) return "worktree path is required"
    let display_path = herd_display_worktree(worktree_path)
    let canonical = herd_canonical_worktree(display_path)
    if (herd_canonical_worktree(g_herd[index].worktree_path) == canonical) return ""
    for (entry in g_herd[index].satellites) {
        if (herd_canonical_worktree(entry.worktree_path) == canonical) return ""
    }
    g_herd[index].satellites |> push(HerdWorkspaceEntry(repository_id = repository_id,
        worktree_path = display_path, added_by = added_by))
    g_herd[index].updated_wall_s = wall_seconds()
    g_herd_revision++
    return save_registry()
}
