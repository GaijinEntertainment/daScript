.. _tutorial_dasLLAMA_vision_chat:

=========================
dasLLAMA-14 — Vision Chat
=========================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Vision
    single: Tutorial; Multimodal

Tutorial 08 gave a conversation ears. This one gives it eyes. A vision chat
model pairs a normal text decoder with an image encoder: the encoder turns
pixels into **soft tokens** — embedding rows the decoder reads inline with
text, one row per group of image patches. The supported families are gemma-4
dense (the gemma4uv embedder), gemma-4 E-series (the gemma4v ViT tower), and
gemma-3 (the gemma3v SigLIP tower),
each shipped as a decoder GGUF plus its vision multimodal projector (mmproj)
GGUF; the loader sniffs which family an mmproj is.

Run it with a decoder, its mmproj, and any image stbimage decodes::

   daslang.exe -jit tutorials/dasLLAMA/14_vision_chat.das -- decoder.gguf mmproj.gguf photo.jpg "What is in this picture?"

A conversation that can see
===========================

The easy path is four calls. ``load_image_rgb`` decodes the file to an RGB8
``VisionImage``. ``load_vision_embedder`` reads the mmproj — the vision half
of the model. ``create_chat(model, embedder)`` moves the embedder into the
chat, and ``add_user_image`` queues the picture for the next turn: geometry,
letterbox and the encoder run right there, so ``respond`` only prefills and
generates. One image per turn; text via ``add_user`` rides along:

.. das-doc: given var m = Model()
.. das-doc: given let image_path = "photo.jpg"
.. das-doc: given let mmproj_path = "mmproj.gguf"
.. das-doc: given let prompt = "Describe this image in one sentence."
.. das-doc: given var s = Session()
.. das-doc: given var rows : array<float>
.. das-doc: given let np = 0l
.. das-doc: given let n_head = 0l
.. code-block:: das

   var inscope img <- load_image_rgb(image_path)
   var emb <- load_vision_embedder(mmproj_path)
   var chat <- create_chat(m, emb, "", 96l)
   add_user_image(chat, img)
   add_user(chat, "Describe this image in one sentence.")
   respond(m, chat, SamplingParams()) $(piece) {
       print("{piece}")
       return true
   }

Soft tokens by hand
===================

Now the same turn built ourselves — the shape a scheduler uses when the
encoder runs on a worker and the decoder session lives elsewhere.
``encode_image`` is the whole image path in one call: geometry, letterbox,
normalize, encode. It appends ``npos`` rows of ``model.config.dim`` floats
and returns ``npos``. ``render_turn_image`` renders the turn's *tokens* as
two spans — ``head`` before the image rows, ``tail`` after — split exactly at
the template's image marker, so tokenizer merges never cross the picture:

.. code-block:: das

   var scratch = VisionState()
   var img_rows : array<float>
   let n_img = encode_image(emb, scratch, img, "tag", img_rows)

   var chat = create_chat_renderer(m, "", 96l)
   add_user(chat, prompt)
   var head : array<int64>
   var tail : array<int64>
   render_turn_image(m, chat, head, tail)

The prompt we prefill is head rows + image rows + tail rows, one array —
prefilling tokens *is* prefilling their embedding rows (tutorial 08 proves
that identity), so ``embed_text_rows`` fills the text parts. Then
``eval_embd_span`` runs the whole thing with one twist: the rows in
``[span_lo, span_hi)`` are **non-causal** — every query in the span attends
the whole span, because a picture has no left-to-right order. The text around
it stays causal:

.. code-block:: das

   eval_embd_span(m, s, rows, np, n_head, n_head + n_img)
   // ...then sample() one token at a time, like any session

``eval_embd`` is the same call without a span — every row causal. Audio rows
keep their time order, so the audio family prefills through it; use it
whenever you already hold the embedding rows and no image span is in play.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/14_vision_chat.das <../../../../tutorials/dasLLAMA/14_vision_chat.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_prefix_cache`

   The audio twin of this tutorial: :ref:`tutorial_dasLLAMA_audio_chat`
