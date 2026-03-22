options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module audio_live shared public

//! Preserves audio_boost handles (command channel, collect channel, sound SID)
//! across live reloads. Just `require live/audio_live` — no boilerplate needed.
//!
//! The three opaque pointers (`Channel?`, `Atomic64?`) live in the C++ DLL
//! and survive reload, but the daScript globals that reference them are lost.
//! This module saves them as `uint64` via Archive before reload and restores
//! them after reload using the add_ref/release-safe setters.

require daslib/archive
require audio/audio_boost
require live_host

let private STORE_KEY = "__audio_live_handles"

[before_reload]
def private save_audio_handles() {
    var cmd_ch = unsafe(reinterpret<uint64>(get_audio_command_channel()))
    var col_ch = unsafe(reinterpret<uint64>(get_collect_command_channel()))
    var sid_ptr = unsafe(reinterpret<uint64>(get_sound_sid()))
    if (cmd_ch == 0ul && col_ch == 0ul && sid_ptr == 0ul) {
        return
    }
    var ser = new MemSerializer()
    var arch = Archive(reading = false, stream = ser)
    arch |> serialize(cmd_ch)
    arch |> serialize(col_ch)
    arch |> serialize(sid_ptr)
    let data <- ser->extractData()
    live_store_bytes(STORE_KEY, data)
    unsafe { delete ser; }
}

[after_reload]
def private restore_audio_handles() {
    var data : array<uint8>
    if (live_load_bytes(STORE_KEY, data) && length(data) > 0) {
        var ser = new MemSerializer(data)
        var arch = Archive(reading = true, stream = ser)
        var cmd_ch : uint64
        var col_ch : uint64
        var sid_ptr : uint64
        arch |> serialize(cmd_ch)
        arch |> serialize(col_ch)
        arch |> serialize(sid_ptr)
        unsafe {
            set_audio_thread_command_channel(reinterpret<void?>(cmd_ch))
            set_collect_thread_command_channel(reinterpret<void?>(col_ch))
            set_sound_sid(reinterpret<void?>(sid_ptr))
            delete ser
        }
    }
}
