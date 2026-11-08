options gen2
options no_aot

require daslib/clargs
require daslib/fio
require dashv/dashv_boost

// The supervised child for test_watchdog.das. `state_dir mode`: a run counter in state_dir
// tells the child which launch it is, and `mode` picks the story it acts out:
//   crash-then-ok    run 1 prints the ready line and exits 9; run 2 prints it and exits 0
//   tune-then-ok     run 1 prints the tune marker and exits 3; run 2 exits 0
//   config-restart   run 1 exits 4; run 2 exits 0
//   tune-abort       run 1 exits 3 with no restart marker; run 2 exits 0
//   park             prints a line, waits for the stop file named by CADMUS_STOP_FILE (or, with
//                    none named, to be terminated), exits 0
//   chatty           prints twenty thousand lines and exits 0 - what the supervisor's heaps see
//                    from a talkative child
//   serve-then-stall run 1 serves GET /health on the port in the
//                    third argument until the first request is
//                    answered, then stops pumping its loop and parks
//                    with the port still bound, until terminated;
//                    run 2 exits 0
//   serve-then-exit  serves GET /health on the port in the third
//                    argument for three seconds, then exits 0
//   serve-blip-serve serves GET /health on the port in the third
//                    argument until the first request is answered,
//                    stops pumping for 4.5 seconds, serves 4 more,
//                    then exits 0: one failed poll amid healthy ones
// Codes return from main: a das exit() is an abnormal termination and reports 1.
var g_health_served = false

class StallServer : HvWebServer {
    def override onInit {
        GET("/health") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            g_health_served = true
            return resp |> TEXT_PLAIN("ok")
        }
    }
}

//! the health server on 127.0.0.1:port, null when the bind failed
def start_health_server(port : int) : StallServer? {
    var server = new StallServer()
    server->init(port)
    server->set_bind_host("127.0.0.1")
    if (server->start() != 0) {
        unsafe {
            delete server
        }
        return null
    }
    print("listening on http://127.0.0.1:{server->bound_port()}/\n")
    return server
}

def serve_then_exit(port : int) : int {
    var server = start_health_server(port)
    if (server == null) return 8
    let started = ref_time_ticks()
    while (get_time_usec(started) < 3000000) {
        server->tick()
        sleep(5u)
    }
    server->stop()
    unsafe {
        delete server
    }
    return 0
}

def serve_blip_serve(port : int) : int {
    var server = start_health_server(port)
    if (server == null) return 8
    let started = ref_time_ticks()
    while (!g_health_served) {
        if (get_time_usec(started) > 30000000) return 6
        server->tick()
        sleep(5u)
    }
    sleep(4500u)   // past the poller's 2 s request timeout: one poll fails
    let resumed = ref_time_ticks()
    while (get_time_usec(resumed) < 4000000) {
        server->tick()
        sleep(5u)
    }
    server->stop()
    unsafe {
        delete server
    }
    return 0
}

//! the exit code of a health-serving mode, -1 when `mode` is not one
def health_mode(mode : string; port, run : int) : int {
    if (mode == "serve-then-stall") return run == 1 ? serve_then_stall(port) : 0
    if (mode == "serve-then-exit") return serve_then_exit(port)
    if (mode == "serve-blip-serve") return serve_blip_serve(port)
    return -1
}

def serve_then_stall(port : int) : int {
    var server = start_health_server(port)
    if (server == null) return 8
    let started = ref_time_ticks()
    while (!g_health_served) {
        if (get_time_usec(started) > 30000000) return 6
        server->tick()
        sleep(5u)
    }
    // the loop is no longer pumped: the socket stays bound, every
    // request now hangs, and the supervisor's unhealthy timeout is the
    // only way out before the guard
    let stalled = ref_time_ticks()
    while (get_time_usec(stalled) < 60000000) {
        sleep(50u)
    }
    return 6
}
def run_number(state_dir : string) : int {
    let path = path_join(state_dir, "runs")
    var runs = 0
    if (fexist(path)) {
        fopen(path, "rb") $(f) {
            if (f != null) {
                runs = to_int(fread(f))
            }
        }
    }
    runs++
    fwrite(path, "{runs}")
    return runs
}

[export]
def main() : int {
    let args <- get_cli_arguments()
    if (length(args) < 2) return 2
    let run = run_number(args[0])
    let mode = args[1]
    if (mode == "crash-then-ok") {
        print("[I] LLVM JIT: DLL cache hit .cache/daslang/sub/fake.dll\n")
        print("listening on http://127.0.0.1:1/\n")
        return run == 1 ? 9 : 0
    }
    if (mode == "tune-then-ok") {
        if (run == 1) {
            print("llvm_tune: tuning scope 'fixture'\n")
            print("llvm_tune: restart to apply the winners\n")
            return 3
        }
        return 0
    }
    if (mode == "config-restart") {
        return run == 1 ? 4 : 0
    }
    if (mode == "tune-abort") {
        return run == 1 ? 3 : 0
    }
    if (mode == "chatty") {
        for (i in range(20000)) {
            print("line {i} of a child that never stops talking about itself\n")
        }
        return 0
    }
    let port = length(args) > 2 ? to_int(args[2]) : 0
    let served = health_mode(mode, port, run)
    if (served >= 0) return served
    if (mode == "park") {
        print("parked\n")
        // parks until the stop file the supervisor named appears; with none named it parks until
        // terminated (the deadlock guard is the only other way out)
        let stop_file = has_env_variable("CADMUS_STOP_FILE") ? get_env_variable("CADMUS_STOP_FILE") : ""
        let started = ref_time_ticks()
        while (empty(stop_file) || !fexist(stop_file)) {
            if (get_time_usec(started) > 30000000) return 6
            sleep(5u)
        }
        return 0
    }
    return 7
}
