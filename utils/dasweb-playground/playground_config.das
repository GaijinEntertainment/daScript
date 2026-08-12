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

    @clarg_doc = "Bearer token for the remote builder endpoints; empty disables the build surface (production sets it in the toml, not on the command line)"
    build_token : string = ""

    @clarg_doc = "Artifact cache root (blobs); empty disables artifact storage and serving"
    blobs_dir : string = "blobs"

    @clarg_doc = "Separate origin page artifacts (user-influenced html/js) are served from, e.g. https://run.daslang.io; empty refuses page builds outright (set it to this server's own origin for local page testing)"
    page_origin : string = ""

    @clarg_doc = "Seconds before a claimed build is considered abandoned and requeued"
    build_claim_timeout_seconds : int = 600

    @clarg_doc = "Claim attempts before a build job is failed"
    max_build_attempts : int = 3

    @clarg_doc = "TOML config path (default: dasweb-playground.toml in cwd or beside the tool)"
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
    // TOML integers arrive as _longint, floats as _number — accept both, since
    // `port = 8101.0` is a number the user plainly meant.
    if (v == null || !((v.value is _longint) || (v.value is _number))) {
        error = "config key '{key}' must be a number"
        return false
    }
    field = int(v ?? 0l)
    sources[key] = "toml"
    return true
}

def mark_cli_sources(user_args : array<string>; var sources : table<string; string>) {
    for (key in fixed_array("port", "db", "base_url", "max_source_bytes", "max_inserts_per_ip_hour", "curated_dir",
        "build_token", "blobs_dir", "page_origin", "build_claim_timeout_seconds", "max_build_attempts")) {
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
    let ok = (
        cfg_from_toml(root, "port", cfg.port, auth, user_args, sources, error) &&
        cfg_from_toml(root, "db", cfg.db, auth, user_args, sources, error) &&
        cfg_from_toml(root, "base_url", cfg.base_url, auth, user_args, sources, error) &&
        cfg_from_toml(root, "max_source_bytes", cfg.max_source_bytes, auth, user_args, sources, error) &&
        cfg_from_toml(root, "max_inserts_per_ip_hour", cfg.max_inserts_per_ip_hour, auth, user_args, sources, error) &&
        cfg_from_toml(root, "curated_dir", cfg.curated_dir, auth, user_args, sources, error) &&
        cfg_from_toml(root, "build_token", cfg.build_token, auth, user_args, sources, error) &&
        cfg_from_toml(root, "blobs_dir", cfg.blobs_dir, auth, user_args, sources, error) &&
        cfg_from_toml(root, "page_origin", cfg.page_origin, auth, user_args, sources, error) &&
        cfg_from_toml(root, "build_claim_timeout_seconds", cfg.build_claim_timeout_seconds, auth, user_args, sources, error) &&
        cfg_from_toml(root, "max_build_attempts", cfg.max_build_attempts, auth, user_args, sources, error))
    unsafe {
        delete root   // read_toml minted this tree; nothing outlives this scope
    }
    return ok
}

def normalize_origin(origin : string) : string {
    //! An operator writing `https://run.daslang.io/` means the same origin as
    //! one without the slash, but the two are not interchangeable downstream:
    //! the host half would carry the `/` and then never equal a Host header, so
    //! every page artifact 404s with nothing explaining why. Normalize once,
    //! here, rather than teaching each consumer to tolerate both spellings.
    return rtrim(origin, "/")
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
    // after every source has had its say, so a CLI value normalizes too
    cfg.page_origin = normalize_origin(cfg.page_origin)
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
        // the token is a secret: the banner records that it is set and where
        // it came from, never its value
        build_token_set = !empty(cfg.build_token),
        build_token_src = sources?["build_token"] ?? "default",
        blobs_dir = cfg.blobs_dir,
        blobs_dir_src = sources?["blobs_dir"] ?? "default",
        page_origin = cfg.page_origin,
        page_origin_src = sources?["page_origin"] ?? "default",
        build_claim_timeout_seconds = cfg.build_claim_timeout_seconds,
        build_claim_timeout_seconds_src = sources?["build_claim_timeout_seconds"] ?? "default",
        max_build_attempts = cfg.max_build_attempts,
        max_build_attempts_src = sources?["max_build_attempts"] ?? "default",
        config = cfg.config))
}
