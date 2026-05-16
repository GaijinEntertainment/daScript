options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// Minimal JSON-RPC 2.0 stdio server built on daslib/jsonrpc.
//
// Three toy methods:
//   echo(...)       — returns params verbatim
//   add({a,b})      — returns a + b
//   status()        — returns a greeting string
//
// Reads newline-delimited JSON-RPC from stdin, writes responses to stdout.
// Handles single requests, §6 batches, and notifications (no response).
//
// Usage:
//   echo '{"jsonrpc":"2.0","id":1,"method":"echo","params":[42]}' | daslang server.das
//   echo '[{"id":1,"method":"add","params":{"a":2,"b":3}},{"id":2,"method":"status"}]' | daslang server.das

require daslib/jsonrpc
require daslib/json
require daslib/json_boost
require daslib/fio
require strings

def dispatch_method(method, params_json : string) : string {
    if (method == "echo") return jsonrpc::compact_json_whitespace(params_json)
    if (method == "add") {
        var err = ""
        let p = read_json(params_json, err)
        if (p == null || !(p.value is _object)) return "\{\"error\":\"add requires object params\"}"
        let obj & = unsafe(p.value as _object)
        let a_v = unsafe(reinterpret<json::JsonValue?>(obj?["a"] ?? null))
        let b_v = unsafe(reinterpret<json::JsonValue?>(obj?["b"] ?? null))
        let a = (a_v != null && a_v.value is _longint) ? int(a_v.value as _longint) : 0
        let b = (b_v != null && b_v.value is _longint) ? int(b_v.value as _longint) : 0
        return "{a + b}"
    }
    if (method == "status") return "\"ok\""
    // Unknown method — return an error JSON value as result. The transport
    // wraps it in a result envelope; spec-strict servers would emit
    // METHOD_NOT_FOUND via the lower-level parse_batch API instead.
    return "\{\"error\":\"unknown method: {method}\"}"
}

[export]
def main() {
    let sin = fstdin()
    let sout = fstdout()
    while (!feof(sin)) {
        let chunk = fgets(sin) // nolint:PERF003
        let len = length(chunk)
        if (len == 0) break
        var line = chunk
        if (character_at(chunk, len - 1) == '\n') { // nolint:PERF003
            line = (len > 1 && character_at(chunk, len - 2) == '\r') ? slice(chunk, 0, len - 2) : slice(chunk, 0, len - 1) // nolint:PERF003
        }
        if (empty(line)) continue
        let response = jsonrpc::dispatch_line(line, false) $(method, params_json) {
            return dispatch_method(method, params_json)
        }
        if (!empty(response)) {
            fprint(sout, response)
            fprint(sout, "\n")
            fflush(sout)
        }
    }
}
