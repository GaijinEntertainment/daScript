options gen2
options no_aot
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/fio
require strings
require ../../utils/watchdog/lsp_front.das

//! the LSP front's cross-tree guard over throwaway git-style trees: a file in another tree than
//! the compiler warns with the hint that tree's state calls for, the compiler's own tree is
//! silent, and a `project_root` scoping the file silences it on purpose

def private make_tree(base, name : string; with_bin, with_mcp : bool) : string {
    let root = path_join(base, name)
    mkdir(root)
    mkdir(path_join(root, "sub"))
    fwrite(path_join(root, ".git"), "gitdir: elsewhere")
    if (with_bin) {
        mkdir(path_join(root, "bin"))
        fwrite(path_join(root, "bin/daslang"), "")
    }
    if (with_mcp) {
        fwrite(path_join(root, ".mcp.json"), "\{\}")
    }
    return root
}

[test]
def test_crosstree_guard(t : T?) {
    let tmp = create_temp_directory_result("das lsp crosstree ")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let base = tmp as value
    let binroot = make_tree(base, "binder", true, false)
    let wt_bin = make_tree(base, "worktree-bin", true, false)
    let wt_boot = make_tree(base, "worktree-boot", true, true)
    let wt_bare = make_tree(base, "worktree-bare", false, false)
    let compiler = path_join(binroot, "bin/daslang")

    t |> run("git_tree_root walks up to the .git entry") @(t : T?) {
        t |> equal(git_tree_root(path_join(wt_bin, "sub")), get_full_file_name(wt_bin))
    }
    t |> run("a tree with a binary and no MCP config warns with the --no-build hint") @(t : T?) {
        let d = crosstree_diagnostic(path_join(wt_bin, "sub/x.das"), compiler, "")
        t |> success(find(d, "CROSS-TREE") >= 0, "warns: {d}")
        t |> success(find(d, "\"severity\":2") >= 0, "a warning, not an error: {d}")
        t |> success(find(d, "no MCP config") >= 0 && find(d, "--no-build") >= 0, "the bin-but-no-mcp hint: {d}")
    }
    t |> run("a bootstrapped tree says to open a session there") @(t : T?) {
        let d = crosstree_diagnostic(path_join(wt_boot, "sub/x.das"), compiler, "")
        t |> success(find(d, "bootstrapped") >= 0 && find(d, "session rooted") >= 0, "the bootstrapped hint: {d}")
    }
    t |> run("a bare tree says it is not bootstrapped") @(t : T?) {
        let d = crosstree_diagnostic(path_join(wt_bare, "sub/x.das"), compiler, "")
        t |> success(find(d, "isn't bootstrapped") >= 0, "the not-bootstrapped hint: {d}")
    }
    t |> run("the compiler's own tree is silent") @(t : T?) {
        t |> equal(crosstree_diagnostic(path_join(binroot, "sub/y.das"), compiler, ""), "")
    }
    t |> run("a project_root scoping the file silences the warning") @(t : T?) {
        t |> equal(crosstree_diagnostic(path_join(wt_bin, "sub/x.das"), compiler, wt_bin), "")
    }
    let cleanup = rmdir_rec_result(base)
    t |> success(cleanup is value && cleanup as value, "temp dir removed")
}
