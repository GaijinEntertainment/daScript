# dasllama-server - an OpenAI-compatible HTTP server

A drop-in OpenAI-compatible server for [dasLLAMA](../../modules/dasLLAMA) inference (CPU, or
hybrid with the Metal/Vulkan GPU tiers via `gpu = metal|vulkan`), written
entirely in daslang over the public dasLLAMA facade + the `dasHV` HTTP layer. Point any OpenAI
client (opencode, Open WebUI, the `llm` CLI, the `openai` Python SDK, ...) at `http://127.0.0.1:<port>/v1`.

It reaches **only** public facade verbs (`load_model` / `create_chat_renderer` / `add_user` /
`render_assistant` / `render_turn` / `eval_batch` via `dasllama_scheduler` / `transcribe` / `embed`) -
that is the point: the server is the acceptance test for the API rework. If it builds with no
reach into engine internals, the facade is complete.

## Get it

The server ships as a standalone download from the rolling
`dasllama-server` release, refreshed with every daslang release:
<https://github.com/GaijinEntertainment/daScript/releases/tag/dasllama-server>

| Platform | Asset |
|---|---|
| macOS, Apple silicon | `dasllama-server-darwin-arm64.zip` |
| Windows x64 | `dasllama-server-windows-x64.zip` |
| Linux x86_64 | `dasllama-server-linux-x86_64.tar.gz` |
| Linux arm64 | `dasllama-server-linux-arm64.tar.gz` |

The Linux bundles need glibc 2.35 or newer (Debian 12, Ubuntu 22.04 and later), the system
OpenSSL 3 (`libssl3` on Debian and Ubuntu, `openssl-libs` on Fedora) and `curl` for the catalog
downloads; the macOS and Windows bundles carry their own OpenSSL.

Unpack it and start the supervisor beside the server - `watchdog` (`watchdog.exe`), or on
macOS the `dasllama-server.app` itself, whose launcher is the watchdog. The server binary
beside it is `dasllama-server.exe` on Linux and Windows alike (the macOS app carries it as
`Contents/MacOS/dasllama-server`). The watchdog keeps the server up, puts the dasllama mark in
the notification area, and a click on it opens the control page, <http://127.0.0.1:8080/>.
With no model configured the server starts in **setup mode** (below): pick a model from the
catalog, it downloads into `~/.dasllama/models`, and *serve this model* restarts into it. The
config it writes, `dasllama-server.toml` beside the exe, and the tune sidecar beside it survive
an upgrade unpacked over the old directory on Linux and Windows; on macOS a new `.app` replaces
the old one whole, so copy the two files out of `Contents/MacOS` first and back in after.

