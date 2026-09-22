.. _utils_dasllama_server:

.. index::
   single: Utils; dasllama-server
   single: Utils; OpenAI-compatible server
   single: Utils; LLM inference server

======================================================================
 dasllama-server --- an OpenAI-compatible server over dasLLAMA
======================================================================

``dasllama-server`` (directory: ``utils/dasllama-server/``) is a drop-in
OpenAI-compatible HTTP server for :ref:`dasLLAMA <tutorial_dasLLAMA_hello_generate>`
inference - on the CPU, or with the Metal and Vulkan GPU tiers - written
entirely in daslang over the public ``dasllama`` facade plus the
:ref:`dasHV <tutorial_dasHV_http_server>` HTTP layer. Point any OpenAI client
(opencode, Open WebUI, the ``llm`` CLI, the ``openai`` Python SDK, ...) at
``http://127.0.0.1:<port>/v1``.

It reaches **only** public facade verbs --- ``load_model``,
``create_chat_renderer`` / ``add_user`` / ``render_assistant`` / ``render_turn``,
``eval_batch`` through ``dasllama_scheduler``, ``transcribe``, ``embed`` ---
and that is the point: the server is the acceptance test for the dasLLAMA API.
If a full OpenAI surface builds with no reach into engine internals, the
facade is complete.

.. contents::
   :local:
   :depth: 2


Get it
======

The server ships as a standalone download from the rolling
``dasllama-server`` release,
https://github.com/GaijinEntertainment/daScript/releases/tag/dasllama-server,
refreshed with every daslang release: ``dasllama-server-darwin-arm64.zip``
(macOS, Apple silicon), ``dasllama-server-windows-x64.zip``,
``dasllama-server-linux-x86_64.tar.gz`` and ``dasllama-server-linux-arm64.tar.gz``.
The Linux bundles need glibc 2.35 or newer (Debian 12, Ubuntu 22.04 and later),
the system OpenSSL 3 (``libssl3`` on Debian and Ubuntu, ``openssl-libs`` on
Fedora) and ``curl`` for the catalog downloads; the macOS and Windows bundles
carry their own OpenSSL.

Unpack it and start the supervisor beside the server - ``watchdog``
(``watchdog.exe``), or on macOS the ``dasllama-server.app`` itself, whose
launcher is the watchdog. The server binary beside it is ``dasllama-server.exe``
on Linux and Windows alike; the macOS app carries it as
``Contents/MacOS/dasllama-server``. The watchdog keeps the server up, puts the
dasllama mark in the notification area, and a click on it opens the control
page at ``http://127.0.0.1:8080/``. With no model configured the server starts
in setup mode: pick a model from the catalog, it downloads into
``~/.dasllama/models``, and *serve this model* restarts into it. The config it
writes, ``dasllama-server.toml`` beside the exe, and the tune sidecar beside it
survive an upgrade unpacked over the old directory on Linux and Windows; on
macOS a new ``.app`` replaces the old one whole, so copy the two files out of
``Contents/MacOS`` first and back in after.

