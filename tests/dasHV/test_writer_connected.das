// Tests for is_writer_connected — writer liveness while held in active_writers
//
// The async writer ops never report a dead peer; is_writer_connected is the primitive a server
// polls to evict streams whose client disconnected (dasllama-server's S4 eviction).

options gen2
options persistent_heap
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require _dashv_test_common
require strings

// Server-context globals: handlers run serially on the tick thread, in the server thread's context.
var g_held : HttpResponseWriter?
var g_at_open = false

class HoldServer : HvWebServer {
    def override onInit {
        // Hold a stream open forever: record liveness at open, stash the writer, flush headers
        // with one event, and never close — the client's read timeout is what ends the connection.
        STREAM("/hold") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            g_at_open = is_writer_connected(server, writer)
            g_held = writer
            sse_event(server, writer, "held", "")
        }
        GET("/status") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let now = g_held != null ? is_writer_connected(server, g_held) : false
            let nul = is_writer_connected(server, null)
            return resp |> TEXT_PLAIN("open={g_at_open} now={now} null={nul}")
        }
    }
}

def private fetch_status(base_url : string) : string {
    var body = ""
    GET("{base_url}/status") $(var resp) {
        if (resp != null) {
            body = clone_string(string(resp.body))
        }
    }
    return body
}

[test]
def test_writer_connected(t : T?) {
    with_test_server(type<HoldServer>, PORT_WRITER_CONNECTED) $(base_url) {
        t |> run("null / no held writer reads not-connected") @(t : T?) {
            let st = fetch_status(base_url)
            t |> success(find(st, "now=false") >= 0, "no writer held yet (got '{st}')")
            t |> success(find(st, "null=false") >= 0, "null writer is not connected (got '{st}')")
        }
        t |> run("held writer: connected at open, disconnected after the client goes away") @(t : T?) {
            // The stream never ends, so the 1s read timeout fires and the client closes the socket.
            with_http_request() $(var req) {
                req.method = http_method.GET
                req.url := "{base_url}/hold"
                set_timeout(req, 1)
                request(req) $(var resp) {}   // timeout: resp is null or partial — irrelevant
            }
            var st = ""
            for (_attempt in range(100)) {   // up to ~10s for the server loop to see the EOF
                st = fetch_status(base_url)
                if (find(st, "now=false") >= 0) {
                    break
                }
                sleep(100u)
            }
            t |> success(find(st, "open=true") >= 0, "writer was connected at handler time (got '{st}')")
            t |> success(find(st, "now=false") >= 0, "held writer reads disconnected after client abort (got '{st}')")
        }
    }
}
