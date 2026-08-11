# dasllama-server — an OpenAI-compatible HTTP server

A drop-in OpenAI-compatible server for [dasLLAMA](../../modules/dasLLAMA) inference (CPU, or
hybrid with the Metal/Vulkan GPU tiers via `gpu = metal|vulkan`), written
entirely in daslang over the public dasLLAMA facade + the `dasHV` HTTP layer. Point any OpenAI
client (opencode, Open WebUI, the `llm` CLI, the `openai` Python SDK, …) at `http://127.0.0.1:<port>/v1`.

It reaches **only** public facade verbs (`load_model` / `create_chat_renderer` / `add_user` /
`render_assistant` / `render_turn` / `eval_batch` via `dasllama_scheduler` / `transcribe` / `embed`) —
that is the point: the server is the acceptance test for the API rework. If it builds with no
reach into engine internals, the facade is complete.

## Run

```sh
bin/daslang -jit utils/dasllama-server/main.das -- --model <model.gguf> [--port 8080] [--quant q8] \
                                                    [--asr <asr.bin>] [--asr-workers 2] [--mmproj <mmproj.gguf>] [--ctx 4096] \
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
| `--asr-workers` | — | `1` | Long-lived ASR request threads; each owns a model and reusable session. Set `2` for two parallel transcriptions |
| `--mmproj` | — | — | mmproj GGUF for the Qwen3-ASR route (paired with `--asr`) |
| `--ctx` | — | *model* | Context-length cap in tokens (default: the model's trained `context_length`; set it to bound `--flat` KV or trim RAM) |
| `--max-tokens` | — | `256` | Default reply token budget when a request omits `max_tokens` (clamped to `--ctx` per request) |
| `--streams` | `-s` | `4` | Max concurrent generation streams |
| `--threads` | `-t` | `16` | Worker-lane cap for the matmul dispatch (`-1` = all cores) — decode is bandwidth-bound, so an uncapped dispatch just fights the rest of the box |
| `--team-dispatch` | — | `hybrid` | `hybrid`: LLM uses the worker team while ASR callers run inline; `team`: all callers use serialized team publishes; `inline`: every caller runs independently |
| `--chunk` | — | `64` | Prefill quantum in tokens — decode stalls at most this per tick |
| `--page-rows` | — | `64` | KV page size in positions for paged serving |
| `--prefix` | — | *auto* | Prefix-cache retention cap in pages (auto: one full context per stream; `-1` = unbounded) |
| `--flat` | — | — | Flat preallocated KV sessions — disables paged serving and the prefix cache |
| `--mtp` | — | — | MTP/NextN self-speculative decode for greedy requests (`temperature: 0`, no repetition penalty) — needs a model with an in-file NextN head (the `-MTP-` GGUFs). Output-invariant; up to ~2x decode on the dense qwen35/qwen3.6 models (measured rows: `performance/results_llm.md`), ~nothing on the MoEs. `/v1/stats` reports `mtp_drafted`/`mtp_accepted` |
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
team_dispatch = "hybrid" # LLM team dispatch + independent inline ASR caller threads
asr_workers = 2    # two independent transcription requests; each worker owns an ASR model
```

