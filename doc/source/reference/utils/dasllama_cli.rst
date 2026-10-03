.. _utils_dasllama_cli:

.. index::
   single: Utils; dasllama-cli
   single: Utils; LLM command line
   single: Utils; speech command line

======================================================
 dasllama-cli --- dasLLAMA from the shell
======================================================

``dasllama-cli`` (``utils/dasllama-server/cli.das``) is the shell front end of
:ref:`dasLLAMA <tutorial_dasLLAMA_hello_generate>`: the same engine, the same
public facade and the same backend pick as :ref:`dasllama-server
<utils_dasllama_server>`, without the HTTP layer. One model, one conversation,
one process - a script or a cron job asks a model, hears a clip or speaks a
line with no server running. It ships beside the server in every bundle
(``dasllama-cli.exe`` on Linux and Windows,
``dasllama-server.app/Contents/MacOS/dasllama-cli`` on a Mac) and runs from
the source tree under the JIT:

.. code-block:: sh

   bin/daslang -jit utils/dasllama-server/cli.das -- chat --model <model.gguf>

Commands
========

.. list-table::
   :header-rows: 1
   :widths: 14 86

   * - Command
     - What it does
   * - ``complete``
     - Complete a prompt, streamed to stdout; the prompt is ``--prompt``, ``--file`` or the
       first bare argument. ``--image <file>`` with ``--image-mmproj <gguf>`` asks about a
       picture. ``--quiet`` drops the token counters.
   * - ``chat``
     - A conversation from the terminal, or from ``--script <file>`` one message per line.
       ``/help`` lists the slash commands: ``/image`` and ``/audio`` attach a file to the next
       message (``--image-mmproj`` / ``--audio-mmproj``; the gemma-4 E-series mmproj carries both
       towers, pass it twice), ``/read`` sends a file's text, ``/save`` and ``/load`` keep the
       transcript as JSON, ``/regen``, ``/clear``, ``/system``, ``/stats``, ``/speak on`` (with
       ``--tts``, every reply lands in ``reply_<n>.wav`` under ``--out-dir``), ``/exit``. A line
       of ``"""`` opens and closes a multi-line message. A thinking model's reasoning streams
       before its answer, a blank line between; ``--hide-thinking`` hides it, ``--no-think``
       answers directly on a hybrid model.
   * - ``transcribe``
     - An audio file (wav, mp3, flac, ogg) to text through ``--asr`` (a GGUF pair takes
       ``--mmproj``); ``--out`` writes it to a file, ``--lang`` hints the language where the
       model takes one - a model that detects it refuses a hint, and a code the model does
       not speak is refused with the codes it does.
   * - ``speak``
     - Text to a 16-bit WAV through ``--tts`` (``--voice``, ``--speed``, ``--tts-lane q8 | f32``).
       A voice the model does not carry is refused with the voices it does; a speed on a model
       that takes none is refused.
   * - ``talk``
     - The chain: ``--in <audio>`` heard by ``--asr``, answered by ``--model``, spoken by
       ``--tts`` into ``--out``. ``--prompt <text>`` skips the hearing. The transcript, the
       reply and the three stage times are printed.
   * - ``embed``
     - A text's embedding vector, one float per line, or ``--json``.
   * - ``tokenize``
     - A text's token ids and pieces, one ``id<TAB>piece`` per line, or ``--ids``.
   * - ``bench``
     - The llama-bench rows on a model: ``pp512`` and ``tg128`` (``-p``, ``-n``, ``-r``), and
       ``tg128@N`` with ``--npl N``; ``-o md`` prints the table. The standalone
       ``dasllama-bench`` remains the records instrument.

Every example runs as written on a box whose models directory carries the named
files; ``utils/dasllama-server/test_cli.das`` runs each one:

.. code-block:: sh

   dasllama-cli complete -m SmolLM2-135M-Instruct-Q8_0.gguf --quiet "The capital of France is"
   dasllama-cli speak --tts kitten-nano.gguf -o hello.wav "Hello from the command line."
   dasllama-cli transcribe --asr Qwen3-ASR-0.6B-Q8_0.gguf --mmproj mmproj-Qwen3-ASR-0.6B-bf16.gguf question.wav
   dasllama-cli talk --asr Qwen3-ASR-0.6B-Q8_0.gguf --mmproj mmproj-Qwen3-ASR-0.6B-bf16.gguf --model Qwen3.5-0.8B-Q8_0.gguf --tts kokoro-82m.gguf --in question.wav --out answer.wav
   dasllama-cli chat -m gemma-4-E2B-it-Q4_K_M.gguf --image-mmproj mmproj-gemma-4-E2B-it-bf16.gguf --audio-mmproj mmproj-gemma-4-E2B-it-bf16.gguf
   dasllama-cli bench -m SmolLM2-135M-Instruct-Q8_0.gguf --npl 2 -o md

Flags
=====

``<command> --help`` prints the command's own flags, then the groups it takes
(from the source tree ``-?`` or ``--show-help``, since the daslang host takes
``--help`` for itself). The shared flags are the server's knobs under the
server's names; the config file fills whatever they leave empty (below).

.. list-table:: Shared flags (every command)
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--config`` / ``-c``
     - TOML config file (default: ``dasllama-server.toml`` in the cwd, else in ``~/.dasllama``, else beside the program); keys mirror the long flag names, explicit flags win
   * - ``--model`` / ``-m``
     - The LLM GGUF: a path, or a file name under ``--models-dir``
   * - ``--quant`` / ``-q``
     - Weight serving mode: ``fp32`` | ``q8`` (default; the file-format spellings serve natively under ``q8``) | ``q4`` = the legacy requant tier
   * - ``--kv-dtype``
     - KV cache codec: ``f32`` | ``f16`` (default) | ``q8_0`` | ``tq4`` (rotated 4-bit; needs a power-of-two head size); under ``--gpu vulkan`` the whole-model driver holds its cache in the same codec for ``f16``, ``q8_0`` and ``tq4``
   * - ``--gpu``
     - GPU backend: ``auto`` (default: metal or vulkan when detected, else CPU) | ``off`` | ``metal`` | ``metal-required`` | ``vulkan``; ``vulkan`` serves a model that fits the card whole from the device, the conversation's cache with it
   * - ``--metal``
     - Metal serving mode when ``--gpu`` is unset: ``off`` | ``auto`` | ``required``
   * - ``--gpu-layers``
     - vulkan: resident MoE expert-stack layers, offloaded from the end (default auto under ``--gpu vulkan``)
   * - ``--gpu-stream``
     - vulkan: streamed prefill MoE layers below the resident set (default auto under ``--gpu vulkan``)
   * - ``--gpu-dn``
     - vulkan: deltanet (recurrent) layers through the device chain (default on under ``--gpu vulkan``)
   * - ``--gpu-attn``
     - vulkan: full-attention layers through the device chain (default on under ``--gpu vulkan``)
   * - ``--gpu-dense``
     - vulkan: dense attention-side planes resident (default off)
   * - ``--gpu-vram-mb``
     - vulkan: resident-weight VRAM cap override in MB (default: query the device)
   * - ``--ctx``
     - Context length in tokens, served whole: the whole-model GPU driver holds it or declines to the per-op rails, and the load log names the room. Default: the model's trained ``context_length``, shortened to what the card holds
   * - ``--rope-scaling``
     - RoPE scaling override: ``yarn`` | ``linear`` | ``none`` (default: the model file's own ``rope.scaling`` keys; ``none`` drops them, a file's per-pair factor tensors stay). The Qwen families publish the YaRN recipe and enable it as a setting, for long conversations only: ``--rope-scaling yarn --rope-scale 4``; no other vendor validates it, and a non-Qwen file logs a warning. The override is baked into the prepared image under its own lane
   * - ``--rope-scale``
     - RoPE scaling factor for the override (default: the file's ``rope.scaling.factor``; ``yarn`` needs one)
   * - ``--yarn-orig-ctx``
     - YaRN: the original training context the factor extends (default: the file's ``original_context_length``, else its ``context_length``)
   * - ``--threads`` / ``-t``
     - Worker-lane cap for the matmul dispatch (default 16; ``-1`` = all cores)
   * - ``--models-dir``
     - Where a bare model name resolves (default ``~/.dasllama/models``, where the server's catalog downloads land; ``DASLLAMA_MODELS_DIR`` overrides)
   * - ``--tune``
     - Re-tune this box's dasLLAMA kernels, then relaunch (a fat build carries no tuner and ignores it)
   * - ``--verbose``
     - Echo the engine's log records (``dasllama-cli.log`` - ``~/.dasllama/logs`` from an installed build, the das root's ``logs/`` from the source tree: a missing Metal profile, a GPU decline, a model load) to stderr as they land; stdout stays the answer
   * - ``--help`` / ``-?``
     - The command's help (``--show-help`` from the source tree)

.. list-table:: Sampling (``complete``, ``chat``, ``talk``)
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--temp``
     - Sampling temperature; ``0`` = greedy. Unset, ``complete`` is greedy and ``chat`` / ``talk`` sample at 0.7 - under greedy decoding a thinking model loops on its own draft and never reaches its answer
   * - ``--top-k``
     - Top-k cutoff (unset: 20 on the sampled default, off under greedy)
   * - ``--top-p``
     - Nucleus cutoff (unset: 0.95 on the sampled default)
   * - ``--min-p``
     - Drop tokens below this fraction of the top probability (default off)
   * - ``--repeat-penalty``
     - Multiplicative repetition penalty over the recent window (default 1 = none)
   * - ``--presence-penalty``
     - Subtracted from every token already in the window (default 0)
   * - ``--frequency-penalty``
     - Subtracted per occurrence in the window (default 0)
   * - ``--seed`` / ``-s``
     - The sampler's seed for a reproducible run (default: the session's fixed seed)
   * - ``--max-tokens`` / ``-n``
     - Reply token budget (default 256 for ``complete``, 16384 for ``chat`` and ``talk``)

.. list-table:: Speaker (``chat``, ``speak``, ``talk``)
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--play``
     - Also play each spoken reply through the speaker once its WAV has landed; the run blocks until the clip ends. A box with no audio device says so once and keeps writing the files
   * - ``--null-audio``
     - With ``--play``: drive the null audio backend instead of a device (a test box, a CI runner)

.. list-table:: ``complete``
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--prompt`` / ``-p``
     - The prompt (or ``--file``, or the first bare argument)
   * - ``--file`` / ``-f``
     - Read the prompt from this file
   * - ``--image``
     - A picture to ask about (needs ``--image-mmproj``); the family's own thinking default applies
   * - ``--image-mmproj``
     - The vision mmproj GGUF for ``--image``
   * - ``--quiet``
     - Print the completion only - no token counts or rates after it

.. list-table:: ``chat``
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--system``
     - The system prompt
   * - ``--script``
     - Read the turns from this file instead of the terminal, one message per line (slash commands work); the reply follows each
   * - ``--image-mmproj``
     - The vision mmproj GGUF: arms ``/image``
   * - ``--audio-mmproj``
     - The audio mmproj GGUF: arms ``/audio`` (the E-series mmproj serves both towers, pass the same file twice)
   * - ``--tts``
     - A TTS model GGUF: ``/speak on`` writes every reply to a WAV file
   * - ``--voice`` / ``-v``
     - Voice name for ``--tts`` (default: the model's first voice)
   * - ``--speed``
     - Speech speed multiplier for ``--tts`` (default 1.0)
   * - ``--out-dir``
     - Where the spoken replies land, ``reply_<n>.wav`` each (default: the cwd)
   * - ``--hide-thinking``
     - Hide a thinking model's reasoning (default: it streams before the answer, a blank line between)
   * - ``--no-think``
     - Answer directly on a hybrid thinking model (default: the model's own default)
   * - ``--quiet``
     - Print the reply only - no token counts or rates after it

.. list-table:: ``transcribe``
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--asr`` / ``-a``
     - The ASR model (GGUF or ``.bin``): a path, or a file name under ``--models-dir``
   * - ``--mmproj``
     - mmproj for a GGUF ASR model (paired with ``--asr``)
   * - ``--file`` / ``-f``
     - The audio file to transcribe (wav / mp3 / flac / ogg; or the first bare argument)
   * - ``--out`` / ``-o``
     - Write the transcript here instead of stdout
   * - ``--lang`` / ``-l``
     - Language hint (default: auto for a model that detects it, else ``en``); a model that detects the language refuses a hint, a code the model does not speak is refused with the codes it does

.. list-table:: ``speak``
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--tts``
     - The TTS model GGUF: a path, or a file name under ``--models-dir``
   * - ``--text``
     - The text to speak (or ``--file``, or the first bare argument)
   * - ``--file`` / ``-f``
     - Read the text from this file
   * - ``--out`` / ``-o``
     - The WAV file to write (16-bit PCM, the model's rate; default ``out.wav``)
   * - ``--voice`` / ``-v``
     - Voice name or alias (default: the model's first voice); a voice the model does not carry is refused with the voices it does
   * - ``--speed``
     - Speech speed multiplier (default 1.0); a speed on a model that takes none is refused
   * - ``--tts-lane``
     - TTS weight lane: ``q8`` (default; the prepared quant image beside the GGUF) | ``f32`` (the file's own planes)
   * - ``--prof``
     - Print the generator's per-op profile

.. list-table:: ``talk``
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--asr`` / ``-a``
     - The ASR model (GGUF or ``.bin``) that hears ``--in``
   * - ``--mmproj``
     - mmproj for a GGUF ASR model (paired with ``--asr``)
   * - ``--in`` / ``-i``
     - The audio file to answer (wav / mp3 / flac / ogg; or the first bare argument)
   * - ``--prompt`` / ``-p``
     - A text prompt to answer instead of ``--in``
   * - ``--system``
     - The system prompt
   * - ``--lang`` / ``-l``
     - Language hint for the ASR model (default: auto for a model that detects it, else ``en``)
   * - ``--tts``
     - The TTS model GGUF that speaks the reply
   * - ``--voice`` / ``-v``
     - Voice name for ``--tts`` (default: the model's first voice)
   * - ``--speed``
     - Speech speed multiplier for ``--tts`` (default 1.0)
   * - ``--out`` / ``-o``
     - The WAV file the reply lands in (default ``reply.wav``)
   * - ``--no-think``
     - Answer directly on a hybrid thinking model (default: the model's own default)
   * - ``--quiet``
     - Print the reply only - no transcript, stage times or token counts

.. list-table:: ``embed``
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--text``
     - The text to embed (or ``--file``, or the first bare argument)
   * - ``--file`` / ``-f``
     - Read the text from this file
   * - ``--json``
     - Print the vector as a JSON array (default: one float per line)

.. list-table:: ``tokenize``
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--text``
     - The text to tokenize (or ``--file``, or the first bare argument)
   * - ``--file`` / ``-f``
     - Read the text from this file
   * - ``--ids``
     - Print the ids only, space-separated (default: one ``id<TAB>piece`` per line)
   * - ``--no-special``
     - Do not add the model's BOS / leading specials

.. list-table:: ``bench``
   :header-rows: 1
   :widths: 22 78

   * - Flag
     - Meaning
   * - ``--plen`` / ``-p``
     - Prompt tokens per pp rep (default 512, llama-bench ``-p``; 0 skips the row)
   * - ``--ngen`` / ``-n``
     - Generated tokens per tg rep (default 128, llama-bench ``-n``; 0 skips the row)
   * - ``--reps`` / ``-r``
     - Timed repetitions per row after an untimed warmup (default 5, llama-bench ``-r``)
   * - ``--npl``
     - Streams of the batched tg row, ``tg<n>@<npl>``: that many streams through the scheduler, the summed rate (default 0 = no batched row)
   * - ``--output`` / ``-o``
     - ``txt`` (default) | ``md`` (a llama-bench-style table)

The config file
===============

A bare model name resolves under ``--models-dir`` (``~/.dasllama/models``,
where the server's catalog downloads land; ``DASLLAMA_MODELS_DIR`` overrides).

The ``dasllama-server.toml`` in the current directory, else in
``~/.dasllama`` (where the server's control page saves it), else beside the
program - the server's own lookup - fills whatever the flags leave empty: the model (a ``[[models]]``
roster's default entry included), its ``image_mmproj``, the ``asr`` and
``mmproj`` pair, the ``tts`` model and its lane (an ``[[asr]]`` / ``[[tts]]``
roster's first table), ``gpu`` and the Vulkan detail
keys, ``threads``, ``ctx``, ``models_dir``. On a box the server's setup page
configured, ``dasllama-cli chat`` with no flags talks to the served model on
the served backend. Explicit flags win; ``--config`` names another file.

Output goes three ways: the answer - the completion, the reply, the transcript,
the vector, the ids - on stdout alone, so it pipes; the CLI's progress lines and
the token counters (prompt and generated tokens, time to first token, prefill and
decode rates) on stderr, ``--quiet`` dropping the counters; and the engine's own
notices - a missing Metal profile, a GPU decline - in ``dasllama-cli.log``
(``~/.dasllama/logs`` from an installed build, the das root's ``logs/`` from the
source tree), where the server's land too, echoed to stderr as they land
under ``--verbose`` (from the source tree the tune policy guard still prints its
one status line first). A refusal - no
model, a file that does not exist, a flag value that does not parse - names
what is missing and exits non-zero.

Tuning
======

The CLI shares the box's tune winners with the server: requiring dasLLAMA pulls
in its ``[tune_scope]``, and the policy is ``auto`` - the first run from the
source tree on an untuned box tunes and relaunches, every later run is instant,
``--tune`` forces a re-tune. A fat bundle carries no tuner: its kernels are the
class clones, and its first ``dasllama-cli`` run on a Mac races the Metal crowns
once into ``~/.dasllama/tune/dasllama-cli.tune.json``, as the server's first
run does into its own. See :ref:`Kernel tuning <tune>`.

Testing
=======

``utils/dasllama-server/test_cli_args.das`` is the model-free half, run
everywhere: the plan a command line parses to, the help surfaces, the config
file's keys and roster, the model-path resolution, and the chat loop's line
logic. ``utils/dasllama-server/test_cli.das`` runs every command end to end,
one child process per cell, on the small models a stocked box carries - the
``talk`` chain on two model columns and the picture and the clip through the
gemma-4-E2B towers - and skips by name where a model is absent.

.. seealso::

   :ref:`utils_dasllama_server` - the OpenAI-compatible server the CLI sits beside

   :ref:`tutorial_dasLLAMA_audio_chat` - the audio turn the ``/audio`` command and ``talk`` are built on
