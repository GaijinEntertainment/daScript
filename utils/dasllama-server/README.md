# dasllama-server — an OpenAI-compatible HTTP server

A drop-in OpenAI-compatible server for [dasLLAMA](../../modules/dasLLAMA) CPU inference, written
entirely in daslang over the public dasLLAMA facade + the `dasHV` HTTP layer. Point any OpenAI
client (opencode, Open WebUI, the `llm` CLI, the `openai` Python SDK, …) at `http://127.0.0.1:<port>/v1`.

It reaches **only** public facade verbs (`load_model` / `create_chat_renderer` / `add_user` /
`render_assistant` / `render_turn` / `eval_batch` via `llm_scheduler` / `transcribe` / `embed`) —
that is the point: the server is the acceptance test for the API rework. If it builds with no
reach into engine internals, the facade is complete.

## Run

```sh
bin/daslang -jit utils/dasllama-server/main.das -- --model <model.gguf> [--port 8080] [--quant q8] \
                                                    [--asr <asr.bin>] [--mmproj <mmproj.gguf>] [--ctx 4096] \
                                                    [--streams 4] [--chunk 64] [--page-rows 64] [--prefix N]
```

Run under `-jit` — interpreted inference is far too slow. Flags:

| Flag | Short | Default | Meaning |
|---|---|---|---|
| `--config` | `-c` | *auto* | TOML config file; keys mirror the long flag names, explicit CLI flags override. Without the flag, a `dasllama-server.toml` in the cwd or next to the program loads automatically |
| `--model` | `-m` | *(required)* | GGUF model to serve (here or in `--config`) |
| `--port` | `-p` | `8080` | Listen port |
| `--quant` | `-q` | `q8` | Weight quantization: `fp32` \| `q8` \| `q4` — plus the loader's file-format spellings `q4_k` \| `q5_k` \| `q6_k` \| `mxfp4` \| `f16` \| `bf16` (all serve on the `q8` kquant-native tier) |
| `--kv-dtype` | — | `f16` | KV-cache codec: `f32` \| `f16` \| `q8_0` \| `tq4` (rotated 4-bit; needs pow2 head_size) |
| `--asr` | `-a` | — | ASR model (whisper/parakeet/qwen3-asr) — enables the `/v1/audio/*` routes |
| `--mmproj` | — | — | mmproj GGUF for the Qwen3-ASR route (paired with `--asr`) |
| `--ctx` | — | *model* | Context-length cap in tokens (default: the model's trained `context_length`; set it to bound `--flat` KV or trim RAM) |
| `--max-tokens` | — | `256` | Default reply token budget when a request omits `max_tokens` (clamped to `--ctx` per request) |
| `--streams` | `-s` | `4` | Max concurrent generation streams |
| `--threads` | `-t` | `16` | Worker-lane cap for the matmul dispatch (`-1` = all cores) — decode is bandwidth-bound, so an uncapped dispatch just fights the rest of the box |
| `--chunk` | — | `64` | Prefill quantum in tokens — decode stalls at most this per tick |
| `--page-rows` | — | `64` | KV page size in positions for paged serving |
| `--prefix` | — | *auto* | Prefix-cache retention cap in pages (auto: one full context per stream; `-1` = unbounded) |
| `--flat` | — | — | Flat preallocated KV sessions — disables paged serving and the prefix cache |
| `--mtp` | — | — | MTP/NextN self-speculative decode for greedy requests (`temperature: 0`, no repetition penalty) — needs a model with an in-file NextN head (the `-MTP-` GGUFs). Output-invariant; ~2× decode on the dense qwen35/qwen3.6 models, ~nothing on the MoEs. `/v1/stats` reports `mtp_drafted`/`mtp_accepted` |
| `--help` | `-?` | — | Show help and exit |

A config file replaces long command lines; keys are the long flag names with underscores.
A `dasllama-server.toml` in the cwd (or next to the program — the exe dir in a release
bundle) loads automatically; `--config other.toml` picks a different one.

```toml
model = "D:/models/SmolLM2-135M-Instruct-Q8_0.gguf"
port = 8080
quant = "q8"
kv_dtype = "tq4"   # rotated 4-bit KV — half the q8_0 cache bytes
ctx = 4096
max_tokens = 4096  # default reply budget for clients that omit max_tokens (e.g. `llm chat`)
streams = 4
threads = 16       # matmul dispatch lane cap; -1 = all cores
```

Chat and completion requests **batch continuously** (`llm_scheduler.das`): up to `--streams`
generations run concurrently through one `eval_batch` decode step per tick, with long prompts
prefilled in `--chunk`-token slices so a new arrival never stalls running streams for more than
one chunk. Requests beyond `--streams` queue (up to 32; then 503). KV is **paged** by default —
cache memory tracks each stream's actual context, and finished streams donate their pages to a
**prefix cache**, so a repeated prompt prefix (a shared system prompt, the next turn of the same
conversation) attaches instead of re-prefilling — time-to-first-token collapses on warm prompts.
Clients whose connection drops mid-generation are evicted within a tick. The buffered endpoints
(`/v1/models`, `/v1/embeddings`, `/v1/audio/*`) still run to completion in-handler and pause
generation for their duration. OpenAI is stateless — the client resends the full transcript each
turn.

## Deploying (daspkg release)

```sh
bin/daslang utils/daspkg/main.das -- release --root utils/dasllama-server --out <target>
```

Produces `<target>/dasllama-server/` — the standalone exe, the shared-module dylibs (dasHV,
dasAudio), the runtime DLLs, and the `[tune]` sidecar (`dasllama-server.tune.json`; incomplete
scopes are tuned and baked during the release build). Re-releasing over an existing bundle is the
upgrade path: a `dasllama-server.toml` living in the bundle is NOT part of the release manifest,
so the deployed config survives. Stop a running server first — Windows locks the exe.

## Endpoints

| Method | Path | Notes |
|---|---|---|
| `GET`  | `/v1/models` | Lists the served model (and `--asr` if loaded) |
| `POST` | `/v1/chat/completions` | Chat; `stream: true` → SSE, else buffered; OpenAI function calling (`tools`) |
| `POST` | `/v1/completions` | Raw completion; `stream: true` → SSE, else buffered |
| `POST` | `/v1/embeddings` | Mean-pooled, L2-normalized sentence embeddings |
| `POST` | `/v1/audio/transcriptions` | Speech→text (multipart upload; needs `--asr`) |
| `POST` | `/v1/audio/translations` | Speech→English text (needs `--asr`) |
| `GET`  | `/v1/stats` | Scheduler counters: active/queued/peak_active, decode_steps, avg_batch, evicted, cached_tokens, prefix_pages, … |
| `POST` | `/shutdown` | Graceful stop |

### Chat

```sh
curl http://127.0.0.1:8080/v1/chat/completions -H 'Content-Type: application/json' -d '{
  "messages": [{"role": "user", "content": "Say hello in one word."}],
  "max_tokens": 16, "stream": false
}'
```

### Sampling parameters

Both completion routes accept the OpenAI sampling fields plus the llama.cpp-style extensions —
absent fields keep the greedy default (`temperature: 0`):

| Field | Default | Meaning |
|---|---|---|
| `temperature` | 0 | `<= 0` = greedy argmax |
| `top_k` | 0 (off) | keep only the k highest logits |
| `top_p` | 1 (off) | nucleus: keep the smallest probability mass `>= top_p` |
| `min_p` | 0 (off) | drop tokens with probability `< min_p * max_prob` |
| `repeat_penalty` | 1 (off) | multiplicative repetition penalty over the recent window, applied once per unique token (llama.cpp semantics) |
| `presence_penalty` | 0 | flat logit subtraction for every distinct token in the recent window |
| `frequency_penalty` | 0 | per-occurrence logit subtraction (OpenAI semantics) |
| `seed` | 0 (default seed) | non-zero overrides the stream's RNG seed; sessions start from a fixed default seed, so identical no-seed requests already reproduce |

The cutoffs (`top_k` / `top_p` / `min_p`) shape the sampled distribution, so they only take
effect with `temperature > 0` — greedy short-circuits to argmax; the penalties apply in greedy
mode too. E.g. Qwen3.6's instruct-mode card settings: `"temperature": 0.7, "top_p": 0.8,
"presence_penalty": 1.5`.

### Thinking control

Hybrid thinking models (the Qwen3/Qwen3.6 family) reason in a `<think>` block by default. Set
`"enable_thinking": false` (top-level, or the llama.cpp spelling
`"chat_template_kwargs": {"enable_thinking": false}`) and the server appends the template's
empty think block (`<think>\n\n</think>\n\n`) to the generation prompt — the model answers
directly, matching the family Jinja's `enable_thinking=false` form. A no-op for models whose
vocab has no think tokens.

### Tool / function calling

`/v1/chat/completions` speaks the OpenAI function-calling protocol: pass `tools` (and optionally
`tool_choice`; `"none"` disables, the forced-function object form is not honored), get back
`finish_reason: "tool_calls"` with `message.tool_calls`, send the results as `role: "tool"`
messages, repeat. Assistant `tool_calls` turns and `role: "tool"` results replay exactly through
the chat template on each stateless resend, so agent loops (opencode, pi, …) work end-to-end.

The wire format is per model family: the ChatML/Hermes shape (Qwen2.5 / Qwen3 — tool JSONs in a
`<tools>` system block, calls as `<tool_call>{"name":…,"arguments":…}</tool_call>`) is
implemented; a model whose chat template declares no tool format gets an honest 400. Streaming
with tools streams content up to the first tool-call marker, then buffers and emits the parsed
calls as one `delta.tool_calls` chunk at finish.

Requests the server does NOT fully understand are visible in the log: unknown endpoints 404
through a catch-all that logs method + path + body head, and known routes warn per ignored field
(`response_format`, `stop`, multimodal content parts, …).

### Embeddings

`input` is a string or an array of strings. Each vector is `model.config.dim` floats,
mean-pooled over the decoder's last-layer hidden state (post-final-norm) and L2-normalized.

```sh
curl http://127.0.0.1:8080/v1/embeddings -H 'Content-Type: application/json' -d '{
  "input": ["the quick brown fox", "a lazy dog"]
}'
# -> {"object":"list","data":[{"object":"embedding","embedding":[...],"index":0}, ...],
#     "model":"...","usage":{"prompt_tokens":N,"total_tokens":N}}
```

A decoder-only LLM used as an embedder gives **RAG-grade** vectors (good for retrieval /
similarity), not a substitute for a dedicated embedding model.

### Transcription (with `--asr`)

```sh
curl http://127.0.0.1:8080/v1/audio/transcriptions \
  -F file=@audio.wav -F response_format=verbose_json
