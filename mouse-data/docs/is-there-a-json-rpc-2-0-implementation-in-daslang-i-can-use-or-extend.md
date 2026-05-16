---
slug: is-there-a-json-rpc-2-0-implementation-in-daslang-i-can-use-or-extend
title: Is there a JSON-RPC 2.0 implementation in daslang I can use or extend?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

Yes — **`daslib/jsonrpc`** is the canonical transport-agnostic JSON-RPC 2.0 implementation. `require daslib/jsonrpc` and you get:

- **Envelope builders:** `response(id, result_json)`, `error(id, code, message)`, `error_with_data(id, code, message, data_json)`, `serialize_id(JsonValue?)`.
- **Server-side parsers:** `parse_request(line, strict=false) → ParsedRequest`, `parse_batch(line, strict=false) → ParsedBatch` (handles §6 batch, empty array, top-level malformed JSON).
- **Client-side builders:** `make_request(method, params_json, id : int|string)`, `make_notification(method, params_json)`, `make_batch(messages)`.
- **Client-side response parsers:** `parse_response(line) → ParsedResponse`, `parse_response_batch(line) → ParsedResponseBatch`.
- **High-level convenience:** `dispatch_line(line, strict, dispatcher_block) → string` — handles parsing, envelope wrapping, batch fan-out, notification suppression in one call.
- **Standard error code constants:** `PARSE_ERROR (-32700)`, `INVALID_REQUEST (-32600)`, `METHOD_NOT_FOUND (-32601)`, `INVALID_PARAMS (-32602)`, `INTERNAL_ERROR (-32603)`.
- **Framing helper:** `compact_json_whitespace(s)` — escape-aware whitespace stripper for newline-framed wires.

Spec compliance: §4 (request shape, id type, params type), §5.1 (error codes), §6 (batch), notification semantics. Permissive default — `jsonrpc:"2.0"` field is optional; pass `strict=true` for full §4 enforcement. Tests live under `tests/jsonrpc/` (~120 [test] cases covering envelope, parser, batch, strict mode, sending API, round-trip).

In-tree consumers:
- `modules/dasLiveHost/live/live_api_stdio.das` — JSON-RPC 2.0 stdio transport for live commands.
- `utils/mcp/protocol.das` — the daslang MCP server.

For runnable examples, see `examples/mcp/echo/` (a minimal client+server pair) and the `tutorials/jsonrpc/` walkthrough.

History: prior to PR introducing `daslib/jsonrpc` (May 2026), the same logic was hand-rolled twice — once in `live_api_stdio.das`, once in `utils/mcp/protocol.das`. The library extracted the shared bits; both transports now `require daslib/jsonrpc` and inherit batch support for free.

## Questions
- Is there a JSON-RPC 2.0 implementation in daslang I can use or extend?
- How do I parse a JSON-RPC request in daslang?
- How do I send a JSON-RPC batch?
- Where are the JSON-RPC error code constants?
