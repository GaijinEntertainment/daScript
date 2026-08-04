options gen2
options indenting = 4

module playground_config public

require daslib/clargs public
require daslib/toml
require daslib/json_boost
require daslib/fio
require daslib/module_path
require strings

//! Config schema and the defaults < toml < explicit-CLI merge (authoritative
//! toml flips the top, the dasllama-server convention), with per-key
//! provenance. No HTTP, no SQL — pure data, unit-testable.

[CommandLineArgs]
struct ServerArgs {
    @clarg_short = "p"
    @clarg_doc = "HTTP port (binds 127.0.0.1 only)"
    port : int = 8101

    @clarg_doc = "SQLite database path"
    db : string = "samples.db"

    @clarg_doc = "Public base URL minted into share links"
    base_url : string = "https://daslang.io"

    @clarg_doc = "Reject sources larger than this many bytes"
    max_source_bytes : int = 262144

    @clarg_doc = "Per-IP sample-insert ceiling per hour"
    max_inserts_per_ip_hour : int = 100

    @clarg_doc = "Deployed curated samples dir (data.json + sources); empty = no curated import"
    curated_dir : string = ""

    @clarg_doc = "TOML config path (default: dasweb-playground.toml in cwd or beside the tool)"
    config : string = ""

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit (the daslang host eats --help; use -?)"
    help : bool = false
}

def private cli_has_flag(user_args : array<string>; flag : string) : bool {
    let eqform = "{flag}="
    for (a in user_args) {
        if (a == flag || a |> starts_with(eqform)) {
            return true
        }
    }
    return false
}

def private from_cli(user_args : array<string>; key : string) : bool {
    let flag = "--{replace(key, "_", "-")}"
    if (cli_has_flag(user_args, flag)) {
        return true
    }
    return key == "port" && cli_has_flag(user_args, "-p")
}

def private toml_has(root : JsonValue?; key : string) : bool {
    return root != null && (root.value is _object) && key_exists(root.value as _object, key)
}

def private cfg_from_toml(root : JsonValue?; key : string; var field : string&; auth : bool; user_args : array<string>; var sources : table<string; string>) {
    if (toml_has(root, key) && (auth || !from_cli(user_args, key))) {
        field = root?[key] ?? ""
        sources[key] = "toml"
    }
}

def private cfg_from_toml(root : JsonValue?; key : string; var field : int&; auth : bool; user_args : array<string>; var sources : table<string; string>) {
    if (toml_has(root, key) && (auth || !from_cli(user_args, key))) {
        field = int(root?[key] ?? 0l)
        sources[key] = "toml"
    }
}

def mark_cli_sources(user_args : array<string>; var sources : table<string; string>) {
    for (key in fixed_array("port", "db", "base_url", "max_source_bytes", "max_inserts_per_ip_hour", "curated_dir")) {
        if (from_cli(user_args, key)) {
            sources[key] = "cli"
        }
    }
}

def apply_config_text(var cfg : ServerArgs; body : string; user_args : array<string>; var sources : table<string; string>; var error : string&) : bool {
    //! Merge one TOML document into cfg per the precedence rules. `sources`
    //! records "toml" per key it changed (on top of "cli" marks).
    var terr = ""
    var root = read_toml(body, terr)
    if (root == null) {
        error = terr
        return false
    }
    let auth : bool = root?["authoritative"] ?? false
    cfg_from_toml(root, "port", cfg.port, auth, user_args, sources)
    cfg_from_toml(root, "db", cfg.db, auth, user_args, sources)
    cfg_from_toml(root, "base_url", cfg.base_url, auth, user_args, sources)
    cfg_from_toml(root, "max_source_bytes", cfg.max_source_bytes, auth, user_args, sources)
    cfg_from_toml(root, "max_inserts_per_ip_hour", cfg.max_inserts_per_ip_hour, auth, user_args, sources)
    cfg_from_toml(root, "curated_dir", cfg.curated_dir, auth, user_args, sources)
    unsafe {
        delete root
    }
    return true
}

def load_config(var cfg : ServerArgs; var sources : table<string; string>; var error : string&) : bool {
    //! CLI marks + config discovery (explicit --config, else dasweb-playground.toml
    //! in cwd, else beside the tool) + merge. Missing explicit config is an error;
    //! absent discovered config is fine.
    var user_args <- get_user_args()
    mark_cli_sources(user_args, sources)
    if (empty(cfg.config)) {
        let here = "dasweb-playground.toml"
        let beside = path_join(get_this_module_dir(), "dasweb-playground.toml")
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

def config_banner(cfg : ServerArgs; sources : table<string; string>) : JsonValue? {
    //! The startup banner payload: every effective key with its provenance.
    return JV((
        port = cfg.port, port_src = sources?["port"] ?? "default",
        db = cfg.db, db_src = sources?["db"] ?? "default",
        base_url = cfg.base_url, base_url_src = sources?["base_url"] ?? "default",
        max_source_bytes = cfg.max_source_bytes,
        max_source_bytes_src = sources?["max_source_bytes"] ?? "default",
        max_inserts_per_ip_hour = cfg.max_inserts_per_ip_hour,
        max_inserts_per_ip_hour_src = sources?["max_inserts_per_ip_hour"] ?? "default",
        curated_dir = cfg.curated_dir,
        curated_dir_src = sources?["curated_dir"] ?? "default",
        config = cfg.config))
}
