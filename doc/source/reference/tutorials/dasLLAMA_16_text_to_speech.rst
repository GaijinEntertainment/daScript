.. _tutorial_dasLLAMA_text_to_speech:

==============================
dasLLAMA-16 — Text to Speech
==============================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Text to Speech
    single: Tutorial; Speech Synthesis

dasLLAMA speaks through one small surface: a loader that reads the family from
the GGUF, a ``caps()`` call that lists the voices, and two verbs —
``synthesize`` for the whole text at once, ``synthesize_stream`` for one
sentence at a time. The same program runs KittenTTS (kitten-nano and
kitten-mini) and Kokoro-82M.

All three are StyleTTS2-lineage models, and they share one assembly. The text
becomes phonemes, a text encoder predicts how long each phoneme lasts, a
prosody branch predicts pitch and energy, and a decoder plus an iSTFT
generator — inverse short-time Fourier transform, the step that turns
per-frame spectra back into samples — write the waveform. ``caps`` reports the
rate each model emits; the three above emit 24 kHz.

Run it with a TTS GGUF that has its two front-end packs beside it::

   daslang.exe -jit tutorials/dasLLAMA/16_text_to_speech.das -- kitten-nano.gguf hello.wav
   daslang.exe -jit ... -- kokoro-82m.gguf hello.wav --voice af_heart
   daslang.exe -jit ... -- kitten-nano.gguf hello.wav --f32

Use ``-jit``. Speech synthesis runs the same tuned kernels the language models
run, and the interpreter is far too slow for them.

One loader, the packs beside the model
======================================

``load_tts_model`` reads the family from the GGUF's own architecture metadata,
so no family name appears in the call. Two more files travel with the model:
``tts_g2p.bin`` and ``tts_postag.bin``, and the loader reads them from the
model's directory. A file that is not a TTS family panics and names the
architecture it found.

.. code-block:: das

   var m <- load_tts_model("kitten-nano.gguf")   // tts_g2p.bin + tts_postag.bin sit beside it

caps(): ask, don't assume
=========================

``caps`` answers what the loaded model serves: the voice names it accepts, the
language each one speaks, the rate of the PCM it hands back, and whether it can
clone a voice from a recording. Ask for a voice or a language ``caps`` does not
carry and the call panics — the request is never quietly swapped for something
the model does have.

``voices`` can come back **empty**. A model file may carry packs whose
languages this front end does not phonemize — Kokoro ships fifty-four in nine
languages, and ``caps`` lists the twenty-eight it can drive — so a build that
phonemizes none of them offers none, and there is no first voice to fall back
on. Check before you index.

.. code-block:: das

   let c <- caps(m)
   print("{length(c.voices)} voices, {c.sample_rate} Hz, clones a voice: {c.cloning}\n")
   if (empty(c.voices)) {
       print("this model has no voice the front end can drive\n")
       return
   }
   let voice = c.voices[0]

Speak it
========

``synthesize`` takes the text and hands back ``TtsAudio``: mono f32 PCM, the
sample rate, and the timings. ``write_wav_pcm16`` writes that PCM as a 16-bit
RIFF/WAVE file, which is what a player expects.

.. code-block:: das

   let audio <- synthesize(m, "daslang speaks.", voice)
   write_wav_pcm16("hello.wav", audio.pcm, audio.sample_rate)

What the model receives
=======================

The two packs beside the GGUF are the text front end, and they run before any
model weight is touched. ``tts_postag.bin`` carries a tokenizer and a
part-of-speech tagger; ``tts_g2p.bin`` carries the lexicons that turn words
into phonemes. Three passes run in order: normalization rewrites numbers,
money, dates, times and abbreviations into the words a reader says, the tagger
labels each token, and the grapheme-to-phoneme pass turns the labelled tokens
into the 45 US phoneme symbols the model was trained on. The tag is what
settles a heteronym — ``read`` as a verb and ``read`` as a past tense are the
same letters and different sounds.

``synthesize`` runs all three for you. Two facade calls run them alone, so you
can see what the model is asked to say. ``tts_normalize`` is the first pass,
and it needs no model — the normalizer is rules, not a pack.

.. code-block:: das

   print("{tts_normalize("Dr. Chen read 3.5 pages and paid $12.")}\n")
   // output: Doctor Chen read three point five pages and paid twelve dollars.

``tts_phonemize`` is the last pass, and it takes one already-normalized
sentence — the same piece ``synthesize`` hands the model. It answers in the
front end's own inventory, before any family rewrites those symbols into the
ones it was trained on, so two models with different symbol tables give you the
same string here.

.. code-block:: das

   let spoken = tts_normalize("Dr. Chen read 3.5 pages and paid $12.")
   print("{tts_phonemize(m, spoken)}\n")
   // output: dˈɑktəɹ ʧˈɛn ɹˈɛd θɹˈi pˈYnt fˈIv pˈAʤᵻz ænd pˈAd twˈɛlv dˈɑləɹz.

