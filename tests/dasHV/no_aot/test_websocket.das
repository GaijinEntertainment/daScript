options gen2
options no_aot // raw TCP fixtures require daslib/network's interpreter adapter
options no_unused_function_arguments = false

require dastest/testing_boost public
require dashv/dashv_boost
require _wire_peer

class AdmissionServer : HvWebServer {
    opened, decisions : int
    hold_decision : bool = true
    deadline_ms : int = 1000
    event_budget : int = 16
    pending : WebSocketAdmission = default<WebSocketAdmission>
    def override onInit {
        verify(set_limits(server, 1024, 1024, event_budget, 32768))
        GET("/ping") <| @(var req : HttpRequest?; var response : HttpResponse?) : http_status {
            return response |> TEXT_PLAIN("pong")
        }
        verify(WEBSOCKET_UPGRADE(deadline_ms) <| @(var req : HttpRequest?#; admission : WebSocketAdmission) {
            self.decisions++
            if (get_header(req, "Origin") != "https://allowed.example" || get_cookie(req, "sid") != "fixture") {
                reject_websocket(admission, 403)
            } elif (self.hold_decision) {
                self.pending = admission
            } else {
                accept_websocket(admission, "test.v1")
            }
        })
    }
    def override onWsOpen(channel : WebSocketChannel; url : string#) { opened++ }
}

def private upgrade_request(origin : string = "https://allowed.example") : string =>
    "GET /socket HTTP/1.1\r\nHost: localhost\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nOrigin: {origin}\r\nCookie: sid=fixture\r\nSec-WebSocket-Protocol: test.v1\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nSec-WebSocket-Version: 13\r\n\r\n"

[test(description = "A WebSocket upgrade waits for application approval without blocking plain HTTP requests")]
def approval_precedes_upgrade_without_blocking_http(t : T?) {
    var server = new AdmissionServer()
    server->init(0)
    t |> equal(server->start(), 0)
    var ws = new WirePeer()
    t |> success(ws->connect("127.0.0.1", server->bound_port()))
    t |> success(ws->write_text(upgrade_request()))
    t |> success(pump(server, ws, $ { return server.decisions == 1; }))
    t |> equal(server.opened, 0)
    t |> equal(length(ws.got), 0, "no 101 before the application approves")
    var http = new WirePeer()
    t |> success(http->connect("127.0.0.1", server->bound_port()))
    t |> success(http->write_text("GET /ping HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n"))
    t |> success(pump(server, http, $ { return http.disconnected; }))
    t |> success(contains(http->text(), "pong"))
    t |> equal(server.opened, 0)
    t |> equal(accept_websocket(server.pending, "test.v1"), 0)
    t |> success(pump(server, ws, $ { return server.opened == 1 && contains(ws->text(), "101"); }))
    t |> success(contains(ws->text(), "test.v1"))
    ws->close(); http->close()
    server->stop(); server->cleanup()
    unsafe { delete ws; delete http; delete server }
}

[test(description = "A denied origin gets 403 and never becomes a WebSocket")]
def denied_origin_never_becomes_a_websocket(t : T?) {
    var server = new AdmissionServer()
    server->init(0)
    t |> equal(server->start(), 0)
    var peer = new WirePeer()
    t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text(upgrade_request("https://elsewhere.example")))
    t |> success(pump(server, peer, $ { return peer.disconnected; }))
    t |> success(contains(peer->text(), "403"))
    t |> success(!contains(peer->text(), "101"))
    t |> equal(server.opened, 0)
    peer->close(); server->stop(); server->cleanup()
    unsafe { delete peer; delete server }
}

def private with_pending(t : T?; deadline_ms : int; run : block<(var server : AdmissionServer?; var peer : WirePeer?) : void>) {
    var server = new AdmissionServer()
    server.deadline_ms = deadline_ms
    server->init(0)
    t |> equal(server->start(), 0)
    var peer = new WirePeer()
    t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text(upgrade_request()))
    t |> success(pump(server, peer, $ { return server.decisions == 1; }))
    invoke(run, server, peer)
    peer->close(); server->stop(); server->cleanup()
    unsafe { delete peer; delete server }
}

