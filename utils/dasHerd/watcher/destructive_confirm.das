options gen2
options indenting = 4

module destructive_confirm shared public

require strings public

// The one confirmation modal every destructive verb routes through. Pure
// logic, no ImGui: the UI captures what the dialog names when it opens, and
// the Confirm button asks this module what to send.
struct public DestructiveConfirm {
    // "terminate" | "herd_close" | "worktree_delete"
    kind : string
    // Session id, herd session id, or worktree path, per kind.
    target : string
    // Only worktree_delete needs one; captured beside the target.
    repository : string
    text : string
}

struct public DestructiveAction {
    // Empty kind means "send nothing" — a confirmation that cannot name what
    // it acts on must do nothing rather than aim at whatever is current.
    kind : string
    target : string
    repository : string
}

def public destructive_confirm_open(kind, target, text : string;
                                    repository : string = "") : DestructiveConfirm {
    //! Capture what the dialog is about to name, at open time.
    return DestructiveConfirm(kind = kind, target = target,
        repository = repository, text = text)
}

def public destructive_confirm_action(confirm : DestructiveConfirm const) : DestructiveAction {
    //! What Confirm must send. Every field comes from the capture: the
    //! selection, the session list and the Git inventory all move while a modal
    //! sits open, so anything re-read here is not what the dialog named.
    if (empty(confirm.target)) return DestructiveAction()
    if (confirm.kind == "terminate" || confirm.kind == "herd_close") {
        return DestructiveAction(kind = confirm.kind, target = confirm.target)
    }
    if (confirm.kind == "worktree_delete") {
        // With no repository the watcher, not the dialog, would decide which
        // worktree this names.
        if (empty(confirm.repository)) return DestructiveAction()
        return DestructiveAction(kind = confirm.kind, target = confirm.target,
            repository = confirm.repository)
    }
    return DestructiveAction()
}
