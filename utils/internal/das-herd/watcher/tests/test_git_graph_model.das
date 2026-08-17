options gen2
options rtti

require dastest/testing_boost public
require ../git_graph_model.das

def private graph_commit(hash, parents : string; time : int64) : GitCommitState {
    return GitCommitState(worktree_path = "D:/Work/tree", hash = hash,
        short_hash = hash, authored_at = time, author = "Agent",
        subject = hash, parents = parents)
}

def private graph_ref(name, hash : string; remote : bool = false;
                      current : bool = false; upstream : string = "") : GitRefState {
    return GitRefState(worktree_path = "D:/Work/tree",
        full_name = remote ? "refs/remotes/{name}" : "refs/heads/{name}",
        name = name, hash = hash, current = current,
        remote = remote, upstream = upstream)
}

def private graph_node(model : GitTreeModel const; hash : string) : int {
    for (index in range(length(model.nodes))) {
        if (model.nodes[index].hash == hash) return index
    }
    return -1
}

[test]
def test_git_graph_model(t : T?) {
    t |> run("semantic Tree model retains branch, push, and focus roles") <| @(t : T?) {
        var repository = RepositorySnapshot()
        repository.history_commits <- [
            graph_commit("F2", "F1", 60l),
            graph_commit("P1", "B", 55l),
            graph_commit("F1", "B", 50l),
            graph_commit("B", "A", 40l),
            graph_commit("A", "", 30l)]
        repository.refs <- [
            graph_ref("origin/feature", "F1", true),
            graph_ref("feature", "F2", false, true, "origin/feature"),
            graph_ref("origin/parallel", "P1", true),
            graph_ref("parallel", "P1", false, false, "origin/parallel"),
            graph_ref("origin/master", "B", true),
            graph_ref("merged-label", "B"),
            graph_ref("origin/unrelated", "F2", true)]
        let worktree = WorktreeState(path = "D:/Work/tree", head = "F2",
            branch = "feature", review_base_ref = "origin/master")
        var model = GitTreeModel()
        git_tree_build_model(repository, worktree,
            GitTreeFilter.auto_, "feature", model)
        t |> equal(length(model.nodes), 5)
        t |> equal(length(model.edges), 4)
        t |> success(model.included_ref_count >= 5,
            "Auto includes outgoing locals, upstreams, base, and recent merged refs")
        let f2 = graph_node(model, "F2")
        let f1 = graph_node(model, "F1")
        let p1 = graph_node(model, "P1")
        let b = graph_node(model, "B")
        t |> success(f2 >= 0 && f1 >= 0 && p1 >= 0 && b >= 0)
        t |> success(model.nodes[f2].head && model.nodes[f2].focus_head)
        t |> equal(model.nodes[f2].refs_text, "[feature]")
        t |> equal(model.nodes[f2].primary_ref, "feature")
        t |> success(!model.nodes[f2].pushed,
            "local head beyond its upstream remains visibly unpushed")
        t |> success(model.nodes[f1].head && model.nodes[f1].pushed)
        t |> success(model.nodes[f2].focused && model.nodes[f1].focused)
        t |> success(!model.nodes[b].focused,
            "selected outgoing range stops at base ancestry")
        t |> success(model.nodes[p1].head && model.nodes[p1].pushed)
        t |> equal(model.nodes[p1].primary_ref, "parallel",
            "a local branch remains the primary badge even when its remote sorts first")
        t |> success(model.max_lane > 0,
            "parallel history occupies a separate semantic lane")

        git_tree_build_model(repository, worktree,
            GitTreeFilter.all, "feature", model)
        let all_f2 = graph_node(model, "F2")
        t |> success(all_f2 >= 0 && !model.nodes[all_f2].pushed,
            "a commit on an unrelated remote is not pushed for the owning branch")
        delete model
        delete repository.history_commits
        delete repository.refs
    }
}
