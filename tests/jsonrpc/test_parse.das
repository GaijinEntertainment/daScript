options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/jsonrpc
require daslib/json
require strings

// ===== Valid request parsing =====

[test(description = "parse_request reads a request with only a method as a notification with null params")]
def test_method_only_no_params_no_id(t : T?) {
    let r = parse_request("\{\"method\":\"status\"}")
    t |> equal(r.error_envelope, "")
    t |> equal(r.method, "status")
    t |> equal(r.id_str, "null")
    t |> equal(r.params_json, "null")
    t |> success(r.params == null, "params is null when absent")
    t |> success(r.is_notification, "no id → notification")
}

[test(description = "parse_request accepts integer, string, null and float ids")]
def test_id_types_accepted(t : T?) {
    t |> run("integer id passes through") @(t : T?) {
        let r = parse_request("\{\"jsonrpc\":\"2.0\",\"id\":42,\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.id_str, "42")
        t |> equal(r.method, "status")
        t |> success(!r.is_notification, "present id → not a notification")
    }
    t |> run("string id passes through quoted") @(t : T?) {
        let r = parse_request("\{\"id\":\"call-1\",\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.id_str, "\"call-1\"")
    }
    t |> run("explicit null id is NOT a notification (spec §4.1)") @(t : T?) {
        // JSON-RPC 2.0 distinguishes "absent id" (notification) from
        // "explicit null id" (request expecting a response with id:null).
        let r = parse_request("\{\"id\":null,\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> success(!r.is_notification, "explicit null id ≠ notification")
        t |> equal(r.id_str, "null")
    }
    t |> run("number id accepted (float)") @(t : T?) {
        let r = parse_request("\{\"id\":3.14,\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
    }
}

[test(description = "parse_request returns object and array params as JSON text and absent params as null")]
def test_params_round_trip(t : T?) {
    t |> run("object params populate params_json") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"set_color\",\"params\":\{\"r\":0.5}}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.method, "set_color")
        t |> success(find(r.params_json, "\"r\"") >= 0, "params_json carries the r key")
        t |> success(r.params != null, "params is not null when present")
    }
    t |> run("array params") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"add\",\"params\":[1,2,3]}")
        t |> equal(r.error_envelope, "")
        // params_json comes from write_json which pretty-prints; compact for comparison.
        t |> equal(compact_json_whitespace(r.params_json), "[1,2,3]")
    }
    t |> run("missing params → 'null'") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"ping\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.params_json, "null")
        t |> success(r.params == null, "params field stays null")
    }
}

// ===== gc_node reachability test =====
//
// The parser parses the body into a local JsonValue tree (`var parsed = read_json(...)`),
// then stores a view into `ParsedRequest.params` pointing at a child node. The local
// `parsed` goes out of scope when parse_request returns. JsonValue? is a plain raw
// pointer tracked by gc_node (not a smart_ptr — those are mostly retired); the GC
// keeps the tree reachable while `result.params` still points into it. This test
// confirms the borrowed child view is live after the parser returns — if the GC
// freed the tree, accessing params would crash or return garbage.

[test(description = "the params of a parsed request remain readable after parse_request returns")]
def test_params_lifetime_survives_parser_return(t : T?) {
    // Parse, return, then read params well after the parser's stack is gone.
    let req = parse_request("\{\"id\":1,\"method\":\"set\",\"params\":\{\"a\":1,\"b\":\"hello\",\"c\":[1,2,3]}}")
    t |> equal(req.error_envelope, "")
    t |> success(req.params != null, "params survives parser return")
    // Re-serialize the live params node — if the underlying memory was freed,
    // write_json would crash or produce garbage.
    let re = compact_json_whitespace(write_json(req.params))
    t |> success(find(re, "\"a\"") >= 0, "field 'a' present in re-serialized params")
    t |> success(find(re, "\"hello\"") >= 0, "string value 'hello' present")
    t |> success(find(re, "[1,2,3]") >= 0, "nested array present")
    // Direct field access through the gc-tracked pointer — confirms the variant data is live.
    t |> success(req.params.value is _object, "params is still an _object after parser return")
}

