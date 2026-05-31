.. _tutorial_dasOPENAI_streaming:

===========================================
OPENAI-07 — Streaming Chat
===========================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Streaming
    single: Tutorial; SSE

Streaming delivers the assistant's reply token-by-token over Server-Sent Events
(SSE) instead of waiting for the whole response. ``chat_stream`` invokes your
``on_delta`` block for each text increment and also accumulates the full text.

Streaming with on_delta
=======================

``chat_stream`` sets ``stream = true`` for you. Each text increment is passed to
the trailing block; the return value carries the full accumulated content and
the ``finish_reason``:

.. code-block:: das

   var req = ChatCompletionRequest(model = "gpt-4o-mini",
       messages <- [ChatMessage(role = "user", content = "Stream a short greeting.")])

   let result = chat_stream(client, req) $(delta : string) {
       print(delta)          // each increment, as it arrives
   }

   if (result.ok) {
       print("\naccumulated: {result.content}\n")
       print("finish_reason: {result.finish_reason}\n")
   }

How it works
============

Under the hood ``chat_stream`` reads the SSE response with ``request_cb``,
splits it into ``data:`` lines, decodes each ``ChatCompletionChunk``, appends
``choices[0].delta.content``, and stops at ``data: [DONE]``. It tolerates CRLF
line endings and accumulates increments efficiently (joining once at the end).

Scope
=====

``chat_stream`` accumulates text deltas (``choices[0].delta.content``) only.
Streamed tool/function calls are a separate, later feature — for tool calls use
the non-streaming :ref:`chat() <tutorial_dasOPENAI_tools>`.

Quick Reference
===============

.. list-table::
   :header-rows: 1
   :widths: 45 55

   * - Function / value
     - Description
   * - ``chat_stream(client, req) $(delta) { … }``
     - Stream a reply; block runs per increment
   * - ``result.content``
     - Full accumulated assistant text
   * - ``result.finish_reason``
     - Why generation stopped (e.g. ``"stop"``)
   * - ``result.ok`` / ``result.error``
     - Success flag / unified error

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/07_streaming_chat.das <../../../../tutorials/dasOPENAI/07_streaming_chat.das>`

   For the SSE protocol and ``request_cb`` see :ref:`tutorial_dasHV_sse_and_streaming`.
