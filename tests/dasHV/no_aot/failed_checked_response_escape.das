//! a checked HTTPS response borrowed by its callback cannot be stored
//! in a variable that outlives the call (expect 30915)
options gen2
expect 30915
require dashv/dashv_boost
[export]
def main {
    var escaped : HttpResponse? = null
    with_http_request() $(var request) {
        request.url := "https://localhost/"
        request_checked(request, "", 64) $(response) {
            escaped = response
        }
    }
}
