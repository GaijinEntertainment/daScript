# dasHV — HTTP / WebSocket server + client (libhv)

Read this before writing or editing any `.das` that serves HTTP, streams SSE, makes outbound
HTTP requests, or tests a server. `dasHV` wraps [libhv](https://github.com/ithewei/libhv);
the high-level das layer is `dashv/dashv_boost`, the native bindings are the `dashv` module.
Worked example end-to-end: `utils/dasllama-server/` (an OpenAI-compatible server) and
`tests/dasHV/` (repo-only), both in the daslang repo.

```das
require dashv/dashv_boost public   // HvWebServer, HvWebSocketClient, with_http_request, get_body_bytes
require daslib/jobque_boost        // with_job_status / with_atomic32 (server threading, test harness)
require daslib/json_boost          // read_json / write_json / sprint_json for JSON bodies
require daslib/fio                 // sleep (the tick loop) — dashv_boost does not pull it in
```

## Server — subclass `HvWebServer`, register routes in `onInit`

```das
class MyServer : HvWebServer {
    def override onInit {
        GET("/hello") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("hi")
        }
        POST("/echo") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON(write_json(JV((got = string(req.body)))))
        }
    }
    def override onTick { }   // optional: periodic work on the tick thread
}

def run() {
    var server = new MyServer()
    server->init(port)          // or init_wss(port, httpsPort, certPath) for TLS
    server->start()
    while (!done) { server->tick(); sleep(1u) }   // tick pumps HTTP + WS events
    server->stop(); server->cleanup()
    unsafe { delete server }
}
```

- **Route methods** (all on the server class): `GET` `POST` `PUT` `DELETE` `PATCH` `HEAD` `ANY` and
  `SSE` take buffered handlers (`SSE` matches any method — use it when the whole event-stream body
  is built up front); `STREAM` takes the incremental `HttpResponseWriter` handler for live SSE /
  chunked. Static serving: `STATIC(path, dir)`, plus `set_document_root` / `set_home_page` /
  `set_index_of` / `set_error_page` / `allow_cors` / `set_bind_host`. Literal routes outrank
  `:param` routes outrank `ANY("*")` (trie router; pinned by `test_server_routes.das`).
- **Request access:** `string(req.body)`, `req.method`, `get_param(req, "id")` (path `:id` or query),
  `each_param(req) $(k, v) { }`, `get_header(req, "X-…")`, `each_header`, multipart via
  `each_form_field(req) $(name, content, filename) { }` + `save_form_file(req, name, path)`
  (binary-safe — multipart `content` is `const char*`, null-truncated; never read binary from it).
- **Buffered response** — the handler builds the whole body and returns an `http_status`:
  `resp |> JSON(body [, status])`, `resp |> TEXT_PLAIN(text)`, `REDIRECT`, `DATA`, `SERVE_FILE`;
  `set_header(resp, k, v)`, `set_content_type`. `resp.status_code`, `resp.content_length`.

## Streaming (SSE / chunked) — `STREAM` route + `HttpResponseWriter`

```das
STREAM("/events") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
    sse_event(server, writer, "{\"n\":1}", "")   // data, event-name ("" = unnamed)
    respond(server, writer, int(http_status.OK), "application/json", whole_body)  // buffered-through-writer
    close_writer(server, writer)
}
```

- **Writer ops are handle-first** — `respond` / `sse_event` / `write_chunked` / `end_headers` /
  `set_header` / `SERVE_FILE` / `close_writer` / `release_writer` / `is_writer_connected` all take
  the **server handle first**, then the `writer`. That is deliberate: generation runs on the tick
  thread, and the op posts the socket write to the connection's event loop
  (`adapter->loop()->runInLoop`) so writes marshal to the right loop.
- **Large or binary bodies must go through the writer rail.** The buffered path cannot raise
  libhv's per-connection write-buf cap (16MB) — a bigger body would get the connection closed
  MID-BODY, a truncated response after a clean 200 — so buffered `SERVE_FILE`/`DATA` REFUSE a
  body over ~15MB with a 500 naming the writer rail (fail closed, never truncate).
  `SERVE_FILE(server, writer, path)` reads the file on the connection loop (binary-safe, content
  type from the file name) and covers the body with the cap; `respond` covers its body the same
  way. Set extra headers first with `set_header(server, writer, k, v)` — they serialize when the
  terminal op writes. Writer-rail write failures are still async: the das call cannot return
  them; they land as `dasHV:` ERROR lines in libhv's log (`bin/libhv.*.log` by default — route
  with `DASLIVE_HV_LOG=stderr`), and `is_writer_connected` flips false afterwards.
- `is_writer_connected(server, writer)` — true while the writer's client connection is still open
  (false for null / unknown / released writers). The async write ops never report a dead peer, so
  a long-lived stream **polls this to evict abandoned clients** (see `openai_server.das`'s
  `reap_disconnected`).
