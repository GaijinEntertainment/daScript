options gen2
options indenting = 4
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
        verify(WEBSOCKET_UPGRADE(deadline_ms) <| @(var req : HttpRequest?; admission : WebSocketAdmission) {
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

def private upgrade_request(origin : string = "https://allowed.example") : string {
    return "GET /socket HTTP/1.1\r\nHost: localhost\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nOrigin: {origin}\r\nCookie: sid=fixture\r\nSec-WebSocket-Protocol: test.v1\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nSec-WebSocket-Version: 13\r\n\r\n"
}

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
def unoffered_protocol_is_not_selected(t : T?) {
    with_pending(t, 1000) $(var server, peer) {
        t |> equal(accept_websocket(server.pending, "not.offered"), 0)
        t |> success(pump(server, peer, $ { return peer.disconnected; }))
        t |> success(contains(peer->text(), "400"))
        t |> success(!contains(peer->text(), "101"))
        t |> equal(server.opened, 0)
    }
}

[test]
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

[test]
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

[test]
def outstanding_decisions_keep_reserved_capacity_after_callbacks(t : T?) {
    var server = new AdmissionServer()
    server.event_budget = 2
    server.deadline_ms = 5000
    server->init(0)
    t |> equal(server->start(), 0)
    var peers : array<WirePeer?>
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
