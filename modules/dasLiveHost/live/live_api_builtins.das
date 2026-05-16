options gen2
options rtti
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module live_api_builtins shared public

//! Transport-agnostic built-in live commands.
//!
//! Registers the lifecycle/status commands as `[live_command]`s so any
//! transport that routes through `dispatch_command` (HTTP via `live_api`,
//! stdin/stdout JSON-RPC via `live_api_stdio`, or anything else) can
//! invoke them by name.
//!
//! Commands:
//!   status       — JSON with fps, uptime, paused, dt, has_error
//!   last_error   — last compilation error string, or JSON null if none
//!   reload       — incremental reload (no recompile of @live)
//!   reload_full  — full recompile (clears @live vars)
//!   pause        — pauses execution
//!   unpause      — resumes execution
//!   shutdown     — graceful process exit

require daslib/json
require live/live_commands
require live_host

[live_command(description="JSON with fps, uptime, paused, dt, has_error")]
def status(input : JsonValue?) : JsonValue? {
    // Build the response as a table rather than via `JV(LiveStatus(...))`.
    // The json_boost macro-generated `apply` instance for an in-module
    // struct hashes non-deterministically across AOT-gen vs runtime when
    // the module is required transitively, causing error[50101] link
    // failures in test_aot. The HTTP transport keeps its own LiveStatus
    // struct in live_api.das where it is the only consumer.
    var tab : table<string; JsonValue?>
    tab |> insert("fps", JV(get_fps()))
    tab |> insert("uptime", JV(get_uptime()))
    tab |> insert("paused", JV(is_paused()))
    tab |> insert("dt", JV(get_dt()))
    tab |> insert("has_error", JV(!empty(get_last_error())))
    return JV(tab)
}

[live_command(description="Last compilation error string, or JSON null if none")]
def last_error(input : JsonValue?) : JsonValue? {
    let err = get_last_error()
    return empty(err) ? JVNull() : JV(err)
}

[live_command(description="Trigger incremental reload")]
def reload(input : JsonValue?) : JsonValue? {
    request_reload(false)
    return JV(true)
}

[live_command(description="Trigger full recompile (clears @live vars)")]
def reload_full(input : JsonValue?) : JsonValue? {
    request_reload(true)
    return JV(true)
}

[live_command(description="Pause execution")]
def pause(input : JsonValue?) : JsonValue? {
    set_paused(true)
    return JV(true)
}

[live_command(description="Resume execution")]
def unpause(input : JsonValue?) : JsonValue? {
    set_paused(false)
    return JV(true)
}

[live_command(description="Graceful shutdown")]
def shutdown(input : JsonValue?) : JsonValue? {
    request_exit()
    return JV(true)
}
