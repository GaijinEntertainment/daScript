options gen2
options no_aot

module config shared

require strings
require daslib/fio
require daslib/json
require daslib/strings_boost
require daslib/toml

struct public PreflightSettings {
    jit_threads : int = 0
    jit_max_file_time : float = 120.0
}

//! The pure half of the untracked-files gate: repo-relative paths from
//! `git ls-files --others --exclude-standard` output, CR and blank lines dropped.
def public untracked_from_ls(out : string) : array<string> {
    return <- [for (ln in split(out, "\n")); strip(ln); where !empty(strip(ln))]
}

//! The pure half of the dasLLAMA gates' trigger: does a changed path (as `git diff --name-only`
//! prints them - repo-relative, forward slashes, maybe CR-terminated) sit under `prefix`?
def public has_path_under(paths : array<string>; prefix : string) : bool {
    for (p in paths) {
        if (strip(p) |> starts_with(prefix)) {
            return true
        }
    }
    return false
}

def public has_path_under_any(paths : array<string>; prefixes : array<string>) : bool {
    for (prefix in prefixes) {
        if (has_path_under(paths, prefix)) {
            return true
        }
    }
    return false
}

//! the paths whose change reaches every gate that names a reach set
let public CORE_REACH <- ["src/", "include/", "daslib/", "dastest/", "CMakeLists.txt", "cmake/"]

//! The pure half of a gate's reach test: a gate with an empty reach set runs whatever changed; one
//! with a reach set runs when a changed path sits under one of its prefixes or under the core.
def public reach_hit(changed : array<string>; reach : array<string>) : bool {
    return true if (empty(reach))
    return has_path_under_any(changed, reach) || has_path_under_any(changed, CORE_REACH)
}

//! The pure half of the compile sweep's root filter: a `.das` under the swept trees that is not a test,
//! not generated, and not a module tree reached through a junction inside an example.
def public is_sweep_root_path(path : string) : bool {
    let p = path |> replace("\\", "/")
    let name = base_name(p)
    // "/modules/" inside the path is a module tree nested in an example - a junction the module's own gates cover
    return ((p |> ends_with(".das"))
        && !(name |> starts_with("test_")) && !(name |> starts_with("_"))
        && find(p, "/tests/") < 0 && find(p, "/_aot_generated/") < 0
        && find(p, "/modules/") < 0)
}

struct public GateReport {
    tag : string
    name : string
    seconds : double
    detail : string
    breakdown : string
}

//! A lane child's gate lines, `[TAG] name (12.3s) — detail`, each with the indented breakdown under it,
//! in print order. A child that died before its report yields none; the docs lane yields several.
def public parse_gate_reports(out : string) : array<GateReport> {
    var reports : array<GateReport>
    var breakdown : array<string>
    var open_report = false
    for (raw in split(out, "\n")) {
        let ln = raw |> replace("\r", "")
        if (open_report && ln |> starts_with("       ") && !empty(strip(ln))) {
            breakdown |> push(strip(ln))
            continue
        }
        if (open_report) {
            reports[length(reports) - 1].breakdown = join(breakdown, "\n")
            breakdown |> clear()
            open_report = false
        }
        continue if (!(ln |> starts_with("[")))
        let close = find(ln, "]")
        continue if (close < 0)
        let tag = strip(slice(ln, 1, close))
        continue if (tag != "PASS" && tag != "FAIL" && tag != "SKIP")
        let rest = slice(ln, close + 1)
        var head = strip(rest)
        var seconds = 0.0lf
        var detail = ""
        let dash = find(head, " — ")
        if (dash >= 0) {
            detail = strip(slice(head, dash + length(" — ")))
            head = strip(slice(head, 0, dash))
        }
        let open = find(head, " (")
        if (open >= 0 && head |> ends_with("s)")) {
            seconds = double(to_float(slice(head, open + 2, length(head) - 2)))
            head = strip(slice(head, 0, open))
        }
        reports |> push(GateReport(tag = tag, name = head, seconds = seconds, detail = detail))
        open_report = true
    }
    if (open_report) {
        reports[length(reports) - 1].breakdown = join(breakdown, "\n")
    }
    return <- reports
}

