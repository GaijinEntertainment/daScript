options gen2

require dastest/testing_boost public
require ../changelist_selection.das

[test]
def test_selection_click_contract(t : T?) {
    t |> run("plain click selects only this row and moves the anchor") <| @(t : T?) {
        var sel = GitChangelistSelection()
        selection_plain_click(sel, "repo", "D:/Work/tree", 2,
            selection_key("working", "a.das"))
        selection_plain_click(sel, "repo", "D:/Work/tree", 2,
            selection_key("working", "b.das"))
        t |> equal(selection_count(sel), 1)
        t |> success(selection_contains(sel, "working:b.das"))
        t |> equal(sel.anchor_key, "working:b.das")
        selection_clear(sel)
    }
    t |> run("ctrl click toggles without touching the rest") <| @(t : T?) {
        var sel = GitChangelistSelection()
        selection_plain_click(sel, "repo", "D:/Work/tree", 2, "working:a.das")
        selection_toggle_click(sel, "repo", "D:/Work/tree", 2, "working:b.das")
        t |> equal(selection_count(sel), 2)
        selection_toggle_click(sel, "repo", "D:/Work/tree", 2, "working:a.das")
        t |> equal(selection_count(sel), 1)
        t |> success(selection_contains(sel, "working:b.das"))
        selection_toggle_click(sel, "repo", "D:/Work/tree", 2, "working:b.das")
        t |> equal(selection_count(sel), 0,
            "toggling the last row empties the set")
        selection_clear(sel)
    }
}

[test]
def test_selection_range_contract(t : T?) {
    t |> run("shift click ranges from the anchor within one group") <| @(t : T?) {
        var sel = GitChangelistSelection()
        let keys <- [selection_key("working", "a.das"),
            selection_key("working", "b.das"),
            selection_key("working", "c.das"),
            selection_key("working", "d.das")]
        selection_plain_click(sel, "repo", "D:/Work/tree", 2, keys[1])
        selection_range_click(sel, "repo", "D:/Work/tree", 2, keys, keys[3])
        t |> equal(selection_count(sel), 3)
        t |> success(!selection_contains(sel, keys[0]))
        t |> success(selection_contains(sel, keys[1]))
        t |> success(selection_contains(sel, keys[3]))
        // Reverse direction works the same.
        selection_range_click(sel, "repo", "D:/Work/tree", 2, keys, keys[0])
        t |> equal(selection_count(sel), 2)
        t |> success(selection_contains(sel, keys[0]))
        t |> success(selection_contains(sel, keys[1]))
        selection_clear(sel)
    }
    t |> run("a range refuses to cross group boundaries") <| @(t : T?) {
        var sel = GitChangelistSelection()
        let staged_keys <- [selection_key("staged", "a.das")]
        let working_keys <- [selection_key("working", "b.das"),
            selection_key("working", "c.das")]
        selection_plain_click(sel, "repo", "D:/Work/tree", 1, staged_keys[0])
        // The anchor lives in STAGED; a shift-click in MODIFIED degrades to a
        // plain click there instead of spanning two git commands.
        selection_range_click(sel, "repo", "D:/Work/tree", 2, working_keys,
            working_keys[1])
        t |> equal(selection_count(sel), 1)
        t |> success(selection_contains(sel, working_keys[1]))
        t |> equal(sel.anchor_group, 2)
        selection_clear(sel)
    }
    t |> run("shift click with no anchor is a plain click") <| @(t : T?) {
        var sel = GitChangelistSelection()
        let keys <- [selection_key("working", "a.das"),
            selection_key("working", "b.das")]
        selection_range_click(sel, "repo", "D:/Work/tree", 2, keys, keys[1])
        t |> equal(selection_count(sel), 1)
        t |> success(selection_contains(sel, keys[1]))
        selection_clear(sel)
    }
}

[test]
def test_selection_group_checkbox(t : T?) {
    t |> run("the header checkbox is tri-state and group-local") <| @(t : T?) {
        var sel = GitChangelistSelection()
        let group_keys <- [selection_key("working", "a.das"),
            selection_key("working", "b.das")]
        let other_keys <- [selection_key("staged", "z.das")]
        t |> equal(selection_group_state(sel, group_keys), 0)
        selection_set_group(sel, "repo", "D:/Work/tree", 2, group_keys, true)
        t |> equal(selection_group_state(sel, group_keys), 2)
        selection_toggle_click(sel, "repo", "D:/Work/tree", 1, other_keys[0])
        selection_toggle_click(sel, "repo", "D:/Work/tree", 2, group_keys[0])
        t |> equal(selection_group_state(sel, group_keys), 1,
            "one of two selected reads as SOME")
        selection_set_group(sel, "repo", "D:/Work/tree", 2, group_keys, false)
        t |> equal(selection_group_state(sel, group_keys), 0)
        t |> success(selection_contains(sel, other_keys[0]),
            "deselecting one group leaves the other group's rows alone")
        selection_clear(sel)
    }
}

