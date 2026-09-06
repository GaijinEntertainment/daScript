options gen2
options stack = 65536

// A standalone context on the full runtime: dasHV and fio are C++ modules, linked as static
// archives. The generated context registers them (their handled types resolve through the
// module registry) and the last context destroyed shuts them down - main.cpp constructs the
// context and nothing else.

require dashv/dashv_boost
require daslib/fio

//! HTTP status of `url`, or -1 when nothing answers within two seconds.
[export]
def http_status(url : string) : int {
    var status = -1
    with_http_request() $(var req) {
        req.method = http_method.GET
        req.url := url
        req.timeout = 2
        req.connect_timeout = 2
        request(req) $(resp) {
            if (resp != null) {
                status = int(resp.status_code)
            }
        }
    }
    return status
}

//! Runs `exe arg`, prints every line the child writes, and returns its exit code.
[export]
def run_child(exe, arg : string) : int {
    let code = unsafe(popen_argv([exe, arg], 10.0, $(f) {
        while (!feof(f)) {
            let line = fgets(f)
            if (!empty(line)) {
                print("child: {line}")
            }
        }
    }))
    return code
}
