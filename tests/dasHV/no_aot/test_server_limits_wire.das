options gen2
options indenting = 4
options no_aot // raw TCP driver requires daslib/network, which has no AOT adapter
options no_unused_function_arguments = false
require dastest/testing_boost public
require ../_server_limits_fixture.das
require _wire_peer

[test]
def chunked_overflow_rejects_without_running_handler(t : T?) {
    var server = new LimitedServer()
    server->init(0)
    t |> equal(server->start(), 0)
    var peer = new WirePeer()
    t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text("POST /echo HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\nTransfer-Encoding: chunked\r\n\r\n4\r\n1234\r\n4\r\n5678\r\n1\r\n9\r\n0\r\n\r\n"))
    t |> success(pump(server, peer, $ { return peer.disconnected; }))
    t |> success(contains(peer->text(), "413"))
    t |> equal(server.handled, 0)
    peer->close()
    server->stop()
    server->cleanup()
    unsafe { delete peer; delete server }
}

def private check_saturation(t : T?; event_budget, byte_budget : int; body : string) {
    var server = new LimitedServer(event_limit = event_budget, byte_limit = byte_budget, http_limit = length(body))
    server->init(0)
    t |> equal(server->start(), 0)
    var peers : array<WirePeer?>
    peers |> reserve(3)
    for (_ in range(3)) {
        var peer = new WirePeer()
        t |> success(peer->connect("127.0.0.1", server->bound_port()))
        t |> success(peer->write_text("POST /echo HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\nContent-Length: {length(body)}\r\n\r\n{body}"))
        peers |> push(peer)
    }
    var rejected = 0
    // Deliberately do not tick the application: exactly one request can wait for it.
    for (_ in range(3000)) {
        rejected = 0
        for (peer in peers) {
            peer->tick()
            if (contains(peer->text(), "503")) { rejected++ }
        }
        if (rejected == 2) { break }
        sleep(1u)
    }
    t |> equal(rejected, 2)
    t |> equal(server.handled, 0)
    var finished = 0
    for (_ in range(3000)) {
        server->tick()
        finished = 0
        for (peer in peers) { peer->tick(); if (peer.disconnected) { finished++ } }
        if (finished == 3) { break }
        sleep(1u)
    }
    t |> equal(finished, 3)
    t |> equal(server.handled, 1)
    for (peer in peers) { peer->close(); unsafe { delete peer } }
    unsafe { delete peers }
    server->stop()
    server->cleanup()
    unsafe { delete server }
}

[test]
def pending_event_count_closes_admission_until_tick(t : T?) {
    check_saturation(t, 1, 4096, "x")
}

[test]
def pending_payload_bytes_close_admission_until_tick(t : T?) {
    check_saturation(t, 8, 12288, repeat("x", 8192))
}

[test]
def binary_websocket_message_keeps_zero_bytes(t : T?) {
    var server = new LimitedServer()
    server->init(0)
    t |> equal(server->start(), 0)
    var peer = new WirePeer()
    t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text("GET /ws HTTP/1.1\r\nHost: localhost\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nSec-WebSocket-Version: 13\r\n\r\n"))
    t |> success(pump(server, peer, $ { return server.opened == 1 && contains(peer->text(), "\r\n\r\n"); }))
    let before = length(peer.got)
    var packet <- frame(2, true, [uint8(65), uint8(0), uint8(66)])
    t |> success(peer->write_bytes(packet))
    t |> success(pump(server, peer, $ { return length(peer.got) >= before + 5; }))
    t |> equal(server.last_message_size, 3)
    t |> equal(server.handled, 1)
    if (length(peer.got) >= before + 5) {
        for (expected, index in [uint8(0x82), uint8(3), uint8(65), uint8(0), uint8(66)], count()) {
            t |> equal(peer.got[before + index], expected)
        }
    }
    peer->close()
    t |> success(pump(server, peer, $ { return server.closed == 1; }))
    server->stop()
    server->cleanup()
    unsafe { delete peer; delete server }
}

