.. _tutorial_dasOPENAI_audio:

==================================================
OPENAI-06 — Audio (Speech and Transcription)
==================================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Text to Speech
    single: Tutorial; Transcription

This tutorial covers text-to-speech (TTS) and speech-to-text (STT).

Text to speech
==============

``speak`` synthesizes text into audio and returns the raw bytes
(``array<uint8>``, empty on error). Write them to a file to play later, or
stream them onward — the byte format is whatever the server produced:

.. code-block:: das

   require openai/openai_audio
   require daslib/fio

   var audio <- speak(client, "tts-1", "alloy", "Hello from daslang.")
   print("received {length(audio)} audio bytes\n")
   fopen("out.wav", "wb") $(f) {
       fwrite(f, audio)
   }
   delete audio

For the full request (response format, speed) use ``speech(client, req)`` with a
``SpeechRequest``.

Speech to text
==============

``transcribe`` uploads an audio file (multipart) and returns the recognized
text. ``translate`` is the same but returns English:

.. code-block:: das

   let res = transcribe(client, "whisper-1", "recording.wav")
   if (res.ok) {
       print("transcript: {res.text}\n")
   } else {
       print("error [{res.error.kind}/{res.error.status}]: {res.error.message}\n")
   }

Quick Reference
===============

==========================================  ==============================================
Function                                    Description
==========================================  ==============================================
``speak(client, model, voice, text)``       TTS → audio bytes (``array<uint8>``)
``speech(client, req)``                      Full TTS request (``SpeechResult``)
``transcribe(client, model, path)``          STT: audio file → text
``translate(client, model, path)``           STT into English
==========================================  ==============================================

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/06_audio.das <../../../../tutorials/dasOPENAI/06_audio.das>`

   Next tutorial: :ref:`tutorial_dasOPENAI_streaming`