The exe is a fat build (`daspkg release --fat`): plain code for the platform's baseline CPU
class - `x86-avx2` on x86, `arm-neon` on arm64 - with one clone of every `[tune]` kernel per
class the engine ships a profile for (`x86-vnni512`, `x86-amx`, `arm-i8mm`), picked from cpuid
at start. On a Mac the Metal crowns are raced once at the first start and kept beside the exe.
That is the good default, and it is as far as a solid executable goes: **a fat build cannot
tune**. Its kernels are baked, there is no tuner in it, and `--tune`, the sidecar exchange
and a re-tune have nothing to act on (the control page's exchange levers refuse). To tune
every kernel for this box, install daslang with dasLLAMA - the SDK - and run the server
through the JIT, `bin/daslang -jit utils/dasllama-server/main.das` below, or `daspkg release`
this package on that box: either mints the box's own sidecar, every kernel raced on your own
hardware instead of picked from a class profile.

## Measure your box

The control page's *benchmark* button runs the llama-bench rows on the served model right in
the server - pp512 and tg128, three timed reps each after an untimed warmup - and shows tokens
per second beside the hardware line; every route that could contend holds for the run's
duration (the text, audio and speech routes answer 503, the model-switching, bake and download
routes 409), so nothing shares the cores with it. A run is seconds on a small model - about a
second for a 135M model and six for a 1B one on an M1 Max. The same rows from a shell, and the
comparison the page cannot run itself, come from `dasllama-bench` beside the server -
`dasllama-bench.exe` on Linux and Windows, `dasllama-server.app/Contents/MacOS/dasllama-bench`
on a Mac:

```sh
./dasllama-bench.exe -m ~/.dasllama/models/<model>.gguf           # pp512/tg128, five reps
./dasllama-bench.exe -m <model.gguf> --ref /path/to/llama-bench   # and llama.cpp on the same GGUF
```

It is `modules/dasLLAMA/benchmarks/lcpp_bench.das` baked with the server's class; `--help`
lists the rest (`-p`, `-n`, `-r`, `-t`, `-o md`). Its first start on a Mac races the Metal
crowns like the server's does. The two run the same rows, not the same numbers: the page
measures the served slot as it is - its device (Metal, or the Vulkan tier where the slot armed
one), its KV codec - on a session sized to the rows, while `dasllama-bench` runs f16 KV on the
CPU unless `-ngl` says otherwise, the llama-bench defaults, five reps to the page's three. The
page's result names what produced it - the device that served the rows, the KV codec, the exec
tier (`exe-native` in a bundle, `jit` from the SDK), the tune state (`fat` in a bundle; `tuned`
or `untuned` under the JIT) - and the `llama-bench` line that matches it. One difference to
know about: the rows run llama-bench's flat session, so on the Vulkan tier they take the
resident driver where the paged serving path declines it, and the result says `gpu:resident`
when they did. A pp rep on a large CPU model can outlast the watchdog's two-second health
probe; the tray reads unhealthy for that rep and recovers on its own.

The bundles are not code-signed. macOS quarantines a downloaded app, and an app started from
the download folder runs from a read-only copy where nothing it writes beside itself survives -
so first drag `dasllama-server.app` out of Downloads with the Finder (Applications, or any folder
of yours), then either allow it under System Settings > Privacy & Security after the first
refused start, or clear the flag once - `xattr -dr com.apple.quarantine dasllama-server.app`.
Windows SmartScreen shows an unknown publisher: *More info*, *Run anyway*.

## Run from the source tree

```sh
bin/daslang -jit utils/dasllama-server/main.das -- --model <model.gguf> [--port 8080] [--quant q8] \
                                                    [--asr <asr.bin>] [--asr-workers 2] [--mmproj <mmproj.gguf>] \
                                                    [--image-mmproj <mmproj.gguf>] [--ctx 4096] \
                                                    [--streams 4] [--chunk 64] [--page-rows 64] [--prefix N]
```

Run under `-jit` - the interpreter is refused, it is far too slow for inference. Flags:

| Flag | Short | Default | Meaning |
|---|---|---|---|
| `--config` | `-c` | *auto* | TOML config file; keys mirror the long flag names, explicit CLI flags override. Without the flag, a `dasllama-server.toml` in the cwd or next to the program loads automatically |
| `--model` | `-m` | *(required)* | GGUF model to serve (here or in `--config`) |
| `--port` | `-p` | `8080` | Listen port |
| `--quant` | `-q` | `q8` | Weight quantization: `fp32` \| `q8` \| `q4` - plus the loader's file-format spellings `q4_k` \| `q5_k` \| `q6_k` \| `mxfp4` \| `f16` \| `bf16` (all serve on the `q8` kquant-native tier) |
| `--kv-dtype` | - | `f16` | KV-cache codec: `f32` \| `f16` \| `q8_0` \| `tq4` (rotated 4-bit; needs pow2 head_size) |
| `--asr` | `-a` | - | ASR model (whisper/parakeet/qwen3-asr) - enables the `/v1/audio/*` routes |
| `--asr-workers` | - | `1` | Long-lived ASR request threads; each owns a model and reusable session. Set `2` for two parallel transcriptions |
| `--mmproj` | - | - | mmproj GGUF for the Qwen3-ASR route (paired with `--asr`) |
| `--tts` | - | - | TTS model GGUF (`kitten-nano`, `kitten-mini`, `kokoro-82m`, with the front-end packs `tts_g2p.bin` + `tts_postag.bin` beside them; or a `pocket-tts-<lang>-q8` file, which reads text, needs no pack and clones a voice) - enables `/v1/audio/speech` |
| `--tts-lane` | - | `q8` | Weight lane the speech worker pins around its load: `q8` (the prepared quant image beside the GGUF) or `f32` (the file's own planes - the reference lane). An unknown spelling warns and serves `q8` |
| `--tts-voices-dir` | - | - | Directory of voice clips (wav / flac / mp3 / ogg, a few seconds of one speaker each, 60 s at most) a cloning speech model adds to its voices at boot, each under its file's stem (`mine.wav` -> voice `mine`; a stem the model already carries replaces that voice, with a log line). A clip that does not decode or runs past 60 s is logged and skipped; the key on a model that cannot clone is logged and ignored |
| `--image-mmproj` | - | - | Vision mmproj (gemma4uv, gemma4v, or gemma3v, sniffed) for the default model - enables `image_url` parts on `/v1/chat/completions`. Per-model in a `[[models]]` roster: `image_mmproj = "..."`. When the file also carries a gemma4a audio encoder (the E-series mmproj carries both towers), the same flag arms **native audio**: `input_audio` parts serve through the same slot - one decoder, one mmproj, no dedicated ASR model copy |
| `--ctx` | - | *model* | Context-length cap in tokens (default: the model's trained `context_length`; set it to bound `--flat` KV or trim RAM) |
| `--max-tokens` | - | `256` | Default reply token budget when a request omits `max_tokens` (clamped to `--ctx` per request) |
| `--streams` | `-s` | `4` | Max concurrent generation streams |
| `--threads` | `-t` | `16` | Worker-lane cap for the matmul dispatch (`-1` = all cores) - decode is bandwidth-bound, so an uncapped dispatch just fights the rest of the box |
| `--team-dispatch` | - | `hybrid` | `hybrid`: LLM uses the worker team while the ASR and TTS workers run inline; `team`: all callers use serialized team publishes; `inline`: every caller runs independently |
| `--chunk` | - | `64` | Prefill quantum in tokens - decode stalls at most this per tick |
| `--page-rows` | - | `64` | KV page size in positions for paged serving |
| `--prefix` | - | *auto* | Prefix-cache retention cap in pages (auto: one full context per stream; `-1` = unbounded) |
| `--flat` | - | - | Flat preallocated KV sessions - disables paged serving and the prefix cache |
| `--mtp` | - | - | MTP/NextN self-speculative decode - needs a model with an in-file NextN head (the `-MTP-` GGUFs). Greedy requests are output-invariant; a sampled request (`temperature` > 0, penalties included) draws each verify row with its own sampler and keeps the plain sampled distribution, at a lower acceptance rate. Up to ~2x decode on the dense qwen35/qwen3.6 models (measured rows: `modules/dasLLAMA/performance/records/<box>.json`, on the site board), ~nothing on the MoEs. `/v1/stats` reports `mtp_drafted`/`mtp_accepted` |
| `--models-dir` | - | `~/.dasllama/models` | Where the model catalog downloads land (`DASLLAMA_MODELS_DIR` overrides both this and the config key) |
| `--tune` | - | - | Re-tune this box's dasLLAMA kernels, then relaunch (the JIT run; a fat build carries no tuner and ignores it) |
| `--help` | `-?` | - | Show help and exit |

A config file replaces long command lines; keys are the long flag names with underscores.
A `dasllama-server.toml` in the cwd (or next to the program - the exe dir in a release
bundle) loads automatically; `--config other.toml` picks a different one.

```toml
model = "D:/models/SmolLM2-135M-Instruct-Q8_0.gguf"
port = 8080
quant = "q8"
kv_dtype = "tq4"   # rotated 4-bit KV - half the q8_0 cache bytes
ctx = 4096
max_tokens = 4096  # default reply budget for clients that omit max_tokens (e.g. `llm chat`)
streams = 4
threads = 16       # matmul dispatch lane cap; -1 = all cores
team_dispatch = "hybrid" # LLM team dispatch + independent inline ASR/TTS worker threads
asr_workers = 2    # two independent transcription requests; each worker owns an ASR model
```

## Setup mode and the model catalog

A start with **no LLM model at all** (no `--model`, no config, or every configured path
missing) **and no servable `--tts`** does not exit - it boots into **setup mode**: the port
opens, the control page serves, every inference route answers with a clean error, and the
page leads with the **model catalog** (sec. 03) - a curated, sha-pinned list of current models
(`model_catalog.das`; commit-pinned
HF URLs, canonical sha256, one download at a time, `curl -C -` resume). A finished download
is verified against its pinned sha before it is renamed into `models_dir` (default
`~/.dasllama/models`; the `models_dir` config key or `DASLLAMA_MODELS_DIR` move it) with the
house `.sha` sidecar beside it. In setup mode a downloaded entry offers **serve this
model** - the page writes the config and restarts (exit 4, the watchdog contract) straight
into serving it. The same catalog stays available on a serving server for pulling more
models.

**`--tts` alone is a serving start, not a setup start.** A speech-only server has no LLM slot,
so `/v1/stats` answers the slotless shape - but its `setup` reads `false`, and the page keeps
its serving surface instead of leading with the catalog.

**A `--tts` the server cannot serve degrades, it does not die.** A missing file (the GGUF, or
`tts_g2p.bin` / `tts_postag.bin` beside it) is logged as an error before the worker starts, and
so is a load that fails with the whole set present - a front-end pack from before this build, a
truncated GGUF, a model whose every voice speaks a language the front end does not phonemize.
Either way the speech route is dropped and the boot serves whatever is left - the LLM slots if
any loaded, else setup mode by the rule above. Nothing panics: a crash-looping boot under the
watchdog never gets a page to fix the config from. A failed LOAD also leaves the `tts` block on
`/v1/stats` with `ready: false` and an `error`, so the page can say why instead of waiting.

**A Pocket TTS file (`pocket-tts-<lang>-q8.gguf`) reads text and clones voices.** It needs no
front-end pack, so the file is its whole set; `/v1/audio/phonemes` answers its chunks with an
empty phoneme string; `speed` other than `1.0` is refused with a `400` (the model has no speed
control); the `tts` block reads `cloning: true` and names the one language the file speaks. Its
voices are the file's bundled clips plus every clip under `--tts-voices-dir` (`tts_voices_dir`
in the TOML), each under its file's stem.

Catalog entries carry their **towers**: a vision-capable row offers its pinned mmproj
(download -> **enable vision** -> restart wires `image_mmproj`), and a **dictation** strip
under the table offers the ASR tower (parakeet v3; wires the `asr` key the same way) -
`POST /catalog/download` takes `{"name", "tower": "vision"}` or `{"tower": "asr"}` on the
same one-at-a-time rail. A **speech** strip sits beside it for the text-to-speech set the
`/catalog` document's `tts` list carries: the two front-end packs first (every speech model
loads them), then one model, then **enable speech** wires the `tts` key -
`{"tower": "tts", "file": <file>}` pulls one file of that set on the same rail; a Pocket file is
its whole set. Setup-mode
**serve this model** wires any tower already on disk automatically. Each row also wears a
**fit badge** (fits gpu / fits / tight / too big) from
the box facts the `/catalog` document carries (`box.ram_gb`, and the armed tier's weight
budget as `box.vram_mb`); the advertised working set is a hint, not a load gate.

Several models serve LIVE from one process via a `[[models]]` roster instead of the flat `model`
key - requests route on their `"model"` field (absent -> the default entry). Execution is
serialized (one scheduler steps at a time), switches are fast, and every cache level survives a
switch: host weights stay mmap'd, each slot keeps its own KV pool + prefix cache, and ONE model's
GPU state lives in VRAM at a time (the tier drops + re-arms on switch; `backend = "cpu"` slots
never evict the GPU owner, so gpu<->cpu alternation is free). Blank keys inherit the flat defaults;
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
backend = "cpu"    # never touches the device - alternating with the GPU slot costs nothing
```

Chat and completion requests **batch continuously** (`dasllama/dasllama_scheduler.das`): up to `--streams`
generations run concurrently through one `eval_batch` decode step per tick, with long prompts
prefilled in `--chunk`-token slices so a new arrival never stalls running streams for more than
one chunk. Requests beyond `--streams` queue (up to 32; then 503). KV is **paged** by default -
cache memory tracks each stream's actual context, and finished streams donate their pages to a
**prefix cache**, so a repeated prompt prefix (a shared system prompt, the next turn of the same
conversation) attaches instead of re-prefilling - time-to-first-token collapses on warm prompts.
Clients whose connection drops mid-generation are evicted within a tick. Audio uploads queue to
long-lived `new_thread` ASR workers and do not block chat generation; `--asr-workers 2` permits two
transcriptions at once. Each worker owns its model/context and reuses language-specific session
scratch, so memory settles at the workers' high-water mark. OpenAI is stateless - the client
resends the full transcript each turn.

The main server context configures the shared job queue. ASR, TTS and media worker contexts
enable their own fork-context pools before loading or evaluating a model: that setting is
context-local, not inherited from the main context. Worker setup preserves the shared queue's
team/FIFO policy and applies only the worker's selected team-dispatch participation.

## Supervised deployment

The server runs under the shared watchdog in `utils/watchdog/`, in the release bundle and in a
JIT deployment alike. The watchdog needs no arguments: in the bundle it finds the baked exe
beside it, in a JIT deployment `main.das` beside `bin/Release/daslang.exe`, and `watchdog.json`
pins the name so logs land in `logs/dasllama-watchdog.log`, turns the tray on and names
`tray.ico` as its mark. A JIT deployment on Windows:

```powershell
Set-Location E:/dasllama-server

# Run once from an elevated PowerShell. Installs an app-specific WER normal-minidump policy;
# it does not dump the model weights/private heap.
.\watchdog.exe --install-local-dumps

# Day-to-day launch, no elevation. dasllama-server.toml is auto-loaded from this directory.
$env:DAS_JOBQUE_THREADS = "16"
.\watchdog.exe --require-dumps
```

From the source tree the watchdog does not sit beside the script, so pass `--cwd`:

```powershell
bin/Release/watchdog.exe --cwd utils/dasllama-server --jit-stack
```

The first JIT start on an untuned box writes the tune sidecar and exits with code 3; the watchdog
recognizes that bootstrap exit and relaunches. That cold path - DLL cache miss, codegen, tuning every kernel family, model load - takes minutes, so the watchdog logs ranked startup stages
(`jit_codegen` -> `jit_linked` -> `tuning` -> `model_load` -> `ready`) with the elapsed time of
each, and reports health only on transition plus a heartbeat.

It writes rotating JSON-line logs and polls `/v1/models`
(use `--health-url`/`--shutdown-url` for a non-default port). `--jit-stack` records every generated
daslang call in the logical stack; Windows JIT links also retain a compact `.map` beside the
`.dll/.o`. After a crash it waits for the WER minidump, copies it with the matching JIT artifacts,
tune manifest, metadata and log into `logs/crashes/`, shows a Windows notification, and restarts
with bounded exponential backoff. The ten newest bundles are retained.

Full watchdog reference - config keys, discovery rules, the log: `utils/watchdog/README.md`.

## Deploying (daspkg release)

`daspkg release --root utils/dasllama-server --out <dir>` bakes the server into a standalone
bundle: the exe, the shared modules and runtime libraries it needs, `watchdog` beside it,
`dasllama-bench` (the `release_program` companion - lcpp_bench baked under the same class,
sharing the bundle's libraries), `watchdog.json`, `control.html` and `tray.ico`. Plain `release` tunes the kernels on the build
box and ships that box's sidecar - the bundle for a machine you own. `release --fat x86-avx2`
(`arm-neon` on arm64) is what the public download is built from: no mint, no sidecar, one
clone of every kernel per shipped class profile, the runtime section minted at the first start
on whatever box runs it (the Metal twin kernels race where a Metal device exists; elsewhere it
records the knob defaults). The `.das_package` names the launcher: on macOS the `.app` opens the
watchdog, so a double click supervises. The exchange levers on the control page - apply a
sidecar, share, re-tune - refuse on a fat release, which has no tune of its own.

A JIT deployment instead stages the toolchain - `main.das`, `bin/Release/daslang.exe` plus the
runtime DLLs and shared modules, `watchdog.exe`, `watchdog.json`, `control.html`, `tray.ico` -
into the target directory (`deploy-jit.ps1` does it on Windows). Either way, keep the deployed
`dasllama-server.toml` and `dasllama-server.tune.json` across upgrades, and stop a running
server first; Windows locks the DLLs.

## Endpoints

| Method | Path | Notes |
|---|---|---|
| `GET`  | `/` | Control page: live stats + charts, models panel (per-slot cards with state/GPU badges, prefix hit rate, switch telemetry, activate buttons; VRAM bar + switch strip), stream swimlane + live text cards, prefix-cache table, a chat panel (all sampling knobs, `<think>` inline, mic input - dictation under `--asr`, else the clip attaches to the next message when the slot serves native audio; under `--tts` every answer wears a speaker button that reads its content half back on the studio's voice, split at sentence boundaries into pieces the speech route accepts), a speech studio (type a line, hear it, and read the front end's normalized text and per-chunk phonemes under the waveform), config editor with the `[[models]]` roster table + save/restart, GC + drain buttons. Serves `control.html` from beside the server sources - polls `/v1/stats` + `/v1/streams` at 1 Hz |
| `GET`  | `/v1/models` | Lists every served slot (and `--asr` if loaded) - requests route on these ids via their `"model"` field |
| `POST` | `/v1/models/activate` | `{"model": name}` loopback-only admin warm-switch: make `name` the DEFAULT + stepped slot (model-less page requests follow) and move the GPU tier to it now (instead of waiting for the owner to drain). `409` while any work is live, `404` on an unknown name; `200` reports `switch_ms` + `backend_effective` |
| `POST` | `/v1/models/load` | `{"path", "id"?, "backend"?: "auto"\|"cpu", "quant"?, "ctx"?, "image_mmproj"?, "activate"?: true}` loopback-only live load: a downloaded GGUF joins as a NEW serving slot with no restart and no JIT recompile (the load blocks the tick for its duration; a prepared `.dlim` image loads in well under a second). `"auto"` follows the boot GPU policy - same want + `--ctx` clamp every boot load got, the current owner's VRAM state drops first (boot-order semantics) and re-arms if the load fails or stays off the device; `"ctx"` in the body overrides the clamp. An `image_mmproj` arms vision (and audio, when the file carries the gemma4a tower) with a media-worker bounce - a bad mmproj degrades to text-only with the reason in `tower_note`, and a load panic (corrupt GGUF, refused KV geometry) answers `400` with the slot unwound. `409` on a live stream set, a taken id, or a GGUF another slot already serves (one slot per file) |
| `POST` | `/v1/models/unload` | `{"model": name}` loopback-only: free the slot's weights, KV pool, and (for the GPU owner) VRAM. The DEFAULT slot refuses (`400`) - activate another model first - which also keeps the last model serving. `409` while any work is live |
| `POST` | `/v1/chat/completions` | Chat; `stream: true` -> SSE, else buffered; OpenAI function calling (`tools`); `image_url` content parts under `--image-mmproj`; `input_audio` content parts when the mmproj carries the audio tower (one image OR one audio clip per request, on the final user message - the soft tokens splice into the serving slot's prefill like vision, so continuous batching covers audio too) |
| `POST` | `/v1/completions` | Raw completion; `stream: true` -> SSE, else buffered |
| `POST` | `/v1/embeddings` | Mean-pooled, L2-normalized sentence embeddings |
| `POST` | `/v1/audio/transcriptions` | Speech->text (multipart upload; needs `--asr`). `response_format=verbose_json` adds timed segments |
| `POST` | `/v1/audio/translations` | Speech->English text (needs `--asr`) |
| `POST` | `/v1/audio/speech` | Text->speech (needs `--tts`): `{"input", "voice"?, "speed"?, "response_format"?: "wav" \| "pcm"}` - the OpenAI shape; `wav` (default) is 16-bit PCM at the model's rate, `pcm` the raw samples; the compressed formats answer `400` (no encoder here). One synthesis at a time on the TTS worker (its kernels run inline under `hybrid`, like the ASR workers'), 16 queued |
| `POST` | `/v1/audio/phonemes` | The front end alone (needs `--tts`): `{"model"?, "input", "voice"?}` -> `{"normalized", "lang", "chunks": [{"text", "phonemes"}]}` - the normalizer's spoken form of the text, the dialect the voice speaks (`lang`), then one row per chunk a synthesis of it would take, each carrying that chunk beside its phoneme string in that dialect. A model whose front end phonemizes ONE language reads every voice name in it - an alias, or a name it does not carry, since there is no other answer to give; a model that phonemizes several requires a voice from its `caps` and refuses an unservable one with the speech route's own 400. `model` is read the way the speech route reads it (`404` on an id that is not the served one). Answered by the TTS worker on the same queue as a synthesis (the same 4096-CHARACTER cap - codepoints, not bytes - and the same 503 when no speech model is served), so the speech studio can show what the model will actually say |
| `POST` | `/vad` | Silero speech spans over an uploaded clip (the control page's waveform overlay; in-handler, <=120 s, needs the in-repo `silero_vad.bin`) |
| `GET`  | `/catalog` | The curated model list with local presence, the `asr` tower row, the `tts` list (the three speech GGUFs and the two front-end packs, each `file`/`bytes`/`pack`/`present`/`path`), the `box` memory facts + the download state machine (`idle | downloading | verifying | done | failed`, byte progress) |
| `POST` | `/catalog/download` | `{"name": <entry>}` - start one catalog download; `{"name", "tower": "vision"}` / `{"tower": "asr"}` pull a tower, `{"tower": "tts", "file": <file>}` one file of the speech set (409 while one runs or the file exists; sha-verified, never waived) |
| `POST` | `/bench` | Loopback-only: start the benchmark, quiesced. In process by default: pp512 and tg128 on the served model, an untimed warmup then three timed reps each, one pp prefill or one tg token per tick, the text, audio and speech routes (503) and the model-switching, bake and catalog-download routes (409) holding until it finishes - in either mode; with `lcpp_bin` in the config on a source-tree daslang, the A/B child instead - our lcpp_bench then llama-bench on the same GGUF. 400 in setup mode or when the served context is shorter than pp512, 409 while a bench, a bake or a catalog download runs or streams are active, 503 while draining |
| `GET`  | `/bench` | Bench state (`idle | running | done | failed`), `mode` (`inprocess` | `ab`: what a `POST` runs), live log lines, the result JSON - `ours_pp`, `ours_tg`, `threads`, `elapsed_s`, `ts`; in process also `mode`, `pp_sd`, `tg_sd`, `reps`, `model`, `gguf`, `backend` (`metal`, `gpu:resident`, or the slot's word), `kv` (the codec the rows ran), `exec` (`exe-native` | `jit` | `interpreted`), `tune` (`fat` | `tuned` | `untuned (N of M on fallback)` | `none`) and `ref_cmd` (the llama-bench line for the comparison); the A/B also `theirs_pp`, `theirs_tg`, `pp_ratio`, `tg_ratio` and `record` - and the hardware line |
| `POST` | `/bake` | `{"model"?: name}` loopback-only: bake the slot's prepared `.dlim` image by spawning `dasllama-convert` (empty body bakes the default slot; 409 while a bake or bench runs or streams are active; the dlim GC of never-loadable images runs on completion) |
| `GET`  | `/bake` | Bake state (`idle | running | done | failed`), the slot it runs for, log lines, the result JSON |
| `GET`  | `/v1/images` | Per-slot prepared-image inventory: source GGUF path, the flavor THIS process mapped (planar/vulkan/metal, or raw gguf), the trimmed flag, and each on-disk `.dlim`'s info - plus the slot name a bake is currently running for |
| `GET`  | `/v1/stats` | Scheduler counters (`gen_tokens`, `prefill_tokens`, TTFT last/avg, ...) plus `model`/`active_model`/`ctx`/`uptime_s`/`draining` identity fields, memory footprint (`weights_bytes`, `kv_bytes`, das heaps, `gpu_vram_bytes`/`gpu_budget_bytes`), a `hardware` line (CPU * lanes * GPU), `asr_workers`, `asr_ready`, `asr_active`, `asr_pending`, speech counters (`tts_done_jobs` - syntheses served since boot, `tts_audio_s` - the speech seconds they carried), a `tts` block present ONLY while a speech model is configured - and still there when its worker could not load it (`id`, `ready`, `pending`, `done_jobs`, `audio_s`, `voices[]` and `sample_rate` as the loaded model declares them, `cloning` (the model takes a voice from a clip), `speed` (a `speed` other than 1.0 is honoured; false for a Pocket model, which refuses one) and `lang` (the first language it declares), the `lane` its worker pinned or, before there is an answer, the one the boot asked for, and `error` - the loader's reason, present only when the configured model failed to load, which the control page's offer card reads instead of claiming the model is still on its way) - the control page's speech studio gates on `ready`, media counters (`media_pending`, `media_rows`, `mrope_streams` - streams whose media rode the qwen mrope grid walk), and `models[]` - one entry per slot: `file` (source GGUF base name - the page's serve-live gate), `is_active`, `holds_gpu`, requested `backend` vs `backend_effective` (`cpu`/`gpu:rails`/`gpu:resident`), `vision` and `audio` (the towers that slot actually loaded - the catalog row's chip reads them, and `audio` is what tells the chat mic to attach a clip instead of transcribing it), per-slot cache counters, `last_used_s`, switch count/avg ms |
| `GET`  | `/v1/streams` | Per-stream poll surface: `model` (the slot it runs on), state (`queued`/`prefilling`/`decoding`/`finished`), token counts, TTFT, and capped text tails (prompt head + generated tail); finished streams linger ~10 s flagged `finished`. Plus `cache`: the prefix-cache donation chains (tokens, live pages, hits, age, preview) and `asr`: recent ASR jobs (state, audio s, wall ms, RTF) |
| `GET`  | `/config` | Effective config with per-key source (`default`/`cli`/`toml`) - one entry per row of the flags table above, `tts` (the speech model path), `tts_lane` (`q8` | `f32`) and `tts_voices_dir` (the clip directory a cloning model reads) included - plus the `[[models]]` roster, model files beside the served one, active rail (gguf vs prepared `.dlim`), GPU tier status (`supported` + `reason` when the loaded model can't ride it) |
| `POST` | `/config` | Validate a `{key: value}` JSON body and write it as an **authoritative** TOML (`authoritative = true`) to the config path (or `dasllama-server.toml` beside the program on a config-less start). Applies on the next restart |
| `POST` | `/restart` | Drain like `/shutdown`, then exit with code **4** - the watchdog relaunches, picking up the saved config (3 stays the tune-restart code) |
| `GET`  | `/exchange` | The sidecar-exchange surface: policy (url/accept/submit/configured), the consent state (`consent`: accepted/declined/empty, `consent_notice`: the first-contact text), + the current tune sidecar's identity and share state (sha, origin, box/applied_box, version gate, shared-yet) |
| `GET`  | `/exchange/matches` | Live lookup of this box against the exchange (a network call - seconds; the control page requests it explicitly) |
| `POST` | `/exchange/apply` | `{"sha": ...}`: download + validate (content sha, schema, `DASLLAMA_RELEASE`) + adopt that sidecar, then drain and exit **4** so the relaunch stamps its winners |
| `POST` | `/exchange/submit` | Privacy-strip and submit this box's own tune to the exchange (refuses exchange-sourced or foreign-box sidecars) |
| `POST` | `/exchange/retune` | Arm a local re-tune: removes the sidecar, skips the exchange once, restarts - the next boot races this box (~12 min, quiet machine) |
| `POST` | `/exchange/consent` | `{"accept": true\|false}`: record the first-contact choice; replies `{ok, accepted, restarting?}`. Accept on an untuned/stale box drains and exits **4** so the relaunch runs the lookup |
| `POST` | `/gc` | Schedule a validated collection at the next lifecycle safe point; concurrent requests coalesce |
| `POST` | `/shutdown` | Stop admitting new LLM/ASR work, drain accepted work, then exit |

Config precedence: `defaults < config TOML < explicit CLI flags` - unless the TOML carries
`authoritative = true` (what the control page saves), which flips the top: `defaults < CLI <
authoritative TOML`.

### The sidecar exchange {#exchange-policy}

Closed by default: the three config-only keys `exchange_accept` (`verified | any | off`, default
`off`), `exchange_submit` (`ask | always | never`, default `never`) and `exchange_url` are the
only way in, and every `/exchange*` route resolves them from `g_config_save_path`, the path a
`POST /config` writes, whether or not that file exists yet. The client rides the tune framework:
a build without dasLLVM registers none of it and every `/exchange*` route answers 404, and a fat
release refuses the levers (its kernels are the shipped class clones).

### The gpu key

The `gpu` key (`auto | off | metal | metal-required | vulkan`) is the first-class
backend selector, and **defaults-first: unset (with no legacy `--metal` flag) behaves as
`auto`** - the boot probes the box and serves on the best detected backend (the Metal rails
where the box has them, else the Vulkan tier when a device answers, else the CPU), logging
one `gpu backend auto-detected:` line; `gpu = off` is the explicit opt-out, and an explicit
`DASLLAMA_GPU=0` in the environment keeps its meaning - auto-detection never out-votes it
(the env master only overrides the `auto_tier` field of an explicit want, so the pick
resolves to `off` before any want is constructed). Per-model
support is unchanged - a model the armed tier cannot serve falls back to the CPU with the
reason on the control page. `gpu = vulkan` arms the MoE tier in its blessed shape - expert stacks sized
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

### Images

A slot started with `--image-mmproj` accepts the OpenAI content-parts image form. The URL must be
a `data:` URI - the server never fetches a remote URL on its request thread:

```sh
curl http://127.0.0.1:8080/v1/chat/completions -H 'Content-Type: application/json' -d '{
  "messages": [{"role": "user", "content": [
    {"type": "image_url", "image_url": {"url": "data:image/jpeg;base64,'"$(base64 < cats.jpg | tr -d '\n')"'"}},
    {"type": "text", "text": "What animals are in this picture?"}
  ]}], "max_tokens": 64
}'
```

The image is decoded and encoded on a dedicated vision worker thread - a large upload never
stalls the other streams' decode - and its soft-token rows then prefill between the two token
spans of the rendered turn, as one non-causal span. Wrapped base64 (GNU `base64`'s 76-column
default) decodes fine; the payload caps at 32 MB of file and 64 MP decoded, each a named 400.
Today's rules:

- **One image per request, on the FINAL user message.** Two images is a 400; an image anywhere
  else is dropped with a warning, so a follow-up question re-attaches it (the control page does
  this for you - an attached image rides every message until you remove it).
- A slot with no vision arm answers 400 rather than silently ignoring the picture.
- An image stream neither reads nor writes the prefix cache: its KV past the splice does not follow
  from its token ids. Everything else - batching, streaming, stops, usage - is the ordinary path,
  and `usage.prompt_tokens` counts the soft-token rows as positions.

On the control page the chat panel grows an `[#] image` button (shown when `/v1/stats` reports a
vision arm); paste and drag-and-drop work too. The panel defaults to `temperature 0` - it is a
test harness as much as a demo, so a turn someone reports should reproduce - and its `thinking`
box seeds from `/v1/stats`'s `thinking_default`, sending `enable_thinking` only once you touch it
so each family's own default stands otherwise.

