options gen2

require debugapi
require daslib/fio
require jobque

[export]
def main() {
    let args <- get_command_line_arguments()
    let first_marker = args[length(args) - 2]
    let second_marker = args[length(args) - 1]
    new_debugger_thread() {
        let worker_args <- get_command_line_arguments()
        fwrite(worker_args[length(worker_args) - 2], "done")
        print("DAP_LIFECYCLE_WORKER=1\n")
    }
    debugger_thread_context_ready(this_context())
    while (!fexist(first_marker)) {
        sleep(1u)
    }
    var started = false
    while (!started) {
        try {
            new_debugger_thread() {
                let worker_args <- get_command_line_arguments()
                fwrite(worker_args[length(worker_args) - 1], "done")
                print("DAP_LIFECYCLE_WORKER=2\n")
            }
            started = true
        } recover {
            sleep(1u)
        }
    }
    debugger_thread_context_ready(this_context())
    while (!fexist(second_marker)) {
        sleep(1u)
    }
    print("DAP_LIFECYCLE=2\n")
}
