---
slug: is-there-a-json-rpc-2-0-implementation-in-daslang-i-can-use-or-extend
title: Is there a JSON-RPC 2.0 implementation in daslang I can use or extend?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

A partial one — `modules/dasLiveHost/live/live_api_stdio.das` ships an 80%-compliant JSON-RPC 2.0 transport for live commands over stdin/stdout (newline-delimited framing). Reusable building blocks are public:

- `parse_jsonrpc_request(line) → ParsedJsonRpc` — pure parser; returns id/method/inner_json/error_envelope/is_notification
- `compact_json_whitespace(s) → string` — escape-quote-aware whitespace stripper (one-message-per-line guarantee)
- `json_rpc_response(id_json, result_json) → string`, `json_rpc_error(id_json, code, message) → string`
- `serialize_id(id_val) → string`

What's solidly spec-compliant: response envelope, id type validation (string/number/null only — objects/arrays/bools get -32600), `id:null` on parse failure, method-must-be-string, -32700 / -32600, notification semantics for well-formed requests, framing.

What's permissive: `jsonrpc:"2.0"` member is optional; non-"2.0" values are accepted. Documented in the module docstring and `doc/source/reference/utils/daslang_live.rst`.

What's missing if you want a real-deal daslib JSON-RPC server (gap list also in the live_api_stdio.das module docstring as of PR #2674):
- **Batch requests (§6)** — array-of-requests / array-of-responses. Currently rejected as -32600. Biggest single gap vs spec.
- **`params` type validation (§4.2)** — spec says MUST be Array or Object; we forward whatever's there. No -32602.
- **-32601 method not found / -32603 internal error** — `dispatch_command` returns its own error JSON shape inside `result` instead of a JSON-RPC `error` envelope. Wrapping would belong at the transport layer.
- **Optional `data` field on errors** — never emitted (genuinely optional per spec).

Tests live at `tests/live_host/test_live_api_stdio.das` (~40 cases across 8 [test] functions) and cover the parser layer in isolation; the integration path is covered by `examples/daslive/hello_stdio/`.

For a daslib promotion: lift `parse_jsonrpc_request` + `compact_json_whitespace` + envelope helpers into `daslib/jsonrpc.das`, add batch dispatch (top-level array → fan out → array response), validate `params`, and let callers wrap their own dispatch table with proper -32601 / -32603 envelope handling.

## Questions
- Is there a JSON-RPC 2.0 implementation in daslang I can use or extend?
