# dasHV - bindings for HV - cross platform network library

## PREREQUISITES:
- **OpenSSL** (works with `3.5.1`)

## Installation:

### Linux (Debian/Ubuntu):
```bash
sudo apt update
sudo apt install openssl=3.*
```

### macOS (Homebrew):
```bash
brew install openssl@3
```

### Windows:
Using official installer:
- For x86-64 download 3.5.1 installer from https://slproweb.com/download/Win64OpenSSL-3_5_1.exe
- For x86 download 3.5.1 installer from https://slproweb.com/download/Win32OpenSSL-3_5_1.exe

Or via vcpkg:
```powershell
vcpkg install openssl:x64-windows --version=3.5.1 # for x86-64
vcpkg install openssl:x86-windows --version=3.5.1 # for x86
```

If OpenSSL is missing on Windows it will be built from sources.

*Troubleshooting:*
- OpenSSL build requires Strawberry Perl
- Under Ninja you may need to run under VS Developer Command Prompt to ensure all variables are set for OpenSSL build

## Bounded servers and deferred responses

Call `HvWebServer.set_limits(http_body_bytes, websocket_message_bytes,
pending_events, pending_bytes)` before `start`. All four values must be positive;
configuration changes while running return false. Without this call, legacy parser
and event-queue limits remain unchanged. Body/message limits are enforced before
allocation, including chunked HTTP and fragmented WebSocket messages. HTTP body
violations return 413; exhausted request admission returns 503. WebSocket message
violations close the connection without delivering a partial message.

The event budget includes callbacks currently executing, not only queued callbacks.
Deferred writers and admitted/pending WebSocket channels also have count bounds.
Deferred writers are released when the peer disconnects. Complete a live writer
with `close_writer`/`respond`, or transfer it to `SERVE_FILE_STREAM`.
`is_writer_connected` reports whether that writer is still admitted.
Ordinary asynchronous HTTP/1 replies retain request order. While a reply is pending,
up to 64 KiB of subsequent socket data may wait; additional data closes the connection.
This pipeline bound is independent of the per-request body limit.

Override `onWsMessageFrame(channel, message, byte_count, opcode)` to receive complete
messages with an authoritative byte count and opcode, including embedded NUL bytes.
Its default implementation calls the existing `onWsMessage` callback. Copy borrowed
message/request data before retaining it beyond the callback; do not share script
objects with other contexts.

## Bounded file responses

From a `STREAM` handler, call
`SERVE_FILE_STREAM(server, writer, filepath, max_bytes)`. The byte limit must be
positive. Zero return means the operation accepted ownership; a negative return
means an invalid server/writer/argument and leaves the caller responsible for any
still-live response. After acceptance, do not issue other operations on that writer.

The native event loop opens a regular file through the UTF-8 file API, checks its
size, and transfers it using a 64 KiB source buffer. It reads another chunk only
after the preceding write drains. OS/TLS buffers are separate from that bound.
Caller headers are retained, except transfer framing is set to the actual file size.
Without a content type, the response uses `application/octet-stream`. Missing or
non-regular files return 404; an oversized file returns 413. An error after headers
closes the connection instead of completing a truncated body as a successful reply.
HEAD returns the same file length as GET without reading or sending the body.
Empty files and keepalive/pipelined requests retain correct response framing.

Completion, peer disconnect and server shutdown close the file before releasing writer
admission. This operation does not replace application authentication or path policy;
authorize before calling it and do not construct trusted paths from unchecked input.
The existing writer `SERVE_FILE` remains the buffered helper.

## WebSocket admission before upgrade

`WEBSOCKET_UPGRADE(timeout_ms, handler)` installs one admission handler before server
start. The handler runs on the script tick thread and receives the HTTP request and a
`WebSocketAdmission` ticket. It can defer a decision while an isolated worker checks
credentials. No WebSocket channel or 101 response exists before acceptance.

- `accept_websocket(ticket, protocol)` selects a protocol offered by the client.
- `reject_websocket(ticket, status)` accepts an HTTP status from 400 through 599.
- Both return zero when the decision is submitted; nonzero means it was rejected.
- `is_alive(ticket)` is false after a decision, timeout, disconnect or server shutdown.
- Tickets are generation-checked and single-use. Keep the ticket, not a request/writer
  pointer, while waiting. Clone any required request strings in the owning context.
