options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require dashv/dashv_boost
require _dashv_test_common
require strings
require daslib/strings_boost
require _tls_fixture
require daslib/clargs

[test(description = "SERVE_FILE_STREAM refuses a null owner or an empty path with -1 without touching the file system")]
def file_stream_rejects_invalid_ownership_without_file_access(t : T?) {
    t |> equal(SERVE_FILE_STREAM(default<WebSocketServer>, null, "unused.bin", 1024l), -1)
    t |> equal(SERVE_FILE_STREAM(default<WebSocketServer>, null, "", 0l), -1)
}

// `hv_set_log_file` moves libhv's own daily log out of the working directory: a server started
// after the call logs into `<path>.YYYYMMDD.log`, and an empty path changes nothing.


class LogTestServer : HvWebServer {
    def override onInit {
        GET("/ping") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("pong")
        }
    }
}

[test(description = "The server's libhv log lines land beside the path hv_set_log_file names")]
def test_log_file(t : T?) {
    t |> run("the server's libhv lines land beside the path the call names") @(t : T?) {
        var terr = ""
        let dir = path_join(temp_directory(terr), "dashv_log_file_{ref_time_ticks()}")
        mkdir_rec(dir)
        hv_set_log_file("")
        hv_set_log_file(path_join(dir, "hvlog"))
        with_test_server(type<LogTestServer>) $(base_url) {
            GET("{base_url}/ping") $(resp) {
                t |> equal(resp.body, "pong")
            }
        }
        var logs : array<string>
        dir(dir) $(name) {
            if (starts_with(name, "hvlog.") && ends_with(name, ".log")) {
                logs |> push(name)
            }
        }
        t |> success(length(logs) == 1, "one daily log under the named path: {logs}")
        hv_set_log_file("libhv")
        rmdir_rec(dir)
    }
}

class PrivateLogServer : HvWebServer {
    def override onInit {
        verify(set_access_log(server, false))
        ANY("*") <| @(var request : HttpRequest?; var response : HttpResponse?) : http_status {
            return response |> TEXT_PLAIN("ok")
        }
    }
}

[test(description = "With access logging off the log file never echoes request paths, queries or cookies")]
def disabled_access_log_does_not_echo_request_secrets(t : T?) {
    var error : string
    let root = create_temp_directory("hv_private_log", error)
    t |> equal(error, "")
    if (!empty(error)) { return }
    hv_set_log_file("{root}/capture")
    with_test_server(type<PrivateLogServer>) $(base_url) {
        GET("{base_url}/SENTINEL_PATH?code=SENTINEL_CODE") $(_response) {}
        GET("{base_url}/%0aSENTINEL_INVALID_PATH") $(_response) {}
        with_http_request() $(var request) {
            request.method = http_method.GET
            request.url := "{base_url}/cookie"
            set_header(request, "Cookie", "SENTINEL_COOKIE_MALFORMED")
            request(request) $(_response) {}
        }
    }
    hv_set_log_file("libhv")
    var captured : string
    dir(root) $(filename) {
        if (ends_with(filename, ".log")) { captured += fread("{root}/{filename}") }
    }
    t |> success(!empty(captured), "server lifecycle/errors were actually captured")
    t |> success(!contains(captured, "SENTINEL"), "no request secrets echoed by lower-level logs")
    let removed = rmdir_rec_result(root)
    t |> success(removed is value && removed as value)
}

[test(description = "The default HTTPS client refuses a certificate from a CA that is not a trusted public issuer")]
def default_https_refuses_an_untrusted_certificate(t : T?) {
    with_tls_fixture(t) $(_root, dns_port, _ip_port) {
        GET("https://localhost:{dns_port}/ok") $(response) {
            t |> success(response == null, "a fresh test CA is not a trusted public issuer")
        }
    }
}

[test(description = "The default HTTPS client does not send a downgraded plaintext request after a redirect, proven by a child process", timeout = 1200)]
def default_https_does_not_send_a_downgraded_request(t : T?) {
    with_tls_fixture(t) $(root, dns_port, _ip_port) {
        let fixture = "{get_das_root()}/tests/dasHV/no_aot/_fixture_https_redirect.das"
        var output : string
        let code = run_and_capture([get_host_binary(), "-dasroot", get_das_root(), fixture, "--", "https://localhost:{dns_port}"], "", ["SSL_CERT_FILE={root}/ca.pem"], output, 15.0)
        t |> equal(code, 0, output)
    }
}
