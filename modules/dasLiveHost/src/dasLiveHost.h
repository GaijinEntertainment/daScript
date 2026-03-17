#pragma once

#include "daScript/daScript.h"

namespace das {

    // Shared state between C++ host (daslang-live.exe) and Module_LiveHost.
    // Always lives in the DLL. Host accesses it via exported C functions.
    struct LiveHostState {
        // Flags (set by script, read by host)
        bool exit_requested = false;
        bool reload_requested = false;
        bool full_reload = false;

        // Flags (set by host, read by script)
        bool is_reload = false;
        bool paused = false;

        // Timing (set by host)
        float dt = 0.0f;
        float uptime = 0.0f;
        float fps = 0.0f;

        // Error (set by host)
        string last_error;

        // File watcher (set by file watcher agent)
        bool files_changed = false;

        // Watched files (set by host after compile)
        vector<string> watched_files;

        // Persistent key-value store (survives reloads)
        mutex store_mutex;
        unordered_map<string, vector<uint8_t>> store;
    };

    // Functions bound to daScript — declared here for AOT.
    void live_request_exit();
    bool live_exit_requested();
    void live_request_reload(bool full);
    bool live_is_reload();
    float live_get_dt();
    float live_get_uptime();
    float live_get_fps();
    bool live_is_paused();
    const char * live_get_last_error(Context * ctx);
    void live_signal_files_changed();
    void live_store_bytes(const char * key, const TArray<uint8_t> & data);
    bool live_load_bytes(const char * key, TArray<uint8_t> & data, Context * ctx);
    void live_collect_gc(Context * ctx);
    void live_collect_string_gc(Context * ctx);

}
