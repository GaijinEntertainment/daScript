.. _tutorial_dasOPENAI_image_generation:

=====================================
OPENAI-09 — Image Generation
=====================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Image Generation

This tutorial covers generating images from a text prompt. As with every
dasOPENAI script, the consumer root needs ``options rtti``.

Building the Request
====================

Only ``prompt`` is required on ``ImageRequest``; the rest are optional and are
omitted from the wire payload when unset (``options rtti`` honors ``@optional``):

.. code-block:: das

   require openai/openai_images

   let req = ImageRequest(prompt = "a serene mountain lake at dawn",
       size = "1024x1024", n = 1, response_format = "url")
   let res = generate_image(client, req)

Reading the Result
==================

``generate_image`` returns an ``ImageResult``; when ``ok`` is true,
``response.data`` holds the generated images. With ``response_format`` ``"url"``
the bytes live at ``url``; with ``"b64_json"`` they arrive base64-encoded in
``b64_json``. ``revised_prompt`` is the prompt the model actually used, if it
rewrote yours:

.. code-block:: das

   if (res.ok) {
       for (img in res.response.data) {
           print("url:            {img.url}\n")
           print("revised prompt: {img.revised_prompt}\n")
       }
   } else {
       print("error [{res.error.kind}/{res.error.status}]: {res.error.message}\n")
   }

.. note::

   The bundled mock returns a canned URL so the tutorial runs offline. To
   generate real images, point ``base_url`` at a backend whose
   ``/images/generations`` endpoint is OpenAI-compatible (OpenAI, OpenRouter).

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/09_image_generation.das <../../../../tutorials/dasOPENAI/09_image_generation.das>`

   Previous tutorial: :ref:`tutorial_dasOPENAI_vision`

   Next tutorial: :ref:`tutorial_dasOPENAI_moderations`
