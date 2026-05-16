// Tutorial JSONRPC-01: Building requests and parsing responses
//
// This tutorial covers:
//   - Building JSON-RPC 2.0 requests with `make_request` (int and string ids)
//   - Building notifications with `make_notification` (fire-and-forget)
//   - Wrapping pre-built messages as a §6 batch with `make_batch`
//   - Parsing server responses with `parse_response`
//   - Parsing batched server responses with `parse_response_batch`
//
// requires: daslib/jsonrpc
// Run: daslang.exe tutorials/jsonrpc/01_request_response.das

options gen2

require daslib/jsonrpc

[export]
def main() {
    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — Building a request
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `make_request(method, params_json, id)` builds a JSON-RPC 2.0 request as
    // a single-line string. `params_json` is a *pre-serialized* JSON value —
    // pass `"null"` for no params, or any valid JSON literal/object/array.

    print("=== Section 1: Building a request ===\n")

    let req1 = make_request("echo", "[\"hello\"]", 1)
    print("integer id:  {req1}\n")
    // → {"jsonrpc":"2.0","id":1,"method":"echo","params":["hello"]}

    let req2 = make_request("status", "null", "call-7")
    print("string id:   {req2}\n")
    // → {"jsonrpc":"2.0","id":"call-7","method":"status","params":null}

    // ──────────────────────────────────────────────────────────────────────────
    // Section 2 — Notifications (no response expected)
    // ──────────────────────────────────────────────────────────────────────────
    //
    // A notification omits the `id` field. Per JSON-RPC 2.0 §4.1, the server
    // MUST NOT send a response. Use these for fire-and-forget side effects
    // (logging, state updates, etc.).

    print("\n=== Section 2: Notifications ===\n")

    let notif = make_notification("log", "\{\"msg\":\"ready\"}")
    print("notification: {notif}\n")
    // → {"jsonrpc":"2.0","method":"log","params":{"msg":"ready"}}
    // Note: no "id" field.

    // ──────────────────────────────────────────────────────────────────────────
    // Section 3 — §6 batch: multiple messages in one wire payload
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `make_batch(messages)` wraps an array of pre-built request/notification
    // strings as a JSON array. The server returns an array of responses
    // (notification entries are suppressed from the response).

    print("\n=== Section 3: Building a batch ===\n")

    let entries <- [
        make_request("status", "null", 10),
        make_request("echo", "[1, 2, 3]", 11),
        make_notification("log", "\{\"level\":\"info\"\}")
    ]
    let batch = make_batch(entries)
    print("batch wire: {batch}\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 4 — Parsing a success response
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `parse_response(line)` returns a `ParsedResponse` struct. Check
    // `is_success` to distinguish `{"result":...}` from `{"error":...}`.
    // The result JSON is in `result_json` (raw, may be pretty-printed by
    // `write_json` on the server — use `compact_json_whitespace` if needed).

    print("\n=== Section 4: Parsing success ===\n")

    let success_wire = "\{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":[\"hello\"]\}"
    let r = parse_response(success_wire)
    print("is_success:  {r.is_success}\n")    // true
    print("id_str:      {r.id_str}\n")        // "1"
    print("result_json: {r.result_json}\n")   // ["hello"]
    print("parse_error: '{r.parse_error}'\n") // empty

    // ──────────────────────────────────────────────────────────────────────────
    // Section 5 — Parsing an error response
    // ──────────────────────────────────────────────────────────────────────────
    //
    // Error responses populate `error_code`, `error_msg`, and optionally
    // `error_data`. Standard codes are exposed as constants:
    //   PARSE_ERROR (-32700)        Invalid JSON
    //   INVALID_REQUEST (-32600)    Not a valid request object
    //   METHOD_NOT_FOUND (-32601)   Unknown method
    //   INVALID_PARAMS (-32602)     Bad params type
    //   INTERNAL_ERROR (-32603)     Server-side failure

    print("\n=== Section 5: Parsing error ===\n")

    let error_wire = "\{\"jsonrpc\":\"2.0\",\"id\":1,\"error\":\{\"code\":-32601,\"message\":\"method not found\"\}\}"
    let er = parse_response(error_wire)
    print("is_success: {er.is_success}\n")    // false
    print("error_code: {er.error_code}\n")    // -32601
    print("error_msg:  {er.error_msg}\n")     // method not found
    print("matches METHOD_NOT_FOUND: {er.error_code == METHOD_NOT_FOUND}\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 6 — Parsing a batched response
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `parse_response_batch(line)` detects whether the wire was a single
    // response or an array. For batches, `is_batch=true` and `responses[]`
    // holds one entry per array element.

    print("\n=== Section 6: Parsing a batch response ===\n")

    let batch_wire = "[\{\"jsonrpc\":\"2.0\",\"id\":10,\"result\":\"ok\"\},\{\"jsonrpc\":\"2.0\",\"id\":11,\"error\":\{\"code\":-32602,\"message\":\"bad params\"\}\}]"
    let pb = parse_response_batch(batch_wire)
    print("is_batch:    {pb.is_batch}\n")    // true
    print("count:       {length(pb.responses)}\n")
    for (entry in pb.responses) {
        if (entry.is_success) {
            print("  [{entry.id_str}] result: {entry.result_json}\n")
        } else {
            print("  [{entry.id_str}] error {entry.error_code}: {entry.error_msg}\n")
        }
    }

    // ──────────────────────────────────────────────────────────────────────────
    // Next steps
    // ──────────────────────────────────────────────────────────────────────────
    //
    // - 02_dispatch_line.das — implementing a server with `dispatch_line`
    // - 03_batch.das         — server-side batch handling and per-entry errors
    //
    // For a runnable end-to-end pair, see examples/mcp/echo/.

    print("\nDone.\n")
}
