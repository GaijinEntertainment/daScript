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
    jit_max_file_time : float = 60.0
}

//! The pure half of the untracked-files gate: repo-relative paths from
//! `git ls-files --others --exclude-standard` output, CR and blank lines dropped.
def public untracked_from_ls(out : string) : array<string> {
    return <- [for (ln in split(out, "\n")); strip(ln); where !empty(strip(ln))]
}

def public valid_jit_max_file_time_override(value : float) : bool {
    return value == -1.0 || value >= 0.0
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
