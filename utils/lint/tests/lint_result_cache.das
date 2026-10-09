options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/strings_boost
require daslib/fio

var private _tmp_dir : string
var private _tmp_dir_inited = false

def private tmp_root() : string {
    if (!_tmp_dir_inited) {
        let r = create_temp_directory_result("lint_result_cache")
        if (!(r is value)) {
            panic("could not create temp dir for the lint cache test: {unsafe(r.error)}")
        }
        _tmp_dir = unsafe(r.value)
        _tmp_dir_inited = true
    }
    return _tmp_dir
}

[finalize]
def cleanup_temp_dir {
    if (_tmp_dir_inited && !empty(_tmp_dir)) {
        rmdir_rec(_tmp_dir)
    }
}

def private plant(dir, name, content : string) {
    fopen(path_join(dir, name), "wb") $(f) {
        if (f != null) {
            fwrite(f, content)
        }
    }
}

//! A tree of three files: `a.das` requires the module `b.das`, `c.das` carries a LINT003 finding
def private build_tree(name : string) : string {
    let root = path_join(tmp_root(), name)
    mkdir_rec(root)
    plant(root, "b.das", "options gen2\n\nmodule b public\n\ndef public twice(x : int) : int => x * 2\n")
    plant(root, "a.das", "options gen2\n\nrequire b\n\n[export]\ndef main \{\n    print(\"\{twice(2)\}\\n\")\n\}\n")
    plant(root, "c.das", "options gen2\n\n[export]\ndef main \{\n    var x = 13\n    print(\"\{x\}\\n\")\n\}\n")
    return root
}

def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

def private run_lint(root : string; extra : array<string>) : string {
    var argv <- [das_exe(), "-dasroot", get_das_root(), "{get_das_root()}/utils/lint/main.das", "--", root, "--cache-dir", path_join(root, "cache")]
    argv |> push_from(extra)
    var output : string
    unsafe(popen_argv(argv, 0.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
    return output
}

def private entry_count(root : string) : int {
    var n = 0
    dir(path_join(root, "cache")) $(name) {
        n++ if (name != "." && name != ".." && name != "lint_hash.txt")
    }
    return n
}

def private status_of(out, root, file : string) : string {
    for (line in split(out, "\n")) {
        for (status in ["PASS", "CACHED", "WARN"]) {
            return status if (line |> starts_with("{status} ") && find(line, path_join(root, file)) >= 0)
        }
    }
    return "?"
}

[test]
def test_lint_result_cache(t : T?) {
    t |> run("a second run takes clean files from the cache, never a file with findings") @(t : T?) {
        let root = build_tree("repeat")
        let first = run_lint(root, [])
        t |> equal(status_of(first, root, "a.das"), "PASS", "a cold run lints\n{first}")
        let second = run_lint(root, [])
        t |> equal(status_of(second, root, "a.das"), "CACHED", "an unchanged clean file is a hit\n{second}")
        t |> equal(status_of(second, root, "c.das"), "WARN", "a file with findings is linted every run\n{second}")
    }
    t |> run("an edit to a required module re-lints its users") @(t : T?) {
        let root = build_tree("edit")
        run_lint(root, [])
        plant(root, "b.das", "options gen2\n\nmodule b public\n\ndef public twice(x : int) : int => x + x\n")
        let out = run_lint(root, [])
        t |> equal(status_of(out, root, "a.das"), "PASS", "the require graph changed\n{out}")
        t |> equal(entry_count(root), 2, "a re-lint rewrites the file's entry in place: one per clean file")
    }
    t |> run("a run under another config path misses and overwrites the entry, never adds one") @(t : T?) {
        let root = build_tree("config")
        plant(root, "alt.lint_config", fread("{get_das_root()}/.lint_config"))
        run_lint(root, [])
        set_env_variable("DAS_LINT_CONFIG_PATH", path_join(root, "alt.lint_config"))
        let out = run_lint(root, [])
        set_env_variable("DAS_LINT_CONFIG_PATH", "")
        t |> equal(status_of(out, root, "a.das"), "PASS", "the config path is part of the entry\n{out}")
        t |> equal(entry_count(root), 2, "one entry per clean file, whatever ran before")
    }
    t |> run("a cache another lint version wrote is emptied, then refilled") @(t : T?) {
        let root = build_tree("version")
        run_lint(root, [])
        let stamp = path_join(path_join(root, "cache"), "lint_hash.txt")
        let current = fread(stamp)
        t |> success(!empty(current), "a run records the lint tool's semantic hash")
        plant(path_join(root, "cache"), "lint_hash.txt", "older")
        let out = run_lint(root, [])
        t |> equal(status_of(out, root, "a.das"), "PASS", "nothing survives a version change\n{out}")
        t |> equal(fread(stamp), current, "the new version's hash is recorded")
        t |> equal(entry_count(root), 2, "one entry per clean file")
    }
    t |> run("--no-cache lints every file") @(t : T?) {
        let root = build_tree("off")
        run_lint(root, [])
        let out = run_lint(root, ["--no-cache"])
        t |> equal(status_of(out, root, "a.das"), "PASS", "the cache is not read\n{out}")
    }
}
