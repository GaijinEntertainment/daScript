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
Gemma-4 E-series audio are served too, but not by ``load_audio_tower``:
:ref:`tutorial 07 <tutorial_dasLLAMA_speech_to_text>`'s two-path
``load_asr_model`` transcribes them, and a Gemma-4 E-series pair chats on this
page through the ``AudioEmbedder`` carrier rail at the end.

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

The rows themselves come from the ``AudioEmbedder`` carrier — the
family-neutral encoder a scheduler owns. It serves every audio mmproj on this
page: the ``load_audio_tower`` families and the gemma-4 E-series alike. Probe
the mmproj with ``audio_probe_proj_dim`` (0 means no audio family serves the
file), load it with ``load_audio_embedder``, and ``encode_audio`` turns 16 kHz
PCM into the soft-token rows that splice between the two spans —
``encode_image``'s audio twin, and exactly what the server's media worker does
per clip. The tutorial asks a second question first:
``audio_tower_probe_proj_dim`` answers non-zero for a file ``load_audio_tower``
serves. Such a pair runs every section, the carrier ones last. A gemma-4
E-series file answers 0 there, so it runs only the carrier rail —
``section_render_spans`` plus ``section_carrier_encode``.

The carrier rail closes the loop at the chat layer with the pre-encoded-rows
seam, ``add_user_image_rows``'s audio twin: ``add_user_audio_rows`` moves the
encoder's rows onto a *plain* chat — no tower attached — and ``respond`` runs
the spliced turn, the audio span rendered around the rows. That is how a
gemma-4 E-series pair hears in a conversation at all, and the path for a
scheduler that owns its own encoder; the rows are ``dim``-wide on every family
and the call length-checks them. ``audio_span_bare(e)`` says whether the span
takes markers: an ultravox span sits bare in a stock Llama template, which
knows no audio marker, so pass it as ``bare``:

.. das-doc: given var rows : array<float>; let n = 0l; var e = AudioEmbedder()
.. code-block:: das

   var chat <- create_chat(m, "", 96l)
   add_user_audio_rows(m, chat, rows, n, audio_span_bare(e))   // moves the rows in
   add_user(chat, "What did you hear?")
   respond(m, chat, SamplingParams()) $(piece) {
       print("{piece}")
       return true
   }

The audio in its place: one body by hand
========================================

So far the audio led the turn. A user message often carries its media in the
middle: "Here is a recording. <audio> What is being said?". ``add_user_span``
puts the audio where it sits: ``text_before`` bytes into the turn's text. The
span carries the media's content key — the same clip gives the same key.
``render_turn`` then writes ``n_rows`` *media position ids* where the rows go.
They are negative numbers, which no vocab id uses, so a prefix cache matches
across the span the same way it matches across text. ``render_turn_audio``
and ``render_turn_image`` take the same ``text_before`` for the two-span shape.

A program that owns its encoder and scheduler prefills that turn as *one body*
instead of three evals (text, rows, text). ``media_body_rows`` builds the
body's rows — the head text, the media rows, the tail text — and
``eval_embd_body`` prefills them. The span ``(0, 0)`` is empty, so every row
stays causal, as audio wants; an image passes its non-causal span and its
grid. On a gemma-4 E-series decoder the body does one more thing: each text
row's token id rides on the session, so text rows get their own per-layer
input, and ``eval_embd_body`` spends the ids:

.. das-doc: given let bare = false; let key = 0ul
.. code-block:: das

   let lead = "Here is a recording. "
   var rchat <- create_chat_renderer(m, "", 64l)
   add_user(rchat, "{lead}What is being said?")
   add_user_span(m, rchat, key, n, length(lead), false, bare)   // audio, not image
   var turn <- render_turn(m, rchat)
   var lo = 0l
   while (!is_media_position(turn[lo])) {
       lo ++
   }
   var head <- [for (k in range64(lo)); turn[k]]
   var tail <- [for (k in range64(lo + n, long_length(turn))); turn[k]]
   var s <- create_session(m)
   var body : array<float>
   media_body_rows(m, s, head, rows, n, m.config.dim, tail, body)
   eval_embd_body(m, s, body, long_length(turn), 0l, 0l, int2(0))
   // sample(s, ...) now answers the turn

On Llama-3.2-1B with the ultravox mmproj and the JFK clip the turn renders as
``36 text tokens | 187 media ids | 13 text tokens``; on gemma-4 E2B it is
``17 | 100 | 14``. Both decoders then describe the clip from the one body.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/08_audio_chat.das <../../../../tutorials/dasLLAMA/08_audio_chat.das>`

   Previous tutorial: :ref:`tutorial_dasLLAMA_speech_to_text` · Next tutorial: :ref:`tutorial_dasLLAMA_embeddings`

   The audio-chat CLI: ``examples/dasLLAMA/audio_chat.das``
