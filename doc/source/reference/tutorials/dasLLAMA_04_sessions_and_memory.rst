.. _tutorial_dasLLAMA_sessions_and_memory:

================================================
dasLLAMA-04 — Sessions, the KV Cache, and Memory
================================================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; KV Cache
    single: Tutorial; Sessions

The model is the expensive, read-only part (the weights). A ``Session`` is one
generation stream's state: the attention KV cache plus scratch buffers. This
tutorial sizes that cache, runs independent sessions over one model, hand-rolls
the loop inside ``generate``, and frees everything properly.

Run it like tutorial 01::

   daslang.exe -jit tutorials/dasLLAMA/04_sessions_and_memory.das -- path/to/model.gguf

The KV cache, and why you cap seq_len
=====================================

The KV cache is sized to ``config.seq_len`` *at* ``create_session`` time —
roughly ``2 * n_layers * seq_len * kv_dim`` floats. Models ship with big
native contexts (Llama-3's native ``seq_len`` is 131072, which means tens of
GB of fp32 KV), so cap ``seq_len`` to the context you actually need **before**
creating sessions:

.. code-block:: das

   m.config.seq_len = min(m.config.seq_len, 1024l)
   var s = create_session(m)

On SmolLM2-135M that's the difference between ~377 MB per session at the
native 8192 and ~47 MB at 1024.

One model, many sessions
========================

Each session has its own position (``n_past``), KV cache, and sampling RNG —
two sessions over one model don't see each other. This is how one loaded model
serves many conversations.

eval and sample by hand
=======================

``generate`` is a convenience over two primitives. ``eval`` runs tokens at the
session's current position and advances it — prefill is just the whole prompt
in one call. ``sample`` picks the next token from ``session.logits``:

.. code-block:: das

   var s = create_session(m)
   eval(m, s, prompt)              // prefill: n_past goes 0 -> length(prompt)
   var one : array<int64>
   one |> resize(1)
   for (_step in range64(16l)) {
       let tok = sample(s, SamplingParams())
       print("{piece(m, tok)}")
       one[0] = tok
       eval(m, s, one)             // feed the sampled token back in
   }

``generate`` adds the timing stats and the repetition-penalty window — this
skeleton is all the model ever does.

Freeing: persistent_heap + delete
=================================

A one-shot script can just exit — the context frees everything. A long-lived
process that loads model after model must free explicitly, and that takes both
ingredients:

* ``options persistent_heap`` at the top of the file — a malloc-backed heap
  where ``delete`` returns memory immediately (on the default linear heap a
  mid-context delete is a no-op);
* an explicit ``delete`` of every ``Session`` and ``Model`` you created.

.. code-block:: das

   options persistent_heap
   // ...
   delete s
   delete m   // heap drops by the model's full footprint

One practical note baked into the tutorial file itself: each section lives in
its own function. A daslang function's stack frame is statically sized for
*all* its locals, and the default 16 KB context stack must also fit the
model's forward-pass call chain — piling every section's locals into one fat
``main()`` overflows it. Keep model-driving ``main`` functions lean (or raise
``options stack``).

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/04_sessions_and_memory.das <../../../../tutorials/dasLLAMA/04_sessions_and_memory.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_performance`
