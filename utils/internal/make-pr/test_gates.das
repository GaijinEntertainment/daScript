options gen2

require dastest/testing_boost public
require daslib/fio
require gates

[test]
def test_want_gate(t : T?) {
    let no_skip : array<string>
    t |> success(want_gate("", no_skip, "sync"), "default: every gate wanted")
    t |> success(want_gate("dupes", no_skip, "dupes"), "only-mode: named gate wanted")
    t |> success(!want_gate("dupes", no_skip, "sync"), "only-mode: other gates not wanted")
    let skip <- ["jit-smoke"]
    t |> success(!want_gate("", skip, "jit-smoke"), "skip-mode: skipped gate not wanted")
    t |> success(want_gate("", skip, "sync"), "skip-mode: others still wanted")
}

[test]
def test_unknown_gate_names(t : T?) {
    let no_skip : array<string>
    t |> equal(length(unknown_gate_names("", no_skip)), 0)
    t |> equal(length(unknown_gate_names("dupes", no_skip)), 0)
    let bad_only <- unknown_gate_names("dupess", no_skip)
    t |> equal(length(bad_only), 1)
    t |> equal(bad_only[0], "dupess")
    let skip <- ["sync", "ast-verfiy"]
    let bad_skip <- unknown_gate_names("", skip)
    t |> equal(length(bad_skip), 1)
    t |> equal(bad_skip[0], "ast-verfiy")
}

[test]
def test_scopes_of(t : T?) {
    let files <- ["utils/mcp/tools/format_file.das", "utils/mcp/registry_das.das",
                  "daslib\\das_source_formatter.das", "README.md"]
    let scopes <- scopes_of(files)
    t |> equal(length(scopes), 2, "dedup + single-component path dropped")
    t |> equal(scopes[0], "utils/mcp")
    t |> equal(scopes[1], "daslib", "a top-level file scopes to its parent DIRECTORY, not itself")
}

[test]
def test_any_path_matches(t : T?) {
    let files <- ["src\\ast\\ast_simulate.cpp", "doc/source/index.rst"]
    t |> success(any_path_matches(files, ["src/ast/"]), "backslash path normalizes before match")
    t |> success(any_path_matches(files, ["nope/", "doc/"]), "second pattern hits")
    t |> success(!any_path_matches(files, ["daslib/"]), "no false hit")
}

[test]
def test_pick_newest(t : T?) {
    var terr : string
    let dir = create_temp_directory("make_pr_pick", terr)
    if (empty(dir)) {
        t |> failure("temp dir: {terr}")
        return
    }
    let missing = path_join(dir, "missing.exe")
    let present = path_join(dir, "present.exe")
    fwrite(present, "x")
    t |> equal(pick_newest([missing, present]), present, "existing candidate wins over missing")
    t |> equal(pick_newest([missing]), "", "no existing candidate yields empty")
    remove(present)
    rmdir(dir)
}