### Demo load

`demo_load.das` (a sibling of the server, dogfooding the dashv client) drives varied chat
completions from worker threads - start it, then watch the control page's swimlane fill and the
tok/s stair-step:

```sh
bin/daslang utils/dasllama-server/demo_load.das -- --url http://127.0.0.1:8080   # staged ramp 1 -> 2 -> 4 -> 8
bin/daslang utils/dasllama-server/demo_load.das -- -n 4 -r 10                    # constant 4 workers
```

### Sampling parameters

Both completion routes accept the OpenAI sampling fields plus the llama.cpp-style extensions -
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
effect with `temperature > 0` - greedy short-circuits to argmax; the penalties apply in greedy
mode too. E.g. Qwen3.6's instruct-mode card settings: `"temperature": 0.7, "top_p": 0.8,
"presence_penalty": 1.5`.

### Thinking control and `reasoning_content`

`enable_thinking` is tri-state: ABSENT leaves the model template's own default in force
(thinking-capable Qwen3/3.5/3.6, GLM, gemma-4, and gpt-oss templates default on; a bare Qwen
Instruct generation branch such as Qwen3 Instruct-2507 defaults off), and a present bool -
top-level or the llama.cpp spelling
`"chat_template_kwargs": {"enable_thinking": ...}` - overrides it. `false` on a
`<think>`-family appends the template's empty think block so the model answers directly;
`false` on gemma-4 prefills the closed empty thought channel (the instruct opt-out). A no-op
for models whose vocab has no think tokens.

A thinking reply's reasoning span comes back as **`reasoning_content`** (the
DeepSeek/llama.cpp framing) with `content` clean of the family's markers: on the
`chat.completion` message for buffered requests, and as `delta.reasoning_content` chunks -
streamed before the `delta.content` chunks - for `stream: true`. Tool-calling replies split
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
the chat template on each stateless resend, so agent loops (opencode, pi, ...) work end-to-end.

The wire format is per model family (`ToolMode`, `dasllama_tools.das`): **hermes** (Qwen2.5 /
Qwen3 family - `<tools>` system block, `<tool_call>` JSON), **harmony** (gpt-oss - developer-turn
TypeScript namespace defs, commentary-channel recipient calls; reasoning and calls come from one
channel walk), **gemma4** (gemma-4 - declaration/call/response DSL with the `<|"|>` quote token),
**mistral** (v0.3+ - `[AVAILABLE_TOOLS]` defs, `[TOOL_CALLS]` array, bare-array replies
tolerated since the SPM stream suppresses control-token pieces), and **llama_json** (llama-3.x -
the whole reply is one `{"name","parameters"}` object, results on the `ipython` role). A model
whose chat template declares no tool format (GLM until its zen2 leg) gets an honest 400.
Streaming with tools buffers the native envelope and emits the parsed calls as one
`delta.tool_calls` chunk at finish.

