options gen2
options no_aot // raw TCP driver requires the interpreter-only network module
options no_unused_function_arguments = false

require dastest/testing_boost public
require ../_dashv_test_common.das
require _wire_peer
require math
require daslib/fio
require daslib/clargs
require ../_server_limits_fixture.das

class FileStreamServer : HvWebServer {
    directory : string
    held : HttpResponseWriter?
    last_admission : int
    def override onInit {
        verify(set_bind_host("127.0.0.1"))
        let temp = temp_directory_result()
        verify(temp is value)
        let made = create_temp_directory_result("{temp as value}/hv-stream-")
        verify(made is value)
        directory = clone_string(made as value)
        fopen("{directory}/large.bin", "wb") $(file) {
            verify(file != null)
            let chunk = repeat("0123456789abcdef", 4096)
            for (_ in range(128)) { fwrite(file, chunk) }
        }
        write_test_file("{directory}/empty.bin", "")
        write_test_file("{directory}/имя.txt", "UTF-8 path")
        STREAM("/file") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            held = writer
            set_header(server, writer, "Content-Type", "application/octet-stream")
            last_admission = SERVE_FILE_STREAM(server, writer, "{directory}/large.bin", 8388608l)
        }
        STREAM("/limit") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            last_admission = SERVE_FILE_STREAM(server, writer, "{directory}/large.bin", 1024l)
        }
        STREAM("/missing") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            last_admission = SERVE_FILE_STREAM(server, writer, "{directory}/missing.bin", 1024l)
        }
        STREAM("/empty") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            last_admission = SERVE_FILE_STREAM(server, writer, "{directory}/empty.bin", 1024l)
        }
        STREAM("/name") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            last_admission = SERVE_FILE_STREAM(server, writer, "{directory}/имя.txt", 1024l)
        }
    }
}

[test(description = "A bounded file writer streams an 8 MB file to a slow peer and keeps the connection alive correctly", timeout = 1200)]
def bounded_file_writer_handles_slow_peer_and_keepalive(t : T?) {
    var server = new FileStreamServer()
    server->init(0); t |> equal(server->start(), 0)
    {
        var peer = new WirePeer()
        t |> success(peer->connect("127.0.0.1", server->bound_port()))
        t |> success(peer->write_text("GET /file HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n"))
        for (_ in range(100)) { server->tick(); sleep(1u) }
        t |> success(pump(server, peer, $ { return peer.disconnected; }))
        let response = peer->text(); let split_at = find(response, "\r\n\r\n")
        t |> equal(server.last_admission, 0)
        t |> success(split_at > 0)
        t |> success(contains(slice(response, 0, max(split_at, 0)), "Content-Length: 8388608"))
        t |> equal(length(response) - split_at - 4, 8388608)
        t |> success(ends_with(response, "0123456789abcdef"))
        t |> success(!is_writer_connected(server.server, server.held), "completed transfer releases admission")
        peer->close(); unsafe { delete peer }
        for (path in ["limit", "missing", "empty"]) {
            peer = new WirePeer()
            t |> success(peer->connect("127.0.0.1", server->bound_port()))
            t |> success(peer->write_text("GET /{path} HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n"))
            t |> success(pump(server, peer, $ { return peer.disconnected; }))
            let status = path == "limit" ? "413" : path == "missing" ? "404" : "200"
            t |> success(contains(peer->text(), "HTTP/1.1 {status}"))
            peer->close(); unsafe { delete peer }
        }
        peer = new WirePeer()
        t |> success(peer->connect("127.0.0.1", server->bound_port()))
        t |> success(peer->write_text("GET /name HTTP/1.1\r\nHost: localhost\r\n\r\nGET /empty HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n"))
        t |> success(pump(server, peer, $ { return peer.disconnected; }))
        t |> success(contains(peer->text(), "UTF-8 pathHTTP/1.1 200"), "next response follows the exact first body")
        peer->close(); unsafe { delete peer }
        peer = new WirePeer()
        t |> success(peer->connect("127.0.0.1", server->bound_port()))
        t |> success(peer->write_text("HEAD /name HTTP/1.1\r\nHost: localhost\r\n\r\nGET /empty HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n"))
        t |> success(pump(server, peer, $ { return peer.disconnected; }))
        t |> success(contains(peer->text(), "Content-Length: 10"), "HEAD describes the GET body")
        t |> success(contains(peer->text(), "\r\n\r\nHTTP/1.1 200"), "HEAD sends no body before the next response")
        t |> success(!contains(peer->text(), "UTF-8 path"), "HEAD must not send file content")
        peer->close(); unsafe { delete peer }
        peer = new WirePeer()
        t |> success(peer->connect("127.0.0.1", server->bound_port()))
        t |> success(peer->write_text("GET /file HTTP/1.1\r\nHost: localhost\r\n\r\n"))
        for (_ in range(30)) { server->tick(); sleep(1u) }
        peer->close(); unsafe { delete peer }
        for (_ in range(100)) { server->tick(); sleep(1u) }
        t |> success(!is_writer_connected(server.server, server.held), "disconnected transfer releases admission")
        peer = new WirePeer()
        t |> success(peer->connect("127.0.0.1", server->bound_port()))
        t |> success(peer->write_text("GET /file HTTP/1.1\r\nHost: localhost\r\n\r\n"))
        for (_ in range(30)) { server->tick(); sleep(1u) }
        server->stop()
        t |> success(!is_writer_connected(server.server, server.held), "server stop releases an active transfer")
        peer->close(); unsafe { delete peer }
    } finally {
        server->stop(); server->cleanup()
        let removed = rmdir_rec_result(server.directory)
        t |> success(removed is value && removed as value)
        unsafe { delete server }
    }
}

[test(description = "Serving a special file such as a fifo does not block server shutdown, proven by a child process", timeout = 1200)]
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
                let port = to_int(fread("{root}/ready"))
                t |> success(port > 0 && port <= 65535, "ready publishes the complete port")
                var peer = new WirePeer()
                t |> success(peer->connect("127.0.0.1", port))
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

class CookieWireServer : LimitedServer {
    raw_cookie, iterated_cookie, other_cookie : string
    cookie_headers : int
    def override onInit {
        GET("/cookies") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            self.raw_cookie = get_header(req, "Cookie")
            self.other_cookie = get_cookie(req, "b")
            each_header(req) $(name, value) {
                if (to_lower(name) == "cookie") { self.cookie_headers++; self.iterated_cookie = clone_string(value) }
            }
            return resp |> TEXT_PLAIN("ok")
        }
    }
}
[test(description = "The raw Cookie header keeps duplicate cookies, joined across header lines, and header iteration lists it once")]
def raw_cookie_header_preserves_duplicates_and_header_iteration_is_not_duplicated(t : T?) {
    var server = new CookieWireServer()
    server.init(0)
    t |> equal(server.start(), 0)
    var peer = new WirePeer()
    t |> success(peer.connect("127.0.0.1", server.bound_port()))
    t |> success(peer.write_text("GET /cookies HTTP/1.1\r\nHost: localhost\r\ncOoKiE: a=one; a=two\r\ncookie: b=three\r\nConnection: close\r\n\r\n"))
    t |> success(pump(server, peer, $ { return peer.disconnected; }))
    t |> equal(server.raw_cookie, "a=one; a=two; b=three")
    t |> equal(server.other_cookie, "three", "convenience cookie lookup still works")
    t |> equal(server.cookie_headers, 1)
    t |> equal(server.iterated_cookie, server.raw_cookie)
    peer.close(); server.stop(); server.cleanup()
    unsafe { delete peer; delete server }
}
