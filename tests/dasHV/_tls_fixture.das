options gen2
options indenting = 4
module _tls_fixture shared public
require dastest/testing_boost public
require dashv/dashv_boost public
require daslib/fio public
require daslib/clargs
require daslib/strings_boost public
require strings public

def with_tls_fixture_mode(t : T?; mode : string; run : block<(root : string; dns_port, ip_port : int) : void>) {
    var error : string
    let root = create_temp_directory("dashv_tls", error)
    if (!empty(error)) { t |> failure(error); return }
    let suffix = get_platform_name() == "windows" ? ".exe" : ""
    let executable = "{dir_name(get_host_binary())}/hv_tls_fixture{suffix}"
    var arguments <- [executable, root]
    with_process(arguments) $(var process) {
        let started = ref_time_ticks()
        var diagnostics : string
        var record : string
        while (get_time_usec(started) < 15000000) {
            record = fread("{root}/ready")
            if (!empty(record)) { break }
            unsafe(process_drain(process) $(line) { diagnostics += line })
            if (unsafe(process_poll(process)) != process_running) { break }
            sleep(5u)
        }
        let ports <- split(record, "\n")
        if (length(ports) < 2) {
            unsafe(process_drain(process) $(line) { diagnostics += line })
            let status = unsafe(process_poll(process))
            let what = (empty(record) ? "TLS fixture did not start"
                : "invalid TLS fixture ready record [{record}]")
            t |> failure("{what}: {executable}; exit/status={status}; {diagnostics}")
        } else {
            if (mode == "clients") {
                // Set trust before either CRT initializes; Debug/Release CRT env caches differ.
                with_process([executable, root, "clients"], "", ["SSL_CERT_FILE={root}/ca.pem"]) $(var client) {
                    let client_exit = unsafe(process_wait(client, 15.0))
                    unsafe(process_drain(client) $(line) { diagnostics += "{line}\n" })
                    t |> equal(client_exit, 0, diagnostics)
                }
            }
            invoke(run, root, to_int(ports[0]), to_int(ports[1]))
            fwrite("{root}/release", "done")
            let exit_code = unsafe(process_wait(process, 10.0))
            unsafe(process_drain(process) $(line) { diagnostics += "{line}\n" })
            t |> equal(exit_code, 0, diagnostics)
        }
    }
    let removed = rmdir_rec_result(root)
    t |> success(removed is value && removed as value)
}

def with_tls_fixture(t : T?; run : block<(root : string; dns_port, ip_port : int) : void>) {
    with_tls_fixture_mode(t, "", run)
}
