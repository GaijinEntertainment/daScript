options gen2
options indenting = 4

module buildd_config public

require daslib/clargs public
require daslib/toml
require daslib/json_boost
require daslib/fio
require daslib/module_path
require strings

//! Config schema and the defaults < toml < explicit-CLI merge (authoritative
//! toml flips the top), with per-key provenance — the dasweb-playground
//! convention. No HTTP, no process spawning.

[CommandLineArgs]
struct BuilddArgs {
    @clarg_short = "p"
    @clarg_doc = "Health/shutdown port (binds 127.0.0.1 only)"
    port : int = 8111

    @clarg_doc = "The dasweb-playground base URL the queue is pulled from"
    server_url : string = "http://127.0.0.1:8101"

    @clarg_doc = "Bearer token for the build endpoints; empty refuses to start (production sets it in the toml, not on the command line)"
    token : string = ""

    @clarg_doc = "The wasm toolchain worktree; its HEAD sha is the toolchain id"
    worktree : string = ""

    @clarg_doc = "Explicit toolchain id override (skips deriving from the worktree)"
    toolchain : string = ""

    @clarg_doc = "Build command prefix, space-separated; invoked as `<cmd> <src_dir> <out_dir> <mode> <entry>`"
    build_cmd : string = ""

    @clarg_doc = "Scratch root for per-job source/output dirs; empty = system temp"
    scratch_dir : string = ""

    @clarg_doc = "Seconds between queue polls when idle"
    poll_interval_seconds : int = 5

    @clarg_doc = "Wall-clock seconds one build may take before it is killed"
    build_timeout_seconds : int = 600

    @clarg_doc = "Announce the toolchain to the server at startup (the go-live; run the ABI canary first)"
    announce : bool = true

    @clarg_doc = "TOML config path (default: dasweb-buildd.toml in cwd or beside the tool)"
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
    // a wrong-typed value must be a startup error, not a silent ""/0/false
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

def private cfg_from_toml(root : JsonValue?; key : string; var field : bool&; auth : bool; user_args : array<string>; var sources : table<string; string>; var error : string&) : bool {
    if (!toml_wins(root, key, auth, user_args)) {
        return true
    }
    let v = root?[key]
    if (v == null || !(v.value is _bool)) {
        error = "config key '{key}' must be a boolean"
        return false
    }
    field = v ?? false
    sources[key] = "toml"
    return true
}

def mark_cli_sources(user_args : array<string>; var sources : table<string; string>) {
    for (key in fixed_array("port", "server_url", "token", "worktree", "toolchain", "build_cmd",
        "scratch_dir", "poll_interval_seconds", "build_timeout_seconds", "announce")) {
        if (from_cli(user_args, key)) {
            sources[key] = "cli"
        }
    }
}

def apply_config_text(var cfg : BuilddArgs; body : string; user_args : array<string>; var sources : table<string; string>; var error : string&) : bool {
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
        cfg_from_toml(root, "server_url", cfg.server_url, auth, user_args, sources, error) &&
        cfg_from_toml(root, "token", cfg.token, auth, user_args, sources, error) &&
        cfg_from_toml(root, "worktree", cfg.worktree, auth, user_args, sources, error) &&
        cfg_from_toml(root, "toolchain", cfg.toolchain, auth, user_args, sources, error) &&
        cfg_from_toml(root, "build_cmd", cfg.build_cmd, auth, user_args, sources, error) &&
        cfg_from_toml(root, "scratch_dir", cfg.scratch_dir, auth, user_args, sources, error) &&
        cfg_from_toml(root, "poll_interval_seconds", cfg.poll_interval_seconds, auth, user_args, sources, error) &&
        cfg_from_toml(root, "build_timeout_seconds", cfg.build_timeout_seconds, auth, user_args, sources, error) &&
        cfg_from_toml(root, "announce", cfg.announce, auth, user_args, sources, error))
    unsafe {
        delete root   // read_toml minted this tree; nothing outlives this scope
    }
    return ok
}

def load_config(var cfg : BuilddArgs; var sources : table<string; string>; var error : string&) : bool {
    //! CLI marks + config discovery (explicit --config, else dasweb-buildd.toml
    //! in cwd, else beside the tool) + merge. Missing explicit config is an
    //! error; absent discovered config is fine.
    var user_args <- get_user_args()
    mark_cli_sources(user_args, sources)
    if (empty(cfg.config)) {
        let here = "dasweb-buildd.toml"
        let beside = path_join(get_this_module_dir(), "dasweb-buildd.toml")
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

def config_banner(cfg : BuilddArgs; sources : table<string; string>) : JsonValue? {
    //! The startup banner payload: every effective key with its provenance.
    //! The token is a secret — set/unset and provenance only, never the value.
    return JV((
        port = cfg.port, port_src = sources?["port"] ?? "default",
        server_url = cfg.server_url, server_url_src = sources?["server_url"] ?? "default",
        token_set = !empty(cfg.token), token_src = sources?["token"] ?? "default",
        worktree = cfg.worktree, worktree_src = sources?["worktree"] ?? "default",
        toolchain = cfg.toolchain, toolchain_src = sources?["toolchain"] ?? "default",
        build_cmd = cfg.build_cmd, build_cmd_src = sources?["build_cmd"] ?? "default",
        scratch_dir = cfg.scratch_dir, scratch_dir_src = sources?["scratch_dir"] ?? "default",
        poll_interval_seconds = cfg.poll_interval_seconds,
        poll_interval_seconds_src = sources?["poll_interval_seconds"] ?? "default",
        build_timeout_seconds = cfg.build_timeout_seconds,
        build_timeout_seconds_src = sources?["build_timeout_seconds"] ?? "default",
        announce = cfg.announce, announce_src = sources?["announce"] ?? "default",
        config = cfg.config))
}
