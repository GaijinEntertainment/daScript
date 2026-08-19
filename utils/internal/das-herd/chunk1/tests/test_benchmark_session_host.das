options gen2

require dastest/testing_boost public
require ../benchmark_session_host.das

[test]
def test_read_file_reports_its_failure(t : T?) {
    t |> run("a missing screen file surfaces its error to the caller") @(t : T?) {
        var error = ""
        let text = read_file("/nonexistent/das-herd/screen.txt", error)
        t |> equal(text, "")
        t |> success(!empty(error), "the out-parameter must carry the reason - it was a by-value copy once, and every failed read reported ok")
    }
}
