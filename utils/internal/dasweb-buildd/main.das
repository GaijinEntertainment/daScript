options gen2
options indenting = 4
options gc
options persistent_heap
options stack = 262144

require buildd_config
require buildd_service
require daslib/logger
require live/live_gc
require live_host

// dasweb-buildd — the wasm build worker on the compute box
// (plans/dasweb_wasm_pipeline.md). Pulls the dasweb-playground build queue
// over HTTPS, runs each job through the configured sandboxed build command,
// uploads artifacts or the compiler error. Canonical application shape:
// exported init/update/shutdown + a standalone main with the GC boundary;
// supervised by utils/watchdog in production.

// Lifecycle state is global: JIT heap collection cannot see stack locals as
// GC roots, so main keeps no collectable value alive across maybe_collect_gc().
var private g_cfg = BuilddArgs()
var private g_sources : table<string; string>
var private g_ready = false
var private g_exit_code = 0
var private g_args_parsed = false

def parse_args {
    g_args_parsed = true
    var r <- parse_args(type<BuilddArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<BuilddArgs>), "dasweb-buildd")
        g_exit_code = 1
        request_exit()
        return
    }
    g_cfg <- r |> move_unwrap
    if (g_cfg.help) {
        print_help(get_command_info(type<BuilddArgs>), "dasweb-buildd")
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
    // request_exit() only raises a flag — -? and a config error both fall
    // through to here; starting anyway would begin claiming production jobs
    return if (g_exit_code != 0 || exit_requested())
    if (!g_args_parsed) {
        parse_args()   // daslang-live calls init directly; standalone main parsed already
        return if (g_exit_code != 0 || exit_requested())
    }
    logger_init_tee("dasweb-buildd")
    logger_info("buildd", "effective config", config_banner(g_cfg, g_sources))
    g_ready = init_service(g_cfg)
    if (!g_ready) {
        g_exit_code = 1
    }
}

[export]
def update {
    if (!g_ready || !update_service()) {
        request_exit()
    }
}

[export]
def shutdown {
    shutdown_service()
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