def public valid_jit_max_file_time_override(value : float) : bool {
    return value == -1.0 || value >= 0.0
}

//! The pure half of the lint gate's rail selection: `--lint-skip-exe-rail`
//! keeps the host + linux-mirror interp rails and drops the -exe compiled one.
def public lint_runs_exe_rail(skip_exe_rail : bool) : bool {
    return !skip_exe_rail
}

//! The lint gate's Pass detail — the only observable that distinguishes a
//! full three-rail pass from a rail-skipping one (both return GateStatus.Pass).
def public lint_pass_detail(n_files : int; ran_exe_rail : bool) : string {
    return (ran_exe_rail
        ? "{n_files} file(s) clean on all three rails (host, linux-mirror, exe)"
        : "{n_files} file(s) clean on interp rails (host, linux-mirror); exe rail skipped")
}

//! Resolve the read-only per-user config. ``DAS_PREFLIGHT_CONFIG`` overrides
//! the platform default for hermetic tests and unusual installations.
def public default_preflight_config_path() : string {
    if (has_env_variable("DAS_PREFLIGHT_CONFIG")) {
        let path = get_env_variable("DAS_PREFLIGHT_CONFIG")
        if (!empty(path)) return path
    }
    let platform = get_platform_name()
    if (platform == "windows" && has_env_variable("LOCALAPPDATA")) {
        let base = get_env_variable("LOCALAPPDATA")
        if (!empty(base)) return path_join(base, "daslang/preflight.toml")
    }
    if (platform == "darwin" && has_env_variable("HOME")) {
        let home = get_env_variable("HOME")
        if (!empty(home)) return path_join(home, "Library/Application Support/daslang/preflight.toml")
    }
    if (has_env_variable("XDG_CONFIG_HOME")) {
        let base = get_env_variable("XDG_CONFIG_HOME")
        if (!empty(base)) return path_join(base, "daslang/preflight.toml")
    }
    if (has_env_variable("HOME")) {
        let home = get_env_variable("HOME")
        if (!empty(home)) return path_join(home, ".config/daslang/preflight.toml")
    }
    return ""
}

def private object_field(value : JsonValue const?; name : string) : JsonValue const? {
    if (value == null || !(value.value is _object)) return null
    return (value.value as _object)?[name] ?? null
}

//! Parse a config from an explicit path. Missing/empty files use defaults;
//! malformed or wrongly typed settings report an error and are not applied.
def public load_preflight_settings(path : string; var error : string&) : PreflightSettings {
    var settings = PreflightSettings()
    error = ""
    if (empty(path) || !fexist(path)) return settings
    let body = fread(path)
    if (empty(body)) return settings

    var root : JsonValue? = read_toml(body, error)
    if (!empty(error) || root == null || !(root.value is _object)) {
        if (empty(error)) {
            error = "preflight config root must be a TOML table"
        }
        return settings
    }

    let jit = object_field(root, "jit")
    if (jit != null && !(jit.value is _object)) {
        error = "[jit] must be a TOML table"
        return settings
    }
    if (jit != null) {
        let threads = object_field(jit, "threads")
        if (threads != null) {
            if (!(threads.value is _longint)) {
                error = "jit.threads must be an integer"
                return settings
            }
            let value = int(threads.value as _longint)
            if (value < 0) {
                error = "jit.threads must be 0 (automatic) or positive"
                return settings
            }
            settings.jit_threads = value
        }

        let max_time = object_field(jit, "max_file_time")
        if (max_time != null) {
            if (max_time.value is _longint) {
                settings.jit_max_file_time = float(max_time.value as _longint)
            } elif (max_time.value is _number) {
                settings.jit_max_file_time = float(max_time.value as _number)
            } else {
                error = "jit.max_file_time must be a number"
                return settings
            }
            if (settings.jit_max_file_time < 0.0) {
                error = "jit.max_file_time must be non-negative"
                return PreflightSettings()
            }
        }
    }

    return settings
}