[test]
def test_selection_refresh_prune(t : T?) {
    t |> run("a row that left the status list drops out of the set") <| @(t : T?) {
        var sel = GitChangelistSelection()
        selection_plain_click(sel, "repo", "D:/Work/tree", 2, "working:a.txt")
        selection_toggle_click(sel, "repo", "D:/Work/tree", 2, "working:b.txt")
        // a.txt was staged: its MODIFIED row is gone, b.txt's is not.
        var live <- {"staged:a.txt", "working:b.txt"}
        selection_prune(sel, live)
        t |> equal(selection_count(sel), 1)
        t |> success(!selection_contains(sel, "working:a.txt"))
        t |> success(selection_contains(sel, "working:b.txt"))
        t |> success(!selection_contains(sel, "staged:a.txt"),
            "pruning never SELECTS a row, it only drops one")
        selection_clear(sel)
        delete live
    }
    t |> run("a file that comes back to a group comes back UNSELECTED") <| @(t : T?) {
        var sel = GitChangelistSelection()
        selection_plain_click(sel, "repo", "D:/Work/tree", 2, "working:a.txt")
        var staged <- {"staged:a.txt"}
        selection_prune(sel, staged)
        t |> equal(selection_count(sel), 0)
        // The user edits a.txt again: "MM" puts it back in MODIFIED. Without
        // the prune above it would be pre-checked here and ride the next
        // stage or DISCARD batch with no click.
        var both <- {"staged:a.txt", "working:a.txt"}
        selection_prune(sel, both)
        t |> success(!selection_contains(sel, "working:a.txt"))
        t |> equal(selection_count(sel), 0)
        selection_clear(sel)
        delete staged
        delete both
    }
    t |> run("pruning away the anchor clears it, so no range starts from a ghost") <| @(t : T?) {
        var sel = GitChangelistSelection()
        selection_plain_click(sel, "repo", "D:/Work/tree", 2, "working:a.txt")
        selection_toggle_click(sel, "repo", "D:/Work/tree", 2, "working:b.txt")
        t |> equal(sel.anchor_key, "working:b.txt")
        var live <- {"working:a.txt"}
        selection_prune(sel, live)
        t |> equal(sel.anchor_key, "")
        t |> equal(sel.anchor_group, -1)
        t |> equal(selection_count(sel), 1)
        // A surviving anchor is left alone.
        selection_toggle_click(sel, "repo", "D:/Work/tree", 2, "working:c.txt")
        var wider <- {"working:a.txt", "working:c.txt"}
        selection_prune(sel, wider)
        t |> equal(sel.anchor_key, "working:c.txt")
        selection_clear(sel)
        delete live
        delete wider
    }
    t |> run("an empty selection and an empty status list are both no-ops") <| @(t : T?) {
        var sel = GitChangelistSelection()
        var live <- {"working:a.txt"}
        selection_prune(sel, live)
        t |> equal(selection_count(sel), 0)
        selection_plain_click(sel, "repo", "D:/Work/tree", 2, "working:a.txt")
        var clean : table<string>
        selection_prune(sel, clean)
        t |> equal(selection_count(sel), 0,
            "a clean working tree offers no keys, so nothing stays selected")
        selection_clear(sel)
        delete live
        delete clean
    }
}

[test]
def test_selection_worktree_scope(t : T?) {
    t |> run("changing worktree or repository clears the set") <| @(t : T?) {
        var sel = GitChangelistSelection()
        selection_plain_click(sel, "repo", "D:/Work/tree", 2, "working:a.das")
        selection_toggle_click(sel, "repo", "D:/Work/tree", 2, "working:b.das")
        t |> equal(selection_count(sel), 2)
        // A click under another worktree drops every key from the old one:
        // the set can NEVER hold paths from two worktrees.
        selection_toggle_click(sel, "repo", "D:/Work/other", 2, "working:c.das")
        t |> equal(selection_count(sel), 1)
        t |> success(selection_contains(sel, "working:c.das"))
        t |> equal(sel.worktree_path, "D:/Work/other")
        // The view reports what it is showing; a mismatch clears too.
        selection_scope(sel, "repo2", "D:/Work/other")
        t |> equal(selection_count(sel), 0)
        // An idle empty selection does not adopt a scope from the view.
        selection_scope(sel, "repo2", "D:/Work/other")
        t |> equal(sel.repository_id, "")
        selection_clear(sel)
    }
}
