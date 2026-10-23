options gen2
options indenting = 4

require _lint_child public

require daslib/fio

//! A tree of three files: `a.das` requires the module `b.das`, `c.das` carries a LINT003 finding
def private build_tree(name : string) : string {
    let root = path_join(test_temp_dir(), name)
    mkdir_rec(root)
    plant(root, "b.das", "options gen2\n\nmodule b public\n\ndef public twice(x : int) : int => x * 2\n")
    plant(root, "a.das", "options gen2\n\nrequire b\n\n[export]\ndef main \{\n    print(\"\{twice(2)\}\\n\")\n\}\n")
    plant(root, "c.das", "options gen2\n\n[export]\ndef main \{\n    var x = 13\n    print(\"\{x\}\\n\")\n\}\n")
    return root
}

[test]
def test_lint_result_cache(t : T?) {
    t |> run("a second run takes clean files from the cache, never a file with findings") @(t : T?) {
        let root = build_tree("repeat")
        let first = run_lint(root, [], [])
        t |> equal(status_of(first, root, "a.das"), "PASS", "a cold run lints\n{first}")
        let second = run_lint(root, [], [])
        t |> equal(status_of(second, root, "a.das"), "CACHED", "an unchanged clean file is a hit\n{second}")
        t |> equal(status_of(second, root, "c.das"), "WARN", "a file with findings is linted every run\n{second}")
    }
    t |> run("an edit to a required module re-lints its users") @(t : T?) {
        let root = build_tree("edit")
        run_lint(root, [], [])
        plant(root, "b.das", "options gen2\n\nmodule b public\n\ndef public twice(x : int) : int => x + x\n")
        let out = run_lint(root, [], [])
        t |> equal(status_of(out, root, "a.das"), "PASS", "the require graph changed\n{out}")
        t |> equal(entry_count(root), 2, "a re-lint rewrites the file's entry in place: one per clean file")
    }
    t |> run("a run under another config path misses and overwrites the entry, never adds one") @(t : T?) {
        let root = build_tree("config")
        plant(root, "alt.lint_config", fread("{get_das_root()}/.lint_config"))
        run_lint(root, [], [])
        set_env_variable("DAS_LINT_CONFIG_PATH", path_join(root, "alt.lint_config"))
        let out = run_lint(root, [], [])
        set_env_variable("DAS_LINT_CONFIG_PATH", "")
        t |> equal(status_of(out, root, "a.das"), "PASS", "the config path is part of the entry\n{out}")
        t |> equal(entry_count(root), 2, "one entry per clean file, whatever ran before")
    }
    t |> run("a host started without a module misses the entry a host with it wrote") @(t : T?) {
        let root = build_tree("modules")
        run_lint(root, [], [])
        let out = run_lint(root, [], ["--disable-module", "dasSQLITE"])
        t |> equal(status_of(out, root, "a.das"), "PASS",
            "the host's disabled-module set is part of the entry\n{out}")
        t |> equal(entry_count(root), 2, "one entry per clean file, whatever host ran before")
        let again = run_lint(root, [], ["--disable-module", "dasSQLITE"])
        t |> equal(status_of(again, root, "a.das"), "CACHED", "the same host set is a hit\n{again}")
    }
    t |> run("a cache another lint version wrote is emptied, then refilled") @(t : T?) {
        let root = build_tree("version")
        run_lint(root, [], [])
        let stamp = path_join(path_join(root, "cache"), "lint_hash.txt")
        let current = fread(stamp)
        t |> success(!empty(current), "a run records the lint tool's semantic hash")
        plant(path_join(root, "cache"), "lint_hash.txt", "older")
        let out = run_lint(root, [], [])
        t |> equal(status_of(out, root, "a.das"), "PASS", "nothing survives a version change\n{out}")
        t |> equal(fread(stamp), current, "the new version's hash is recorded")
        t |> equal(entry_count(root), 2, "one entry per clean file")
    }
    t |> run("--no-cache lints every file") @(t : T?) {
        let root = build_tree("off")
        run_lint(root, [], [])
        let out = run_lint(root, ["--no-cache"], [])
        t |> equal(status_of(out, root, "a.das"), "PASS", "the cache is not read\n{out}")
    }
}
