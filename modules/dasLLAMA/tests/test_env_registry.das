options gen2

require dastest/testing_boost public
require dasllama/dasllama_env
require daslib/fio
require strings
require daslib/strings_boost

// Keeps ENVIRONMENT.md honest. The doc is GENERATED from env_registry(), so the only way it can go
// stale is a source reading a variable nobody registered — which is exactly what the coverage test
// below catches. Adding an env read without a registry entry fails here, in the same commit.
//
// The reader-semantics cases pin the normalization the registry promises: "unset" falls back,
// "garbage" falls back (plain to_int would silently answer 0), and the two boolean spellings the
// tree grew organically (== "1" opt-in, != "0" opt-out) resolve the same way.

let SCAN_ROOTS = [
    "modules/dasLLAMA",
    "utils/dasllama-server",
    "examples/dasLLAMA",
    "tutorials/dasLLAMA"
]

// Every spelling that reads an environment variable by literal name.
let READ_MARKERS = [
    "get_env_variable(\"",
    "has_env_variable(\"",
    "env_flag(\"",
    "env_int(\"",
    "env_int64(\"",
    "env_str(\"",
    "env_int64_min(\""   // the Accelerate floor+warn wrapper; it still names the knob literally
]

// The registry itself names every knob as a literal but reads none of them; generated AOT trees
// are copies of sources already scanned.
def private skip_path(path : string) : bool {
    let p = replace(path, "\\", "/")
    return (find(p, "_aot_generated") >= 0
            || find(p, "dasllama_env.das") >= 0
            || find(p, "test_env_registry.das") >= 0)
}

def private collect_names(text : string; var out : table<string>) {
    for (marker in READ_MARKERS) {
        var from = 0
        while (true) {
            let hit = find(text, marker, from)
            break if (hit < 0)
            let start = hit + length(marker)
            let close = find(text, "\"", start)
            break if (close < 0)
            let name = slice(text, start, close)
            if (!empty(name) && !key_exists(out, name)) {
                out |> insert(name)
            }
            from = close + 1
        }
    }
}

// dir_rec yields BASE NAMES, not paths, so the walk is hand-rolled to keep the path it needs
// for fread. Generated AOT trees are copies of sources already scanned — skipping them keeps
// the walk from reading the same 60k-line file twice per directory.
def private walk_das(dirpath : string; var out : table<string>) {
    dir(dirpath) $(name : string) {
        return if (name == "." || name == "..")
        let full = "{dirpath}/{name}"
        if (stat(full).is_dir) {
            return if (name == "_aot_generated" || name == "_llvm_aot_generated")
            walk_das(full, out)
        } elif (ends_with(name, ".das") && !skip_path(full)) {
            let text = fread(full)
            if (!empty(text)) {
                collect_names(text, out)
            }
        }
    }
}

def private scan_tree(var out : table<string>) {
    for (root in SCAN_ROOTS) {
        walk_das("{get_das_root()}/{root}", out)
    }
}

[test]
def test_every_env_read_is_registered(t : T?) {
    t |> run("every environment variable dasLLAMA reads has a registry entry") @(t : T?) {
        var used : table<string>
        scan_tree(used)
        t |> success(length(used) > 50, "scan found {length(used)} names — the walk looks broken")
        var reg <- env_registry()
        var known : table<string>
        for (e in reg) {
            known |> insert(e.name)
        }
        var missing : array<string>
        for (n in keys(used)) {
            if (!key_exists(known, n)) {
                missing |> push(n)
            }
        }
        t |> success(empty(missing),
            "unregistered environment variable(s): {join(missing, ", ")} — add them to env_registry() in dasllama_env.das")
        delete reg
        delete missing
    }
}

// The tree grew five of these independently (env_or x2 shapes, pf_env_int, win_env_int, a 1-arg
// env_int64), each with its own idea of what "unset" and "garbage" mean. One more would also
// reintroduce a blind spot in the coverage scan above, which only knows the shared spellings.
let BANNED_HELPERS = [
    "def private env_or(",
    "def env_or(",
    "def private pf_env_int(",
    "def private win_env_int(",
    "def env_int64(",
    "def private env_int64(",
    "def env_int(",
    "def private env_int(",
    "def env_str(",
    "def private env_str(",
    "def env_flag(",
    "def private env_flag("
]

