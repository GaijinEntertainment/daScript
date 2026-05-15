options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module strudel_live shared public

//! Preserves strudel_player state (bank, timing, SID) and SF2 soundfont
//! across live reloads. Just `require strudel/strudel_live` — no boilerplate needed.
//!
//! SF2 data is serialized via Archive with bulk memcpy for the sample_data
//! array (~140MB for a GM font). This avoids re-reading from disk on reload.

require strudel/strudel_player
require live_host

let private STORE_KEY = "__strudel_player"
let private SF2_STORE_KEY = "__strudel_sf2"

[before_reload]
def private save_strudel_state() {
    var t0 = ref_time_ticks()
    let data <- strudel_save_state()
    to_log(0, "strudel_live: save_state {get_time_usec(t0)}us ({length(data)} bytes)\n")
    if (!empty(data)) {
        t0 = ref_time_ticks()
        live_store_bytes(STORE_KEY, data)
        to_log(0, "strudel_live: store player {get_time_usec(t0)}us\n")
    }
    t0 = ref_time_ticks()
    let sf2_data <- strudel_save_sf2_state()
    to_log(0, "strudel_live: save_sf2 {get_time_usec(t0)}us ({length(sf2_data)} bytes)\n")
    if (!empty(sf2_data)) {
        t0 = ref_time_ticks()
        live_store_bytes(SF2_STORE_KEY, sf2_data)
        to_log(0, "strudel_live: store sf2 {get_time_usec(t0)}us\n")
    }
}

[after_reload]
def private restore_strudel_state() {
    var t0 = ref_time_ticks()
    var sf2_data : array<uint8>
    if (live_load_bytes(SF2_STORE_KEY, sf2_data) && !empty(sf2_data)) {
        to_log(0, "strudel_live: load_bytes sf2 {get_time_usec(t0)}us ({length(sf2_data)} bytes)\n")
        t0 = ref_time_ticks()
        strudel_restore_sf2_state(sf2_data)
        to_log(0, "strudel_live: restore_sf2 {get_time_usec(t0)}us\n")
    }
    t0 = ref_time_ticks()
    var data : array<uint8>
    if (live_load_bytes(STORE_KEY, data) && !empty(data)) {
        to_log(0, "strudel_live: load_bytes player {get_time_usec(t0)}us ({length(data)} bytes)\n")
        t0 = ref_time_ticks()
        strudel_restore_state(data)
        to_log(0, "strudel_live: restore_state {get_time_usec(t0)}us\n")
    }
}