Requests the server does NOT fully understand are visible in the log: unknown endpoints 404
through a catch-all that logs method + path + body head, and known routes warn per ignored field
(`response_format`, `stop`, multimodal content parts, ...).

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

- `test_openai_server.das` - endpoint conformance (`/v1/models`, `/v1/embeddings`, buffered chat,
  the tools-unsupported 400, the unknown-endpoint 404) over the real dashv HTTP client, plus
  model-free `parse_tool_calls` unit tests; needs `tinyllama-1.1b-chat-v1.0.Q8_0.gguf`.
- `modules/dasLLAMA/tests/test_scheduler.das` - the continuous-batching scheduler against `generate()` references (moved with the module)
  (bit-exact single stream, chunk invariance, staggered admits, eviction); needs
  `SmolLM2-135M-Instruct-Q8_0.gguf`.
- `test_openai_server_stream.das` - SSE chunk framing, the over-long-prompt 400, two concurrent
  clients batching on one server (`peak_active >= 2` via `/v1/stats`), mid-generation disconnect
  eviction, and the prefix cache returning an identical completion for a repeated request; needs
  `SmolLM2-135M-Instruct-Q8_0.gguf`.
- `test_openai_server_vision.das` - the image route end to end: a data-URI photo on
  `/v1/chat/completions` reaches the embedder, splices as soft tokens between the two rendered
  token spans, and comes back as a caption about the picture; plus the decode-failure and
  remote-URL 400s. Needs `gemma-4-12B-it-Q4_K_M.gguf`, `mmproj-gemma-4-12B-it-BF16.gguf` and the
  coco cats jpeg.
