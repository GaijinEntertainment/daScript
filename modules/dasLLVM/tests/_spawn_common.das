options gen2
module _spawn_common shared public

require daslib/fio
require daslib/env_registry
require strings

//! runs `cmd` with stderr folded into stdout, collects every non-empty stripped line, returns the exit code
def spawn_child(cmd : string; var lines : array<string>) : int {
    let rc = unsafe(popen_timeout("{cmd} 2>&1", 300.0) $(f) {
        if (f == null) {
            return
        }
        while (!feof(f)) {
            let ln = strip(fgets(f))
            if (!empty(ln)) {
                lines |> push("{ln}")
            }
        }
    })
    return rc
}

//! runs `blk` with DAS_LOG_LEVEL=info for the children it spawns - their `LLVM JIT` lines and `--jit-dump` log at info - and puts the caller's level back after
def with_child_log_info(blk : block<() : void>) {
    let saved = env_value_of("DAS_LOG_LEVEL")
    set_env_variable("DAS_LOG_LEVEL", "info")
    invoke(blk)
    set_env_variable("DAS_LOG_LEVEL", saved)
}
