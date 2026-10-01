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
    if (!empty(mode)) { arguments |> push(mode) }
    with_process(arguments) $(var process) {
        let started = ref_time_ticks()
        var diagnostics : string
        while (!fexist("{root}/ready") && get_time_usec(started) < 15000000) {
            unsafe(process_drain(process) $(line) { diagnostics += line })
            if (unsafe(process_poll(process)) != process_running) { break }
            sleep(5u)
        }
        if (!fexist("{root}/ready")) {
            unsafe(process_drain(process) $(line) { diagnostics += line })
            let status = unsafe(process_poll(process))
            t |> failure("TLS fixture did not start: {executable}; exit/status={status}; {diagnostics}")
        } else {
            let ports <- split(fread("{root}/ready"), "\n")
            if (length(ports) >= 2) { invoke(run, root, to_int(ports[0]), to_int(ports[1])) }
            else { t |> failure("invalid TLS fixture ready record") }
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
