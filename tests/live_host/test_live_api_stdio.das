options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/json
require daslib/json_boost
require live/live_api_stdio
require strings

// Tests cover the parse layer of live_api_stdio (parse_jsonrpc_request +
// envelope helpers). The full handle_jsonrpc_line call path goes through
// live_host's dispatch_command bridge, which only resolves under
// daslang-live; the demo in examples/daslive/hello_stdio/ is the
// integration smoke for that.

// --- envelope helpers ---

[test]
def test_envelope_response(t : T?) {
    t |> run("result envelope wraps id + result verbatim") @(t : T?) {
        let r = json_rpc_response("1", "true")
        t |> equal(r, "\{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":true}")
    }
    t |> run("result envelope embeds raw JSON object") @(t : T?) {
        let r = json_rpc_response("42", "\{\"fps\":60}")
        t |> equal(r, "\{\"jsonrpc\":\"2.0\",\"id\":42,\"result\":\{\"fps\":60}}")
    }
    t |> run("error envelope wraps code + message") @(t : T?) {
        let r = json_rpc_error("\"abc\"", -32601, "method not found")
        t |> equal(r, "\{\"jsonrpc\":\"2.0\",\"id\":\"abc\",\"error\":\{\"code\":-32601,\"message\":\"method not found\"}}")
    }
}

[test]
def test_serialize_id(t : T?) {
    t |> run("null id serializes to 'null'") @(t : T?) {
        t |> equal(serialize_id(null), "null")
    }
    t |> run("integer id round-trips") @(t : T?) {
        t |> equal(serialize_id(JV(7)), "7")
    }
    t |> run("string id keeps quoting") @(t : T?) {
        t |> equal(serialize_id(JV("abc")), "\"abc\"")
    }
}

// --- parse_jsonrpc_request: valid requests ---

[test]
def test_parse_valid(t : T?) {
    t |> run("method only, no params, no id") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.method, "status")
        t |> equal(r.id_str, "null")
        t |> equal(r.inner_json, "\{\"name\":\"status\",\"args\":null}")
    }
    t |> run("integer id passes through") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"jsonrpc\":\"2.0\",\"id\":42,\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.id_str, "42")
        t |> equal(r.method, "status")
    }
    t |> run("string id passes through quoted") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"id\":\"call-1\",\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.id_str, "\"call-1\"")
    }
    t |> run("params object is embedded in inner_json") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"id\":1,\"method\":\"set_color\",\"params\":\{\"r\":0.5}}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.method, "set_color")
        t |> success(find(r.inner_json, "\"name\":\"set_color\"") >= 0, "inner_json carries method as name")
        t |> success(find(r.inner_json, "\"args\":") >= 0, "inner_json carries args field")
        t |> success(find(r.inner_json, "\"r\"") >= 0, "args carry the r key from params")
    }
    t |> run("missing params becomes args:null") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"id\":1,\"method\":\"ping\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.inner_json, "\{\"name\":\"ping\",\"args\":null}")
    }
}

// --- parse_jsonrpc_request: notification detection (JSON-RPC 2.0 §4.1) ---

[test]
def test_parse_notification(t : T?) {
    t |> run("missing id flags notification") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"method\":\"shutdown\"}")
        t |> equal(r.error_envelope, "")
        t |> success(r.is_notification, "is_notification should be true when id is absent")
    }
    t |> run("present id flags non-notification") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"id\":1,\"method\":\"status\"}")
        t |> success(!r.is_notification, "is_notification should be false when id is present")
    }
    t |> run("explicit null id is still a normal request") @(t : T?) {
        // JSON-RPC 2.0 distinguishes "absent id" (notification) from
        // "explicit null id" (request expecting a response with id:null).
        let r = parse_jsonrpc_request("\{\"id\":null,\"method\":\"status\"}")
        t |> success(!r.is_notification, "explicit null id is NOT a notification per spec §4.1")
    }
    t |> run("malformed JSON is NOT a notification — envelope is emitted") @(t : T?) {
        // Parse failures must produce a -32700 response per JSON-RPC 2.0 §5.1;
        // suppressing them would let invalid clients hang waiting for a reply.
        let r = parse_jsonrpc_request("garbage")
        t |> success(!empty(r.error_envelope), "envelope built")
        t |> success(!r.is_notification, "parse failure is NOT a notification")
        t |> success(find(r.error_envelope, "-32700") >= 0, "parse-error code")
    }
    t |> run("non-object body is NOT a notification — envelope is emitted") @(t : T?) {
        let r = parse_jsonrpc_request("42")
        t |> success(!r.is_notification, "non-object is NOT a notification")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
    }
    t |> run("empty object (no id, no method) is NOT a notification") @(t : T?) {
        // A notification is a *well-formed* request that omits id. `{}` is missing
        // the required `method` member, so it's an invalid request that must
        // reach the wire with -32600 per JSON-RPC §5.1 — not silently swallowed.
        let r = parse_jsonrpc_request("\{}")
        t |> success(!r.is_notification, "missing method with no id is NOT a notification")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
        t |> success(find(r.error_envelope, "\"id\":null") >= 0, "id null when absent")
    }
    t |> run("non-string method with no id is NOT a notification") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"method\":42}")
        t |> success(!r.is_notification, "non-string method with no id is NOT a notification")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
    }
}

