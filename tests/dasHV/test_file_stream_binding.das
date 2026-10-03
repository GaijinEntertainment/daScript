options gen2
require dastest/testing_boost public
require dashv/dashv_boost

[test]
def file_stream_rejects_invalid_ownership_without_file_access(t : T?) {
    t |> equal(SERVE_FILE_STREAM(default<WebSocketServer>, null, "unused.bin", 1024l), -1)
    t |> equal(SERVE_FILE_STREAM(default<WebSocketServer>, null, "", 0l), -1)
}
