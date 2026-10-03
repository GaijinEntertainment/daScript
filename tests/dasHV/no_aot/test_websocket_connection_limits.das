options gen2
options indenting = 4
options no_aot // raw TCP fixture uses the interpreter-only network adapter
options no_unused_function_arguments = false
require dastest/testing_boost public
require dashv/dashv_boost
require _wire_peer

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
def private handshake : string {
    return "GET /ws HTTP/1.1\r\nHost: localhost\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nSec-WebSocket-Version: 13\r\n\r\n"
}
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
[test]
def per_connection_message_count_preserves_other_peers(t : T?) { check_connection_budget(t, 2, 64) }
[test]
def per_connection_payload_budget_preserves_other_peers(t : T?) { check_connection_budget(t, 8, 6) }

[test]
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

[test]
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