[test(description = "An admission that times out answers 408 and its ticket can no longer be used")]
def admission_timeout_invalidates_late_replies(t : T?) {
    with_pending(t, 50) $(var server, peer) {
        let old = server.pending
        t |> success(is_alive(old))
        t |> success(pump(server, peer, $ { return peer.disconnected; }))
        t |> success(contains(peer->text(), "408"))
        t |> success(!is_alive(old))
        t |> equal(accept_websocket(old, "test.v1"), -1)
        t |> equal(server.opened, 0)
    }
}

[test(description = "A ticket whose peer disconnected cannot approve a later handshake")]
def disconnected_ticket_cannot_approve_a_new_handshake(t : T?) {
    with_pending(t, 1000) $(var server, peer) {
        let old = server.pending
        peer->close()
        t |> success(pump(server, peer, $ { return !is_alive(old); }))
        var next_peer = new WirePeer()
        t |> success(next_peer->connect("127.0.0.1", server->bound_port()))
        t |> success(next_peer->write_text(upgrade_request()))
        t |> success(pump(server, next_peer, $ { return server.decisions == 2; }))
        t |> success(server.pending != old)
        t |> equal(accept_websocket(old, "test.v1"), -1)
        t |> equal(server.opened, 0)
        t |> equal(accept_websocket(server.pending, "test.v1"), 0)
        t |> success(pump(server, next_peer, $ { return server.opened == 1; }))
        next_peer->close()
        unsafe { delete next_peer }
    }
}

[test(description = "An admission ticket accepts exactly one decision")]
def admission_decision_is_single_use(t : T?) {
    with_pending(t, 1000) $(var server, peer) {
        let ticket = server.pending
        t |> equal(accept_websocket(ticket, "test.v1"), 0)
        t |> equal(reject_websocket(ticket, 403), -1)
        t |> equal(accept_websocket(ticket, "test.v1"), -1)
        t |> success(!is_alive(ticket))
        t |> success(pump(server, peer, $ { return server.opened == 1 && contains(peer->text(), "101"); }))
    }
}

[test(description = "Accepting with a subprotocol the client did not offer answers 400 and opens nothing")]
def unoffered_protocol_is_not_selected(t : T?) {
    with_pending(t, 1000) $(var server, peer) {
        t |> equal(accept_websocket(server.pending, "not.offered"), 0)
        t |> success(pump(server, peer, $ { return peer.disconnected; }))
        t |> success(contains(peer->text(), "400"))
        t |> success(!contains(peer->text(), "101"))
        t |> equal(server.opened, 0)
    }
}

[test(description = "Server shutdown invalidates a pending admission ticket")]
def shutdown_invalidates_pending_admission(t : T?) {
    with_pending(t, 1000) $(var server, peer) {
        let ticket = server.pending
        server->stop()
        t |> success(pump(server, peer, $ { return peer.disconnected; }))
        t |> success(!is_alive(ticket))
        t |> equal(accept_websocket(ticket, "test.v1"), -1)
        t |> equal(server.opened, 0)
    }
}

[test(description = "A malformed handshake is rejected with 400 before the authorizer sees it")]
def malformed_handshake_is_rejected_before_authorizer(t : T?) {
    var server = new AdmissionServer()
    server->init(0)
    t |> equal(server->start(), 0)
    var peer = new WirePeer()
    t |> success(peer->connect("127.0.0.1", server->bound_port()))
    let request = replace(upgrade_request(), "dGhlIHNhbXBsZSBub25jZQ==", "invalid")
    t |> success(peer->write_text(request))
    t |> success(pump(server, peer, $ { return peer.disconnected; }))
    t |> success(contains(peer->text(), "400"))
    t |> equal(server.decisions, 0)
    t |> equal(server.opened, 0)
    peer->close(); server->stop(); server->cleanup()
    unsafe { delete peer; delete server }
}