Several models serve LIVE from one process via a `[[models]]` roster instead of the flat `model`
key — requests route on their `"model"` field (absent → the default entry). Execution is
serialized (one scheduler steps at a time), switches are fast, and every cache level survives a
switch: host weights stay mmap'd, each slot keeps its own KV pool + prefix cache, and ONE model's
GPU state lives in VRAM at a time (the tier drops + re-arms on switch; `backend = "cpu"` slots
never evict the GPU owner, so gpu↔cpu alternation is free). Blank keys inherit the flat defaults;
`backend` is `auto | cpu | gpu` (auto = the engine's decline ladder), and per-entry `ctx`, `quant`,
`kv_dtype`, `streams`, `chunk`, `page_rows`, `prefix`, `mtp` override per model:

```toml
[[models]]
name = "qwen"      # route id (default: the file's basename)
path = "D:/models/Qwen3.6-35B-A3B-Q8_0.gguf"
default = true

[[models]]
name = "smol"
path = "D:/models/SmolLM2-135M-Instruct-Q8_0.gguf"
backend = "cpu"    # never touches the device — alternating with the GPU slot costs nothing
```

Chat and completion requests **batch continuously** (`dasllama/dasllama_scheduler.das`): up to `--streams`
generations run concurrently through one `eval_batch` decode step per tick, with long prompts
prefilled in `--chunk`-token slices so a new arrival never stalls running streams for more than
one chunk. Requests beyond `--streams` queue (up to 32; then 503). KV is **paged** by default —
cache memory tracks each stream's actual context, and finished streams donate their pages to a
**prefix cache**, so a repeated prompt prefix (a shared system prompt, the next turn of the same
conversation) attaches instead of re-prefilling — time-to-first-token collapses on warm prompts.
Clients whose connection drops mid-generation are evicted within a tick. Audio uploads queue to
long-lived `new_thread` ASR workers and do not block chat generation; `--asr-workers 2` permits two
transcriptions at once. Each worker owns its model/context and reuses language-specific session
scratch, so memory settles at the workers' high-water mark. OpenAI is stateless — the client
resends the full transcript each turn.

## Supervised deployment

dasllama-server is JIT-only (per-box `[tune]`/`[llvm_code]` kernels, plus a shared-module `[init]`
global a baked exe mis-wires), so it is deployed as `daslang -jit main.das` under the shared
watchdog in `utils/watchdog/`. A deployed bundle needs no arguments — the watchdog finds `main.das`
beside `bin/Release/daslang.exe` and supervises that, and `watchdog.json` pins the name so logs land
in `logs/dasllama-watchdog.log`:

```powershell
Set-Location E:/dasllama-server

# Run once from an elevated PowerShell. Installs an app-specific WER normal-minidump policy;
# it does not dump the model weights/private heap.
python ./watchdog.py --install-local-dumps

# Day-to-day launch, no elevation. dasllama-server.toml is auto-loaded from this directory.
$env:DAS_JOBQUE_THREADS = "16"
python ./watchdog.py --require-dumps
```

From the source tree the watchdog no longer sits beside the script, so pass `--cwd`:

```powershell
python utils/watchdog/watchdog.py --cwd utils/dasllama-server --jit-stack
```

The first JIT start on an untuned box writes the tune sidecar and exits with code 3; the watchdog
recognizes that bootstrap exit and relaunches. That cold path — DLL cache miss, codegen, tuning every kernel family, model load — takes minutes, so the watchdog logs ranked startup stages
(`jit_codegen` -> `jit_linked` -> `tuning` -> `model_load` -> `ready`) with the elapsed time of
each, and reports health only on transition plus a heartbeat.

It writes rotating JSON-line logs, samples process memory once a minute, and polls `/v1/models`
(use `--health-url`/`--shutdown-url` for a non-default port). `--jit-stack` records every generated
daslang call in the logical stack; Windows JIT links also retain a compact `.map` beside the
`.dll/.o`. After a crash it waits for the WER minidump, copies it with the matching JIT artifacts,
tune manifest, metadata and log into `logs/crashes/`, shows a Windows notification, and restarts
with bounded exponential backoff. The ten newest bundles are retained.

Full watchdog reference — config keys, discovery rules, control plugins: `utils/watchdog/README.md`.

## Deploying (daspkg release)

`release_requires_jit()` makes `daspkg release` refuse this package outright: baking a `-exe`
would drop the per-box JIT kernels and ship a broken binary. Deploy by staging the JIT bundle —
`main.das`, `bin/Release/daslang.exe` plus the runtime DLLs and shared modules, `watchdog.py`,
`watchdog.json`, `control.html` — into the target directory, and keep the deployed
`dasllama-server.toml` and `dasllama-server.tune.json` across upgrades. Stop a running server first;
Windows locks the DLLs.

## Endpoints

| Method | Path | Notes |
|---|---|---|
| `GET`  | `/` | Control page: live stats + charts, models panel (per-slot cards with state/GPU badges, prefix hit rate, switch telemetry, activate buttons; VRAM bar + switch strip), stream swimlane + live text cards, prefix-cache table, a chat panel (all sampling knobs, `<think>` inline, mic input under `--asr`), config editor with the `[[models]]` roster table + save/restart, GC + drain buttons. Serves `control.html` from beside the server sources — polls `/v1/stats` + `/v1/streams` at 1 Hz |
| `GET`  | `/v1/models` | Lists every served slot (and `--asr` if loaded) — requests route on these ids via their `"model"` field |
| `POST` | `/v1/models/activate` | `{"model": name}` admin warm-switch: make `name` the stepped slot and move the GPU tier to it now (instead of waiting for the owner to drain). `409` while any work is live, `404` on an unknown name; `200` reports `switch_ms` + `backend_effective` |
| `POST` | `/v1/chat/completions` | Chat; `stream: true` → SSE, else buffered; OpenAI function calling (`tools`) |
| `POST` | `/v1/completions` | Raw completion; `stream: true` → SSE, else buffered |
| `POST` | `/v1/embeddings` | Mean-pooled, L2-normalized sentence embeddings |
| `POST` | `/v1/audio/transcriptions` | Speech→text (multipart upload; needs `--asr`). `response_format=verbose_json` adds timed segments |
| `POST` | `/v1/audio/translations` | Speech→English text (needs `--asr`) |
| `POST` | `/vad` | Silero speech spans over an uploaded clip (the control page's waveform overlay; in-handler, ≤120 s, needs the in-repo `silero_vad.bin`) |
| `GET`  | `/v1/stats` | Scheduler counters (`gen_tokens`, `prefill_tokens`, TTFT last/avg, …) plus `model`/`active_model`/`ctx`/`uptime_s`/`draining` identity fields, memory footprint (`weights_bytes`, `kv_bytes`, das heaps, `gpu_vram_bytes`/`gpu_budget_bytes`), a `hardware` line (CPU · lanes · GPU), `asr_workers`, `asr_ready`, `asr_active`, `asr_pending`, and `models[]` — one entry per slot: `is_active`, `holds_gpu`, requested `backend` vs `backend_effective` (`cpu`/`gpu:rails`/`gpu:resident`), per-slot cache counters, `last_used_s`, switch count/avg ms |
| `GET`  | `/v1/streams` | Per-stream poll surface: `model` (the slot it runs on), state (`queued`/`prefilling`/`decoding`/`finished`), token counts, TTFT, and capped text tails (prompt head + generated tail); finished streams linger ~10 s flagged `finished`. Plus `cache`: the prefix-cache donation chains (tokens, live pages, hits, age, preview) and `asr`: recent ASR jobs (state, audio s, wall ms, RTF) |
| `GET`  | `/config` | Effective config with per-key source (`default`/`cli`/`toml`), the `[[models]]` roster, model files beside the served one, active rail (gguf vs prepared `.dlim`), GPU tier status (`supported` + `reason` when the loaded model can't ride it) |
| `POST` | `/config` | Validate a `{key: value}` JSON body and write it as an **authoritative** TOML (`authoritative = true`) to the config path (or `dasllama-server.toml` beside the program on a config-less start). Applies on the next restart |
| `POST` | `/restart` | Drain like `/shutdown`, then exit with code **4** — the watchdog relaunches, picking up the saved config (3 stays the tune-restart code) |
| `POST` | `/gc` | Schedule a validated collection at the next lifecycle safe point; concurrent requests coalesce |
| `POST` | `/shutdown` | Stop admitting new LLM/ASR work, drain accepted work, then exit |

Config precedence: `defaults < config TOML < explicit CLI flags` — unless the TOML carries
`authoritative = true` (what the control page saves), which flips the top: `defaults < CLI <
authoritative TOML`. The `gpu` key (`off | metal | metal-required | vulkan`) is the first-class
backend selector; `gpu = vulkan` arms the MoE tier in its blessed shape — expert stacks sized
**automatically** (resident layers fill the VRAM budget, the rest stream) plus DN + ATTN + dense +
the resident shared expert. `gpu_layers` / `gpu_stream` are `0` = auto by default; set either to a
positive value to pin it exactly, and `gpu_dn` / `gpu_attn` / `gpu_dense` / `gpu_vram_mb` override
the rest. The `DASLLAMA_GPU_*` env vars still override everything (they remain the A/B levers), and
`DASLLAMA_GPU=1` requests the same auto shape without a config file.

### Chat

```sh
curl http://127.0.0.1:8080/v1/chat/completions -H 'Content-Type: application/json' -d '{
  "messages": [{"role": "user", "content": "Say hello in one word."}],
  "max_tokens": 16, "stream": false, "truncation": "auto"
}'
```

### Demo load

`demo_load.das` (a sibling of the server, dogfooding the dashv client) drives varied chat
completions from worker threads — start it, then watch the control page's swimlane fill and the
tok/s stair-step:

```sh
bin/daslang utils/dasllama-server/demo_load.das -- --url http://127.0.0.1:8080   # staged ramp 1 -> 2 -> 4 -> 8
bin/daslang utils/dasllama-server/demo_load.das -- -n 4 -r 10                    # constant 4 workers
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

### Thinking control and `reasoning_content`

`enable_thinking` is tri-state: ABSENT leaves the family's own default in force (the
Qwen3/3.5/3.6 and GLM families think by default; gemma-4 E-series is instruct unless asked;
gpt-oss always thinks), and a present bool — top-level or the llama.cpp spelling
`"chat_template_kwargs": {"enable_thinking": ...}` — overrides it. `false` on a
`<think>`-family appends the template's empty think block so the model answers directly;
`true` on gemma-4 opens the system turn with the `<|think|>` gate and lets the model emit its
thought channel. A no-op for models whose vocab has no think tokens.

A thinking reply's reasoning span comes back as **`reasoning_content`** (the
DeepSeek/llama.cpp framing) with `content` clean of the family's markers: on the
`chat.completion` message for buffered requests, and as `delta.reasoning_content` chunks —
streamed before the `delta.content` chunks — for `stream: true`. Tool-calling replies split
reasoning first, so a thinking model that calls tools returns `reasoning_content` AND
`tool_calls` in one response. The field is absent (never empty) when the model did not think.

### Context truncation

By default, an over-context rendered prompt returns HTTP 400. Set `truncation: "auto"` to preserve
system messages and tools while dropping the oldest complete user-led turns until the prompt plus
the requested `max_tokens` output budget fits. If the system/tools/latest turn cannot fit, the
request still returns 400. `finish_reason: "length"` means generation consumed its output budget.

### Tool / function calling

`/v1/chat/completions` speaks the OpenAI function-calling protocol: pass `tools` (and optionally
`tool_choice`; `"none"` disables, the forced-function object form is not honored), get back
`finish_reason: "tool_calls"` with `message.tool_calls`, send the results as `role: "tool"`
messages, repeat. Assistant `tool_calls` turns and `role: "tool"` results replay exactly through
the chat template on each stateless resend, so agent loops (opencode, pi, …) work end-to-end.

The wire format is per model family (`ToolMode`, `dasllama_tools.das`): **hermes** (Qwen2.5 /
Qwen3 family — `<tools>` system block, `<tool_call>` JSON), **harmony** (gpt-oss — developer-turn
TypeScript namespace defs, commentary-channel recipient calls; reasoning and calls come from one
channel walk), **gemma4** (gemma-4 — declaration/call/response DSL with the `<|"|>` quote token),
**mistral** (v0.3+ — `[AVAILABLE_TOOLS]` defs, `[TOOL_CALLS]` array, bare-array replies
tolerated since the SPM stream suppresses control-token pieces), and **llama_json** (llama-3.x —
the whole reply is one `{"name","parameters"}` object, results on the `ipython` role). A model
whose chat template declares no tool format (GLM until its zen2 leg) gets an honest 400.
Streaming with tools buffers the native envelope and emits the parsed calls as one
`delta.tool_calls` chunk at finish.

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

ASR work is asynchronous with respect to the HTTP tick and LLM scheduler. Requests beyond the
worker count wait in a bounded queue (32 uploads; further requests receive 503). Use one importer
request thread per ASR worker to keep the workers occupied without duplicating database work.

## Testing

All tests in this directory are model-gated and JIT-only (they skip cleanly when the GGUF is
absent; set `DASLLAMA_MODELS_DIR`):

- `test_openai_server.das` — endpoint conformance (`/v1/models`, `/v1/embeddings`, buffered chat,
  the tools-unsupported 400, the unknown-endpoint 404) over the real dashv HTTP client, plus
  model-free `parse_tool_calls` unit tests; needs `tinyllama-1.1b-chat-v1.0.Q8_0.gguf`.
- `modules/dasLLAMA/tests/test_scheduler.das` — the continuous-batching scheduler against `generate()` references (moved with the module)
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
