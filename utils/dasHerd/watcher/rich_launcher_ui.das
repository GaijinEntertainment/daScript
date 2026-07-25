options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_launcher_ui

require imgui/imgui_harness
require imgui/imgui_icons
require imgui/imgui_docking_builtin
require daslib/safe_addr
require daslib/strings_boost
require ./watcher_core.das
require ./herd_sessions.das
require ./repository_core.das
require math
require strings
require ./rich_state.das
require ./rich_net.das

// Dockable session launcher: profile + workflow + workspace in one pane,
// replacing the modal Codex-only dialog. Creating in an existing worktree goes
// through the durable herd rail directly; a dedicated worktree goes through
// the watcher's create-then-launch pipeline.

def agent_repository_checkout(repository_id, fallback : string) : string {
    if (g_client != null) {
        for (repository in g_client.repositories) {
            if (repository.record.id == repository_id) {
                return repository.record.checkout_path
            }
        }
    }
    return fallback
}

def set_agent_launch_source(repository_id, worktree_path : string) {
    g_agent_source_repository_id = repository_id
    g_agent_source_worktree_path = worktree_path
}

def collect_agent_launch_locations(
                                   var locations : array<AgentLaunchLocation>&) {
    delete locations
    if (g_client == null) return
    var total = 0
    for (repository in g_client.repositories) {
        total += length(repository.worktrees)
    }
    locations |> reserve(total)
    for (repository in g_client.repositories) {
        for (worktree in repository.worktrees) {
            let branch = (worktree.detached
                ? "detached {slice(worktree.head, 0, min(8, length(worktree.head)))}"
                : worktree.branch)
            locations |> push(AgentLaunchLocation(
                repository_id = repository.record.id,
                worktree_path = worktree.path,
                display_label = "{repository.record.display_name} | {branch} | {worktree.path}"))
        }
    }
}

def private launcher_name_slug : string {
    return replace(strip(LAUNCHER_NAME.value), " ", "-")
}

def private suggest_from_name() {
    let slug = launcher_name_slug()
    if (empty(slug)) return
    LAUNCHER_BRANCH.pending_value = "codex/{slug}"
    LAUNCHER_BRANCH.has_pending = true
    let checkout = agent_repository_checkout(
        g_agent_source_repository_id, g_agent_source_worktree_path)
    LAUNCHER_WORKTREE_PATH.pending_value = herd_display_worktree(path_join(
        path_join(checkout, ".codex/worktrees"), slug))
    LAUNCHER_WORKTREE_PATH.has_pending = true
}

def prepare_launcher(repository_id, worktree_path : string) {
    set_agent_launch_source(repository_id, worktree_path)
    LAUNCHER_NAME.pending_value = ""
    LAUNCHER_NAME.has_pending = true
    LAUNCHER_BRANCH.pending_value = ""
    LAUNCHER_BRANCH.has_pending = true
    LAUNCHER_BASE_REF.pending_value = "HEAD"
    LAUNCHER_BASE_REF.has_pending = true
    LAUNCHER_WORKTREE_PATH.pending_value = ""
    LAUNCHER_WORKTREE_PATH.has_pending = true
    LAUNCHER_TASK.pending_value = ""
    LAUNCHER_TASK.has_pending = true
    LAUNCHER_SOURCE.value = -1
    // Run-in-selected-worktree is the default: choosing an existing worktree
    // in "Start from" means run THERE unless the user opts into a new one.
    LAUNCHER_NEW_WORKTREE.value = false
}

def private default_profile_index : int {
    if (g_client == null) return -1
    for (index in range(length(g_client.herd_profiles))) {
        if (g_client.herd_profiles[index].id == "claude") return index
    }
    return empty(g_client.herd_profiles) ? -1 : 0
}

