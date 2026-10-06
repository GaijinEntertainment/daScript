options gen2
options indenting = 4
options no_aot // raw TCP fixture uses the interpreter network adapter
options no_unused_function_arguments = false
require dastest/testing_boost public
require dashv/dashv_boost
require _wire_peer

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
[test]
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
[test]
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
[test]
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
