options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module strudel_live shared public

//! Preserves strudel_player state (bank, timing, SID) across live reloads.
//! Just `require strudel/strudel_live` — no boilerplate needed.

require strudel/strudel_player
require live_host

let private STORE_KEY = "__strudel_player"

[before_reload]
def private save_strudel_state() {
    let data <- strudel_save_state()
    if (length(data) > 0) {
        live_store_bytes(STORE_KEY, data)
    }
}

[after_reload]
def private restore_strudel_state() {
    var data : array<uint8>
    if (live_load_bytes(STORE_KEY, data) && length(data) > 0) {
        strudel_restore_state(data)
    }
}