```

## Testing

All tests in this directory are model-gated and JIT-only (they skip cleanly when the GGUF is
absent; set `DASLLAMA_MODELS_DIR`):

- `test_openai_server.das` — endpoint conformance (`/v1/models`, `/v1/embeddings`, buffered chat,
  the tools-unsupported 400, the unknown-endpoint 404) over the real dashv HTTP client, plus
  model-free `parse_tool_calls` unit tests; needs `tinyllama-1.1b-chat-v1.0.Q8_0.gguf`.
- `test_llm_scheduler.das` — the continuous-batching scheduler against `generate()` references
  (bit-exact single stream, chunk invariance, staggered admits, eviction); needs
  `SmolLM2-135M-Instruct-Q8_0.gguf`.
- `test_openai_server_stream.das` — SSE chunk framing, the over-long-prompt 400, two concurrent
  clients batching on one server (`peak_active >= 2` via `/v1/stats`), mid-generation disconnect
  eviction, and the prefix cache returning an identical completion for a repeated request; needs
  `SmolLM2-135M-Instruct-Q8_0.gguf`.

```sh
bin/daslang -jit dastest/dastest.das -- --test utils/dasllama-server/test_openai_server.das
```

`server_bench.das` (same directory) measures the serving latencies directly through the scheduler
seam: tok/s + TTFT + inter-token percentiles vs batch size, decode stall per prefill chunk size,
and warm-vs-cold TTFT for the prefix cache.

## Not yet implemented

Multimodal content arrays in chat messages, the request's `stop` / `response_format` fields, and
the forced-function `tool_choice` object form — all logged when a request carries them.
