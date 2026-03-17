options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module live_watch shared public

//! File watcher debug agent for daslang-live.
//!
//! Polls `stat()` on watched files and calls `signal_files_changed()`
//! when any file's mtime changes. Auto-installs as a named debug agent.
//!
//! The host stores watched files in LiveHostState after compilation.
//! The agent reads them via `get_watched_file_count()` / `get_watched_file()`.

require fio
require debugapi
require live_host

// --- Watched file state ---

var watch_mtimes : table<string; clock>
var watch_last_check : clock
var watch_file_count : int = -1

// --- Refresh file list from host ---

def private refresh_file_list() {
    let count = get_watched_file_count()
    if (count == watch_file_count) {
        return
    }
    watch_file_count = count
    watch_mtimes |> clear()
    for (i in range(count)) {
        let f = get_watched_file(i)
        if (!empty(f)) {
            var st : FStat
            if (stat(f, st) && st.is_reg) {
                watch_mtimes[f] = st.mtime
            }
        }
    }
    watch_last_check = get_clock()
}

// --- Agent ---

class LiveWatchAgent : DapiDebugAgent {
    def override onInstall(agent : DebugAgent?) : void {
        pass
    }
    def override onUninstall(agent : DebugAgent?) : void {
        pass
    }
    def override onTick() : void {
        // Refresh file list if host updated it
        refresh_file_list()
        if (watch_file_count <= 0) {
            return
        }
        // Throttle: check at most once per second
        let now = get_clock()
        if (float(now - watch_last_check) < 1.0) {
            return
        }
        watch_last_check = now
        // Check file mtimes
        for (f, mtime in keys(watch_mtimes), values(watch_mtimes)) {
            var st : FStat
            if (stat(f, st) && st.is_reg) {
                if (mtime != st.mtime) {
                    mtime = st.mtime
                    signal_files_changed()
                    return
                }
            }
        }
    }
}

def debug_agent(ctx : Context) {
    assert(this_context().category.debug_context)
    install_new_debug_agent(new LiveWatchAgent(), "live_watch")
}

[_macro]
def installing() {
    if (is_compiling_macros_in_module("live_watch") && !is_in_completion()) {
        if (!is_in_debug_agent_creation() && is_live_mode()) {
            if (!has_debug_agent_context("live_watch")) {
                fork_debug_agent_context(@@debug_agent)
            }
        }
    }
}
