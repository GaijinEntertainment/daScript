options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_focus

require daslib/json_boost
require ./watcher_core.das
require ./rich_state.das
require ./rich_net.das

def select_bundle_participant(participant : WatcherBundleParticipant const) {
    if (g_client == null) return
    g_selected_repository_id = participant.repository_id
    g_selected_worktree_path = participant.worktree_path
    g_selection_explicit = true
    g_client->refresh_repository(participant.repository_id)
    g_client->request_repository_review(participant.repository_id,
        participant.worktree_path)
    g_open_git_windows_pending = true
}

def activate_bundle_focus(bundle : WatcherReviewBundle const;
                          repository_id : string = "";
                          worktree_path : string = "";
                          file_path : string = "") : bool {
    if (g_client == null) return false
    for (message in g_client.mailbox) {
        if (message.direction != "outbox" || message.bundle_id != bundle.id) continue
        for (target_index in range(length(message.focus.targets))) {
            let target & = unsafe(message.focus.targets[target_index])
            let target_repository = watcher_focus_repository(message.focus, target)
            let target_worktree = watcher_focus_worktree(message.focus, target)
            if ((!empty(repository_id) && target_repository != repository_id)
                    || (!empty(worktree_path) && target_worktree != worktree_path)
                    || (!empty(file_path) && target.file_path != file_path)) continue
            activate_focus_target(message, target_index)
            return true
        }
    }
    return false
}

def activate_focus_target(message : WatcherMailboxMessage const;
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
    g_selection_explicit = true
    if (message.status == "new") g_client->acknowledge_mailbox(message.id)
    if (selection_changed) {
        g_client->refresh_repository(repository_id)
        g_client->request_repository_review(repository_id, worktree_path)
        g_client->request_repository_history(repository_id, worktree_path)
        g_client->request_repository_refs(repository_id, worktree_path)
    }
    g_client->inspect_file(repository_id, worktree_path, target.file_path,
        comparison, "full", revision)
    g_open_inspector_pending = true
}

def navigate_active_focus(delta : int) {
    if (g_client == null || empty(g_active_focus_message)) return
    for (message in g_client.mailbox) {
        if (message.id != g_active_focus_message || empty(message.focus.targets)) continue
        let target_index = (g_active_focus_target + delta
            + length(message.focus.targets)) % length(message.focus.targets)
        activate_focus_target(message, target_index)
        return
    }
}

def active_focus_range_count() : int {
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

def navigate_active_focus_range(delta : int) {
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
