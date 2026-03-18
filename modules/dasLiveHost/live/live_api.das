options gen2
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
//!   GET  /status  — JSON with fps, uptime, paused, last_error
//!   POST /reload  — triggers a reload
//!   POST /command — dispatches a [live_command] via host bridge

require dashv/dashv_boost
require daslib/json
require daslib/json_boost
require daslib/jobque_boost
require debugapi
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
            if (empty(err)) {
                return resp |> JSON(write_json(JVNull()), http_status.OK)
            }
            // Return error as plain text to avoid JSON escaping issues
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
            set_paused(true)
            return resp |> JSON(write_json(JV(true)), http_status.OK)
        }

        POST("/unpause") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            set_paused(false)
            return resp |> JSON(write_json(JV(true)), http_status.OK)
        }

        POST("/command") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let body = string(req.body)
            if (empty(body)) {
                return resp |> JSON(write_json(JV("empty body")), http_status.BAD_REQUEST)
            }
            // Dispatch via C++ bridge — calls live_dispatch_command in the main context.
            // Safe because HV handler runs on the main thread during tick(),
            // and the main context is idle at that point.
            let result = dispatch_command(body)
            return resp |> JSON(result, http_status.OK)
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
