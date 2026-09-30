options gen2
options indenting = 4
options no_unused_function_arguments = false
module _server_limits_fixture shared public
require dashv/dashv_boost public
require strings

class LimitedServer : HvWebServer {
    handled, opened, closed : int
    http_limit : int = 8
    ws_limit : int = 16
    event_limit : int = 8
    byte_limit : int = 4096
    last_message_size : int
    def override onInit {
        verify(set_limits(server, http_limit, ws_limit, event_limit, byte_limit))
        POST("/echo") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            self.handled++
            return resp |> TEXT_PLAIN(string(req.body))
        }
        GET("/handled") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("{self.handled}")
        }
        GET("/locked") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN(set_limits(server, 64, 64, 8, 4096) ? "changed" : "locked")
        }
    }
    def override onWsOpen(channel : WebSocketChannel; url : string#) { opened++ }
    def override onWsClose(channel : WebSocketChannel) { closed++ }
    def override onWsMessageFrame(channel : WebSocketChannel; msg : string#; byte_count : int; opcode : ws_opcode) {
        handled++
        last_message_size = byte_count
        send(channel, msg, byte_count, opcode, true)
    }
}
