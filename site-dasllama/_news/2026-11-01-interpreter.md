---
date: 2026-11-01
tag: examples
title: Do you speak Russian? Try our interpreter.
---

[The interpreter](/examples/interpreter/interpreter.html) is the fourth browser example: press listen and talk in
Russian - Говорите по-русски - and hear it in English. Silero VAD cuts each utterance at the pause, GigaAM transcribes
it, Mozilla's Firefox Translations student puts it into English and Kyutai's Pocket TTS says it, every stage on the engine
compiled to WebAssembly in your tab; what you say stays there. The speech model is the 154 MB Q4_K file served as its
own planes, beside a 32 MB translator and a 13 MB voice, with a 1 MB voice-activity model.
