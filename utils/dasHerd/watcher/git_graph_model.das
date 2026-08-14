options gen2
options gc
options persistent_heap
options indenting = 4

module git_graph_model shared public

require ./repository_core.das public
require daslib/strings_boost
require math
require strings

enum GitTreeFilter {
    auto_
    all
}

struct GitTreeNode {
    commit_index : int = -1
    hash : string
    row : int
    lane : int
    color_slot : int
    head : bool
    pushed : bool
    focused : bool
    focus_head : bool
    refs_text : string
    primary_ref : string
}

struct GitTreeEdge {
    child : int = -1
    parent : int = -1
    color_slot : int
    focused : bool
}

struct GitTreeModel {
    nodes : array<GitTreeNode>
    edges : array<GitTreeEdge>
    max_lane : int
    included_ref_count : int
    focus_ref : string
    base_ref : string
    filter : GitTreeFilter
    def operator delete {
        delete nodes
        delete edges
    }
}

def public git_tree_color_slot(identity : string) : int {
    var value = 2166136261u
    peek_data(identity) $(bytes) {
        for (index in range(length(identity))) {
            value = (value ^ uint(bytes[index])) * 16777619u
        }
    }
    return int(value % 12u)
}

def private graph_lane_index(lanes : array<string> const; hash : string) : int {
    for (index in range(length(lanes))) {
        if (lanes[index] == hash) return index
    }
    return -1
}

def private graph_ref_index(repository : RepositorySnapshot const;
                            worktree_path, name : string) : int {
    for (index in range(length(repository.refs))) {
        let ref = repository.refs[index]
        if (ref.worktree_path == worktree_path
                && (ref.name == name || ref.full_name == name)) return index
    }
    return -1
}

def private graph_base_ref(repository : RepositorySnapshot const;
                           worktree : WorktreeState const) : string {
    if (!empty(worktree.review_base_ref)
            && graph_ref_index(repository, worktree.path,
                worktree.review_base_ref) >= 0) return worktree.review_base_ref
    for (candidate in ["origin/master", "origin/main", "master", "main"]) {
        if (graph_ref_index(repository, worktree.path, candidate) >= 0) {
            return candidate
        }
    }
    return ""
}

def private graph_mark_ancestors(head : string;
                                 commits : array<GitCommitState> const;
                                 commit_by_hash : table<string; int> const;
                                 var marked : table<string; bool>) {
    if (empty(head) || !key_exists(commit_by_hash, head)) return
    var inscope pending : array<string>
    pending |> push(head)
    while (!empty(pending)) {
        let hash = pending[length(pending) - 1]
        pending |> pop()
        if (key_exists(marked, hash) || !key_exists(commit_by_hash, hash)) continue
        marked[hash] = true
        let commit = commits[commit_by_hash?[hash] ?? -1]
        for (parent in split(commit.parents, " ")) {
            if (!empty(parent) && key_exists(commit_by_hash, parent)
                    && !key_exists(marked, parent)) pending |> push(parent)
        }
    }
}

def private graph_owned_commit_key(owner, hash : string) : string {
    return "{owner}\n{hash}"
}

def private graph_mark_owned_ancestors(owner, head : string;
                                       commits : array<GitCommitState> const;
                                       commit_by_hash : table<string; int> const;
                                       var marked : table<string; bool>) {
    if (empty(owner) || empty(head) || !key_exists(commit_by_hash, head)) return
    var inscope pending : array<string>
    pending |> push(head)
    while (!empty(pending)) {
        let hash = pending[length(pending) - 1]
        pending |> pop()
        let key = graph_owned_commit_key(owner, hash)
        if (key_exists(marked, key) || !key_exists(commit_by_hash, hash)) continue
        marked[key] = true
        let commit = commits[commit_by_hash?[hash] ?? -1]
        for (parent in split(commit.parents, " ")) {
            if (!empty(parent) && key_exists(commit_by_hash, parent)) {
                pending |> push(parent)
            }
        }
    }
}

def private graph_mark_ref_pushed(identity : string; ref_index : int;
                                  repository : RepositorySnapshot const;
                                  worktree_path : string;
                                  commit_by_hash : table<string; int> const;
                                  var marked : table<string; bool>) {
    if (ref_index < 0 || ref_index >= length(repository.refs)) return
    let ref = repository.refs[ref_index]
    var remote_head = ref.remote ? ref.hash : ""
    if (!ref.remote && !empty(ref.upstream)) {
        let upstream_index = graph_ref_index(
            repository, worktree_path, ref.upstream)
        if (upstream_index >= 0 && repository.refs[upstream_index].remote) {
            remote_head = repository.refs[upstream_index].hash
        }
    }
    graph_mark_owned_ancestors(identity, remote_head,
        repository.history_commits, commit_by_hash, marked)
}