[test(description = "array params of a parsed request remain readable after parse_request returns")]
def test_params_lifetime_array_root(t : T?) {
    // Same test but params is an array (different storage path in JsonValue).
    let req = parse_request("\{\"id\":7,\"method\":\"sum\",\"params\":[10,20,30]}")
    t |> equal(req.error_envelope, "")
    t |> success(req.params != null, "array params survives")
    t |> success(req.params.value is _array, "still an array")
    let re = compact_json_whitespace(write_json(req.params))
    t |> equal(re, "[10,20,30]")
}

// ===== Permissive jsonrpc field (default mode) =====

[test(description = "by default parse_request accepts a missing or non-2.0 jsonrpc field")]
def test_permissive_jsonrpc_field(t : T?) {
    t |> run("missing jsonrpc field accepted by default") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.method, "status")
    }
    t |> run("non-2.0 jsonrpc value accepted by default") @(t : T?) {
        let r = parse_request("\{\"jsonrpc\":\"1.0\",\"id\":1,\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.method, "status")
    }
    t |> run("strict=false flag is identical to default") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"status\"}", false)
        t |> equal(r.error_envelope, "")
    }
}

// ===== Params type validation (§4.2) =====

[test(description = "parse_request rejects scalar and string params and accepts null, object and array params")]
def test_params_must_be_array_or_object(t : T?) {
    t |> run("scalar params rejected") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"x\",\"params\":42}")
        t |> success(!empty(r.error_envelope), "envelope built")
        t |> success(find(r.error_envelope, "-32602") >= 0, "INVALID_PARAMS code")
        t |> success(find(r.error_envelope, "array or object") >= 0, "message names the rule")
    }
    t |> run("string params rejected") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"x\",\"params\":\"hello\"}")
        t |> success(find(r.error_envelope, "-32602") >= 0, "INVALID_PARAMS code")
    }
    t |> run("null params accepted (treated as absent)") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"x\",\"params\":null}")
        t |> equal(r.error_envelope, "")
    }
    t |> run("object params accepted") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"x\",\"params\":\{\"a\":1}}")
        t |> equal(r.error_envelope, "")
    }
    t |> run("array params accepted") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":\"x\",\"params\":[1,2]}")
        t |> equal(r.error_envelope, "")
    }
}

// ===== JSON-RPC 2.0 §6 batch parsing =====

[test(description = "parse_batch of one object reports is_batch false with one request")]
def test_single_request_not_batch(t : T?) {
    let pb = parse_batch("\{\"id\":1,\"method\":\"x\"}")
    t |> success(!pb.is_batch, "single object → is_batch=false")
    t |> equal(length(pb.requests), 1)
    t |> equal(pb.framing_error, "")
    t |> equal(pb.requests[0].method, "x")
    t |> equal(pb.requests[0].id_str, "1")
}

[test(description = "parse_batch of an array of two requests reports is_batch true with both")]
def test_batch_two_valid(t : T?) {
    let pb = parse_batch("[\{\"id\":1,\"method\":\"a\"},\{\"id\":2,\"method\":\"b\"}]")
    t |> success(pb.is_batch, "array → is_batch=true")
    t |> equal(length(pb.requests), 2)
    t |> equal(pb.framing_error, "")
    t |> equal(pb.requests[0].method, "a")
    t |> equal(pb.requests[0].id_str, "1")
    t |> equal(pb.requests[1].method, "b")
    t |> equal(pb.requests[1].id_str, "2")
}

