#pragma once

#include "daScript/daScript.h"
#include <chrono>

namespace das {

    // Shared state between C++ host (daslang-live.exe) and Module_LiveHost.
    // Always lives in the DLL. Host accesses it via exported C functions.
    struct LiveHostState {
        // Flags (set by script, read by host)
        bool exit_requested = false;
        bool reload_requested = false;
        bool full_reload = false;
        bool reset_requested = false;   // re-simulate the compiled program, no recompile (clean-slate)

        // Flags (set by host, read by script)
        bool live_mode = false;
        bool is_reload = false;
        bool paused = false;

        // Bumped by host after every completed reload/reset. Clients poll /status and
        // wait for this to change for a deterministic "reload/reset done" signal.
        uint64_t reload_generation = 0;

        // Timing (set by host, or computed internally)
        float dt = 0.0f;
        float uptime = 0.0f;        // float mirror of uptime_accum (the daslang-visible value)
        // Accumulate uptime in double to avoid cumulative float rounding drift over long
        // sessions (float ulp at ~1h is ~5e-4s, which drifts seconds across ~200k frames).
        // `uptime` is kept as float(uptime_accum) for the get_uptime() binding.
        double uptime_accum = 0.0;
        float fps = 0.0f;
        std::chrono::steady_clock::time_point last_time;
        bool time_initialized = false;
        // Lockstep recording: 0 = wall-clock (normal), > 0 = fixed dt per frame.
        // When set, the host advances the clock by exactly fixed_dt each frame
        // instead of measured wall time, so capture cadence, animation, and the
        // convert grid all sit on one uniform content clock. Set by the recorder.
        float fixed_dt = 0.0f;

        // Error (set by host)
        string last_error;

        // File watcher (set by file watcher agent)
        bool files_changed = false;

        // Watched files (set by host after compile)
        vector<string> watched_files;

        // Persistent key-value store (survives reloads)
        mutex store_mutex;
        unordered_map<string, vector<uint8_t>> store;
        // String-typed sibling of `store`. Used by live_store_string / live_load_string,
        // the JSON-text rail for the LiveVarsPassMacro / dasImgui widget serializer
        // pivot (PR1 of the sprint_json_at refactor). Kept separate from `store`
        // so the runtime debugger can present JSON text directly without bytes-as-string
        // squinting.
        unordered_map<string, string> store_strings;

        // Command dispatch bridge (set by host after compile)
        Context * dispatch_context = nullptr;
        SimFunction * dispatch_fn = nullptr;

        // Set by DLL when command exception kills context
        bool context_dead = false;
    };

    // Functions bound to daScript — declared here for AOT.
    bool live_is_live_mode();
    void live_request_exit();
    bool live_exit_requested();
    void live_request_reload(bool full);
    void live_request_reset();
    uint64_t live_get_reload_generation();
    bool live_is_reload();
    float live_get_dt();
    float live_get_uptime();
    float live_get_fps();
    void live_set_fixed_dt(float v);
    bool live_is_paused();
    void live_set_paused(bool v);
    const char * live_get_last_error(Context * ctx);
    void live_signal_files_changed();
    int32_t live_get_watched_file_count();
    const char * live_get_watched_file(int32_t index, Context * ctx);
    void live_store_bytes(const char * key, const TArray<uint8_t> & data);
    bool live_load_bytes(const char * key, TArray<uint8_t> & data, Context * ctx);
    void live_store_string(const char * key, const char * value);
    bool live_load_string(const char * key, char * & value, Context * ctx);
    void live_collect_gc(Context * ctx);
    void live_collect_string_gc(Context * ctx);
    const char * live_dispatch_command_via_host(const char * cmd_json, Context * callerCtx);

}
