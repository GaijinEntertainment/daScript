options gen2
options indenting = 4
options gc
options persistent_heap
options stack = 262144

require playground_config
require playground_server
require daslib/logger
require live/live_gc
require live_host

// dasweb-playground — the daslang.io playground backend (share links + sample
// store; plans/dasweb_backend.md). Canonical application shape: exported
// init/update/shutdown + a standalone main with the GC boundary; hostable by
// daslang-live, supervised by utils/watchdog in production.

// Lifecycle state is global: JIT heap collection cannot see stack locals as
// GC roots, so main keeps no collectable value alive across maybe_collect_gc().
var private g_cfg = ServerArgs()
var private g_sources : table<string; string>
var private g_ready = false
var private g_exit_code = 0
var private g_args_parsed = false

// The no-argument overload wraps clargs' parse_args(type<T>) so standalone
// main has no collectable config local across the GC point.
def parse_args {
    g_args_parsed = true
    var r <- parse_args(type<ServerArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<ServerArgs>), "dasweb-playground")
        g_exit_code = 1
        request_exit()
        return
    }
    g_cfg <- r |> move_unwrap
    if (g_cfg.help) {
        print_help(get_command_info(type<ServerArgs>), "dasweb-playground")
        request_exit()
        return
    }
    var cfg_err = ""
    if (!load_config(g_cfg, g_sources, cfg_err)) {
        print("error: {cfg_err}\n")
        g_exit_code = 1
        request_exit()
    }
}

[export]
def init {
    // request_exit() only raises a flag — it does not abort — so -? and a
    // config error both fall through to here. Starting up anyway would open the
    // production database, run migrations and the curated importer, and bind
    // the port out from under the running service.
    return if (g_exit_code != 0 || exit_requested())
    if (!g_args_parsed) {
        parse_args()   // daslang-live calls init directly; standalone main parsed already
        return if (g_exit_code != 0 || exit_requested())   // -? leaves the code 0 and only raises the flag
    }
    // tee every to_log/print to logs/dasweb-playground.log (timestamped ndjson) AND the console
    logger_init_tee("dasweb-playground")
    logger_info("playground", "effective config", config_banner(g_cfg, g_sources))
    g_ready = init_server(g_cfg.port, g_cfg.db,
        StorePolicy(
            max_source_bytes = g_cfg.max_source_bytes,
            max_inserts_per_ip_hour = g_cfg.max_inserts_per_ip_hour),
        g_cfg.base_url, g_cfg.curated_dir,
        BuildOptions(
            token = g_cfg.build_token,
            blobs_dir = g_cfg.blobs_dir,
            queue = BuildConfig(
                claim_timeout_seconds = g_cfg.build_claim_timeout_seconds,
                max_build_attempts = g_cfg.max_build_attempts)))
    if (!g_ready) {
        g_exit_code = 1
    }
}

[export]
def update {
    if (!g_ready || !update_server()) {
        request_exit()
    }
}

[export]
def shutdown {
    shutdown_server()
    g_ready = false
    logger_flush()
}

[export]
def main : int {
    parse_args()
    init()
    while (!exit_requested()) {
        update()
        maybe_collect_gc()
    }
    shutdown()
    return g_exit_code
}
