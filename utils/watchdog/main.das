options gen2
options gc
options persistent_heap
options stack = 65536

require daslib/fio
require daslib/clargs
require watchdog
require stdio_front

//! Two hosts share this entry. The standalone exe (`bin/watchdog`, a `-ctx` context on the full
//! runtime) calls `start` / `tick` / `request_stop` / `result` from its C++ main, which owns argv,
//! the pid and the signals. The interpreter (`daslang utils/watchdog/main.das -- --cwd <dir> ...`)
//! runs `main`, the same supervision without the pid file or a signal hook. Both collect the heaps
//! between ticks: every log line and every tray refresh leaves strings and JSON nodes behind, and
//! a supervisor runs for months.

let private COLLECT_SECONDS = 60
let private COLLECT_STRING_BYTES = 1024ul * 1024ul

var private g_supervisor : Supervisor?
var private g_front : StdioFront?
var private g_exit_code = 0

//! `--stdio` before the `--` picks the front over the supervisor
def private wants_stdio(args : array<string>) : bool {
    for (arg in args) {
        if (arg == "--") break
        if (arg == "--stdio") return true
    }
    return false
}
var private g_last_collect = 0l
var private g_strings_after_collect = 0ul

def private program_dir_from(args : array<string>) : string {
    for (i in range(length(args) - 1)) {
        if (args[i] == "--cwd") return get_full_file_name(args[i + 1])
        if (args[i] == "--") break
    }
    return getcwd()
}

//! Every minute, or as soon as the string heap grew a megabyte past what the last collect left
//! live - a fixed ceiling would collect every tick once the live set outgrew it - with only this
//! module's frame on the stack: the collector walks the interpreter stack from here.
def private collect_if_due() {
    let due = (g_last_collect == 0l || get_time_usec(g_last_collect) > COLLECT_SECONDS * 1000000
        || string_heap_bytes_allocated() > g_strings_after_collect + COLLECT_STRING_BYTES)
    if (!due) return
    unsafe {
        heap_collect(true, false)
    }
    g_strings_after_collect = string_heap_bytes_allocated()
    g_last_collect = ref_time_ticks()
}

//! The exe host. `argv[0]` locates the bundle: `watchdog.json` and the discovered program sit
//! beside the executable, exactly where the Python sat beside what it supervised.
[export]
def start(self_pid : int) : bool {
    let argv <- get_command_line_arguments()
    let args <- [for (i in range(1, length(argv))); argv[i]]
    let program_dir = !empty(argv) ? dir_name(get_full_file_name(argv[0])) : getcwd()
    if (wants_stdio(args)) {
        g_front = stdio_front_start(args, program_dir, g_exit_code)
        return g_front != null
    }
    g_supervisor = watchdog_start(args, program_dir, self_pid, g_exit_code)
    return g_supervisor != null
}

[export]
def tick() : bool {
    var done = true
    if (g_front != null) {
        done = g_front->tick()
    } elif (g_supervisor != null) {
        done = g_supervisor->tick()
    }
    collect_if_due()
    return done
}

[export]
def request_stop() {
    if (g_supervisor != null) {
        g_supervisor->request_stop()
    }
}

[export]
def result() : int {
    if (g_front != null) {
        g_exit_code = g_front.result
        unsafe {
            delete g_front
        }
    }
    if (g_supervisor != null) {
        g_exit_code = g_supervisor.result
        unsafe {
            delete g_supervisor
        }
    }
    return g_exit_code
}

//! The interpreter host: the program directory is `--cwd`, or the current directory.
[export]
def main() : int {
    let args <- get_cli_arguments()
    var code = 0
    if (wants_stdio(args)) {
        var front = stdio_front_start(args, program_dir_from(args), code)
        if (front == null) return code
        while (!front->tick()) {
            collect_if_due()
        }
        code = front.result
        unsafe {
            delete front
        }
        return code
    }
    var sup = watchdog_start(args, program_dir_from(args), 0, code)
    if (sup == null) return code
    while (!sup->tick()) {
        collect_if_due()
    }
    code = sup.result
    unsafe {
        delete sup
    }
    return code
}
