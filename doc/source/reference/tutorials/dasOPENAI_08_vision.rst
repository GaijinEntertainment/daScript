.. _tutorial_dasOPENAI_vision:

=====================================
OPENAI-08 — Vision (Image Input)
=====================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Vision
    single: Tutorial; Multimodal

This tutorial covers multimodal chat — sending an image alongside text. As with
every dasOPENAI script, the consumer root needs ``options rtti``.

chat_vision
===========

``chat_vision`` sends one user turn (text + a single image) and returns the
normal ``ChatResult`` — the assistant's text is in
``choices[0].message.content``. The image can be an ``http(s)`` URL or a
``data:`` URL (base64-inlined bytes):

.. code-block:: das

   require openai/openai_vision

   let client = openai_client(base_url)
   let res = chat_vision(client, "gpt-4o", "What is in this image?",
       "https://example.com/cat.png", [max_tokens = 100])
   if (res.ok) {
       print("{res.response.choices[0].message.content}\n")
   }

The Multimodal Request Shape
============================

A vision message can't use the plain ``content : string`` shape — its content is
an **array of parts** (a text part and an ``image_url`` part). ``vision_request_body``
builds that JSON, so you can see the wire format or post it yourself:

.. code-block:: das

   let body = vision_request_body("gpt-4o", "Describe this", "https://example.com/cat.png")
   // {
   //   "model": "gpt-4o",
   //   "messages": [{ "role": "user", "content": [
   //       { "type": "text", "text": "Describe this" },
   //       { "type": "image_url", "image_url": { "url": "https://example.com/cat.png" } }
   //   ]}],
   //   "max_tokens": 300
   // }

.. note::

   To run against a real vision backend, point ``base_url`` at it. Ollama with a
   vision model (``ollama run llava``) serves the same ``/chat/completions``
   shape, so the only change is the URL.

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/08_vision.das <../../../../tutorials/dasOPENAI/08_vision.das>`

   Previous tutorial: :ref:`tutorial_dasOPENAI_streaming`

   Next tutorial: :ref:`tutorial_dasOPENAI_image_generation`
