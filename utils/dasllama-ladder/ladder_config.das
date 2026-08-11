options gen2
options indenting = 4

module ladder_config public

require daslib/clargs public
require daslib/toml
require daslib/json_boost
require daslib/fio
require daslib/module_path
require strings

//! Config schema and the defaults < toml < explicit-CLI merge (authoritative
//! toml flips the top, the dasweb-playground convention), with per-key
//! provenance. No HTTP, no SQL — pure data, unit-testable.

[CommandLineArgs]
struct LadderArgs {
    @clarg_short = "p"
    @clarg_doc = "HTTP port (binds 127.0.0.1 only)"
    port : int = 8201

    @clarg_doc = "SQLite database path"
    db : string = "ladder.db"

    @clarg_doc = "Reject record submissions larger than this many bytes"
    max_records_bytes : int = 4194304

    @clarg_doc = "Reject sidecar submissions larger than this many bytes"
    max_sidecar_bytes : int = 1048576

    @clarg_doc = "Per-IP accepted-submission ceiling per hour"
    max_submissions_per_ip_hour : int = 20

    @clarg_doc = "Official record stores dir (records/<box>.json); imported at startup; empty = no import"
    official_dir : string = ""

    @clarg_doc = "TOML config path (default: dasllama-ladder.toml in cwd or beside the tool)"
    config : string = ""

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit (the daslang host eats --help; use -?)"
    help : bool = false
}

def private from_cli(user_args : array<string>; key : string) : bool {
    let flag = "--{replace(key, "_", "-")}"
    if (flag_present(user_args, flag)) {
        return true
    }
    return key == "port" && flag_present(user_args, "-p")
}

def private toml_has(root : JsonValue?; key : string) : bool {
    return root != null && (root.value is _object) && key_exists(root.value as _object, key)
}

def private toml_wins(root : JsonValue?; key : string; auth : bool; user_args : array<string>) : bool {
    return toml_has(root, key) && (auth || !from_cli(user_args, key))
}

def private cfg_from_toml(root : JsonValue?; key : string; var field : string&; auth : bool; user_args : array<string>; var sources : table<string; string>; var error : string&) : bool {
    if (!toml_wins(root, key, auth, user_args)) {
        return true
    }
    let v = root?[key]
    // A wrong-typed value coerces to ""/0 through `??`, which would start the
    // service on a silently different config than the file states.
    if (v == null || !(v.value is _string)) {
        error = "config key '{key}' must be a string"
        return false
    }
    field = v ?? ""
    sources[key] = "toml"
    return true
}

def private cfg_from_toml(root : JsonValue?; key : string; var field : int&; auth : bool; user_args : array<string>; var sources : table<string; string>; var error : string&) : bool {
    if (!toml_wins(root, key, auth, user_args)) {
        return true
    }
    let v = root?[key]
    // TOML integers arrive as _longint, floats as _number — accept both
    if (v == null || !((v.value is _longint) || (v.value is _number))) {
        error = "config key '{key}' must be a number"
        return false
    }
    field = int(v ?? 0l)
    sources[key] = "toml"
    return true
}

def mark_cli_sources(user_args : array<string>; var sources : table<string; string>) {
    for (key in fixed_array("port", "db", "max_records_bytes", "max_sidecar_bytes",
        "max_submissions_per_ip_hour", "official_dir")) {
        if (from_cli(user_args, key)) {
            sources[key] = "cli"
        }
    }
}

def apply_config_text(var cfg : LadderArgs; body : string; user_args : array<string>; var sources : table<string; string>; var error : string&) : bool {
    //! Merge one TOML document into cfg per the precedence rules. `sources`
    //! records "toml" per key it changed (on top of "cli" marks).
    var terr = ""
    var root = read_toml(body, terr)
    if (root == null) {
        error = terr
        return false
    }
    let auth : bool = root?["authoritative"] ?? false
    let ok = (
        cfg_from_toml(root, "port", cfg.port, auth, user_args, sources, error) &&
        cfg_from_toml(root, "db", cfg.db, auth, user_args, sources, error) &&
        cfg_from_toml(root, "max_records_bytes", cfg.max_records_bytes, auth, user_args, sources, error) &&
        cfg_from_toml(root, "max_sidecar_bytes", cfg.max_sidecar_bytes, auth, user_args, sources, error) &&
        cfg_from_toml(root, "max_submissions_per_ip_hour", cfg.max_submissions_per_ip_hour, auth, user_args, sources, error) &&
        cfg_from_toml(root, "official_dir", cfg.official_dir, auth, user_args, sources, error))
    unsafe {
        delete root   // read_toml minted this tree; nothing outlives this scope
    }
    return ok
}

def load_config(var cfg : LadderArgs; var sources : table<string; string>; var error : string&) : bool {
    //! CLI marks + config discovery (explicit --config, else dasllama-ladder.toml
    //! in cwd, else beside the tool) + merge. Missing explicit config is an error;
    //! absent discovered config is fine.
    var user_args <- get_user_args()
    mark_cli_sources(user_args, sources)
    if (empty(cfg.config)) {
        let here = "dasllama-ladder.toml"
        let beside = path_join(get_this_module_dir(), "dasllama-ladder.toml")
        if (stat(here).is_valid) {
            cfg.config = here
        } elif (stat(beside).is_valid) {
            cfg.config = beside
        }
    }
    var ok = true
    if (!empty(cfg.config)) {
        let body = fread(cfg.config)
        if (empty(body)) {
            error = "cannot read config '{cfg.config}'"
            ok = false
        } else {
            ok = apply_config_text(cfg, body, user_args, sources, error)
        }
    }
    delete user_args
    return ok
}

def config_banner(cfg : LadderArgs; sources : table<string; string>) : JsonValue? {
    //! The startup banner payload: every effective key with its provenance.
    return JV((
        port = cfg.port, port_src = sources?["port"] ?? "default",
        db = cfg.db, db_src = sources?["db"] ?? "default",
        max_records_bytes = cfg.max_records_bytes,
        max_records_bytes_src = sources?["max_records_bytes"] ?? "default",
        max_sidecar_bytes = cfg.max_sidecar_bytes,
        max_sidecar_bytes_src = sources?["max_sidecar_bytes"] ?? "default",
        max_submissions_per_ip_hour = cfg.max_submissions_per_ip_hour,
        max_submissions_per_ip_hour_src = sources?["max_submissions_per_ip_hour"] ?? "default",
        official_dir = cfg.official_dir,
        official_dir_src = sources?["official_dir"] ?? "default",
        config = cfg.config))
}