``read`` comes out ``ɹˈɛd``, the past tense, because the tagger called it one.

That two-argument form reads American English. A synthesis, though,
phonemizes in the dialect **the voice was trained on** — Kokoro's ``bf_emma``
reads ``en-gb`` where ``af_heart`` reads ``en-us`` — so a document about what
a particular voice will say has to ask in that voice's language, or it
describes a different sound. ``caps`` carries one language per voice in
``voice_langs``, beside ``voices``; ``tts_voice_lang`` answers for a family
alias too, and panics on a voice the model cannot drive with the message a
synthesis would give.

.. code-block:: das

   let lang = tts_voice_lang(m, voice)          // "en-us" | "en-gb"
   print("{voice} reads {lang}\n")
   print("{tts_phonemize(m, spoken, lang)}\n")  // the string that voice is asked to say

The front end is where ``synthesize`` spends its first microseconds, and
``TtsTimings`` counts them separately from the model stages.

Where the time went
===================

``TtsTimings`` is microseconds of wall clock per stage, model loading
excluded: the front end, then bert — a phoneme-level language model that gives
each phoneme the context of its neighbours — the text encoder, the durations,
the prosody branch, the decoder, the source and the generator. ``rtf`` is the
real-time factor, seconds of work per second of audio produced, so a number
below 1 is faster than real time. ``timings_line`` puts the whole split on one
line.

.. code-block:: das

   print("{timings_line(audio.timings, audio.sample_rate)}\n")
   print("rtf {rtf(audio.timings, audio.sample_rate)}\n")

One sentence at a time
======================

``synthesize_stream`` runs the same pieces and hands each one to the block as
soon as it exists, in order. A player can start on the first sentence while
the second is still being made, so the wait a listener feels is the first
chunk, not the whole text. Both forms walk the same chunk list, and each chunk
draws its source noise from its own index, so the streamed audio and the
buffered audio are the same samples.

.. code-block:: das

   synthesize_stream(m, "First sentence. Second sentence.", voice, 1.0) $(chunk) {
       print("{length(chunk.pcm)} samples at {chunk.sample_rate} Hz\n")
   }

The chunker
===========

One synthesis takes one sentence-sized piece, and ``tts_chunks`` is the
splitter ``synthesize`` runs over the normalized text. It cuts at sentence
ends and splits a sentence past ``TTS_CHUNK_CHARS`` codepoints on a word
boundary. Given the model, it follows that family's own driver: Kitten's
appends a comma to a piece the split left bare, so the model hears a prosodic
close; Kokoro's sends the text as it is, because its voices read an added mark
out loud as a breath. The cap counts codepoints, not bytes — an em dash costs
three bytes and one character. A period inside ``Dr.`` or ``3.5`` is not a
sentence end.

.. code-block:: das

   for (piece in tts_chunks(m, "Dr. Smith read it. It ran to 3.5 pages.")) {
       print("{piece}\n")
   }
   // -> "Dr. Smith read it."
   // -> "It ran to 3.5 pages."

Speed and voice
===============

``speed`` scales the predicted durations, so the same words come out in less
time at the same pitch. The voice is a name from ``caps().voices``; each one is
a style vector the model was trained with, and switching costs nothing but the
lookup. A KittenTTS voice carries its own speed prior on top of the number you
pass, so the same ``speed`` on two voices is not the same tempo — hold the
voice fixed when you want to read the knob.

.. code-block:: das

   let quick <- synthesize(m, "daslang speaks.", voice, 1.25)
   let other <- synthesize(m, "daslang speaks.", c.voices[1])

The two weight lanes
====================

The decoder and generator matrix multiplies are served from one of two
prepared images beside the GGUF. The q8 lane holds those weights as Q8_0
quants and is what a load serves by default. The f32 lane holds the file's own
planes; it is the reference the parity tests hold the q8 lane against.

``tts_serves_q8`` answers which lane the next load takes. ``set_tts_q8`` pins
it, and ``reset_tts_q8`` returns to the default. The pin is process-wide
state, so it must not outlive the load it was set for: a stale pin changes the
lane of the next model this process loads. Pin through ``defer`` and every path
out resets it, a panic included.

.. code-block:: das

   defer() {
       reset_tts_q8()
   }
   set_tts_q8(false)                 // the f32 reference lane, for this load only
   print("next load serves q8: {tts_serves_q8()}\n")
   var reference <- load_tts_model("kitten-nano.gguf")

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/16_text_to_speech.das <../../../../tutorials/dasLLAMA/16_text_to_speech.das>`

   Previous tutorial: :ref:`tutorial_dasLLAMA_prefix_cache`

   The other direction, speech to text: :ref:`tutorial_dasLLAMA_speech_to_text`

   Serving speech over HTTP: :ref:`tutorial_dasLLAMA_serving`

   The speech CLI: ``utils/dasllama-server/txt2wav.das``; a client for the
   server's ``/v1/audio/speech`` route: ``examples/dasLLAMA/speak_server.das``
