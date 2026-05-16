# examples/mcp/echo

A minimal JSON-RPC 2.0 client + server pair built on
[daslib/jsonrpc](../../../daslib/jsonrpc.das) — the canonical bidirectional
JSON-RPC library in daslang.

The server implements three toy methods (`echo`, `add`, `status`) over
newline-delimited stdio JSON-RPC. The client demonstrates the outgoing
side: request/notification/batch builders, response parsers.

This is a teaching example — for the real production usage, see
[modules/dasLiveHost/live/live_api_stdio.das](../../../modules/dasLiveHost/live/live_api_stdio.das)
(live-command server) and [utils/mcp/protocol.das](../../../utils/mcp/protocol.das)
(MCP server).

## Run

The server reads newline-delimited JSON-RPC from stdin and writes responses
to stdout:

```sh
echo '{"jsonrpc":"2.0","id":1,"method":"echo","params":["hello"]}' | daslang server.das
# → {"jsonrpc":"2.0","id":1,"result":["hello"]}

echo '{"id":2,"method":"add","params":{"a":7,"b":35}}' | daslang server.das
# → {"jsonrpc":"2.0","id":2,"result":42}
```

A `§6` batch — multiple requests + a notification in one message — comes back
as a JSON array of responses, with the notification suppressed:

```sh
echo '[{"id":1,"method":"add","params":{"a":1,"b":2}},{"method":"notif"},{"id":2,"method":"status"}]' \
    | daslang server.das
# → [{"jsonrpc":"2.0","id":1,"result":3},{"jsonrpc":"2.0","id":2,"result":"ok"}]
```

A pure notification (no `id`) produces no output:

```sh
echo '{"method":"echo","params":["fire-and-forget"]}' | daslang server.das
# → (no output)
```

## Client modes

The client uses [daslib/clargs](../../../daslib/clargs.das) for argument
parsing. Daslang's script host forwards args after `--`.

**Demo mode** (default) — self-contained: builds sample wires, dispatches
them inline, parses sample responses:

```sh
daslang client.das
```

**Emit mode** — print sample requests to stdout (pipe into the server for a
real round-trip):

```sh
daslang client.das -- --mode=Emit | daslang server.das
```

## API surface used

Server-side:
- `jsonrpc::dispatch_line(line, strict)` — high-level: handles parsing,
  envelope wrapping, batch fan-out, notification suppression. Takes a
  dispatcher block `(method, params_json) → result_json`.
- `jsonrpc::compact_json_whitespace(s)` — flattens pretty-printed
  `write_json` output to a single line for newline-framed wires.

Client-side:
- `make_request(method, params_json, id : int | string)` — outgoing request.
- `make_notification(method, params_json)` — fire-and-forget.
- `make_batch(messages)` — wrap N pre-built wires as a §6 batch.
- `parse_response(line)` — server response → `ParsedResponse {is_success,
  result_json, error_code, error_msg, error_data}`.
- `parse_response_batch(line)` — same for §6 batched responses.
