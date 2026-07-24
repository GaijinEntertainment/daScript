options gen2
options rtti
options persistent_heap
options gc
options indenting = 4

require ./watcher_server.das
require daslib/random
require daslib/fio
require daslib/module_path
require live_host
require strings
require math

// Lifecycle state is global: standalone JIT heap collection cannot see ordinary stack locals as
// GC roots. main therefore keeps no collectable value alive across maybe_collect_gc(). The same
// init/update/shutdown boundary also lets daslang-live (and future hosts) own the outer loop.
var private g_server : DasHerdWatcherServer?
var private g_args_parsed = false
var private g_watcher_initialized = false
var private g_repositories_initialized = false
var private g_server_started = false
var private g_exit_code = 0
var private g_port = 9191
var private g_token : string
var private g_log_root : string
var private g_config_path : string
var private g_last_memory_log_at = 0l
var private g_last_gc_at = 0l
var private g_started_at = 0l
var private g_exit_after_ms = 0l
let GC_PRESSURE_MIN_INTERVAL_SECONDS = 10l
let GC_COMPACT_MIN_INTERVAL_SECONDS = 60l

def private option_value(prefix : string; fallback : string = "") : string {
    for (argument in get_command_line_arguments()) {
        if (starts_with(argument, prefix)) return slice(argument, length(prefix))
    }
    return fallback
}

def private make_local_token : string {
    // This first slice is deliberately loopback-only. The token prevents an
    // accidental drive-by from another local page; remote deployment will use
    // a real authentication and pairing design.
    var seed = random_seed(int(ref_time_ticks()))
    return "{random_uint(seed)}-{random_uint(seed)}-{random_uint(seed)}-{random_uint(seed)}"
}

// Parse directly into global lifecycle state so no command-line strings survive on main's stack.
def private parse_watcher_args() {
    g_args_parsed = true
    g_exit_code = 0
    g_port = clamp(to_int(option_value("--port=", "9191")), 1024, 65535)
    g_token = option_value("--token=")
    if (empty(g_token)) {
        g_token = make_local_token()
    }
    g_log_root = option_value("--log-root=",
        path_join(get_das_root(), "logs/dasHerd/watcher/sessions"))
    g_config_path = option_value("--config=", repository_default_config_path())
    g_exit_after_ms = max(0l, int64(to_int(option_value("--exit-after-ms=", "0"))))
}

// Log das-managed memory independently of process RSS. All formatting temporaries die before
// main reaches maybe_collect_gc().
def private log_memory_if_due() {
    let now = int64(get_clock())
    if (g_last_memory_log_at != 0l && now - g_last_memory_log_at < 60l) return
    g_last_memory_log_at = now
    print("dasHerd watcher: memory das_heap={heap_bytes_allocated()}/{heap_total_allocated()} " +
        "string_heap={string_heap_bytes_allocated()}/{string_heap_total_allocated()} " +
        "allocations={heap_allocation_count()} string_allocations={string_heap_allocation_count()}\n")
}

// Standalone-loop GC. Request and HTTP-handler locals lived inside update() and are dead by the
// time main calls this function. daslang-live already collects between update calls; an explicit
// request remains useful there for diagnostics.
def private maybe_collect_gc() {
    let forced = watcher_take_gc_request()
    return if (is_live_mode() && !forced)
    let now = int64(get_clock())
    if (forced) {
        g_last_gc_at = now
        let gc_started = ref_time_ticks()
        print("dasHerd watcher: GC begin reason=forced_endpoint validate=true " +
            "das_heap={heap_bytes_allocated()}/{heap_total_allocated()} " +
            "string_heap={string_heap_bytes_allocated()}/{string_heap_total_allocated()}\n")
        unsafe(heap_collect(true, true))
        print("dasHerd watcher: GC end reason=forced_endpoint " +
            "elapsed_ms={int64(get_time_usec(gc_started)) / 1000l} " +
            "das_heap={heap_bytes_allocated()}/{heap_total_allocated()} " +
            "string_heap={string_heap_bytes_allocated()}/{string_heap_total_allocated()}\n")
        return
    }
    let heap_used = heap_bytes_allocated()
    let heap_total = heap_total_allocated()
    let string_used = string_heap_bytes_allocated()
    let string_total = string_heap_total_allocated()
    let reason = watcher_gc_reason(string_used, string_total,
        heap_used, heap_total)
    if (empty(reason)) return
    let minimum_interval = ((reason == "string_pressure" || reason == "heap_pressure")
        ? GC_PRESSURE_MIN_INTERVAL_SECONDS : GC_COMPACT_MIN_INTERVAL_SECONDS)
    if (g_last_gc_at != 0l && now - g_last_gc_at < minimum_interval) return
    g_last_gc_at = now
    let gc_started = ref_time_ticks()
    print("dasHerd watcher: GC begin reason={reason} validate=true " +
        "das_heap={heap_used}/{heap_total} " +
        "string_heap={string_used}/{string_total}\n")
    unsafe(heap_collect(true, true))
    print("dasHerd watcher: GC end reason={reason} " +
        "elapsed_ms={int64(get_time_usec(gc_started)) / 1000l} " +
        "das_heap={heap_bytes_allocated()}/{heap_total_allocated()} " +
        "string_heap={string_heap_bytes_allocated()}/{string_heap_total_allocated()}\n")
}

