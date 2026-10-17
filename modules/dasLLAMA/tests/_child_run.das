options gen2
options _dasllama_internal = true

module _child_run shared public

require daslib/fio
require strings

// a test's child process: this binary on a root fixture of this folder, under named environment
// settings, its stdout and stderr read whole - the instrument of every once-per-process contract

//! the running daslang binary, as the command line names it
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

//! run `cmd` to its end (five minutes at most); `output` is everything it printed, the result its exit code
def run_child(cmd : string; var output : string&) : int {
    return unsafe(popen_timeout(cmd, 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

//! the command that runs `root` (a file of this folder) under `envs` on this platform's shell, stderr folded into stdout;
//! an empty value unsets the variable on both shells; `args` follow the root after `--`, and `jit` runs it under -jit
def child_cmd(root : string; envs : array<tuple<name : string; value : string>>; args : string = ""; jit : bool = false) : string {
    let win = get_platform_name() == "windows"
    let prefix = build_string() $(w) {
        for (e in envs) {
            w |> write(win ? "set \"{e.name}={e.value}\"&& " : "{e.name}={e.value} ")
        }
    }
    let flags = jit ? " -jit" : ""
    let tail = empty(args) ? "" : " -- {args}"
    return "{prefix}\"{das_exe()}\"{flags} -dasroot \"{get_das_root()}\" \"{get_das_root()}/modules/dasLLAMA/tests/{root}\"{tail} 2>&1"
}