The exe is a fat build (``daspkg release --fat``): plain code for the
platform's baseline CPU class - ``x86-avx2`` on x86, ``arm-neon`` on arm64 -
with one clone of every ``[tune]`` kernel per class the engine ships a profile
for (``x86-vnni512``, ``x86-amx``, ``arm-i8mm``), picked from cpuid at start.
On a Mac the Metal kernel choices are raced once at the first start and kept
beside the exe. That is the good default, and it is as far as a solid
executable goes: a fat build cannot tune - its kernels are baked, it carries no
tuner, and ``--tune``, the sidecar exchange and a re-tune have nothing to act
on (the control page's exchange levers refuse). To tune every kernel for this
box, install daslang with dasLLAMA (the SDK) and run the server through the
JIT - the command under *Run from the source tree* - or ``daspkg release`` the
package on that box: either mints the box's own sidecar, every kernel raced on
your own hardware instead of picked from a class profile.

The bundles are not code-signed. macOS quarantines a downloaded app, and an app
started from the download folder runs from a read-only copy where nothing it
writes beside itself survives - so first drag ``dasllama-server.app`` out of
Downloads with the Finder, then either allow it under System Settings >
Privacy & Security after the first refused start, or clear the flag once with
``xattr -dr com.apple.quarantine dasllama-server.app``. Windows SmartScreen
shows an unknown publisher: *More info*, *Run anyway*.


Measure your box
================

The control page's *benchmark* button runs the llama-bench rows on the served
model right in the server - pp512 and tg128, three timed reps each after an
untimed warmup, and on a slot that serves two streams or more tg128@N, N
streams (up to four) through the scheduler, the summed rate - and shows tokens
per second beside the hardware line. Every route that could contend holds for
the run (the text, audio and speech routes answer 503, the model-switching,
bake and download routes 409). A run is seconds on a small model. The same
rows from a shell, and the llama.cpp comparison the page cannot run itself,
come from ``dasllama-bench`` beside the server (``dasllama-bench.exe`` on Linux
and Windows, ``dasllama-server.app/Contents/MacOS/dasllama-bench`` on a Mac)::

   ./dasllama-bench.exe -m ~/.dasllama/models/<model>.gguf           # pp512/tg128, five reps
   ./dasllama-bench.exe -m <model.gguf> --ref /path/to/llama-bench   # and llama.cpp on the same GGUF

The two run the same rows, not the same numbers: the page measures the served
slot as it is - its device, its KV codec - while ``dasllama-bench`` runs f16
KV on the CPU unless ``-ngl`` says otherwise, five reps to the page's three.
The page's result names what produced it: the device that served the rows,
the KV codec, the exec tier (``exe-native`` in a bundle, ``jit`` from the SDK),
the tune state (``fat`` in a bundle; ``tuned`` or ``untuned`` under the JIT),
and the ``llama-bench`` line that matches it.


Run from the source tree
========================

Run under ``-jit`` --- the interpreter is refused, it is far too slow for inference::

   bin/daslang -jit utils/dasllama-server/main.das -- --model <model.gguf> [--port 8080] [--quant q8] \
       [--asr <asr.bin>] [--asr-workers 2] [--mmproj <mmproj.gguf>] [--image-mmproj <mmproj.gguf>] \
       [--ctx 4096] [--streams 4] [--chunk 64] [--page-rows 64] [--prefix N] [--tune]

.. list-table::
   :header-rows: 1
   :widths: 14 6 10 70

   * - Flag
     - Short
     - Default
     - Meaning
   * - ``--config``
     - ``-c``
     - *auto*
     - TOML config file; keys mirror the long flag names, explicit CLI flags override. Without the flag, a ``dasllama-server.toml`` in the cwd or next to the program loads automatically
   * - ``--model``
     - ``-m``
     - *(required)*
     - GGUF model to serve (here or in ``--config``)
   * - ``--port``
     - ``-p``
     - ``8080``
     - Listen port
   * - ``--quant``
     - ``-q``
     - ``q8``
     - Weight serving mode: ``fp32`` | ``q8`` | ``q4`` (the legacy requant tier); the file-format spellings ``q8_0`` | ``q4_0`` | ``q4_k`` | ``q5_k`` | ``q6_k`` | ``mxfp4`` | ``f16`` | ``bf16`` select ``q8``, where those planes serve natively
   * - ``--kv-dtype``
     -
     - ``f16``
     - KV-cache codec: ``f32`` | ``f16`` | ``q8_0`` | ``tq4`` (rotated 4-bit; needs a power-of-two head size)
   * - ``--gpu``
     -
     - ``auto``
     - GPU backend: ``auto`` (metal or vulkan when detected, else CPU) | ``off`` | ``metal`` | ``metal-required`` | ``vulkan``. ``vulkan`` serves a model that fits the card whole from the GPU, every stream's cache with it; the control page's model card says what a slot got. Details under *The gpu key*
   * - ``--asr``
     - ``-a``
     - ---
     - ASR model (whisper / parakeet / qwen3-asr) --- enables the ``/v1/audio/*`` routes
   * - ``--asr-workers``
     -
     - ``1``
     - Long-lived ASR request threads; each owns a model and a reusable session. ``2`` permits two parallel transcriptions
   * - ``--mmproj``
     -
     - ---
     - mmproj GGUF for the Qwen3-ASR route (paired with ``--asr``)
   * - ``--tts``
     -
     - ---
     - TTS model GGUF (kitten-nano, kitten-mini, kokoro-82m with the front-end packs ``tts_g2p.bin`` + ``tts_postag.bin`` beside them; or a ``pocket-tts-<lang>-q8`` file, which reads text, needs no pack and clones a voice) --- enables ``/v1/audio/speech``
   * - ``--tts-lane``
     -
     - ``q8``
     - Weight lane the speech worker pins around its load: ``q8`` (the prepared quant image beside the GGUF) or ``f32`` (the file's own planes). An unknown spelling warns and serves ``q8``
   * - ``--tts-voices-dir``
     -
     - ---
     - Directory of voice clips (wav / flac / mp3 / ogg, a few seconds of one speaker each, 60 s at most) a cloning speech model adds to its voices at boot, each under its file's stem; a clip that does not decode or runs past 60 s is logged and skipped, the key on a model that cannot clone is logged and ignored
   * - ``--image-mmproj``
     -
     - ---
     - Vision mmproj (gemma4uv, gemma4v or gemma3v, sniffed) for the default model --- the chat route then accepts ``image_url`` content parts. When the file also carries a gemma4a audio encoder (the E-series mmproj carries both towers) the same flag arms native audio: ``input_audio`` parts serve through the same slot
   * - ``--ctx``
     -
     - *model*
     - Context-length cap in tokens (default: the model's trained ``context_length``)
   * - ``--max-tokens``
     -
     - ``16384``
     - Default reply token budget when a request omits ``max_tokens`` (clamped to ``--ctx`` per request)
   * - ``--streams``
     - ``-s``
     - ``4``
     - Max concurrent generation streams
   * - ``--threads``
     - ``-t``
     - ``16``
     - Worker-lane cap for the matmul dispatch (``-1`` = all cores). Decode is bandwidth-bound, so an uncapped dispatch only fights the rest of the box
   * - ``--team-dispatch``
     -
     - ``hybrid``
     - ``hybrid``: the LLM uses the worker team while the ASR and TTS workers run inline; ``team``: every caller uses serialized team publishes; ``inline``: every caller runs independently
   * - ``--chunk``
     -
     - ``64``
     - Prefill quantum in tokens: decode stalls at most this many per tick
   * - ``--page-rows``
     -
     - ``64``
     - KV page size in positions for paged serving
   * - ``--prefix``
     -
     - *auto*
     - Prefix-cache retention cap in pages (auto: one full context per stream; ``-1`` = unbounded)
   * - ``--flat``
     -
     - ---
     - Flat preallocated KV sessions --- disables paged serving and the prefix cache
   * - ``--mtp``
     -
     - *auto*
     - MTP/NextN self-speculative decode. Unset, a slot turns it on when it runs one stream (``streams = 1``) host-cached and leaves it off otherwise: at one stream the draft-and-verify round cuts decode time on the dense Qwen3.5 MTP models (0.8B 1.20x, 4B 1.21x, 9B 1.10x), at several streams the plain batched step is faster, and a device-resident slot (``--gpu vulkan``) keeps plain decode, since an armed round keeps every stream's cache on the host. ``true`` / ``false`` set it outright. It needs a model with an in-file NextN head (the ``-MTP-`` GGUFs); on any other model the server logs one line and serves plain. Greedy requests are output-invariant; a sampled request draws each verify row with its own sampler and keeps the plain sampled distribution, at a lower acceptance rate. ``/v1/stats`` reports ``mtp_drafted`` / ``mtp_accepted``
   * - ``--models-dir``
     -
     - ``~/.dasllama/models``
     - Where the model catalog downloads land (``DASLLAMA_MODELS_DIR`` overrides both this and the config key)
   * - ``--tune``
     -
     - ---
     - Re-tune this box's dasLLAMA kernels, then relaunch (see *Per-box tuning*; a fat build carries no tuner and ignores it)
   * - ``--help``
     - ``-?``
     - ---
     - Show help and exit

A config file replaces long command lines; keys are the long flag names with
underscores. A ``dasllama-server.toml`` in the cwd, or next to the program (the
exe directory in a release bundle), loads automatically; ``--config other.toml``
picks a different one:

.. code-block:: toml

   model = "D:/models/SmolLM2-135M-Instruct-Q8_0.gguf"
   port = 8080
   quant = "q8"
   kv_dtype = "tq4"   # rotated 4-bit KV - half the q8_0 cache bytes
   ctx = 4096
   max_tokens = 4096  # default reply budget for clients that omit max_tokens
   streams = 4
   threads = 16       # matmul dispatch lane cap; -1 = all cores
   team_dispatch = "hybrid"
   asr_workers = 2    # two independent transcription requests; each worker owns an ASR model

Config precedence is ``defaults < config TOML < explicit CLI flags`` - unless
the TOML carries ``authoritative = true`` (what the control page saves), which
flips the top: ``defaults < CLI < authoritative TOML``.

Several models serve live from one process through a ``[[models]]`` roster
instead of the flat ``model`` key. Requests route on their ``"model"`` field
(absent means the default entry). Execution is serialized - one scheduler steps
at a time - switches are fast, and every cache level survives a switch: host
weights stay mmap'd, each slot keeps its own KV pool and prefix cache, and one
model's GPU state lives in VRAM at a time (the tier drops and re-arms on
switch; ``backend = "cpu"`` slots never evict the GPU owner). Blank keys
inherit the flat defaults; ``backend`` is ``auto`` | ``cpu`` | ``gpu``, and
per-entry ``ctx``, ``quant``, ``kv_dtype``, ``streams``, ``chunk``,
``page_rows``, ``prefix``, ``mtp`` and ``image_mmproj`` override per model:

.. code-block:: toml

   [[models]]
   name = "qwen"      # route id (default: the file's basename)
   path = "D:/models/Qwen3.6-35B-A3B-Q8_0.gguf"
   default = true

   [[models]]
   name = "smol"
   path = "D:/models/SmolLM2-135M-Instruct-Q8_0.gguf"
   backend = "cpu"    # never touches the device - alternating with the GPU slot costs nothing

Chat and completion requests batch continuously (``dasllama/dasllama_scheduler.das``):
up to ``--streams`` generations run concurrently through one ``eval_batch``
decode step per tick, with long prompts prefilled in ``--chunk``-token slices,
so a new arrival never stalls the running streams for more than one chunk.
Requests beyond ``--streams`` queue (up to 32; then 503). KV is paged by
default - cache memory tracks each stream's actual context, and finished
streams donate their pages to a prefix cache, so a repeated prompt prefix (a
shared system prompt, the next turn of the same conversation) attaches instead
of re-prefilling. Clients whose connection drops mid-generation are evicted
within a tick. Audio uploads queue to long-lived ASR workers and do not block
chat generation. OpenAI is stateless --- the client resends the full transcript
each turn.


Setup mode and the model catalog
================================

A start with no LLM model at all - no ``--model``, no config, or every
configured path missing - and no servable ``--tts`` boots into setup mode: the
port opens, the control page serves, every inference route answers with a
clean error, and the page leads with the model catalog - a curated, sha-pinned
list of current models (``model_catalog.das``, a view over the module's one
model set; commit-pinned HF URLs, canonical sha256, one download at a time,
``curl -C -`` resume). A finished download is verified against its pinned sha
before it is renamed into ``models_dir`` with the ``.sha`` sidecar beside it. In
setup mode a downloaded entry offers *serve this model* - the page writes the
config and restarts (exit 4, the watchdog contract) into serving it. The same
catalog stays available on a serving server for pulling more models.

Catalog entries carry their towers: a vision-capable row offers its pinned
mmproj (download, *enable vision*, restart), a dictation strip under the table
offers the ASR tower (parakeet v3), and a speech strip offers the text-to-speech
set - the two front-end packs, then one model, or a Pocket file that stands
alone. Setup-mode *serve this model* wires any tower already on disk. Each row
wears a fit badge (fits gpu / fits / tight / too big) from the box facts the
``/catalog`` document carries; the advertised working set is a hint, not a load
gate.

``--tts`` alone is a serving start, not a setup start: a speech-only server has
no LLM slot, so ``/v1/stats`` answers the slotless shape with ``setup`` false.
A ``--tts`` the server cannot serve degrades, it does not die: a missing file
or a failed load is logged, the speech route is dropped, and the boot serves
whatever is left - the LLM slots if any loaded, else setup mode.


Per-box tuning
==============

This section is the JIT run's - the SDK path. The standalone download is a fat
build: one baked clone of every kernel per CPU class, picked at start, no
tuner, no sidecar of its own (only the runtime section its first start writes
beside the exe); nothing below applies to it, and ``--tune`` is ignored there.

The server declares ``[tune_policy(missing = "auto")]``, so the first start on
an untuned box runs the dasLLAMA kernel tuner, writes the per-box manifest, and
relaunches itself with the winners; thereafter it serves directly and logs the
tune status at startup. ``--tune`` forces a re-tune. ``DAS_TUNE_POLICY=error``
skips per-start tuning while developing (it prints the tuner command instead of
running it).

The winners live at ``<das_root>/dasllama.tune.json`` and are shared by every
dasLLAMA application on the box. Three sibling CLI tools ship alongside the
server, each with the same ``[tune_policy(missing = "auto")]`` and reading the
same manifest: ``ask`` (a one-shot ``--prompt`` to completion, reporting ttft
and prefill/decode t/s), ``wav2txt`` (an ``--file`` audio to transcript,
reporting decode/transcribe time and the real-time factor) and ``txt2wav`` (a
``--tts`` model plus ``--text`` or ``--file`` to a WAV, reporting the per-stage
synthesis time and the real-time factor). Whichever of the four you run first
tunes the box; the rest are then instant. See :ref:`Kernel tuning <tune>` for
the framework.

The sidecar exchange - dasllama.io's tune-sidecar service - is closed by
default: the three config-only keys ``exchange_accept`` (``verified`` | ``any``
| ``off``, default ``off``), ``exchange_submit`` (``ask`` | ``always`` |
``never``, default ``never``) and ``exchange_url`` are the only way in. A build
without dasLLVM answers 404 on every ``/exchange*`` route, and a fat release
refuses the levers.


The gpu key
===========

``gpu`` (``auto`` | ``off`` | ``metal`` | ``metal-required`` | ``vulkan``) is
the backend selector, and unset behaves as ``auto``: the boot probes the box and
serves on the best detected backend - the Metal rails where the box has them,
else the Vulkan tier when a device answers, else the CPU - logging one
``gpu backend auto-detected:`` line. ``gpu = off`` is the explicit opt-out, and
``DASLLAMA_GPU=0`` in the environment keeps its meaning. A model the armed tier
cannot serve falls back to the CPU with the reason on the control page.

``gpu = vulkan`` asks for the whole-model resident driver first: a model that
fits the card serves entirely from VRAM, and so do its streams - the driver
holds a K/V region per stream, the prompt prefills on the device in 512-token
windows, the streams decode in one batched step, and a conversation's next turn
picks up its earlier rows from the region they were left in. ``/v1/stats``
counts every call the device handed back to the CPU (``gpu_cpu_passes``); a
slot served this way keeps it empty. The control page's model card says which
of these a slot got (``served``) and why it got less when it did
(``served_note``). A recurrent (deltanet) model serves the same way with two
differences: a prompt prefills in one piece, and a next turn re-prefills its
whole transcript. A slot with a vision or audio tower, and a self-speculative
slot, keep host-cached sessions. Beneath the driver the per-op rails arm as the
fallback for a model that does not fit: expert stacks sized automatically
(resident layers fill the VRAM budget, the rest stream) plus the deltanet,
attention and dense rails. The config keys ``gpu_layers`` / ``gpu_stream`` are
``0`` = auto; ``gpu_dn`` / ``gpu_attn`` / ``gpu_dense`` / ``gpu_vram_mb``
override the rest, and the ``DASLLAMA_GPU_*`` environment variables override
everything.


Endpoints
=========

.. list-table::
   :header-rows: 1
   :widths: 10 26 64

   * - Method
     - Path
     - Notes
   * - ``GET``
     - ``/``
     - The control page (``control.html`` beside the server): live stats and charts, the models panel (per-slot cards with state and GPU badges, prefix hit rate, activate buttons), the stream swimlane, a chat panel (every sampling knob, thinking inline, mic input, a speaker button under ``--tts``), a speech studio, the config editor with the ``[[models]]`` roster and save/restart, GC and drain buttons. Polls ``/v1/stats`` and ``/v1/streams`` at 1 Hz
   * - ``GET``
     - ``/v1/models``
     - Lists every served slot (and ``--asr`` if loaded); requests route on these ids through their ``"model"`` field
   * - ``POST``
     - ``/v1/models/activate``
     - ``{"model": name}`` --- make ``name`` the default + stepped slot and move the GPU tier to it now (loopback-only; 409 while work is live)
   * - ``POST``
     - ``/v1/models/load``
     - ``{"path", "id"?, "backend"?, "quant"?, "ctx"?, "image_mmproj"?, "activate"?}`` --- load a GGUF into a new serving slot with no restart (loopback-only; 409 on a live stream set, a taken id, or a GGUF another slot already serves)
   * - ``POST``
     - ``/v1/models/unload``
     - ``{"model": name}`` --- free the slot's weights, KV and VRAM; the default slot refuses (loopback-only)
   * - ``POST``
     - ``/v1/chat/completions``
     - Chat; ``stream: true`` gives SSE, else a buffered reply. OpenAI function calling (``tools``); ``image_url`` parts under ``--image-mmproj``; ``input_audio`` parts when the mmproj carries the audio tower (one image or one clip per request, on the final user message)
   * - ``POST``
     - ``/v1/completions``
     - Raw completion; ``stream: true`` gives SSE, else buffered
   * - ``POST``
     - ``/v1/embeddings``
     - Mean-pooled, L2-normalized sentence embeddings
   * - ``POST``
     - ``/v1/audio/transcriptions``
     - Speech to text (multipart upload; needs ``--asr``); ``response_format=verbose_json`` adds timed segments
   * - ``POST``
     - ``/v1/audio/translations``
     - Speech to English text (needs ``--asr``)
   * - ``POST``
     - ``/v1/audio/speech``
     - Text to speech (needs ``--tts``): ``{"input", "voice"?, "speed"?, "response_format"?}``; ``wav`` (default) is 16-bit PCM at the model's rate, ``pcm`` the raw samples, the compressed formats answer 400. One synthesis at a time, 16 queued
   * - ``POST``
     - ``/v1/audio/phonemes``
     - ``{"input", "voice"?}`` gives the normalized text, the dialect the voice speaks (``lang``) and, per chunk a synthesis would take, its phoneme string --- empty for a text-reading model such as Pocket (needs ``--tts``)
   * - ``GET``
     - ``/v1/stats``
     - Scheduler counters (``gen_tokens``, ``prefill_tokens``, TTFT, ``mtp_drafted`` / ``mtp_accepted``, the media counters), memory footprint, the hardware line, the ASR and speech worker state, the ``tts`` block while a speech model is configured, ``gpu_cpu_passes``, and ``models[]`` --- one entry per slot with ``served``, ``served_note``, the towers it loaded and its cache counters
   * - ``GET``
     - ``/v1/streams``
     - Per-stream states and text tails, the prefix-cache chains, recent ASR jobs
   * - ``GET``
     - ``/v1/images``
     - Per-slot prepared-image (``.dlim``) inventory: source path, mapped flavor, each image's info
   * - ``POST``
     - ``/vad``
     - Silero speech spans over an uploaded clip --- the control page's waveform overlay (120 s at most)
   * - ``GET``
     - ``/catalog``
     - The curated model list with local presence, the ``asr`` tower row, the ``tts`` list, the box memory facts and the download state machine
   * - ``POST``
     - ``/catalog/download``
     - ``{"name": <entry>}`` --- start one catalog download (sha-verified; ``{"name", "tower": "vision"}`` / ``{"tower": "asr"}`` pull a tower, ``{"tower": "tts", "file": <file>}`` one file of the speech set)
   * - ``GET`` / ``POST``
     - ``/bench``
     - Read the bench state, mode, log and result / start the quiesced benchmark: in process by default (pp512 and tg128 on the served model, then tg128@N on a slot serving two streams or more), or the llama.cpp A/B child when ``lcpp_bin`` is configured on a source-tree daslang (``POST`` is loopback-only)
   * - ``GET`` / ``POST``
     - ``/bake``
     - Read the bake state and log / bake the slot's prepared ``.dlim`` image through ``dasllama-convert`` (``POST`` is loopback-only)
   * - ``GET`` / ``POST``
     - ``/config``
     - The effective config with per-key source (``default`` / ``cli`` / ``toml``), the ``[[models]]`` roster and the GPU tier status / validate and save an authoritative TOML, applied on the next restart (``POST`` is loopback-only)
   * - ``GET``
     - ``/exchange``
     - The tune-sidecar exchange surface: policy, the consent state, the current sidecar's identity and share state
   * - ``GET``
     - ``/exchange/matches``
     - Live lookup of this box against the exchange (a network call --- seconds)
   * - ``POST``
     - ``/exchange/apply``
     - ``{"sha": ...}`` --- download, validate and adopt that sidecar, then drain and restart
   * - ``POST``
     - ``/exchange/submit``
     - Privacy-strip and submit this box's own tune to the exchange
   * - ``POST``
     - ``/exchange/retune``
     - Arm a local re-tune and restart --- the next boot races this box
   * - ``POST``
     - ``/exchange/consent``
     - ``{"accept": true|false}`` --- record the first-contact choice about contacting the exchange; replies ``{ok, accepted, restarting?}``, and an accept on an untuned or stale box drains and restarts the server so the boot resolver runs the lookup
   * - ``POST``
     - ``/gc``
     - Schedule a validated collection at the next lifecycle safe point; concurrent requests coalesce
   * - ``POST``
     - ``/restart``
     - Drain, then exit 4 --- the watchdog relaunches with the saved config
   * - ``POST``
     - ``/shutdown``
     - Stop admitting new work, drain, then exit

Unknown endpoints answer 404 through a catch-all that logs the method, the
path and the body head; known routes warn per ignored field.


Chat
----

.. code-block:: sh

   curl http://127.0.0.1:8080/v1/chat/completions -H 'Content-Type: application/json' -d '{
     "messages": [{"role": "user", "content": "Say hello in one word."}],
     "max_tokens": 16, "stream": false, "truncation": "auto"
   }'


Sampling parameters
-------------------

Both completion routes accept the OpenAI sampling fields plus the llama.cpp
extensions; an absent field keeps the greedy default.

.. list-table::
   :header-rows: 1
   :widths: 20 14 66

   * - Field
     - Default
     - Meaning
   * - ``temperature``
     - ``0``
     - ``<= 0`` is greedy argmax
   * - ``top_k``
     - ``0`` (off)
     - keep only the k highest logits
   * - ``top_p``
     - ``1`` (off)
     - nucleus: keep the smallest probability mass ``>= top_p``
   * - ``min_p``
     - ``0`` (off)
     - drop tokens with probability ``< min_p * max_prob``
   * - ``repeat_penalty``
     - ``1`` (off)
     - multiplicative repetition penalty over the recent window, once per unique token
   * - ``presence_penalty``
     - ``0``
     - flat logit subtraction for every distinct token in the recent window
   * - ``frequency_penalty``
     - ``0``
     - per-occurrence logit subtraction
   * - ``seed``
     - ``0``
     - non-zero overrides the stream's RNG seed; identical no-seed requests already reproduce

The cutoffs shape the sampled distribution, so they act only with
``temperature > 0``; the penalties apply in greedy mode too.


Thinking and ``reasoning_content``
----------------------------------

``enable_thinking`` is tri-state. Absent, the model template's own default is
in force (Qwen3/3.5/3.6, GLM, gemma-4 and gpt-oss templates default on; a bare
Qwen Instruct branch defaults off); a present bool - top-level or the
llama.cpp spelling ``"chat_template_kwargs": {"enable_thinking": ...}`` -
overrides it. ``false`` on a ``<think>`` family appends the template's empty
think block so the model answers directly; ``false`` on gemma-4 prefills the
closed empty thought channel. A thinking reply's reasoning span comes back as
``reasoning_content`` with ``content`` clean of the family's markers: on the
message for buffered requests, and as ``delta.reasoning_content`` chunks,
streamed before the ``delta.content`` chunks, for ``stream: true``. The field is
absent when the model did not think.


Context truncation
------------------

An over-context rendered prompt returns 400. ``truncation: "auto"`` keeps the
system messages and the tools and drops the oldest complete user-led turns
until the prompt plus the requested ``max_tokens`` budget fits; when the
system, tools and latest turn cannot fit, the request still returns 400.
``finish_reason: "length"`` means generation consumed its output budget.


Tool / function calling
-----------------------

``/v1/chat/completions`` speaks the OpenAI function-calling protocol: pass
``tools`` (and ``tool_choice``; ``"none"`` disables, the forced-function object
form is not honored), get back ``finish_reason: "tool_calls"`` with
``message.tool_calls``, send the results as ``role: "tool"`` messages, repeat.
Assistant ``tool_calls`` turns and ``role: "tool"`` results replay through the
chat template on each stateless resend, so agent loops (opencode, pi, ...) work
end to end. A thinking model that calls tools returns ``reasoning_content`` and
``tool_calls`` in one response.

The wire format follows the model family: **hermes** (Qwen2.5 / Qwen3 family -
a ``<tools>`` system block, ``<tool_call>`` JSON), **harmony** (gpt-oss -
developer-turn TypeScript namespace defs, commentary-channel recipient calls),
**gemma4** (gemma-4 - the declaration / call / response DSL), **mistral**
(v0.3+ - ``[AVAILABLE_TOOLS]`` defs, a ``[TOOL_CALLS]`` array) and
**llama_json** (llama-3.x - the whole reply is one ``{"name","parameters"}``
object, results on the ``ipython`` role). A model whose chat template declares
no tool format gets a 400. Streaming with tools buffers the native envelope and
emits the parsed calls as one ``delta.tool_calls`` chunk at finish.


Images
------

A slot started with ``--image-mmproj`` accepts the OpenAI content-parts image
form. The URL must be a ``data:`` URI - the server never fetches a remote URL:

.. code-block:: sh

   curl http://127.0.0.1:8080/v1/chat/completions -H 'Content-Type: application/json' -d '{
     "messages": [{"role": "user", "content": [
       {"type": "image_url", "image_url": {"url": "data:image/jpeg;base64,'"$(base64 < cats.jpg | tr -d '\n')"'"}},
       {"type": "text", "text": "What animals are in this picture?"}
     ]}], "max_tokens": 64
   }'

The image is decoded and encoded on a dedicated vision worker thread, and its
soft-token rows prefill between the two token spans of the rendered turn. One
image per request, on the final user message: two images is a 400, an image
anywhere else is dropped with a warning. A slot with no vision arm answers 400.
An image stream neither reads nor writes the prefix cache, and
``usage.prompt_tokens`` counts the soft-token rows as positions. The payload
caps at 32 MB of file and 67 MP decoded.


Embeddings
----------

``input`` is a string or an array of strings. Each vector is ``model.config.dim``
floats, mean-pooled over the decoder's last-layer hidden state (post-final-norm)
and L2-normalized. A decoder-only LLM used as an embedder gives **RAG-grade**
vectors (good for retrieval / similarity), not a substitute for a dedicated
embedding model. See :ref:`tutorial_dasLLAMA_embeddings` for the facade side.

.. code-block:: sh

   curl http://127.0.0.1:8080/v1/embeddings -H 'Content-Type: application/json' -d '{
     "input": ["the quick brown fox", "a lazy dog"]
   }'
   # -> {"object":"list","data":[{"object":"embedding","embedding":[...],"index":0}, ...],
   #     "model":"...","usage":{"prompt_tokens":N,"total_tokens":N}}


Transcription (with ``--asr``)
------------------------------

.. code-block:: sh

   curl http://127.0.0.1:8080/v1/audio/transcriptions \
     -F file=@audio.wav -F response_format=verbose_json

ASR work runs on its own worker threads beside the HTTP tick and the LLM
scheduler. Requests beyond the worker count wait in a bounded queue (32
uploads; then 503).


Supervised deployment
=====================

The server runs under the shared watchdog in ``utils/watchdog/``, in the
release bundle and in a JIT deployment alike. The watchdog needs no arguments:
in the bundle it finds the baked exe beside it, in a JIT deployment ``main.das``
beside ``bin/Release/daslang.exe``, and ``watchdog.json`` pins the name so logs
land in ``logs/dasllama-watchdog.log``, turns the tray on and names ``tray.ico``
as its mark. From the source tree the watchdog does not sit beside the script,
so pass ``--cwd``::

   bin/Release/watchdog.exe --cwd utils/dasllama-server --jit-stack

The first JIT start on an untuned box writes the tune sidecar and exits with
code 3; the watchdog recognizes that bootstrap exit and relaunches. It writes
rotating JSON-line logs, polls ``/v1/models``, and after a crash copies the
minidump with the matching JIT artifacts, tune manifest and log into
``logs/crashes/``, then restarts with bounded exponential backoff.

``daspkg release --root utils/dasllama-server --out <dir>`` bakes the server
into a standalone bundle: the exe, the shared modules and runtime libraries it
needs, ``watchdog`` beside it, ``dasllama-bench``, ``watchdog.json``,
``control.html`` and ``tray.ico``. Plain ``release`` tunes the kernels on the
build box and ships that box's sidecar; ``release --fat x86-avx2`` (``arm-neon``
on arm64) is what the public download is built from. Keep the deployed
``dasllama-server.toml`` and ``dasllama-server.tune.json`` across upgrades, and
stop a running server first - Windows locks the DLLs.


Testing
=======

The tests in the tool directory are model-gated and JIT-only: they skip
cleanly when the GGUF is absent (set ``DASLLAMA_MODELS_DIR``, one of the knobs
in :ref:`the dasLLAMA knob reference <dasllama_env>`)::

   bin/daslang -jit dastest/dastest.das -- --test utils/dasllama-server/test_openai_server.das

- ``test_openai_server.das`` --- endpoint conformance (``/v1/models``,
  ``/v1/embeddings``, buffered chat, the tools-unsupported 400, the
  unknown-endpoint 404) over the real dashv HTTP client, plus model-free
  ``parse_tool_calls`` unit tests; needs ``tinyllama-1.1b-chat-v1.0.Q8_0.gguf``.
- ``test_openai_server_stream.das`` --- SSE chunk framing, the over-long-prompt
  400, two concurrent clients batching on one server, mid-generation
  disconnect eviction, the prefix cache returning an identical completion for a
  repeated request; needs ``SmolLM2-135M-Instruct-Q8_0.gguf``.
- ``test_openai_server_vision.das`` --- the image route end to end on
  gemma-4-12B with its mmproj and the coco cats jpeg.
- ``test_openai_server_speech.das`` --- the speech route end to end on a
  TTS-only boot; needs ``kitten-nano.gguf`` and the front-end packs beside it.
- ``test_openai_server_audio.das`` and ``test_openai_server_think.das`` --- the
  native-audio content parts and the thinking control.
- ``test_openai_server_mtp.das`` --- the self-speculation default: a
  NextN-headed slot drafts at one stream and decodes plain at four, an explicit
  ``mtp`` wins either way, a head-less model serves plain; needs
  ``Qwen3.5-0.8B-MTP-Q8_0.gguf`` and the TinyLlama file.
- ``test_exchange_client.das``, ``test_model_catalog.das``,
  ``test_setup_mode.das`` --- model-free: the sidecar exchange client against a
  fake exchange, the catalog-table invariants, a slotless setup-mode boot.
- ``tests/`` --- the control page itself under Playwright (Node + chromium),
  against JSON/SSE fixtures captured from a real server; ``npm ci && npx
  playwright test`` in that directory.

``server_bench.das`` (same directory) measures the serving latencies through
the scheduler seam: tok/s, TTFT and inter-token percentiles against batch size,
the decode stall per prefill chunk size, and warm against cold TTFT for the
prefix cache.


Not yet implemented
===================

The request's ``stop`` and ``response_format`` fields and the forced-function
``tool_choice`` object form - each logged when a request carries it. On the
media path: more than one media clip per request, media on earlier turns of a
conversation, and remote ``image_url`` fetches.
