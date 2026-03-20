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

        // Flags (set by host, read by script)
        bool live_mode = false;
        bool is_reload = false;
        bool paused = false;

        // Timing (set by host, or computed internally)
        float dt = 0.0f;
        float uptime = 0.0f;
        float fps = 0.0f;
        std::chrono::steady_clock::time_point last_time;
        bool time_initialized = false;

        // Error (set by host)
        string last_error;

        // File watcher (set by file watcher agent)
        bool files_changed = false;

        // Watched files (set by host after compile)
        vector<string> watched_files;

        // Persistent key-value store (survives reloads)
        mutex store_mutex;
        unordered_map<string, vector<uint8_t>> store;

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
    bool live_is_reload();
    float live_get_dt();
    float live_get_uptime();
    float live_get_fps();
    bool live_is_paused();
    void live_set_paused(bool v);
    const char * live_get_last_error(Context * ctx);
    void live_signal_files_changed();
    int32_t live_get_watched_file_count();
    const char * live_get_watched_file(int32_t index, Context * ctx);
    void live_store_bytes(const char * key, const TArray<uint8_t> & data);
    bool live_load_bytes(const char * key, TArray<uint8_t> & data, Context * ctx);
    void live_collect_gc(Context * ctx);
    void live_collect_string_gc(Context * ctx);
    const char * live_dispatch_command_via_host(const char * cmd_json, Context * callerCtx);

}
