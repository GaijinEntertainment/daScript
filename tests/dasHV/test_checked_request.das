options gen2
options indenting = 4
require dastest/testing_boost public
require dashv/dashv_boost

[test]
def checked_request_rejects_invalid_budget_and_plaintext(t : T?) {
    var invoked = 0
    with_http_request() $(var request) {
        request.url := "https://127.0.0.1:9/"
        request.timeout = 1
        for (limit in [-1, 0, 16777217]) {
            let result = request_checked(request, "", limit) $(response) { invoked++ }
            t |> equal(result, 1011, "invalid argument is rejected before transport")
        }
        request.url := "http://127.0.0.1:9/"
        let result = request_checked(request, "", 4096) $(response) { invoked++ }
        t |> equal(result, 1018, "plaintext is not a checked HTTPS request")
    }
    t |> equal(invoked, 0)
}
