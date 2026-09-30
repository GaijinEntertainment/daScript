options gen2
options indenting = 4
options no_aot // native TLS fixture is controlled through subprocess APIs
require _tls_fixture

[test]
def checked_https_verifies_trust_and_peer_name(t : T?) {
    with_tls_fixture(t) $(root, dns_port, ip_port) {
        var replies = 0
        with_http_request() $(var request) {
            request.timeout = 3
            request.url := "https://localhost:{dns_port}/ok"
            let trusted = request_checked(request, "{root}/ca.pem", 64) $(response) {
                replies++
                t |> equal(int(response.status_code), 200)
                t |> equal(response.body, "verified")
            }
            t |> equal(trusted, 0)
            let untrusted = request_checked(request, "{root}/wrong.pem", 64) $(_response) { replies++ }
            t |> success(untrusted != 0)
            request.url := "https://127.0.0.1:{dns_port}/ok"
            let mismatch = request_checked(request, "{root}/ca.pem", 64) $(_response) { replies++ }
            t |> success(mismatch != 0, "trusted issuer does not authorize an unmatched IP")
            request.url := "https://127.0.0.1:{ip_port}/ok"
            let ip_match = request_checked(request, "{root}/ca.pem", 64) $(response) {
                replies++
                t |> equal(response.body, "verified")
            }
            t |> equal(ip_match, 0)
        }
        t |> equal(replies, 2, "only verified requests publish a response")
    }
}

[test]
def checked_https_bounds_bodies_and_does_not_follow_redirects(t : T?) {
    with_tls_fixture(t) $(root, dns_port, ip_port) {
        var replies = 0
        with_http_request() $(var request) {
            request.timeout = 3
            request.url := "https://localhost:{dns_port}/large"
            let too_large = request_checked(request, "{root}/ca.pem", 64) $(_response) { replies++ }
            t |> equal(too_large, 1022, "response exceeded the byte budget")
            request.url := "https://localhost:{dns_port}/redirect"
            let redirect = request_checked(request, "{root}/ca.pem", 64) $(response) {
                replies++
                t |> equal(int(response.status_code), 302)
            }
            t |> equal(redirect, 0)
        }
        t |> equal(replies, 1)
    }
}

[test]
def checked_https_never_publishes_truncated_response(t : T?) {
    with_tls_fixture(t) $(root, dns_port, ip_port) {
        var replies = 0
        with_http_request() $(var request) {
            request.timeout = 3
            request.url := "https://localhost:{dns_port}/truncated"
            let result = request_checked(request, "{root}/ca.pem", 1024) $(_response) { replies++ }
            t |> success(result != 0)
            t |> equal(replies, 0)
            request.url := "https://localhost:{dns_port}/count"
            let count_result = request_checked(request, "{root}/ca.pem", 64) $(response) {
                t |> equal(response.body, "1", "transport does not retry a completed request")
            }
            t |> equal(count_result, 0)
        }
    }
}
