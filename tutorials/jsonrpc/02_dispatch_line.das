// Tutorial JSONRPC-02: Implementing a server with dispatch_line
//
// This tutorial covers:
//   - The `dispatch_line(line, strict, dispatcher_block) → response` convenience
//   - Writing a dispatcher block: (method, params_json) → result_json
//   - Notification semantics: dispatcher runs, but no response is emitted
//   - Top-level parse failure: framing_error returned to wire automatically
//   - Strict vs permissive JSON-RPC compliance
//
// requires: daslib/jsonrpc
// Run: daslang.exe tutorials/jsonrpc/02_dispatch_line.das

options gen2

require daslib/jsonrpc

// A minimal dispatcher: three toy methods.
def dispatch_method(method, params_json : string) : string {
    if (method == "echo") return jsonrpc::compact_json_whitespace(params_json)
    if (method == "ping") return "\"pong\""
    if (method == "ok") return "true"
    return "\{\"error\":\"unknown method: {method}\"}"
}

[export]
def main() {
    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — Single request through dispatch_line
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `dispatch_line` parses the wire line, calls the dispatcher block for
    // each non-notification request, wraps the result in a JSON-RPC response
    // envelope, and returns the wire-ready string. Notification handling,
    // batch fan-out, and framing-error suppression all happen for free.

    print("=== Section 1: Single request ===\n")

    let req = "\{\"id\":1,\"method\":\"ping\"}"
    let resp = jsonrpc::dispatch_line(req, false) $(method, params_json) {
        return dispatch_method(method, params_json)
    }
    print("request:  {req}\n")
    print("response: {resp}\n")
    // → {"jsonrpc":"2.0","id":1,"result":"pong"}

    // ──────────────────────────────────────────────────────────────────────────
    // Section 2 — Notification: dispatcher runs, no response is emitted
    // ──────────────────────────────────────────────────────────────────────────

    print("\n=== Section 2: Notification ===\n")

    var notif_count = 0
    let notif = "\{\"method\":\"log\",\"params\":[\"hi\"]}"
    let notif_resp = jsonrpc::dispatch_line(notif, false) $(method, params_json) {
        notif_count++  // side effect: dispatcher runs
        return "\"ignored\""
    }
    print("notification:       {notif}\n")
    print("dispatcher ran:     {notif_count == 1}\n")
    print("response is empty:  {empty(notif_resp)}\n")
    // Per JSON-RPC 2.0 §4.1, a notification (no id) gets no response.

    // ──────────────────────────────────────────────────────────────────────────
    // Section 3 — Top-level parse failure (no dispatcher call)
    // ──────────────────────────────────────────────────────────────────────────
    //
    // If the wire line isn't valid JSON, dispatch_line returns a pre-built
    // PARSE_ERROR envelope and the dispatcher is NEVER called. Same for
    // top-level §6 violations (empty batch array).

    print("\n=== Section 3: Top-level parse error ===\n")

    var bad_count = 0
    let bad_resp = jsonrpc::dispatch_line("not json", false) $(method, params_json) {
        bad_count++
        return "ok"
    }
    print("malformed input → response: {bad_resp}\n")
    print("dispatcher ran: {bad_count == 1} (expected false)\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 4 — Strict mode
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `dispatch_line(line, strict=false)` is permissive: the `jsonrpc` field
    // is optional and any string value is accepted. Pass `strict=true` to
    // enforce §4 — missing/non-"2.0" jsonrpc field yields INVALID_REQUEST.

    print("\n=== Section 4: Strict mode ===\n")

    let no_jsonrpc = "\{\"id\":1,\"method\":\"ping\"}"
    let permissive = jsonrpc::dispatch_line(no_jsonrpc, false) $(method, params_json) {
        return dispatch_method(method, params_json)
    }
    print("permissive accepts: {permissive}\n")

    let strict = jsonrpc::dispatch_line(no_jsonrpc, true) $(method, params_json) {
        return dispatch_method(method, params_json)
    }
    print("strict rejects:     {strict}\n")
    // → contains -32600 INVALID_REQUEST

    // ──────────────────────────────────────────────────────────────────────────
    // Section 5 — Lower-level API when dispatch_line doesn't fit
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `dispatch_line` wraps all results in a success envelope (the dispatcher
    // can only return result JSON). When you need to emit specific error
    // codes per method (e.g. METHOD_NOT_FOUND, INVALID_PARAMS), drop down to
    // `parse_request` + envelope builders directly:

    print("\n=== Section 5: Lower-level (parse_request + envelopes) ===\n")

    let custom_req = "\{\"id\":42,\"method\":\"unknown\"}"
    let parsed = parse_request(custom_req)
    var custom_resp = ""
    if (!empty(parsed.error_envelope)) {
        custom_resp = parsed.is_notification ? "" : parsed.error_envelope
    } elif (parsed.method != "ping" && parsed.method != "echo") {
        custom_resp = error(parsed.id_str, METHOD_NOT_FOUND, "no such method: {parsed.method}")
    } else {
        custom_resp = response(parsed.id_str, dispatch_method(parsed.method, parsed.params_json))
    }
    print("custom dispatch with -32601: {custom_resp}\n")

    print("\nDone.\n")
}
