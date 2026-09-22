// Shared helpers for dasHV tests
//
// Provides:
//   - with_test_server() lifecycle helper
//   - File I/O helpers for form upload tests

options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

module _dashv_test_common shared public

require dashv/dashv_boost public
require daslib/jobque_boost public
require daslib/fio public

// ─────────────────────────────────────────────────────────────────────────
// Generic server lifecycle
// ─────────────────────────────────────────────────────────────────────────
//
// Starts a HvWebServer subclass on a dedicated thread, waits for the port it
// bound, runs the user's block, then shuts down.

def with_test_server(server_type : auto(ServerType); blk : block<(base_url : string) : void>) {
    with_job_status(1) $(started) {
        with_job_status(1) $(finished) {
            with_atomic32() $(bound) {
            with_atomic32() $(stop_flag) {
            new_thread() @() {
                var server = new ServerType()
                server->init(0)
                var port = -1
                if (server->start() == 0) {
                    port = server->bound_port()
                }
                bound |> set(port)
                started |> notify_and_release
                while (stop_flag |> get == 0) {
                    server->tick()
                    sleep(1u)
                }
                server->stop()
                server->cleanup()
                unsafe {
                    delete server
                }
                finished |> notify_and_release
            }
            started |> join
            let port = bound |> get
            invoke(blk, "http://127.0.0.1:{port}")
            stop_flag |> set(1)
            finished |> join
            }
            }
        }
    }
}

def write_test_file(path, content : string) {
    fopen(path, "w") $(f) {
        if (f != null) {
            fwrite(f, content)
        }
    }
}

def read_test_file(path : string) : string {
    var result = ""
    fopen(path, "r") $(f) {
        if (f != null) {
            result = fread(f)
        }
    }
    return result
}
