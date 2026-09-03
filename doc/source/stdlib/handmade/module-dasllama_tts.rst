Text to speech in pure daslang: load a converted StyleTTS2-lineage GGUF (KittenTTS nano and
mini, Kokoro-82M), run text through the das-native front end (normalizer, part-of-speech tagger,
grapheme-to-phoneme), and synthesize mono f32 PCM per sentence chunk, timed per model stage.
Run with ``-jit``; ``utils/dasllama-server/txt2wav.das`` is the canonical program shape, and
the server's ``/v1/audio/speech`` route serves the same facade.

The front-end packs ``tts_g2p.bin`` and ``tts_postag.bin`` load from the GGUF's directory. A
model's voices and sample rate come from ``caps``; the rows GEMMs serve as Q8_0 by default, and
``set_tts_q8`` pins the lane for the loads that follow.

Hands-on: :ref:`the text to speech tutorial <tutorial_dasLLAMA_text_to_speech>`.