- `sse_event(server, writer, data, "")` emits exact `data: <json>\n\n` framing. Terminate an SSE
  stream with a final `sse_event(server, writer, "[DONE]", "")` then `close_writer`.
- One `STREAM` route serves both streaming and non-streaming responses: stream with `sse_event`,
  or return the whole body once via `respond` — same async route.
- Terminal error on a stream: `respond(server, writer, int(http_status.BAD_REQUEST), "application/json", err)`.
- A retained `STREAM` writer inherits libhv's 75,000 ms idle keepalive timeout. For deferred work
  that can run longer, call `set_writer_keepalive_timeout(server, writer, timeout_ms)` before
  returning from the handler. Restore 75,000 ms immediately before the terminal `respond` or
  `close_writer`, so a reused HTTP connection does not keep the extended idle lifetime.

## HTTP client — `GET` / `POST` / `request`, response via a block

Client `GET`/`POST` are free functions (distinct from the server-class route methods — the
compiler picks by argument types). The response arrives in a trailing block as `HttpResponse?`:

```das
GET("http://127.0.0.1:8080/v1/models") <| $(resp) {
    if (resp != null && resp.status_code == http_status.OK) {
        var err : string
        let js = read_json(string(resp.body), err)
        // ...
    }
}
POST("http://…/v1/embeddings", request_json) <| $(resp) { }

// Full control (any method, headers, timeouts) via the builder:
with_http_request() <| $(var req) {
    req.method = http_method.POST
    req.url := "http://…/v1/chat/completions"
    set_header(req, "Authorization", "Bearer x")
    request(req) <| $(resp) { }
}
```

- `get_body_bytes(resp) : array<uint8>` extracts a binary body (empty if null / non-OK / no content).
- `get_header(resp, "Content-Type")`, `resp.status_code`, `string(resp.body)`.
- `url_encode(s)` percent-encodes a query-string component (RFC 3986 unreserved set kept
  verbatim) — use it on any value interpolated into a URL query.
- `req.timeout` / `req.connect_timeout` (seconds) on the builder request bound outbound calls —
  set both on anything a server's tick thread waits on.

## Testing — `with_test_server` (runs the server on its own thread/context)

`tests/dasHV/_dashv_test_common.das` (repo-only) provides the harness. It spawns the server on `new_thread()`
(**a separate context**), waits until it's up, runs your block with `base_url`, then stops + joins:

```das
require _dashv_test_common
with_test_server(type<MyServer>, PORT_X) $(base_url) {
    t |> run("GET /hello") @(t : T?) {
        GET("{base_url}/hello") <| $(resp) { t |> equal(string(resp.body), "hi") }
    }
}
```

- Give each test file a **unique port** (see the `PORT_*` constants) to avoid bind collisions.
- `with_test_server` hardcodes `new ServerType(); init; start; tick-loop`. If your server needs
  per-thread setup (module-global state, a job queue), it does **not** fit — write a small variant
  of the harness whose thread lambda does that setup first (see the per-context gotcha below).

## Gotchas

- **Keep native-retained route lambdas GC-visible.** `HvWebServer` stores every buffered and
  streaming route callback in das-visible arrays because libhv's native `std::function` copies are
  opaque to the das GC. Retain callbacks with `push` (copy/alias semantics), never `emplace`
  (move semantics would consume the handler before native route registration).
- **Module-global state is per-context.** A server started on `new_thread()` runs in its own
  context, so any `var g_… ` module global (loaded model, config, stop flag) must be set **inside
  the server thread** — a value set on the main thread is invisible to the handlers. Corollary:
  stop the server by POSTing a `/shutdown` route (which flips its own context's flag), not by
  calling a `request_stop()` from the main thread (that flips the main context's copy).
- **`options stack = 262144`** (or higher) in `main.das` when a handler drives a deep call chain
  (e.g. an LLM forward) under the tick thread + C++→das handler bridge — the default 16 KB
  overflows. Plain compute handlers are fine at the default.
- **`options fast_math = true`** for FP-lax inference/graphics tools; `_jit_fast_math` is a legacy
  alias — use `fast_math`.
- **Serialized by design** — the tick loop is single-threaded, so handlers run one at a time. Fine
  for a stateless request/response server; don't assume handler concurrency.
- `delete writer` / `delete server` / `delete req` (from `with_http_request`) need `unsafe`.
- JSON: build with `write_json(JV(...))` / `sprint_json(struct, false)`; parse with
  `read_json(string(body), err)` (returns `JsonValue?`, null on parse failure) — see `skills/daslang/references/json.md`.
- **JIT-only for heavy handlers** — an inference server must run under `-jit` (interpreted is far
  too slow); wrap generation in `with_job_que()` + the tool's job-queue setup so kernels thread.
