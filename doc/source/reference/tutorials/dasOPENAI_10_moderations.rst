.. _tutorial_dasOPENAI_moderations:

=====================================
OPENAI-10 — Moderations
=====================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Moderation

This tutorial covers the content-moderation endpoint — classifying text for
policy violations. As with every dasOPENAI script, the consumer root needs
``options rtti``.

Classifying Text
================

``ModerationRequest`` takes a model and an array of input strings;
``moderations`` returns one verdict per input in ``response.results``:

.. code-block:: das

   require openai/openai_moderations

   let req = ModerationRequest(model = "text-moderation-latest",
       input <- ["some text to classify"])
   let res = moderations(client, req)

Reading the Verdict
===================

Each ``ModerationCategoryResult`` carries a ``flagged`` boolean plus two tables
keyed by **category name** (``"violence"``, ``"hate"``, ``"self-harm"``, …) —
``categories`` (booleans) and ``category_scores`` (confidence in ``[0, 1]``).
Because they are tables, use safe lookup rather than assuming a key exists:

.. code-block:: das

   for (verdict in res.response.results) {
       let violence = verdict.category_scores?["violence"] ?? 0.0lf
       print("flagged={verdict.flagged}, violence score={violence}\n")
   }

.. note::

   The bundled mock flags any input containing the sentinel ``"flagged-example"``
   so both outcomes are demonstrable offline; a real backend decides per its
   model.

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/10_moderations.das <../../../../tutorials/dasOPENAI/10_moderations.das>`

   Previous tutorial: :ref:`tutorial_dasOPENAI_image_generation`

   Next tutorial: :ref:`tutorial_dasOPENAI_completions`