[test(description = "parse_batch of [] reports a batch with no requests and a framing error")]
def test_batch_empty_array(t : T?) {
    // §6: empty array → server responds with a SINGLE INVALID_REQUEST envelope, not an array.
    let pb = parse_batch("[]")
    t |> success(pb.is_batch, "is_batch=true for empty array")
    t |> success(empty(pb.requests), "no requests")
    t |> success(!empty(pb.framing_error), "framing_error populated")
    t |> success(find(pb.framing_error, "-32600") >= 0, "INVALID_REQUEST")
    t |> success(find(pb.framing_error, "empty batch") >= 0, "message names the rule")
}

[test(description = "parse_batch of text that is not JSON reports no batch, no requests and a framing error")]
def test_batch_top_level_parse_error(t : T?) {
    let pb = parse_batch("not json")
    t |> success(!pb.is_batch, "is_batch=false on top-level parse failure")
    t |> success(empty(pb.requests), "no requests")
    t |> success(!empty(pb.framing_error), "framing_error populated")
    t |> success(find(pb.framing_error, "-32700") >= 0, "PARSE_ERROR")
}

[test(description = "parse_batch keeps a batch entry that is invalid beside its valid siblings")]
def test_batch_mixed_valid_invalid(t : T?) {
    let pb = parse_batch("[\{\"id\":1,\"method\":\"a\"},\{\"bad\":1},\{\"id\":3,\"method\":\"c\"}]")
    t |> success(pb.is_batch, "is_batch=true")
    t |> equal(length(pb.requests), 3)
    t |> equal(pb.framing_error, "")
    t |> equal(pb.requests[0].error_envelope, "")
    t |> success(!empty(pb.requests[1].error_envelope), "second entry rejected (missing method)")
    t |> equal(pb.requests[2].error_envelope, "")
}

[test(description = "parse_batch marks every request of a notification-only batch as a notification")]
def test_batch_all_notifications(t : T?) {
    // §6: all-notifications batch → server returns nothing (empty wire). parse_batch
    // doesn't drop anything itself; the caller's dispatcher decides what to write back.
    let pb = parse_batch("[\{\"method\":\"a\"},\{\"method\":\"b\"}]")
    t |> success(pb.is_batch, "is_batch=true")
    t |> equal(length(pb.requests), 2)
    t |> success(pb.requests[0].is_notification, "first is notification")
    t |> success(pb.requests[1].is_notification, "second is notification")
}

[test(description = "parse_batch of an array of non-objects yields one invalid request per element")]
def test_batch_non_object_elements(t : T?) {
    // §6: non-object elements get individual INVALID_REQUEST envelopes; batch shape preserved.
    let pb = parse_batch("[1, 2, 3]")
    t |> success(pb.is_batch, "is_batch=true")
    t |> equal(length(pb.requests), 3)
    t |> equal(pb.framing_error, "")
    for (req in pb.requests) {
        t |> success(!empty(req.error_envelope), "each scalar element rejected")
        t |> success(find(req.error_envelope, "-32600") >= 0, "INVALID_REQUEST code")
    }
}

// ===== PARSE_ERROR (-32700) =====

[test(description = "parse_request returns -32700 with a null id for garbage, empty and truncated input")]
def test_parse_error_malformed_json(t : T?) {
    t |> run("garbage text returns -32700 with id:null") @(t : T?) {
        let r = parse_request("not json at all")
        t |> success(!empty(r.error_envelope), "envelope built")
        t |> equal(r.method, "")
        t |> success(find(r.error_envelope, "-32700") >= 0, "parse-error code")
        t |> success(find(r.error_envelope, "\"id\":null") >= 0, "id defaults to null")
    }
    t |> run("empty input returns parse error") @(t : T?) {
        let r = parse_request("")
        t |> success(find(r.error_envelope, "-32700") >= 0, "parse-error code")
    }
    t |> run("truncated object returns parse error") @(t : T?) {
        let r = parse_request("\{\"method\":\"x\"")
        t |> success(find(r.error_envelope, "-32700") >= 0, "parse-error code")
    }
}

// ===== INVALID_REQUEST (-32600) — non-object body =====

