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

// --- Server ---

class LiveApiServer : HvWebServer {
    def override onInit() {
        allow_cors()

        GET("/status") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let status_json = "\{\"fps\": {get_fps()}, \"uptime\": {get_uptime()}, \"paused\": {is_paused()}, \"dt\": {get_dt()}}"
            return resp |> JSON(status_json, http_status.OK)
        }

        POST("/reload") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            request_reload(false)
            return resp |> JSON("\{\"ok\": true}", http_status.OK)
        }

        POST("/reload/full") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            request_reload(true)
            return resp |> JSON("\{\"ok\": true}", http_status.OK)
        }

        POST("/command") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let body = string(req.body)
            if (empty(body)) {
                return resp |> JSON("\{\"error\": \"empty body\"}", http_status.BAD_REQUEST)
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