def draw_launcher_form() {
    if (g_client == null || !g_client.connected) {
        text_disabled("Waiting for the watcher connection...")
        return
    }
    if (empty(g_client.herd_profiles)) {
        text_disabled("Waiting for agent profiles from the watcher...")
        return
    }
    var inscope profile_labels : array<string>
    profile_labels |> reserve(length(g_client.herd_profiles))
    for (profile in g_client.herd_profiles) {
        profile_labels |> push(!empty(profile.display_name)
            ? profile.display_name : profile.id)
    }
    if (LAUNCHER_PROFILE.value < 0
            || LAUNCHER_PROFILE.value >= length(profile_labels)) {
        LAUNCHER_PROFILE.value = default_profile_index()
    }
    let _profile_changed = combo(LAUNCHER_PROFILE,
        (text = "Profile", items <- profile_labels,
         popup_max_height_in_items = 8))
    var profile_is_agent = false
    var profile_id = ""
    if (LAUNCHER_PROFILE.value >= 0
            && LAUNCHER_PROFILE.value < length(g_client.herd_profiles)) {
        let profile & = unsafe(g_client.herd_profiles[LAUNCHER_PROFILE.value])
        profile_is_agent = profile.agent
        profile_id = profile.id
    }
    var inscope workflow_labels <- ["Regular work", "Assist a PR", "Review someone's PR"]
    if (LAUNCHER_WORKFLOW.value < 0
            || LAUNCHER_WORKFLOW.value >= length(workflow_labels)) {
        LAUNCHER_WORKFLOW.value = 0
    }
    let _workflow_changed = combo(LAUNCHER_WORKFLOW,
        (text = "Workflow", items <- workflow_labels,
         popup_max_height_in_items = 4))
    let workflow_id = (LAUNCHER_WORKFLOW.value == 1 ? "assist-pr"
        : (LAUNCHER_WORKFLOW.value == 2 ? "review-pr" : "regular"))
    if (input_text_growable(LAUNCHER_NAME, (text = "Session name"))
            && LAUNCHER_NEW_WORKTREE.value) {
        suggest_from_name()
    }
    separator()
    var inscope locations : array<AgentLaunchLocation>
    collect_agent_launch_locations(locations)
    var source_index = -1
    for (index in range(length(locations))) {
        if (locations[index].repository_id == g_agent_source_repository_id
                && locations[index].worktree_path
                    == g_agent_source_worktree_path) {
            source_index = index
            break
        }
    }
    if (source_index < 0 && !empty(locations)) {
        source_index = 0
        set_agent_launch_source(locations[0].repository_id,
            locations[0].worktree_path)
    }
    LAUNCHER_SOURCE.value = source_index
    let source_labels <- [for (location in locations); location.display_label]
    if (combo(LAUNCHER_SOURCE,
            (text = "Start from", items <- source_labels,
             popup_max_height_in_items = 12))
            && LAUNCHER_SOURCE.value >= 0
            && LAUNCHER_SOURCE.value < length(locations)) {
        let selected & = unsafe(locations[LAUNCHER_SOURCE.value])
        set_agent_launch_source(selected.repository_id,
            selected.worktree_path)
        if (LAUNCHER_NEW_WORKTREE.value) suggest_from_name()
    }
    checkbox(LAUNCHER_NEW_WORKTREE, (text = "Create a dedicated worktree"))
    if (LAUNCHER_NEW_WORKTREE.value) {
        input_text_growable(LAUNCHER_BRANCH, (text = "New branch"))
        input_text_growable(LAUNCHER_BASE_REF, (text = "Base ref"))
        input_text_growable(LAUNCHER_WORKTREE_PATH, (text = "New worktree path"))
    } else {
        text_disabled("The session runs in the selected worktree.")
    }
    if (profile_is_agent) {
        text("Task (optional)")
        input_text_multiline(LAUNCHER_TASK,
            (text = "##launcher-task", size = float2(-FLT_MIN, GetTextLineHeight() * 6.0f),
             flags = ImGuiInputTextFlags.None))
    }
    separator()
    let worktree_ok = (!LAUNCHER_NEW_WORKTREE.value
        || (!empty(launcher_name_slug()) && !empty(LAUNCHER_BRANCH.value)
            && !empty(LAUNCHER_WORKTREE_PATH.value)))
    let connected = g_client != null && g_client.connected
    let launch_pending = (g_launch_pending_revision >= 0l && connected
        && g_client.herd_revision == g_launch_pending_revision
        && get_time_nsec(g_launch_pending_started) / 1000000l < 10000l)
    // ### keeps the widget identity stable while the label changes.
    let launch_label = (LAUNCHER_NEW_WORKTREE.value
        ? "Create worktree + launch###launcher-launch"
        : "Launch in {base_name(g_agent_source_worktree_path)}###launcher-launch")
    if (button(LAUNCHER_LAUNCH,
            (text = launch_label, enabled = connected && !launch_pending
                && !empty(g_agent_source_repository_id)
                && !empty(g_agent_source_worktree_path) && worktree_ok
                && !empty(profile_id)))) {
        let session_name = strip(LAUNCHER_NAME.value)
        let task = profile_is_agent ? LAUNCHER_TASK.value : ""
        ui_event("launcher-launch profile={profile_id} workflow={workflow_id}")
        if (LAUNCHER_NEW_WORKTREE.value) {
            g_client->create_worktree_and_launch_agent(
                g_agent_source_worktree_path, g_agent_source_repository_id,
                LAUNCHER_WORKTREE_PATH.value, LAUNCHER_BRANCH.value,
                LAUNCHER_BASE_REF.value, task, profile_id, session_name,
                workflow_id)
        } else {
            g_client->request_herd_create(session_name, profile_id,
                workflow_id, g_agent_source_repository_id,
                g_agent_source_worktree_path, task)
        }
        g_launch_pending_revision = g_client.herd_revision
        g_launch_pending_started = ref_time_ticks()
        LAUNCHER_WIN.open = false
    }
    if (LAUNCHER_LAUNCH.clicked) {
        // Click-frame forensics: logs every enabled term, so a click that the
        // branch above ignored explains itself in herder_ui_events.
        ui_event("launcher-button clicked pending={launch_pending} " +
            "connected={connected} profile={profile_id} worktree_ok={worktree_ok} " +
            "src={g_agent_source_repository_id}")
    }
    // The button never goes silently dead: the ladder mirrors the enabled
    // expression exactly, so every disabled state names itself.
    if (launch_pending) {
        text_disabled("Launching...")
    } elif (!connected) {
        text_disabled("Waiting for the watcher connection...")
    } elif (empty(profile_id)) {
        text_disabled("Waiting for agent profiles from the watcher...")
    } elif (empty(g_agent_source_repository_id)
            || empty(g_agent_source_worktree_path)) {
        text_disabled("Waiting for the repository list...")
    } elif (LAUNCHER_NEW_WORKTREE.value && !worktree_ok) {
        text_disabled("Name the session to derive the branch and worktree path.")
    }
}

def launcher_ui_init() {
    LAUNCHER_NAME.capacity = 1024
    LAUNCHER_BRANCH.capacity = 1024
    LAUNCHER_BASE_REF.capacity = 1024
    LAUNCHER_WORKTREE_PATH.capacity = 16 * 1024
    LAUNCHER_TASK.capacity = 16 * 1024
    LAUNCHER_NEW_WORKTREE.value = false
    LAUNCHER_PROFILE.value = -1
    LAUNCHER_WORKFLOW.value = 0
    LAUNCHER_WIN.open = false
}

def draw_launcher_dock() {
    dock_window(LAUNCHER_WIN.PUBLIC, (text = "New Session###NewSession",
            closable = true, flags = ImGuiWindowFlags.None)) {
        draw_launcher_form()
    }
}
