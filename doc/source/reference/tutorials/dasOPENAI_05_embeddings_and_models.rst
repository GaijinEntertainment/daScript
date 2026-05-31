.. _tutorial_dasOPENAI_embeddings_models:

===========================================
OPENAI-05 — Embeddings and Models
===========================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Embeddings
    single: Tutorial; Models

This tutorial covers text embeddings (and comparing them), listing and
retrieving models, and why model ids are percent-encoded in the URL path.

Embeddings and cosine similarity
================================

``embed`` turns a string into a vector of floats. Semantically similar texts
produce vectors that point in similar directions — measured by cosine
similarity (1.0 = identical direction, 0.0 = orthogonal):

.. code-block:: das

   require openai/openai_embeddings
   require math

   let v1 = embed(client, "text-embedding-3-small", "a cat sat on the mat")
   let v2 = embed(client, "text-embedding-3-small", "the feline rested on the rug")

   def cosine(a, b : array<float>) : float {
       var dot = 0.0; var na = 0.0; var nb = 0.0
       for (i in range(length(a))) {
           dot += a[i] * b[i]; na += a[i] * a[i]; nb += b[i] * b[i]
       }
       return dot / (sqrt(na) * sqrt(nb))
   }
   print("similarity: {cosine(v1, v2)}\n")

For the full ``EmbeddingResponse`` (per-input vectors + usage), use
``embeddings(client, req)`` with an ``EmbeddingRequest``.

Listing and retrieving models
=============================

.. code-block:: das

   require openai/openai_models

   let list = list_models(client)
   for (m in list.models) {
       print("  {m.id} (owned_by {m.owned_by})\n")
   }

   let one = retrieve_model(client, "gpt-4o-mini")
   if (one.ok) {
       print("retrieved: {one.model.id}\n")
   }

Percent-encoded model ids
=========================

Many providers use model ids with slashes (e.g. ``google/gemma-2:free``).
``retrieve_model`` percent-encodes the id so the ``/`` is not parsed as extra
path segments. ``percent_encode`` (in ``openai_common``) is the helper that does
it:

.. code-block:: das

   require openai/openai_common
   print("{percent_encode("google/gemma-2:free")}\n")
   // → google%2Fgemma-2%3Afree

Quick Reference
===============

==========================================  ==============================================
Function                                    Description
==========================================  ==============================================
``embed(client, model, text)``              Single string → vector (``array<float>``)
``embeddings(client, req)``                 Full ``EmbeddingResult`` (vectors + usage)
``list_models(client)``                     All available models
``retrieve_model(client, id)``              One model by id (path-encoded)
``percent_encode(s)``                       URL-encode a path segment
==========================================  ==============================================

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/05_embeddings_and_models.das <../../../../tutorials/dasOPENAI/05_embeddings_and_models.das>`

   Next tutorial: :ref:`tutorial_dasOPENAI_audio`
