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
// read for pedagogy, but only of a DECLARED name. The scanners and marker lists are
// daslib/env_registry's; this file supplies the roots.

let STRICT_ROOTS = [
    "modules/dasLLAMA",
    "utils/dasllama-server",
    "utils/dasllama-convert"
]

let COVERAGE_ROOTS = [
    "examples/dasLLAMA",
    "tutorials/dasLLAMA"
]

// The declarations themselves name every knob but read none of them; the test names them all.
let SKIP_FILES = [
    "dasllama_env.das",
    "test_env_registry.das"
]

[test]
def test_no_env_reads_outside_the_declarations(t : T?) {
    t |> run("engine and tool code reads knobs as g_env_* fields, never the environment") @(t : T?) {
        var offenders <- scan_env_raw_reads(STRICT_ROOTS, SKIP_FILES)
        t |> success(empty(offenders),
            "raw environment read(s) outside dasllama_env.das: {join(offenders, "; ")} — declare the knob as an [EnvConfig] field and read the g_env_* global (dynamic names: env_is_set/env_value_of)")
        delete offenders
    }
}

[test]
def test_every_env_read_is_declared(t : T?) {
    t |> run("every environment variable read anywhere has an [EnvConfig] declaration") @(t : T?) {
        var used : table<string>
        let files_scanned = scan_env_used_names(STRICT_ROOTS, SKIP_FILES, used)
        scan_env_used_names(COVERAGE_ROOTS, SKIP_FILES, used)
        t |> success(files_scanned > 100, "walk visited {files_scanned} files under the strict roots — a real walk sees hundreds; a broken walk must not let both enforcement tests pass vacuously")
        t |> success(length(used) > 0, "scan found {length(used)} names — zero means even the sanctioned-spelling scan broke")
        var names <- registered_env_names()
        var missing <- env_undeclared_names(used, names)
        t |> success(empty(missing),
            "undeclared environment variable(s): {join(missing, ", ")} — add the field to the area struct in dasllama_env.das")
        delete names
        delete missing
    }
}

[test]
def test_no_ad_hoc_env_helpers(t : T?) {
    t |> run("nobody re-declares a private env reader") @(t : T?) {
        var offenders <- scan_env_banned_helpers(STRICT_ROOTS, SKIP_FILES)
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
        var dupes <- env_duplicate_names(names)
        t |> success(empty(dupes), "duplicate declarations: {join(dupes, ", ")}")
        delete names
        delete dupes
    }
}

[test]
def test_every_knob_is_documented(t : T?) {
    t |> run("every rendered doc row carries a non-empty Effect column") @(t : T?) {
        var undocumented <- env_undocumented_rows(env_markdown())
        t |> success(empty(undocumented), "undocumented knob(s): {join(undocumented, ", ")}")
        delete undocumented
    }
}