- `test_openai_server_speech.das` - the speech route end to end on a TTS-only boot: a WAV answer
  with a RIFF header of speech length, the raw `pcm` form, the declined `mp3`, the missing
  `input`, the unknown voice, the TTS id on `/v1/models`, the `tts` block on `/v1/stats`, the
  `/v1/audio/phonemes` document against the facade's own normalizer and chunker, and a
  second boot on the `f32` lane proving the pin reaches the worker. Needs `kitten-nano.gguf` and
  the front-end packs beside it.
- `test_exchange_client.das` - the exception: model-free and runs everywhere. The sidecar
  exchange client against a fake exchange on 127.0.0.1:18131 (lookup/pick, the fetch-and-apply
  gate, applied_box staleness, the privacy strip, both submit rails, policy parsing).
- `test_model_catalog.das` - model-free: catalog-table invariants (pinned URLs, unique ids,
  one default), the models-dir precedence, presence detection, download-start refusals.
- `test_setup_mode.das` - model-free, runs end to end even interpreted (setup mode never
  infers): a slotless boot serves setup stats and the catalog while every inference route
  fails closed.
- `tests/` - the control page itself, under real Playwright (Node + chromium): badge states,
  models panel, streams/history, chat wire + SSE rendering, the speech studio, config editor,
  exchange section, the confirm-gated controls. Model-free - the page runs against JSON/SSE fixtures captured
  from a real server (`tests/fixtures/README.md` is the regeneration rail). Run with
  `npm ci && npx playwright test` in `tests/`; CI: `.github/workflows/dasllama-server-e2e.yml`.

```sh
bin/daslang -jit dastest/dastest.das -- --test utils/dasllama-server/test_openai_server.das
```

`server_bench.das` (same directory) measures the serving latencies directly through the scheduler
seam: tok/s + TTFT + inter-token percentiles vs batch size, decode stall per prefill chunk size,
and warm-vs-cold TTFT for the prefix cache.

## Not yet implemented

The request's `stop` / `response_format` fields and the forced-function `tool_choice` object form
 - all logged when a request carries them. On the media path: more than one media clip per request,
media on earlier turns of a conversation, and remote `image_url` fetches.
