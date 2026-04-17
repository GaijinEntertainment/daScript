// Tutorial DASTRUDEL-15: Live-Reloading Patterns
//
// This tutorial covers:
//   - Running daStrudel under daslang-live.exe for instant code reload
//   - Lifecycle functions: init / update / shutdown
//   - [live_command] — REST-callable functions for external control
//   - Preserving player state across reloads via strudel_live
//   - The persistent byte store: live_store_bytes / live_load_bytes
//
// Run (live mode, recommended):
//   bin/Release/daslang-live.exe tutorials/daStrudel/daStrudel_15_live_reloading.das
//
// Also works in standalone mode:
//   bin/Release/daslang.exe tutorials/daStrudel/daStrudel_15_live_reloading.das
//
// In live mode, edit this file and save — the pattern updates without
// stopping the audio stream. The SID stays the same, the sample bank stays
// loaded (no decode cost), and any tracked BPM/volume settings survive.

options gen2
options indenting = 4
options persistent_heap

require strudel/strudel public
require strudel/strudel_player
// strudel_live registers [before_reload]/[after_reload] hooks automatically —
// just require it and player state (tracks, SID, SF2 data) is preserved.
require strudel/strudel_live
require audio/audio_boost
require live/audio_live
require live/live_commands
require live/live_api
require live_host
require daslib/fio
require daslib/json
require daslib/json_boost

let MEDIA = "{get_das_root()}/examples/media"

// ──────────────────────────────────────────────────────────────────────────
// Part A — The pattern (edit and save to hot-reload)
// ──────────────────────────────────────────────────────────────────────────
//
// Every time this file is saved, daslang-live recompiles and calls init()
// again. strudel_live saves the player state before reload and restores it
// after — so the audio thread keeps ticking, but strudel_add_track picks up
// the newly compiled pattern.

def strudel_main() {
    // Classic drum pattern — try editing this! Save the file and hear it
    // change live. Suggested edits:
    //   s("bd [bd sd] hh [sd cp]")
    //   s("bd*2 sd*4, hh*8") |> gain(0.8)
    //   s("bd sd hh cp") |> speed(1.5)
    strudel_add_track(s("bd [hh hh] sd [hh cp]"))
    strudel_set_bpm(120.0lf)
    strudel_play()
}

def load_samples() {
    strudel_load_sound("{MEDIA}/drums/bd", "bd")
    strudel_load_sound("{MEDIA}/drums/sd", "sd")
    strudel_load_sound("{MEDIA}/drums/hh", "hh")
    strudel_load_sound("{MEDIA}/drums/cp", "cp")
}

// ──────────────────────────────────────────────────────────────────────────
// Part B — Live commands (REST API)
// ──────────────────────────────────────────────────────────────────────────
//
// [live_command] annotations expose functions over HTTP so external tools
// (or the MCP server's live_command tool) can drive the running script.
// Call with:
//   curl -X POST http://localhost:9090/command -d '{"name":"cmd_set_bpm","args":{"bpm":140}}'
//
// The input JsonValue is decoded into a struct via from_JV; the return
// value is serialized back to the caller.

struct CmdBpmArgs {
    bpm : double = 120.0lf
}

[live_command(description="Set tempo in BPM")]
def cmd_set_bpm(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdBpmArgs>)
    strudel_set_bpm(args.bpm)
    return JV("bpm set to {args.bpm}")
}

struct CmdVolumeArgs {
    volume : float = 1.0
    time : float = 0.0
}

[live_command(description="Set master volume (0..1) with optional fade time")]
def cmd_set_volume(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdVolumeArgs>)
    strudel_set_volume(args.volume, args.time)
    return JV("volume set to {args.volume} (fade {args.time}s)")
}

[live_command(description="Pause playback")]
def cmd_pause(input : JsonValue?) : JsonValue? {
    strudel_set_pause(true)
    return JV("paused")
}

[live_command(description="Unpause playback")]
def cmd_unpause(input : JsonValue?) : JsonValue? {
    strudel_set_pause(false)
    return JV("unpaused")
}

// ──────────────────────────────────────────────────────────────────────────
// Part C — Custom persistent state with live_store_bytes
// ──────────────────────────────────────────────────────────────────────────
//
// strudel_live handles the player, but your own script state also needs to
// survive a reload. [before_reload] and [after_reload] run at reload time;
// inside them, use live_store_bytes / live_load_bytes to stash raw bytes.
// Here we persist a simple reload-counter so we can see reloads happening.

var g_reload_count : int = 0

[before_reload]
def save_my_state() {
    var data : array<uint8>
    data |> resize(4)
    unsafe { *reinterpret<int?>(addr(data[0])) = g_reload_count; }
    live_store_bytes("tutorial15_reload_count", data)
}

[after_reload]
def restore_my_state() {
    var data : array<uint8>
    if (live_load_bytes("tutorial15_reload_count", data) && length(data) >= 4) {
        unsafe { g_reload_count = *reinterpret<int?>(addr(data[0])); }
        g_reload_count ++
        to_log(LOG_INFO, "tutorial15: reload #{g_reload_count}\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Lifecycle — daslang-live calls init() once, then update() every frame,
// then shutdown() on exit. init() also fires after every reload (but the
// audio system / player already exist, thanks to strudel_live).
// ──────────────────────────────────────────────────────────────────────────

var asch : AudioSystemChannels

[export]
def init() {
    print("daStrudel tutorial 15 — live reload\n")
    if (get_audio_command_stream() == null) {
        asch = audio_system_create()
    }
    load_samples()
    strudel_init(@@strudel_main)
    print("playing — edit this file and save to hot-reload the pattern!\n")
}

[export]
def update() {
    pass
}

[export]
def shutdown() {
    print("shutdown\n")
    strudel_shutdown()
    audio_system_finalize(asch.command, asch.next_sid)
}

// Standalone fallback — if run under daslang.exe, act as a regular demo.
[export]
def main() {
    print("daStrudel tutorial 15 (standalone — no hot-reload)\n")
    with_audio_system() {
        load_samples()
        strudel_init(@@strudel_main)
        print("playing 10 seconds...\n")
        sleep(10000u)
        strudel_shutdown()
    }
}
