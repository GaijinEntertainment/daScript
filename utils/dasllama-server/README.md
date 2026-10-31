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

The server ships as the `dasllama` package, cut on its own release tags (`dasllama-vX.Y.Z`,
`dasllama-vX.Y.Z-RCn` for a candidate), independent of the daslang SDK's:

| Platform | Install |
|---|---|
| macOS, Apple silicon | `brew install --cask borisbat/daslang/dasllama` (the tray app), or `brew install borisbat/daslang/dasllama` (the commands) |
| Windows x64 | `scoop bucket add daslang https://github.com/borisbat/scoop-daslang`, then `scoop install dasllama` |
| Debian, Ubuntu | `sudo apt install ./dasllama_<version>_<arch>.deb`, from the release page |
| Fedora | `sudo dnf install ./dasllama-<version>-1.<arch>.rpm`, from the release page |
| Linux (Homebrew) | `brew install borisbat/daslang/dasllama` |
| any, with Python | `pip install dasllama` |

Or unpack an archive from the rolling `dasllama-server` release:
<https://github.com/GaijinEntertainment/daScript/releases/tag/dasllama-server>

| Platform | Asset |
|---|---|
| macOS, Apple silicon | `dasllama-darwin-arm64.zip` |
| Windows x64 | `dasllama-windows-x64.zip` |
| Linux x86_64 | `dasllama-linux-x86_64.tar.gz` |
| Linux arm64 | `dasllama-linux-arm64.tar.gz` |

Every form carries the same four programs, on PATH as `dasllama-server`, `dasllama-cli`,
`dasllama-bench` and `dasllama-watchdog` wherever a package manager installed them. Inside an
archive they are `dasllama-server.exe`, `dasllama-cli.exe`, `dasllama-bench.exe` and `watchdog`
(`watchdog.exe` on Windows) on Linux and Windows alike; the macOS `.app` carries them without the
suffix in `Contents/MacOS`.

The Linux builds need glibc 2.35 or newer (Debian 12, Ubuntu 22.04 and later), the system
OpenSSL 3 (`libssl3` on Debian and Ubuntu, `openssl-libs` on Fedora) and `curl` for the catalog
downloads - the `.deb` and `.rpm` pull them in; the macOS and Windows builds carry their own
OpenSSL.

Start the supervisor - `dasllama-watchdog` (`watchdog` in an archive), or on macOS the
`dasllama-server.app` itself, whose launcher is the watchdog; `brew services start dasllama`
starts it at login. The watchdog keeps the server up, puts the dasllama mark in the notification
area, and a click on it opens the control page, <http://127.0.0.1:8080/>. With no model
configured the server starts in **setup mode** (below): pick a model from the catalog, it
downloads into `~/.dasllama/models`, and *serve this model* restarts into it.

Everything the programs write lives under `~/.dasllama` (`%USERPROFILE%\.dasllama` on Windows),
never beside them, so an installed copy stays read-only and an upgrade keeps it all:

| Path | What |
|---|---|
| `models/` | the catalog downloads, and the `.dlim` images baked beside them |
| `dasllama-server.toml` | the config the control page saves |
| `tune/<program>.tune.json` | the box's runtime section - the Metal kernel crowns raced at the first start, the knobs |
| `logs/` | `dasllama-server.log`, `dasllama-cli.log`, the watchdog's log and crash bundles, libhv's daily log |

A config is looked for in the working directory first, then in `~/.dasllama`, then beside the
program; `--config` names one outright. A config or sidecar an earlier download kept beside the
exe still reads - the sidecar is copied to `tune/` at the first start.

