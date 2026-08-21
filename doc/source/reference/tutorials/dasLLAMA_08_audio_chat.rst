.. _tutorial_dasLLAMA_audio_chat:

==========================
dasLLAMA-08 — Audio Chat
==========================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Audio Chat
    single: Tutorial; Multimodal

Audio-capable chat models pair a normal text decoder with a whisper-family
audio encoder — the *tower* — that turns 16 kHz PCM into soft tokens the
decoder reads inline with text. Supported pairs (decoder + mmproj GGUF):
Qwen2-Audio, Qwen2.5-Omni (audio side), Ultravox v0.5 (over *stock* Llama-3
decoders), and Voxtral-Mini. The chat template picks the audio framing
automatically — the code below is identical for every pair. Qwen3-Omni and
Gemma-4 E-series audio are served too, but through the ASR surface
(:ref:`tutorial 07 <tutorial_dasLLAMA_speech_to_text>`'s two-path
``load_asr_model``), not ``load_audio_tower``.

Run::

   daslang.exe -jit tutorials/dasLLAMA/08_audio_chat.das -- decoder.gguf mmproj.gguf clip.wav

A chat that hears
=================

``create_chat(model, tower)`` moves the tower into the chat (which owns the
encoder scratch too — one chat, one hearing apparatus). ``add_user_audio``
encodes the clip to soft tokens immediately and queues them at the head of the
next turn; ``add_user`` contributes the turn's text after the audio span.
``respond`` renders the turn — template framing, audio splice, embedding
prefill — and streams the reply.

.. das-doc: given var samples : array<float>
.. code-block:: das

   var m <- load_model("Llama-3.2-1B-Instruct-Q8_0.gguf", QuantMode.q8)
   var tower <- load_audio_tower("mmproj-ultravox-1b-f32.gguf")

   var chat <- create_chat(m, tower)       // panics on a mismatched pair
   add_user_audio(chat, samples)           // 16 kHz mono f32 PCM
   add_user(chat, "What is being said in this audio?")
   respond(m, chat, SamplingParams()) $(piece) {
       print("{piece}")
       return true
   }

Follow-up turns work like any text chat — the audio turn is in the KV cache,
so the model remembers what it heard. A later ``add_user_audio`` call brings a
second clip into a new turn.

What the template does with audio
=================================

Each model family frames the audio span its own way, and the chat layer wires
it from the model's template: the qwen2 family wraps the soft tokens in
``<|audio_bos|>`` / ``<|audio_eos|>``, Llama-3-based Ultravox splices bare
embeddings, Voxtral opens the span with ``[BEGIN_AUDIO]``. Text runs break at
the audio boundary, so tokenizer merges never cross it — the rendered turn is
token-identical to llama.cpp's ``mtmd`` reference for every family.

caps() for chat models
======================

``caps(model)`` returns ``LlmCaps`` — the chat layer's honesty contract. Its
first entry exists because gemma has no system role: the chat layer folds the
system prompt into the first user turn, and ``system_prompt == false`` is how
a program finds out instead of being silently absorbed.

.. code-block:: das

   if (!caps(m).system_prompt) {
       print("note: this model has no system role — the prompt is folded into turn 1\n")
   }

Under the hood: the splice
==========================

``generate`` and ``generate_embd`` meet at the embedding rows: prefilling
tokens *is* prefilling their embedding rows. The tutorial proves it by
identity — same prompt, both roads, greedy, byte-identical output:

.. code-block:: das

   let prompt <- encode(m, "The capital of France is")
   var embd : array<float>
   embd |> resize(long_length(prompt) * m.config.dim)
   embed_text_rows(m, prompt, embd, 0l)   // the rows generate() would prefill
   var s2 = create_session(m)
   generate_embd(m, s2, embd, long_length(prompt), SamplingParams(), 8l) $(_id, piece) {
       print(piece)
       return true
   }
   // -> " Paris. ..." — byte-identical to generate() on the same prompt

An audio turn is this exact array with a span of rows replaced by the
tower's soft tokens. ``add_user_audio`` builds it for you; a custom modality
builds it by hand and hands it to ``generate_embd``. A scheduler serving many
streams builds it by hand too: ``render_turn_audio`` renders the turn's tokens
as two spans — head before the audio rows, tail after, split exactly at the
template's audio marker (``render_turn_image``'s audio twin) — and splices the
encoder's rows between them. Unlike an image span, audio rows stay *causal*:
sound has a left-to-right order.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/08_audio_chat.das <../../../../tutorials/dasLLAMA/08_audio_chat.das>`

   Previous tutorial: :ref:`tutorial_dasLLAMA_speech_to_text` · Next tutorial: :ref:`tutorial_dasLLAMA_embeddings`

   The audio-chat CLI: ``examples/dasLLAMA/audio_chat.das``
