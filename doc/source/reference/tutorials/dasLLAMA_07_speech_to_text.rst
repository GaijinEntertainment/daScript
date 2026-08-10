.. _tutorial_dasLLAMA_speech_to_text:

==============================
dasLLAMA-07 — Speech to Text
==============================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Speech Recognition
    single: Tutorial; Transcription

dasLLAMA transcribes speech through one uniform surface: a single loader that
sniffs the file format, one verb set, and a ``caps()`` call that tells you what
the loaded model honestly supports. No family names appear in the API — the
same program runs a whisper.cpp bin, a Qwen3-ASR GGUF pair, or a Parakeet-TDT
bin.

Run it with any supported model and a 16 kHz mono PCM16 WAV::

   daslang.exe -jit tutorials/dasLLAMA/07_speech_to_text.das -- ggml-tiny.bin jfk.wav
   daslang.exe -jit ... -- Qwen3-ASR-0.6B-Q8_0.gguf mmproj-Qwen3-ASR-0.6B-bf16.gguf jfk.wav
   daslang.exe -jit ... -- ggml-parakeet-tdt-0.6b-v2-f32.bin jfk.wav

One loader, sniffed formats
===========================

``load_asr_model`` looks at the file, not the filename: a ggml bin routes to
whisper or Parakeet by its vocabulary size; a GGUF decoder takes its
audio-encoder mmproj as a second path. A mismatched or unsupported file panics
with a message that says what to do instead.

.. code-block:: das

   var m <- load_asr_model("ggml-tiny.bin")                    // whisper family
   var q <- load_asr_model("Qwen3-ASR-0.6B-Q8_0.gguf",
                           "mmproj-Qwen3-ASR-0.6B-bf16.gguf")  // GGUF pair

caps(): ask, don't assume
=========================

Capabilities are advisory — they let a front end grey out what a model can't
do. Invalid requests still panic loudly at the call site; nothing is silently
ignored.

.. code-block:: das

   let c <- caps(m)
   // c.languages  — codes create_session accepts (empty = the model detects itself)
   // c.translate  — speech-to-English translation task
   // c.timestamps — none / segment / word granularity
   // c.streaming  — native incremental decode
   // c.prompt     — context/prompt conditioning (Qwen3-ASR: AsrSession.context)

Transcribe
==========

The one-shot form returns the full text; the block form yields each
``TranscribeSegment`` as its window completes — with centisecond timestamps,
the raw token ids, and ``avg_logprob``, the mean per-token log-probability
(closer to zero = more confident).

.. das-doc: given var samples : array<float>
.. code-block:: das

   var s <- create_session(m, "auto")   // "auto": whisper detects the language
   let text = transcribe(m, s, samples) // one-shot
   print("{s.detected_lang}: {text}\n")

   transcribe(m, s, samples) $(seg) {   // per-segment
       print("[{seg.t0} - {seg.t1}] {seg.text} (avg_logprob {seg.avg_logprob})\n")
   }

Language auto-detection costs whisper one extra prompt decode on the first
audio, then sticks for the session. Qwen3-ASR always detects — it reports the
spoken language itself, surfaced through the same ``detected_lang`` field.

The push-chunks rail
====================

``feed`` buffers 16 kHz samples, ``drain`` transcribes every *complete* 30 s
window (leaving the rest pending), and ``flush`` finishes the tail. This is
the shape a live audio source drives — pair it with dasAudio's microphone
capture (:ref:`tutorial_dasAudio_recording`). Models that transcribe whole
clips at once say so with a loud panic, matching ``caps().streaming``.

.. das-doc: given var chunk : array<float>
.. code-block:: das

   feed(m, s, chunk)             // as audio arrives
   drain(m, s) $(seg) {          // complete windows only
       print("{seg.text}")
   }
   flush(m, s) $(seg) {          // the sub-30 s tail, at end of stream
       print("{seg.text}")
   }

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/07_speech_to_text.das <../../../../tutorials/dasLLAMA/07_speech_to_text.das>`

   Previous tutorial: :ref:`tutorial_dasLLAMA_add_an_arch` · Next tutorial: :ref:`tutorial_dasLLAMA_audio_chat`

   The transcription CLI: ``examples/dasLLAMA/transcribe.das``
