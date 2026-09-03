# Captured fixtures for the control-page suite

Every file here - JSON, SSE, and the one captured WAV - came off a REAL
dasllama-server run, never hand-written, so the page is tested against the wire
shapes the server actually emits. If an endpoint's schema changes, re-capture
rather than edit.

## Regenerating

Start a roster server on a free port (any two small GGUFs; `smol` must be the
default entry, `tiny` a `backend = "cpu"` second slot - models-panel specs
key on slot names). Put the two GGUFs (or symlinks to them) in a scratch
directory and point the config at that: `/config`'s `status.model_files`
lists the served model's whole directory, and a personal model library does
not belong in a fixture.

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
| `exchange.json` | `curl $B/exchange` (a box with a fresh local tune -> the local-unshared state) |
| `bench_idle.json` / `bench_failed.json` | `curl $B/bench` before / after a run. An in-tree server ALWAYS fails the run (lcpp_bench refuses `-jit` script runs - it measures released exes only), which is exactly the state `bench_failed` pins. A result-carrying `bench_done.json` must be captured from a deployed bundle |
| `exchange_matches.json` (hits) | run the test_exchange_client-style fake exchange on 18131, launch the server with `DASLLAMA_EXCHANGE_URL=http://127.0.0.1:18131`, then `curl $B/exchange/matches` - the hits pass through the server's real transform. `exchange_matches_unreachable.json` = same call with nothing on 18131 |
| `images.json` | `curl $B/v1/images` |
| `activate_response.json` | `curl -X POST $B/v1/models/activate -d '{"model":"tiny"}'` (idle server) |
| `sse_chat.txt` | `curl -N $B/v1/chat/completions -d '{"messages":[{"role":"user","content":"hi"}],"stream":true,"max_tokens":24}'` - raw bytes, keep the exact framing |
| `sse_think.txt` | same against a thinking model, or a capture that carries `delta.reasoning_content` chunks |

Speech fixtures (the same server, restarted with
`--tts <models>/kitten-nano.gguf` beside its `tts_g2p.bin` + `tts_postag.bin`;
`S=$B/v1/audio/speech`):

| File | Command |
|---|---|
| `stats_tts.json` | `curl $B/v1/stats` after a synthesis or two - the `tts` block (voices, sample rate, lane) rides the full stats shape |
| `config_tts.json` | `curl $B/config` on that same boot - the `tts` path and `tts_lane` as the speech studio's offer card reads them |
| `speech.wav` | `curl -X POST $S -d '{"input":"Hi.","voice":"expr-voice-2-f","response_format":"wav"}' -o speech.wav` - raw bytes, keep the exact RIFF header. Say ONE short word: the answer is uncompressed 16-bit PCM, so every second costs ~48 KB |
| `speech_error.json` | `curl -X POST $S -d '{"input":"Hi.","response_format":"mp3"}'` (the declined-format 400 body) |

`sse_expected.json` pairs the SSE captures with the text the page must render:
`{content, think, think_content}` = the concatenated `delta.content` of
`sse_chat.txt`, and the concatenated `delta.reasoning_content` /
`delta.content` of `sse_think.txt`. Update it whenever the SSE captures move.

Catalog/setup fixtures (same server, booted with NO model for the setup ones):

| File | Command |
|---|---|
| `stats_setup.json` | `curl $B/v1/stats` on a no-model (setup mode) boot |
| `catalog_idle.json` | `curl $B/catalog` on a boot whose `DASLLAMA_MODELS_DIR` names a scratch dir holding SYMLINKS to exactly what the fixture must read present: at least one catalog entry, plus `kitten-nano.gguf` and both `tts_g2p.bin` / `tts_postag.bin`, so the speech card's enable state is captured rather than derived. Symlink, never copy - a personal model library does not belong in a fixture, and neither does a second copy of one |
| `catalog_empty.json` | the same call with `DASLLAMA_MODELS_DIR` on an EMPTY scratch dir: every entry, the ASR tower and the whole speech set absent - the document the download offers are read from |
| `catalog_downloading.json` + `catalog_done.json` | `bin/daslang -jit utils/dasllama-server/tests/fixtures/capture_catalog.das` (repo root) - boots a setup server on a scratch dir, downloads the smallest card FOR REAL, snapshots `/catalog` mid-flight and after it lands, normalizes paths, deletes the scratch download |
| `catalog_refusal.json` | `POST $B/catalog/download` for a second entry while one runs (the 409 body) |

After capture, mechanically normalize machine-local paths (model dirs ->
`D:/models`, the capture TOML path -> `D:/models/dasllama-server.toml`, the capture user's
home -> `C:/Users/user`; a `/catalog` capture's scratch models dir maps to
`C:\Users\user\.dasllama\models`, the home-relative default those fixtures record, exactly as
`capture_catalog.das` does it) -
paths are the ONE permitted edit; every other byte stays as served. The
hardware/box identity lines stay as captured.
