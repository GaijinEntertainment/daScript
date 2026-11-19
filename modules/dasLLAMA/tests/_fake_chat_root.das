options gen2
options persistent_heap
options gc

require _fake_chat_server
require daslib/clargs
require daslib/fio

// The fake chat server as a process of its own, for the model bench runner's cells, which start a server by its command
// line: `--port` to listen on, `--name` the one id of its model list (a path gives its file name), `--sized` every reply's
// prompt tokens following the request's size, `--exit` to leave before it listens. `--arm-mark` is an arm's flag the
// runner adds; it changes nothing.

[CommandLineArgs]
struct FakeArgs {
    port : int
    name : string
    sized : bool
    @clarg_name = "exit"
    leave : bool
    @clarg_name = "arm-mark"
    arm_mark : bool
}

[export]
def main : int {
    var r <- parse_args(type<FakeArgs>)
    return 2 if (r |> is_err)
    let args <- r |> move_unwrap
    return 0 if (args.leave)
    set_fake_chat(base_name(args.name), args.sized)
    var server = new FakeChatServer()
    server->init(args.port)
    return 1 if (server->start() != 0)
    while (true) {
        server->tick()
        sleep(1u)
    }
    return 0
}
