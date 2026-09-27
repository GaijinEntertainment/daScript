options gen2

// `hv_set_log_file` moves libhv's own daily log out of the working directory: a server started
// after the call logs into `<path>.YYYYMMDD.log`, and an empty path changes nothing.

require dastest/testing_boost public
require _dashv_test_common
require strings

class LogTestServer : HvWebServer {
    def override onInit {
        GET("/ping") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("pong")
        }
    }
}

[test]
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
