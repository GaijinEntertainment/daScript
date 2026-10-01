options gen2
options indenting = 4
options no_unused_function_arguments = false
require dastest/testing_boost public
require _dashv_test_common
require daslib/strings_boost

class PrivateLogServer : HvWebServer {
    def override onInit {
        verify(set_access_log(server, false))
        ANY("*") <| @(var request : HttpRequest?; var response : HttpResponse?) : http_status {
            return response |> TEXT_PLAIN("ok")
        }
    }
}

[test]
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