[test(description = "parse_request returns -32600 for a number, string or null body")]
def test_invalid_request_non_object_body(t : T?) {
    t |> run("number body returns -32600") @(t : T?) {
        let r = parse_request("42")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
        t |> success(find(r.error_envelope, "not a JSON object") >= 0, "message names the rule")
    }
    t |> run("string body returns -32600") @(t : T?) {
        let r = parse_request("\"hello\"")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
    }
    t |> run("null body returns -32600") @(t : T?) {
        let r = parse_request("null")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
    }
}

// ===== INVALID_REQUEST (-32600) — missing/invalid method =====

[test(description = "parse_request returns -32600 for a missing or non-string method, keeping the id")]
def test_invalid_request_method(t : T?) {
    t |> run("missing method, id preserved") @(t : T?) {
        let r = parse_request("\{\"id\":7}")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
        t |> success(find(r.error_envelope, "\"id\":7") >= 0, "id preserved")
        t |> success(find(r.error_envelope, "missing or non-string method") >= 0, "message names the rule")
    }
    t |> run("non-string method") @(t : T?) {
        let r = parse_request("\{\"id\":1,\"method\":42}")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
        t |> success(find(r.error_envelope, "missing or non-string method") >= 0, "message names the rule")
    }
    t |> run("empty object (no method, no id) is NOT a notification") @(t : T?) {
        // A notification is a *well-formed* request without id. `{}` is missing
        // the required `method` member, so it must reach the wire with -32600.
        let r = parse_request("\{}")
        t |> success(!r.is_notification, "no method + no id is still NOT a notification")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
    }
}

// ===== INVALID_REQUEST (-32600) — invalid id types =====

[test(description = "parse_request rejects object, array and boolean ids with -32600 and a null id")]
def test_invalid_id_types(t : T?) {
    t |> run("object id rejected with id:null") @(t : T?) {
        let r = parse_request("\{\"id\":\{\"weird\":1},\"method\":\"x\"}")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
        t |> success(find(r.error_envelope, "\"id\":null") >= 0, "response id reset to null")
        t |> success(find(r.error_envelope, "id must be string, number, or null") >= 0, "message names the rule")
    }
    t |> run("array id rejected") @(t : T?) {
        let r = parse_request("\{\"id\":[1,2],\"method\":\"x\"}")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
    }
    t |> run("boolean id rejected") @(t : T?) {
        let r = parse_request("\{\"id\":true,\"method\":\"x\"}")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
    }
}

// ===== Notification semantics on error =====

[test(description = "malformed JSON and non-object bodies still get an error response and are not treated as notifications")]
def test_notification_error_suppression(t : T?) {
    t |> run("malformed JSON is NOT a notification (response emitted)") @(t : T?) {
        // Parse failures must produce -32700 — never silent — even though no id.
        let r = parse_request("garbage")
        t |> success(!empty(r.error_envelope), "envelope built")
        t |> success(!r.is_notification, "parse failure ≠ notification")
    }
    t |> run("non-object body is NOT a notification") @(t : T?) {
        let r = parse_request("42")
        t |> success(!r.is_notification, "non-object ≠ notification")
    }
}

// ===== parse_response — success cases =====

[test(description = "parse_response reads scalar and object results with integer, string and null ids")]
def test_parse_response_success(t : T?) {
    t |> run("scalar result, integer id") @(t : T?) {
        let r = parse_response("\{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":42}")
        t |> equal(r.parse_error, "")
        t |> success(r.is_success, "is_success=true")
        t |> equal(r.id_str, "1")
        t |> equal(r.result_json, "42")
        t |> equal(r.error_code, 0)
    }
    t |> run("object result") @(t : T?) {
        let r = parse_response("\{\"id\":2,\"result\":\{\"fps\":60}}")
        t |> success(r.is_success, "is_success=true")
        // result_json comes from write_json (pretty-printed); just confirm fields present
        t |> success(find(r.result_json, "\"fps\"") >= 0, "fps field in result")
        t |> success(find(r.result_json, "60") >= 0, "value in result")
    }
    t |> run("string id round-trips") @(t : T?) {
        let r = parse_response("\{\"id\":\"abc\",\"result\":null}")
        t |> equal(r.id_str, "\"abc\"")
        t |> success(r.is_success, "null result still counts as success")
    }
    t |> run("null id round-trips") @(t : T?) {
        let r = parse_response("\{\"id\":null,\"result\":1}")
        t |> equal(r.id_str, "null")
    }
}

