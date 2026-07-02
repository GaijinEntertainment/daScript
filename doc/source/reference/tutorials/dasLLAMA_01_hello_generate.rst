.. _tutorial_dasLLAMA_hello_generate:

===============================
dasLLAMA-01 — Hello, Generation
===============================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; LLM Inference
    single: Tutorial; GGUF

This tutorial introduces ``dasLLAMA`` — CPU large-language-model inference in
pure daslang: load a GGUF model, tokenize, and stream a text continuation.
Everything goes through the public facade, ``dasllama/dasllama``; requiring it
also registers every supported architecture (Llama-2/3, Mistral, Qwen, Phi-3,
Gemma-2/3/4, Qwen-MoE, gpt-oss), so the same program runs any of them.

The companion ``.das`` files are in ``tutorials/dasLLAMA/``. They need a GGUF
model file on disk (models are not shipped with the repo) — a good tiny one is
`SmolLM2-135M-Instruct Q8_0
<https://huggingface.co/bartowski/SmolLM2-135M-Instruct-GGUF>`_ (~145 MB).
Pass the model path on the command line (or set ``DASLLAMA_MODEL``), and always
run with ``-jit`` — interpreted inference is far too slow for model work::

   daslang.exe -jit tutorials/dasLLAMA/01_hello_generate.das -- path/to/SmolLM2-135M-Instruct-Q8_0.gguf

Loading a model
===============

``load_model`` is the one entry point: it reads the GGUF, picks the
architecture and tokenizer from the file's metadata, and loads the weights at
the precision you ask for. ``QuantMode.q8`` (int8) is the fast everyday choice;
``QuantMode.fp32`` is the token-exact reference the test suite validates
against llama.cpp.

.. code-block:: das

   require dasllama/dasllama

   var m <- load_model(path, QuantMode.q8)
   let c = m.config
   print("arch={m.arch} dim={c.dim} layers={c.n_layers} vocab={c.vocab_size} ctx={c.seq_len}\n")

Tokens: encode, decode, piece
=============================

Models consume token ids, not text. ``encode`` uses the model's own tokenizer
(BPE or SentencePiece, chosen at load); ``decode`` is the inverse; ``piece``
decodes a single token — that's what streaming callbacks receive.

.. code-block:: das

   let ids <- encode(m, "Once upon a time")
   // -> [ 6403, 1980, 253, 655]
   print("{decode(m, ids)}\n")       // -> Once upon a time
   print("{piece(m, ids[1])}\n")     // -> " upon"

Generating
==========

A ``Session`` holds the KV cache and scratch buffers for one generation
stream. ``generate`` prefills the prompt, then samples one token at a time,
calling the trailing block with ``(id, piece)`` — return ``false`` to stop
early. ``SamplingParams()`` defaults are greedy (always the most likely
token); :ref:`tutorial 03 <tutorial_dasLLAMA_sampling>` covers the knobs.

The kernels thread through the job queue — wrap generation in
``with_job_que()`` (from ``daslib/jobque_boost``), or model code will panic
asking for one.

.. code-block:: das

   with_job_que() {
       set_jobque_fork_pool(true, true)   // pool per-job fork contexts
       var s = create_session(m)
       generate(m, s, ids, SamplingParams(), 48l) $(_id, piece) {
           fprint(fstdout(), piece)
           fflush(fstdout())
           return true
       }
   }

Stats
=====

``stats`` reports the last ``generate``/``respond`` call on the session:
token counts, time to first token, and prefill / generation throughput.

.. code-block:: das

   let st = stats(s)
   print("prompt {st.n_prompt} tok | gen {st.n_gen} tok | ttft {int(st.ttft_s * 1000.0lf)}ms | ")
   print("prefill {int(st.prefill_tps)} t/s | gen {int(st.gen_tps)} t/s\n")

Quick Reference
===============

==============================================  ========================================================
Function                                        Description
==============================================  ========================================================
``load_model(path, mode)``                      GGUF → ``Model`` (arch + tokenizer auto-detected)
``create_session(model)``                       Fresh KV cache + scratch, sized to ``config.seq_len``
``encode(model, text)``                         Text → token ids (model's own tokenizer)
``decode(model, ids)`` / ``piece(model, id)``   Token ids → text / one token's text
``generate(model, s, prompt, params, n)``       Streaming generation, trailing block per token
``stats(s)``                                    Counts, ttft, prefill / generation tok/s
==============================================  ========================================================

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/01_hello_generate.das <../../../../tutorials/dasLLAMA/01_hello_generate.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_chat`
