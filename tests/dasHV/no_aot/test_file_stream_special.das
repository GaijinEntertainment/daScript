options gen2
options no_aot // raw TCP fixture uses the interpreter-only network adapter
require dastest/testing_boost public
require daslib/fio
require daslib/clargs
require _wire_peer

[test]
def special_file_response_does_not_block_shutdown(t : T?) {
    let made = create_temp_directory_result("hv-special-")
    t |> success(made is value)
    if (!(made is value)) { return }
    let root = made as value
    {
        if (get_platform_name() != "windows") {
            with_process(["mkfifo", "{root}/special"]) $(var maker) {
                t |> equal(unsafe(process_wait(maker, 5.0)), 0)
            }
        }
        let fixture = "{get_das_root()}/tests/dasHV/no_aot/_file_stream_child.das"
        with_process([get_host_binary(), "-dasroot", get_das_root(), fixture, "--", root]) $(var child) {
            let started = ref_time_ticks()
            while (!fexist("{root}/ready") && get_time_usec(started) < 15000000) {
                unsafe(process_drain(child) $(_line) {})
                if (unsafe(process_poll(child)) != process_running) { break }
                sleep(5u)
            }
            t |> success(fexist("{root}/ready"), "child server started")
            if (fexist("{root}/ready")) {
                var peer = new WirePeer()
                t |> success(peer->connect("127.0.0.1", to_int(fread("{root}/ready"))))
                t |> success(peer->write_text("GET /special HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n"))
                let waiting = ref_time_ticks()
                while (!peer.disconnected && get_time_usec(waiting) < 3000000) { peer->tick(); sleep(1u) }
                t |> success(contains(peer->text(), "HTTP/1.1 404"), "special file is rejected without waiting for a producer")
                peer->close(); unsafe { delete peer }
                verify(fwrite("{root}/release", "done"))
                t |> equal(unsafe(process_wait(child, 3.0)), 0, "server stops without a blocked file open")
            }
        }
    } finally {
        let removed = rmdir_rec_result(root)
        t |> success(removed is value && removed as value)
    }
}