def private graph_assign_first_parent(head, identity : string;
                                      commits : array<GitCommitState> const;
                                      commit_by_hash : table<string; int> const;
                                      visible : table<string; bool> const;
                                      var owner_slot : table<string; int>;
                                      var owner_name : table<string; string>) {
    var hash = head
    let slot = git_tree_color_slot(identity)
    while (!empty(hash) && key_exists(commit_by_hash, hash)
            && key_exists(visible, hash) && !key_exists(owner_slot, hash)) {
        owner_slot[hash] = slot
        owner_name[hash] = identity
        let parents = split(commits[commit_by_hash?[hash] ?? -1].parents, " ")
        hash = !empty(parents) ? parents[0] : ""
    }
}

def private graph_append_ref_label(var text : string&; name : string;
                                   var count : int&) {
    if (count >= 4 || empty(name) || ends_with(name, "/HEAD")) return
    if (!empty(text)) {
        text += " "
    }
    text += "[{name}]"
    count++
}

def public git_tree_build_model(repository : RepositorySnapshot const; // nolint:STYLE037,STYLE038 — one commit-graph layout pipeline; every phase consumes the tables the previous phase built
                                worktree : WorktreeState const;
                                filter : GitTreeFilter;
                                requested_focus_ref : string;
                                var model : GitTreeModel) {
    delete model.nodes
    delete model.edges
    model.max_lane = 0
    model.included_ref_count = 0
    model.filter = filter

    var inscope commit_by_hash : table<string; int>
    for (index in range(length(repository.history_commits))) {
        let commit = repository.history_commits[index]
        if (commit.worktree_path == worktree.path) {
            commit_by_hash[commit.hash] = index
        }
    }

    model.base_ref = graph_base_ref(repository, worktree)
    let base_ref_index = graph_ref_index(repository, worktree.path, model.base_ref)
    let base_hash = base_ref_index >= 0 ? repository.refs[base_ref_index].hash : ""
    var inscope base_ancestors : table<string; bool>
    graph_mark_ancestors(base_hash, repository.history_commits,
        commit_by_hash, base_ancestors)

    var inscope included_refs : table<int; bool>
    for (index in range(length(repository.refs))) {
        let ref = repository.refs[index]
        if (ref.worktree_path != worktree.path || ends_with(ref.name, "/HEAD")
                || !key_exists(commit_by_hash, ref.hash)) continue
        var should_include = (filter == GitTreeFilter.all)
        if (filter == GitTreeFilter.auto_) {
            let commit_row = commit_by_hash?[ref.hash] ?? -1
            should_include = (ref.current || ref.name == worktree.branch
                || ref.name == model.base_ref || ref.full_name == model.base_ref
                || (!ref.remote && (!key_exists(base_ancestors, ref.hash)
                    || commit_row < 64)))
        }
        if (should_include) {
            included_refs[index] = true
        }
    }
    // Auto retains the configured upstream for every included local branch so
    // pushed state and coalesced local/remote heads remain intelligible.
    if (filter == GitTreeFilter.auto_) {
        for (index in range(length(repository.refs))) {
            if (!key_exists(included_refs, index)) continue
            let ref = repository.refs[index]
            if (ref.remote || empty(ref.upstream)) continue
            let upstream_index = graph_ref_index(
                repository, worktree.path, ref.upstream)
            if (upstream_index >= 0) {
                included_refs[upstream_index] = true
            }
        }
    }
    model.included_ref_count = length(included_refs)

    var inscope visible : table<string; bool>
    for (index in range(length(repository.refs))) {
        if (!key_exists(included_refs, index)) continue
        graph_mark_ancestors(repository.refs[index].hash,
            repository.history_commits, commit_by_hash, visible)
    }
    if (empty(visible) && key_exists(commit_by_hash, worktree.head)) {
        graph_mark_ancestors(worktree.head, repository.history_commits,
            commit_by_hash, visible)
    }

    model.focus_ref = (empty(requested_focus_ref)
        ? worktree.branch : requested_focus_ref)
    var focus_ref_index = graph_ref_index(
        repository, worktree.path, model.focus_ref)
    if (focus_ref_index < 0) {
        model.focus_ref = worktree.branch
        focus_ref_index = graph_ref_index(
            repository, worktree.path, model.focus_ref)
    }
    let focus_hash = (focus_ref_index >= 0
        ? repository.refs[focus_ref_index].hash : worktree.head)
    var inscope focus_range : table<string; bool>
    var inscope focus_pending : array<string>
    if (!empty(focus_hash)) focus_pending |> push(focus_hash)
    while (!empty(focus_pending)) {
        let hash = focus_pending[length(focus_pending) - 1]
        focus_pending |> pop()
        if (key_exists(focus_range, hash) || key_exists(base_ancestors, hash)
                || !key_exists(commit_by_hash, hash)) continue
        focus_range[hash] = true
        let commit = repository.history_commits[commit_by_hash?[hash] ?? -1]
        for (parent in split(commit.parents, " ")) {
            if (!empty(parent)) focus_pending |> push(parent)
        }
    }

    var inscope owner_slot : table<string; int>
    var inscope owner_name : table<string; string>
    if (base_ref_index >= 0) {
        graph_assign_first_parent(base_hash, model.base_ref,
            repository.history_commits, commit_by_hash, visible,
            owner_slot, owner_name)
    }
    if (focus_ref_index >= 0) {
        graph_assign_first_parent(focus_hash, model.focus_ref,
            repository.history_commits, commit_by_hash, visible,
            owner_slot, owner_name)
    }
    for (index in range(length(repository.refs))) {
        if (!key_exists(included_refs, index)) continue
        let ref = repository.refs[index]
        graph_assign_first_parent(ref.hash, ref.name,
            repository.history_commits, commit_by_hash, visible,
            owner_slot, owner_name)
    }

    var inscope pushed_by_owner : table<string; bool>
    graph_mark_ref_pushed(model.base_ref, base_ref_index,
        repository, worktree.path, commit_by_hash, pushed_by_owner)
    graph_mark_ref_pushed(model.focus_ref, focus_ref_index,
        repository, worktree.path, commit_by_hash, pushed_by_owner)
    for (index in range(length(repository.refs))) {
        if (!key_exists(included_refs, index)) continue
        graph_mark_ref_pushed(repository.refs[index].name, index,
            repository, worktree.path, commit_by_hash, pushed_by_owner)
    }

    var inscope node_by_hash : table<string; int>
    for (commit_index in range(length(repository.history_commits))) {
        let commit = repository.history_commits[commit_index]
        if (commit.worktree_path != worktree.path
                || !key_exists(visible, commit.hash)) continue
        var refs_text : string
        var refs_count = 0
        var primary_ref : string
        var primary_remote = true
        var head = false
        for (ref_index in range(length(repository.refs))) {
            if (!key_exists(included_refs, ref_index)) continue
            let ref = repository.refs[ref_index]
            if (ref.hash != commit.hash) continue
            head = true
            graph_append_ref_label(refs_text, ref.name, refs_count)
            if (empty(primary_ref) || ref.current
                    || (!ref.remote && primary_remote)) {
                primary_ref = ref.name
                primary_remote = ref.remote
            }
        }
        let color_slot = (key_exists(owner_slot, commit.hash)
            ? (owner_slot?[commit.hash] ?? 0)
            : git_tree_color_slot(commit.hash))
        let node_index = length(model.nodes)
        node_by_hash[commit.hash] = node_index
        let owner = key_exists(owner_name, commit.hash) ? (owner_name?[commit.hash] ?? "") : ""
        model.nodes |> push(GitTreeNode(
            commit_index = commit_index, hash = commit.hash,
            row = node_index, color_slot = color_slot,
            head = head, pushed = key_exists(pushed_by_owner,
                graph_owned_commit_key(owner, commit.hash)),
            focused = key_exists(focus_range, commit.hash),
            focus_head = commit.hash == focus_hash,
            refs_text = refs_text, primary_ref = primary_ref))
    }

    var inscope lanes : array<string>
    for (node_index in range(length(model.nodes))) {
        var node & = unsafe(model.nodes[node_index])
        var lane = graph_lane_index(lanes, node.hash)
        if (lane < 0) {
            lanes |> push(node.hash)
            lane = length(lanes) - 1
        }
        node.lane = lane
        model.max_lane = max(model.max_lane, lane)
        let commit = repository.history_commits[node.commit_index]
        var inscope visible_parents : array<string>
        for (parent in split(commit.parents, " ")) {
            if (!empty(parent) && key_exists(node_by_hash, parent)) {
                visible_parents |> push(parent)
            }
        }
        if (empty(visible_parents)) {
            lanes |> erase(lane)
        } else {
            let first_parent = visible_parents[0]
            let existing = graph_lane_index(lanes, first_parent)
            if (existing >= 0 && existing != lane) {
                lanes |> erase(lane)
            } else {
                lanes[lane] = first_parent
            }
            for (parent_index in range(1, length(visible_parents))) {
                let parent = visible_parents[parent_index]
                if (graph_lane_index(lanes, parent) < 0) lanes |> push(parent)
            }
        }
    }

    for (child_index in range(length(model.nodes))) {
        let child = model.nodes[child_index]
        let commit = repository.history_commits[child.commit_index]
        for (parent in split(commit.parents, " ")) {
            if (empty(parent) || !key_exists(node_by_hash, parent)) continue
            let parent_index = node_by_hash?[parent] ?? -1
            model.edges |> push(GitTreeEdge(
                child = child_index, parent = parent_index,
                color_slot = model.nodes[parent_index].color_slot,
                focused = child.focused))
        }
    }
}
