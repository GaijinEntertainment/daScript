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
CPU inference, written entirely in daslang over the public ``dasllama``
facade plus the :ref:`dasHV <tutorial_dasHV_http_server>` HTTP layer. Point any
OpenAI client (opencode, Open WebUI, the ``llm`` CLI, the ``openai`` Python SDK,
…) at ``http://127.0.0.1:<port>/v1``.

It reaches **only** public facade verbs --- ``load_model`` / ``create_chat`` /
``add_user`` / ``add_assistant`` / ``respond`` / ``transcribe`` / ``embed`` ---
and that is the point: the server is the acceptance test for the dasLLAMA API
rework. If a full OpenAI surface builds with no reach into engine internals, the
facade is complete.

.. contents::
   :local:
   :depth: 2


Run
===

Run under ``-jit`` --- interpreted inference is far too slow for model work::

   bin/daslang -jit utils/dasllama-server/main.das -- --model <model.gguf> \
       [--port 8080] [--quant q8] [--asr <asr.bin>] [--mmproj <mmproj.gguf>] \
       [--image-mmproj <mmproj.gguf>] [--ctx 4096] [--tune]

.. list-table::
   :header-rows: 1
   :widths: 14 8 14 64

   * - Flag
     - Short
     - Default
     - Meaning
   * - ``--model``
     - ``-m``
     - *(required)*
     - GGUF model to serve
   * - ``--port``
     - ``-p``
     - ``8080``
     - Listen port
   * - ``--quant``
     - ``-q``
     - ``q8``
     - Weight quantization: ``fp32`` | ``q8`` | ``q4``
   * - ``--asr``
     - ``-a``
     - ---
     - ASR model (whisper / parakeet / qwen3-asr) --- enables the ``/v1/audio/*`` routes
   * - ``--mmproj``
     -
     - ---
     - mmproj GGUF for the Qwen3-ASR route (paired with ``--asr``)
   * - ``--image-mmproj``
     -
     - ---
     - vision mmproj GGUF (gemma 3 family) --- the chat route then accepts
       ``image_url`` content parts
   * - ``--ctx``
     -
     - ``4096``
     - Context-length cap in tokens
   * - ``--tune``
     -
     - ---
     - Re-tune this box's dasLLAMA kernels, then relaunch (see *Per-box tuning*)
   * - ``--help``
     - ``-?``
     - ---
     - Show help and exit

Generation runs on one tick thread: a scheduler interleaves every live stream's
decode, so concurrent requests share the model rather than queue behind each
other. Audio transcription and image encoding each run on their own worker
thread and rejoin the tick loop with their result. OpenAI is stateless --- the
client resends the full transcript each turn.

Per-box tuning
==============

The server declares ``[tune_policy(missing = "auto")]``, so the **first start
on an untuned box** runs the dasLLAMA kernel tuner (``gen_tune_probe``), writes
the per-box manifest, and relaunches itself with the winners; thereafter it
serves directly and logs the tune status at startup. ``--tune`` forces a
re-tune. ``DAS_TUNE_POLICY=error`` skips per-start tuning while developing (it
prints the tuner command instead of running it).

The winners live at ``<das_root>/dasllama.tune.json`` and are shared by every
dasLLAMA application on the box — no per-app scope declaration, since requiring
dasLLAMA pulls in its ``[tune_scope]``. Two sibling CLI tools ship alongside
the server, each with the same ``[tune_policy(missing = "auto")]`` and reading
the same manifest: ``ask`` (a one-shot ``--prompt`` → completion, reporting
ttft and prefill/decode t/s) and ``wav2txt`` (an ``--file`` audio → transcript,
reporting decode/transcribe time and the real-time factor). Whichever of the
three you run first tunes the box; the rest are then instant. See
:ref:`Kernel tuning <tune>` for the framework, and
``modules/dasLLAMA/tune_for_this_box.md`` for the measurement discipline.


Endpoints
=========

.. list-table::
   :header-rows: 1
   :widths: 10 34 56

   * - Method
     - Path
     - Notes
   * - ``GET``
     - ``/v1/models``
     - Lists the served model (and ``--asr`` if loaded)
   * - ``POST``
     - ``/v1/chat/completions``
     - Chat; ``stream: true`` → SSE, else buffered
   * - ``POST``
     - ``/v1/completions``
     - Raw completion; ``stream: true`` → SSE, else buffered
   * - ``POST``
     - ``/v1/embeddings``
     - Mean-pooled, L2-normalized sentence embeddings
   * - ``POST``
     - ``/v1/audio/transcriptions``
     - Speech→text (multipart upload; needs ``--asr``)
   * - ``POST``
     - ``/v1/audio/translations``
     - Speech→English text (needs ``--asr``)
   * - ``POST``
     - ``/shutdown``
     - Graceful stop


Chat
----

.. code-block:: sh

   curl http://127.0.0.1:8080/v1/chat/completions -H 'Content-Type: application/json' -d '{
     "messages": [{"role": "user", "content": "Say hello in one word."}],
     "max_tokens": 16, "stream": false
   }'


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


Testing
=======

``test_openai_server.das`` (in the tool directory) is a model-gated, JIT-only
conformance test: it starts the server on its own thread, then drives
``/v1/models``, ``/v1/embeddings``, and ``/v1/chat/completions`` over the real
dashv HTTP client. It skips cleanly when the model GGUF is absent; set
``DASLLAMA_MODELS_DIR`` (one of the knobs in :ref:`the dasLLAMA knob reference
<dasllama_env>`) to a directory containing
``tinyllama-1.1b-chat-v1.0.Q8_0.gguf``, then::

   bin/daslang -jit dastest/dastest.das -- --test utils/dasllama-server/test_openai_server.das


Not yet implemented
===================

Tool / function calling (``tools``, ``tool_choice``) --- parked as a follow-up.
