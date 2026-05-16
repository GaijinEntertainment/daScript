options gen2
options rtti
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module live_api shared public

//! REST API debug agent for daslang-live.
//!
//! Starts an HV HTTP server and exposes endpoints for status, reload,
//! and live command dispatch. Commands are dispatched via a C++ bridge
//! that calls `live_dispatch_command` in the main application context.
//!
//! Endpoints:
//!   GET  /status      — JSON with fps, uptime, paused, dt, has_error
//!   GET  /error       — last compilation error (plain text)
//!   POST /reload      — triggers incremental reload
//!   POST /reload/full — triggers full recompile
//!   POST /pause       — pauses the host
//!   POST /unpause     — unpauses the host
//!   POST /shutdown    — graceful shutdown
//!   POST /command     — dispatches a single [live_command] via host bridge
//!   POST /commands    — dispatches an array of [live_command]s (continue-on-error)
//!
//! When a compilation error is active, /command, /pause, and /unpause
//! return HTTP 503 with the error text. Use /reload to fix.

require dashv/dashv_boost
require daslib/json
require daslib/json_boost
require daslib/jobque_boost
require daslib/debugger
require strings
require live_host

var live_api_port : int = 9090

// --- Public API ---

[export]
def live_api_set_port(port : int) {
    //! Sets the port for the HTTP server. Must be called before agent installs.
    live_api_port = port
}

// --- Response types ---

struct LiveStatus {
    fps : float
    uptime : float
    paused : bool
    dt : float
    has_error : bool
}

// --- Compilation error guard ---

def compilation_error_response(resp : HttpResponse?) : http_status {
    //! If there is a compilation error, writes a 503 JSON response and returns SERVICE_UNAVAILABLE.
    //! Returns OK (200) if there is no error — caller should continue with normal logic.
    let err = get_last_error()
    if (!empty(err)) {
        var tab : table<string; JsonValue?>
        tab |> insert("error", JV(err))
        tab |> insert("hint", JV("POST /reload to retry"))
        return resp |> JSON(write_json(JV(tab)), http_status.SERVICE_UNAVAILABLE)
    }
    return http_status.OK
}

// --- Help ---

def live_api_help_json() : string {
    //! Returns JSON help listing all endpoints with descriptions and curl examples.
    var endpoints : array<JsonValue?>  // nolint:STYLE012 — values reference cmd_desc/cmd_curl defined later; literal would break decl order
    endpoints |> push(JV({ "method" => "GET", "path" => "/status", "description" => "JSON with fps, uptime, paused, dt, has_error", "curl" => "curl http://localhost:{live_api_port}/status" }))
    endpoints |> push(JV({ "method" => "GET", "path" => "/error", "description" => "Last compilation error (plain text, or null)", "curl" => "curl http://localhost:{live_api_port}/error" }))
    endpoints |> push(JV({ "method" => "POST", "path" => "/reload", "description" => "Trigger incremental reload", "curl" => "curl -X POST http://localhost:{live_api_port}/reload" }))
    endpoints |> push(JV({ "method" => "POST", "path" => "/reload/full", "description" => "Trigger full recompile", "curl" => "curl -X POST http://localhost:{live_api_port}/reload/full" }))
    endpoints |> push(JV({ "method" => "POST", "path" => "/pause", "description" => "Pause the host", "curl" => "curl -X POST http://localhost:{live_api_port}/pause" }))
    endpoints |> push(JV({ "method" => "POST", "path" => "/unpause", "description" => "Unpause the host", "curl" => "curl -X POST http://localhost:{live_api_port}/unpause" }))
    endpoints |> push(JV({ "method" => "POST", "path" => "/shutdown", "description" => "Graceful shutdown", "curl" => "curl -X POST http://localhost:{live_api_port}/shutdown" }))
    let cmd_desc = "Dispatch a [live_command] (JSON body: \{\"name\":\"cmd\",\"args\":\{...\}\})"
    let cmd_curl = "curl -X POST -H \"Content-Type: application/json\" -d '\{\"name\":\"help\"\}' http://localhost:{live_api_port}/command"
    endpoints |> push(JV({ "method" => "POST", "path" => "/command", "description" => cmd_desc, "curl" => cmd_curl }))
    let cmds_desc = "Dispatch a batch of [live_command]s in one round-trip (JSON array of \{name,args\}); continue-on-error, result array preserves input order"
    let cmds_curl = "curl -X POST -H \"Content-Type: application/json\" -d '[\{\"name\":\"help\"\}]' http://localhost:{live_api_port}/commands"
    endpoints |> push(JV({ "method" => "POST", "path" => "/commands", "description" => cmds_desc, "curl" => cmds_curl }))
    var result : table<string; JsonValue?>
    result |> insert("endpoints", JV(endpoints))
    let err = get_last_error()
    if (!empty(err)) {
        result |> insert("error", JV(err))
    }
    return write_json(JV(result))
}

// --- Server ---

class LiveApiServer : HvWebServer {
    def override onInit() {
        allow_cors()

        GET("/status") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let status = LiveStatus(
                fps = get_fps(),
                uptime = get_uptime(),
                paused = is_paused(),
                dt = get_dt(),
                has_error = !empty(get_last_error())
            )
            return resp |> JSON(write_json(JV(status)), http_status.OK)
        }