[test(description = "Outstanding admission decisions keep their reserved capacity after their callbacks return")]
def outstanding_decisions_keep_reserved_capacity_after_callbacks(t : T?) {
    var server = new AdmissionServer(event_budget = 2, deadline_ms = 5000)
    server->init(0)
    t |> equal(server->start(), 0)
    var peers : array<WirePeer?>
    peers |> reserve(3)
    for (index in range(3)) {
        var peer = new WirePeer()
        t |> success(peer->connect("127.0.0.1", server->bound_port()))
        t |> success(peer->write_text(upgrade_request()))
        if (index < 2) {
            t |> success(pump(server, peer, $ { return server.decisions == index + 1; }))
        } else {
            t |> success(pump(server, peer, $ { return peer.disconnected; }))
            t |> success(contains(peer->text(), "503"))
        }
        peers |> push(peer)
    }
    t |> equal(server.decisions, 2)
    t |> equal(server.opened, 0)
    for (peer in peers) { peer->close(); unsafe { delete peer } }
    unsafe { delete peers }
    server->stop(); server->cleanup()
    unsafe { delete server }
}

[test(description = "Invalid decision arguments fail and leave the ticket available for a valid decision")]
def invalid_decision_arguments_leave_the_ticket_available(t : T?) {
    with_pending(t, 5000) $(var server, peer) {
        for (status in [399, 600]) {
            t |> success(reject_websocket(server.pending, status) != 0)
            t |> success(is_alive(server.pending))
        }
        for (protocol in ["test.v1\r\nX-Injected: yes", repeat("a", 129)]) {
            t |> success(accept_websocket(server.pending, protocol) != 0)
            t |> success(is_alive(server.pending))
        }
        t |> equal(accept_websocket(server.pending, "test.v1"), 0)
        t |> success(pump(server, peer, $ { return server.opened == 1; }))
    }
}