// ===== parse_response — error cases =====

[test(description = "parse_response reads an error object with its code, message and optional data")]
def test_parse_response_error(t : T?) {
    t |> run("method-not-found error") @(t : T?) {
        let r = parse_response("\{\"id\":1,\"error\":\{\"code\":-32601,\"message\":\"method not found\"}}")
        t |> equal(r.parse_error, "")
        t |> success(!r.is_success, "is_success=false on error")
        t |> equal(r.error_code, -32601)
        t |> equal(r.error_msg, "method not found")
        t |> equal(r.error_data, "")
    }
    t |> run("error with data field") @(t : T?) {
        let r = parse_response("\{\"id\":1,\"error\":\{\"code\":-32603,\"message\":\"oops\",\"data\":\{\"trace\":\"foo\"}}}")
        t |> success(!r.is_success, "is_success=false")
        t |> equal(r.error_code, -32603)
        t |> success(find(r.error_data, "\"trace\"") >= 0, "data field captured")
    }
    t |> run("error with scalar data") @(t : T?) {
        let r = parse_response("\{\"id\":1,\"error\":\{\"code\":-1,\"message\":\"x\",\"data\":42}}")
        t |> equal(r.error_data, "42")
    }
}

// ===== parse_response — malformed wires =====

[test(description = "parse_response sets parse_error for non-JSON, non-object bodies and a response with neither result nor error")]
def test_parse_response_malformed(t : T?) {
    t |> run("non-JSON returns parse_error") @(t : T?) {
        let r = parse_response("garbage")
        t |> success(!empty(r.parse_error), "parse_error populated")
        t |> success(find(r.parse_error, "parse error") >= 0, "names the failure")
    }
    t |> run("non-object body returns parse_error") @(t : T?) {
        let r = parse_response("42")
        t |> success(!empty(r.parse_error), "parse_error populated")
    }
    t |> run("missing both result and error") @(t : T?) {
        let r = parse_response("\{\"id\":1}")
        t |> success(!empty(r.parse_error), "parse_error populated")
    }
}

// ===== parse_response_batch =====

[test(description = "parse_response_batch of one object reports is_batch false with one response")]
def test_parse_response_batch_single(t : T?) {
    let pb = parse_response_batch("\{\"id\":1,\"result\":true}")
    t |> success(!pb.is_batch, "single object → is_batch=false")
    t |> equal(length(pb.responses), 1)
    t |> success(pb.responses[0].is_success, "")
}

[test(description = "parse_response_batch of an array reports is_batch true with every response")]
def test_parse_response_batch_array(t : T?) {
    let pb = parse_response_batch("[\{\"id\":1,\"result\":1},\{\"id\":2,\"error\":\{\"code\":-1,\"message\":\"e\"}}]")
    t |> success(pb.is_batch, "array → is_batch=true")
    t |> equal(length(pb.responses), 2)
    t |> equal(pb.parse_error, "")
    t |> success(pb.responses[0].is_success, "first success")
    t |> success(!pb.responses[1].is_success, "second error")
    t |> equal(pb.responses[1].error_code, -1)
}

[test(description = "parse_response_batch of non-JSON sets parse_error and returns no responses")]
def test_parse_response_batch_parse_error(t : T?) {
    let pb = parse_response_batch("not json")
    t |> success(!pb.is_batch, "")
    t |> success(!empty(pb.parse_error), "parse_error populated")
    t |> success(empty(pb.responses), "no responses on top-level parse failure")
}