        GET("/error") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let err = get_last_error()
            if (empty(err)) return resp |> JSON(write_json(JVNull()), http_status.OK)
            set_header(resp, "Content-Type", "text/plain")
            resp.body := err
            return http_status.OK
        }

        POST("/reload") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            request_reload(false)
            return resp |> JSON(write_json(JV(true)), http_status.OK)
        }

        POST("/reload/full") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            request_reload(true)
            return resp |> JSON(write_json(JV(true)), http_status.OK)
        }

        POST("/pause") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let guard = compilation_error_response(resp)
            if (guard != http_status.OK) return guard
            set_paused(true)
            return resp |> JSON(write_json(JV(true)), http_status.OK)
        }

        POST("/unpause") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let guard = compilation_error_response(resp)
            if (guard != http_status.OK) return guard
            set_paused(false)
            return resp |> JSON(write_json(JV(true)), http_status.OK)
        }

        POST("/shutdown") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            request_exit()
            return resp |> JSON(write_json(JV(true)), http_status.OK)
        }

        POST("/command") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let guard = compilation_error_response(resp)
            if (guard != http_status.OK) return guard
            let body = string(req.body)
            if (empty(body)) return resp |> JSON(write_json(JV("empty body")), http_status.BAD_REQUEST)
            let result = dispatch_command(body)
            return resp |> JSON(result, http_status.OK)
        }

        // Batch: array of {name, args} objects. Continue-on-error — result array
        // preserves input order; malformed entries get {"error":...} in their slot.
        POST("/commands") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let guard = compilation_error_response(resp)
            if (guard != http_status.OK) return guard
            let body = string(req.body)
            if (empty(body)) return resp |> JSON(write_json(JV("empty body")), http_status.BAD_REQUEST)
            var parse_error = ""
            var parsed = read_json(body, parse_error)
            if (parsed == null || !(parsed.value is _array)) {
                return resp |> JSON(write_json(JV("body must be a JSON array of \{name,args\} objects")), http_status.BAD_REQUEST)
            }
            let arr & = unsafe(parsed.value as _array)
            let result = build_string() $(var w) {
                write_char(w, int('['))
                var first = true
                for (entry in arr) {
                    if (!first) write_char(w, int(','))
                    first = false
                    let entry_v = unsafe(reinterpret<JsonValue?>(entry))
                    if (entry_v == null || !(entry_v.value is _object)) {
                        write(w, "\{\"error\":\"invalid command object\"}")
                        continue
                    }
                    let entry_obj & = unsafe(entry_v.value as _object)
                    let name_v = unsafe(reinterpret<JsonValue?>(entry_obj?["name"] ?? null))
                    if (name_v == null || !(name_v.value is _string)) {
                        write(w, "\{\"error\":\"missing or non-string name\"}")
                        continue
                    }
                    write(w, dispatch_command(write_json(entry_v)))
                }
                write_char(w, int(']'))
            }
            return resp |> JSON(result, http_status.OK)
        }

        // Help on the root path only — no ANY("*") catch-all. libhv's
        // HttpService stores routes in std::unordered_map<std::string,...>
        // (vendored libhv include/hv/HttpService.h:105) and GetRoute
        // iterates in container order, taking the first wildcard or path
        // match. With ANY("*") registered, libhv's bucket layout on Linux
        // libstdc++ (and intermittently on Windows MSVC, depending on
        // rehash timing) enumerates "*" BEFORE specific paths like
        // /status, /shutdown, /command — so every request hits the
        // wildcard handler, request_exit() never runs, and daslang-live
        // never exits. Symptom: dasImgui playwright tests hang for the
        // full popen watchdog (120 s) on POSIX, get SIGKILLed, drain ends
        // with popen_exit_code=DAS_POPEN_TIMEOUT (0x7FFFFF01).
        //
        // GET / is a specific exact-match path, so it never collides with
        // /status etc. Unknown paths now return libhv's default 404 —
        // programmatic clients (playwright, MCP live tools) never relied
        // on the catch-all. Humans typing curl :9090/ still get the help
        // JSON.
        //
        // Proper fix lives in libhv (sort wildcards last in GetRoute, or
        // switch pathHandlers to std::map); tracking upstream.
        GET("/") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON(live_api_help_json(), http_status.OK)
        }
    }
}

// --- Agent ---

class LiveApiAgent : DapiDebugAgent {
    server : LiveApiServer?
    def LiveApiAgent() {
        server = new LiveApiServer()
        server->init(live_api_port)
        server->start()
        print("live_api: server started on port {live_api_port}\n")
    }
    def operator delete() {
        if (server != null) {
            server->stop()
            server->cleanup()
            unsafe {
                delete server
            }
        }
    }
    def override onTick() : void {
        if (server != null) {
            server->tick()
        }
    }
}

def debug_agent(ctx : Context) {
    assert(this_context().category.debug_context)
    install_new_debug_agent(new LiveApiAgent(), "live_api")
}

[_macro]
def installing() {
    if (is_compiling_macros_in_module("live_api") && !is_in_completion()) {
        if (!is_in_debug_agent_creation() && is_live_mode()) {
            if (!has_debug_agent_context("live_api")) {
                fork_debug_agent_context(@@debug_agent)
            }
        }
    }
}
