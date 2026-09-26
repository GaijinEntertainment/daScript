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

Shared flags and the config file
================================

Every command takes the server's knobs under the server's names: ``--gpu auto |
off | metal | metal-required | vulkan`` (the same automatic pick - Metal on a
Mac, Vulkan on a card, the CPU otherwise; ``vulkan`` serves a model that fits
the card whole from the device, the conversation's cache with it), ``--quant``,
``--kv-dtype``, ``--ctx``, ``--threads``, ``--models-dir``, and the sampler
where a command samples (``--temp``, ``--top-k``, ``--top-p``, ``--min-p``, the
penalties, ``--seed``, ``-n`` / ``--max-tokens``); ``<command> --help`` lists them
(from the source tree ``-?`` or ``--show-help``, since the daslang host takes
``--help`` for itself). A sampler knob left unset
takes the command's default: greedy for ``complete``, and for ``chat`` and
``talk`` temperature 0.7, top-k 20, top-p 0.95 - under greedy decoding a
thinking model loops on its own draft and never reaches its answer. ``<command> --help`` lists
them beside the command's own flags. A bare model name resolves under
``--models-dir`` (``~/.dasllama/models``, where the server's catalog downloads
land; ``DASLLAMA_MODELS_DIR`` overrides).

The ``dasllama-server.toml`` in the current directory, else beside the
program - the server's own lookup - fills whatever the flags leave empty: the model (a ``[[models]]``
roster's default entry included), its ``image_mmproj``, the ``asr`` and
``mmproj`` pair, the ``tts`` model and its lane, ``gpu`` and the Vulkan detail
keys, ``threads``, ``ctx``, ``models_dir``. On a box the server's setup page
configured, ``dasllama-cli chat`` with no flags talks to the served model on
the served backend. Explicit flags win; ``--config`` names another file.

Output goes three ways: the answer - the completion, the reply, the transcript,
the vector, the ids - on stdout alone, so it pipes; the CLI's progress lines and
the token counters (prompt and generated tokens, time to first token, prefill and
decode rates) on stderr, ``--quiet`` dropping the counters; and the engine's own
notices - a missing Metal profile, a GPU decline - in ``logs/dasllama-cli.log``
under the das root, where the server's land too (from the source tree the tune
policy guard still prints its one status line first). A refusal - no
model, a file that does not exist, a flag value that does not parse - names
what is missing and exits non-zero.

Tuning
======

The CLI shares the box's tune winners with the server: requiring dasLLAMA pulls
in its ``[tune_scope]``, and the policy is ``auto`` - the first run from the
source tree on an untuned box tunes and relaunches, every later run is instant,
``--tune`` forces a re-tune. A fat bundle carries no tuner: its kernels are the
class clones, and its first ``dasllama-cli`` run on a Mac races the Metal crowns
once into ``dasllama-cli.tune.json`` beside the program, as the server's first
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
