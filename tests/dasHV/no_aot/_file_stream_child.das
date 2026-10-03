options gen2
options no_unused_function_arguments = false
require dashv/dashv_boost
require daslib/fio
require daslib/clargs

class SpecialFileServer : HvWebServer {
    path : string
    def override onInit {
        verify(set_bind_host("127.0.0.1"))
        STREAM("/special") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            verify(SERVE_FILE_STREAM(server, writer, path, 1024l) == 0)
        }
    }
}

[export]
def main() : int {
    let args <- get_cli_arguments()
    if (length(args) != 1) { return 2 }
    let root = args[0]
    var server = new SpecialFileServer()
    server.path = clone_string(get_platform_name() == "windows" ? "NUL" : "{root}/special")
    server->init(0)
    if (server->start() != 0) { return 3 }
    verify(fwrite("{root}/ready", "{server->bound_port()}"))
    while (!fexist("{root}/release")) { server->tick(); sleep(1u) }
    server->stop(); server->cleanup()
    unsafe { delete server }
    return 0
}
