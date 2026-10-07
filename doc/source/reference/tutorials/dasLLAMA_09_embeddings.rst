.. _tutorial_dasLLAMA_embeddings:

==========================
dasLLAMA-09 — Embeddings
==========================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Embeddings
    single: Tutorial; Semantic search

Any chat model dasLLAMA loads doubles as an embedder. ``embed(model, text)``
runs one forward pass, pools the decoder's last-layer hidden state
(post-final-norm) and L2-normalizes the result to unit length. The pooling
is the file's: a chat model names none and is read by the mean over every
position; an embedding model such as Qwen3-Embedding names last-token pooling
(``pooling_type`` in its GGUF) and asks for an appended end token, and ``embed``
reads the row at that token, the way the model was trained. A third argument
overrides the file (``POOLING_MEAN``, ``POOLING_CLS``, ``POOLING_LAST``). The
vector width is ``model.config.dim``, the decoder's own embedding dimension.

Run::

   daslang.exe -jit tutorials/dasLLAMA/09_embeddings.das -- model.gguf

One vector per sentence
=======================

``embed`` is the whole API: text in, a fixed-width unit vector out. Because
every vector is unit length, cosine similarity is just the dot product — a
vector's dot product with itself is ``1.0``, the cheapest check that the
result really is normalized. ``cosine`` is the tutorial file's own helper: it
multiplies the two vectors coordinate by coordinate and adds up the results.

.. das-doc: given def cosine(a, b : array<float>) : float { var acc = 0.0; for (x, y in a, b) { acc += x * y }; return acc }
.. code-block:: das

   var m <- load_model("SmolLM2-135M-Instruct-Q8_0.gguf", QuantMode.q8)

   with_job_que() {                 // the forward pass needs the job queue
       setup_dasllama_jobque()
       let qv <- embed(m, "How do I sort a list in Python?")
       print("{length(qv)} floats, self-similarity {cosine(qv, qv)}\n")
   }

``embed`` runs a forward pass, so — like ``generate`` — it must run inside
``with_job_que()``; model code outside one panics.

The third argument overrides the file's pooling — ``POOLING_MEAN``, ``POOLING_CLS``
(the first row) or ``POOLING_LAST`` (the last row). On a chat model the last row is a
different, weaker vector than the mean; on Qwen3-Embedding it is the one the model was
trained for, and the file already asks for it. The fourth keeps only the leading
components, renormalized — the width a vector store is sized for; an embedding model
trained for that Matryoshka read keeps its meaning in them, a chat model's cut is only
the shape.

.. code-block:: das

   with_job_que() {
       setup_dasllama_jobque()
       let qv <- embed(m, "How do I sort a list in Python?")
       let lv <- embed(m, "How do I sort a list in Python?", POOLING_LAST)
       print("last-token read against the mean read: cosine {cosine(qv, lv)}\n")
       let short <- embed(m, "How do I sort a list in Python?", POOLING_MEAN, 64l)
       print("a 64-wide cut: {length(short)} floats\n")
   }

Semantic ranking
================

Retrieval in miniature: embed a handful of candidate sentences, score each
against the query by cosine similarity, and sort. The on-topic answers float
to the top and the unrelated ones sink; the model scores by meaning, so the
concept sentence ("Quicksort and merge sort …") ranks high without sharing the
query's words.

.. das-doc: given var m = Model()
.. das-doc: given var qv : array<float>
.. das-doc: given let candidates = ["Use the sorted() built-in to order a Python list.", "The cat curled up on the warm windowsill."]
.. das-doc: alt
.. code-block:: das

   var scored : array<tuple<float; string>>
   for (c in candidates) {
       let cv <- embed(m, c)
       scored |> push((cosine(qv, cv), c))
   }
   sort(scored) $(a, b) => a._0 > b._0     // most similar first

Running it against SmolLM2-135M ranks the two sorting answers above the oven
and cat sentences — the vectors group by meaning, not by shared tokens.

RAG-grade, and what that means
==============================

A decoder used as an embedder gives **RAG-grade** vectors: good enough to
retrieve the relevant passage from a corpus or rank candidates by meaning,
which is what retrieval-augmented generation needs. It is *not* a substitute
for a dedicated embedding model (BGE, E5, …) — those train with a contrastive
objective and score higher on similarity benchmarks. Reach for ``embed`` when
the model is already loaded and "close enough" retrieval is the job; reach for
a real embedder when embedding quality is the product.

The same ``embed`` verb backs the server's ``POST /v1/embeddings`` route, so an
OpenAI embeddings client talks to a loaded chat model unchanged.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/09_embeddings.das <../../../../tutorials/dasLLAMA/09_embeddings.das>`

   Previous tutorial: :ref:`tutorial_dasLLAMA_audio_chat`

   Next tutorial: :ref:`tutorial_dasLLAMA_thinking`

   The embeddings server route: :ref:`utils_dasllama_server` (``POST /v1/embeddings``)