class ConnectionLimitServer : HvWebServer {
    opened, messages, closed : int
    per_messages : int = 2
    per_bytes : int = 64
    pressure : bool
    send_failed : bool
    def override onInit {
        verify(set_limits(server, 1024, 1024, 64, 65536))
        verify(set_connection_limits(server, per_messages, per_bytes, 512))
    }
    def override onWsOpen(channel : WebSocketChannel; url : string#) { opened++ }
    def override onWsClose(channel : WebSocketChannel) { closed++ }
    def override onWsMessageFrame(channel : WebSocketChannel; msg : string#; size : int; opcode : ws_opcode) {
        messages++
        if (pressure) {
            let payload = repeat("x", 65536)
            for (_ in range(256)) {
                if (send(channel, payload, ws_opcode.WS_OPCODE_BINARY, true) < 0) { send_failed = true; break }
            }
            return
        }
        send(channel, msg, size, opcode, true)
    }
}
def private handshake : string =>
    "GET /ws HTTP/1.1\r\nHost: localhost\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nSec-WebSocket-Version: 13\r\n\r\n"
def private check_connection_budget(t : T?; count, bytes : int) {
    var server = new ConnectionLimitServer(per_messages = count, per_bytes = bytes)
    server->init(0); t |> success(server->set_bind_host("127.0.0.1")); t |> equal(server->start(), 0)
    var flood = new WirePeer(); var other = new WirePeer()
    t |> success(flood->connect("127.0.0.1", server->bound_port()))
    t |> success(other->connect("127.0.0.1", server->bound_port()))
    t |> success(flood->write_text(handshake())); t |> success(other->write_text(handshake()))
    t |> success(pump(server, flood, $ { other->tick(); return server.opened == 2; }))
    var packet <- frame(1, true, [uint8(65), uint8(66), uint8(67)])
    var burst : array<uint8>; reserve(burst, 9 * 8)
    for (_ in range(8)) { burst |> push_from(packet) }
    t |> success(flood->write_bytes(burst))
    for (_ in range(3000)) { flood->tick(); if (flood.disconnected) { break }; sleep(1u) }
    t |> success(flood.disconnected, "one peer cannot fill the global queue")
    let before = length(other.got)
    t |> success(other->write_bytes(packet))
    t |> success(pump(server, other, $ { return length(other.got) >= before + 5; }))
    t |> success(!other.disconnected, "another connection still receives its echo")
    t |> success(server.messages <= 3, "only bounded messages reach the owner")
    flood->close(); other->close(); server->stop(); server->cleanup()
    unsafe { delete flood; delete other; delete server }
}
[test(description = "A connection past its message-count budget is cut without disturbing other peers")]
def per_connection_message_count_preserves_other_peers(t : T?) { check_connection_budget(t, 2, 64) }
[test(description = "A connection past its payload-byte budget is cut without disturbing other peers")]
def per_connection_payload_budget_preserves_other_peers(t : T?) { check_connection_budget(t, 8, 6) }

[test(description = "A slow reader past the write-buffer budget is disconnected and limits are immutable while running")]
def per_connection_write_buffer_rejects_slow_reader(t : T?) {
    var server = new ConnectionLimitServer(pressure = true)
    server->init(0)
    t |> success(!set_connection_limits(server.server, 0, 64, 512))
    t |> success(server->set_bind_host("127.0.0.1")); t |> equal(server->start(), 0)
    t |> success(!set_connection_limits(server.server, 2, 64, 512), "configuration is immutable while running")
    var peer = new WirePeer(); t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text(handshake()))
    t |> success(pump(server, peer, $ { return server.opened == 1; }))
    var packet <- frame(1, true, [uint8(65)])
    t |> success(peer->write_bytes(packet))
    for (_ in range(3000)) { server->tick(); if (server.messages == 1) { break }; sleep(1u) }
    t |> success(server.send_failed, "bounded native output refuses queued data")
    peer->close(); server->stop(); server->cleanup()
    unsafe { delete peer; delete server }
}

[test(description = "Draining messages gives a connection its message and byte capacity back")]
def draining_messages_replenishes_connection_capacity(t : T?) {
    var server = new ConnectionLimitServer(per_messages = 2, per_bytes = 6)
    server->init(0); t |> success(server->set_bind_host("127.0.0.1")); t |> equal(server->start(), 0)
    var peer = new WirePeer(); t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text(handshake()))
    t |> success(pump(server, peer, $ { return server.opened == 1 && contains(peer->text(), "\r\n\r\n"); }))
    let packet <- frame(1, true, [uint8(65), uint8(66), uint8(67)])
    var batch : array<uint8>; reserve(batch, 18); batch |> push_from(packet, packet)
    for (round in range(4)) {
        let before = length(peer.got)
        t |> success(peer->write_bytes(batch))
        t |> success(pump(server, peer, $ { return length(peer.got) >= before + 10; }))
        t |> equal(server.messages, (round + 1) * 2)
        t |> success(!peer.disconnected)
    }
    peer->close(); server->stop(); server->cleanup()
    unsafe { delete peer; delete server }
}

