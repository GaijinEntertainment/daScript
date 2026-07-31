options gen2
options indenting = 4

module changelist_selection shared public

require math
require strings public

// The Changelist multiselect. Pure logic, no ImGui: the UI reports clicks
// (with modifier flags) and renders from the set; actions receive explicit
// path lists derived from it. A selection is scoped to ONE worktree — any
// interaction under a different scope clears it first, so a stale set can
// never leak a path from another tree into an argv (a cross-tree write).
struct public GitChangelistSelection {
    repository_id : string
    worktree_path : string
    // Set of "{comparison}:{path}" keys — the id the rows already build.
    keys : table<string>
    // The last plain-clicked row: Shift ranges extend from here, and only
    // within the anchor's group (STAGED and UNTRACKED need different git
    // commands, so a range spanning them cannot map to one action).
    anchor_key : string
    anchor_group : int
}

def public selection_key(comparison, path : string) : string => "{comparison}:{path}"

def public selection_count(sel : GitChangelistSelection) : int => length(sel.keys)

def public selection_contains(sel : GitChangelistSelection; key : string) : bool {
    return key_exists(sel.keys, key)
}

def public selection_clear(var sel : GitChangelistSelection) {
    sel.repository_id = ""
    sel.worktree_path = ""
    sel.keys |> clear()
    sel.anchor_key = ""
    sel.anchor_group = -1
}

def private selection_rescope(var sel : GitChangelistSelection;
                              repository_id, worktree_path : string) {
    if (sel.repository_id == repository_id
            && sel.worktree_path == worktree_path) return
    selection_clear(sel)
    sel.repository_id = repository_id
    sel.worktree_path = worktree_path
}

def public selection_scope(var sel : GitChangelistSelection;
                           repository_id, worktree_path : string) {
    //! Called by the view every frame with the worktree it is showing:
    //! changing worktree or repository clears the set.
    if ((empty(sel.repository_id) && empty(sel.worktree_path))
            || (sel.repository_id == repository_id
                && sel.worktree_path == worktree_path)) return
    selection_clear(sel)
}

def public selection_prune(var sel : GitChangelistSelection;
                           live_keys : table<string>) {
    //! Called on every refreshed status list with the keys it actually offers.
    //! A key whose row LEFT the list is dropped, so a file that later comes
    //! back to that group (staged, then edited again into "MM") returns
    //! UNSELECTED instead of silently riding the next stage or discard batch.
    return if (empty(sel.keys))
    var inscope stale <- [for (key in keys(sel.keys)); clone_string(key);
        where !key_exists(live_keys, key)]
    for (key in stale) {
        sel.keys |> erase(key)
    }
    if (!empty(sel.anchor_key) && !key_exists(sel.keys, sel.anchor_key)) {
        sel.anchor_key = ""
        sel.anchor_group = -1
    }
}

def public selection_plain_click(var sel : GitChangelistSelection;
                                 repository_id, worktree_path : string;
                                 group : int; key : string) {
    //! Plain click: select ONLY this row and anchor future ranges here. The
    //! caller also opens the row in the inspector — that part is not ours.
    selection_rescope(sel, repository_id, worktree_path)
    sel.keys |> clear()
    sel.keys |> insert(key)
    sel.anchor_key = key
    sel.anchor_group = group
}

def public selection_toggle_click(var sel : GitChangelistSelection;
                                  repository_id, worktree_path : string;
                                  group : int; key : string) {
    //! Ctrl+click: toggle this row, leave the rest (and the inspector) alone.
    selection_rescope(sel, repository_id, worktree_path)
    if (key_exists(sel.keys, key)) {
        sel.keys |> erase(key)
        if (sel.anchor_key == key) {
            sel.anchor_key = ""
            sel.anchor_group = -1
        }
    } else {
        sel.keys |> insert(key)
        sel.anchor_key = key
        sel.anchor_group = group
    }
}

def public selection_range_click(var sel : GitChangelistSelection;
                                 repository_id, worktree_path : string;
                                 group : int; group_keys : array<string>;
                                 key : string) {
    //! Shift+click: select the range from the anchor to this row within one
    //! group, replacing the current set (the file-manager convention). An
    //! anchor in ANOTHER group cannot range here — that degrades to a plain
    //! click rather than silently spanning two incompatible git commands.
    selection_rescope(sel, repository_id, worktree_path)
    var anchor_index = -1
    var target_index = -1
    for (index in range(length(group_keys))) {
        if (group_keys[index] == sel.anchor_key) {
            anchor_index = index
        }
        if (group_keys[index] == key) {
            target_index = index
        }
    }
    if (target_index < 0) return
    if (anchor_index < 0 || sel.anchor_group != group) {
        selection_plain_click(sel, repository_id, worktree_path, group, key)
        return
    }
    sel.keys |> clear()
    let low = min(anchor_index, target_index)
    let high = max(anchor_index, target_index)
    for (index in range(low, high + 1)) {
        sel.keys |> insert(group_keys[index])
    }
}

def public selection_group_state(sel : GitChangelistSelection;
                                 group_keys : array<string>) : int {
    //! Tri-state for the group header checkbox: 0 none, 1 some, 2 all.
    var selected = 0
    for (key in group_keys) {
        if (key_exists(sel.keys, key)) selected++
    }
    if (selected == 0) return 0
    return selected == length(group_keys) ? 2 : 1
}

def public selection_set_group(var sel : GitChangelistSelection;
                               repository_id, worktree_path : string;
                               group : int; group_keys : array<string>;
                               selected : bool) {
    //! The header checkbox: select or deselect every row of one group without
    //! touching what is selected in the other groups.
    selection_rescope(sel, repository_id, worktree_path)
    for (key in group_keys) {
        if (selected) {
            sel.keys |> insert(key)
        } elif (key_exists(sel.keys, key)) {
            sel.keys |> erase(key)
        }
    }
    if (selected && !empty(group_keys)) {
        sel.anchor_key = group_keys[0]
        sel.anchor_group = group
    }
}