[export]
def init() {
    if (!g_args_parsed) {
        parse_watcher_args()
    }
    if (g_server != null || g_watcher_initialized) {
        print("dasHerd watcher: init called while already initialized\n")
        g_exit_code = 1
        request_exit()
        return
    }
    watcher_reset_gc_request()
    g_last_memory_log_at = 0l
    g_last_gc_at = 0l
    g_started_at = ref_time_ticks()
    let init_error = watcher_init(g_log_root)
    if (!empty(init_error)) {
        print("dasHerd watcher: could not initialize logs: {init_error}\n")
        g_exit_code = 2
        request_exit()
        return
    }
    g_watcher_initialized = true
    let repository_error = repository_init(g_config_path, get_das_root())
    if (!empty(repository_error)) {
        print("dasHerd watcher: could not initialize repositories: {repository_error}\n")
        g_exit_code = 5
        request_exit()
        return
    }
    g_repositories_initialized = true

    g_server = new DasHerdWatcherServer()
    g_server.token = g_token
    g_server.agent_url = "http://127.0.0.1:{g_port}"
    g_server.agent_skill_path = path_join(get_das_root(), "utils/dasHerd/dasherder.md")
    let watcher_dir = get_this_module_dir()
    let vendor_dir = path_join(watcher_dir, "vendor/xterm")
    g_server.control_html = path_join(watcher_dir, "control.html")
    g_server.xterm_js = path_join(vendor_dir, "xterm.js")
    g_server.xterm_css = path_join(vendor_dir, "xterm.css")
    g_server.xterm_fit_js = path_join(vendor_dir, "addon-fit.js")
    g_server.xterm_unicode_js = path_join(vendor_dir, "addon-unicode11.js")
    g_server.xterm_web_links_js = path_join(vendor_dir, "addon-web-links.js")
    g_server->init(g_port)
    if (!g_server->set_bind_host("127.0.0.1")) {
        print("dasHerd watcher: could not select loopback bind address\n")
        g_exit_code = 3
        request_exit()
        return
    }
    let start_result = g_server->start()
    if (start_result != 0) {
        print("dasHerd watcher: server start failed ({start_result})\n")
        g_exit_code = 4
        request_exit()
        return
    }
    g_server_started = true

    print("dasHerd watcher ready\n")
    print("  control: http://127.0.0.1:{g_port}/?token={g_token}\n")
    print("  logs:    {g_log_root}\n")
    print("  config:  {g_config_path}\n")
    print("  bind:    127.0.0.1:{g_port}\n")
    if (g_exit_after_ms > 0l) {
        print("  lifetime: {g_exit_after_ms} ms (diagnostic)\n")
    }
}

[export]
def update() {
    if (!g_server_started || g_server == null) {
        request_exit()
        return
    }
    g_server->tick()
    log_memory_if_due()
    if (g_exit_after_ms > 0l
            && get_time_nsec(g_started_at) / 1000000l >= g_exit_after_ms) {
        print("dasHerd watcher: diagnostic lifetime elapsed\n")
        request_exit()
    }
    sleep(2u)
}

// Release lifecycle-owned objects in dependency order. Safe after a partially failed init.
[export]
def shutdown() {
    if (g_server_started && g_server != null) {
        let _ = g_server->stop()
    }
    g_server_started = false
    if (g_server != null) {
        g_server->cleanup_clients()
    }
    if (g_watcher_initialized) {
        if (g_repositories_initialized) {
            repository_shutdown()
            g_repositories_initialized = false
        }
        watcher_shutdown()
        g_watcher_initialized = false
    }
    if (g_server != null) {
        // Watcher shutdown releases any staged-view file handles before the
        // final artifact-removal retry.
        g_server->cleanup_inspection_artifacts()
        g_server->cleanup()
        unsafe { delete g_server }
        g_server = null
    }
    watcher_reset_gc_request()
}

[export]
def main : int {
    parse_watcher_args()
    init()
    while (!exit_requested()) {
        update()
        maybe_collect_gc()
    }
    shutdown()
    return g_exit_code
}
