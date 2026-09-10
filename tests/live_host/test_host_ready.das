options gen2
options no_aot
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/fio
require daslib/command_line
require daslib/strings_boost
require dashv/dashv_boost
require strings

//! daslang-live serves its HTTP API on the port its argv names. The host reads the LiveHost
//! shared module's lifecycle exports before any require names that module, so a module scan
//! that defers it has to bring it in first - otherwise live mode never turns on, no server
//! starts, and every client waits out its ready budget.

let PORT_FIRST = 19390
let PORT_SPAN = 20

//! the daslang-live beside the daslang running dastest; empty where the tree has not built it
def private live_exe() : string {
    let beside = path_join(dir_name(get_full_file_name(get_das_exe())), "daslang-live{get_platform_name() == "windows" ? ".exe" : ""}")
    return fexist(beside) ? beside : ""
}

def private status_answers(port : int) : bool {
    var ok = false
    with_http_request() $(var req) {
        req.method = http_method.GET
        req.url := "http://127.0.0.1:{port}/status"
        request(req) $(resp) {
            ok = resp != null && int(resp.status_code) == 200
        }
    }
    return ok
}

//! the first port of the span no host answers on, so a /status answer is this test's own child
//! and its /shutdown reaches nothing else; -1 when every port has a host
def private free_port() : int {
    for (port in range(PORT_FIRST, PORT_FIRST + PORT_SPAN)) {
        if (!status_answers(port)) return port
    }
    return -1
}

def private post_shutdown(port : int) {
    with_http_request() $(var req) {
        req.method = http_method.POST
        req.url := "http://127.0.0.1:{port}/shutdown"
        request(req) $(_resp) {
        }
    }
}

//! seconds until /status answers, draining the host's stdout meanwhile; -1 past the deadline
//! or once the host is gone
def private wait_ready(var host : process; port : int; seconds : float; var lines : array<string>) : float {
    let started = ref_time_ticks()
    while (float(get_time_usec(started)) < seconds * 1000000.0) {
        unsafe(process_drain(host) $(line) {
            lines |> push(line)
        })
        if (unsafe(process_poll(host)) != process_running) return -1.0
        if (status_answers(port)) return float(get_time_usec(started)) / 1000000.0
        sleep(250u)
    }
    return -1.0
}

//! the exit code once the host ends, draining its stdout meanwhile; process_running past the deadline
def private wait_exit_draining(var host : process; seconds : float; var lines : array<string>) : int {
    let started = ref_time_ticks()
    while (float(get_time_usec(started)) < seconds * 1000000.0) {
        unsafe(process_drain(host) $(line) {
            lines |> push(line)
        })
        let rc = unsafe(process_poll(host))
        if (rc != process_running) return rc
        sleep(20u)
    }
    return process_running
}

[test]
def test_host_serves_status(t : T?) {
    let exe = live_exe()
    if (empty(exe)) {
        t |> skip("no daslang-live beside {get_das_exe()}")
        return
    }
    let port = free_port()
    if (port < 0) {
        t |> skip("a host answers on every port from {PORT_FIRST} to {PORT_FIRST + PORT_SPAN - 1}")
        return
    }
    t |> run("/status answers within the ready budget, and /shutdown ends the host with 0") @(t : T?) {
        let script = path_join(get_das_root(), "tests/live_host/_fixture_host_ready.das")
        let no_env : array<string>
        var host = unsafe(spawn_process([exe, "--live-port={port}", script], get_das_root(), no_env))
        var lines : array<string>
        let ready = wait_ready(host, port, 30.0, lines)
        t |> success(ready >= 0.0, "daslang-live did not serve /status within 30 s:\n{join(lines, "\n")}")
        if (ready >= 0.0) {
            post_shutdown(port)
            let rc = wait_exit_draining(host, 20.0, lines)
            t |> equal(rc, 0, "the host exited with 0 after /shutdown:\n{join(lines, "\n")}")
        }
        unsafe {
            close_process(host)   //! ends a child that never became ready
        }
    }
}
