options gen2

require daslib/debugger
require daslib/fio
require jobque

class ObserverAgent : DapiDebugAgent {
}

class WorkerAgent : DapiDebugAgent {
}

def setup_observer(_ctx : Context) {
    install_new_debug_agent(new ObserverAgent(), "context_destroy_observer")
}

def setup_worker(_ctx : Context) {
    install_new_debug_agent(new WorkerAgent(), "context_destroy_worker")
}

[export, pinvoke]
def start_waiting_worker() {
    new_debugger_thread() {
        panic("destroyed context worker ran")
    }
}

[export]
def main() {
    let args <- get_command_line_arguments()
    let marker = args[length(args) - 1]
    fork_debug_agent_context(@@setup_observer)
    fork_debug_agent_context(@@setup_worker)
    unsafe {
        invoke_in_context(
            get_debug_agent_context("context_destroy_worker"),
            "start_waiting_worker")
    }
    delete_debug_agent_context("context_destroy_worker")
    var started = false
    while (!started) {
        try {
            new_debugger_thread() {
                let worker_args <- get_command_line_arguments()
                fwrite(worker_args[length(worker_args) - 1], "done")
                print("DAP_CONTEXT_DESTROY_RECOVERED=1\n")
            }
            started = true
        } recover {
            sleep(1u)
        }
    }
    debugger_thread_context_ready(this_context())
    while (!fexist(marker)) {
        sleep(1u)
    }
    delete_debug_agent_context("context_destroy_observer")
}
