options gen2
options indenting = 4

require _lint_child public

require strings
require daslib/fio
require daslib/style_lint

//! A tree of three files: `a.das` requires the module `b.das`, `c.das` carries a LINT003 finding
def private build_cache_tree(name : string) : string {
    let root = path_join(test_temp_dir(), name)
    mkdir_rec(root)
    plant(root, "b.das", "options gen2\n\nmodule b public\n\ndef public twice(x : int) : int => x * 2\n")
    plant(root, "a.das", "options gen2\n\nrequire b\n\n[export]\ndef main \{\n    print(\"\{twice(2)\}\\n\")\n\}\n")
    plant(root, "c.das", "options gen2\n\n[export]\ndef main \{\n    var x = 13\n    print(\"\{x\}\\n\")\n\}\n")
    return root
}

[test(description = "the result cache serves unchanged clean files and never a file with findings, and misses on an edited module, config path, host module set, lint version or --no-cache", timeout = 1200)]
def test_lint_result_cache(t : T?) {
    t |> run("a second run takes clean files from the cache, never a file with findings") @(t : T?) {
        let root = build_cache_tree("repeat")
        let first = run_lint(root, [], [])
        t |> equal(status_of(first, root, "a.das"), "PASS", "a cold run lints\n{first}")
        let second = run_lint(root, [], [])
        t |> equal(status_of(second, root, "a.das"), "CACHED", "an unchanged clean file is a hit\n{second}")
        t |> equal(status_of(second, root, "c.das"), "WARN", "a file with findings is linted every run\n{second}")
    }
    t |> run("an edit to a required module re-lints its users") @(t : T?) {
        let root = build_cache_tree("edit")
        run_lint(root, [], [])
        plant(root, "b.das", "options gen2\n\nmodule b public\n\ndef public twice(x : int) : int => x + x\n")
        let out = run_lint(root, [], [])
        t |> equal(status_of(out, root, "a.das"), "PASS", "the require graph changed\n{out}")
        t |> equal(entry_count(root), 2, "a re-lint rewrites the file's entry in place: one per clean file")
    }
    t |> run("a run under another config path misses and overwrites the entry, never adds one") @(t : T?) {
        let root = build_cache_tree("config")
        plant(root, "alt.lint_config", fread("{get_das_root()}/.lint_config"))
        run_lint(root, [], [])
        set_env_variable("DAS_LINT_CONFIG_PATH", path_join(root, "alt.lint_config"))
        let out = run_lint(root, [], [])
        set_env_variable("DAS_LINT_CONFIG_PATH", "")
        t |> equal(status_of(out, root, "a.das"), "PASS", "the config path is part of the entry\n{out}")
        t |> equal(entry_count(root), 2, "one entry per clean file, whatever ran before")
    }
    t |> run("a host started without a module misses the entry a host with it wrote") @(t : T?) {
        let root = build_cache_tree("modules")
        run_lint(root, [], [])
        let out = run_lint(root, [], ["--disable-module", "dasSQLITE"])
        t |> equal(status_of(out, root, "a.das"), "PASS",
            "the host's disabled-module set is part of the entry\n{out}")
        t |> equal(entry_count(root), 2, "one entry per clean file, whatever host ran before")
        let again = run_lint(root, [], ["--disable-module", "dasSQLITE"])
        t |> equal(status_of(again, root, "a.das"), "CACHED", "the same host set is a hit\n{again}")
    }
    t |> run("a cache another lint version wrote is emptied, then refilled") @(t : T?) {
        let root = build_cache_tree("version")
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
        let root = build_cache_tree("off")
        run_lint(root, [], [])
        let out = run_lint(root, ["--no-cache"], [])
        t |> equal(status_of(out, root, "a.das"), "PASS", "the cache is not read\n{out}")
    }
}

//! A tree of five files over one shared module: `a.das` and `c.das`
//! require the shared `b.das`, `sub/e.das` requires it by file path,
//! `c.das` carries a LINT003 finding, `d.das` requires only a builtin
//! module; the directory walk visits `a` before `b`, so the require
//! order has to move `b` ahead
def private build_shared_tree(name : string) : string {
    let root = path_join(test_temp_dir(), name)
    mkdir_rec(root)
    plant(root, "b.das", "options gen2\n\nmodule b shared public\n\n" +
        "def public twice(x : int) : int => x * 2\n")
    plant(root, "a.das", "options gen2\n\nrequire b\n\n[export]\ndef main \{\n" +
        "    print(\"\{twice(2)\}\\n\")\n\}\n")
    plant(root, "c.das", "options gen2\n\nrequire b public\n\n[export]\ndef main \{\n" +
        "    var x = twice(13)\n    print(\"\{x\}\\n\")\n\}\n")
    plant(root, "d.das", "options gen2\n\nrequire strings\n\n[export]\ndef main \{\n" +
        "    print(\"\{to_upper(\"d\")\}\\n\")\n\}\n")
    mkdir_rec(path_join(root, "sub"))
    plant(path_join(root, "sub"), "e.das", "options gen2\n\nrequire ../b.das\n\n[export]\n" +
        "def main \{\n    print(\"\{twice(3)\}\\n\")\n\}\n")
    return root
}

//! the promoted-module count on a file's status line, -1 where the
//! line carries none
def private promoted_of(out, root, file : string) : int {
    var line = ""
    return -1 if (line_of(out, root, file, ["PASS ", "WARN "], line) < 0)
    let at = find(line, "(promoted ")
    return at < 0 ? -1 : to_int(slice(line, at + 10, find(line, ")", at)))
}