- The timeout is 1–60000 milliseconds and runs independently of application ticking.
  A client must wait for the 101 response before sending WebSocket frames.

The application still owns origin, authentication, authorization and protocol policy.
Without an admission handler, existing upgrade behavior is preserved.

## Verified outbound HTTPS

Native OpenSSL client defaults verify the certificate chain and DNS/IP identity.
Default trust uses OpenSSL trust paths plus platform roots on macOS and Windows.
An explicit CA file uses that trust file instead. Server-side TLS contexts are not
reused implicitly by clients. Untrusted self-signed endpoints therefore require an
explicit trust configuration; disabling verification is not the default.

`request_checked(request, ca_file, max_response_bytes, block)` performs a synchronous
HTTPS request with a separate verified TLS context. An empty CA filename selects
system trust. It rejects HTTP URLs, user-info URLs, URLs over 8192 bytes or containing
raw spaces or ASCII control bytes, response limits outside 1–16777216 bytes, and timeouts outside 1–120 seconds.
Redirects, automatic retries and request proxies are disabled for this operation.
The body limit is applied before response allocation. Truncated responses fail.

The return value is zero for transport success, otherwise a libhv error code. The
callback runs only on transport success, including non-2xx HTTP responses; inspect
`response.status_code` yourself. The response is borrowed for the callback duration.
Use `strings::to_bytes(response.body)` to copy binary bodies without losing NUL bytes.
Do not retain the response pointer. This API does not validate provider JSON or claims.

Ordinary native synchronous and asynchronous requests retain redirects, bounded to five hops, but do
not follow an HTTPS-to-HTTP downgrade; cross-origin redirects remove authorization
and cookie headers. Pooled asynchronous connections are separated by scheme,
hostname, port and resolved address. Services handling credentials should prefer `request_checked`
and explicitly decide which responses and destinations are acceptable.

## Request logging

`set_access_log(server, false)` disables libhv access logging before start; changing
it while running returns false. Raw URL, header and proxy destination values are
excluded from the patched low-level diagnostic paths. Lifecycle/error diagnostics
remain available. Application logs, request bodies and response handling still need
an explicit privacy policy; this switch does not implement one.

## Verification

Run `bin/daslang dastest/dastest.das -- --test tests/dasHV`. The test-enabled build
creates `bin/hv_tls_fixture`, which generates temporary local test certificates.
TLS tests do not require public network access or installed test roots. Raw-wire
and subprocess tests live in `tests/dasHV/no_aot`; the remaining tests also join AOT.
The CTest cases `hv_parser_limits` and `dashv_patch_libhv` check parser limits and
repeatable application of patches to the pinned libhv source excerpts.

## Per-connection WebSocket budgets

`set_connection_limits(server, pending_messages, pending_bytes, write_buffer_bytes)`
sets additional per-WebSocket budgets before the server starts. All values must be
positive; attempts to change them after `start` return false. Pending message count
and payload bytes include the batch currently draining on the owner thread. A peer
that exceeds either limit is closed without consuming another peer's allowance.
These limits supplement, rather than replace, the global pending queue budgets.
The write limit bounds libhv's unsent socket buffer; it is not a maximum message
size or a bound on bytes already accepted by the operating system. A full buffer
refuses the write and closes the connection. Leaving this configuration unset keeps
the existing behavior.

## Opened WebSocket request

Override `onWsOpenRequest(channel, request)` to read the handshake request associated
with a newly opened channel. The adapter delivers a request snapshot on the owner's
tick thread and charges its headers/body to the pending-byte budget. The request is
borrowed for this callback only; clone strings needed after it returns.

An override replaces `onWsOpen` for that server. Servers which do not override it
retain their existing `onWsOpen(channel, url)` callback. Admission decisions still
belong in `WEBSOCKET_UPGRADE`, before the HTTP 101 response. Rebuild the native
module and generated/AOT consumers when updating the callback interface.
