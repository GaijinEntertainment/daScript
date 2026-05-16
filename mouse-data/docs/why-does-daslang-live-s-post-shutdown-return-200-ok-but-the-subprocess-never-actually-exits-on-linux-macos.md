---
slug: why-does-daslang-live-s-post-shutdown-return-200-ok-but-the-subprocess-never-actually-exits-on-linux-macos
title: Why does daslang-live's POST /shutdown return 200 OK but the subprocess never actually exits on Linux/macOS?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

**Root cause is in vendored libhv (v1.3.4), not daslang.** `live_api.das` registers an `ANY("*")` catch-all alongside specific routes like `GET /status` and `POST /shutdown`. libhv's `Any(path)` (`include/hv/HttpService.h:268-277`) expands internally to one `Handle("METHOD", path, h)` per HTTP verb on the same path key.

libhv stores all `(path -> method_handlers)` pairs in `pathHandlers`, declared as **`std::unordered_map<std::string, ...>`** at `include/hv/HttpService.h:105`. `HttpService::GetRoute` (`http/server/HttpService.cpp:72-127`) iterates that map in **container order** and takes the first wildcard or path match it finds.

Because `std::unordered_map`'s iteration order is implementation- and bucket-defined, `"*"` happens to enumerate BEFORE specific paths like `/status` on Linux libstdc++ (deterministic), and intermittently on Windows MSVC depending on rehash timing. When that happens, every request — including `POST /shutdown` — hits the wildcard handler, which returns the help JSON (200 OK). The real `/shutdown` handler that calls `request_exit()` is never invoked. The main loop spins forever, and the parent times out the subprocess with `popen_exit_code = DAS_POPEN_TIMEOUT = 0x7FFFFF01`.

**How to confirm you're hitting this:** while the daslang-live subprocess is running, `curl -s http://127.0.0.1:9090/status`. If the body contains `"endpoints"` (the help JSON), the wildcard is winning. If it returns real status JSON with `"has_error"` / `"paused"` / `"fps"` fields, routing works.

**Workaround (landed as daslang PR #2688, `bbatkin/live-api-drop-any-catchall`):** drop `ANY("*")` from `live_api.das` and serve the help JSON from a specific `GET("/")` handler instead. `/` is an exact-match path, so it never collides with `/status` etc. — the ordering hazard is gone regardless of how libhv stores routes. Unknown paths return libhv's default 404; programmatic clients (playwright, MCP `live_*` tools) never relied on the catch-all.

**Follow-up work (deferred until libhv-side fix lands):**
1. File the libhv upstream issue — unreported per 2026-05-16 web research; we'd be first.
2. Wire libhv's built-in `errorHandler` hook through dasHV (`WebServer_Adapter` setter), rewrite `live_api.das` to drop the `GET("/")` workaround and use `errorHandler` for the help fallback. That's the idiomatic shape on libhv's terms.
3. Address Copilot review comments on PR #2688 as part of that rewrite: (a) `live_api.das:25` module-level `//!` endpoint list should mention `GET /`; (b) `live_api.das:217` "curl :9090/" example — actually valid curl shorthand for localhost, but rephrasing to `curl http://localhost:9090/` is friendlier.
4. Consider a lint rule against `ANY("*")` registered alongside specific paths.

**Upstream fix status (researched 2026-05-16):**
- **Unreported** — exhaustive search of `ithewei/libhv` issues for `route|wildcard|Any|GetRoute|HttpService|pathHandlers` returned zero hits on this bug. We'd be the first to file.
- **Not fixed in master** — libhv's `http/server/HttpService.cpp` last touched 2023-07-29 (before v1.3.4); same `std::unordered_map<std::string, ...>` for `pathHandlers` at `http/server/HttpService.h:107`; same first-match-wins loop in `GetRoute` at line ~72. v1.3.4 IS the latest release (2025-10-25).
- **Known wart upstream**: `docs/PLAN.md` lists "Path router: optimized matching via trie?" as an open question — maintainers know the router is suboptimal, just haven't prioritized it.
- **Industry comparison**: Crow uses a trie; cpp-httplib preserves registration order so static paths always precede regex catch-alls. This is a libhv-specific defect, not industry norm.
- **Documented behavior**: libhv docs (`README.md`, `docs/API.md`, `docs/cn/HttpServer.md`) say NOTHING about route precedence or wildcard semantics. Behavior is implementation-defined.

**Better script-side option (not yet exercised): `errorHandler` hook.** libhv has a built-in fallback handler that fires after the processor chain runs and status ≥ 400 with empty body (`http/server/HttpService.h:133` + `http/server/HttpHandler.cpp:476-486`). Official example wiring at `examples/httpd/router.cpp:15` + `examples/httpd/handler.cpp:46`. Drop `ANY("*")`, assign `errorHandler` instead — `/status` / `/shutdown` win deterministically, everything else falls through to the help dump. **dasHV does NOT currently expose `errorHandler`**, so this needs C++ glue (a setter on `WebServer_Adapter`) before live_api can use it. PR #2688's `GET("/")` workaround is the interim fix.

**Also worth knowing**: official libhv examples never register bare `Any("*")` alongside specific paths — they use prefix wildcards like `GET("/wildcard*", ...)` (see `examples/httpd/router.cpp:70`). The maintainers don't exercise our usage pattern, so the bug has presumably never surfaced for upstream users.

**Why Windows seems to work most of the time:** MSVC's `std::unordered_map` bucket layout for the specific paths daslang-live registers happens to enumerate specific paths before `"*"`. It's not a guarantee — different route counts (e.g. adding more endpoints) trigger rehashes and can flip the order. Not "Windows-correct, Linux-broken" — both are subject to the same hazard.

**Symptom to watch for in CI logs:** `popen_exit_code=2147483393` (0x7FFFFF01 = `DAS_POPEN_TIMEOUT`) on POSIX, with the playwright drain step taking the full `DEFAULT_TEST_TIMEOUT_SEC` (120s default). Subprocess gets SIGKILLed at the watchdog.

## Questions
- Why does daslang-live's POST /shutdown return 200 OK but the subprocess never actually exits on Linux/macOS?
