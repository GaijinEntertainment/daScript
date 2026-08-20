# Captured fixtures for the control-page suite

Every JSON/SSE file here was captured from a REAL dasllama-server run — never
hand-written — so the page is tested against the wire shapes the server
actually emits. If an endpoint's schema changes, re-capture rather than edit.

## Regenerating

Start a roster server on a free port (any two small GGUFs; `smol` must be the
default entry, `tiny` a `backend = "cpu"` second slot — models-panel specs
key on slot names):

```sh
bin/daslang -jit utils/dasllama-server/main.das -- --config capture-server.toml
```

with `capture-server.toml`:

```toml
model = "<models>/SmolLM2-135M-Instruct-Q8_0.gguf"
port = 18132
streams = 4
max_tokens = 64

[[models]]
name = "smol"
path = "<models>/SmolLM2-135M-Instruct-Q8_0.gguf"
default = true

[[models]]
name = "tiny"
path = "<models>/tinyllama-1.1b-chat-v1.0.Q8_0.gguf"
backend = "cpu"
```

Then capture (`B=http://127.0.0.1:18132`):

| File | Command |
|---|---|
| `stats_multi.json` | `curl $B/v1/stats` (idle) |
| `stats.json` | `stats_multi.json` with `models` cut to the active slot (a single-model server serves the same shape with one entry) |
| `streams_idle.json` | `curl $B/v1/streams` (idle) |
| `streams_active.json` | fire 2 concurrent chats (`max_tokens: 128`), then `curl $B/v1/streams` mid-flight; re-poll until one stream is `decoding`. Re-send one finished prompt first so `cache` carries a chain |
| `config.json` | `curl $B/config` |
| `exchange.json` | `curl $B/exchange` (a box with a fresh local tune → the local-unshared state) |
| `bench_idle.json` / `bench_failed.json` | `curl $B/bench` before / after a run. An in-tree server ALWAYS fails the run (lcpp_bench refuses `-jit` script runs — it measures released exes only), which is exactly the state `bench_failed` pins. A result-carrying `bench_done.json` must be captured from a deployed bundle |
| `exchange_matches.json` (hits) | run the test_exchange_client-style fake exchange on 18131, launch the server with `DASLLAMA_EXCHANGE_URL=http://127.0.0.1:18131`, then `curl $B/exchange/matches` — the hits pass through the server's real transform. `exchange_matches_unreachable.json` = same call with nothing on 18131 |
| `images.json` | `curl $B/v1/images` |
| `activate_response.json` | `curl -X POST $B/v1/models/activate -d '{"model":"tiny"}'` (idle server) |
| `sse_chat.txt` | `curl -N $B/v1/chat/completions -d '{"messages":[{"role":"user","content":"hi"}],"stream":true,"max_tokens":24}'` — raw bytes, keep the exact framing |
| `sse_think.txt` | same against a thinking model, or a capture that carries `delta.reasoning_content` chunks |

`sse_expected.json` pairs the SSE captures with the text the page must render:
`{content, think, think_content}` = the concatenated `delta.content` of
`sse_chat.txt`, and the concatenated `delta.reasoning_content` /
`delta.content` of `sse_think.txt`. Update it whenever the SSE captures move.

Catalog/setup fixtures (same server, booted with NO model for the setup ones):

| File | Command |
|---|---|
| `stats_setup.json` | `curl $B/v1/stats` on a no-model (setup mode) boot |
| `catalog_idle.json` | `curl $B/catalog` with at least one entry already downloaded |
| `catalog_downloading.json` | start a download (`POST $B/catalog/download`), then `curl $B/catalog` mid-flight |
| `catalog_done.json` | `curl $B/catalog` after downloads landed (from a serving boot) |
| `catalog_refusal.json` | `POST $B/catalog/download` for a second entry while one runs (the 409 body) |

After capture, mechanically normalize machine-local paths (model dirs →
`D:/models`, the capture TOML path → `D:/models/dasllama-server.toml`, the capture user's
home → `C:/Users/user`) —
paths are the ONE permitted edit; every other byte stays as served. The
hardware/box identity lines stay as captured.