//! the position of `file`'s "checking" line in the run's output, -1
//! when the run never reached it
def private checked_at(out, root, file : string) : int {
    var line = ""
    return line_of(out, root, file, ["checking "], line)
}

def private run_lint(root : string; extra : array<string>) : string => run_lint(root, extra, [])

[test(description = "lint shares modules across a run without changing any file status, lints a module ahead of its users, honors --exclude, and shares inside each parallel worker", timeout = 1200)]
def test_lint_shared_modules(t : T?) {
    t |> run("a run reads as one without sharing, counts its promoted modules, caches") @(t : T?) {
        let root = build_shared_tree("same")
        let plain = run_lint(root, ["--no-cache", "--no-shared-modules"])
        let pooled = run_lint(root, [])
        for (file in ["a.das", "b.das", "c.das", "d.das"]) {
            t |> equal(status_of(pooled, root, file), status_of(plain, root, file),
                "{file} reads alike\nplain:\n{plain}\nshared:\n{pooled}")
        }
        t |> equal(status_of(pooled, root, "c.das"), "WARN", "the finding survives\n{pooled}")
        t |> success(find(pooled, "LINT003") >= 0, "the finding is the same rule\n{pooled}")
        t |> equal(entry_count(root), 4, "the clean files' entries land: a, b, d and sub/e")
        let again = run_lint(root, [])
        t |> equal(status_of(again, root, "a.das"), "CACHED", "a clean file is a hit\n{again}")
        t |> equal(status_of(again, root, "c.das"), "WARN", "a finding lints every run\n{again}")
        let d_took = promoted_of(pooled, root, "d.das")
        t |> equal(d_took, 0, "d.das requires none of the set; builtin counts nothing\n{pooled}")
        t |> success(promoted_of(pooled, root, "a.das") > d_took,
            "a.das's compile took its module promoted from the process\n{pooled}")
        t |> success(promoted_of(pooled, root, "c.das") > d_took,
            "c.das's compile took its module promoted from the process\n{pooled}")
        t |> equal(promoted_of(plain, root, "a.das"), -1, "no sharing, no count\n{plain}")
    }
    t |> run("a module lints ahead of the files that require it") @(t : T?) {
        let root = build_shared_tree("order")
        let pooled = run_lint(root, [])
        let b_at = checked_at(pooled, root, "b.das")
        t |> success(b_at >= 0, "b.das was linted\n{pooled}")
        t |> success(b_at < checked_at(pooled, root, "a.das"), "b.das before a.das\n{pooled}")
        t |> success(b_at < checked_at(pooled, root, "c.das"), "b.das before c.das\n{pooled}")
        t |> success(b_at < checked_at(pooled, root, "sub/e.das"),
            "b.das before sub/e.das, which requires it by file path\n{pooled}")
        t |> equal(status_of(pooled, root, "sub/e.das"), "PASS", "the file-path require\n{pooled}")
        t |> equal(status_of(pooled, root, "b.das"), "PASS",
            "the module lints clean after its users' compiles promote it\n{pooled}")
    }
    t |> run("--exclude leaves a folder out of the walk") @(t : T?) {
        let root = build_shared_tree("exclude")
        mkdir_rec(path_join(root, "skip"))
        plant(path_join(root, "skip"), "e.das", "options gen2\n\n[export]\ndef main \{\n" +
            "    var x = 13\n    print(\"\{x\}\\n\")\n\}\n")
        let walked = run_lint(root, ["--no-cache"])
        t |> equal(status_of(walked, root, "skip/e.das"), "WARN", "the finding is seen\n{walked}")
        let left_out = run_lint(root, ["--no-cache", "--exclude", path_join(root, "skip")])
        t |> equal(status_of(left_out, root, "skip/e.das"), "?", "never linted\n{left_out}")
        t |> equal(status_of(left_out, root, "c.das"), "WARN", "the rest lints\n{left_out}")
    }
    t |> run("a parallel run shares inside each worker") @(t : T?) {
        let root = path_join(test_temp_dir(), "wide")
        mkdir_rec(root)
        plant(root, "b.das", "options gen2\n\nmodule b shared public\n\n" +
            "def public twice(x : int) : int => x * 2\n")
        for (i in range(16)) {   // past the floor below which a run stays serial
            plant(root, "a{i}.das", "options gen2\n\nrequire b\n\n[export]\ndef main \{\n" +
                "    print(\"\{twice({i})\}\\n\")\n\}\n")
        }
        let out = run_lint(root, ["--no-cache", "--workers", "2"])
        t |> equal(status_of(out, root, "b.das"), "PASS", "the module in its worker\n{out}")
        for (i in range(16)) {
            t |> equal(status_of(out, root, "a{i}.das"), "PASS", "a{i}.das in its worker\n{out}")
            t |> success(promoted_of(out, root, "a{i}.das") >= 1,
                "a{i}.das's compile took its module promoted from its worker's process\n{out}")
        }
    }
}

def match_rules(x : int; blk : block<(r : int) : void>) {
    invoke(blk, x)
}

def call_named_match(x : int) : string {
    var res = ""
    match_rules(x) $(r) {
        if (r == 3) {
            res = "three"
        }
    }
    return res
}

[test(description = "STYLE044 does not treat a call whose name merely contains match as a match head")]
def test_a_call_named_like_match_is_no_match_head(t : T?) {
    t |> equal(call_named_match(3), "three")
}
