.. _tutorial_dasLLAMA_prefix_cache:

======================================
dasLLAMA-15 — The Prefix Cache by Hand
======================================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Prefix Cache
    single: Tutorial; KV Cache

Tutorial 13's scheduler ran a prefix cache for us. Here we drive the same
cache by hand — the shape you need when you build your own scheduler, or when
you want to see exactly where the skipped prefill goes.

The idea in one line: two requests that start with the same system prompt
compute the same KV rows for it, so the first request can *donate* those rows
and the second can *attach* them instead of recomputing. The cache works in
whole KV pages (the pool's ``page_rows``), so the match is page-granular.

Run it like tutorial 01 (SmolLM2-135M-Instruct works)::

   daslang.exe -jit tutorials/dasLLAMA/15_prefix_cache.das -- path/to/model.gguf

Donate, then attach
===================

``create_prefix_cache`` serves the paged sessions of one ``create_kv_pool``
pool. Stream A prefills the full prompt, answers, and donates with
``prefix_insert`` — ``tokens`` is A's full evaled history, and the cache keeps
every full page of it. The preview string is only a label for dashboards:

.. das-doc: given var m = Model()
.. das-doc: given let SYSTEM = "You are a terse assistant."
.. das-doc: given let Q1 = " Do you sell drills?"
.. das-doc: given let Q2 = " When do you open?"
.. code-block:: das

   var pool <- create_kv_pool(m, 16l)
   var cache <- create_prefix_cache()

   let p1 <- encode(m, "{SYSTEM}{Q1}")
   var s1 = create_session(m, pool)
   eval(m, s1, p1)
   prefix_insert(cache, pool, s1, p1, "system prompt")
   release_kv_pages(s1)

Stream B starts fresh from the same pool. ``prefix_attach`` walks B's prompt
against the cached chains: matched pages join B's page table, B's ``n_past``
jumps past them, and we prefill only the tail. The match is capped one token
short of the prompt — the model still needs one eval to make logits:

.. code-block:: das

   let p2 <- encode(m, "{SYSTEM}{Q2}")
   var s2 = create_session(m, pool)
   let matched = prefix_attach(cache, pool, s2, p2)
   // eval() only p2[matched..] — the matched pages are already KV

On the tutorial's 60-token system prompt, B prefills a handful of tail tokens
instead of the whole prompt — the same ``cached N of M`` line tutorial 13's
scheduler printed, now with every step visible.

What the cache holds, and giving it back
========================================

``prefix_chain_list`` snapshots every donation — tokens covered, pages still
alive, hit count, and the preview label. ``prefix_held_groups`` counts the
pool pages the cache keeps for reuse — pages a plain release would have
returned. The cache *borrows* from the pool, so teardown order matters:
``prefix_release`` hands every cached page back, and only then is the pool
safe to delete:

.. code-block:: das

   var inscope chains <- prefix_chain_list(cache)
   for (c in chains) {
       print("chain {c.id}: {c.n_tokens} tokens, {c.hits} hit(s), '{c.preview}'\n")
   }
   print("cache holds {prefix_held_groups(cache)} page(s)\n")
   prefix_release(cache, pool)
   delete cache
   delete pool

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/15_prefix_cache.das <../../../../tutorials/dasLLAMA/15_prefix_cache.das>`

   The scheduler that automates this: :ref:`tutorial_dasLLAMA_serving`

   Sessions and the KV cache: :ref:`tutorial_dasLLAMA_sessions_and_memory`
