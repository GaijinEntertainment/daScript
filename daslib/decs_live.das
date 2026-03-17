options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module decs_live shared public

//! DECS persistence across live reloads.
//!
//! Just `require daslib/decs_live` in your script — DECS state is automatically
//! saved before reload and restored after. No boilerplate needed.

require daslib/decs
require daslib/archive
require live_host

[before_reload]
def save_decs_state() {
    //! Serializes the current DECS state to the persistent store before a live reload.
    var data <- mem_archive_save(decsState)
    live_store_bytes("__decs_state", data)
}

[after_reload]
def restore_decs_state() {
    //! Restores DECS state from the persistent store after a live reload.
    var data : array<uint8>
    if (live_load_bytes("__decs_state", data)) {
        try {
            data |> mem_archive_load(decsState)
        } recover {
            print("decs_live: failed to restore DECS state\n")
        }
    }
}