// --- parse_jsonrpc_request: id type validation (JSON-RPC 2.0 §4) ---

[test]
def test_parse_id_types(t : T?) {
    t |> run("object id is rejected with -32600 and id:null") @(t : T?) {
        // An object/array id would pretty-print across multiple lines through
        // write_json — breaks framing. Reject early with id:null.
        let r = parse_jsonrpc_request("\{\"id\":\{\"weird\":1},\"method\":\"status\"}")
        t |> success(!empty(r.error_envelope), "envelope built")
        t |> success(!r.is_notification, "invalid-id request is not a notification")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
        t |> success(find(r.error_envelope, "\"id\":null") >= 0, "response id reset to null")
        t |> success(find(r.error_envelope, "id must be string, number, or null") >= 0, "message names the constraint")
    }
    t |> run("array id is rejected") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"id\":[1,2],\"method\":\"status\"}")
        t |> success(!empty(r.error_envelope), "envelope built")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
    }
    t |> run("boolean id is rejected") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"id\":true,\"method\":\"status\"}")
        t |> success(!empty(r.error_envelope), "envelope built")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
    }
    t |> run("number id is accepted") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"id\":3.14,\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
    }
}

// --- Permissive jsonrpc field handling ---

[test]
def test_parse_permissive_jsonrpc(t : T?) {
    t |> run("missing jsonrpc field accepted (permissive subset)") @(t : T?) {
        // Strict JSON-RPC 2.0 would reject this with -32600 (jsonrpc:"2.0" is
        // required). The transport intentionally accepts it for clients that
        // omit the field, as documented.
        let r = parse_jsonrpc_request("\{\"id\":1,\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.method, "status")
    }
    t |> run("non-2.0 jsonrpc value accepted (permissive)") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"jsonrpc\":\"1.0\",\"id\":1,\"method\":\"status\"}")
        t |> equal(r.error_envelope, "")
        t |> equal(r.method, "status")
    }
}

// --- compact_json_whitespace: pretty-print stripping ---

[test]
def test_compact_json_whitespace(t : T?) {
    t |> run("strips newlines/tabs between tokens") @(t : T?) {
        let r = compact_json_whitespace("\{\n\t\"fps\" : 60,\n\t\"paused\" : false\n}")
        t |> equal(r, "\{\"fps\":60,\"paused\":false}")
    }
    t |> run("preserves whitespace inside string values") @(t : T?) {
        let r = compact_json_whitespace("\{\n\t\"msg\" : \"hello\\nworld with spaces\"\n}")
        t |> equal(r, "\{\"msg\":\"hello\\nworld with spaces\"}")
    }
    t |> run("preserves escaped quotes inside strings") @(t : T?) {
        let r = compact_json_whitespace("\{\n\t\"q\" : \"a\\\"b\\\"c\"\n}")
        t |> equal(r, "\{\"q\":\"a\\\"b\\\"c\"}")
    }
    t |> run("empty string returns empty") @(t : T?) {
        t |> equal(compact_json_whitespace(""), "")
    }
    t |> run("already-compact JSON is unchanged") @(t : T?) {
        let r = compact_json_whitespace("\{\"a\":1,\"b\":[1,2,3]}")
        t |> equal(r, "\{\"a\":1,\"b\":[1,2,3]}")
    }
}

// --- parse_jsonrpc_request: invalid requests ---

[test]
def test_parse_errors(t : T?) {
    t |> run("malformed JSON returns -32700 with null id") @(t : T?) {
        let r = parse_jsonrpc_request("not json at all")
        t |> success(!empty(r.error_envelope), "error_envelope populated")
        t |> equal(r.method, "")
        t |> equal(r.inner_json, "")
        t |> success(find(r.error_envelope, "-32700") >= 0, "envelope carries parse-error code")
        t |> success(find(r.error_envelope, "\"id\":null") >= 0, "id defaults to null on parse fail")
    }
    t |> run("empty input returns parse error") @(t : T?) {
        let r = parse_jsonrpc_request("")
        t |> success(!empty(r.error_envelope), "error_envelope populated")
        t |> success(find(r.error_envelope, "-32700") >= 0, "parse-error code")
    }
    t |> run("non-object body returns -32600") @(t : T?) {
        let r = parse_jsonrpc_request("42")
        t |> success(!empty(r.error_envelope), "error_envelope populated")
        t |> equal(r.method, "")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
        t |> success(find(r.error_envelope, "not a JSON object") >= 0, "message names the issue")
    }
    t |> run("missing method returns -32600 with id preserved") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"id\":7}")
        t |> success(!empty(r.error_envelope), "error_envelope populated")
        t |> equal(r.method, "")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
        t |> success(find(r.error_envelope, "\"id\":7") >= 0, "id from the request is preserved")
        t |> success(find(r.error_envelope, "missing or non-string method") >= 0, "message names the issue")
    }
    t |> run("non-string method returns -32600") @(t : T?) {
        let r = parse_jsonrpc_request("\{\"id\":1,\"method\":42}")
        t |> success(!empty(r.error_envelope), "error_envelope populated")
        t |> success(find(r.error_envelope, "-32600") >= 0, "invalid-request code")
        t |> success(find(r.error_envelope, "missing or non-string method") >= 0, "message names the issue")
    }
}
