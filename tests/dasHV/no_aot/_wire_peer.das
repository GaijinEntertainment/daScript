options gen2
options indenting = 4
options no_aot // raw TCP fixtures require the interpreter-only network module
options no_unused_function_arguments = false
module _wire_peer shared public
require daslib/network public
require daslib/fio public
require daslib/strings_boost public
require strings public

class WirePeer : Client {
    got : array<uint8>
    disconnected : bool
    def WirePeer() { super(); make_client_adapter() }
    def override onConnect { pass }
    def override onDisconnect { disconnected = true }
    def override onError(msg : string; code : int) { pass }
    def override onLog(msg : string) { pass }
    def override onData(buf : uint8?; size : int) {
        for (index in range(size)) { got |> push(unsafe(buf[index])) }
    }
    def text() : string { return string(got) }
    def write_text(text : string) : bool {
        return send(unsafe(reinterpret<uint8?>(text)), length(text))
    }
    def write_bytes(var bytes : array<uint8>) : bool {
        if (empty(bytes)) { return true }
        return send(unsafe(addr(bytes[0])), length(bytes))
    }
}

def pump(var server : auto; var peer : WirePeer?; ready : block<() : bool>) : bool {
    for (_ in range(3000)) {
        server->tick()
        peer->tick()
        if (invoke(ready)) { return true }
        sleep(1u)
    }
    return false
}

def frame(opcode : int; final_frame : bool; payload : array<uint8>) : array<uint8> {
    var result <- [uint8(opcode | (final_frame ? 128 : 0)), uint8(128 | length(payload)), uint8(0), uint8(0), uint8(0), uint8(0)]
    for (byte in payload) { result |> push(byte) }
    return <- result
}
