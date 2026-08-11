.. _tutorial_dasLLAMA_voice_activity:

======================================
dasLLAMA-12 — Voice Activity Detection
======================================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Voice Activity
    single: Tutorial; Silero VAD

Voice-activity detection answers one question about audio: *where is the
speech?* dasLLAMA ships Silero-VAD with its weights checked in beside the
module, and a four-second public-domain speech fixture next to them — so this
is the one dasLLAMA tutorial that runs with no downloads and no arguments::

   daslang.exe -jit tutorials/dasLLAMA/12_voice_activity.das

Pass your own 16 kHz mono PCM16 WAV to analyze it instead; pass an ASR model
(as in :ref:`tutorial 07 <tutorial_dasLLAMA_speech_to_text>`) and the last
section transcribes what the gate lets through.

A trained gate, not an energy gate
==================================

``vad_probs`` runs the model over 512-sample chunks — 32 ms at 16 kHz — and
returns one speech probability per chunk:

.. das-doc: given var m = VadModel()
.. das-doc: given var speech : array<float>
.. code-block:: das

   let probs <- vad_probs(m, speech)

The tutorial feeds it three loud fakes — white noise, a pure tone, and a
formant-shaped fake vowel — plus real speech. An energy detector fires on all
three fakes. Silero pins noise and tone near zero; the fake vowel, built to
mimic speech's structure, brushes the 0.5 threshold; real speech pins the
ceiling::

   peak speech probability: fake vowel 0.62, noise 0.01, tone 0.004
   peak speech probability: real speech 0.9997

The gate reads structure, not volume. One practical note: the model is
stateful — it judges each chunk in the context of what came before, so
synthetic tests give every span some leading silence.

Segments: where the speech is
=============================

``find_speech_timestamps`` turns the probability curve into ``[start, end)``
sample regions with silero's hysteresis: a segment opens at probability ≥
``threshold`` (0.5), closes only after it stays below ``neg_threshold``
(0.35) for ``min_silence`` (100 ms), segments shorter than ``min_speech``
(250 ms) are dropped, and boundaries get ``speech_pad`` (30 ms) of margin.
``default_vad_opts()`` is that tuning; override fields for a different trade:

.. code-block:: das

   var segs <- find_speech_timestamps(m, speech, default_vad_opts())
   for (g in segs) {
       print("{float(g.start) / 16000.0} - {float(g.end) / 16000.0} s\n")
   }
   // -> 0.322 - 2.27 s
   //    3.266 - 4 s

Streaming: the live-microphone shape
====================================

A live capture cannot wait for the whole recording. ``VadIter`` is the online
segmenter: feed audio in whatever chunk sizes arrive, and it emits a
``speech_start`` / ``speech_end`` event the moment each boundary is crossed;
leftover samples buffer until the next call. Same hysteresis as above:

.. code-block:: das

   var it = VadIter()
   vad_iter_reset(it, default_vad_opts())
   vad_iter_feed(m, it, speech) $(ev) {
       print("{ev.kind} at {float(ev.sample) / 16000.0} s\n")
   }

The pipeline payoff
===================

VAD in front of a speech-to-text model is the standard rig: long silences
never reach the transcriber, and each detected segment becomes its own
``transcribe`` call. The tutorial's last section does exactly that when an
ASR model is on the command line, and skips politely when one is not —
:ref:`tutorial 07 <tutorial_dasLLAMA_speech_to_text>` covers the ASR surface
itself.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/12_voice_activity.das <../../../../tutorials/dasLLAMA/12_voice_activity.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_serving`

   Speech to text: :ref:`tutorial_dasLLAMA_speech_to_text`
