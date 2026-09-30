// Limits must reject bodies before routing, including cumulative chunked bodies.
options gen2
options indenting = 4
options persistent_heap
options gc
options no_unused_function_arguments = false
require dastest/testing_boost public
require _dashv_test_common
require _server_limits_fixture

[test]
def body_limit_preserves_boundary_and_rejects_before_handler(t : T?) {
    with_test_server(type<LimitedServer>) $(base_url) {
        POST("{base_url}/echo", "12345678") $(resp) {
            t |> equal(int(resp.status_code), 200)
            t |> equal(resp.body, "12345678")
        }
        POST("{base_url}/echo", "123456789") $(resp) {
            t |> equal(int(resp.status_code), 413)
        }
        GET("{base_url}/handled") $(resp) {
            t |> equal(resp.body, "1", "oversized body never reaches application code")
        }
        GET("{base_url}/locked") $(resp) {
            t |> equal(resp.body, "locked", "limits are immutable while workers run")
        }
    }
}
