options gen2

require dastest/testing_boost public
require ../destructive_confirm.das

[test]
def test_confirm_survives_a_moved_selection(t : T?) {
    // The watcher re-points the attached session from the network: a "launched"
    // or "herd_relaunched" broadcast attaches whatever was just started, and a
    // pruned session hands the selection to the first running one. All of that
    // happens while a modal sits open, so a terminate confirmation must act on
    // the session it named, not on whatever is selected when Confirm is hit.
    let confirm = destructive_confirm_open("terminate", "sess-a",
        "Terminate session sess-a?")
    let action = destructive_confirm_action(confirm)
    t |> equal(action.kind, "terminate", "the confirmation still terminates")
    t |> equal(action.target, "sess-a", "it terminates the session it named")
}

[test]
def test_confirm_keeps_its_target_when_the_selection_holds(t : T?) {
    let confirm = destructive_confirm_open("terminate", "sess-a",
        "Terminate session sess-a?")
    let action = destructive_confirm_action(confirm)
    t |> equal(action.kind, "terminate", "an unmoved selection still terminates")
    t |> equal(action.target, "sess-a", "on the same session")
}

[test]
def test_confirm_close_uses_the_captured_session(t : T?) {
    let confirm = destructive_confirm_open("herd_close", "herd-7",
        "Close session 'seven' and terminate its PTY?")
    let action = destructive_confirm_action(confirm)
    t |> equal(action.kind, "herd_close", "close is dispatched")
    t |> equal(action.target, "herd-7", "on the captured herd session")
}

[test]
def test_confirm_worktree_delete_needs_its_repository(t : T?) {
    // A delete with no repository id would be aimed by the watcher instead of
    // by the dialog, which is the one thing a confirmation must not allow.
    let blank = destructive_confirm_open("worktree_delete", "D:/w/feature",
        "Delete D:/w/feature?")
    let refused = destructive_confirm_action(blank)
    t |> equal(refused.kind, "", "a repository-less delete sends nothing")

    let confirm = destructive_confirm_open("worktree_delete", "D:/w/feature",
        "Delete D:/w/feature?", "repo-1")
    let action = destructive_confirm_action(confirm)
    t |> equal(action.kind, "worktree_delete", "a captured delete is dispatched")
    t |> equal(action.target, "D:/w/feature", "on the captured worktree")
    t |> equal(action.repository, "repo-1", "in the captured repository")
}

[test]
def test_confirm_without_a_target_sends_nothing(t : T?) {
    let empty_confirm = DestructiveConfirm()
    let action = destructive_confirm_action(empty_confirm)
    t |> equal(action.kind, "", "an unset confirmation sends nothing")

    let targetless = destructive_confirm_open("terminate", "", "Terminate?")
    let nothing = destructive_confirm_action(targetless)
    t |> equal(nothing.kind, "", "a targetless confirmation sends nothing")
}
