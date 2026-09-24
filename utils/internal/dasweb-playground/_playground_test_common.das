options gen2
options indenting = 4

module _playground_test_common public

require dashv/dashv_boost
require daslib/json_boost

def posted_hash(base_url : string; source : string) : string {
    var hash = ""
    POST("{base_url}/api/samples", source) $(resp) {
        if (resp != null && resp.status_code == http_status.OK) {
            var jerr = ""
            var doc = read_json(string(resp.body), jerr)
            hash = doc?["hash"] ?? ""
        }
    }
    return hash
}
