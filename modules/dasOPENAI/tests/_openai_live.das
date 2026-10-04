// Test fixture: the gate a live cell opens on. A live cell talks to a server this repo does not start -
// Ollama, a local speech server, OpenRouter - and skips, naming what is missing, on a box without it.
//
// The leading underscore keeps dastest from treating this file as a test.

options gen2

require dastest/testing_boost public
require dashv/dashv_boost
require daslib/fio

//! whether a server answers at `base_url`; when none does the cell is skipped by name
def live_server(t : T?; base_url : string) : bool {
    var up = false
    GET("{base_url}/models") $(resp) {
        up = resp != null && int(resp.status_code) > 0
    }
    if (!up) {
        t |> skip("no server at {base_url}")
    }
    return up
}

//! whether the environment carries the key a hosted cell needs; when it does not the cell is skipped by name
def live_key(t : T?; name : string) : bool {
    let has = has_env_variable(name)
    if (!has) {
        t |> skip("{name} is not set")
    }
    return has
}
