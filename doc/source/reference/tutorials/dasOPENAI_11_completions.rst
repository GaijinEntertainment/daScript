.. _tutorial_dasOPENAI_completions:

=====================================
OPENAI-11 — Legacy Completions
=====================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Completions

This tutorial covers the older (pre-chat) text-completion endpoint. Prefer chat
(:ref:`tutorial_dasOPENAI_first_chat`) for new code; ``/completions`` exists for
older models and OpenAI-compatible backends that only expose it. As with every
dasOPENAI script, the consumer root needs ``options rtti``.

The completions() Call
======================

``completions`` takes a ``CompletionRequest`` and returns a ``CompletionResult``.
On success, ``response.choices`` holds the generated text and ``finish_reason``,
and ``response.usage`` the token accounting:

.. code-block:: das

   require openai/openai_completions

   let req = CompletionRequest(model = "gpt-3.5-turbo-instruct",
       prompt = "The capital of France is", max_tokens = 16)
   let res = completions(client, req)
   if (res.ok) {
       print("{res.response.choices[0].text}\n")
   }

The complete() Wrapper
======================

``complete`` is the one-liner: prompt in, text out (empty string on error):

.. code-block:: das

   let text = complete(client, "gpt-3.5-turbo-instruct", "Once upon a time")

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/11_completions.das <../../../../tutorials/dasOPENAI/11_completions.das>`

   Previous tutorial: :ref:`tutorial_dasOPENAI_moderations`
