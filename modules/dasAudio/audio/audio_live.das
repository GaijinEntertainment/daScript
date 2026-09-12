options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module audio_live shared public

//! Preserves audio_boost handles (command stream, sound SID) across live reloads.
//! Just `require live/audio_live` — no boilerplate needed.
//!
//! The opaque pointers (`Stream?`, `Atomic64?`) live in the C++ DLL and survive
//! reload, but the daScript globals that reference them are lost. This module
//! saves them as `uint64` via Archive before reload and restores them after
//! reload; the stream's reference passes from the old context's global to the
//! new one (`adopt_audio_thread_command_stream`), so a reload leaves the count
//! where the real finalize expects it.

require daslib/archive
require jobque   //! the Stream and Atomic64 types audio_live_channels casts to
require audio/audio_boost
require live_host

let private STORE_KEY = "__audio_live_handles"

def public audio_live_keeps_handles : bool {
    //! True in a shutdown() a reload runs - the audio system stays up and its handles carry
    //! over (a reset and a full reload clear the store's data entries and keep its
    //! infrastructure entries, the handles among them); false in the real shutdown. A program's
    //! shutdown() finalizes the audio system exactly when this is false.
    return is_reload()
}

[before_reload]
def private save_audio_handles() {
    var cmd_ch = unsafe(reinterpret<uint64>(get_audio_command_stream()))
    var sid_ptr = unsafe(reinterpret<uint64>(get_sound_sid()))
    if (cmd_ch == 0ul && sid_ptr == 0ul) return
    var ser = new MemSerializer()
    var arch = Archive(reading = false, stream = ser)
    arch |> serialize(cmd_ch)
    arch |> serialize(sid_ptr)
    let data <- ser->extractData()
    live_store_bytes(STORE_KEY, data)
    unsafe { delete ser; }
}

def public audio_live_channels : AudioSystemChannels {
    //! The handles this module carried across the reload, for a program whose own copy
    //! (the tuple `audio_system_create` answered) the reload zeroed.
    return (
        command = unsafe(reinterpret<Stream?>(get_audio_command_stream())),
        next_sid = unsafe(reinterpret<Atomic64?>(get_sound_sid()))
    )
}

def public audio_live_system : AudioSystemChannels {
    //! The audio system a live program keeps: created on a cold start, the carried-over
    //! handles after a reload. A program's init() assigns its channels from this.
    return get_audio_command_stream() == null ? audio_system_create() : audio_live_channels()
}

def public audio_live_finalize(var channels : AudioSystemChannels) {
    //! A program's shutdown() counterpart of audio_live_system: finalizes the audio system
    //! at the real shutdown and leaves it up through a reload.
    if (!audio_live_keeps_handles()) {
        audio_system_finalize(channels.command, channels.next_sid)
    }
}

[after_reload]
def private restore_audio_handles() {
    var data : array<uint8>
    if (live_load_bytes(STORE_KEY, data) && !empty(data)) {
        var ser = new MemSerializer(data)
        var arch = Archive(reading = true, stream = ser)
        var cmd_ch : uint64
        var sid_ptr : uint64
        arch |> serialize(cmd_ch)
        arch |> serialize(sid_ptr)
        unsafe {
            adopt_audio_thread_command_stream(reinterpret<void?>(cmd_ch))
            set_sound_sid(reinterpret<void?>(sid_ptr))
            delete ser
        }
    }
}
