.. _tutorial_dasOPENAI_conversations:

===========================================
OPENAI-02 — Conversations and Parameters
===========================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Conversation
    single: Tutorial; Chat Parameters

This tutorial covers multi-turn conversations, the optional request
parameters, and reading token usage.

Multi-turn conversations
=========================

A chat request carries the whole conversation as an ordered array of messages.
The model sees every prior turn. Roles are ``system`` (instructions),
``user``, ``assistant`` (previous model replies), and ``tool`` (function
results — see :ref:`tutorial_dasOPENAI_tools`):

.. code-block:: das

   let req = ChatCompletionRequest(model = "gpt-4o-mini", messages <- [
       ChatMessage(role = "system", content = "You are a terse assistant."),
       ChatMessage(role = "user", content = "Remember the number 7."),
       ChatMessage(role = "assistant", content = "Got it — 7."),
       ChatMessage(role = "user", content = "What number did I ask you to remember?")])
   let res = chat(client, req)

Optional parameters
===================

``ChatCompletionRequest`` exposes the common knobs as ``@optional`` fields.
Because ``options rtti`` is set, default-valued optionals are omitted from the
wire — only the ones you set are sent. Set them with named arguments
(use ``<-`` for the array-valued ``stop``):

.. code-block:: das

   let req = ChatCompletionRequest(model = "gpt-4o-mini",
       messages <- [ChatMessage(role = "user", content = "Write one word.")],
       temperature = 0.2,
       max_tokens = 16,
       seed = 42,
       stop <- ["\n\n"])

Other optionals include ``top_p``, ``n``, ``max_completion_tokens``,
``frequency_penalty``, ``presence_penalty``, ``user``, and ``tool_choice``.

Token usage
===========

Most responses include a ``Usage`` breakdown — prompt, completion, and total
tokens — useful for cost tracking and staying within context limits:

.. code-block:: das

   let u = res.response.usage
   print("prompt={u.prompt_tokens} completion={u.completion_tokens} total={u.total_tokens}\n")

Quick Reference
===============

==========================================  ==============================================
Field / value                               Meaning
==========================================  ==============================================
``messages`` (array of ``ChatMessage``)     The full conversation, in order
``role``                                     ``system`` / ``user`` / ``assistant`` / ``tool``
``temperature``, ``top_p``                   Sampling controls
``max_tokens``, ``seed``, ``stop``           Length cap, determinism, stop sequences
``response.usage``                           Prompt / completion / total token counts
==========================================  ==============================================

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/02_conversations_and_params.das <../../../../tutorials/dasOPENAI/02_conversations_and_params.das>`

   Next tutorial: :ref:`tutorial_dasOPENAI_structured_outputs`
