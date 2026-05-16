// Tutorial JSONRPC-03: §6 batches — multiple messages in one wire payload
//
// This tutorial covers:
//   - Sending a §6 batch: requests + notifications mixed
//   - Server-side dispatch_line preserves input order, suppresses
//     notification entries from the response array
//   - Per-entry errors: malformed entries get individual error envelopes
//   - Empty array: rejected with a single INVALID_REQUEST (not an array)
//   - All-notifications batch: no response at all
//
// requires: daslib/jsonrpc
// Run: daslang.exe tutorials/jsonrpc/03_batch.das

options gen2

require daslib/jsonrpc

def dispatch_method(method, params_json : string) : string {
    if (method == "echo") return jsonrpc::compact_json_whitespace(params_json)
    if (method == "ping") return "\"pong\""
    return "\{\"error\":\"unknown: {method}\"}"
}

[export]
def main() {
    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — Round-trip: build, dispatch, parse
    // ──────────────────────────────────────────────────────────────────────────
    //
    // Client builds a batch of two requests + one notification. Server
    // dispatches them. Response is an array of TWO entries (notification
    // produces no response, input order preserved).

    print("=== Section 1: Mixed batch round-trip ===\n")

    let batch_entries <- [
        make_request("ping", "null", 1),
        make_notification("log", "\{\"msg\":\"hello\"}"),
        make_request("echo", "[42]", 2)
    ]
    let wire = make_batch(batch_entries)
    print("client wire: {wire}\n")

    let resp = jsonrpc::dispatch_line(wire, false) $(method, params_json) {
        return dispatch_method(method, params_json)
    }
    print("server resp: {resp}\n")
    // → [{"jsonrpc":"2.0","id":1,"result":"pong"},{"jsonrpc":"2.0","id":2,"result":[42]}]

    let rb = parse_response_batch(resp)
    print("parsed batch: is_batch={rb.is_batch}, count={length(rb.responses)}\n")
    for (entry in rb.responses) {
        print("  id={entry.id_str}, result={entry.result_json}\n")
    }

    // ──────────────────────────────────────────────────────────────────────────
    // Section 2 — Per-entry errors: continue-on-error semantics
    // ──────────────────────────────────────────────────────────────────────────
    //
    // Malformed entries (missing method, bad id type, etc.) get individual
    // error envelopes in their array slot. Valid entries still dispatch.

    print("\n=== Section 2: Per-entry errors ===\n")

    let mixed = "[\{\"id\":1,\"method\":\"ping\"},\{\"id\":2},\{\"id\":3,\"method\":\"echo\",\"params\":[\"x\"]}]"
    print("input:  {mixed}\n")
    let mixed_resp = jsonrpc::dispatch_line(mixed, false) $(method, params_json) {
        return dispatch_method(method, params_json)
    }
    print("output: {mixed_resp}\n")
    // → three entries: id=1 result, id=2 error -32600 (missing method), id=3 result

    // ──────────────────────────────────────────────────────────────────────────
    // Section 3 — Empty batch array → single INVALID_REQUEST (NOT an array)
    // ──────────────────────────────────────────────────────────────────────────
    //
    // Per JSON-RPC 2.0 §6, an empty array `[]` is itself an invalid request;
    // the server responds with a SINGLE error envelope, not an array.

    print("\n=== Section 3: Empty batch ===\n")

    let empty_resp = jsonrpc::dispatch_line("[]", false) $(method, params_json) {
        return "unused"
    }
    print("[] response: {empty_resp}\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 4 — All-notifications batch → no wire response
    // ──────────────────────────────────────────────────────────────────────────
    //
    // If every entry in a batch is a notification, the server processes them
    // (dispatcher runs for each) but emits nothing — the response array
    // would be empty, so per §6 the server returns nothing at all.

    print("\n=== Section 4: All-notifications batch ===\n")

    var notif_calls = 0
    let all_notif = "[\{\"method\":\"a\"},\{\"method\":\"b\"},\{\"method\":\"c\"}]"
    let all_notif_resp = jsonrpc::dispatch_line(all_notif, false) $(method, params_json) {
        notif_calls++
        return "ignored"
    }
    print("dispatcher invoked: {notif_calls} times\n")
    print("response wire empty: {empty(all_notif_resp)}\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 5 — Manual batch handling (when you need full control)
    // ──────────────────────────────────────────────────────────────────────────
    //
    // For custom error semantics (per-method INVALID_PARAMS, METHOD_NOT_FOUND,
    // logging per entry, etc.), use `parse_batch` directly.

    print("\n=== Section 5: Manual batch via parse_batch ===\n")

    let pb = parse_batch(wire)
    print("is_batch: {pb.is_batch}, requests: {length(pb.requests)}, framing_error: '{pb.framing_error}'\n")
    for (req in pb.requests) {
        if (!empty(req.error_envelope)) {
            print("  malformed: {req.error_envelope}\n")
        } else {
            let kind = req.is_notification ? "notif" : "req"
            print("  {kind}: method={req.method}, id={req.id_str}, params={req.params_json}\n")
        }
    }

    print("\nDone.\n")
}
