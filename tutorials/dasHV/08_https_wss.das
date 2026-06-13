// Tutorial HV-08: HTTPS / WSS server with TLS
//
// This tutorial covers:
//   - Starting a TLS server with init_wss(port, httpsPort) instead of init(port)
//   - The bundled self-signed certificate under modules/dasHV/cert
//   - Serving the same routes over HTTPS
//   - Making an HTTPS request from the client
//
// Prerequisites: Tutorial HV-03 (HTTP server basics)
//
// init_wss starts the server on BOTH a plain HTTP port and a TLS port. The
// certificate/key default to modules/dasHV/cert (server.crt / server.key),
// which ship with dasHV — pass a directory to init_wss to use your own.
//
// Run: daslang.exe tutorials/dasHV/08_https_wss.das

options gen2
options persistent_heap
options gc

require dashv/dashv_boost public
require daslib/jobque_boost
require daslib/json_boost
require daslib/fio

let SERVER_PORT = 18091         // plain HTTP
let SERVER_HTTPS_PORT = 18443   // TLS

// ──────────────────────────────────────────────────────────────────────────
// A minimal TLS server — same route API as a plain HvWebServer
// ──────────────────────────────────────────────────────────────────────────

class TutorialTlsServer : HvWebServer {
    def override onInit {
        GET("/secure") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("secure hello over TLS")
        }
        GET("/api/info") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let payload = (scheme = "https", tls = true)
            return resp |> JSON(write_json(JV(payload)))
        }
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Server lifecycle helper — identical to HV-03's, but init_wss for TLS
// ──────────────────────────────────────────────────────────────────────────

def with_wss_server(port, https_port : int; blk : block<(https_url : string) : void>) {
    with_job_status(1) $(started) {
        with_job_status(1) $(finished) {
            with_atomic32() $(stop_flag) {
                new_thread() @() {
                    var server = new TutorialTlsServer()
                    server->init_wss(port, https_port)   // default cert dir: modules/dasHV/cert
                    server->start()
                    started |> notify_and_release
                    while (stop_flag |> get == 0) {
                        server->tick()
                        sleep(10u)
                    }
                    server->stop()
                    server->cleanup()
                    unsafe {
                        delete server
                    }
                    finished |> notify_and_release
                }
                started |> join
                let https_url = "https://127.0.0.1:{https_port}"
                invoke(blk, https_url)
                stop_flag |> set(1)
                finished |> join
            }
        }
    }
}

[export]
def main() {
    print("HV-08: HTTPS / WSS server with TLS\n\n")
    with_wss_server(SERVER_PORT, SERVER_HTTPS_PORT) $(https_url) {

        // ──────────────────────────────────────────────────────────────────
        // Section 1 — An HTTPS GET
        // ──────────────────────────────────────────────────────────────────
        //
        // The client follows the https:// scheme and negotiates TLS. The
        // bundled certificate is self-signed, which the client accepts for
        // this local test.

        print("=== Section 1: HTTPS GET ===\n")
        var ok_secure = false
        GET("{https_url}/secure") $(resp) {
            print("GET {https_url}/secure -> {resp.status_code}: {resp.body}\n")
            ok_secure = resp.status_code == http_status.OK && resp.body == "secure hello over TLS"
        }
        assert(ok_secure, "HTTPS GET /secure did not return the expected response")

        // ──────────────────────────────────────────────────────────────────
        // Section 2 — A JSON route over HTTPS
        // ──────────────────────────────────────────────────────────────────

        print("\n=== Section 2: JSON over HTTPS ===\n")
        var ok_json = false
        GET("{https_url}/api/info") $(resp) {
            print("GET {https_url}/api/info -> {resp.status_code}: {resp.body}\n")
            var err : string
            var parsed = read_json(string(resp.body), err)
            ok_json = parsed != null
        }
        assert(ok_json, "HTTPS GET /api/info did not return valid JSON")
    }
    print("\nDone.\n")
}
