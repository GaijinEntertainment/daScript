options gen2

require dastest/testing_boost public
require dasllama/dasllama_env
require daslib/fio
require strings
require daslib/strings_boost

// Keeps ENVIRONMENT.md and the [EnvConfig] declarations honest. The doc is GENERATED from the
// declarations in dasllama_env.das, so the only way it can go stale is an env read that bypasses
// them — which is exactly what the two scans below catch, in the same commit.
//
// Two tiers of enforcement: engine/tool code (modules/dasLLAMA, utils/dasllama-*) may not read
// the environment AT ALL outside dasllama_env.das — knobs are g_env_* struct fields, and
// dynamic-name reads go through env_is_set/env_value_of. Examples and tutorials may keep a raw
// read for pedagogy, but only of a DECLARED name.

let STRICT_ROOTS = [
    "modules/dasLLAMA",
    "utils/dasllama-server",
    "utils/dasllama-convert"
]

let COVERAGE_ROOTS = [
    "examples/dasLLAMA",
    "tutorials/dasLLAMA"
]

// Every spelling that reads an environment variable — raw fio reads plus the clargs loaders
// (a literal name through env_config_* would bypass the declarations just as surely).
let READ_MARKERS = [
    "get_env_variable(\"",
    "has_env_variable(\"",
    "env_config_flag(\"",
    "env_config_int(\"",
    "env_config_int64(\"",
    "env_config_float(\"",
    "env_config_string(\"",
    "env_config_text(\""
]

// The sanctioned dynamic-name spellings: legal anywhere in the module, but a LITERAL name
// through them must still be declared — they feed the coverage scan, never the strict ban.
let SANCTIONED_MARKERS = [
    "env_is_set(\"",
    "env_value_of(\""
]

// Foreign-knob WRITES (arming a library's environment, e.g. MoltenVK's) are legal outside
// dasllama_env.das — but the name must still be a declared knob, or a rename moves the read
// while the write silently keeps the old spelling.
let WRITE_MARKERS = [
    "set_env_variable(\""
]

// The declarations themselves name every knob but read none of them; generated AOT trees are
// copies of sources already scanned.
def private skip_path(path : string) : bool {
    let p = replace(path, "\\", "/")
    return (find(p, "_aot_generated") >= 0
            || find(p, "dasllama_env.das") >= 0
            || find(p, "test_env_registry.das") >= 0)
}

def private collect_names(text : string; markers : array<string>; var out : table<string>) {
    for (marker in markers) {
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
// for fread.
def private walk_das(dirpath : string; blk : block<(path : string; text : string) : void>) {
    dir(dirpath) $(name : string) {
        return if (name == "." || name == "..")
        let full = "{dirpath}/{name}"
        if (stat(full).is_dir) {
            return if (name == "_aot_generated" || name == "_llvm_aot_generated")
            walk_das(full, blk)
        } elif (ends_with(name, ".das") && !skip_path(full)) {
            let text = fread(full)
            if (!empty(text)) {
                blk |> invoke(full, text)
            }
        }
    }
}

[test]
def test_no_env_reads_outside_the_declarations(t : T?) {
    t |> run("engine and tool code reads knobs as g_env_* fields, never the environment") @(t : T?) {
        var offenders : array<string>
        for (root in STRICT_ROOTS) {
            walk_das("{get_das_root()}/{root}") $(path, text) {
                for (marker in READ_MARKERS) {
                    if (find(text, marker) >= 0) {
                        offenders |> push("{path}: {marker}...\")")
                        break
                    }
                }
            }
        }
        t |> success(empty(offenders),
            "raw environment read(s) outside dasllama_env.das: {join(offenders, "; ")} — declare the knob as an [EnvConfig] field and read the g_env_* global (dynamic names: env_is_set/env_value_of)")
        delete offenders
    }
}

[test]
def test_every_env_read_is_declared(t : T?) {
    t |> run("every environment variable read anywhere has an [EnvConfig] declaration") @(t : T?) {
        var used : table<string>
        var files_scanned = 0
        for (root in STRICT_ROOTS) {
            walk_das("{get_das_root()}/{root}") $(path, text) {
                files_scanned++
                collect_names(text, READ_MARKERS, used)
                collect_names(text, SANCTIONED_MARKERS, used)
                collect_names(text, WRITE_MARKERS, used)
            }
        }
        for (root in COVERAGE_ROOTS) {
            walk_das("{get_das_root()}/{root}") $(path, text) {
                collect_names(text, READ_MARKERS, used)
                collect_names(text, SANCTIONED_MARKERS, used)
                collect_names(text, WRITE_MARKERS, used)
            }
        }
        t |> success(files_scanned > 100, "walk visited {files_scanned} files under the strict roots — a real walk sees hundreds; a broken walk must not let both enforcement tests pass vacuously")
        t |> success(length(used) > 0, "scan found {length(used)} names — zero means even the sanctioned-spelling scan broke")
        var names <- registered_env_names()
        var known : table<string>
        for (n in names) {
            known |> insert(n)
        }
        var missing : array<string>
        for (n in keys(used)) {
            if (!key_exists(known, n)) {
                missing |> push(n)
            }
        }
        t |> success(empty(missing),
            "undeclared environment variable(s): {join(missing, ", ")} — add the field to the area struct in dasllama_env.das")
        delete names
        delete missing
    }
}

// A fresh helper would dodge both scans above and reintroduce a private idea of "unset".
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
    "def private env_flag(",
    "def env_int64_min(",
    "def private env_int64_min("
]

[test]
def test_no_ad_hoc_env_helpers(t : T?) {
    t |> run("nobody re-declares a private env reader") @(t : T?) {
        var offenders : array<string>
        for (root in STRICT_ROOTS) {
            walk_das("{get_das_root()}/{root}") $(path, text) {
                for (sig in BANNED_HELPERS) {
                    if (find(text, sig) >= 0) {
                        offenders |> push("{base_name(path)}: {sig}...)")
                    }
                }
            }
        }
        t |> success(empty(offenders),
            "re-declared env helper(s): {join(offenders, "; ")} — knobs are [EnvConfig] fields in dasllama_env.das")
        delete offenders
    }
}

[test]
def test_environment_md_is_current(t : T?) {
    t |> run("the checked-in ENVIRONMENT.md matches what the declarations render") @(t : T?) {
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
    t |> run("each knob is declared once across the area structs") @(t : T?) {
        var names <- registered_env_names()
        t |> success(length(names) > 140, "only {length(names)} declared names — an area struct dropped out")
        var seen : table<string>
        var dupes : array<string>
        for (n in names) {
            if (key_exists(seen, n)) {
                dupes |> push(n)
            } else {
                seen |> insert(n)
            }
        }
        t |> success(empty(dupes), "duplicate declarations: {join(dupes, ", ")}")
        delete names
        delete dupes
    }
}

[test]
def test_every_knob_is_documented(t : T?) {
    t |> run("every rendered doc row carries a non-empty Effect column") @(t : T?) {
        var undocumented : array<string>
        for (line in split(env_markdown(), "\n")) {
            continue if (!starts_with(line, "| `"))
            // | `NAME` | kind | default | doc |
            var cols <- split(line, "|")
            if (length(cols) >= 5 && empty(strip(cols[4]))) {
                undocumented |> push(strip(cols[1]))
            }
            delete cols
        }
        t |> success(empty(undocumented), "undocumented knob(s): {join(undocumented, ", ")}")
        delete undocumented
    }
}
