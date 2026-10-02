options gen2
require dashv/dashv_boost
require daslib/clargs
require strings
[export]
def main {
    let args <- get_cli_arguments()
    verify(length(args) == 1)
    let base = args[0]
    GET("{base}/ok") $(response) {
        verify(response != null)
        verify(string(response.body) == "verified")
    }
    GET("{base}/redirect") $(response) { verify(response == null) }
    GET("{base}/plaintext-count") $(response) {
        verify(response != null)
        verify(string(response.body) == "0")
    }
}
