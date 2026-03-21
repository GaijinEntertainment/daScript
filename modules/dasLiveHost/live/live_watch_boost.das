options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module live_watch_boost shared public

//! Diagnostic [live_command] functions for the file watcher.
//!
//! Require this module to add ``cmd_watch_status`` and ``cmd_watch_touch``
//! commands to any live application::
//!
//!     require live/live_watch_boost

require daslib/fio
require daslib/debugger
require daslib/json
require daslib/json_boost
require live_host
require live/live_watch
require live/live_commands

// --- Structs ---

struct WatchFileInfo {
    path : string
    size : uint64
    mtime : int64
    stat_ok : bool
}

struct WatchStatusResult {
    file_count : int
    uptime : float
    has_agent : bool
    files : array<WatchFileInfo>
}

// --- Commands ---

[live_command(description="Show watched files with stat info (mtime, size)")]
def cmd_watch_status(input : JsonValue?) : JsonValue? {
    let count = get_watched_file_count()
    var files : array<WatchFileInfo>
    for (i in range(count)) {
        let f = get_watched_file(i)
        if (!empty(f)) {
            var st : FStat
            if (stat(f, st) && st.is_reg) {
                files |> push(WatchFileInfo(path = f, size = st.size, mtime = int64(st.mtime), stat_ok = true))
            } else {
                files |> push(WatchFileInfo(path = f, stat_ok = false))
            }
        }
    }
    return JV(WatchStatusResult(
        file_count = count,
        uptime = get_uptime(),
        has_agent = has_debug_agent_context("live_watch"),
        files <- files
    ))
}

[live_command(description="Signal files changed (bypass watcher, trigger reload)")]
def cmd_watch_signal(input : JsonValue?) : JsonValue? {
    signal_files_changed()
    return JV("signaled")
}

[live_command(description="Touch first watched file to trigger watcher reload")]
def cmd_watch_touch(input : JsonValue?) : JsonValue? {
    let count = get_watched_file_count()
    if (count <= 0) {
        return JV("no watched files")
    }
    let f = get_watched_file(0)
    if (empty(f)) {
        return JV("first watched file is empty")
    }
    var ok = false
    fopen(f, "ab") <| $(fh) {
        if (fh != null) {
            ok = true
        }
    }
    if (ok) {
        return JV("touched {f}")
    } else {
        return JV("failed to touch {f}")
    }
}
