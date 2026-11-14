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

//! A shell prefix that sets each (name, value) pair for the one child
//! command after it: `set K=V&& ` a pair on Windows, `K=V `
//! elsewhere. An empty value clears the variable for the child.
def child_env(vars : array<tuple<string; string>>) : string {
    let windows = get_platform_name() == "windows"
    return build_string() $(var w) {
        for (v in vars) {
            w |> write(windows ? "set {v._0}={v._1}&& " : "{v._0}={v._1} ")
        }
    }
}

//! runs `blk` with DAS_LOG_LEVEL=info for the children it spawns - their `LLVM JIT` lines and `--jit-dump` log at info - and puts the caller's level back after
def with_child_log_info(blk : block<() : void>) {
    let saved = env_value_of("DAS_LOG_LEVEL")
    set_env_variable("DAS_LOG_LEVEL", "info")
    invoke(blk)
    set_env_variable("DAS_LOG_LEVEL", saved)
}