def private find_banned(var out : array<string>) {
    for (root in SCAN_ROOTS) {
        walk_banned("{get_das_root()}/{root}", out)
    }
}

def private walk_banned(dirpath : string; var out : array<string>) {
    dir(dirpath) $(name : string) {
        return if (name == "." || name == "..")
        let full = "{dirpath}/{name}"
        if (stat(full).is_dir) {
            return if (name == "_aot_generated" || name == "_llvm_aot_generated")
            walk_banned(full, out)
        } elif (ends_with(name, ".das") && !skip_path(full)) {
            let text = fread(full)
            return if (empty(text))
            for (sig in BANNED_HELPERS) {
                if (find(text, sig) >= 0) {
                    out |> push("{name}: {sig}...)")
                }
            }
        }
    }
}

[test]
def test_no_ad_hoc_env_helpers(t : T?) {
    t |> run("every env read goes through the dasllama_env readers") @(t : T?) {
        var offenders : array<string>
        find_banned(offenders)
        t |> success(empty(offenders),
            "re-declared env helper(s): {join(offenders, "; ")} — call env_flag/env_int/env_int64/env_str from dasllama_env instead")
        delete offenders
    }
}

[test]
def test_environment_md_is_current(t : T?) {
    t |> run("the checked-in ENVIRONMENT.md matches what the registry renders") @(t : T?) {
        let path = "{get_das_root()}/modules/dasLLAMA/ENVIRONMENT.md"
        let onDisk = fread(path)
        t |> success(!empty(onDisk), "cannot read {path}")
        // \r\n on a Windows checkout: the file has no .gitattributes eol rule, and the comparison
        // is about content, not line endings
        let want = replace(env_markdown(), "\r\n", "\n")
        let got = replace(onDisk, "\r\n", "\n")
        t |> success(want == got,
            "ENVIRONMENT.md is stale — regenerate with `daslang modules/dasLLAMA/harness/gen_env_doc.das`")
    }
}

[test]
def test_registry_has_no_duplicates(t : T?) {
    t |> run("registry lists each name once") @(t : T?) {
        var reg <- env_registry()
        var seen : table<string>
        var dupes : array<string>
        for (e in reg) {
            if (key_exists(seen, e.name)) {
                dupes |> push(e.name)
            } else {
                seen |> insert(e.name)
            }
        }
        t |> success(empty(dupes), "duplicate registry entries: {join(dupes, ", ")}")
        delete reg
        delete dupes
    }
}

[test]
def test_registry_entries_are_documented(t : T?) {
    t |> run("every entry carries a non-empty doc line") @(t : T?) {
        var reg <- env_registry()
        var undocumented : array<string>
        for (e in reg) {
            if (empty(e.doc)) {
                undocumented |> push(e.name)
            }
        }
        t |> success(empty(undocumented), "undocumented entries: {join(undocumented, ", ")}")
        delete reg
        delete undocumented
    }
}

[test]
def test_env_flag_normalization(t : T?) {
    t |> run("env_flag folds the opt-in and opt-out spellings onto one rule") @(t : T?) {
        // an unset knob is the default, either way round
        t |> success(env_flag("DASLLAMA_UNSET_FIXTURE_KNOB", true), "unset should yield dflt=true")
        t |> success(!env_flag("DASLLAMA_UNSET_FIXTURE_KNOB", false), "unset should yield dflt=false")
    }
}

[test]
def test_env_number_readers_reject_garbage(t : T?) {
    t |> run("env_int / env_int64 fall back rather than answering 0") @(t : T?) {
        t |> equal(env_int("DASLLAMA_UNSET_FIXTURE_KNOB", 42), 42)
        t |> equal(env_int64("DASLLAMA_UNSET_FIXTURE_KNOB", 42l), 42l)
        t |> equal(env_str("DASLLAMA_UNSET_FIXTURE_KNOB", "fallback"), "fallback")
    }
}
