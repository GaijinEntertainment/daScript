options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require daslib/network
require strings

//! the TCP client end of the network module against the server end in the same process: a
//! connect, bytes both ways, the server's close seen by the client, and the port probe

class private Echo : Server {
    got : string
    connects : int = 0
    def Echo() {
        super()
        make_server_adapter()
    }
    def override onConnect() {
        connects++
    }
    def override onDisconnect() {
        pass
    }
    def override onData(buf : uint8?; size : int) {
        for (i in range(size)) {
            got = "{got}{to_char(int(unsafe(buf[i])))}"
        }
        send(buf, size)
    }
    def override onError(msg : string; code : int) {
        pass
    }
    def override onLog(msg : string) {
        pass
    }
}

class private Peer : Client {
    got : string
    disconnects : int = 0
    def Peer() {
        super()
        make_client_adapter()
    }
    def override onConnect() {
        pass
    }
    def override onDisconnect() {
        disconnects++
    }
    def override onData(buf : uint8?; size : int) {
        for (i in range(size)) {
            got = "{got}{to_char(int(unsafe(buf[i])))}"
        }
    }
    def override onError(msg : string; code : int) {
        pass
    }
    def override onLog(msg : string) {
        pass
    }
}

def private pump_until(var server : Echo?; var client : Peer?; pred : block<() : bool>) : bool {
    for (_i in range(2000)) {
        server->tick()
        client->tick()
        return true if (invoke(pred))
        sleep(1u)
    }
    return false
}

[test]
def test_client_round_trip(t : T?) {
    t |> run("a client connects, echoes through the server, and sees the server go away") @(t : T?) {
        let port = probe_local_port("127.0.0.1", 0)
        t |> success(port > 0, "the probe found a free port: {port}")
        var server = new Echo()
        t |> success(server->init(port), "the server listens")
        var client = new Peer()
        t |> success(client->connect("127.0.0.1", port), "the client connects")
        t |> success(pump_until(server, client, $ {
            return server.connects == 1
        }), "the server accepted")
        let hello = "hello over loopback"
        t |> success(client->send(unsafe(reinterpret<uint8?>(hello)), length(hello)), "the client sends")
        t |> success(pump_until(server, client, $ {
            return client.got == hello
        }), "the echo came back: '{client.got}'")
        t |> equal(server.got, hello)
        unsafe {
            delete server
        }
        var saw_close = false
        for (_i in range(2000)) {
            client->tick()
            saw_close = client.disconnects == 1
            break if (saw_close)
            sleep(1u)
        }
        t |> success(saw_close, "the client saw the server close")
        t |> success(!client->is_connected(), "and is no longer connected")
        unsafe {
            delete client
        }
    }
    t |> run("a refused port fails the connect and a held port fails the probe") @(t : T?) {
        let port = probe_local_port("127.0.0.1", 0)
        var client = new Peer()
        t |> success(!client->connect("127.0.0.1", port), "nothing listens there")
        t |> success(!client->connect("localhost", port), "the name resolves and nothing listens there either")
        unsafe {
            delete client
        }
        var holder = new Echo()
        t |> success(holder->init(port), "the test holds the port")
        t |> success(probe_local_port("127.0.0.1", port) < 0, "the held port is not available")
        unsafe {
            delete holder
        }
    }
    t |> run("a peer that never answers fails the connect at the timeout") @(t : T?) {
        // 192.0.2.1 is TEST-NET-1: never routed, so a SYN there is dropped or refused, never answered
        var client = new Peer()
        let started = ref_time_ticks()
        t |> success(!client->connect("192.0.2.1", 9, 300), "no answer within the timeout")
        let waited_ms = get_time_usec(started) / 1000
        t |> success(waited_ms < 5000, "the connect gave up at the timeout, not at the OS default: {waited_ms} ms")
        unsafe {
            delete client
        }
    }
}