[test]
def fragmented_websocket_overflow_closes_without_delivery(t : T?) {
    var server = new LimitedServer()
    server->init(0)
    t |> equal(server->start(), 0)
    var peer = new WirePeer()
    t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text("GET /ws HTTP/1.1\r\nHost: localhost\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nSec-WebSocket-Version: 13\r\n\r\n"))
    t |> success(pump(server, peer, $ { return server.opened == 1; }))
    var first <- frame(1, false, [uint8(65), uint8(65), uint8(65), uint8(65), uint8(65), uint8(65), uint8(65), uint8(65), uint8(65)])
    var second <- frame(0, true, [uint8(66), uint8(66), uint8(66), uint8(66), uint8(66), uint8(66), uint8(66), uint8(66)])
    t |> success(peer->write_bytes(first))
    t |> success(peer->write_bytes(second))
    t |> success(pump(server, peer, $ { return peer.disconnected && server.closed == 1; }))
    t |> equal(server.handled, 0)
    peer->close()
    server->stop()
    server->cleanup()
    unsafe { delete peer; delete server }
}

[test]
def pipelined_http_replies_keep_request_order_and_bodies(t : T?) {
    var server = new LimitedServer()
    server.event_limit = 1
    server->init(0)
    t |> equal(server->start(), 0)
    var peer = new WirePeer()
    t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text("POST /echo HTTP/1.1\r\nHost: localhost\r\nContent-Length: 1\r\n\r\naPOST /echo HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\nContent-Length: 1\r\n\r\nb"))
    t |> success(pump(server, peer, $ { return peer.disconnected; }))
    let response = peer->text()
    t |> success(starts_with(response, "HTTP/1.1 200"), "the first request owns the first response")
    let first_body = find(response, "\r\n\r\n") + 4
    if (first_body >= 4 && first_body < length(response)) {
        t |> equal(slice(response, first_body, first_body + 1), "a", "first response body; complete wire response: {response}")
        let next = find(response, "HTTP/1.1 ", first_body + 1)
        t |> success(next > first_body)
        if (next > first_body) {
            let second = slice(response, next)
            t |> success(starts_with(second, "HTTP/1.1 200") || starts_with(second, "HTTP/1.1 503"))
            if (starts_with(second, "HTTP/1.1 200")) {
                let second_body = find(second, "\r\n\r\n") + 4
                t |> success(second_body < length(second))
                if (second_body < length(second)) { t |> equal(slice(second, second_body, second_body + 1), "b") }
            }
        }
    } else { t |> failure("first response body is missing") }
    peer->close(); server->stop(); server->cleanup()
    unsafe { delete peer; delete server }
}

[test]
def pipelined_requests_resume_after_each_response(t : T?) {
    var server = new LimitedServer()
    server->init(0)
    t |> equal(server->start(), 0)
    var peer = new WirePeer()
    t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text("POST /echo HTTP/1.1\r\nHost: localhost\r\nContent-Length: 1\r\n\r\naPOST /echo HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\nContent-Length: 1\r\n\r\nb"))
    t |> success(pump(server, peer, $ { return peer.disconnected; }))
    let replies <- split(peer->text(), "HTTP/1.1 ")
    t |> equal(length(replies), 3)
    if (length(replies) == 3) {
        for (reply, body in [replies[1], replies[2]], ["a", "b"]) {
            t |> success(starts_with(reply, "200"))
            let offset = find(reply, "\r\n\r\n") + 4
            t |> success(offset >= 4)
            t |> equal(slice(reply, offset), body)
        }
    }
    t |> equal(server.handled, 2)
    peer->close(); server->stop(); server->cleanup()
    unsafe { delete peer; delete server }
}

[test]
def pending_http_pipeline_is_bounded_before_dispatch(t : T?) {
    var server = new LimitedServer()
    server->init(0)
    t |> equal(server->start(), 0)
    var peer = new WirePeer()
    t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text("POST /echo HTTP/1.1\r\nHost: localhost\r\nContent-Length: 1\r\n\r\na"))
    t |> success(peer->write_text(repeat("x", 70000)))
    for (_ in range(3000)) {
        peer->tick()
        if (peer.disconnected) { break }
        sleep(1u)
    }
    t |> success(peer.disconnected, "excess pipeline bytes close a pending connection")
    t |> equal(server.handled, 0)
    server->tick()
    peer->close(); server->stop(); server->cleanup()
    unsafe { delete peer; delete server }
}
