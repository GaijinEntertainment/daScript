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
Closed deferred writers remain leased until `close_writer`/`respond` releases them;
release every acquired writer. `is_writer_connected` reports peer disconnection.
Ordinary asynchronous HTTP/1 replies retain request order. While a reply is pending,
up to 64 KiB of subsequent socket data may wait; additional data closes the connection.
This pipeline bound is independent of the per-request body limit.

Override `onWsMessageFrame(channel, message, byte_count, opcode)` to receive complete
messages with an authoritative byte count and opcode, including embedded NUL bytes.
Its default implementation calls the existing `onWsMessage` callback. Copy borrowed
message/request data before retaining it beyond the callback; do not share script
objects with other contexts.

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
