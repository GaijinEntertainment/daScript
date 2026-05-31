.. _tutorial_dasOPENAI_structured_outputs:

===========================================
OPENAI-03 — Structured Outputs (JSON mode)
===========================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; JSON mode
    single: Tutorial; Structured Outputs

JSON mode asks the server to emit valid JSON as the message content. You then
decode that content string into your own struct — using the same ``sscan_json``
machinery the client uses internally.

Requesting JSON
===============

``json_object_format()`` returns the ``response_format`` value for JSON mode.
Assign it to the request:

.. code-block:: das

   var req = ChatCompletionRequest(model = "gpt-4o-mini",
       messages <- [ChatMessage(role = "user",
           content = "Return the answer to 6*7 as JSON: {\"answer\": N}.")])
   req.response_format = json_object_format()
   let res = chat(client, req)

Decoding the content
====================

Declare a struct matching the JSON you expect, then decode the assistant's
content into it. With ``options rtti``, ``sscan_json`` maps JSON keys onto the
struct fields:

.. code-block:: das

   struct Answer {
       answer : int
   }

   let content = res.response.choices[0].message.content
   var parsed : Answer
   if (sscan_json(content, parsed)) {
       print("decoded answer = {parsed.answer}\n")
   }

This is the bridge from "model produced text" to "typed daslang value" — the
model returns a JSON string, and ``sscan_json`` turns it into a struct you can
use directly.

Quick Reference
===============

==========================================  ==============================================
Function / value                            Description
==========================================  ==============================================
``json_object_format()``                    The ``response_format`` value for JSON mode
``req.response_format = …``                  Enable JSON mode on a request
``sscan_json(content, parsed)``              Decode a JSON string into a struct (needs rtti)
==========================================  ==============================================

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/03_structured_outputs.das <../../../../tutorials/dasOPENAI/03_structured_outputs.das>`

   For general JSON handling see :ref:`tutorial_json`.

   Next tutorial: :ref:`tutorial_dasOPENAI_tools`