class RequestServer : HvWebServer {
    cookies, paths, urls, origins : array<string>
    legacy_opens : int
    def override onInit {
        verify(set_limits(server, 1024, 1024, 16, 32768))
        verify(WEBSOCKET_UPGRADE(1000) @(var request : HttpRequest?#; admission : WebSocketAdmission) {
            let cookie = get_cookie(request, "sid")
            if (cookie == "alpha" || cookie == "bravo") { accept_websocket(admission, "test.v1") }
            else { reject_websocket(admission, 403) }
        })
    }
    def override onWsOpen(channel : WebSocketChannel; url : string#) { legacy_opens++ }
    def override onWsOpenRequest(channel : WebSocketChannel; var request : HttpRequest?#) {
        cookies |> push(clone(get_cookie(request, "sid")))
        paths |> push(clone(string(request.path)))
        urls |> push(clone(string(request.url)))
        origins |> push(clone(get_header(request, "Origin")))
    }
}
def private handshake(cookie, path : string) : string =>
    "GET {path} HTTP/1.1\r\nHost: localhost\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nOrigin: https://allowed.example\r\nCookie: sid={cookie}\r\nSec-WebSocket-Protocol: test.v1\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nSec-WebSocket-Version: 13\r\n\r\n"
[test(description = "An opened request snapshot belongs to its own channel and rejected requests never open")]
def opened_request_belongs_to_its_channel_and_rejected_requests_never_open(t : T?) {
    var server = new RequestServer(); server->init(0)
    t |> success(server->set_bind_host("127.0.0.1"))
    t |> equal(server->start(), 0)
    var first = new WirePeer(); var second = new WirePeer()
    t |> success(first->connect("127.0.0.1", server->bound_port()))
    t |> success(second->connect("127.0.0.1", server->bound_port()))
    t |> success(first->write_text(handshake("alpha", "/first")))
    t |> success(second->write_text(handshake("bravo", "/second")))
    t |> success(pump(server, first, $ { second->tick(); return length(server.cookies) == 2; }))
    first->close(); second->close()
    t |> equal(server.legacy_opens, 0, "request override replaces the legacy callback")
    if (length(server.cookies) == 2) {
        for (cookie, path, url, origin in server.cookies, server.paths, server.urls, server.origins) {
            t |> success(cookie == "alpha" || cookie == "bravo")
            t |> equal(path, cookie == "alpha" ? "/first" : "/second")
            t |> equal(url, "http://localhost{path}")
            t |> equal(origin, "https://allowed.example")
        }
        t |> success(server.cookies[0] != server.cookies[1])
    }
    var denied = new WirePeer()
    t |> success(denied->connect("127.0.0.1", server->bound_port()))
    t |> success(denied->write_text(handshake("denied", "/denied")))
    t |> success(pump(server, denied, $ { return denied.disconnected; }))
    t |> success(contains(denied->text(), "403")); t |> equal(length(server.cookies), 2)
    denied->close(); server->stop(); server->cleanup()
    unsafe { delete first; delete second; delete denied; delete server }
}

class BudgetRequestServer : RequestServer {
    byte_budget : int = 32768
    def override onInit { verify(set_limits(server, 1024, 1024, 16, byte_budget)) }
}
[test(description = "A request snapshot over the pending-byte budget is refused and never opens")]
def request_snapshot_respects_pending_bytes(t : T?) {
    var server = new BudgetRequestServer(); server.byte_budget = 64; server->init(0)
    t |> success(server->set_bind_host("127.0.0.1")); t |> equal(server->start(), 0)
    var peer = new WirePeer(); t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text(handshake("alpha", "/bounded")))
    t |> success(pump(server, peer, $ { return peer.disconnected; }))
    t |> equal(length(server.cookies), 0); t |> equal(server.legacy_opens, 0)
    peer->close(); server->stop(); server->cleanup()
    unsafe { delete peer; delete server }
}
[test(description = "cleanup discards open snapshots that were never drained")]
def cleanup_discards_undrained_open_snapshots(t : T?) {
    var server = new BudgetRequestServer(); server->init(0)
    t |> success(server->set_bind_host("127.0.0.1")); t |> equal(server->start(), 0)
    var peer = new WirePeer(); t |> success(peer->connect("127.0.0.1", server->bound_port()))
    t |> success(peer->write_text(handshake("alpha", "/undrained")))
    for (_ in range(3000)) { peer->tick(); if (contains(peer->text(), "101")) { break }; sleep(1u) }
    t |> success(contains(peer->text(), "101"))
    t |> equal(length(server.cookies), 0)
    peer->close(); server->stop(); server->cleanup()
    t |> equal(length(server.cookies), 0); t |> equal(server.legacy_opens, 0)
    unsafe { delete peer; delete server }
}
