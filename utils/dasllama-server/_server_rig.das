options gen2
options _dasllama_internal = true

module _server_rig shared

require dastest/testing_boost public
require daslib/fio
require dasllama/dasllama_env

//! The server suites' model roots: ONE resolution - `DASLLAMA_MODELS_DIR`, else the rig default -
//! so a test file never spells a second one.
def models_dir() : string => models_dir_resolved()

//! The suites' model gate: a JIT-only run skips interpreted, a missing file skips by name.
def model_missing(t : T?; path : string) : bool {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA server test is JIT-only)")
        return true
    }
    if (!stat(path).is_valid) {
        t |> skip("{base_name(path)} not present (set DASLLAMA_MODELS_DIR)")
        return true
    }
    return false
}
