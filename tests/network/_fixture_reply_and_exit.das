options gen2
options no_aot

//! a server process that answers its first request with REPLY_BYTES
//! bytes, deletes its only socket object and exits: the reply must
//! reach a peer that has not read yet. Argument: the port.

require daslib/fio
require daslib/network
require strings
require daslib/strings_convert   // try_to_int

let REPLY_BYTES = 8192

class private ReplyOnce : Server {
    answered : bool = false
    def ReplyOnce() {
        super()
        make_server_adapter()
    }
    def override onConnect() {
        pass
    }
    def override onDisconnect() {
        pass
    }
    def override onData(buf : uint8?; size : int) {
        return if (answered)
        let reply = repeat("r", REPLY_BYTES)
        send(unsafe(reinterpret<uint8?>(reply)), length(reply))
        answered = true
    }
    def override onError(msg : string; code : int) {
        pass
    }
    def override onLog(msg : string) {
        pass
    }
}

[export]
def main() {
    let args <- get_command_line_arguments()
    let parsed = try_to_int(empty(args) ? "" : args[length(args) - 1])
    if (!is_ok(parsed) || unwrap(parsed) <= 0) {
        print("the last argument is the port to listen on\n")
        unsafe(fio::exit(2))
    }
    let port = unwrap(parsed)
    var server = new ReplyOnce()
    if (!server->init(port)) {
        print("cannot listen on {port}\n")
        unsafe(fio::exit(2))
    }
    print("listening\n")
    for (_i in range(20000)) {
        server->tick()
        break if (server.answered)
        sleep(1u)
    }
    unsafe {
        delete server
    }
}