The exe is a fat build (`daspkg release --fat`): plain code for the platform's baseline CPU
class - `x86-avx2` on x86, `arm-neon` on arm64 - with one clone of every `[tune]` kernel per
class the engine ships a profile for (`x86-vnni512`, `x86-amx`, `arm-i8mm`), picked from cpuid
at start. On a Mac the Metal crowns are raced once at the first start and kept in `~/.dasllama/tune`.
That is the good default, and it is as far as a solid executable goes: **a fat build cannot
tune**. Its kernels are baked, there is no tuner in it, and `--tune`, the sidecar exchange
and a re-tune have nothing to act on (the control page's exchange levers refuse). To tune
every kernel for this box, install daslang with dasLLAMA - the SDK - and run the server
through the JIT, `bin/daslang -jit utils/dasllama-server/main.das` below, or `daspkg release`
this package on that box: either mints the box's own sidecar, every kernel raced on your own
hardware instead of picked from a class profile.

## Measure your box

The control page's *benchmark* button runs the llama-bench rows on the served model right in
the server - pp512 and tg128, three timed reps each after an untimed warmup, and on a slot that
serves two streams or more tg128@N: N streams (up to four) through the scheduler - device-home
where a whole-model driver homes them, host-cached otherwise - the summed rate - and shows tokens per second beside the hardware line; every route that could contend holds for the run's
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
know about: on a slot the Vulkan whole-model driver serves from the device, the rows run on a
device-home session, the path that slot's streams take, in the driver's KV codec; on any other
slot they run llama-bench's flat session. The result says `gpu:resident` when the driver served
the rows, and the log carries a warning when any of their calls ran on the CPU instead. A pp rep on a large CPU model can outlast the watchdog's two-second health
probe; the tray reads unhealthy for that rep and recovers on its own.

The bundles are not code-signed. macOS quarantines a downloaded app, and an app started from
the download folder runs from a read-only copy where nothing it writes beside itself survives -
so first drag `dasllama-server.app` out of Downloads with the Finder (Applications, or any folder
of yours), then either allow it under System Settings > Privacy & Security after the first
refused start, or clear the flag once - `xattr -dr com.apple.quarantine dasllama-server.app`.
Windows SmartScreen shows an unknown publisher: *More info*, *Run anyway*.

## The command line: dasllama-cli

`dasllama-cli` sits beside the server in every bundle (`dasllama-cli.exe` on Linux and Windows,
`dasllama-server.app/Contents/MacOS/dasllama-cli` on a Mac) and from the source tree runs as
`bin/daslang -jit utils/dasllama-server/cli.das -- <command> ...`. It is the same engine, the
same facade and the same backend pick as the server, without the HTTP layer: one model, one
conversation, one process, so a shell script or a cron job can ask a model, hear a clip or speak
a line without a server up.

| Command | What it does | Its flags |
|---|---|---|
| `complete` | complete a prompt, streamed; `--image <file>` with `--image-mmproj` asks about a picture | `--prompt` / `-p` (or `--file` / `-f`, or the first bare argument), `--image`, `--image-mmproj`, `--quiet` (the completion alone, no counters) |
| `chat` | a conversation from the terminal (/help lists the slash commands: /image, /audio, /read, /save, /load, /regen, /clear, /system, /stats, /speak on), or from `--script <file>` one message per line; `--tts` speaks every reply to `reply_<n>.wav` | `--system`, `--script`, `--image-mmproj` (arms /image), `--audio-mmproj` (arms /audio; the E-series mmproj serves both, pass it twice), `--tts`, `--voice` / `-v`, `--speed`, `--out-dir` (where `reply_<n>.wav` land, default the cwd), `--hide-thinking` (a thinking model's reasoning streams before the answer by default), `--no-think` (answer directly on a hybrid model), `--quiet` |
| `transcribe` | an audio file to text through an ASR model (`--asr`, `--mmproj` for a GGUF pair) | `--asr` / `-a`, `--mmproj`, `--file` / `-f` (or the first bare argument), `--out` / `-o` (the transcript to a file instead of stdout), `--lang` / `-l` (a hint where the model takes one; a self-detecting model refuses it) |
| `speak` | text to a 16-bit WAV through a TTS model (`--tts`, `--voice`, `--speed`) | `--tts`, `--text` (or `--file` / `-f`, or the first bare argument), `--out` / `-o` (default `out.wav`), `--voice` / `-v` (an unknown voice is refused with the model's voices), `--speed` (refused on a model that takes none), `--tts-lane q8 \| f32`, `--prof` (the generator's per-op profile) |
| `talk` | the chain: `--in <audio>` heard by `--asr`, answered by `--model`, spoken by `--tts` into `--out`; `--prompt <text>` skips the hearing | `--asr` / `-a`, `--mmproj`, `--in` / `-i` (or the first bare argument), `--prompt` / `-p`, `--system`, `--lang` / `-l`, `--tts`, `--voice` / `-v`, `--speed`, `--out` / `-o` (default `reply.wav`), `--no-think`, `--quiet` (no transcript, stage times or counters) |
| `translate` | type (or say) a sentence, read its translation: a Firefox Translations model (`model.<pair>.intgemm.alphas.bin`, its `vocab.<pair>.spm` beside it; one direction) or a translation LLM GGUF (MiLMMT-46; any direction among its 46 languages) from the terminal, the microphone or a `--script`, one sentence a line; /langs picks an LLM's direction, with `--tts` every translation is spoken into `translation_<n>.wav` under `--out-dir` from the start, /speak off and /speak on toggle it, /exit quits | `--nmt` (or the first bare argument), `--from` / `--to` (an LLM's direction, `--to` default `en`), `--script`, `--exact` (the f32 lane), `--tts`, `--voice` / `-v`, `--speed`, `--out-dir`, `--quiet` (no timing), and the microphone flags |
| `embed` | a text's embedding vector, one float per line or `--json` | `--text` (or `--file` / `-f`, or the first bare argument), `--json` |
| `tokenize` | a text's token ids and pieces | `--text` (or `--file` / `-f`, or the first bare argument), `--ids` (the ids alone, space-separated), `--no-special` (no BOS / leading specials) |
| `bench` | the llama-bench rows on a model - `pp512`, `tg128`, and `tg128@N` with `--npl N` - `-o md` for the table; `dasllama-bench` stays the records instrument | `--plen` / `-p` (default 512), `--ngen` / `-n` (default 128), `--reps` / `-r` (default 5), `--npl` (streams of the batched row), `--output` / `-o` `txt \| md` |

Every example below runs as written on a box whose models directory carries the named files
(`utils/dasllama-server/test_cli.das` runs each one):

```sh
dasllama-cli complete -m SmolLM2-135M-Instruct-Q8_0.gguf --quiet "The capital of France is"
dasllama-cli speak --tts kitten-nano.gguf -o hello.wav "Hello from the command line."
dasllama-cli transcribe --asr Qwen3-ASR-0.6B-Q8_0.gguf --mmproj mmproj-Qwen3-ASR-0.6B-bf16.gguf question.wav
dasllama-cli talk --asr Qwen3-ASR-0.6B-Q8_0.gguf --mmproj mmproj-Qwen3-ASR-0.6B-bf16.gguf --model Qwen3.5-0.8B-Q8_0.gguf --tts kokoro-82m.gguf --in question.wav --out answer.wav
dasllama-cli translate ru-en/model.ruen.intgemm.alphas.bin --tts kitten-nano.gguf --script sentences.txt
dasllama-cli chat -m gemma-4-E2B-it-Q4_K_M.gguf --image-mmproj mmproj-gemma-4-E2B-it-bf16.gguf --audio-mmproj mmproj-gemma-4-E2B-it-bf16.gguf
dasllama-cli bench -m SmolLM2-135M-Instruct-Q8_0.gguf --npl 2 -o md
```

Every command takes the shared flags the server takes for the same knobs, with the server's
meanings and defaults (the table under *Run from the source tree*): `--config` / `-c`, `--model` /
`-m`, `--quant` / `-q`, `--kv-dtype`, `--gpu auto | off | metal | metal-required | vulkan` (the
same auto pick: Metal on a Mac, Vulkan on a card, CPU otherwise; `--gpu vulkan` serves a model
that fits the card whole from the device, the conversation's cache with it), `--metal`, the Vulkan
detail knobs `--gpu-layers`, `--gpu-stream`, `--gpu-dn`, `--gpu-attn`, `--gpu-dense`,
`--gpu-vram-mb`, `--ctx`, the RoPE scaling override `--rope-scaling yarn | linear | none` with
`--rope-scale` and `--yarn-orig-ctx` (the model file's own keys by default; Qwen: `--rope-scaling
yarn --rope-scale 4`), `--threads` / `-t`, `--models-dir`, `--tune`, and two of its own:
`--verbose` echoes the engine's log records to stderr as they land, and `--help` (`-?` or
`--show-help` from the source tree: the daslang host takes `--help` for itself) lists a command's
flags with the groups it takes. `complete`, `chat` and `talk` take the sampler - `--temp`,
`--top-k`, `--top-p`, `--min-p`, `--repeat-penalty`, `--presence-penalty`, `--frequency-penalty`,
`--seed` / `-s`, `--max-tokens` / `-n` - and a knob left unset takes the command's default: greedy
for `complete`, temperature 0.7 / top-k 20 / top-p 0.95 for `chat` and `talk` (under greedy
decoding a thinking model loops on its own draft). `chat`, `speak`, `talk` and `translate` take the speaker
flags: `--play` also plays each spoken reply through the speaker once its WAV has landed (the
run blocks until the clip ends; a box with no device says so and keeps writing the files), and
`--null-audio` drives the null backend for a test box. `chat` and `translate` take the microphone
flags: `--mic vad` listens hands-free (each utterance ends at a pause and becomes the next line),
`--mic ptt` records from an Enter on an empty line to the next Enter (typing still works); `--asr`
(default: the config's) hears it, `--asr-mmproj` is that GGUF ASR's mmproj (default: the config's mmproj when the ASR is the config's), and `--lang` / `-l` is its
language hint - `translate` defaults it to the translator's source language. Each heard line is
echoed as `you: ...`, and the capture is dropped while a reply is spoken. A bare model name resolves under
`--models-dir` (`~/.dasllama/models`, where the catalog downloads land). The `dasllama-server.toml`
in the current directory, else in `~/.dasllama`, else beside the program - the server's own lookup - fills whatever
the flags leave empty - the
model (a `[[models]]` roster's default entry included), its `image_mmproj` and `audio_mmproj`, the `asr` and `tts`
models (an `[[asr]]` / `[[tts]]` roster's first table), the backend, the lane cap - so on a box the setup page configured, `dasllama-cli chat`
with no flags talks to the served model on the served backend; explicit flags win, `--config`
names another file. The answer goes to stdout alone, so it pipes; the CLI's progress lines and
the token counters go to stderr (`--quiet` drops the counters); the engine's own notices - a
missing Metal profile, a GPU decline - land in `dasllama-cli.log` (`~/.dasllama/logs` from an
installed build, the das root's `logs/` from the source tree), the way the server's do, and `--verbose` echoes them to stderr as they land (from the source tree the
tune policy guard still prints its one status line first). A fat bundle's first `dasllama-cli` run on a Mac races the Metal crowns once into
`~/.dasllama/tune/dasllama-cli.tune.json`, like the server's does, and never again; from the source
tree it keeps its own tune sidecar beside `cli.das`.

## Run from the source tree

```sh
bin/daslang -jit utils/dasllama-server/main.das -- --model <model.gguf> [--port 8080] [--quant q8] \
                                                    [--asr <asr.bin>] [--asr-workers 2] [--mmproj <mmproj.gguf>] \
                                                    [--image-mmproj <mmproj.gguf>] [--audio-mmproj <mmproj.gguf>] [--ctx 4096] \
                                                    [--streams 4] [--chunk 512] [--chunk-idle 2048] [--page-rows 64] [--prefix N] \
                                                    [--prefix-states N] [--prefix-state-mb 0]
```

Run under `-jit` - the interpreter is refused, it is far too slow for inference. Flags:

| Flag | Short | Default | Meaning |
|---|---|---|---|
| `--config` | `-c` | *auto* | TOML config file; keys mirror the long flag names, explicit CLI flags override. Without the flag, a `dasllama-server.toml` in the cwd, in `~/.dasllama` or next to the program loads automatically |
| `--model` | `-m` | *(required)* | GGUF model to serve (here or in `--config`); an embedding GGUF (one naming its pooling) serves `/v1/embeddings` from its own worker and is never the default |
| `--port` | `-p` | `8080` | Listen port |
| `--host` | - | `127.0.0.1` | Interface to listen on: this machine only by default; `0.0.0.0` opens the server to every device on the network - the operator's choice, never the default |
| `--quant` | `-q` | `q8` | Weight quantization: `fp32` \| `q8` \| `q4` - plus the loader's file-format spellings `q4_k` \| `q5_k` \| `q6_k` \| `mxfp4` \| `f16` \| `bf16` (all serve on the `q8` kquant-native tier) |
| `--gpu` | - | `auto` | GPU backend: auto (default: metal, else the home `DASLLAMA_CUDA` spells, else vulkan when a Vulkan device answers, else cuda when the CUDA home answers, else CPU) \| off \| metal \| metal-required \| vulkan \| cuda. vulkan and cuda name the discrete home: the whole-model driver serves a model that fits the card whole from the GPU, every stream's cache with it; the control page's model card says what a slot got (a set --metal flag keeps the legacy env-driven path). Details: *The gpu key* below |
| `--metal` | - | `off` | Metal serving mode when `--gpu` is unset: `off` \| `auto` (the GPU when a path's gates pass, the CPU otherwise) \| `required` (a call the GPU cannot serve is a hard failure). A set flag keeps the legacy env-driven path |
| `--gpu-layers` | - | *auto* | vulkan: resident MoE expert-stack layers, offloaded from the end (config key `gpu_layers`; *The gpu key* below) |
| `--gpu-stream` | - | *auto* | vulkan: streamed prefill MoE layers below the resident set (config key `gpu_stream`) |
| `--gpu-dn` | - | on | vulkan: deltanet (recurrent) layers through the device chain |
| `--gpu-attn` | - | on | vulkan: full-attention layers through the device chain |
| `--gpu-dense` | - | off | vulkan: dense attention-side planes resident |
| `--gpu-vram-mb` | - | *device* | vulkan: resident-weight VRAM cap override in MB (default: query the device) |
| `--kv-dtype` | - | `f16` | KV-cache codec: `f32` \| `f16` \| `q8_0` \| `tq4` (rotated 4-bit; needs pow2 head_size). Under `--gpu vulkan` the whole-model driver holds its cache in the same codec for `f16`, `q8_0` and `tq4`: `q8_0` is about half the `f16` cache's VRAM and `tq4` about a quarter, which a longer `--ctx` or more resident layers take |
| `--asr` | `-a` | - | ASR model (whisper/parakeet/gigaam/qwen3-asr) - enables the `/v1/audio/*` routes. Repeatable: each model serves under its file's stem, the first is the default (`--mmproj` and `--asr-workers` are the first one's); the config's `[[asr]]` tables give each model its own `id`, `mmproj` and `workers` |
| `--asr-workers` | - | `1` | Long-lived ASR request threads; each owns a model and reusable session. Set `2` for two parallel transcriptions. With several ASR models, the first one's |
| `--mmproj` | - | - | mmproj GGUF for the Qwen3-ASR route (paired with `--asr`; with several ASR models, the first one's) |
| `--tts` | - | - | TTS model GGUF (`kitten-nano`, `kitten-mini`, `kokoro-82m`, with the front-end packs `tts_g2p.bin` + `tts_postag.bin` beside them; or a `pocket-tts-<lang>-q8` file, which reads text, needs no pack and clones a voice) - enables `/v1/audio/speech`. Repeatable: each model serves under its file's stem, the first is the default (`--tts-lane` and `--tts-voices-dir` are the first one's); the config's `[[tts]]` tables give each model its own `id`, `lane` and `voices_dir` |
| `--tts-lane` | - | `q8` | Weight lane the speech worker pins around its load: `q8` (the prepared quant image beside the GGUF) or `f32` (the file's own planes - the reference lane). An unknown spelling warns and serves `q8`. With several TTS models, the first one's |
| `--tts-voices-dir` | - | - | Directory of voice clips (wav / flac / mp3 / ogg, a few seconds of one speaker each, 60 s at most) a cloning speech model adds to its voices at boot (with several TTS models, the first one's), each under its file's stem (`mine.wav` -> voice `mine`; a stem the model already carries replaces that voice, with a log line). A clip that does not decode or runs past 60 s is logged and skipped; the key on a model that cannot clone is logged and ignored |
| `--translate` | - | - | Translation models, comma-separated: a Firefox Translations `model.<pair>.intgemm.alphas.bin` (its `vocab.<pair>.spm` beside it; one direction, its pair from the file name; listed under its directory's name) or a translation-LLM GGUF (MiLMMT-46: any two of its 46 languages; listed under its file's stem) - enables `/translate` and `/languages` (the LibreTranslate routes) and the `target` field of `/v1/audio/translations`. Each model loads on a worker thread of its own (the TTS worker's dispatch); a pair goes to the first model that serves it. A missing file is logged and left out, a model whose worker cannot load it serves no pair and the `translate` block of `/v1/stats` carries the reason; either way the boot serves on. Config key `translate`: the same comma-separated string, or an array of paths |
| `--image-mmproj` | - | - | Vision mmproj (gemma4uv, gemma4v, or gemma3v, sniffed) for the default model - enables `image_url` parts on `/v1/chat/completions`. Per-model in a `[[models]]` roster: `image_mmproj = "..."`. When the file also carries an audio encoder (the gemma-4 E-series and the Qwen Omni mmprojs carry both towers), the same flag arms **native audio**: `input_audio` parts serve through the same slot - one decoder, one mmproj, no dedicated ASR model copy |
| `--audio-mmproj` | - | - | Audio mmproj for the default model - enables `input_audio` parts on `/v1/chat/completions`: a whisper-class tower with its projector (Qwen2-Audio, Qwen2.5-Omni, Ultravox, Voxtral) or the gemma-4 Conformer. Per-model in a `[[models]]` roster: `audio_mmproj = "..."`. Not needed when `--image-mmproj` names a file that carries the audio tower too |
| `--ctx` | - | *model* | Context length in tokens, served whole: the whole-model GPU driver holds it for every stream or declines to the per-op rails, and the load log names the room. Default: the model's trained `context_length`, shortened to what the card holds (the log says by how much and why). A Metal model served from its image alone (no host weights) is cut to the rows its device KV mirror holds (`DASLLAMA_METAL_KV_MIRROR_MB`), with a warning - a boot load and a live `/v1/models/load` alike. |
| `--max-tokens` | - | `16384` | Default reply token budget when a request omits `max_tokens` (clamped to `--ctx` per request) |
| `--streams` | `-s` | `4` | Max concurrent generation streams |
| `--threads` | `-t` | `16` | Worker-lane cap for the matmul dispatch (`-1` = all cores) - decode is bandwidth-bound, so an uncapped dispatch just fights the rest of the box |
| `--team-dispatch` | - | `hybrid` | `hybrid`: LLM uses the worker team; the ASR workers use it too when every LLM slot decodes on a GPU (Metal, or the whole model resident on a Vulkan device - the team is idle there, and a transcription runs at twice its inline rate) and run inline otherwise; the TTS worker runs inline; `team`: all callers use serialized team publishes; `inline`: every caller runs independently |
| `--affinity` | - | `-1` | Worker CPU affinity: `-1` = the platform default (QoS on darwin, off elsewhere), `0` = off, `1` = an ideal-CPU hint, `2` = a hard mask (`DAS_JOBQUE_AFFINITY` overrides) |
| `--chunk` | - | *backend* | Prefill quantum in tokens while a stream is decoding - decode stalls at most this per tick. Default: 512 where a GPU prefills (a window's cost is mostly fixed there, so a small chunk runs a prompt at a fraction of the window's rate), 64 on the CPU (a chunk costs its token count, so a short one keeps the stall short at 13 - 29% of the prompt's rate) |
| `--chunk-idle` | - | *backend* | Prefill quantum in tokens while no stream is decoding (never under `--chunk`): nothing waits on the tick, so the window can be wide. Default: on a GPU the wider of 2048 and the driver's prompt window, 512 on the CPU |
| `--page-rows` | - | `64` | KV page size in positions for paged serving |
| `--prefix` | - | *auto* | Prefix-cache retention cap in pages (auto: one full context per stream; `-1` = unbounded) |
| `--prefix-states` | - | *auto* | Recurrent models (the Qwen3.5/3.6/3.8 hybrids) cache whole checkpoints instead of pages: the most a slot keeps (`0` = none; `-1`, or unset, = the byte budget's count; config key `prefix_states`). Unset, the count is the byte budget over the model's recurrent state, 256 at least and 1024 at most: the budget binds under the cap, the cap once the budget would hold more (a flat count under the budget evicts a conversation's message-end stops while bytes sit free). Every request leaves one at its opening's shared head (the system text or the tool block, whichever its template writes first), one at its opening with the tools, one where its last message starts, one where it ends (ahead of the generation prompt), one at its finished turn; a conversation's own stops are dropped before shared openings |
| `--prefix-state-mb` | - | *auto* | The checkpoints' byte budget in MB, snapshots and held pages (auto: a quarter of the box's RAM - in a container, of its memory limit where that is lower; config key `prefix_state_mb`). A 35B hybrid's checkpoint is about 160 MB. The server log's checkpoint line carries the standing count and MB |
| `--flat` | - | - | Flat preallocated KV sessions - disables paged serving and the prefix cache |
| `--mtp` | - | *off* | MTP/NextN self-speculative decode. Unset, a slot leaves it off, except one that runs one stream (`streams = 1`) on a GPU the model's catalog row lists under `serve_mtp` (`modules/dasLLAMA/performance/model_specs.das`: the platforms, `metal` or `vulkan`, where that model's self-speculation measured a win - gemma-4-26B-A4B and Qwen3.8-27B on Metal and on a Vulkan device, Qwen3.6-35B-A3B and Qwen3.8-Flash-Next on a Vulkan device; `modules/dasLLAMA/PERF_LEDGER.md` carries the readings) - Metal, or the whole model resident on a Vulkan device. A model with no catalog row, a model whose row lists another platform, several streams and the CPU decode plain: on the CPU the round's second verify row costs a second decode step, at several streams the plain batched step is faster. A MoE whose routed experts the resident driver sums on the host arms too, at the box's NextN depth (1), where the round measured 1.07-1.22x (the ledger's readings per model). A drafting Vulkan slot keeps its streams' caches on the card (device-home) where the whole-model driver runs the draft head, its verify and a rejected round's rollback on the device; where any of them stays on the host (the load log names which), the slot serves host-cached sessions. `true` / `false` set it outright. It needs a model with an in-file NextN head (the `-MTP-` GGUFs), or an assistant drafter GGUF beside the model file (gemma-4: `mtp-<stem>-Q8_0.gguf`, `<stem>` the file name before its quant token, an unsloth `-UD` tag dropped; the catalog's gemma-4 26B card fetches it with `"tower": "drafter"`), which the load attaches on Metal or on the Vulkan resident driver and which drafts at the box's `runtime.mtp_depth_assistant` depth; on any other model the server logs one line and serves plain. Greedy requests are output-invariant; a sampled request (`temperature` > 0, penalties included) draws each verify row with its own sampler and keeps the plain sampled distribution, at a lower acceptance rate. `/v1/stats` reports `mtp_drafted`/`mtp_accepted`; a constrained reply (`response_format`, a forced `tool_choice`) decodes plain |
| `--mtp-accept` | - | *engine* | The sampled speculative round's acceptance, process-wide: `rejection` (the engine's default - each draft accepted with the probability the verify row gives it over the drafter's recorded q, a rejected draft replaced by one draw over the residual), `typical` (a draft accepted wherever the sampler's own cuts admit it on its row, the first cut draft replaced by one draw over the survivors) or `exact` (sample-and-match: a draft accepted only where the row's seeded draw lands on it). A greedy stream is unchanged. Unset keeps the engine's default |
| `--rope-scaling` | - | *file* | RoPE scaling override for the load: `yarn` \| `linear` \| `none`; unset keeps the model file's own `rope.scaling.*` keys, `none` drops them (a file's per-pair factor tensors, Llama-3.1's `rope_freqs`, stay, as llama.cpp keeps them). `yarn` folds the NTK-by-parts frequency ramp and the `1 + 0.1 ln(s)` magnitude into the rope tables the way llama.cpp's `--rope-scaling yarn` does. The Qwen families publish the recipe (Qwen2.5-Instruct 7B and up, Qwen3, Qwen3-Next / 3.5 / 3.8: factor 4 over the trained context) and ship no scaling keys because static YaRN costs a little on short texts - arm it when a conversation needs the length; no other vendor validates it, and a non-Qwen file logs a warning. The override is baked into the prepared image under its own lane (`model.gguf.metal-yarn4.dlim`), so the first load with it mints once. Per-model in a `[[models]]` roster: `rope_scaling = "yarn"` |
| `--rope-scale` | - | *file* | The scaling factor `s` (the context multiplier) for the override; unset reads the file's `rope.scaling.factor`, and `yarn` needs one (config key `rope_scale`) |
| `--yarn-orig-ctx` | - | *file* | YaRN: the original training context the factor extends; unset reads the file's `original_context_length`, else its `context_length` (config key `yarn_orig_ctx`; llama.cpp's `--yarn-orig-ctx`) |
| `--lcpp-bin` | - | - | Path to a llama.cpp `llama-bench` binary: the control page's benchmark button then runs the A/B child (our bench, then llama-bench on the same GGUF) instead of the in-process rows; source-tree daslang only (config key `lcpp_bin`) |
| `--models-dir` | - | `~/.dasllama/models` | Where the model catalog downloads land (`DASLLAMA_MODELS_DIR` overrides both this and the config key) |
| `--tune` | - | - | Re-tune this box's dasLLAMA kernels, then relaunch (the JIT run; a fat build carries no tuner and ignores it) |
| `--help` | `-?` | - | Show help and exit |

A config file replaces long command lines; keys are the long flag names with underscores.
A `dasllama-server.toml` in the cwd, in `~/.dasllama` (where the control page saves it) or next
to the program loads automatically, first found wins; `--config other.toml` picks a different
one.

```toml
model = "D:/models/SmolLM2-135M-Instruct-Q8_0.gguf"
port = 8080
host = "127.0.0.1"   # the interface: this machine only; "0.0.0.0" opens it to the network
quant = "q8"
kv_dtype = "tq4"   # rotated 4-bit KV - half the q8_0 cache bytes
ctx = 4096
max_tokens = 4096  # default reply budget for clients that omit max_tokens (e.g. `llm chat`)
streams = 4
threads = 16       # matmul dispatch lane cap; -1 = all cores
team_dispatch = "hybrid" # LLM team dispatch; ASR workers on the team beside a GPU-decoding LLM, inline beside a CPU one; TTS inline
asr_workers = 2    # two independent transcription requests; each worker owns an ASR model
rope_scaling = "yarn"    # Qwen past its trained context: YaRN, factor 4 over that context
rope_scale = 4
```

Several speech models serve side by side, each on worker threads of its own, so a request to one
never waits behind another. `asr` and `tts` are then rosters instead of paths - a table a model,
the first the default (the model a request with no `model` field gets):

```toml
[[asr]]
path = "models/parakeet-tdt-0.6b-v3.bin"

[[asr]]
id = "qwen-asr"                              # the id requests name; default: the file's stem
path = "models/Qwen3-ASR-1.7B-Q8_0.gguf"
mmproj = "models/mmproj-Qwen3-ASR-1.7B-bf16.gguf"
workers = 2

[[tts]]
path = "models/kokoro-82m.gguf"

[[tts]]
path = "models/pocket-tts-en-q8.gguf"
voices_dir = "voices"
lane = "q8"
```

With a roster, the flat `mmproj` / `asr_workers` / `tts_lane` / `tts_voices_dir` config keys are not
read - each table carries its own - while the same flag on the command line overrides the first
table's value, as a flag overrides any key of a config that is not authoritative. Two models may not share an id: the second is logged and left out.

Translation models ride the same speech thread, a worker each; `translate` lists them, and a boot with
only translation models serves the translation routes with no LLM:

```toml
translate = ["models/bergamot/ru-en-base-memory/model.ruen.intgemm.alphas.bin",
             "models/milmmt/MiLMMT-46-1B-v0.1.Q4_K_M.gguf"]
```

## Setup mode and the model catalog

A start with **no LLM model at all** (no `--model`, no config, or every configured path
missing) **and no servable `--tts` or `--asr`** does not exit - it boots into **setup mode**: the port
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

**`--tts` or `--asr` alone is a serving start, not a setup start.** A speech-only server has no
LLM slot, so `/v1/stats` answers the slotless shape - but its `setup` reads `false`, its ASR and
TTS counters are live, and the page keeps its serving surface instead of leading with the catalog.

**An ASR model whose decoder is an LLM decodes where the server's LLMs do.** A two-file model
(`--asr` with `--mmproj`: Qwen3-ASR, Canary, the gemma audio route) takes the boot's Metal mode in
its worker; a one-file model (whisper, parakeet, gigaam) serves on its own GPU seat when the build
has one and the seat serves the model (`metal` on a Metal build, `vulkan` on a Vulkan one), and
on the CPU otherwise. `/v1/stats` says which under `asr.models[]` - `id`, `workers`, `ready`, and
`decoder` (`metal`, `vulkan` or `cpu`).

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
`POST /catalog/download` takes `{"name", "tower": "vision"}`, `{"name", "tower": "drafter"}` (the gemma-4 assistant drafter sidecar, the accepted reply's id `drafter:<name>` with the sidecar's size) or `{"tower": "asr"}` on the
same one-at-a-time rail. A **speech** strip sits beside it for the text-to-speech set the
`/catalog` document's `tts` list carries: a model on disk that can speak goes straight to
**enable speech**, which wires the `tts` key (a Pocket file stands alone; a phoneme family
needs the two front-end packs beside it - `needs_packs` on the row); otherwise the packs first,
then one model - `{"tower": "tts", "file": <file>}` pulls one file of that set on the same rail.
Setup-mode
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
`kv_dtype`, `streams`, `chunk`, `chunk_idle`, `page_rows`, `prefix`, `mtp`, `rope_scaling`, `rope_scale`,
`yarn_orig_ctx` override per model:

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

An entry whose GGUF names its pooling (the `<arch>.pooling_type` key - Qwen3-Embedding carries it,
a chat model does not) is an **embedding model**: it loads once, on a worker thread of its own
(its `quant` and `ctx` apply; an unset `ctx` keeps the file's trained context capped at 8192
tokens, and an explicit one sets it), never as an engine slot - it serves `/v1/embeddings` alone (see
Embeddings) and is never the default, whether `--model` or `default = true` names it: the boot
logs a warning and the first chat entry is the default; with no chat entry the server serves
`/v1/embeddings` alone.

Chat and completion requests **batch continuously** (`dasllama/dasllama_scheduler.das`): up to `--streams`
generations run concurrently through one `eval_batch` decode step per tick, with long prompts
prefilled in `--chunk`-token slices so a new arrival never stalls running streams for more than
one chunk (with no stream running, the slices are `--chunk-idle` tokens). Requests beyond `--streams` queue (up to 32; then 503). KV is **paged** by default -
cache memory tracks each stream's actual context, and finished streams donate their pages to a
**prefix cache**, so a repeated prompt prefix (a shared system prompt, the next turn of the same
conversation) attaches instead of re-prefilling - time-to-first-token collapses on warm prompts.
The match is the longest common prefix at any length: a request that repeats an earlier one's
opening and changes a word reuses every token before the word (`usage.prompt_tokens_details.cached_tokens`
says how many). A recurrent model (the Qwen3.5/3.6/3.8 hybrids) has a state at one position only, so its
cache holds whole checkpoints instead of pages: every request leaves one at its opening's shared head (the system
text ahead of a Hermes tool block, the tool block ahead of a Qwen3.5/3.6 system text), one at the opening every
request on its system prompt and tool set shares, one where its last message starts (the history the requests of
a conversation share), one at its last message's close, ahead of the generation prompt (so the same prompt asked
again and the conversation's follow-up attach everything up to it, whatever the replayed assistant turn renders -
a thinking-off turn's generation prompt opens a think block the replay does not), and one at its finished turn,
within `--prefix-state-mb` (a quarter of RAM unless set - in a container, of its memory limit
where that is lower - per served slot) and at most `--prefix-states` of them, a
conversation's own stops (its last message's start, its last message's close, its finished turn) dropped before
shared openings, and a prompt attaches the deepest one that is its exact prefix - the shared system block for a new
conversation, the whole earlier exchange for the next turn. A slot whose streams keep their caches on the card
(device-home, the Vulkan whole-model driver) keeps the same cache in host RAM behind them, under the same two flags:
the card holds one finished stream and a few checkpoints a stream slot, so every checkpoint and finished turn is also
copied down, and a request whose opening the card no longer holds takes it back up into its region - milliseconds over
the bus where the prefill would take seconds. `DASLLAMA_DEVICE_POOL=0` keeps the card's copies alone.
Clients whose connection drops mid-generation are evicted within a tick. Audio uploads queue to
long-lived `new_thread` ASR workers and do not block chat generation; `--asr-workers 2` permits two
transcriptions at once. Each worker owns its model/context and reuses language-specific session
scratch, so memory settles at the workers' high-water mark. OpenAI is stateless - the client
resends the full transcript each turn.

**Three kinds of thread serve a request.** The HTTP front owns the sockets and every response
writer and runs no handler: a request crosses to its owner as a message, and the owner's replies
come back as writes addressed by a wire id. The engine - the context that loaded the models -
owns the slots and schedulers and runs every chat, model and config handler. The speech thread,
started when an ASR or TTS model is configured, owns the transcription and synthesis routes and
their model workers; it answers through the front. A handler that holds the engine (a
model load, a long embeddings forward on a chat model) therefore holds no speech request, and the status routes
(`/v1/models`, `/v1/stats`, `/v1/streams`) wait 50 ms on the engine, then the front serves the
engine's last answer for the path under an `X-Dasllama-Stale-Ms` header. The engine
reads the speech state off snapshots the speech thread publishes, and asks for a current one
before each status answer.

The main server context configures the shared job queue. ASR, TTS and media worker contexts
enable their own fork-context pools before loading or evaluating a model: that setting is
context-local, not inherited from the main context. Worker setup preserves the shared queue's
team/FIFO policy and applies only the worker's selected team-dispatch participation, decided once
at start: under `hybrid` the ASR workers join the team when every loaded LLM slot decodes on a GPU.

## Supervised deployment

The server runs under the shared watchdog in `utils/watchdog/`, in the release bundle and in a
JIT deployment alike. The watchdog needs no arguments: in the bundle it finds the baked exe
beside it (`discover_program` picks the server among its companions, the CLI and the bench), in a
JIT deployment `main.das` beside `bin/Release/daslang.exe`, and `watchdog.json` pins the name,
puts its log, pid file and crash bundles under `~/.dasllama/logs` (`state_dir`), turns the tray
on and names `tray.ico` as its mark. A JIT deployment on Windows:

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
sharing the bundle's libraries), `watchdog.json`, `control.html`, `tray.ico`, and `licenses/` -
the daslang license and every third-party notice the SDK installs for the runtime, the shipped
modules and the engine's ported works, under the SDK's names. Plain `release` tunes the kernels on the build
box and ships that box's sidecar - the bundle for a machine you own. `release --fat x86-avx2`
(`arm-neon` on arm64) is what the public download is built from: no mint, no sidecar, one
clone of every kernel per shipped class profile, the runtime section minted at the first start
on whatever box runs it (the Metal twin kernels race where a Metal device exists; elsewhere it
records the knob defaults). The `.das_package` names the launcher: on macOS the `.app` opens the
watchdog, so a double click supervises. The exchange levers on the control page - apply a
sidecar, share, re-tune - refuse on a fat release, which has no tune of its own.

A JIT deployment instead stages the toolchain - `bin/Release/daslang.exe` plus the runtime DLLs
and shared modules, the server sources at their repo path (`utils/dasllama-server/`, with
`utils/common/` beside it, so the relative requires resolve), and `watchdog.exe` at the root with
a `watchdog.json` whose `cwd` points at the server - into the target directory (`deploy-jit.ps1`
does it on Windows); keep the server's `main.tune.json`, the box's tuned kernels, across upgrades. Either way, stop a running server first - Windows locks the
DLLs; the config and the bundle's tune state live in `~/.dasllama` and survive the upgrade.

## Endpoints {#endpoints}

| Method | Path | Notes |
|---|---|---|
| `GET`  | `/` | Control page: live stats + charts, models panel (per-slot cards with state/GPU badges, prefix hit rate, switch telemetry, activate buttons; VRAM bar + switch strip), stream swimlane + live text cards, prefix-cache table, a chat panel (all sampling knobs, `<think>` inline, mic input - dictation under `--asr`, else the clip attaches to the next message when the slot serves native audio; under `--tts` every answer wears a speaker button that reads its content half back on the studio's voice, split at sentence boundaries into pieces the speech route accepts), a speech studio (type a line, hear it, and read the front end's normalized text and per-chunk phonemes under the waveform), config editor with the `[[models]]` roster table + save/restart, GC + drain buttons. Serves `control.html` from beside the server sources - polls `/v1/stats` + `/v1/streams` at 1 Hz |
| `GET`  | `/v1/models` | Lists every served slot, every TTS model under its id, every translation model and every embedding model whose worker loaded it under its id, and the ASR models: the default one as `<default slot>-asr` beside an LLM, and each under its own id when several serve or no LLM does - requests route on these ids via their `"model"` field |
| `POST` | `/v1/models/activate` | `{"model": name}` loopback-only admin warm-switch: make `name` the DEFAULT + stepped slot (model-less page requests follow) and move the GPU tier to it now (instead of waiting for the owner to drain). `409` while any work is live, `404` on an unknown name; `200` reports `switch_ms` + `backend_effective` |
| `POST` | `/v1/models/load` | `{"path", "id"?, "backend"?: "auto"\|"cpu", "quant"?, "ctx"?, "image_mmproj"?, "audio_mmproj"?, "activate"?: true}` loopback-only live load: a downloaded GGUF joins as a NEW serving slot with no restart and no JIT recompile (the load blocks the tick for its duration; a prepared `.dlim` image loads in well under a second). `"auto"` follows the boot GPU policy - same want + `--ctx` clamp every boot load got, the current owner's VRAM state drops first (boot-order semantics) and re-arms if the load fails or stays off the device; `"ctx"` in the body replaces the boot `--ctx`, and is asked whole the same way. An `image_mmproj` arms vision (and audio, when the file carries an audio tower too), an `audio_mmproj` arms audio alone, each with a media-worker bounce - a bad mmproj degrades to text-only with the reason in `tower_note`, and a load panic (corrupt GGUF, refused KV geometry) answers `400` with the slot unwound. `409` on a live stream set, a taken id, or a GGUF another slot already serves (one slot per file) |
| `POST` | `/v1/models/unload` | `{"model": name}` loopback-only: free the slot's weights, KV pool, and (for the GPU owner) VRAM. The DEFAULT slot refuses (`400`) - activate another model first - which also keeps the last model serving. `409` while any work is live |
| `POST` | `/v1/chat/completions` | Chat; `stream: true` -> SSE, else buffered; the request's `stop` (a string or up to 8) ends the reply where a stop begins, `finish_reason: "stop"`; a silent stream gets an empty delta every 10 s; OpenAI function calling (`tools`; a stream sends its text as it comes and the parsed calls at the finish - Tool calling below); `response_format` / a forced `tool_choice` sample the reply through a grammar (Structured output below: a malformed field, an undeclared forced tool, a forced `tool_choice` on a call shape the grammar builder does not write, a schema past the engine's caps or one `strict` cannot serve is a `400`; a `reasoning_effort` other than none, minimal, low, medium, high is a `400` too; `thinking_budget` (a non-negative integer, 0 = none) is the reasoning tokens a thinking reply may spend before the server closes its span for it and the answer begins - the family's own close marker, so `reasoning_content` ends where the budget fell and `content` carries the answer; a negative or non-integer value is a `400`; `finish_reason: "constraint"` ends a reply the grammar cannot continue); `stop` (a string or a list of up to 8) ends the reply where its text begins, `finish_reason: "stop"`, the stop text never emitted - an entry that is one token (a special such as `</think>`) is a stop token, every other a text the scheduler scans the reply for, holding back what could still begin one; a streaming reply silent for 10 s gets an empty `delta` (its framing's) so a client's no-bytes timeout survives a long prefill; `image_url` content parts under `--image-mmproj`; `input_audio` content parts under `--audio-mmproj`, or when the image mmproj carries the audio tower (any user message may carry images and clips, up to 16 parts a request - each splices into the serving slot's prefill where the part sits among its message's text parts, so continuous batching covers audio too; a part is encoded and prefilled once: a continued chat, or the same request again, attaches its span off the prefix cache and never reaches the tower). A known part goes to the scheduler with no rows only where the prefix cache holds its span when the request is submitted; a span the cache drops before the stream's admission makes the server encode every part of the request and submit it again, once, and where that retry cannot be made the reply finishes with `finish_reason` `"media_lost"`. A stream sent with `stream_options: {"include_usage": true}` ends, before `[DONE]`, on one chunk with an empty `choices` list: `usage` (`prompt_tokens`, `completion_tokens`, `total_tokens`, and `prompt_tokens_details.cached_tokens` - the prompt tokens the prefix cache attached) and `timings` (`ttft_ms`, the scheduler's admit-to-first-token wall, and `gen_ms`, first token to finish); a stream that does not ask carries no such chunk |
| `POST` | `/v1/completions` | Raw completion; `stream: true` -> SSE, else buffered; the request's `stop` (a string or up to 8) cuts the text where a stop begins and ends it `finish_reason: "stop"`; a silent stream gets an empty chunk every 10 s; the same `stream_options.include_usage` closing chunk as the chat route |
| `POST` | `/v1/embeddings` | L2-normalized sentence embeddings, pooled the way the model's `pooling_type` asks (last token for Qwen3-Embedding, mean for a chat model); `pooling` overrides, `dimensions` cuts and renormalizes. An embedding model (its GGUF names its pooling) answers from its own worker thread, off the engine loop - a chat stream decodes on meanwhile - with up to 32 requests queued (then `503`); a chat model named here answers on the engine, in-handler. `503` while draining for shutdown or while a bench holds the server; `400` for a bad `input`, `pooling` or `dimensions` and for an input past the model's context; `404` for a model nothing loaded. A request naming no `model` goes to the default chat model, or to the first embedding model on a server with no chat model. `503` also on a chat model whose trimmed image has no CPU planes while every device region holds a live stream (the forward needs a free region); `500` when the embedding worker panics on the request |
| `POST` | `/v1/audio/transcriptions` | Speech->text (multipart upload; needs `--asr`). One ASR model takes every request, whatever `model` says; with several, `model` names one by its id (none, or the `<default slot>-asr` id `/v1/models` lists: the first; any other name: `404`); `language` defaults to English, to detection on a model that only detects, and to its own language on a one-language model. `response_format=verbose_json` adds timed segments |
| `POST` | `/v1/audio/translations` | Speech->English text (needs `--asr`); `model` and `language` are read as the transcription route reads them, the `404` included; a model whose caps declare no translation is a `400`. An optional `target` form field (needs `--translate`) transcribes instead and routes the transcript through the translation model for (`language`, `target`) - `language` is the source, or, absent, the language the ASR model heard - answering in the same `response_format` shapes: `json` and `text` carry the translation, `verbose_json` keeps the segments' timing with each segment's text translated and the whole translation in `text` (`task`: `translate`). Errors with `target`: before the audio is heard, a `400` naming the pair when `language` is given and no loaded translation model serves it, or naming the target when `language` is absent and no loaded model writes it; after the transcript, a `400` when `language` is absent and the ASR model reported none, or the language it heard has no served pair into `target`; a `503` while the translation queue (32) is full; a `500` when the ASR model's verbose transcript does not parse or the translation worker panics (the panic's text as the message). The caps check above does not apply |
| `POST` | `/translate` | LibreTranslate's route (needs `--translate`): a JSON or form body `{q, source, target, format?, api_key?}` - `q` a string or an array of strings -> `{"translatedText": "..."}`, or an array of translations for an array. Each input is split into sentences (a `.`, `!` or `?` followed by a space ends one, so does a newline) and translated a sentence at a time, the separators kept. The pair goes to the first loaded model that serves it. Errors are LibreTranslate's `{"error": "..."}`: `400` for a missing `q` (an array `q` holding a non-string reads as missing), `source` or `target`, a `format` other than `text`, `source: "auto"` (no language detection here), an unserved pair, or input past 20000 characters; `400` for a body that is neither a JSON object nor a form; `503` while no translation model is loaded, the queue (32) is full, the server drains, or a benchmark runs; `500` when the translation worker panics (the panic's text as the error). A urlencoded form's repeated `q` (`q=a&q=b`) is an array; a multipart form carries one value a field. `api_key` is ignored |
| `GET`  | `/languages` | LibreTranslate's language list over every loaded translation model: `[{"code", "name", "targets": [...]}]`, codes sorted; a Bergamot student lists its source with its one target, a translation LLM every language with every other. `name` is the language's English name where the translation facade knows it (a translation LLM's 46), else the code. `[]` with none loaded |
| `POST` | `/v1/audio/speech` | Text->speech (needs `--tts`): `{"model"?, "input", "voice"?, "speed"?, "response_format"?: "wav" \| "pcm"}` - the OpenAI shape; `model` names the TTS model (none: the first; an id no model serves: `404` naming the served ids); `wav` (default) is 16-bit PCM at the model's rate, `pcm` the raw samples; the compressed formats answer `400` (no encoder here). One synthesis at a time on each TTS model's worker (its kernels run inline under `hybrid`), 16 queued across the models |
| `POST` | `/v1/audio/phonemes` | The front end alone (needs `--tts`): `{"model"?, "input", "voice"?}` -> `{"normalized", "lang", "chunks": [{"text", "phonemes"}]}` - the normalizer's spoken form of the text, the dialect the voice speaks (`lang`), then one row per chunk a synthesis of it would take, each carrying that chunk beside its phoneme string in that dialect. A model whose front end phonemizes ONE language reads every voice name in it - an alias, or a name it does not carry, since there is no other answer to give; a model that phonemizes several requires a voice from its `caps` and refuses an unservable one with the speech route's own 400. `model` is read the way the speech route reads it: it names the TTS model that answers, none takes the first, and an id no model serves is a `404`. Answered by the TTS worker on the same queue as a synthesis (the same 4096-CHARACTER cap - codepoints, not bytes - and the same 503 when no speech model is served), so the speech studio can show what the model will actually say |
| `POST` | `/vad` | Silero speech spans over an uploaded clip (the control page's waveform overlay; in-handler, <=120 s, needs the in-repo `silero_vad.bin`) |
| `GET`  | `/catalog` | The curated model list with local presence (a gemma-4 entry with an assistant drafter sidecar carries `drafter_file`/`drafter_bytes`/`drafter_present`), the `asr` tower row, the `tts` list (the two front-end packs the speech route loads, then every served speech GGUF, each `file`/`bytes`/`pack`/`present`/`path`/`needs_packs` - on a model, whether the file on disk reads the packs, true until it is here; false on a pack), the `box` memory facts + the download state machine (`idle | downloading | verifying | done | failed`, byte progress) |
| `POST` | `/catalog/download` | `{"name": <entry>}` - start one catalog download; `{"name", "tower": "vision"}` / `{"name", "tower": "drafter"}` (the assistant drafter sidecar; the accepted reply's id is `drafter:<name>`) / `{"tower": "asr"}` pull a tower, `{"tower": "tts", "file": <file>}` one file of the speech set (409 while one runs or the file exists; sha-verified, never waived) |
| `POST` | `/bench` | Loopback-only: start the benchmark, quiesced. In process by default: pp512 and tg128 on the served model, an untimed warmup then three timed reps each, one pp prefill or one tg token per tick, then tg128@N on a slot serving two streams or more (N streams through the scheduler - device-home where a whole-model driver homes them, host-cached otherwise - the summed rate, a rep per tick; a self-speculative slot ticks them through the joint round), the text, audio and speech routes (503) and the model-switching, bake and catalog-download routes (409) holding until it finishes - in either mode; with `lcpp_bin` in the config on a source-tree daslang, the A/B child instead - our lcpp_bench then llama-bench on the same GGUF. 400 in setup mode or when the served context is shorter than pp512, 409 while a bench, a bake or a catalog download runs or streams are active, 503 while draining |
| `GET`  | `/bench` | Bench state (`idle | running | done | failed`), `mode` (`inprocess` | `ab`: what a `POST` runs), live log lines, the result JSON - `ours_pp`, `ours_tg`, `threads`, `elapsed_s`, `ts`; in process also `mode`, `pp_sd`, `tg_sd`, `reps`, `model`, `gguf`, `backend` (`metal`, `gpu:resident`, or the slot's word), `kv` (the codec the rows ran), `exec` (`exe-native` | `jit` | `interpreted`), `tune` (`fat` | `tuned` | `untuned (N of M on fallback)` | `none`) and `ref_cmd` (the llama-bench line for the comparison, with the llama-batched-bench line after a `;` where the batched row ran), and where the batched row ran `ours_tgb`, `tgb_sd` and `npl`; the A/B also `theirs_pp`, `theirs_tg`, `pp_ratio`, `tg_ratio` and `record` - and the hardware line |
| `POST` | `/bake` | `{"model"?: name}` loopback-only: bake the slot's prepared `.dlim` image by spawning `dasllama-convert` (empty body bakes the default slot; 409 while a bake or bench runs or streams are active; the dlim GC of never-loadable images runs on completion) |
| `GET`  | `/bake` | Bake state (`idle | running | done | failed`), the slot it runs for, log lines, the result JSON |
| `GET`  | `/v1/images` | Per-slot prepared-image inventory: source GGUF path, the flavor THIS process mapped (planar/vulkan/metal, or raw gguf), the trimmed flag, and each on-disk `.dlim`'s info - plus the slot name a bake is currently running for |
| `GET`  | `/v1/stats` | Scheduler counters (`gen_tokens`, `prefill_tokens`, TTFT last/avg, ...) plus `model`/`active_model`/`ctx`/`uptime_s`/`draining` identity fields, memory footprint (`weights_bytes`, `kv_bytes`, das heaps, `gpu_vram_bytes`/`gpu_budget_bytes`), `gpu_cpu_passes` (the calls the armed GPU path handed back to the CPU since the model armed, `{reason, words, count}` per reason that fired - `words` is the reason as the control page prints it; empty means the device served every call), a `hardware` line (CPU * lanes * GPU), `asr_workers`, `asr_ready`, `asr_active`, `asr_pending`, an `asr` block present while an ASR model is configured (`models[]`: each model's `id`, `workers`, `ready` and `decoder` - `metal` or `vulkan` where its workers serve on that device, `cpu` elsewhere, empty until a worker answered), speech counters (`tts_done_jobs` - syntheses served since boot, `tts_audio_s` - the speech seconds they carried), a `tts` block present ONLY while a speech model is configured - and still there when its worker could not load it; it describes the default TTS model (the first that loaded, else the first that failed), and carries `models` - the ids of every loaded TTS model - only when several serve (`id`, `ready`, `pending`, `done_jobs`, `audio_s`, `voices[]` and `sample_rate` as the loaded model declares them, `cloning` (the model takes a voice from a clip), `speed` (a `speed` other than 1.0 is honoured; false for a Pocket model, which refuses one) and `lang` (the first language it declares), the `lane` its worker pinned or, before there is an answer, the one the boot asked for, and `error` - the loader's reason, present only when the configured model failed to load, which the control page's offer card reads instead of claiming the model is still on its way) - the control page's speech studio gates on `ready`, a `translate` block present ONLY while a translation model is configured (`pending`, `done_jobs` - translations answered since boot, and `models[]`: each model's `id`, `ready`, `kind` - `marian` or `llm`, `source` and `target` - a Bergamot student's pair, empty for an LLM, `languages` - how many it knows, and `error` - the loader's reason, present only when its worker failed to load it), an `embed` block present ONLY while an embedding model is configured (`pending`, `done_jobs` - embeddings requests its workers answered `200` since boot, and `models[]`: each model's `id`, `ready`, `dim` - its hidden width, `ctx` - its context, `backend` - `gpu:resident` where the worker's own device tier serves the model whole, `cpu` otherwise (empty until the worker loaded it), `device_prefills` - the prompt windows the worker's device has prefilled since it loaded, `cpu_passes` - the calls its device handed back to the CPU since then, every reason summed, and `error` - the loader's reason, present only when its worker failed to load it; an embedding model is never in `models[]` below), media counters (`media_pending`, `media_rows`, `mrope_streams` - streams whose media rode the qwen mrope grid walk), and `models[]` - one entry per slot: `file` (source GGUF base name - the page's serve-live gate), `is_active`, `holds_gpu`, requested `backend` vs `backend_effective` (`cpu`/`gpu:rails`/`gpu:resident`), `served` (how the slot is served, in plain words: where the weights sit and where the streams' caches do - the control page prints it on the model card), `served_note` (why it is not served better: the whole-model driver's decline with its remedy, the tower or self-speculation that keeps the caches on the host, fewer K/V regions than streams; empty when nothing holds the slot back), `device_kv` (every stream's K/V cache lives on the GPU, a region each), `vision` and `audio` (the towers that slot actually loaded - the catalog row's chip reads them, and `audio` is what tells the chat mic to attach a clip instead of transcribing it), per-slot cache counters, `last_used_s`, switch count/avg ms |
| `GET`  | `/v1/streams` | Per-stream poll surface: `model` (the slot it runs on), state (`queued`/`prefilling`/`decoding`/`finished`), token counts, TTFT, and capped text tails (prompt head + generated tail); finished streams linger ~10 s flagged `finished`. Plus `cache`: the prefix-cache donation chains (tokens, live pages, hits, age, preview) and `asr`: recent ASR jobs (`model` - the id of the ASR model that took the job, empty once its client has gone - state, audio s, wall ms, RTF) |
| `GET`  | `/config` | Effective config with per-key source (`default`/`cli`/`toml`) - one entry per row of the flags table above (`prefix_states` answers `-1` when unset: the per-slot count from the byte budget, 256 to 1024), `tts` (the speech model path), `tts_lane` (`q8` | `f32`) and `tts_voices_dir` (the clip directory a cloning model reads) and `translate` (the translation models, comma-separated) included - plus the `[[models]]` roster, the speech rosters `asr_models` (`id`, `path`, `mmproj`, `workers` an entry) and `tts_models` (`id`, `path`, `lane`, `voices_dir`) - one entry for a one-path `asr` / `tts` key, one a repeated `--asr` / `--tts` flag, one a `[[asr]]` / `[[tts]]` table - model files beside the served one, active rail (gguf vs prepared `.dlim`), GPU tier status (`supported` + `reason` when the loaded model can't ride it). `chunk_idle` answers raw: 0 when unset, meaning the serving backend's default (2048 on Metal, 512 elsewhere) |
| `POST` | `/config` | Validate a `{key: value}` JSON body (`asr` and `tts` each a path, or an array of entries with the `[[asr]]` / `[[tts]]` table keys) and write it as an **authoritative** TOML (`authoritative = true`) to the config path (or `~/.dasllama/dasllama-server.toml` on a config-less start). Applies on the next restart |
| `POST` | `/restart` | Drain like `/shutdown`, then exit with code **4** - the watchdog relaunches, picking up the saved config (3 stays the tune-restart code) |
| `GET`  | `/exchange` | The sidecar-exchange surface: policy (url/accept/submit/configured), the consent state (`consent`: accepted/declined/empty, `consent_notice`: the first-contact text), + the current tune sidecar's identity and share state (sha, origin, box/applied_box, version gate, shared-yet) |
| `GET`  | `/exchange/matches` | Live lookup of this box against the exchange (a network call - seconds; the control page requests it explicitly) |
| `POST` | `/exchange/apply` | `{"sha": ...}`: download + validate (content sha, schema, `DASLLAMA_RELEASE`) + adopt that sidecar, then drain and exit **4** so the relaunch stamps its winners |
| `POST` | `/exchange/submit` | Privacy-strip and submit this box's own tune to the exchange (refuses exchange-sourced or foreign-box sidecars) |
| `POST` | `/exchange/retune` | Arm a local re-tune: removes the sidecar, skips the exchange once, restarts - the next boot races this box (~12 min, quiet machine) |
| `POST` | `/exchange/consent` | `{"accept": true\|false}`: record the first-contact choice; replies `{ok, accepted, restarting?}`. Accept on an untuned/stale box drains and exits **4** so the relaunch runs the lookup |
| `POST` | `/gc` | Schedule a validated collection at the next lifecycle safe point; concurrent requests coalesce |
| `POST` | `/cache/drop` | Loopback-only: every slot's prompt cache - the donated pages and a recurrent model's checkpoints - goes back to its pool, so the next request prefills cold while the model stays loaded; replies `{ok, pages_dropped, checkpoints_dropped}`; 409 while streams are live or a bench holds the engine. A bench that replays the same requests on one server starts each phase here |
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

### The gpu key {#gpu-key}

The `gpu` key (`auto | off | metal | metal-required | vulkan | cuda`) is the first-class
backend selector, and **defaults-first: unset (with no legacy `--metal` flag) behaves as
`auto`** - the boot probes the box and serves on the best detected backend (the Metal rails
where the box has them; else the home `DASLLAMA_CUDA=1` spells where its device answers; else
the Vulkan home when a Vulkan device answers - the fully served home, preferred while the CUDA home
is still growing into every form; else the CUDA home; else the CPU), logging
one `gpu backend auto-detected:` line; `gpu = off` is the explicit opt-out, and an explicit
`DASLLAMA_GPU=0` in the environment keeps its meaning - auto-detection never out-votes it
(the env master only overrides the `auto_tier` field of an explicit want, so the pick
resolves to `off` before any want is constructed). `vulkan` and `cuda` name the discrete HOME the
whole-model driver runs on, and this key is the one switch that picks it: the boot moves the engine's
tier onto that home (`set_gpu_home`) before any load, and `DASLLAMA_CUDA=1` is only the environment's
spelling of the same choice (what `auto` honors). A home this build does not carry, or one the
environment's `DASLLAMA_GPU` already armed the tier away from, is refused by name in the boot log and
the slot serves on the CPU. Every rule below - the region a stream, the context asked whole, the
want, the control page's rail toggles - applies to both homes alike. Per-model
support is unchanged - a model the armed tier cannot serve falls back to the CPU with the
reason on the control page. `gpu = vulkan` asks for the **whole-model resident driver** first - a
model that fits the card serves entirely from VRAM, and so do its streams: the driver holds a
K/V region per stream (`streams`), a request's cache lives only there, the prompt prefills on
the device in 512-token windows, the streams decode in one batched step, and a conversation's
next turn picks up its earlier turns' rows from the region they were left in. A stream's
context is a region's. An explicit context - `ctx`, `--ctx`, or a load body's `ctx` - is asked
whole: the driver holds it for every stream or declines the model to the per-op rails, never
shortening it - except a Metal model served from its image alone, whose context is cut to the rows
its device KV mirror holds, since it has no CPU rail to fall to. With none, the region holds the model's own context or what the card's room divided
by the stream count allows, and the load log says why. A request that fills the region ends with a
`length` stop. `/v1/stats` counts every call the device handed
back to the CPU (`gpu_cpu_passes`); a slot served this way keeps it empty. The control page's
model card says which of these a slot got (`served`), why it got less when it did
(`served_note`), and the passes by reason once any fired. A recurrent (deltanet) model is served the
same way, its recurrent state kept in the stream's region beside its rows; a recurrent model with
hyper-connections (Qwen3.8-Flash-Next) differs in two ways: a prompt prefills in one piece, and a next
turn re-prefills its whole transcript. A slot with a vision or audio tower keeps host-cached sessions, and shows its passes there; a
self-speculative slot does too where any seat of the round (the draft head, its verify, a rejected
round's rollback) stays on the host. The want that asks for the driver
sets `resident` outright rather than riding `auto_tier`, and spells every rail out one by one, so
an explicit `gpu_dn = false` still turns that rail off. The per-op rails arm beneath the driver as the
fallback for a model that does not fit: expert stacks sized
**automatically** (resident layers fill the VRAM budget, the rest stream) plus DN (each deltanet plane in
its own format) + ATTN + the dense layers' attention projections + the resident shared expert (its own planes).
A dense layer's FFN rides the per-op FFN rail (`DASLLAMA_GPU_DENSE_FFN=0` keeps it on the CPU). `gpu_layers` / `gpu_stream` are `0` = auto by default; set either to a
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

### Valid UTF-8 replies {#utf8-replies}

A token piece can end in the middle of a character: a multi-byte character can span tokens, and a
byte-fallback piece carries one raw byte. Every text the model produced, or that the server parsed out
of what it produced, goes out as valid UTF-8, because strict clients abort on an invalid byte. A
streamed reply holds back an incomplete trailing sequence (`utf8_hold_bytes`) and joins it to the next
piece, so a character split across tokens goes out whole in one delta; each delta then passes
`utf8_sanitize` (`daslib/utf8_utils`), which turns a byte no later piece can complete into U+FFFD, and
a stream that ends inside a character sends the held bytes as one last delta through the same
sanitizer and the same reasoning / tool-call splitter, so they go out as U+FFFD on the channel the
stream was in. A buffered reply - chat `content` and `reasoning_content`, completion `text`, a tool
call's `name` and `arguments`, a transcription's `text` and each `verbose_json` segment's - passes
`utf8_sanitize` over its text before it is written as JSON.

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
default) decodes fine; the payload caps at 32 MB of file and 67 MP decoded, each a named 400.
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

A constrained request - a `response_format` that compiles to a grammar, or a forced `tool_choice` -
runs with thinking off whatever `enable_thinking` says: the grammar starts at the reply's first token.
Otherwise `enable_thinking` is tri-state: ABSENT leaves the model template's own default in force
(thinking-capable Qwen3/3.5/3.6, GLM, gemma-4, and gpt-oss templates default on; a bare Qwen
Instruct generation branch such as Qwen3 Instruct-2507 defaults off), and a present bool -
top-level or the llama.cpp spelling
`"chat_template_kwargs": {"enable_thinking": ...}` - overrides it. `false` on a
`<think>`-family appends the template's empty think block so the model answers directly;
`false` on gemma-4 prefills the closed empty thought channel (the instruct opt-out); a gemma-4
that opens its reply with a thought block anyway (the E-series does on a media turn) still
answers, the block coming back as `reasoning_content`, and a channel marker after the answer
ends the turn. A no-op for models whose vocab has no think tokens. gpt-oss cannot skip its analysis
channel: there `enable_thinking: false` writes `Reasoning: low` in the harmony system turn (the shortest
analysis the model writes) where a thinking turn writes `medium`, and the OpenAI `reasoning_effort` field
(`low`, `medium`, `high`) sets the line outright; other families render nothing for it.

`thinking_budget` (a non-negative integer, default 0 = no budget; `null` is no budget, a negative or
fractional value is a 400) caps the reasoning span: once the reply has spent that many tokens inside it,
the server writes the family's own close for the model - Qwen3/3.5/3.6 and GLM get the recipe's
"Considering the limited time..." sentence and `</think>`, gemma-4 its `<channel|>`, gpt-oss the
`<|end|>` and the final channel's header - and the answer follows; `reasoning_content` ends at the
forced close. The span's own close, written by the model before the budget, ends the count. A budget
reached within the close's length of `max_tokens` is forced early, so the close always lands inside
the cap; a cap already shorter than the forced close keeps the model's own close marker alone. A
span the model re-opens after the forced close ends the turn, as a channel marker after content does
in instruct mode. A family whose template carries no think markers ignores the field.

A thinking reply's reasoning span comes back as **`reasoning_content`** (the
DeepSeek/llama.cpp framing) with `content` clean of the family's markers: on the
`chat.completion` message for buffered requests, and as `delta.reasoning_content` chunks -
streamed before the `delta.content` chunks - for `stream: true`. Tool-calling replies split
reasoning first, so a thinking model that calls tools returns `reasoning_content` AND
`tool_calls` in one response. The field is absent (never empty) when the model did not think.

A replayed assistant turn renders without its think block unless the request sets
`"chat_template_kwargs": {"preserve_thinking": true}`: then every assistant message in `messages`
renders `<think>`, its `reasoning_content` (empty for a turn that did not think, the block the
thinking-off generation prompt carried), `</think>` and the content - the tokens the model generated
on that turn, so the prefix cache matches the whole earlier exchange instead of stopping at the first
assistant header. The same no-op as `enable_thinking` on a model whose vocab has no think tokens.

### Context truncation

By default, an over-context rendered prompt returns HTTP 400. Set `truncation: "auto"` to preserve
system messages and tools while dropping the oldest complete user-led turns until the prompt plus
the requested `max_tokens` output budget fits. If the system/tools/latest turn cannot fit, the
request still returns 400. `finish_reason: "length"` means generation consumed its output budget.

### Tool / function calling

`/v1/chat/completions` speaks the OpenAI function-calling protocol: pass `tools` (and optionally
`tool_choice`; `"none"` disables, `"required"` and the forced-function object form sample the call
through a grammar - see Structured output below), get back
`finish_reason: "tool_calls"` with `message.tool_calls`, send the results as `role: "tool"`
messages, repeat. Assistant `tool_calls` turns and `role: "tool"` results replay exactly through
the chat template on each stateless resend, so agent loops (opencode, pi, ...) work end-to-end.

The wire format is per model family (`ToolMode`, `dasllama_tools.das`): **hermes** (Qwen2.5 /
Qwen3 - `<tools>` system block, `<tool_call>` JSON), **xml_function** (Qwen3.5 and Qwen3.6, sniffed off their
templates - the `<tools>` block opens the system turn ahead of the system text, a call is a
`<tool_call>` holding one `<function=NAME>` block of `<parameter=K>` values, results ride user turns as
with hermes), **harmony** (gpt-oss - developer-turn
TypeScript namespace defs, commentary-channel recipient calls; reasoning and calls come from one
channel walk), **gemma4** (gemma-4 - declaration/call/response DSL with the `<|"|>` quote token),
**mistral** (v0.3 and the tekken templates - Mistral-Small 3.x, Magistral, Devstral: `[AVAILABLE_TOOLS]` defs, `[TOOL_CALLS]` array, bare-array replies
tolerated since the SPM stream suppresses control-token pieces), and **llama_json** (llama-3.x -
the whole reply is one `{"name","parameters"}` object, results on the `ipython` role). A model
whose chat template declares no tool format (GLM until its zen2 leg) gets an honest 400.
Streaming with tools puts the reply's text on the wire as it comes - reasoning and content deltas as on a plain
stream - and holds back only the family's call marker: a content tail that could still begin it waits, and from
the marker on nothing goes out until the finish, when the parsed calls arrive as one `delta.tool_calls` chunk; hermes,
xml_function and gemma4 hold at their call marker, llama_json at the call object's opening brace, mistral at the call
array's bracket (its `[TOOL_CALLS]` control token never reaches the decoded text). A family whose calls ride channels
rather than a marker (harmony) is framed whole at the finish.

Requests the server does NOT fully understand are visible in the log: unknown endpoints 404
through a catch-all that logs method + path + body head, and known routes warn per ignored field
(`logprobs`, a `chat_template_kwargs` key other than `enable_thinking` and `preserve_thinking`, ...).

### Structured output

`response_format` constrains the reply through a byte grammar the sampler consults token by token
(`modules/dasLR1`: an LALR(1) acceptor over the model's token pieces, `constraints.das` here):
`{"type":"json_object"}` admits any JSON object; `{"type":"json_schema","json_schema":{"schema":{...},
"strict":true}}` admits exactly the documents the schema describes - object keys in order, `enum`/`const`,
integer `minimum`/`maximum`, `minLength`/`maxItems`-style bounds, local `$ref`, `anyOf`/`allOf`, string
`pattern`, the `date`/`time`/`uuid` formats. A schema the compiler cannot serve is a 400 under `strict`
and a logged warning with an unconstrained reply without it; a repetition bound (`maxItems`, `maxLength`, a
pattern count) past 1024 is refused either way, since each copy is a rule of the grammar. `tool_choice: "required"` or
`{"type":"function","function":{"name":...}}` samples the tool call through the same machinery: the
declared tools' schemas (one, or any of them) inside the family's call markers, so the name is one of the
declared names and the arguments fit the parameters - on the Qwen3.6 format the grammar is the block shape
instead, a `<function=NAME>` of that tool's declared `<parameter=K>` names with any text short of a `<` as a
value; a family whose call shape the grammar builder does not write - gemma-4's DSL, harmony's channels, mistral's
array, llama's bare object - answers 400. A
constrained reply runs with thinking off, drafts nothing under `--mtp` (it decodes plain beside the drafting
streams), and lands its logits on the CPU; the stop token is admitted only where the grammar's sentence is
complete, and a reply the grammar cannot continue ends with `finish_reason: "constraint"`. The first
constrained request a slot serves decodes its vocabulary once (the token piece table, freed with the slot);
compiled grammars are cached by their text, 64 at a time.

### Embeddings

`input` is a string or an array of strings. Each vector is L2-normalized and `model.config.dim`
floats long, or `dimensions` floats when the request cuts it (the leading components,
renormalized). The pooling is the file's `pooling_type` - last token for Qwen3-Embedding, the
mean over every position for a chat model that names none - unless `pooling` (`"mean"`, `"cls"`,
`"last"`) overrides it; any other value is a 400. An instruction prefix (Qwen3-Embedding's
`Instruct: ... Query: `) is the client's to add.

An embedding model - a `[[models]]` entry whose GGUF names its pooling - runs on a worker thread
of its own, with its own context and model: the request queues to it, the worker computes every
input's vector in batched forwards - the texts packed into one window up to its token budget
(8192 tokens, or the entry's `ctx` where that is wider), and into further windows past it -
and the engine hands the answer to the client, so a chat stream on the same server keeps
decoding meanwhile. The worker loads the model once at boot, with
the speech workers' dispatch and the engine's Metal mode; one that cannot load is logged, serves
nothing, and the `embed` block of `/v1/stats` carries the reason. A chat model named in the
request computes its vectors on the engine, in the same batched windows, and holds the engine
until they are done.

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
- `test_openai_server_embed.das` - an embedding model beside a chat slot: the roster's sniff
  (`<arch>.pooling_type`), the model on `/v1/models` and the `embed` block of `/v1/stats` and
  never among the engine slots, its vectors equal to the facade `embed`, a chat stream asked
  beside an embeddings request both finishing, the refusals' texts (the context check under a
  64-token context), a chat model named in the route still answering on the engine, and a worker
  that cannot load degrading its model; on a Vulkan box, a trimmed chat model named in the route
  answering 503 while every device region holds a live stream and 200 once they finish. Needs `Qwen3-Embedding-0.6B-Q8_0.gguf` and
  `Qwen3-0.6B-Q8_0.gguf`.
- `modules/dasLLAMA/tests/test_scheduler.das` - the continuous-batching scheduler against `generate()` references (moved with the module)
  (bit-exact single stream, chunk invariance, staggered admits, eviction); needs
  `SmolLM2-135M-Instruct-Q8_0.gguf`.
- `test_openai_server_stream.das` - SSE chunk framing, the over-long-prompt 400, two concurrent
  clients batching on one server (`peak_active >= 2` via `/v1/stats`), mid-generation disconnect
  eviction, and a request served off the prefix cache repeating itself token for token; needs
  `SmolLM2-135M-Instruct-Q8_0.gguf`.
- `test_openai_server_vision.das` - the image route end to end: a data-URI photo on
  `/v1/chat/completions` reaches the embedder, splices as soft tokens between the two rendered
  token spans, and comes back as a caption about the picture; plus the decode-failure and
  remote-URL 400s. Needs `gemma-4-12B-it-Q4_K_M.gguf`, `mmproj-gemma-4-12B-it-BF16.gguf` and the
  coco cats jpeg.
- `test_openai_server_vision_multi.das` - two vision models of different families on one server
  (gemma-4-E2B and Qwen3-VL-4B, each slot with its own mmproj): an image request routes by its
  `model` field, each model captions the photo alone and both when asked at once, an id no slot
  carries is a 404. Needs both decoders, both mmprojs and the cats photo.
- `test_openai_server_speech.das` - the speech route end to end on a TTS-only boot: a WAV answer
  with a RIFF header of speech length, the raw `pcm` form, the declined `mp3`, the missing
  `input`, the unknown voice, the TTS id on `/v1/models`, the `tts` block on `/v1/stats`, the
  `/v1/audio/phonemes` document against the facade's own normalizer and chunker, and a
  second boot on the `f32` lane proving the pin reaches the worker; and, on a boot with an LLM
  beside it, syntheses answered while an embeddings forward holds the engine (the forward's length
  the control). Needs `kitten-nano.gguf` and the front-end packs beside it.
- `test_openai_server_speech_multi.das` - several speech models on one server with no LLM slot:
  two ASR models and two TTS models, every id on `/v1/models`, each answering under its own id
  (the job's model read off `/v1/streams`) and the first of a kind answering a request that names
  none, the `404` for an id nothing serves with its upload removed, a model added twice served
  once, the four answering four requests sent at once, one ASR model taking any name, an ASR
  model serving alone - its decoder on the CPU with the Metal mode off and on the device with it
  on, read off `/v1/stats`'s `asr.models[].decoder`, the stats counting its worker and its clip -
  an ASR model that cannot load failing the boot, and the one-file GigaAM model serving alone: the
  Russian clip transcribed, the lane its tower serves on reported (`metal` on a Metal build), and
  `/v1/audio/translations` refusing it with a 400. Needs both Qwen3-ASR GGUFs with
  their mmprojs, `kitten-nano.gguf` with the front-end packs, `pocket-tts-en-q8.gguf`, and
  `gigaam-v3-e2e-rnnt-f16.gguf` for its cell.
- `test_openai_server_translate.das` - translation: `/translate` as JSON and as a form, a string `q`,
  an array and a repeated form `q`, `/languages`, and the 400s for an unserved pair, `html`, `auto`
  and a missing `q` on two Bergamot students (`bergamot/ru-en-base-memory`,
  `bergamot/en-ru-base-memory`) and on `milmmt/MiLMMT-46-1B-v0.1.Q4_K_M.gguf` (a language into
  itself refused); the students on `/v1/models` and the `translate` block on `/v1/stats`;
  `/v1/audio/translations` with `target` chaining GigaAM's Russian transcript of `ru_short.wav`
  through the ru-en student (`json`, `text` and `verbose_json`, the source given and the one the
  ASR model heard) and the 400s naming an unserved pair or a target nothing writes. Model-free, run
  in CI: the sentence splitter, the `--translate` path list, a model named twice left out, and a
  boot whose one translation model cannot load - the 400s for a missing `source` or `target`, a
  non-string `q` element and input past 20000 characters (a repeated form `q` counted whole), then
  the 503 while no model is ready, `/languages` empty and the load failure on `/v1/stats`.
- `test_cli_args.das` - dasllama-cli's model-free half, run everywhere: the plan a command line
  parses to, the bare-token walk, the help surfaces, the config file's keys and roster, the
  model-path resolution, and the chat loop's line logic (commands, the `"""` block, the saved
  transcript).
- `test_cli.das` - dasllama-cli end to end, one child process per cell as a user runs it: every
  command on the fastest small models (`SmolLM2-135M-Instruct-Q8_0.gguf`, `kitten-nano.gguf`,
  `Qwen3-ASR-0.6B-Q8_0.gguf` with its mmproj), the `talk` chain on those and on
  `Qwen3.5-0.8B-Q8_0.gguf` + `kokoro-82m.gguf`, the speaker cells (`speak`, `talk` and a
  two-reply `chat` under `--play --null-audio`), the `--verbose` echo with the streams split, and
  the tower cells on `gemma-4-E2B-it-Q4_K_M.gguf` with `mmproj-gemma-4-E2B-it-bf16.gguf` (the
  picture through the vision tower, the JFK clip through the audio tower, spoken back by
  `pocket-tts-en-q8.gguf`, and a `--no-think --hide-thinking` image turn whose leading thought
  block stays off the screen), and `translate`'s loop (its help, langs, speak-with-no-TTS and exit
  commands, `--quiet`, `--exact`, the `--to` and missing-model refusals) on the ru-en Bergamot student
  and its direction flags on `milmmt/MiLMMT-46-1B-v0.1.Q4_K_M.gguf`. Runs on the stocked box at
  release time.
- `test_openai_server_mtp.das` - the self-speculation default: a NextN-headed slot whose file has no
  catalog `serve_mtp` row decodes plain at one stream on Metal, at four and on the CPU, an explicit `mtp` wins either way, and
  a head-less model with no drafter beside it serves plain under the default; read off `/v1/stats`'s `mtp_drafted`. The
  Metal cells skip on a box with no Metal backend. Needs
  `Qwen3.5-0.8B-MTP-Q8_0.gguf` and `tinyllama-1.1b-chat-v1.0.Q8_0.gguf`.
- `test_exchange_client.das` - the exception: model-free and runs everywhere. The sidecar
  exchange client against a fake exchange on 127.0.0.1:18131 (lookup/pick, the fetch-and-apply
  gate, applied_box staleness, the privacy strip, both submit rails, policy parsing).
- `test_model_catalog.das` - model-free: catalog-table invariants (pinned URLs, unique ids,
  one default), the models-dir precedence, presence detection, download-start refusals, the
  `/catalog` document's drafter fields (a planted drafter present with its file and canonical bytes,
  every other row's absent), and the ACCEPTED body's echo (the download id and the bytes of the
  model, its vision tower or its drafter).
- `test_catalog_download.das` - model-free and offline: a drafter pinned to a local `file://` source
  starts its download under the id `drafter:<name>` the document's machine names, and the worker's
  verify fails bytes off the pinned sha. Its own file: the download machine is process state that
  `test_model_catalog.das`'s document cells read idle.
- `test_setup_mode.das` - model-free, runs end to end even interpreted (setup mode never
  infers): a slotless boot serves setup stats and the catalog while every inference route
  fails closed, and its model picker lists a planted GGUF but neither a `.dlim` nor a GGUF that
  serves no chat (an mmproj of arch `clip`, a one-file `gigaam` ASR model).
- `test_cache_probe.das` - model-free: the cache probe's cycle (the sixteen bodies, their kinds and
  conversations, the replayed first replies, the byte-identical repeat) and its summary off a
  fixed table of answers (shares by kind, the cold start, the openings attached, the medians, a
  refused request counted as failed).
- `test_server_flags.das` - the flag-presence test behind the config surface's provenance - a
  flag the process was given reads present, bare or as `flag=value`, and an absent one or a
  prefix of a given one reads absent; the acceptance a process starts with (`get_mtp_accept`, rejection sampling - the file
  never sets it); the self-speculation default's rule as the slot reads it
  (`mtp_auto_arms`, the Vulkan arm and the catalog `serve_mtp` gate included) and as the config
  surface resolves it, with the Metal mode a `--gpu` pick and a `--metal` spelling resolve to;
  the catalog's `serve_mtp` read off a file's base name, and carried by the slot options both
  the boot load and a runtime POST load build; a slot's chunk defaults off its own arm outcome (`slot_chunk_defaults`: the
  CPU's short chunk and one-window idle chunk, a GPU's one-window chunk and the idle floor, the resident driver's idle
  chunk at its prompt window or wider); the checkpoint byte budget off the box's RAM (the RAM
  read itself is `modules/dasLLAMA/tests/test_box_ident.das`'s); the `translate` flag and config key (the string and array
  forms, the flag-versus-config precedence, the row and its source on the config surface) and a
  boot whose translation model file is present serving rather than entering setup mode - all
  model-free. One cell is not: a Vulkan slot over
  `Qwen3.5-0.8B-MTP-Q8_0.gguf` and `Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf`, modeling a catalog
  row that lists vulkan (the 0.8B file has no row of its own), speculates with its stream
  device-home, and the device's draft and verify counters count each drafted step; it
  needs `DASLLAMA_GPU=1` and
  skips without it.
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

`cache_probe.das` (same directory) reads the prefix cache on any model in a minute or two: sixteen
chat requests of about a thousand tokens in a fixed cycle over the HTTP route, sent twice. The
cycle: eight conversations opening on one system prompt and tool set, each with a house note of
its own in the user turn, six of them continued with their first reply and a follow-up, one on
another system prompt with the same tools, and the first request repeated. The second sweep sends
the same sixteen bodies byte for byte, so everything is warm: every request should attach at least
its prompt minus one page (a dense model's cache ends at a page boundary; a hybrid's end checkpoint
sits at the last message's close). One line per request with its `cached_tokens` of `prompt_tokens` and its
wall time, then the first sweep's shares by kind, the cold first rounds, the later rounds that
attached their own earlier exchange, what the other-system and repeated requests attached, and the
second sweep's warm share with the count of requests short of it. Against a running server,
`-- --url http://127.0.0.1:8911`; with `-- -m <model.gguf>` it boots a CPU server of its own on
`--port` (18199) with the server's default knobs. `--preserve-thinking` sends the kwarg a client
that keeps `reasoning_content` sends, so the replayed assistant turns render the think block the
generation prompt carried; a hybrid's follow-up attaches its earlier exchange either way, since the
checkpoint sits at the last message's close, ahead of the generation prompt. `test_cache_probe.das`
holds the model-free half: the bodies the cycle builds and the figures off a table of answers.

## Not yet implemented

A raw grammar field beside `response_format`; a regular-expression `response_format`. On the media
path: remote `image_url` fetches.
