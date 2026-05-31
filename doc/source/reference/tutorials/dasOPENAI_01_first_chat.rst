.. _tutorial_dasOPENAI_first_chat:

===========================================
OPENAI-01 — Your First Chat
===========================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Chat
    single: Tutorial; LLM

This tutorial introduces the ``openai`` module — a pure-daslang client for
OpenAI-compatible APIs (OpenAI, Ollama, OpenRouter, Kokoro, and any server that
speaks the same REST surface). It builds on ``dashv`` for HTTP and
``daslib/json_boost`` for serialization.

The companion ``.das`` files are in ``tutorials/dasOPENAI/``. They run against a
small local mock server (``tutorial_openai_server.das``) so no live LLM is
needed.

Setup
=====

Require the section module you need and set ``options rtti``:

.. code-block:: das

   options gen2
   options rtti          // REQUIRED — see below

   require openai/openai_chat

``options rtti`` is mandatory on every dasOPENAI script. Without it the
``@optional`` / ``@rename`` field annotations are ignored, and requests
serialize *every* field (e.g. ``"n":0``) — which real servers reject.

Constructing a client
======================

``openai_client`` takes a base URL (no trailing slash) and an optional API key:

.. code-block:: das

   // local server, no key
   let client = openai_client("http://localhost:11434/v1")

   // real OpenAI, key from the environment
   let client = openai_client("https://api.openai.com/v1", get_env("OPENAI_API_KEY"))

The chat_text one-liner
=======================

``chat_text`` is the simplest entry point: one user turn in, the assistant's
text out (empty string on error).

.. code-block:: das

   let reply = chat_text(client, "gpt-4o-mini", "Say hello in one short sentence.")
   print("{reply}\n")

The full chat() call
====================

``chat`` takes a ``ChatCompletionRequest`` and returns a ``ChatResult``. When
``ok`` is true, ``response`` holds the decoded reply — choices, the assistant
message, and token usage:

.. code-block:: das

   let req = ChatCompletionRequest(model = "gpt-4o-mini",
       messages <- [ChatMessage(role = "user", content = "What is 2+2?")])
   let res = chat(client, req)
   if (res.ok) {
       print("answer: {res.response.choices[0].message.content}\n")
       print("tokens: {res.response.usage.total_tokens}\n")
   } else {
       print("error [{res.error.kind}/{res.error.status}]: {res.error.message}\n")
   }

Handling errors
===============

Every call returns a unified ``OpenAIError`` rather than crashing. ``kind`` is
one of ``"transport"`` (could not reach the server), ``"http"``, ``"api"`` (the
server returned an error body), or ``"decode"`` (the response did not parse).

Quick Reference
===============

==========================================  ==============================================
Function                                    Description
==========================================  ==============================================
``openai_client(base_url [, api_key])``     Build a client for any OpenAI-compatible server
``chat_text(client, model, prompt)``        One user turn → assistant text ("" on error)
``chat(client, req)``                       Full request → ``ChatResult`` (ok/response/error)
``res.response.choices[0].message``         The assistant ``ChatMessage``
``res.response.usage.total_tokens``         Token accounting
``res.error.kind`` / ``.status``            Error category and HTTP status
==========================================  ==============================================

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/01_first_chat.das <../../../../tutorials/dasOPENAI/01_first_chat.das>`

   Next tutorial: :ref:`tutorial_dasOPENAI_conversations`
