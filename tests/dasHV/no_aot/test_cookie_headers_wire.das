options gen2
options indenting = 4
options no_aot // Raw TCP fixture uses the interpreter-only network module.
options no_unused_function_arguments = false
require dastest/testing_boost public
require ../_server_limits_fixture.das
require _wire_peer

class CookieWireServer : LimitedServer {
    raw_cookie, iterated_cookie, other_cookie : string
    cookie_headers : int
    def override onInit {
        GET("/cookies") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            self.raw_cookie = get_header(req, "Cookie")
            self.other_cookie = get_cookie(req, "b")
            each_header(req) $(name, value) {
                if (to_lower(name) == "cookie") { self.cookie_headers++; self.iterated_cookie = clone_string(value) }
            }
            return resp |> TEXT_PLAIN("ok")
        }
    }
}
[test]
def raw_cookie_header_preserves_duplicates_and_header_iteration_is_not_duplicated(t : T?) {
    var server = new CookieWireServer()
    server.init(0)
    t |> equal(server.start(), 0)
    var peer = new WirePeer()
    t |> success(peer.connect("127.0.0.1", server.bound_port()))
    t |> success(peer.write_text("GET /cookies HTTP/1.1\r\nHost: localhost\r\ncOoKiE: a=one; a=two\r\ncookie: b=three\r\nConnection: close\r\n\r\n"))
    t |> success(pump(server, peer, $ { return peer.disconnected; }))
    t |> equal(server.raw_cookie, "a=one; a=two; b=three")
    t |> equal(server.other_cookie, "three", "convenience cookie lookup still works")
    t |> equal(server.cookie_headers, 1)
    t |> equal(server.iterated_cookie, server.raw_cookie)
    peer.close(); server.stop(); server.cleanup()
    unsafe { delete peer; delete server }
}
